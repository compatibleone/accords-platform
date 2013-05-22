'''
Created on 28 Mar 2013

@author: Jonathan Custance
'''

import logging

class CategoriesBase(object):
    '''
    A Base class to represent a collection of categories
    '''
    def __init__(self):
        '''
        Constructor
        '''
        self.list = {}
        
    def add(self, cat):
        '''
        Add a category
            @param cat: The category to add
        '''
        self.list[cat.catid] = cat
        
    def find(self, cat):
        '''
        Find a category by id
            @param cat: the category
            @return OCCI.Category.Category:
        '''
        if cat in self.list:
            return self.list[cat]
        else:
            return None
        
class Categories(CategoriesBase):
        '''
        Categories collection
        '''
        def __init__(self):
            '''
            Constructor
            '''
            CategoriesBase.__init__(self)
    
        def resolve(self):
            '''
            Resolve rels, collection, actions, links and mixins for a category
            '''
            for catid, cat in self.list.items():
                for name, coll in cat.colls.items():
                    # Find the category for a collection
                    found = self.find(coll.category)
                    if found is None:
                        logging.warn("Could not resolve "+coll.category+" for collection "+name+" category "+catid)
                    else:
                        coll.categoryP = found
                for name, action in cat.actions.items():
                    # Find the category for an action
                    found = self.find(action.actionid)
                    if found is None:
                        #logging.warn("Could not resolve action "+name+" category "+catid)
                        dummy = False
                    else:
                        action.actionidP = found
                for name, link in cat.links.items():
                    # Find the category for a link
                    found = self.find(link.linkid)
                    if found is None:
                        logging.warn("Could not resolve link "+name+" category "+catid)
                    else:
                        link.linkidP = found
                for name, mixin in cat.mixins.items():
                    # Find the category for a mixin
                    found = self.find(mixin.mixinid)
                    if found is None:
                        logging.warn("Could not resolve mixin "+name+" category "+catid)
                    else:
                        mixin.mixinidP = found
                if cat.rel is not None:
                    # Find the rel for a Category
                    found = self.find(cat.rel)
                    if found is None:
                        logging.warn("Could not resolve rel "+cat.rel+" for category "+catid)
                    else:
                        cat.relP = found

class Category(object):
    '''
    The representation of a Category. It contains it's attributes and collections
    '''

    def __init__(self, term, scheme, location, rel, klass, model, structName = None, headerFilename = None):
        '''
        Constructor
            @param term: OCCI term
            @param scheme: OCCI scheme
            @param location: Location
            @param rel: The related kind or mixin etc.  
            @param klass: The klass e.g. kind, mixin, action
            @param model: The Model this Category belongs to
            @param structName: The optional attribute to override the structure name in auto-generated code
            @param headerFilename: The optional attribute to override the name of the auto-generated header file
        '''
        # TODO validation of params as much as is possible
        self.term = term
        self.scheme = scheme
        self.location = location
        self.rel = rel
        self.klass = klass
        self.attrs = {}
        self.colls = {}
        self.actions = {}
        self.links = {}
        self.mixins = {}
        self.model = model
        self.structName = structName;
        # Will hold the rel pointer once resolved
        self.relP = None
        self.headerFilename = headerFilename
        self._component = None
        self._backend = None
    
    def allAttrs(self):
        '''
        Get all the attributes for a given item
            @return list:
        '''
        ret = self.attrs
        if self.relP != None:
            ret.extend(self.relP.allAttrs)
        return ret
    
    def allColls(self):
        '''
        Get all the collections for a given item
            @return list:
        '''
        ret = self.colls
        if self.relP != None:
            ret.extend(self.relP.allColls)
        return ret
        
    def allAttrsAndColls(self):
        '''
        Get all the collections and attributes for a given item
            @return list:
        '''
        ret = self.attrs
        ret.extend(self.colls)
        if self.relP != None:
            ret.extend(self.relP.allAttrsAndColls)
        return ret
    
    def allLinks(self):
        '''
        Get all the links for a given item
            @return list:
        '''
        ret = self.links
        if self.relP != None:
            ret.extend(self.relP.allLinks)
        return ret
    
    def allActions(self):
        '''
        Get all the actions for a given item
            @return list:
        '''
        ret = self.actions
        if self.relP != None:
            ret.extend(self.relP.allActions)
        return ret
    
    def allMixins(self):
        '''
        Get all the mixins for a given item
            @return list:
        '''
        ret = self.mixins
        if self.relP != None:
            ret.extend(self.relP.allMixins)
        return ret
        
    @property
    def catid(self):
        '''
        Get the OCCI Category id
            @return str:
        '''
        return self.scheme+self.term
    
    def addattr(self, attr):
        '''
        Add an attribute
            @param attr: The Attribute to add
        '''
        self.attrs[attr.name] = attr
    
    def addaction(self, action):
        ''' Add an action
            @param action: The Action to add
        '''
        self.actions[action.actionid] = action
        
    def addlink(self, link):
        ''' Add an link
            @param link: The Link to add
        '''
        self.links[link.linkid] = link
        
    def addmixin(self, mixin):
        ''' Add an mixin
            @param mixin: The Mixin to add
        '''
        self.mixins[mixin.mixinid] = mixin
        
    def addcoll(self, coll):
        '''
        Add a collection
            @param coll: The Collection to add
        '''
        self.colls[coll.name] = coll
    
    def getOutputFilename(self):
        '''
        Get the output file name (auto or derived from legacy information)
            @param cat: The category
            @return The filename
        '''
        filename = (self.term + ".h")
        if (self.headerFilename != None):
            filename = self.headerFilename

        return filename
    
    def set_component(self, component):
        self._component = component

    def set_backend(self, backend):
        self._backend = backend
