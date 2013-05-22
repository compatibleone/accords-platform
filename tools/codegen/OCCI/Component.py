from XmlParser import XmlParser

class Component(object):
    
    def __init__(self, categories, name = None, attributes = None):
        self._name = name
        self._attributes = attributes
        self._categories = dict((category, None) for category in categories)
            
    @property
    def name(self):
        return self._name
    
    @property
    def attributes(self):
        return self._attributes
    
    @property
    def category_names(self):
        return self._categories.keys()
    
    def category(self, name):
        return self._categories[name]
    
    def set_category_link(self, name, link_to):
        self._categories[name] = link_to

