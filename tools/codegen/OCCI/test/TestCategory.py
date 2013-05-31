import unittest

from hamcrest import is_, assert_that

from Category import  Category

class MockAttribute(object):
    def __init__(self, name, type_name):
        self.name = name
        self.type_name = type_name
        
    @property
    def attrtype(self):
        return self.type_name

class TestCategory(unittest.TestCase):
    
    def test_that_for_file_returns_false_if_file_is_none(self):
        cat = Category("anything", None, None, None, None, None)
        
        assert_that(cat.for_file(None), is_(False))
        
    def test_that_for_file_returns_true_if_header_filename_matches_filename(self):
        headerFilename = "test"
        cat = Category("anything", None, None, None, None, None, headerFilename = headerFilename)
        
        assert_that(cat.for_file(headerFilename), is_(True))
        
    def test_that_for_file_returns_true_if_file_is_term_plus_dot_h(self):
        term = "something"
        cat = Category(term, None, None, None, None, None)
        
        assert_that(cat.for_file(term + ".h"), is_(True))
        
    def test_that_struct_name_returns_cords_underscore_term_if_not_initialised(self):
        term = "something"
        cat = Category(term, None, None, None, None, None)
        
        assert_that(cat.struct_name, is_("cords_" + term))
        
    def test_that_struct_name_returns_initialised_value_if_initialised(self):
        term = "something"
        struct_name = "something else"
        cat = Category(term, None, None, None, None, None, structName = struct_name)
        
        assert_that(cat.struct_name, is_(struct_name))
        
    def test_that_backend_type_list_yields_id_first(self):
        cat = Category(None, None, None, None, None, None)
        
        name, type_name = cat.backend_type_list().next()
        assert_that(name, is_('id'))
        assert_that(type_name, is_('string'))
        

    def _mock_attributes(self):
        return dict((name, MockAttribute(name, type_name)) for (name, type_name) in enumerate(range(4)))
    
    def _mock_collections(self):
        return dict((name, None) for name in range(3))

    def test_backend_type_list_yields_all_attributes_after_id(self):
        cat = Category(None, None, None, None, None, None)        
        attrs = self._mock_attributes()        
        cat.attrs = attrs   # This is a bit abusive, could add each attr via the public interface
         
        results = cat.backend_type_list()
        
        results.next() # Discard id         
        for attr in attrs.values():
            try:
                name, type_name = results.next()
            except StopIteration:
                self.fail('Not enough results returned from backend_type_list')            
            assert_that(name, is_(attr.name))
            assert_that(type_name, is_(attr.type_name))
            
    def test_backend_type_list_yields_all_collections_after_attributes(self):
        cat = Category(None, None, None, None, None, None)                
        attrs = self._mock_attributes()                
        colls = self._mock_collections()
        cat.attrs = attrs   # This is a bit abusive, could add each attr via the public interface
        cat.colls = colls   # This is a bit abusive, could add each coll via the public interface
                 
        results = cat.backend_type_list()
        
        # Discard id and attributes
        results.next() 
        for _ in attrs.values():
            results.next() 
        for coll in colls.keys():
            try:
                name, type_name = results.next()
            except StopIteration:
                self.fail('Not enough results returned from backend_type_list')            
            assert_that(name, is_(coll))
            assert_that(type_name, is_('string'))
            