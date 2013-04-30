
'''
Created on 28 Mar 2013

@author: Jonathan Custance
'''
import sys
import xml.etree.ElementTree as ET
import Category
import Attribute
import Action
import Link
import Mixin
import Collection
import Model
import logging

class Parser(object):
    '''
    Parse a set of XML model files and return a Models object containing all the
    models that have been successfully parsed.
    '''

    def __init__(self, files):
        '''
        Constructor
            @param files: list of files to parse
        '''
        self.files = files
        self.cats = Category.Categories()
        self.models = Model.Models()
        
        # Add the standard categories from OCCI Core
        self._built_in_model = Model.Model("core", "OCCI Core categories", "1.0,0")
        self.models.add(self._built_in_model)
        # Entity
        entity = Category.Category("entity", "http://schemas.ogf.org/occi/core#", "/entity/", None, "kind", self._built_in_model)
        entity.addattr(Attribute.Attribute("id", "string", "true", "true", None, "true"))
        entity.addattr(Attribute.Attribute("title", "string", "false", "false", None, "false"))
        self.cats.add(entity)
        
        # Resource
        resource = Category.Category("resource", "http://schemas.ogf.org/occi/core#", "/resource/", 
                                     "http://schemas.ogf.org/occi/core#entity", "kind", self._built_in_model)
        resource.addattr(Attribute.Attribute("summary", "string", "false", "false", None, "false"))
        self.cats.add(resource)
        
        # Link
        link = Category.Category("link", "http://schemas.ogf.org/occi/core#", "/link/", 
                                     "http://schemas.ogf.org/occi/core#entity", "link", self._built_in_model)
        link.addattr(Attribute.Attribute("source", "string", "true", "false", None, "false"))
        link.addattr(Attribute.Attribute("target", "string", "true", "false", None, "false"))
        self.cats.add(link)
        
    def parse(self):
        '''
            Do the work of the Parser class
        '''
        # Parse each file / model
        for f in self.files:
            self._parse(f)
        # Resolve rels and collections/instances
        self.cats.resolve()
        return self.models
    
    def _addcoll(self, colls, cat):
        '''
        Parse and add collection attributes to a category
            @param colls: a collection of XML Elements
            @param cat: the Category to add the collections to
        '''
        for coll in colls:
            name = coll.get("name")
            if name == None:
                logging.warn("Category "+cat.term+" - invalid collection")
                continue
            logging.info("Category "+cat.term+" collection "+name)
            try:
                cat.addcoll(Collection.Collection(name, coll.get("category"), coll.get("multiplicity"), 
                                                  coll.get("legacytype"), coll.get("scope"), coll.get("script")))
            except:
                logging.error("Category "+cat.term+"Problem processing collection "+id)
                logging.error(sys.exc_info())
    
    def _addactions(self, category, cat):
        '''
        Parse and add all actions to a category
            @param category: a collection of XML Elements
            @param cat: the Category to add the collections to
        '''
        for action in category.findall("actions/action"):
            actionid = action.get("id")
            if actionid == None:
                logging.warn("Category "+cat.term+" - invalid action")
                continue
            logging.info("Category "+cat.term+" action "+actionid)
            try:
                cat.addaction(Action.Action(actionid))
            except:
                logging.error("Category "+cat.term+" Problem processing action "+actionid)
                logging.error(sys.exc_info())
                
    def _addlinks(self, category, cat):
        '''
        Parse and add all links to a category
            @param category: a collection of XML Elements
            @param cat: the Category to add the collections to
        '''
        for link in category.findall("links/link"):
            linkid = link.get("id")
            if linkid == None:
                logging.warn("Category "+cat.term+" - invalid link")
                continue
            logging.info("Category "+cat.term+" link "+linkid)
            try:
                cat.addlink(Link.Link(linkid))
            except:
                logging.error("Category "+cat.term+" Problem processing link "+linkid)
                logging.error(sys.exc_info())
                
    def _addmixins(self, category, cat):
        '''
        Parse and add all mixins to a category
            @param category: a collection of XML Elements
            @param cat: the Category to add the collections to
        '''
        for mixin in category.findall("mixins/mixin"):
            mixinid = mixin.get("id")
            if mixinid == None:
                logging.warn("Category "+cat.term+" - invalid mixin")
                continue
            logging.info("Category "+cat.term+" mixin "+mixinid)
            try:
                cat.addmixin(Mixin.Mixin(mixinid))
            except:
                logging.error("Category "+cat.term+" Problem processing mixin "+mixinid)
                logging.error(sys.exc_info())
       
    def _addattrs(self, category, cat):
        '''
        Parse and add all attributes to a category
            @param category: a collection of XML Elements
            @param cat: the Category to add the collections to
        '''
        # Parse attributes
        for attr in category.findall("attributes/attribute"):
            name = attr.get("name")
            if name == None:
                logging.warn("Category"+cat.term+" - invalid attribute")
                continue
            logging.info("Category "+cat.term+" attribute "+name)
            try:
                cat.addattr(Attribute.Attribute(name, attr.get("type"), attr.get("required"), 
                                                attr.get("immutable"), attr.get("validation"),
                                                 attr.get("index"), attr.get("default"), attr.get("units"), attr.get("legacytype"), 
                                                 attr.get("scope"),  attr.get("script")))
            except:
                logging.error("Category "+cat.term+"Problem processing attribute "+id)
                logging.error(sys.exc_info())
        
        # Parse instances
        colls = category.findall("attributes/instance")
        self._addcoll(colls, cat)
        
        # Parse collections
        colls = category.findall("attributes/collection")
        self._addcoll(colls, cat)  
            
    def _parse(self, f):
        '''
        Parse a given XML file
            @param f File name
        '''
        logging.info("Processing "+f)
        try:
            tree = ET.parse(f)
            root = tree.getroot()
            # TODO schema version check etc.
            # TODO checking for single model
            # TODO warn about unknown XML nodes
            try:
                xmlmodel = root.find("model")
                model = Model.Model(xmlmodel.get("name"), xmlmodel.get("description"), 
                                    xmlmodel.get("version"), xmlmodel.get("namespace"))
                logging.info("Model is "+model.name+" (v. "+model.version+")")
            except:
                logging.error("Problem processing model")
                return
            # Find all categories
            for category in xmlmodel.findall("category"):
                term = category.get("term")
                if term == None:
                    logging.warn("No category provided")
                    continue
                logging.info("Category "+term)
                # Add a category
                try:
                    scheme, klass, location, rel, structName, headerFilename = category.get("scheme"), category.get("class"), category.get("location"), category.get("rel"), category.get("structname"), category.get("headerfilename")
                    cat = Category.Category(term, scheme, location, rel, klass, model, structName, headerFilename)
                    self._addattrs(category, cat)
                    self._addactions(category, cat)
                    self._addlinks(category, cat)
                    self._addmixins(category, cat)
                    model.add(cat)
                    self.cats.add(cat)
                except:
                    logging.error("Problem processing category "+term)
                    logging.error(sys.exc_info())
            # Add this model to the models collection
            self.models.add(model)
        except:
            logging.error("Problem parsing "+f)
            logging.error(sys.exc_info())
        
        
            
        