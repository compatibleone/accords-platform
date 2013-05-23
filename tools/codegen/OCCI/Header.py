'''
Created on 2 Apr 2013

@author: James Green
'''

import OCCI.Category
import OCCI.Output
import OCCI.Cardinality
import OCCI.Scope

class Header(OCCI.Output.Output):
    '''
    A class to implement "-t Header" to output auto-generated header files.
    '''
     
    def go(self):
        '''
        Generate the header(s).
        '''
        
        for model in self.models.list.values():
            for cat in model.list.values():
                assert isinstance(cat, OCCI.Category.Category)

                # See if we're outputting a single category, or all categories.
                if (self.output_filename != None):
                    if (cat.getOutputFilename() != self.output_filename):
                        continue
                
                # Create the C structure name and see if it has been overridden in the XML
                structName = "cords_" + cat.term
                if (cat.structName != None):
                    structName = cat.structName
                
                # Write out the auto-generated header file.
                f = open((self.output_dir + cat.getOutputFilename()), "w")
                f.write("#ifndef _" + structName + "_h\n")
                f.write("#define _" + structName + "_h\n\n")
                f.write("struct\t" + structName + "\n")
                f.write("{\n")
                
                f.write("\tchar *\tid;\n")            
                
                # TODO - get all attributes (using Category.allAttrs)
                # TODO - Use templating for header generation
                
                for name, attr in cat.attrs.items():
                    if attr.scope is OCCI.Scope.All:
                        varType = self.getCtype(attr.attrtype)
                        
                        if (attr.legacytype != None):
                            varType = attr.legacytype

                        f.write("\t" + varType + "\t" + name + ";\n")
                            
                for name, coll in cat.colls.items():
                    if coll.scope is OCCI.Scope.All:
                        varType = "char *"
                        
                        if (coll.multiplicity.max is OCCI.Cardinality.Unbounded):
                            varType = "int"
    
                        if (coll.legacytype != None):
                            varType = coll.legacytype
                        
                        f.write("\t" + varType + "\t" + name + ";\n")
            
                f.write("};\n\n")
                f.write("#endif\n")
                f.close();
            
        return None
    
    def getCtype(self, atype):
        '''
        Helper function to get the C type of an attribute.
            @param atype: The attribute node's type attribute.
        '''
        if (atype == "string"):
            return "char *"
        elif (atype == "int"):
            return "int"
        else:
            return "unknown"
