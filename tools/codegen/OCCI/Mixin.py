'''
Created on 15 Apr 2013

@author: jevc
'''

class Mixin(object):
    '''
    Class to represent a Mixin
    '''

    def __init__(self, mixinid):
        '''
        Constructor
            @param mixinid: mixin id
        '''
        self.mixinid = mixinid
        # Gets resolved to link definition
        self.mixinidP = None
        