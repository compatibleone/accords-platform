import unittest
from testfixtures import LogCapture

import xml.etree.ElementTree as ET
from hamcrest import assert_that, is_, is_not, has_key, has_entry

from ConfigParser import BackendParser
from Backend import Backend

def _create_valid_backend():    
    root = ET.Element('config')
    backends = ET.SubElement(root, 'backends')
    backend = ET.SubElement(backends, 'backend', {'name':'db', 'plugin':'mysql'})
    return (root, backend)

class TestBackendParsing(unittest.TestCase):
    
    def setUp(self):
        self.l = LogCapture()
        
    def tearDown(self):
        self.l.uninstall()
    
    def test_that_parse_returns_a_backend_dictionary_if_xml_contains_config_element_including_backends_list(self):
        root = ET.Element('config')
        ET.SubElement(root, 'backends')
        op = BackendParser()
        
        op.parse(root)
        result = op.backends
        
        assert_that(result, is_not(None))
        
    def test_that_config_includes_a_backends_list_if_config_contains_backends_definition(self):
        root = ET.Element('config')
        backends = ET.SubElement(root, 'backends')
        name = 'db'
        plugin = 'mysql'
        ET.SubElement(backends, 'backend', {'name':name, 'plugin':plugin})
        op = BackendParser()
        
        op.parse(root)
        result = op.backends
        
        assert_that(result, is_not(None))
        assert_that(len(result), is_not(0))
        assert_that(result, has_key(name))
        assert_that(result[name].plugin, is_(plugin))
        
    def test_that_config_backend_is_ignored_if_plugin_is_empty(self):
        root = ET.Element('config')
        backends = ET.SubElement(root, 'backends')
        ET.SubElement(backends, 'backend', {'name':'db', 'plugin':''})
        op = BackendParser()
        
        op.parse(root)
        result = op.backends
        
        self.l.check(('root', 'WARNING', "Ignoring invalid backend definition, name = 'db', plugin = ''"))
        assert_that(result, is_not(has_key('db')))
        
    def test_that_config_backend_is_ignored_if_name_is_not_specified(self):
        root = ET.Element('config')
        backends = ET.SubElement(root, 'backends')
        ET.SubElement(backends, 'backend', {'plugin':'mysql'})
        op = BackendParser()
        
        op.parse(root)
        result = op.backends
        
        self.l.check(('root', 'WARNING', "Ignoring invalid backend definition, name = 'None', plugin = 'mysql'"))
        assert_that(len(result), is_(0))
        
    def test_that_backends_are_ignored_if_not_included_in_a_config_element(self):
        root = ET.Element('random_tag')
        ET.SubElement(root, 'backends')
        op = BackendParser()
        
        result = op.parse(root)
        
        assert_that(result, is_(None))
        
    def test_that_dictionary_of_parameters_is_created_if_parameters_specified(self):
        (root, backend) = _create_valid_backend()
        key = 'ip'
        value = '1.2.3.4'
        params = ET.SubElement(backend, 'params')
        param = ET.SubElement(params, key)
        param.text = value   
        op = BackendParser()
        
        op.parse(root)
        result = op.backends
        
        assert_that(result[backend.get('name')].params, has_entry(key, value))

