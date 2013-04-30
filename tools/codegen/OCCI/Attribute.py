'''
Created on 28 Mar 2013

@author: Jonathan Custance
'''

import Scope

class Attribute(object):
    '''
    Class to represent an attribute
    '''
    def __init__(self, name, attrtype, required, immutable, validation, index, default = None, units = None, legacytype = None, scope = "all", script = None):
        '''
        Constructor
            @param name: Attribute name
            @param attrtype: Attribute type e.g. int, string
            @param required: OCCI require
            @param immutable: OCCI immutable
            @param validation: Any validation string
            @param index: Is this indexable by backend
            @param default: Default value
            @param units: Units e.g. byte
            @param legacytype: Legacy C type (temp. for header generation)
            @param scope: Scope - xsd or all
            @param script: Any script needed by the CORDS parser
        '''
        self.name = name
        # TODO type validation
        self.attrtype = attrtype
        # TODO parse to Bools
        self.required = required == None and "false" or required
        self.immutable = immutable == None and "false" or immutable
        # TODO validation parsing etc.
        self.validation = validation
        # TODO parse to Bool
        self.index = index == None and "false" or index
        # TODO check default against validation
        self.default = default
        self.units = units
        self.legacytype = legacytype
        self.scope = Scope.Scope.parse(scope)
        self.script = script
        
    
        