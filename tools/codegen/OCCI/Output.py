'''
Created on 4 Apr 2013

@author: jevc
'''

import OCCI

class Output(object):
    '''
    Abstract base class for parser output
    '''
    # TODO consider mechanism to pass options into Output generators

    def __init__(self, models, output_filename, output_dir):
        assert isinstance(models, OCCI.Model.Models)
        self.models = models
        self.output_filename = output_filename
        self.output_dir = output_dir
        
    def go(self):
        '''
        Do the output
        '''
        pass
