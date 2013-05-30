import requests
import json
import re

_request_root = "http://127.0.0.1"
publication_port = '8086'
_publication_loc = "publication"
_all_loc = '-'

_find_server_regex = u"X-OCCI-Attribute: occi.publication.why=\"(\S+)\"\n"

def _attr_regex(category, attr):
    return u"X-OCCI-Attribute: occi.{0}.{1}=\"(\S+)\"\n".format(category, attr)

def _root(port = publication_port):
    return _request_root + ':' + port

def _location(root, category, ident = ''):
    if ident is None:
        return None
    return root + '/' + category + '/' + ident

def _get_match(match, count):
    if match is not None and len(match.groups()) > count:
        return match.groups()[count]
    return None
   
def _find_attribute_in_response(response, regex):        
    match = re.search(regex, response)
    return _get_match(match, 0)

def _find_location_id_in_response(response, regex):        
    match = re.search(regex, response)
    return _get_match(match, 1)

_find_id_regex = u"X-OCCI-Location: (\S+)/(\S+)\n"
def _find_id_of_entry(response):
    return _find_location_id_in_response(response, _find_id_regex)

def _find_all_ids_of_entries(response):        
    matches = re.finditer(_find_id_regex, response)
    return [match.groups()[1] for match in matches]
    
def _headers_with_attributes(attributes, authorization = None):
    attributes_entries = []
    attr_list = attributes.items()
    # This is a horrible hack.  The requests interface doesn't allow sending the same attribute multiple times in the headers.
    # However it doesn't escape the passed in values, so we can manually insert extra headers by prepending them with \r\n, and
    # passing them in as part of the original header.  Urgh!
    if len(attr_list) > 0:
        for name, value in attr_list:
            if value is None:
                raise ValueError("Illegal None value for attribute '{0}'".format(name))
        name, value = attr_list[0]
        first_value = '{0}="{1}"'.format(name, value)
        values = ['\r\nX-OCCI-ATTRIBUTE: {0}="{1}"'.format(name, value) for name, value in attr_list[1:]]
        values.insert(0, first_value)
        values_string = ''.join(values)
        attributes_entries = [['X-OCCI-ATTRIBUTE', values_string]]
    authorization_header = []
    if authorization is not None:
        attributes_entries.append(['X-OCCI-AUTHORIZE', authorization])
    headers = dict(attributes_entries)
    #headers = dict([['X-OCCI-Attribute: {0}'.format(name), '={0}'.format(value)] for name, value in attributes.items()])
    return headers
    
def _filter_by_source(source):
    return _filter_by_key('link', 'source', source)

def _filter_by_key(category, key, target):
    return {'occi.{0}.{1}'.format(category, key):target}
    
class Provisioner(object):    
    def __init__(self):
        self.auth_string = None
    
    def _authorize(self, user_name = 'test-parser'):
        self.auth_string = self.authorize(user_name)        
            
    def get_all(self):
        return self._get(_all_loc)

    def _find_server_of_category(self, ident):
        r = self._get(_publication_loc, ident)
        return _find_attribute_in_response(r.text, _find_server_regex)
    
    def _get(self, category, ident = '', attributes = {}, root = _root()):
        headers = _headers_with_attributes(attributes, self.auth_string)
        url = _location(root, category, ident)
        return requests.get(url, headers = headers)

    def _post(self, url, attributes = {}):
        headers = _headers_with_attributes(attributes, self.auth_string)
        return requests.post(url, headers = headers)
    
    def _put(self, url, attributes = {}):
        headers = _headers_with_attributes(attributes, self.auth_string)
        return requests.put(url, headers = headers)
    
    def find_root_of_category(self, category):
        r = self._get(_publication_loc, attributes = {'occi.publication.what':category})
        if r.status_code is not requests.codes.ok:
            raise LookupError("Couldn't find server for category '{0}'".format(category))
        ids = _find_all_ids_of_entries(r.text)
        servers = [self._find_server_of_category(ident) for ident in ids]
        if len(servers) is 0 or any(server != servers[0] for server in servers):
            raise LookupError("Conflicting servers found for category '{0}'".format(category))
        return servers[0]
       
    def _extract_ids_from_response(self, response):
        idents = None if response.status_code is not requests.codes.ok else _find_all_ids_of_entries(response.text)
        return idents 

    def _request_single_entry_id(self, category, entry):
        idents, root = self._request_entry_ids(category, entry)
        if len(idents) is 0:
            return None, root
        if len(idents) is 1:
            return idents[0], root
        raise LookupError("Too many ids found for entry '{0}'".format(entry))
         
    def _request_entry_ids(self, category, entry):
        attributes, root = self._make_request_args(category, entry)     
        r = self._get(category, attributes = attributes, root = root)
        return self._extract_ids_from_response(r), root   
    
    def _request_link_ids(self, source):
        root = _root()        
        r = self._get('link', attributes = _filter_by_source(source), root = root)
        return self._extract_ids_from_response(r), root           
    
    def _request_link_targets(self, source):
        idents, root = self._request_link_ids(source)
        links = [self._read_attribute_from_location(root, 'link', ident, 'target') for ident in idents]
        return links

    def _add_filter(self, attributes, category, entry, key):
        filters = dict(['occi.{0}.{1}'.format(category, name), value] for name, value in attributes.items())
        filters.update(_filter_by_key(category, key, entry))
        return filters

    def _make_request_args(self, category, entry, attributes = {}, key = 'name'):
        root = self.find_root_of_category(category)
        # TODO Move the filter higher up the stack
        attributes = self._add_filter(attributes, category, entry, key)
        return attributes, root

    def _post_common(self, category, attributes, root, action = ''):
        url = _location(root, category) + action
        r = self._post(url, attributes = attributes)
        return None if r.status_code is not requests.codes.ok else _location(root, category,  _find_id_of_entry(r.text))
    
    def _post_link(self, source, target):
        root = _root()
        attributes = _filter_by_source(source)
        attributes.update({'occi.link.target':target})
        return self._post_common('link', attributes, root)  

    #TODO Merge with vanilla post
    def _post_entry(self, category, entry, attributes = {}):
        attributes, root = self._make_request_args(category, entry, attributes)
        return self._post_common(category, attributes, root)
      
    def _put_entry(self, category, ident, entry, attributes = {}):
        attributes, root = self._make_request_args(category, entry, attributes)
        r = self._put(_location(root, category, ident), attributes = attributes)
        return ident, root
      
    def locate(self, category, entry):    
        ident, root = self._request_single_entry_id(category, entry)
        return _location(root, category, ident)
    
    # TODO Post and Put etc. should take full urls as parameters, not build them up themselves, unless necessary
    def _post_id(self, category, entry, attributes = {}):
        return self._post_entry(category, entry, attributes)  
        
    def _put_id(self, category, ident, entry, attributes = {}):
        ident, root = self._put_entry(category, ident, entry, attributes)
        return _location(root, category, ident) 

    def authorize(self, user):
        auth_location = self.locate('user', user)
        if auth_location is None:
            raise LookupError("Unable to find entry for user '{0}'".format(entry))
        authentication_server = self.find_root_of_category('authorization')
        if authentication_server is None:
            raise LookupError("Authentication server not found")
        post_url = _location(authentication_server, 'authorization')
        post_headers = _headers_with_attributes({'occi.authorization.user':auth_location})
        r = requests.post(post_url, headers = post_headers)
        if r.status_code is not requests.codes.ok:
            return None
        return post_url + _find_id_of_entry(r.text)
    
    def get_marketplace_users(self):
        all = self.get_all() #TODO Is this required?  C parser does it
        
        attributes = {'occi.publication.where':'marketplace', 'occi.publication.what':'user'}
        r = self._get(_publication_loc, attributes = attributes)
        
        return _find_all_ids_of_entries(r.text) if r.status_code is requests.codes.ok else None
    
    def _delete(self, location):
        if location is not None:
            r = requests.delete(location)

    def _clean(self, category, name):
        self._delete(self.locate(category, name))
            
    def _clean_links(self, source):
        idents, root = self._request_link_ids(source)
        if idents is not None:
            for ident in idents:
                self._delete(_location(root, 'link', ident))        

    def clean_all(self):
        clean_pairs = [['manifest', 'cn_any'], ['node', 'cn_any']]
        [self._clean(category, name) for category, name in clean_pairs]
                
    def update_entry(self, category, entry, attributes = {}):
        ident, _ = self._request_single_entry_id(category, entry)
        if ident is None:
            ident = self._post_id(category, entry, attributes)
        else:
            ident = self._put_id(category, ident, entry, attributes)
        return(ident)

    def _read_attribute_from_location(self, root, category, ident, attr_name):
        r = self._get(category, root = root, ident = ident)
        if r.status_code is not requests.codes.ok:
            raise LookupError('Could not find entry {0}/{1}'.format(category, ident))
        return _find_attribute_in_response(r.text, _attr_regex(category, attr_name))
            
    def read_attribute(self, category, entry, attr_name):
        ident, root = self._request_single_entry_id(category, entry)
        return self._read_attribute_from_location(root, category, ident, attr_name)

    

    def _schedule_build_operation(self, vm_url):
        return self.post('schedule', {'operation':'{0}?action=build'.format(vm_url)})


    def _update_node(self, infrastructure_url, image_url, firewall_value):
        node_url = self.update_entry('node', 'cn_any', {'type':'simple', 'access':'public', 'scope':'normal', 'provider':'onapp', 'state':'0', 'infrastructure':infrastructure_url, 'image':image_url, 'operator':"any", 'category':'manifest', 
                'entry':'none', 'firewall':firewall_value})
        return node_url


    def _update_manifest(self, configuration_url, release_url, interface_url, account_url, security_url, plan_value):
        return self.update_entry('manifest', 'cn_any', {'state':'0', 'configuration':configuration_url, 'release':release_url, 'interface':interface_url, 'account':account_url, 'security':security_url, 'nodes':'1', 
                'plan':plan_value})

    def update_all(self):
        self._authorize()
        manifest_url = self.update_entry('manifest', 'cn_any')
        self.update_entry('node', 'cn_any', {'type':'simple', 'access':'public', 'scope':'normal', 'provider':'onapp'})
        self.update_entry('infrastructure', 'cn_any:any')
        compute_url = self.update_entry('compute', 'cn_any:small', {'architecture':'x86_64', 'cores':'1', 'memory':'1G', 'speed':'1G'})
        storage_url = self.update_entry('storage', 'cn_any:small', {'size':'10G'})
        network_url = self.update_entry('network', 'compatibleone', {'label':'ethernet', 'vlan':'100M'})
        port_url = [ \
                    self.update_entry('port', 'http', {'protocol':'tcp', 'from':'80', 'to':'80', 'direction':'inout', 'state':'0'}),
                    self.update_entry('port', 'ssh', {'protocol':'tcp', 'from':'22', 'to':'22', 'direction':'inout', 'state':'0', 'range':'0.0.0.0/0'}), \
                    self.update_entry('port', 'cosacs', {'protocol':'tcp', 'from':'8286', 'to':'8286', 'direction':'inout', 'state':'0', 'range':'0.0.0.0/0'}) \
                    ]
        self.make_links(network_url, port_url)
        infrastructure_url = self.update_entry('infrastructure', 'cn_any:any', {'compute':compute_url, 'storage':storage_url, 'network':network_url, 'state':'0'})
        self.update_entry('image', 'cn_any:ubuntu', {'agent':'none'})
        system_url = self.update_entry('system', 'ubuntu', {'state':'0'})
        image_url = self.update_entry('image', 'cn_any:ubuntu', {'agent':'none', 'state':'0', 'packages':'0', 'system':system_url, 'vm':'cordscript: vm.new(id.value,provider.value); vm.build();'})
        vm_url = self._make_vm(image_url, 'onapp')
        self._schedule_build_operation(vm_url)
        self.update_entry('image', 'cn_any:ubuntu', {'agent':'none', 'state':'0', 'packages':'0', 'system':system_url, 'vm':vm_url})
        node_url = self._update_node(infrastructure_url, image_url, 'cordscript: firewall.new(id.value); firewall.build();')
        firewall_url = self.post('firewall', {'node':node_url})
        self._schedule_build_operation(firewall_url)
        node_url = self._update_node(infrastructure_url, image_url, firewall_url)
        configuration_url = self.post_put('configuration')       
        release_url = self.post_put('release')
        interface_url = self.post_put('interface')
        account_url = self.update_entry('account', 'jc', {'state':'0', 'users':'1'})
        security_url = self.post_put('security')
        self.make_links(manifest_url, [node_url])
        self._update_manifest(configuration_url, release_url, interface_url, account_url, security_url, 'cordscript: plan.new(id.value,name.value);')
        plan_url = self.update_entry('plan', 'cn_any', {'manifest':manifest_url})
        self._update_manifest(configuration_url, release_url, interface_url, account_url, security_url, plan_url)
        self._delete(self.auth_string)
        
        #self.post('plan', action = '{0}?action=instance'.format(plan_url))
        return True
        
        
    def make_links(self, source_url, target_urls):
        self._clean_links(source_url)
        for target in target_urls:
            self.make_link(source_url, target)
    
    def read_link_targets(self, source):
        return self._request_link_targets(source)
    
    def make_link(self, source, target):
        #TODO Make _post_link return full url
        return self._post_link(source, target) 
        
    def _make_vm(self, image_url, provider):
        attributes = {'image':image_url, 'provider':provider}
        return self.post('vm', attributes)
    
    #TODO This is easily testible - do so!
    def post(self, category, attributes = {}, action = ''):        
        root = self.find_root_of_category(category)
        return self._post_common(category, attributes, root, action)
    
    def post_put(self, category):
        #Don't know why this is done as a post then a put, but that's what the c parser does! 
        location = self.post(category)
        self._put(location)
        return location
    
    def broker(self):
        plan_url = self.locate('plan', 'cn_any')
        self._authorize('test-broker')        
        self.post('plan', action = '{0}?action=instance'.format(plan_url))
        self._delete(self.auth_string)