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

def _location(root, category, ident = ''):
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
    
class Provisioner(object):
    user_name = 'test-parser'
    
    def __init__(self):
        self.auth_string = None
    
    def _authorize(self):
        self.auth_string = self.authorize(self.user_name)        
    
    def lookup_all(self, node_name = '"cn_any"'):  
        self._authorize()      
        node = self.find_id('node', node_name)
        if node is None:
            raise LookupError('Unable to find node "{0}"'.format(node_name))
        return False
        
    def get_all(self):
        return self._get(_all_loc)

    def _find_server_of_category(self, ident):
        r = self._get(_publication_loc, ident)
        return _find_attribute_in_response(r.text, _find_server_regex)

    def _request_args(self, category, root, ident, filters):
        headers = _headers_with_attributes(filters, self.auth_string)
        url = _location(root, category, ident)
        return url, headers
    
    def _get(self, category, ident = '', filters = {}, root = _request_root + ':' + publication_port):
        url, headers = self._request_args(category, root, ident, filters)
        return requests.get(url, headers = headers)

    def _post(self, category, ident = '', filters = {}, root = _request_root + ':' + publication_port):
        url, headers = self._request_args(category, root, ident, filters)
        return requests.post(url, headers = headers)
    
    def _put(self, category, ident = '', filters = {}, root = _request_root + ':' + publication_port):
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
    

    def _filters(self, category, entry):
        return {'occi.{0}.name'.format(category):entry}
    
    def _request_entry(self, category, entry):
        root = self.find_root_of_category(category)        
        r = self._get(category, filters = self._filters(category, entry), root = root)
        ident = None if r.status_code is not requests.codes.ok else _find_id_of_entry(r.text)
        return ident, root
    

    def _update_attributes(self, attributes, category, entry):
        filters = dict(['occi.{0}.{1}'.format(category, name), value] for name, value in attributes.items())
        filters.update(self._filters(category, entry))
        return filters

    def _post_entry(self, category, entry, attributes = {}):
        root = self.find_root_of_category(category)
        filters = self._update_attributes(attributes, category, entry)
        r = self._post(category, filters = filters, root = root)
        ident = None if r.status_code is not requests.codes.ok else _find_id_of_entry(r.text)
        return ident, root
      
    def _put_entry(self, category, ident, entry, attributes = {}):
        root = self.find_root_of_category(category)
        filters = self._update_attributes(attributes, category, entry)
        r = self._put(category, ident, filters = filters, root = root)
        return ident, root
      
    def find_id(self, category, entry):    
        ident, _ = self._request_entry(category, entry)
        return ident
    
    def _post_id(self, category, entry, attributes = {}):
        ident, _ = self._post_entry(category, entry, attributes)
        return ident     
    
    def _put_id(self, category, ident, entry, attributes = {}):
        ident, _ = self._put_entry(category, ident, entry, attributes)
        return ident 

    def _location_of_entry(self, category, entry):
        ident, root = self._request_entry(category, entry)
        if ident is None:
            return None
        location = _location(root, category, ident)
        return location

    def authorize(self, user):
        auth_location = self._location_of_entry('user', user)
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
    

    def _clean(self, category, name):
        location = self._location_of_entry(category, name)
        if location is not None:
            r = requests.delete(location)

    def clean_all(self):
        clean_pairs = [['manifest', 'cn_any'], ['node', 'cn_any']]
        [self._clean(category, name) for category, name in clean_pairs]
            
    def make_manifest(self):
        return self._post_id('manifest', 'cn_any')
    
    def update_entry(self, category, entry, attributes = {}):
        ident = self.find_id(category, entry)
        if ident is None:
            ident = self._post_id(category, entry, attributes)
        else:
            ident = self._put_id(category, ident, entry, attributes)
        return(ident)
        
    def read_attribute(self, category, entry, attr_name):
        ident, root = self._request_entry(category, entry)
        r = self._get(category, root = root, ident = ident)
        if r.status_code is not requests.codes.ok:
            raise LookupError('Could not find entry {0}/{1}'.format(category, entry))
        return _find_attribute_in_response(r.text, _attr_regex(category, attr_name))
