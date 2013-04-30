'''
Created on 15 Apr 2013

@author: jevc
'''

class Link(object):
    '''
    Class to represent a Link
    '''

    def __init__(self, linkid):
        '''
        Constructor
            @param linkid: link id
        '''
        self.linkid = linkid
        # Gets resolved to link definition
        self.linkidP = None
        