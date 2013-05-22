class XmlParser(object):
    _parse_type = None
    _root_tag = None
            
    def parse(self, tree):
        if tree.tag == self._root_tag:
            [self._parse(xmlmodel) for xmlmodel in tree.findall(self._parse_type)]
    
    def _parse(self, _):
        raise NotImplementedError("Abstract method")