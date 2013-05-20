from XmlParser import XmlParser

class Component(object):
    
    def __init__(self, categories, name = None, attributes = None):
        self._name = name
        self._attributes = attributes
        self._categories = categories
            
    @property
    def name(self):
        return self._name
    
    @property
    def attributes(self):
        return self._attributes
    
    @property
    def categories(self):
        return self._categories

