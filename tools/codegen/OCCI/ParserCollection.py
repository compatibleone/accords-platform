
from ComponentParser import ComponentParser
from ConfigParser import BackendParser, UsesParser

class ParserCollection(object):
    def __init__(self, component_parser = ComponentParser(), backend_parser = BackendParser(), uses_parser = UsesParser()):
        self._uses_parser = uses_parser
        self._component_parser = component_parser
        self._parsers = [component_parser, backend_parser, uses_parser]
        
    def parse(self, root):
        [parser.parse(root) for parser in self._parsers]
        
    def resolve(self, categories):
        self._uses_parser.resolve(categories, self._component_parser.components)
            