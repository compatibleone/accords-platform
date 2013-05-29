import unittest
import os
import requests

from hamcrest import assert_that, is_, is_not, greater_than

import parsing.utils

class TestInterface(unittest.TestCase):
    '''Tests basic communication with the servers'''
    def setUp(self):
        self.dut = parsing.utils.Provisioner()
    
    def test_that_get_all_on_publication_server_returns_status_okay(self):
        r = self.dut.get_all()
        
        assert_that(r.status_code, is_(requests.codes.ok))

class TestBasicLookupFunctions(unittest.TestCase):
    '''
    Test parsing of manifest files - these tests assume the database is already set up in a sensible state.
    TODO - Remove dependencies on initial database state.
    '''
    
    def setUp(self):
        self.dut = parsing.utils.Provisioner()
        
    def test_that_find_users_returns_list_of_users(self):
        users = self.dut.get_marketplace_users()
        
        assert_that(len(users), is_(greater_than(0)))
        
    @unittest.expectedFailure
    def test_that_find_marketplace_users_doesnt_find_users_if_where_is_not_marketplace(self):
        self.fail("Currently don't have a way to filter on multiple attributes")
    
    def test_that_find_root_returns_correct_uri_for_user_category(self):
        server = self.dut.find_root_of_category('user')
        
        assert_that(server, is_('http://127.0.0.1:8087'))
        
    def test_the_find_root_raises_for_nonsense_category(self):        
        self.assertRaises(LookupError, self.dut.find_root_of_category, 'random-rubbish')
        
    def test_that_find_id_returns_id_for_user_test_parser(self):
        id = self.dut.find_id('user', 'test-parser')
        
        assert_that(id, is_not(None))
        
    def test_that_authorize_returns_authorization_location(self):
        authorization = self.dut.authorize('test-parser')
        
        assert_that(authorization, is_not(None))
        
class TestManifestConstruction(unittest.TestCase):
    def setUp(self):
        self.dut = parsing.utils.Provisioner()       
        self.dut.clean()       
     
    #TODO 
    @unittest.skip 
    def test_that_lookup_all_resources_returns_ok(self):
        assert_that(self.dut.lookup_all(), is_(True))
        
    def test_that_manifest_cn_any_does_not_exist_after_clean(self):  
        id = self.dut.find_id('manifest', 'cn_any')
        
        assert_that(id, is_(None))
        
    def test_that_manifest_is_created_by_make_manifest(self):
        self.dut.make_manifest()
        
        id = self.dut.find_id('manifest', 'cn_any')
        assert_that(id, is_not(None))
        
    def test_that_update_entry_returns_id_when_entry_does_not_exist(self):
        id = self.dut.update_entry('manifest', 'cn_any')
        
        assert_that(id, is_not(None))        
        
    def test_that_update_entry_returns_id_when_entry_does_exist(self):
        self.dut.make_manifest()
        
        id = self.dut.update_entry('manifest', 'cn_any')
        
        assert_that(id, is_not(None))
        
        
