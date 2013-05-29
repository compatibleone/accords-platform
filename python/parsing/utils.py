import requests
import json
import re

_request_root = "http://127.0.0.1"
publication_port = '8086'
_publication_loc = "publication"
_all_loc = '-'

def get_all():
    return _get(_all_loc)

def _get(category, id = '', filters = {}, root = _request_root + ':' + publication_port):
    headers = _headers_with_attributes(filters)
    url = root + '/' + category + '/' + id
    return requests.get(url, headers = headers)
   
def _find_attribute_in_response(response, regex):        
    match = re.search(regex, response)
    if len(match.groups()) > 0:
        return match.groups()[0]
    return None

_find_server_regex = u"X-OCCI-Attribute: occi.publication.why=\"(\S+)\"\n"
def _find_server_of_entry(id):
    r = _get(_publication_loc, id)
    return _find_attribute_in_response(r.text, _find_server_regex)

#TODO Rename
def _find_attribute2_in_response(response, regex):        
    match = re.search(regex, response)
    if len(match.groups()) > 1:
        return match.groups()[1]
    return None

_find_id_regex = u"X-OCCI-Location: (\S+)/(\S+)\n"
def _find_id_of_entry(response):
    return _find_attribute2_in_response(response, _find_id_regex)

def _find_all_ids_of_entries(response):        
    matches = re.finditer(_find_id_regex, response)
    return [match.groups()[1] for match in matches]

def find_root_of_category(category):
    r = _get(_publication_loc, filters = {'occi.publication.what':category})
    if r.status_code is not requests.codes.ok:
        return None
    ids = _find_all_ids_of_entries(r.text)
    servers = [_find_server_of_entry(id) for id in ids]
    if len(servers) is 0 or any(server != servers[0] for server in servers):
        return None
    return servers[0]
    
#TODO This doesn't work properly...since all values have the same key, only the last is actually used
# Could do separate gets for each request, and then manually union them!
def _headers_with_attributes(attributes):
    #comma_separated_attributes = ','.join(('{0}={1}'.format(name, value) for name, value in attributes.items()))
    #headers = {'X-OCCI-Attribute':comma_separated_attributes}
    headers = dict([['X-OCCI-Attribute', '{0}={1}'.format(name, value)] for name, value in attributes.items()])
    #headers = dict([['X-OCCI-Attribute: {0}'.format(name), '={0}'.format(value)] for name, value in attributes.items()])
    return headers

def get_marketplace_users():
    all = get_all() #TODO Is this required?  C parser does it
    
    # TODO Skip where since for now it seems that where is always marketplace 
    #attributes = {'occi.publication.where':'marketplace', 'occi.publication.what':'user'}
    attributes = {'occi.publication.what':'user'}
    r = _get(_publication_loc, filters = attributes)
    
    return _find_all_ids_of_entries(r.text) if r.status_code is requests.codes.ok else None
    
def find_id(category, entry):
    root = find_root_of_category(category)
    
    r = _get(category, filters = {'occi.{0}.name'.format(category):entry}, root = root)
    if r.status_code is not requests.codes.ok:
        return None
    return _find_id_of_entry(r.text)
    
    
    
    
    
    
    