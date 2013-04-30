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

    def __init__(self, models, args):
        assert isinstance(models, OCCI.Model.Models)
        '''
        Constructor
            @param models: The Models that have been parsed
            @param outputDir: The target output directory
        '''
        self.models = models
        self.args = args
        
    def go(self):
        '''
        Do the output
        '''
        pass
