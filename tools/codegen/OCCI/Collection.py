'''
Created on 28 Mar 2013

@author: Jonathan Custance
'''

import Cardinality
import Scope

class MultiplicityException(Exception):
    '''
    Exception for multiplicty parsing
    '''
    def __init__(self, msg):
        '''
        Constructor
            @param msg: Exception message
        '''
        self.msg = msg

class Multiplicity(object):
    '''
    Class to represent the multiplicty of a collection
    '''
    def __init__(self, raw):
        '''
        Constructor
            @param raw: The raw multiplicty string e.g. [0..1]
        '''
        if not raw.startswith("[") and not raw.endswith("]"):
            raise MultiplicityException("Incorrectly formatted: "+raw)
        values = raw.strip("[]").split("..")
        if len(values) != 2:
            raise MultiplicityException("Incorrectly formatted: "+raw)
        self.min = self._getCardinality(values[0])
        self.max = self._getCardinality(values[1])
        
    def _getCardinality(self, value):
        '''
        Get the cardinality from a string
            @return Cardinality: The cardinality
        '''
        if value == "0":
            return Cardinality.Zero
        elif value == "1":
            return Cardinality.One
        else:
            return Cardinality.Unbounded
        

class Collection(object):
    '''
    Class to represent a collection
    '''
    def __init__(self, name, category, multiplicity, legacytype = None, scope = "all", script = None):
        '''
        Constructor
            @param name: Collection name
            @param category: The OCCI category
            @param multiplicity: A representation of the multiplicty e.g. [0..1]
            @param legacytype: The legacy type (note temporary for header generation)
            @param scope: The scope all or xsd
            @param script: Any script which is applicable to CORDS parsing
        '''
        self.name = name
        self.category = category
        self.multiplicity = Multiplicity(multiplicity)
        self.categoryP = None
        self.legacytype = legacytype
        self.scope = Scope.Scope.parse(scope)
        self.script = script

        