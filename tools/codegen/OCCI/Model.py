'''
Created on 4 Apr 2013

@author: jevc
'''

import Category

class Models(dict):
    
    def add(self, model):
        self[model.name] = model
        
    def categories(self):        
        for model in self.values():
            for cat in model.list.values():
                yield(cat)

class Model(Category.CategoriesBase):
    '''
    Representation of a Model 
    '''
    
    def __init__(self, name, description, version, namespace = None):
        '''
        Constructor
            @param name: Model name
            @param description: Summary text about the model
            @param version: Current version of the model
            @param namespace: XML namespace for this model
        '''
        # TODO validations as much as is possible
        self.name = name
        self.description = description
        self.version = version
        self.namespace = namespace
        
        Category.CategoriesBase.__init__(self)
