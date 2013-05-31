import unittest
import os
import requests

from hamcrest import assert_that, is_, is_not, greater_than, has_item

import parsing.utils

class TestInterface(unittest.TestCase):
    '''Tests basic communication with the servers'''
    def setUp(self):
        self.dut = parsing.utils.Provisioner()

    def tearDown(self):
        self.dut.clean_all()
            
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
        
    def tearDown(self):
        self.dut.clean_all()
        
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
        
    def test_that_locate_returns_url_for_user_test_parser(self):
        url = self.dut.locate('user', 'test-parser')
        
        assert_that(url, is_not(None))
        
    def test_that_authorize_returns_authorization_location(self):
        authorization = self.dut.authorize('test-parser')
        
        assert_that(authorization, is_not(None))
        
class TestManifestConstruction(unittest.TestCase):
    def setUp(self):
        self.dut = parsing.utils.Provisioner()       
        self.dut.clean_all()   
        
    def tearDown(self):
        self.dut.clean_all()    
             
    def test_that_manifest_cn_any_does_not_exist_after_clean(self):  
        url = self.dut.locate('manifest', 'cn_any')
        
        assert_that(url, is_(None))
        
    def test_that_manifest_is_created_by_update_entry_manifest(self):
        self.dut.update_entry('manifest', 'cn_any')
        
        ident = self.dut.locate('manifest', 'cn_any')
        assert_that(ident, is_not(None))
        
    def test_that_update_entry_returns_id_when_entry_does_not_exist(self):
        ident = self.dut.update_entry('manifest', 'cn_any')
        
        assert_that(ident, is_not(None))        
        
    def test_that_update_entry_returns_id_when_entry_does_exist(self):
        self.dut.update_entry('manifest', 'cn_any')
        
        ident = self.dut.update_entry('manifest', 'cn_any')
        
        assert_that(ident, is_not(None))
        
    def test_that_update_entry_returns_same_id_as_original_when_entry_does_exist(self):
        ident = self.dut.update_entry('manifest', 'cn_any')
        
        new_ident = self.dut.update_entry('manifest', 'cn_any')
        
        assert_that(new_ident, is_(ident))
        
    def test_that_update_entry_updates_values_when_entry_does_not_exist(self):
        self.dut.update_entry('node', 'cn_any', {'type':'simple'})
        
        type_set = self.dut.read_attribute('node', 'cn_any', 'type')        
        assert_that(type_set, is_('simple'))
        
    def test_that_update_entry_updates_values_when_entry_does_exist(self):
        self.dut.update_entry('node', 'cn_any', {'type':'something'})
        self.dut.update_entry('node', 'cn_any', {'type':'something_else'})
        
        type_set = self.dut.read_attribute('node', 'cn_any', 'type')        
        assert_that(type_set, is_('something_else'))
    
    def test_that_read_attribute_returns_test_parser_for_name_on_user_test_parser(self):
        attr = self.dut.read_attribute('user', 'test-parser', 'name')
        
        assert_that(attr, is_('test-parser'))
        
    def test_that_update_entry_can_update_multiple_attributes_at_once(self):
        self.dut.update_entry('node', 'cn_any', {'type':'simple', 'access':'public'})
        
        type_set = self.dut.read_attribute('node', 'cn_any', 'type')        
        assert_that(type_set, is_('simple'))
        access_set = self.dut.read_attribute('node', 'cn_any', 'access')
        assert_that(access_set, is_('public'))
        
    def test_that_update_entry_can_update_attribute_with_colon_in_name(self):
        self.dut.update_entry('infrastructure', 'cn_any:any')
        
        name_set = self.dut.read_attribute('infrastructure', 'cn_any:any', 'name')        
        assert_that(name_set, is_('cn_any:any'))
        
    def test_that_make_links_creates_link_to_port_specified(self):
        network_id = self.dut.update_entry('network', 'compatibleone', {'label':'ethernet', 'vlan':'100M'})
        port_id = self.dut.update_entry('port', 'http', {'protocol':'tcp', 'from':'80', 'to':'80', 'direction':'inout', 'state':'0'})
        
        self.dut.make_links(network_id, [port_id])
        
        link_target = self.dut.read_link_targets(network_id)
        assert_that(link_target, has_item(port_id))
        
    def test_that_make_links_creates_links_to__multiple_ports_if_specified(self):
        network_id = self.dut.update_entry('network', 'compatibleone', {'label':'ethernet', 'vlan':'100M'})
        port_id = self.dut.update_entry('port', 'http', {'protocol':'tcp', 'from':'80', 'to':'80', 'direction':'inout', 'state':'0'})
        port_id2 = self.dut.update_entry('port', 'something_else', {'protocol':'tcp', 'from':'80', 'to':'80', 'direction':'inout', 'state':'0'})
        
        self.dut.make_links(network_id, [port_id, port_id2])
        
        link_targets = self.dut.read_link_targets(network_id)
        assert_that(link_targets, has_item(port_id))
        assert_that(link_targets, has_item(port_id2))
        
    def test_that_make_links_deletes_all_links_for_network_first(self):
        network_id = self.dut.update_entry('network', 'compatibleone', {'label':'ethernet', 'vlan':'100M'})
        port_id = self.dut.update_entry('port', 'http', {'protocol':'tcp', 'from':'80', 'to':'80', 'direction':'inout', 'state':'0'})
        self.dut.make_links(network_id, [port_id])
        
        self.dut.make_links(network_id, [])
        
        link_targets = self.dut.read_link_targets(network_id)
        assert_that(link_targets, is_([]))
        
    def test_that_update_all_succeeds(self):
        success = self.dut.update_all()
        
        assert_that(success, is_(True))
    #TODO Test for writing VM
        
    
    def test_that_make_link_returns_url_to_link(self):
        network_id = self.dut.update_entry('network', 'compatibleone', {'label':'ethernet', 'vlan':'100M'})
        port_id = self.dut.update_entry('port', 'http', {'protocol':'tcp', 'from':'80', 'to':'80', 'direction':'inout', 'state':'0'})
        
        url = self.dut.make_link(network_id, port_id)
        
        assert_that(url, is_not(None))
        
    def test_that_make_link_creates_link_with_specified_source_and_target(self):
        network_id = self.dut.update_entry('network', 'compatibleone', {'label':'ethernet', 'vlan':'100M'})
        port_id = self.dut.update_entry('port', 'http', {'protocol':'tcp', 'from':'80', 'to':'80', 'direction':'inout', 'state':'0'})
        
        self.dut.make_link(network_id, port_id)
        
        link_target = self.dut.read_link_targets(network_id)
        assert_that(link_target, has_item(port_id))
        
    def test_that_update_entry_raises_if_specified_attribute_value_is_none(self):
        self.assertRaises(ValueError, self.dut.update_entry, 'compute', 'random', {'architecture':None})
        
        
        
