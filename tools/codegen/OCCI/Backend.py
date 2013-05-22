class Backend(object):
    '''
    Backend type with a plugin and zero or more parameters (Parameter functionality is TBD.
    Links to categories handled by this backend
    '''
    def __init__(self, plugin, params):
        self._plugin = plugin
        self._params = params
        self._categories = []
        
    @property
    def plugin(self):
        return self._plugin
    
    @property
    def params(self):
        return self._params
        
    def add_category(self, category):
        self._categories.append(category)
    