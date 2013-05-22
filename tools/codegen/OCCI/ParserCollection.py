
from ComponentParser import ComponentParser
from ConfigParser import BackendParser, UsesParser

class ParserCollection(object):
    '''
    Manages the parsers.
    
    Use parse on one or more xml trees (ElementTree) in order to build up the sets of Components, Backends etc.
    
    Resolve checks for consistency between all the parsed collections and converts the soft (string) links into
    actual links between the objects created 
    '''
    def __init__(self, component_parser = ComponentParser(), backend_parser = BackendParser(), uses_parser = UsesParser()):
        self._uses_parser = uses_parser
        self._component_parser = component_parser
        self._backend_parser = backend_parser
        self._parsers = [component_parser, backend_parser, uses_parser]
        
    def parse(self, root):
        [parser.parse(root) for parser in self._parsers]
        
    def resolve(self, categories):
        self._component_parser.resolve(categories)
        self._uses_parser.resolve(categories, self._component_parser.components, self._backend_parser.backends)
            