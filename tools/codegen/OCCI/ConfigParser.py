import logging

from Uses import Uses, CategoryUse, ComponentUse, PlatformUse
from XmlParser import XmlParser
from Backends import Backend

class UsesParser(XmlParser):
    _parse_type = "uses"
    _root_tag = "config"
    
    def __init__(self):
        self._uses = Uses()
        
    @property
    def uses(self):
        '''
        @return: Single Uses instance
        '''
        return self._uses    
    
    def _parse_platform_use(self, backend, use_element):
        platform_use = PlatformUse(backend)
        if use_element.find("includeall") is not None:
            platform_use.include("all")
        return platform_use
    
    def _parse_component_use(self, backend, use_element):
        component_use = ComponentUse(backend)
        [component_use.include(component.get("name")) for component in use_element.findall("include/component")]
        [component_use.exclude(component.get("name")) for component in use_element.findall("exclude/component")]
        return component_use
    
    def _parse_category_use(self, backend, use_element):
        category_use = CategoryUse(backend)
        [category_use.include(category.get("name")) for category in use_element.findall("include/category")]
        [category_use.exclude(category.get("name")) for category in use_element.findall("exclude/category")]
        return category_use
    
    def _parse_use_element(self, use_element): 
        backend = use_element.get('backend')        
        if backend:
            platform_use = self._parse_platform_use(backend, use_element)
            category_use = self._parse_category_use(backend, use_element)
            component_use = self._parse_component_use(backend, use_element)
            return (backend, (category_use, component_use, platform_use))
        logging.warn("Ignoring invalid use definition, no backend referenced")
        return None         
    
    def _parse_uses(self, xmlmodel):
        #TODO Refactoring: This can be combined with _parse_backend
        pairs = [self._parse_use_element(use) for use in xmlmodel.findall("use")]
        return dict(pair for pair in pairs if pair)
    
    def _parse(self, xmlmodel):
        uses = self._parse_uses(xmlmodel)
        self._uses.add_uses(uses)

    def _resolve_category(self, category, components, backends):
        backend = self._uses.backend_for(category, components)
        if backend is None:
            #TODO Should this be an error?
            logging.warn("Category '{0}' has no backend specified".format(category))
        else:
            if backend in backends:                
                link_backend = backends[backend]
                category.set_backend(link_backend)
                link_backend.add_category(category)             
            else:
                logging.warn("Category '{0}' has link to nonexistant backend {1}".format(category, backend))
        
    def resolve(self, categories, components, backends = {}):
        [self._resolve_category(category, components, backends) for category in categories]

class BackendParser(XmlParser):
    _parse_type = "backends"
    _root_tag = "config"
    
    def __init__(self):
        self._backends = {}
                
    def _use_if_valid(self, backend):
        name = backend.get('name')
        plugin = backend.get('plugin')
        params_element = backend.find('params')
        if params_element is not None:
            params = dict([(el.tag, el.text) for el in params_element])
        else:
            params = {}
        if name and plugin:
            return (name, Backend(plugin, params))
        logging.warn("Ignoring invalid backend definition, name = '{0}', plugin = '{1}'".format(name, plugin))
        return None        
        
    def _parse(self, xmlmodel):
        pairs = [self._use_if_valid(backend) for backend in xmlmodel.findall("backend")]  
        self._backends.update(pair for pair in pairs if pair)
                
    @property
    def backends(self):
        '''
            @return: dictionary of <name:backend> pairs
        '''
        return self._backends
