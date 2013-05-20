import unittest
import xml.etree.ElementTree as ET
from hamcrest import assert_that, is_, is_not, has_key

from Uses import Uses
from ConfigParser import UsesParser

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
        
        result = op.parse(root)
        
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
        root = ET.Element('config')
        uses = ET.SubElement(root, 'uses')
        backend = 'db'
        use = ET.SubElement(uses, 'use', {'backend':backend})
        include = ET.SubElement(use, 'include')
        category = 'publication'
        ET.SubElement(include, 'category', {'name':category})
        op = UsesParser()
        
        op.parse(root)
        result = op.uses
        
        assert_that(result.backend_for(category), is_(backend))
        

    def _build_tree_with_valid_include(self, backend, category_to_include):
        root = ET.Element('config')
        uses = ET.SubElement(root, 'uses')
        backend = 'db'
        use = ET.SubElement(uses, 'use', {'backend':backend})
        include = ET.SubElement(use, 'include')
        ET.SubElement(include, 'category', {'name':category_to_include})
        return root

    def test_that_parsing_two_uses_elements_with_a_category_include_each_includes_both(self):
        backend = 'db'
        category1 = 'publication'
        category2 = 'another'
        root1 = self._build_tree_with_valid_include(backend, category1)
        root2 = self._build_tree_with_valid_include(backend, category2)
        op = UsesParser()
        
        op.parse(root1)
        op.parse(root2)
        result = op.uses 
        
        assert_that(result.backend_for(category1), is_(backend))
        assert_that(result.backend_for(category2), is_(backend))
    
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
        