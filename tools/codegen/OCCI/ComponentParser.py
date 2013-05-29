import logging

from XmlParser import XmlParser
from Component import Component

class ComponentParser(XmlParser):         
    _root_tag = "components"
    _parse_type = "component"
        
    def __init__(self):
        self._components = {}
        
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
            self._components[name] = Component(categories, name, attributes)
    
    def resolve(self, categories):
        for name, component in self._components.items():
            for category_name in component.category_names:
                category_link = [category_link for category_link in categories if category_link.catid == category_name]
                if category_link:
                    category = category_link[0]
                    component.set_category_link(category_name, category)
                    category.set_component(component)                    
                else:
                    logging.warn("Component '{0}' references undefined category '{1}'".format(name, category_name))