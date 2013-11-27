'''
Created on 2 Apr 2013

@author: James Green
'''
import os

import OCCI.Category
import OCCI.Output
import OCCI.Cardinality
import OCCI.Scope

import codegen_types.ctypes as ctypes

class Header(OCCI.Output.Output):
    '''
    A class to implement "-t Header" to output auto-generated header files.
    '''

    def _write_header_preamble(self, struct_name, f):
        f.write("#ifndef _" + struct_name + "_h\n")
        f.write("#define _" + struct_name + "_h\n\n")
        f.write("struct\t" + struct_name + "\n")
        f.write("{\n")
        
    def _write_header_conclusion(self, f):   
        f.write("};\n\n")
        f.write("#endif\n")

    def _write_filter_struct(self, struct_name, filter_names, f):
        self._write_header_preamble("{0}_occi_filter".format(struct_name), f)
        f.write("\tstruct {0}\t*attributes;\n".format(struct_name))
        for name in filter_names:
            f.write("\tint\t{0};\n".format(name))
        self._write_header_conclusion(f)
   
    def _write_header_file(self, cat, struct_name, header_file, filter_file):
        self._write_header_preamble(struct_name, header_file)
                
        header_file.write("\tchar *\tid;\n")
        header_file.write("\tchar *\torderid;\n")
        filter_names = ['id']
    # TODO - get all attributes (using Category.allAttrs)
    # TODO - Use templating for header generation
        for name, attr in cat.attrs.items():
            if attr.scope is OCCI.Scope.All:
                #TODO Templating functions should check legacy type as well
                varType = attr.legacytype if attr.legacytype != None else ctypes.from_platform_type(attr.attrtype)
                header_file.write("\t" + varType + "\t" + name + ";\n")
                filter_names.append(name)
        
        for name, coll in cat.colls.items():
            if coll.scope is OCCI.Scope.All:
                varType = "char *"
                if (coll.multiplicity.max is OCCI.Cardinality.Unbounded):
                    varType = "int"
                if (coll.legacytype != None):
                    varType = coll.legacytype
                header_file.write("\t" + varType + "\t" + name + ";\n")
                filter_names.append(name) 
                         
        self._write_header_conclusion(header_file)
        
        self._write_filter_struct(struct_name, filter_names, filter_file)

    def go(self):
        '''
        Generate the header(s).
        '''
        
        for cat in self.models.categories():
            assert isinstance(cat, OCCI.Category.Category)

            # See if we're outputting a single category, or all categories.
            if not cat.for_file(self.output_filename):
                continue
            
            # TODO This is a bit of a hack, just infer the filename for the filter struct
            # from the filename of the primary struct
            filename_stripped, _ = os.path.splitext(self.output_filename)
            filter_filename = filename_stripped + "_occi_filter.h"
            
            # Write out the auto-generated header file.
            with open((self.output_dir + self.output_filename), "w") as header_file:
                with open(filter_filename, "w") as filter_file: 
                    self._write_header_file(cat, cat.struct_name, header_file, filter_file)
            
        return None
