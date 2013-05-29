

class MockCategory(object):
    def __init__(self, name = 'anything'):
        self.name = name
        self.component = None
        self.backend = None
        self.set_component_was_called = False
        self.set_backend_called = False
        
    @property
    def catid(self):
        return self.name
    
    def set_component(self, component):
        self.component = component
        self.set_component_was_called = True
        
    def set_backend(self, backend):
        self.backend = backend
        self.set_backend_called = True
