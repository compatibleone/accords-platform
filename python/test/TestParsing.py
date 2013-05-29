import unittest
import os
import requests

from hamcrest import assert_that, is_, is_not, greater_than

import parsing.utils

class TestInterface(unittest.TestCase):
    '''Tests basic communication with the servers'''
    
    def test_that_get_all_on_publication_server_returns_status_okay(self):
        r = parsing.utils.get_all()
        
        assert_that(r.status_code, is_(requests.codes.ok))

class TestParsing(unittest.TestCase):
    '''
    Test parsing of manifest files - these tests assume the database is already set up in a sensible state.
    TODO - Remove dependencies on initial database state.
    '''
    
    def test_that_find_users_returns_list_of_users(self):
        users = parsing.utils.get_marketplace_users()
        
        assert_that(len(users), is_(greater_than(0)))
        
    @unittest.expectedFailure
    def test_that_find_marketplace_users_doesnt_find_users_if_where_is_not_marketplace(self):
        self.fail("Currently don't have a way to filter on multiple attributes")
    
    def test_that_find_root_returns_correct_uri_for_user_category(self):
        server = parsing.utils.find_root_of_category('user')
        
        assert_that(server, is_('http://127.0.0.1:8087'))
        
    def test_the_find_root_returns_none_for_nonsense_category(self):
        server = parsing.utils.find_root_of_category('random-rubbish')
        
        assert_that(server, is_(None))
        
    def test_that_find_id_returns_id_for_user_test_parser(self):
        id = parsing.utils.find_id('user', 'test-parser')
        
        assert_that(id, is_not(None))
        
        
    
#     def test_that_post_creates_new_entry_and_returns_id(self):        
#         r = requests.post(_request_root)
#         
#         response = r.text
#         id = self._find_id_of_entry(response) 
#         
#         assert_that(r.status_code, is_(requests.codes.ok))
#         assert_that(id, is_not(None))
#         
#     def test_that_get_returns_not_found_if_id_invalid(self):
#         r = requests.get(_request_root + "arbitrary_id")
#         
#         assert_that(r.status_code, is_(requests.codes.not_found))
#         
#     def test_that_delete_returns_not_found_if_id_invalid(self):
#         r = requests.delete(_request_root + "arbitrary_id")
#         
#         assert_that(r.status_code, is_(requests.codes.not_found))
#         
#     def test_that_delete_returns_ok_if_id_is_valid(self):
#         r = requests.post(_request_root)
#         id = self._find_id_of_entry(r.text)
#         
#         r = requests.delete(_request_root + id) 
#         
#         assert_that(r.status_code, is_(requests.codes.ok))
#         
#     def test_that_delete_deletes_entry(self):
#         r = requests.post(_request_root)
#         id = self._find_id_of_entry(r.text)
#         
#         requests.delete(_request_root + id) 
#         r = requests.get(_request_root + id)
#         
#         assert_that(r.status_code, is_(requests.codes.not_found))
#         
#     def test_that_delete_all_deletes_all_entries(self):
#         requests.post(_request_root)
#         
#         requests.delete(_request_root)
#         r = requests.get(_request_root)   
#         responses = self._find_ids_of_all_entries(r.text)
#         
#         assert_that(len(responses), is_(0))
#         
#     def test_that_delete_responds_ok(self):
#         requests.post(_request_root)
#         
#         r = requests.delete(_request_root)
#         
#         assert_that(r.status_code, is_(requests.codes.ok))
#         
#          
#     def test_that_get_returns_ok_if_id_is_valid(self):
#         r = requests.post(_request_root)
#         id = self._find_id_of_entry(r.text)
#         
#         r = requests.get(_request_root + id) 
#         
#         assert_that(r.status_code, is_(requests.codes.ok))        
#         
#     def _find_ids_of_all_entries(self, response):
#         matches = re.finditer(self._find_id_regex, response)
#         return [match.group(0) for match in matches]
#     
#     def test_that_get_all_returns_list_of_ids(self):
#         # Ensure that at least two entries exist      
#         requests.post(_request_root)   
#         requests.post(_request_root)
#         
#         r = requests.get(_request_root)
#     
#         responses = self._find_ids_of_all_entries(r.text)
#         
#         assert_that(len(responses), is_(greater_than(1)))
# 
# 
#         
#     def _find_attribute(self, response, attribute):            
#         find_attribute = u'X-OCCI-Attribute: occi.publication.{0}="(\S+)"\n'.format(attribute)
#         matches = re.search(find_attribute, response)
#         if matches and matches.groups():
#             return matches.groups()[0]
#         return None
#     
#     def test_that_put_on_invalid_id_returns_not_found(self):
#         r = requests.put(_request_root + "arbitrary_id_string")
#         
#         assert_that(r.status_code, is_(requests.codes.not_found))
#         
# 

# 
#     def test_that_put_reports_updated_entry(self):
#         id = self._create_entry()
#         attr_name = 'operator'
#         operator = u"Bob"        
#         put_headers = self._headers_with_attribute(attr_name, operator)
#         
#         r = requests.put(_request_root + id, None, headers = put_headers)
#         response = r.text
#         
#         assert_that(r.status_code, is_(requests.codes.ok))
#         assert_that(self._find_attribute(response, attr_name), is_(operator))
#         
#     def test_that_put_updates_entry_in_backend(self):
#         id = self._create_entry()
#         attr_name = 'operator'
#         operator = u"Bob"        
#         put_headers = self._headers_with_attribute(attr_name, operator)
#         
#         requests.put(_request_root + id, None, headers = put_headers)
#         r = requests.get(_request_root + id)
#         response = r.text        
#         
#         assert_that(self._find_attribute(response, attr_name), is_(operator))
#         
#     def test_that_post_creates_entry_with_specified_attributes(self):
#         attr_name = 'operator'
#         operator = u"Bob"        
#         post_headers = self._headers_with_attribute(attr_name, operator)
#         
#         r = requests.post(_request_root, None, headers = post_headers)
#         id = self._find_id_of_entry(r.text)
#         
#         r = requests.get(_request_root + id)
#         response = r.text
#         assert_that(self._find_attribute(response, attr_name), is_(operator))
#         
#     def test_that_put_does_not_update_unmodified_items(self):
#         attr_name = 'operator'
#         operator = u"Bob"       
#         post_headers = self._headers_with_attribute(attr_name, operator)
#         put_headers = self._headers_with_attribute('identity', 'anything')        
#         r = requests.post(_request_root, None, headers = post_headers)
#         id = self._find_id_of_entry(r.text)
#                 
#         requests.put(_request_root + id, None, headers = put_headers)
#         
#         r = requests.get(_request_root + id)
#         response = r.text        
#         assert_that(self._find_attribute(response, attr_name), is_(operator))        
#         
#     def _create_entry(self):
#         r = requests.post(_request_root)
#         return self._find_id_of_entry(r.text)
# 
#     def test_suspend_action_returns_okay_status(self):
#         id = self._create_entry()        
#         post_data = {'action':'suspend'}
#         
#         r = requests.post(_request_root + id, params = post_data)
#         
#         assert_that(r.status_code, is_(requests.codes.ok))
# 
#     def test_restart_action_returns_okay_status(self):
#         id = self._create_entry()        
#         post_data = {'action':'restart'}
#         
#         r = requests.post(_request_root + id, params = post_data)
#         
#         assert_that(r.status_code, is_(requests.codes.ok))
#         
#     def test_restart_action_resets_time_values(self):
#         id = self._create_entry()        
#         post_data = {'action':'restart'}        
#         
#         requests.post(_request_root + id, params = post_data)        
#         response = requests.get(_request_root + id).text
#         
#         assert_that(self._find_attribute(response, 'uptime'), is_("0"))
#         time_now = calendar.timegm(time.gmtime())
#         self.assertAlmostEqual(int(self._find_attribute(response, 'when')), time_now, delta = 2)
#         assert_that(self._find_attribute(response, 'state'), is_("1"))
#         
#         
# 
#         
#         
#         
#         
#     
#     
#     
#     
#         
