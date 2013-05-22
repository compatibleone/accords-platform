import unittest
import xml.etree.ElementTree as ET

from testfixtures import LogCapture
from hamcrest import assert_that, is_, is_not, has_entry, has_item, same_instance

from Component import Component
from ComponentParser import ComponentParser

from ParserMocks import MockCategory


def _component_tree_with_category(category, name):
    root = ET.Element('components')
    sample_component = ET.SubElement(root, 'component', {'name':name})
    ET.SubElement(sample_component, 'category', {'name':category})
    return root
    
class TestComponentResolving(unittest.TestCase):
    def setUp(self):
        self.l = LogCapture()
        
    def tearDown(self):
        self.l.uninstall()
        
    def test_that_resolve_does_not_warn_if_component_list_empty(self):
        root = ET.Element('component')
        op = ComponentParser()
        
        op.parse(root)
        op.resolve([])
        
        assert_that(self.l.__str__(), is_("No logging captured"))
        
    def test_that_resolve_does_not_warn_if_all_categories_found(self):
        category_name = 'something'
        category = MockCategory(category_name)
        name = 'sample component'
        root = _component_tree_with_category(category_name, name)
        op = ComponentParser()
        
        op.parse(root)
        op.resolve([category])
        
        assert_that(self.l.__str__(), is_("No logging captured"))
        
    def test_that_resolve_links_to_category(self):
        category_name = 'something'
        category = MockCategory(category_name)
        name = 'sample component'
        root = _component_tree_with_category(category_name, name)
        op = ComponentParser()
        
        op.parse(root)
        op.resolve([category])
        result = op.components[0]
        
        assert_that(result.category(category_name), is_(same_instance(category)))
        assert_that(category.set_component_was_called, is_(True))
        assert_that(category.component, is_(same_instance(result)))
        
    def test_that_resolve_warns_if_category_referenced_which_does_not_exist(self):
        category = 'something'
        name = 'sample component'
        root = _component_tree_with_category(category, name)
        op = ComponentParser()
        
        op.parse(root)
        op.resolve([])
        
        self.l.check(('root', 'WARNING', "Component '{0}' references undefined category '{1}'".format(name, category)))
        
    
    
class TestComponentParsing(unittest.TestCase):
    def setUp(self):
        self.l = LogCapture()
        
    def tearDown(self):
        self.l.uninstall()
    
    def test_that_parse_requires_an_outer_components_element(self):
        root = ET.Element('component')  #Note the lack of a final 's'
        op = ComponentParser()
        
        op.parse(root)
        result = op.components
        
        assert_that(result, is_([]))
        
    
    def test_that_parse_generates_a_component_if_xml_contains_component_element(self):
        root = ET.Element('components')
        ET.SubElement(root, 'component', {'name':"don't care"})
        op = ComponentParser()
        
        op.parse(root)
        result = op.components
        
        assert_that(result, is_not([]))
        assert_that(type(result), is_(type(Component)))
    
    def test_that_parse_generates_two_component_if_xml_contains_two_component_elements(self):
        root = ET.Element('components')
        ET.SubElement(root, 'component', {'name':"don't care"})
        ET.SubElement(root, 'component', {'name':"still don't care"})
        op = ComponentParser()
        
        op.parse(root)
        result = op.components
        
        assert_that(len(result), is_(2))
        assert_that(type(result[0]), is_(type(Component)))
        assert_that(type(result[1]), is_(type(Component)))
                
    def test_that_parse_generates_empty_list_if_xml_does_not_contain_components_element(self):
        root = ET.Element('model')
        op = ComponentParser()
        
        op.parse(root)
        result = op.components
        
        assert_that(result, is_([]))
        
    
    def test_that_parse_generates_a_component_with_attributes_if_specified(self):
        root = ET.Element('components')
        ET.SubElement(root, 'component', { 'name':'example', 'random':'abc', 'random2':'123'})
        op = ComponentParser()
        
        op.parse(root)
        result = op.components[0]
        
        assert_that(result.attributes, is_not(None))
        assert_that(result.attributes, has_entry('name', 'example'))
        assert_that(result.attributes, has_entry('random', 'abc'))
        assert_that(result.attributes, has_entry('random2', '123'))
        
    def test_that_parse_sets_component_names_as_specified(self):
        name = 'sample component'
        root = ET.Element('components')
        ET.SubElement(root, 'component', {'name':name})
        op = ComponentParser()
        
        op.parse(root)
        result = op.components[0]
        
        assert_that(result.name, is_(name))

    def test_that_parse_generates_a_component_including_category_if_specified_with_name(self):
        category = 'publication'
        name = 'sample component'
        root = _component_tree_with_category(category, name)
        op = ComponentParser()
        
        op.parse(root)
        result = op.components[0]
        
        assert_that(result.category_names, is_not(None))
        assert_that(result.category_names, has_item(category))
        
    def test_that_parse_generates_a_component_including_two_categories_if_two_specified(self):
        root = ET.Element('components')
        sample_component = ET.SubElement(root, 'component', {'name':"don't care"})
        ET.SubElement(sample_component, 'category', {'name':"publication"})
        ET.SubElement(sample_component, 'category', {'name':"enquiry"})
        op = ComponentParser()
        
        op.parse(root)
        result = op.components[0]
        
        assert_that(result.category_names, is_not(None))
        assert_that(result.category_names, has_item('publication'))
        assert_that(result.category_names, has_item('enquiry'))
        
    @unittest.expectedFailure
    def test_that_parse_raises_if_category_has_no_name(self):
        '''Allow categories with no name in xml file, just means the element will be skipped'''        
        root = ET.Element('components')
        sample_component = ET.SubElement(root, 'component')
        ET.SubElement(sample_component, 'category')
        op = ComponentParser()
        
        self.assertRaises(AttributeError, op.parse)
        
    def test_that_parse_skip_element_and_generates_warning_if_component_has_no_name(self):
        root = ET.Element('components')
        sample_component = ET.SubElement(root, 'component')
        ET.SubElement(sample_component, 'category')
        op = ComponentParser()
        
        op.parse(root)
        result = op.components
        
        assert_that(result, is_([]))
        self.l.check(('root', 'WARNING', "Ignoring component definition with no name"))
        