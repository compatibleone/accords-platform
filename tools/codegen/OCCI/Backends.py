class Backend(object):
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
    
