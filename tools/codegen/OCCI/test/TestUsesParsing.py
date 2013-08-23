import unittest
from xml.dom import minidom
from xml.etree import ElementTree as ET

from testfixtures import LogCapture
from hamcrest import assert_that, is_, is_not, has_key, same_instance, has_item

from codegen.OCCI.Uses import Uses
from codegen.OCCI.ConfigParser import UsesParser
from codegen.OCCI.Component import Component
from ParserMocks import MockCategory

def _dump_xml_as_file(root, filename):
    with open(filename, 'w+') as f:
        f.write(minidom.parseString(ET.tostring(root)).toprettyxml(encoding="utf-8"))
            
class MockBackend(object):
    def __init__(self):
        self.categories = []
        self.add_category_called = False
    
    def add_category(self, category):
        self.add_category_called = True
        self.categories.append(category)
        
    
def _build_tree_with_include_element(backend):
    root = ET.Element('config')
    uses = ET.SubElement(root, 'uses')
    backend = 'db'
    use = ET.SubElement(uses, 'use', {'backend':backend})
    include = ET.SubElement(use, 'include')
    return include, root

def _build_tree_with_valid_category_include(backend, category_to_include):
    include, root = _build_tree_with_include_element(backend)
    ET.SubElement(include, 'category', {'name':category_to_include.catid})
    return root

def _build_tree_with_valid_component_include(backend, component_to_include):
    include, root = _build_tree_with_include_element(backend)
    ET.SubElement(include, 'component', {'name':component_to_include})
    return root

class TestUsesResolving(unittest.TestCase):
    
    def setUp(self):
        self.l = LogCapture()
        
    def tearDown(self):
        self.l.uninstall()
        
    def test_that_resolving_warns_about_missing_category_backend_if_no_includes_found(self):
        op = UsesParser()
        category = MockCategory()
        
        op.resolve([category], [])
        
        self.l.check(('root', 'WARNING', "Category '{0}' has no backend specified".format(category.catid)))
        
    def test_that_resolving_does_not_warn_if_category_missing_backend_but_include_all_specified(self):
        root = ET.Element('config')
        uses = ET.SubElement(root, 'uses')
        backend = 'db'
        use = ET.SubElement(uses, 'use', {'backend':backend})
        ET.SubElement(use, 'includeall')  
        op = UsesParser()
        category = MockCategory()
        op.parse(root)

        op.resolve([category], [], {backend:MockBackend()})
        
        assert_that(self.l.__str__(), is_("No logging captured"))        
    
    def test_that_resolving_does_not_warn_if_category_missing_backend_but_component_includes_it(self):
        backend = 'db'
        category = MockCategory()
        example_component = Component(categories = [category.catid]) 
        component_name = 'example'
        root = _build_tree_with_valid_component_include(backend, component_name)
        op = UsesParser()
        op.parse(root)        
        
        op.resolve([category], {component_name:example_component}, {backend:MockBackend()})
        
        assert_that(self.l.__str__(), is_("No logging captured"))
        
    def test_that_component_level_exclude_overrides_include_all(self):
        category = MockCategory()      
        example_component = Component(categories = [category.catid]) 
        component_name = 'example'
        root = ET.Element('config')
        uses = ET.SubElement(root, 'uses')
        backend = 'db'
        use = ET.SubElement(uses, 'use', {'backend':backend})
        ET.SubElement(use, 'includeall')
        exclude = ET.SubElement(use, 'exclude')
        ET.SubElement(exclude, 'component', {'name':component_name})
        op = UsesParser()  
        op.parse(root)      
        
        op.resolve([category], {component_name:example_component})
        
        self.l.check(('root', 'WARNING', "Category '{0}' has no backend specified".format(category.catid)))
        
    def test_that_resolve_warns_if_category_linked_to_nonexistant_backend(self):
        backend = 'db'
        category = MockCategory()
        root = _build_tree_with_valid_category_include(backend, category)
        op = UsesParser()
        op.parse(root)
        
        op.resolve([category], {})
        
        self.l.check(('root', 'WARNING', "Category '{0}' has link to nonexistant backend {1}".format(category.catid, backend)))
        
    def test_that_resolve_links_categories_to_backend(self):
        backend = 'db'
        category = MockCategory()
        root = _build_tree_with_valid_category_include(backend, category)
        op = UsesParser()
        op.parse(root)
        example_backend = MockBackend()
        
        op.resolve([category], {}, {backend:example_backend})
        
        assert_that(category.set_backend_called, is_(True))
        assert_that(category.backend, is_(same_instance(example_backend)))
        
    def test_that_resolve_links_backends_to_categories(self):
        backend_name = 'db'
        category = MockCategory()
        root = _build_tree_with_valid_category_include(backend_name, category)
        op = UsesParser()
        op.parse(root)
        backend = MockBackend()
        
        op.resolve([category], {}, {backend_name:backend})
        
        assert_that(backend.add_category_called, is_(True))
        assert_that(backend.categories, has_item(same_instance(category)))
        
    def test_that_resolve_warns_if_component_include_is_not_in_components_list(self): 
        component_name = 'nonexistant'
        root = ET.Element('config')
        uses = ET.SubElement(root, 'uses')
        backend = 'db'
        use = ET.SubElement(uses, 'use', {'backend':backend})
        include = ET.SubElement(use, 'include')
        ET.SubElement(include, 'component', {'name':component_name})
        op = UsesParser()  
        op.parse(root)      
        category_name = "don't care"
        op.resolve([MockCategory(category_name)], {})
        
        # TODO We don't care about the second method here.  Create a better matcher than l.check to allow checking for only a subset of the messages 
        self.l.check(('root', 'WARNING', "Component '{0}' is included/excluded for backend {1}, but does not exist".format(component_name, backend)),
                      ('root', 'WARNING', "Category '{0}' has no backend specified".format(category_name)))
                
    
class TestUsesParsing(unittest.TestCase):
    
    #TODO This is testing an internal implementation detail, can probably be removed / replaced once implementation is
    # completed
    def test_that_parse_returns_a_uses_including_a_dictionary_of_use_sets_if_at_least_one_is_specified(self):
        root = ET.Element('config')
        uses = ET.SubElement(root, 'uses')
        ET.SubElement(uses, 'use', {'backend':'db'})
        op = UsesParser()
        
        op.parse(root)
        result = op.uses
        
        assert_that(result.uses, is_not(None))
        assert_that(result.uses, has_key('db'))
        
     #TODO This is testing an internal implementation detail, can probably be removed / replaced once implementation is
    # completed
    def test_that_parse_ignores_uses_which_dont_reference_a_backend(self):
        root = ET.Element('config')
        uses = ET.SubElement(root, 'uses')
        ET.SubElement(uses, 'use')
        op = UsesParser()
        
        op.parse(root)
        result = op.uses
        
        assert_that(len(result.uses), is_(0))
      
    def test_that_parse_returns_a_uses_if_one_is_specified(self):
        root = ET.Element('config')
        ET.SubElement(root, 'uses')
        op = UsesParser()
        
        op.parse(root)
        result = op.uses
        
        assert_that(result, is_not(None))
        assert_that(type(result), is_(type(Uses)))
        
    def _add_uses_element(self, root, backend, category):
        uses = ET.SubElement(root, 'uses')
        use = ET.SubElement(uses, 'use', {'backend':backend})
        include = ET.SubElement(use, 'include')
        ET.SubElement(include, 'category', {'name':category})

    def test_that_two_uses_in_same_tree_can_both_add_includes(self):
        root = ET.Element('config')
        backend1 = 'db'
        category1 = 'publication'
        backend2 = 'cass'
        category2 = 'another'
        self._add_uses_element(root, backend1, category1)
        self._add_uses_element(root, backend2, category2)
        op = UsesParser()
        
        op.parse(root)
        result = op.uses
        
        assert_that(result.backend_for(category1), is_(backend1))
        assert_that(result.backend_for(category2), is_(backend2))
        
    def test_that_category_is_added_to_includes_for_backend_if_include_specified_in_use_element(self):
        backend = 'db'
        category = MockCategory()
        root = _build_tree_with_valid_category_include(backend, category)
        op = UsesParser()
        
        op.parse(root)
        result = op.uses
        
        assert_that(result.backend_for(category.catid), is_(backend))

    def test_that_parsing_two_uses_elements_with_a_category_include_each_includes_both(self):
        backend = 'db'
        category1 = MockCategory('publication')
        category2 = MockCategory('another')
        root1 = _build_tree_with_valid_category_include(backend, category1)
        root2 = _build_tree_with_valid_category_include(backend, category2)
        op = UsesParser()
        
        op.parse(root1)
        op.parse(root2)
        result = op.uses 
        
        assert_that(result.backend_for(category1.catid), is_(backend))
        assert_that(result.backend_for(category2.catid), is_(backend))
    
    def test_that_category_is_added_to_included_in_backend_with_include_all_specified(self):
        root = ET.Element('config')
        uses = ET.SubElement(root, 'uses')
        backend = 'db'
        use = ET.SubElement(uses, 'use', {'backend':backend})
        ET.SubElement(use, 'includeall')
        op = UsesParser()
        
        op.parse(root)
        result = op.uses
        
        assert_that(result.backend_for('anything'), is_(backend))
        
    def test_that_excluded_category_overrides_include_all_element(self):
        root = ET.Element('config')
        uses = ET.SubElement(root, 'uses')
        backend = 'db'
        use = ET.SubElement(uses, 'use', {'backend':backend})
        ET.SubElement(use, 'includeall')
        include = ET.SubElement(use, 'exclude')
        category = 'publication'
        ET.SubElement(include, 'category', {'name':category})
        op = UsesParser()
        
        op.parse(root)
        result = op.uses
        
        assert_that(result.backend_for(category), is_(None))
        