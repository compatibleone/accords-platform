import unittest

from hamcrest import is_, assert_that

from Category import  Category

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