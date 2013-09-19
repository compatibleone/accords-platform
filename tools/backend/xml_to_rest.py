import argparse
import string
import os

import re
import requests
import xml.etree.ElementTree as ET


######################## This code is taken from TestPublication.py ##########################
# It would be nice to separate it out into a common library 

def _headers_with_attribute(category, name, value):
    headers = {'X-OCCI-Attribute':('occi.{0}.{1}={2}'.format(category, name, value))}
    return headers

def _headers_with_attributes(category, names, values):
    hacked_value = 'occi.{0}.{1}={2}'.format(category, names[0], values[0])
    names = names[1:]
    values = values[1:]
    for name, value in zip(names,values):
        hacked_value = hacked_value + '\r\nX-OCCI-Attribute: occi.{0}.{1}={2}'.format(category, name, value)
    return {'X-OCCI-Attribute':hacked_value}  

def _headers(headers):
    if headers:
        headers['accept'] = 'text'
    else:
        headers = {'accept' : 'text'}
    return headers

def _post(url, params = None, headers = None):
    headers = _headers(headers)
    if params:
        return requests.post(url, params=params, headers=headers)
    else:
        return requests.post(url, None, headers=headers)

def _delete(url):
    headers = {'accept' : 'text'}
    return requests.delete(url, headers=headers)
        
def _category_root(category, port):
    return "http://co.market.onapp.com:{1}/{0}/".format(category, port)

_find_id_regex = u"X-OCCI-Location: (\S+)/(\S+)\n"
def _find_id_of_entry( response):        
    match = re.search(_find_id_regex, response)
    if not match:
        return None
    if len(match.groups()) > 1:
        return match.groups()[1]
    return None

########################## End Common Code ##################################################
    
parser = argparse.ArgumentParser(description='Read an xml (node) backend file, insert it into CO backend via REST API')
parser.add_argument('filename', help='file to read')
parser.add_argument('category', help='name of category in backend')
parser.add_argument('port', help='port of category')

args = parser.parse_args()

print "Deleting all in category: {0}".format(args.category)
root_url = _category_root(args.category, args.port)
_delete(root_url)

tree = ET.parse(args.filename)
root = tree.getroot()
print "Top level tag: {0}".format(root.tag)
for child in root:
    headers = _headers_with_attributes(args.category, *zip(*child.attrib.iteritems()))
    #print headers
    r = _post(root_url, None, headers = headers)
    ident = _find_id_of_entry(r.text)
    if not ident:
        raise IOError("Failed to create entry")
    print ident
    

    

