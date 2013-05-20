import logging

class Use(object):
    _item_type = None
    
    def __init__(self, backend):
        self._backend = backend
        self._includes = []
        self._excludes = []
        
    def _include_and_exclude_warning(self, item):
        logging.warn("{0} '{1}' is both included and excluded in backend '{2}'".format(self._item_type, item, self._backend))
        
    def include(self, item):
        if item in self._excludes:
            self._include_and_exclude_warning(item)
        self._includes.append(item)
        
    def exclude(self, item):
        if item in self._includes:
            self._include_and_exclude_warning(item)
        self._excludes.append(item)
            
    def merge(self, other):
        [self.include(category) for category in other._includes]
        [self.exclude(category) for category in other._excludes]
        
    @property
    def backend(self):
        return self._backend
        
class CategoryUse(Use):
    _item_type = "Category"
    
    def has(self, category, _):
        return category in self._includes
    
    def applies_to(self, category, _):
        return category in (self._includes + self._excludes)
    
class ComponentUse(Use):
    _item_type = "Component"
    
    def has(self, category, components):
        for component_name in self._includes:
            if category in components[component_name].categories:
                return True
    
    def applies_to(self, category, components):
        for component_name in (self._includes + self._excludes):
            if category in components[component_name].categories:
                return True

class PlatformUse(Use):
    _item_type = "Platform"
    
    def has(self, _, _1):
        return len(self._includes) > 0
    
    def applies_to(self, _, _1):
        return (len(self._includes + self._excludes) > 0)
    
class Uses(object):
    
    def __init__(self):
        self._uses = {}        
        
    #TODO Can we eliminate the getter entirely? Only used for tests?
    @property
    def uses(self):
        return self._uses
    
    def backend_for(self, category, components=[]):
        for (backend, use_set) in self._uses.items():
            for use in use_set:
                if (use.applies_to(category, components)):
                    return backend if use.has(category, components) else None
        return None   
  
    def add_uses(self, uses):
        for backend, use_set in uses.items():
            if(self._uses.has_key(backend)):
                for (own, other) in zip(self._uses[backend], use_set):
                    if own:
                        own.merge(other)
            else:
                self._uses[backend] = use_set
        
