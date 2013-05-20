import unittest
from testfixtures import LogCapture

import xml.etree.ElementTree as ET
from hamcrest import assert_that, is_, is_not, has_key

from ConfigParser import BackendParser, Config

class TestBackendParsing(unittest.TestCase):
    
    def setUp(self):
        self.l = LogCapture()
        
    def tearDown(self):
        self.l.uninstall()
    
    def test_that_parse_returns_a_config_if_xml_contains_config_element_including_backends_list(self):
        root = ET.Element('config')
        ET.SubElement(root, 'backends')
        op = BackendParser()
        
        result = op.parse(root)
        
        assert_that(result, is_not(None))
        assert_that(type(result), is_(type(Config)))
        
    def test_that_config_includes_a_backends_list_if_config_contains_backends_definition(self):
        root = ET.Element('config')
        backends = ET.SubElement(root, 'backends')
        ET.SubElement(backends, 'backend', {'name':'db', 'plugin':'mysql'})
        op = BackendParser()
        
        result = op.parse(root)
        
        assert_that(result[0].backends, is_not(None))
        backends = result[0].backends
        assert_that(len(backends), is_not(0))
        assert_that(backends, has_key('db'))
        assert_that(backends['db'], is_('mysql'))
        
    def test_that_config_backend_is_ignored_if_plugin_is_empty(self):
        root = ET.Element('config')
        backends = ET.SubElement(root, 'backends')
        ET.SubElement(backends, 'backend', {'name':'db', 'plugin':''})
        op = BackendParser()
        
        result = op.parse(root)
        
        self.l.check(('root', 'WARNING', "Ignoring invalid backend definition, name = 'db', plugin = ''"))
        assert_that(result[0].backends, is_not(has_key('db')))
        
    def test_that_config_backend_is_ignored_if_name_is_not_specified(self):
        root = ET.Element('config')
        backends = ET.SubElement(root, 'backends')
        ET.SubElement(backends, 'backend', {'plugin':'mysql'})
        op = BackendParser()
        
        result = op.parse(root)
        
        self.l.check(('root', 'WARNING', "Ignoring invalid backend definition, name = 'None', plugin = 'mysql'"))
        assert_that(len(result[0].backends), is_(0))
        
    def test_that_backends_are_ignored_if_not_included_in_a_config_element(self):
        root = ET.Element('random_tag')
        ET.SubElement(root, 'backends')
        op = BackendParser()
        
        result = op.parse(root)
        
        assert_that(result, is_(None))

