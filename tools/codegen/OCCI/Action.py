'''
Created on 15 Apr 2013

@author: jevc
'''

class Action(object):
    '''
    Class to represent an Action
    '''


    def __init__(self, actionid):
        '''
        Constructor
            @param id: action id
        '''
        self.actionid = actionid
        # Gets resolved to action definition
        self.actionidP = None
        