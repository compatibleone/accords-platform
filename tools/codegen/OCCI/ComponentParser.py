import logging

from XmlParser import XmlParser
from Component import Component

class ComponentParser(XmlParser):         
    _root_tag = "components"
    _parse_type = "component"
        
    def __init__(self):
        self._components = []
        
    @property
    def components(self):
        return self._components   
        
    def _parse(self, xmlmodel):
        name = xmlmodel.get('name')
        if not name:
            logging.warn("Ignoring component definition with no name")
        else:
            attributes = xmlmodel.attrib
            categories = [category.get('name') for category in xmlmodel.findall("category")]
            self._components.append(Component(categories, name, attributes))
        #TODO Eliminate return statement
        return self._components
    
    def resolve(self, categories):
        for component in self._components:
            for category in component.categories:
                if category not in categories:
                    logging.warn("Component '{0}' references undefined category '{1}'".format(component.name, category))