'''
Created on 5 Apr 2013

@author: jevc
'''

class Scope(object):
    '''
    Represent the scope of an attribute or collection
    '''
    def parse(value):
        '''
        Parse a string and give a Scope class (enum-esque)
            @param value: String value e.g. xsd or all
            @return Scope: A scope e.g. Xsd or All
        '''
        if value == "xsd":
            return Xsd
        else:
            return All
    parse = staticmethod(parse)
class All(Scope):pass
class Xsd(Scope):pass
