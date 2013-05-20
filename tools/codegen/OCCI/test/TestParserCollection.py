import unittest

from hamcrest import assert_that, is_

from ParserCollection import ParserCollection

class MockParser(object):
    def __init__(self):
        self.parse_called = False
        self.root = None
        
    def parse(self, root):
        self.parse_called = True
        self.root = root
        
class MockUsesParser(MockParser):
    
    def __init__(self):
        self.resolve_called = False
        self.resolve_categories = None
        self.resolve_components = None
    
    def resolve(self, category_names, components):
        self.resolve_called = True
        self.resolve_categories = category_names
        self.resolve_components = components
        
class MockComponentParser(MockParser):
    sample_components = ["Made up"]
    
    @property
    def components(self):
        return self.sample_components

class TestParserCollection(unittest.TestCase):
    
    def test_that_parse_calls_parse_on_all_parsers(self):
        parsers = [MockParser(), MockParser(), MockParser()]
        root = "example"
        dut = ParserCollection(*parsers)
        
        dut.parse(root)
        
        for parser in parsers:
            assert_that(parser.parse_called, is_(True))
            assert_that(parser.root, is_(root)) 
            
    def test_that_resolve_calls_resolve_on_uses_parser(self):
        component_parser = MockComponentParser()
        uses_parser = MockUsesParser()
        backend_parser = MockParser()
        dut = ParserCollection(backend_parser = backend_parser, component_parser = component_parser, uses_parser = uses_parser)
        categories = 5
        
        dut.resolve(categories)
        
        assert_that(uses_parser.resolve_called, is_(True))
        assert_that(uses_parser.resolve_categories, is_(categories))
        assert_that(uses_parser.resolve_components, is_(component_parser.sample_components))
        