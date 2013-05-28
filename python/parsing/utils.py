import requests
import json
import re

_request_root = "http://127.0.0.1"
publication_port = '8086'
_publication_loc = "/publication/"
_all_loc = '-/'

def _get(port, location):
    return requests.get(_request_root + ':' + port + '/' + location)

def get_all(port):
    return _get(port, _all_loc)

_find_id_regex = u"X-OCCI-Location: (\S+)/(\S+)\n"
def _find_id_of_entry(response):        
    match = re.search(_find_id_regex, response)
    if len(match.groups()) > 1:
        return match.groups()[1]
    return None
    