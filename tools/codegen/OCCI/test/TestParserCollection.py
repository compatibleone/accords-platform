import unittest

from hamcrest import assert_that, is_, same_instance

from ParserCollection import ParserCollection
from ParserMocks import MockCategory

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
        self.resolve_backends = None
    
    def resolve(self, category_names, components, backends):
        self.resolve_called = True
        self.resolve_categories = category_names
        self.resolve_components = components
        self.resolve_backends = backends
        
class MockComponentParser(MockParser):
    sample_components = ["Made up"]
    
    def __init__(self):
        self.resolve_categories = None
        self.resolve_called = False
    
    @property
    def components(self):
        return self.sample_components
    
    def resolve(self, categories):
        self.resolve_called = True
        self.resolve_categories = categories
        
class MockBackendParser(MockParser):
    sample_backends = ['db']
    
    @property
    def backends(self):
        return self.sample_backends

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
        dut = ParserCollection(backend_parser = MockBackendParser(), component_parser = component_parser, uses_parser = uses_parser)
        categories = 5
        
        dut.resolve(categories)
        
        assert_that(uses_parser.resolve_called, is_(True))
        assert_that(uses_parser.resolve_categories, is_(categories))
        assert_that(uses_parser.resolve_components, is_(component_parser.sample_components))
        
    def test_that_resolve_passes_backends_to_uses_parser_resolve(self):
        component_parser = MockComponentParser()
        uses_parser = MockUsesParser()
        backend_parser = MockBackendParser()
        dut = ParserCollection(backend_parser = backend_parser, component_parser = component_parser, uses_parser = uses_parser)
        categories = 5
        
        dut.resolve(categories)
        
        assert_that(uses_parser.resolve_backends, is_(same_instance(backend_parser.sample_backends)))
    
    def test_that_resolve_calls_resolve_on_component_parser(self):
        component_parser = MockComponentParser()
        dut = ParserCollection(backend_parser = MockBackendParser(), component_parser = component_parser, uses_parser = MockUsesParser())        
        categories = 'arbitrary'
        
        dut.resolve(categories)
        
        assert_that(component_parser.resolve_called, is_(True))
        assert_that(component_parser.resolve_categories, is_(categories))
        
        
        