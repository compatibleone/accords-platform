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
        name, value = attr_list[0]
        first_value = '{0}={1}'.format(name, value)
        values = ['\r\nX-OCCI-ATTRIBUTE: {0}={1}'.format(name, value) for name, value in attr_list[1:]]
        values.insert(0, first_value)
        values_string = ''.join(values)
        attributes_entries = [['X-OCCI-ATTRIBUTE', values_string]]
    authorization_header = []
    if authorization is not None:
        attributes_entries.append(['X-OCCI-AUTHORIZE', authorization])
    headers = dict(attributes_entries)
    #headers = dict([['X-OCCI-Attribute: {0}'.format(name), '={0}'.format(value)] for name, value in attributes.items()])
    return headers
    

def _filter_by_name(category, entry):
    return {'occi.{0}.name'.format(category):entry}

def _filter_by_source(source):
    return {'occi.link.source':source}
    
class Provisioner(object):
    user_name = 'test-parser'
    
    def __init__(self):
        self.auth_string = None
    
    def _authorize(self):
        self.auth_string = self.authorize(self.user_name)        
            
    def get_all(self):
        return self._get(_all_loc)

    def _find_server_of_category(self, ident):
        r = self._get(_publication_loc, ident)
        return _find_attribute_in_response(r.text, _find_server_regex)

    def _request_args(self, category, root, ident, filters):
        headers = _headers_with_attributes(filters, self.auth_string)
        url = _location(root, category, ident)
        return url, headers
    
    def _get(self, category, ident = '', filters = {}, root = _root()):
        url, headers = self._request_args(category, root, ident, filters)
        return requests.get(url, headers = headers)

    def _post(self, category, ident = '', filters = {}, root = _root()):
        url, headers = self._request_args(category, root, ident, filters)
        return requests.post(url, headers = headers)
    
    def _put(self, category, ident = '', filters = {}, root = _root()):
        url, headers = self._request_args(category, root, ident, filters)
        return requests.put(url, headers = headers)
    
    def find_root_of_category(self, category):
        r = self._get(_publication_loc, filters = {'occi.publication.what':category})
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
        root = self.find_root_of_category(category)        
        r = self._get(category, filters = _filter_by_name(category, entry), root = root)
        return self._extract_ids_from_response(r), root   
    
    def _request_link_ids(self, source):
        root = _root()        
        r = self._get('link', filters = _filter_by_source(source), root = root)
        return self._extract_ids_from_response(r), root           
    
    def _request_link_targets(self, source):
        idents, root = self._request_link_ids(source)
        links = [self._read_attribute_from_location(root, 'link', ident, 'target') for ident in idents]
        return links

    def _update_attributes(self, attributes, category, entry):
        filters = dict(['occi.{0}.{1}'.format(category, name), value] for name, value in attributes.items())
        filters.update(_filter_by_name(category, entry))
        return filters

    def _make_update_args(self, category, entry, attributes):
        root = self.find_root_of_category(category)
        filters = self._update_attributes(attributes, category, entry)
        return filters, root

    def _post_common(self, category, filters, root):
        r = self._post(category, filters = filters, root = root)
        ident = None if r.status_code is not requests.codes.ok else _find_id_of_entry(r.text)
        return ident, root
    
    def _post_link(self, source, target):
        root = _root()
        filters = _filter_by_source(source)
        filters.update({'occi.link.target':target})
        return self._post_common('link', filters, root)        

    def _post_entry(self, category, entry, attributes = {}):
        filters, root = self._make_update_args(category, entry, attributes)
        return self._post_common(category, filters, root)
      
    def _put_entry(self, category, ident, entry, attributes = {}):
        filters, root = self._make_update_args(category, entry, attributes)
        r = self._put(category, ident, filters = filters, root = root)
        return ident, root
      
    def find_id(self, category, entry):    
        ident, root = self._request_single_entry_id(category, entry)
        return _location(root, category, ident)
    
    # TODO Post and Put etc. should take full urls as parameters, not build them up themselves, unless necessary
    def _post_id(self, category, entry, attributes = {}):
        ident, root = self._post_entry(category, entry, attributes)
        return _location(root, category, ident)     
    
    def _put_id(self, category, ident, entry, attributes = {}):
        ident, root = self._put_entry(category, ident, entry, attributes)
        return _location(root, category, ident) 

    def authorize(self, user):
        auth_location = self.find_id('user', user)
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
        r = self._get(_publication_loc, filters = attributes)
        
        return _find_all_ids_of_entries(r.text) if r.status_code is requests.codes.ok else None
    
    def _delete(self, location):
        if location is not None:
            r = requests.delete(location)

    def _clean(self, category, name):
        self._delete(self.find_id(category, name))
            
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

    
    def update_all(self):
        self.update_entry('manifest', 'cn_any')
        self.update_entry('node', 'cn_any', {'type':'simple', 'access':'public', 'scope':'normal', 'provider':'onapp'})
        self.update_entry('infrastructure', 'cn_any:any')
        self.update_entry('compute', 'cn_any:small', {'architecture':'x86_64', 'cores':'1', 'memory':'1G', 'speed':'1G'})
        self.update_entry('storage', 'cn_any:small', {'size':'10G'})
        network_id = self.update_entry('network', 'compatibleone', {'label':'ethernet', 'vlan':'100M'})
        port_id = self.update_entry('port', 'http', {'protocol':'tcp', 'from':'80', 'to':'80', 'direction':'inout', 'state':'0'})
        self.update_entry('port', 'ssh', {'protocol':'tcp', 'from':'22', 'to':'22', 'direction':'inout', 'state':'0', 'range':'0.0.0.0/0'})
        self.update_entry('port', 'cosacs', {'protocol':'tcp', 'from':'8286', 'to':'8286', 'direction':'inout', 'state':'0', 'range':'0.0.0.0/0'})
        
    def make_links_for_network(self, network_id, port_ids):
        self._clean_links(network_id)
        for port in port_ids:
            self.make_link(network_id, port)
    
    def read_link_targets(self, source):
        return self._request_link_targets(source)
    
    def make_link(self, source, target):
        #TODO Make _post_link return full url
        ident, root = self._post_link(source, target)
        return _location(root, 'link', ident)
        