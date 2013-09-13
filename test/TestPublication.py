import unittest
import os
import subprocess
import requests
import json
import re
import time
import calendar

from hamcrest import assert_that, is_, is_not, greater_than, has_item

_co_start_script_path = "../../../../../small-accords" #TODO This path should be discovered in some way, not hardcoded
_request_root = "http://127.0.0.1:8086/publication/"

def _root_with_id(id):
    if not id:
        raise Exception("No id, did a previous lookup fail?")
    return _request_root + id

class TestPublication(unittest.TestCase):
    _find_id_regex = u"X-OCCI-Location: (\S+)/(\S+)\n"
    
    @classmethod
    def setUpClass(cls):
        '''
        Start CO - ideally this would be done for each individual test, but since it takes
        > 80 seconds to start / stop CO, that's not currently practical
        '''
        #os.chdir(_co_start_script_path)
        #subprocess.check_call("co-start")
        pass
    
    @classmethod
    def tearDownClass(cls):
        #subprocess.check_call("co-stop")
        pass
    
    def test_that_co_starts_and_stops(self):
        '''This should go in its own test library, it's convenient to have it here for now'''
        pass
    
    def test_that_get_all_returns_status_okay(self):
        r = self._get(_request_root)
        
        assert_that(r.status_code, is_(requests.codes.ok))

    def _find_id_of_entry(self, response):        
        match = re.search(self._find_id_regex, response)
        if not match:
            return None
        if len(match.groups()) > 1:
            return match.groups()[1]
        return None
    
    def test_that_post_creates_new_entry_and_returns_id(self):        
        r = self._post(_request_root)
        
        response = r.text
        id = self._find_id_of_entry(response) 
        
        assert_that(r.status_code, is_(requests.codes.ok))
        assert_that(id, is_not(None))
        
    def test_that_get_returns_not_found_if_id_invalid(self):
        r = self._get(_request_root + "arbitrary_id")
        
        assert_that(r.status_code, is_(requests.codes.not_found))
        
    def test_that_delete_returns_not_found_if_id_invalid(self):
        r = self._delete(_request_root + "arbitrary_id")
        
        assert_that(r.status_code, is_(requests.codes.not_found))
        
    def test_that_delete_returns_ok_if_id_is_valid(self):
        r = self._post(_request_root)
        id = self._find_id_of_entry(r.text)
        
        r = self._delete(_root_with_id(id)) 
        
        assert_that(r.status_code, is_(requests.codes.ok))
        
    def test_that_delete_deletes_entry(self):
        r = self._post(_request_root)
        id = self._find_id_of_entry(r.text)
        
        self._delete(_root_with_id(id)) 
        r = self._get(_root_with_id(id))
        
        assert_that(r.status_code, is_(requests.codes.not_found))
        
    def test_that_delete_all_deletes_all_entries(self):
        self._post(_request_root)
        
        self._delete(_request_root)
        
        # Riak takes 3 seconds between marking objects as deleted and actually deleting them
        import time
        time.sleep(4)
        
        r = self._get(_request_root)   
        responses = self._find_ids_of_all_entries(r.text)        
        assert_that(len(responses), is_(0))
        
    def test_that_delete_responds_ok(self):
        self._post(_request_root)
        
        r = self._delete(_request_root)
        
        assert_that(r.status_code, is_(requests.codes.ok))        
         
    def test_that_get_returns_ok_if_id_is_valid(self):
        r = self._post(_request_root)
        id = self._find_id_of_entry(r.text)
        
        r = self._get(_root_with_id(id)) 
        
        assert_that(r.status_code, is_(requests.codes.ok))        
        
    def _find_ids_of_all_entries(self, response):
        matches = re.finditer(self._find_id_regex, response)
        return [match.group(2) for match in matches]
    
    def test_that_get_all_returns_list_of_ids(self):
        # Ensure that at least two entries exist      
        self._post(_request_root)   
        self._post(_request_root)
        
        r = self._get(_request_root)
    
        responses = self._find_ids_of_all_entries(r.text)
        
        assert_that(len(responses), is_(greater_than(1)))
        
    def test_that_get_with_single_string_filter_returns_only_matching_ids(self):
        attr_name = 'operator'
        operator = u"Bob"        
        post_headers = self._headers_with_attribute(attr_name, operator)        
        r = self._post(_request_root, None, headers = post_headers)
        matching_id = self._find_id_of_entry(r.text)        
        post_headers = self._headers_with_attribute(attr_name, "something random")        
        r = self._post(_request_root, None, headers = post_headers)        
        non_matching_id = self._find_id_of_entry(r.text)
        
        r = self._get(_request_root, self._headers_with_attribute(attr_name, operator))
        
        responses = self._find_ids_of_all_entries(r.text)
        assert_that(responses, has_item(matching_id))
        assert_that(responses, is_not(has_item(non_matching_id)))
                
    
    def test_that_get_with_single_int_filter_returns_only_matching_ids(self):
        attr_name = 'pid'
        pid = 45        
        post_headers = self._headers_with_attribute(attr_name, pid)        
        r = self._post(_request_root, None, headers = post_headers)
        matching_id = self._find_id_of_entry(r.text)        
        post_headers = self._headers_with_attribute(attr_name, 86)        
        r = self._post(_request_root, None, headers = post_headers)        
        non_matching_id = self._find_id_of_entry(r.text)
        
        r = self._get(_request_root, self._headers_with_attribute(attr_name, pid))
        
        responses = self._find_ids_of_all_entries(r.text)
        assert_that(responses, has_item(matching_id))
        assert_that(responses, is_not(has_item(non_matching_id)))
        
                
    def test_that_get_with_two_filters_returns_only_matching_ids(self):
        attr_1_name = 'operator'
        operator = u"Bob"    
        attr_2_name = 'identity'
        identity = "Alice"    
        post_headers = self._headers_with_attributes([attr_1_name, attr_2_name], [operator, identity])        
        r = self._post(_request_root, None, headers = post_headers)
        matching_id = self._find_id_of_entry(r.text)        
        post_headers = self._headers_with_attributes([attr_1_name, attr_2_name], ["Someone else", identity])
        r = self._post(_request_root, None, headers = post_headers)        
        non_matching_id_1 = self._find_id_of_entry(r.text)    
        post_headers = self._headers_with_attributes([attr_1_name, attr_2_name], [operator, "Zorro"])
        r = self._post(_request_root, None, headers = post_headers)        
        non_matching_id_2 = self._find_id_of_entry(r.text)
        
        r = self._get(_request_root, self._headers_with_attributes([attr_1_name, attr_2_name], [operator, identity]))
        
        responses = self._find_ids_of_all_entries(r.text)
        assert_that(responses, has_item(matching_id))
        assert_that(responses, is_not(has_item(non_matching_id_1)))
        assert_that(responses, is_not(has_item(non_matching_id_2)))
        
    def _find_attribute(self, response, attribute):            
        find_attribute = u'X-OCCI-Attribute: occi.publication.{0}="(\S+)"\n'.format(attribute)
        matches = re.search(find_attribute, response)
        if matches and matches.groups():
            return matches.groups()[0]
        return None
    
    def test_that_put_on_invalid_id_returns_not_found(self):
        r = self._put(_request_root + "arbitrary_id_string")
        
        assert_that(r.status_code, is_(requests.codes.not_found))
        

    def _headers_with_attribute(self, name, value):
        headers = {'X-OCCI-Attribute':('occi.publication.{0}={1}'.format(name, value))}
        return headers
    
    def _headers_with_attributes(self, names, values):
        hacked_value = 'occi.publication.{0}={1}'.format(names[0], values[0])
        names = names[1:]
        values = values[1:]
        for name, value in zip(names,values):
            hacked_value = hacked_value + '\r\nX-OCCI-Attribute: occi.publication.{0}={1}'.format(name, value)
        return {'X-OCCI-Attribute':hacked_value}           

    def test_that_put_reports_updated_entry(self):
        id = self._create_entry()
        attr_name = 'operator'
        operator = u"Bob"        
        put_headers = self._headers_with_attribute(attr_name, operator)
        
        r = self._put(_root_with_id(id), headers = put_headers)
        response = r.text
        
        assert_that(r.status_code, is_(requests.codes.ok))
        assert_that(self._find_attribute(response, attr_name), is_(operator))
        
    def test_that_put_updates_entry_in_backend(self):
        id = self._create_entry()
        attr_name = 'operator'
        operator = u"Bob"        
        put_headers = self._headers_with_attribute(attr_name, operator)
        
        self._put(_root_with_id(id), headers = put_headers)
        r = self._get(_root_with_id(id))
        response = r.text        
        
        assert_that(self._find_attribute(response, attr_name), is_(operator))
        
    def test_that_post_creates_entry_with_specified_attributes(self):
        attr_name = 'operator'
        operator = u"Bob"        
        post_headers = self._headers_with_attribute(attr_name, operator)
        
        r = self._post(_request_root, None, headers = post_headers)
        id = self._find_id_of_entry(r.text)
        
        r = self._get(_root_with_id(id))
        response = r.text
        assert_that(self._find_attribute(response, attr_name), is_(operator))        
    
    def test_that_post_with_integer_attribute_creates_correctly(self):
        attr_name = 'pid'
        pid = 27        
        post_headers = self._headers_with_attribute(attr_name, pid)
        
        r = self._post(_request_root, None, headers = post_headers)
        id = self._find_id_of_entry(r.text)
        
        r = self._get(_root_with_id(id))
        response = r.text
        assert_that(int(self._find_attribute(response, attr_name)), is_(pid))
        
    def test_that_put_does_not_update_unmodified_items(self):
        attr_name = 'operator'
        operator = u"Bob"       
        post_headers = self._headers_with_attribute(attr_name, operator)
        put_headers = self._headers_with_attribute('identity', 'anything')        
        r = self._post(_request_root, None, headers = post_headers)
        id = self._find_id_of_entry(r.text)
                
        self._put(_root_with_id(id), headers = put_headers)
        
        r = self._get(_root_with_id(id))
        response = r.text        
        assert_that(self._find_attribute(response, attr_name), is_(operator))        
        
    def _create_entry(self):
        r = self._post(_request_root)
        retval = self._find_id_of_entry(r.text)
        if not retval:
            raise Exception("Failed to create entry")
        return retval

    def test_suspend_action_returns_okay_status(self):
        id = self._create_entry()        
        post_data = {'action':'suspend'}
        
        r = self._post(_root_with_id(id), params = post_data)
        
        assert_that(r.status_code, is_(requests.codes.ok))

    def test_restart_action_returns_okay_status(self):
        id = self._create_entry()        
        post_data = {'action':'restart'}
        
        r = self._post(_root_with_id(id), params = post_data)
        
        assert_that(r.status_code, is_(requests.codes.ok))
        
    def test_restart_action_resets_time_values(self):
        id = self._create_entry()        
        post_data = {'action':'restart'}        
        
        self._post(_root_with_id(id), params = post_data)        
        response = self._get(_root_with_id(id)).text
        
        assert_that(self._find_attribute(response, 'uptime'), is_("0"))
        time_now = calendar.timegm(time.gmtime())
        self.assertAlmostEqual(int(self._find_attribute(response, 'when')), time_now, delta = 2)
        assert_that(self._find_attribute(response, 'state'), is_("1"))
        
    def _headers(self, headers):
        if headers:
            headers['accept'] = 'text'
        else:
            headers = {'accept' : 'text'}
        return headers
        
    def _post(self, url, params = None, headers = None):
        headers = self._headers(headers)
        if params:
            return requests.post(url, params=params, headers=headers)
        else:
            return requests.post(url, None, headers=headers)
        
    def _get(self, url, headers = None):
        headers = self._headers(headers)
        return requests.get(url, headers=headers)
    
    def _delete(self, url):
        headers = {'accept' : 'text'}
        return requests.delete(url, headers=headers)
    
    def _put(self, url, headers = None):
        headers = self._headers(headers)
        return requests.put(url, None, headers=headers)
        
        

        
        
        
        
    
    
    
    
        
