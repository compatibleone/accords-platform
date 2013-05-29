'''
Created on 2 Apr 2013

@author: James Green
'''

import OCCI.Category
import OCCI.Output
import OCCI.Cardinality
import OCCI.Scope

import codegen_types.ctypes as ctypes

class Header(OCCI.Output.Output):
    '''
    A class to implement "-t Header" to output auto-generated header files.
    '''
   
    def _write_header_file(self, cat, struct_name, f):
        f.write("#ifndef _" + struct_name + "_h\n")
        f.write("#define _" + struct_name + "_h\n\n")
        f.write("struct\t" + struct_name + "\n")
        f.write("{\n")
        f.write("\tchar *\tid;\n")
    # TODO - get all attributes (using Category.allAttrs)
    # TODO - Use templating for header generation
        for name, attr in cat.attrs.items():
            if attr.scope is OCCI.Scope.All:
                #TODO Templating functions should check legacy type as well
                varType = attr.legacytype if attr.legacytype != None else ctypes.from_platform_type(attr.attrtype)
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

    def go(self):
        '''
        Generate the header(s).
        '''
        
        for cat in self.models.categories():
            assert isinstance(cat, OCCI.Category.Category)

            # See if we're outputting a single category, or all categories.
            if not cat.for_file(self.output_filename):
                continue
            
            # Write out the auto-generated header file.
            with open((self.output_dir + self.output_filename), "w") as f: 
                self._write_header_file(cat, cat.struct_name, f)
            
        return None
