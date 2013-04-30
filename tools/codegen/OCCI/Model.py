'''
Created on 4 Apr 2013

@author: jevc
'''

import Category

class Models(object):
    '''
    Hold all the models
    '''
    
    def __init__(self):
        '''
        Constructor
        '''
        self.list = {}
        
    def add(self, model):
        '''
        Add a model
            @param model: The Model to add
        '''
        self.list[model.name] = model

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
