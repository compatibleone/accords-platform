import os

import cog

from codegen import parse_and_generate_code as parse
from codegen import category_for_file

import codegen_types.ctypes as ctypes

# Example invocations from tools directiory:
# python -m cogapp -ed -D cog_category_file=publication.h -D model_dir="../model" -o tmp/output.xsd cog/templates/schema.xsd
# python -m cogapp -ed -D cog_category_file=publication.h -D model_dir='../model' -o tmp/output.h cog/templates/category.h 
# python -m cogapp -ed -D cog_category_file=publication.h -D model_dir='../model' -o tmp/publication_node_backend.h cog/templates/node_backend.h
# python -m cogapp -ed -D cog_category_file=publication.h -D model_dir='../model' -o tmp/publication_node_backend.c cog/templates/node_backend.c 


def init_models(model_dir):
    global models
    models = parse([model_dir], None, None, None)

def _type_conversion(type_name):
    if type_name == "int":
        return "integer"
    return type_name

def _category_name(category_file):
    return category_for_file(category_file, models).struct_name 

def category_name(category_file):
    cog.inline()
    cog.out(_category_name(category_file))   

def category_xsd(category_file):
    category = category_for_file(category_file, models)   
    cog.outl('<xsd:complexType name="{0}">'.format(category.struct_name))
    for name, type_name in category.backend_type_list():
        cog.outl(r'    <xsd:attribute name="{0}" type="xsd:{1}"/>'.format(name, _type_conversion(type_name)))


def node_h_guard(category_file):
    category = category_for_file(category_file, models)
    
    _generic_h_guard('{0}_node_backend'.format(category.struct_name))
    
def category_h_guard(category_file):
    category = category_for_file(category_file, models)
    
    _generic_h_guard(category.struct_name)
    
def _name_root(filename):
    filename_root, _ = os.path.splitext(filename)
    return filename_root.lower()

def filename_root(category_file):
    cog.inline()
    cog.out(_name_root(category_file))

def _generic_h_guard(filename):
    guard = '{0}_h'.format(_name_root(filename))
    
    cog.outl('#ifndef {0}'.format(guard))    
    cog.outl('#define {0}'.format(guard))
    
def category_h_struct(category_file):
    category = category_for_file(category_file, models)
    
    cog.outl('struct {0}'.format(category.struct_name)) 
        
def category_h_members(category_file):
    category = category_for_file(category_file, models)
    
    for name, type_name in category.backend_type_list():
        cog.outl('{0} {1};'.format(ctypes.from_platform_type(type_name), name))

def node_interface_declaration(category_file):
    name = _category_name(category_file)
    cog.outl("struct {0}_backend_interface *{1}_node_interface_func();".format(name, name))
    
def h_include(category_file):
    cog.outl("#include \"{0}\"".format(category_file.lower()))
    
def _check_pass_filter(name, type_name):    
    if (type_name == "string"):
        _check_string_passes_filter(name)
    elif (type_name == "int"):
        _check_int_passes_filter(name)
    else:
        raise(ValueError('Unexpected type {0}'.format(type_name)))
    
def _check_string_passes_filter(name):    
    cog.outl("    if ((fptr->{0})".format(name))
    cog.outl("    && (strlen( fptr->{0} ) != 0)) {{".format(name))
    cog.outl("        if(!( pptr->{0} ))".format(name))
    cog.outl("            return(0);")
    cog.outl("        else if ( strcmp(pptr->{0},fptr->{1}) != 0)".format(name, name))
    cog.outl("            return(0);")
    cog.outl("        }")
    
def _check_int_passes_filter(name):
    cog.outl("    if (( fptr->{0} ) && ( pptr->{1} != fptr->{2} )) return(0);".format(name, name, name))
    
def pass_category_filter(category_file):
    category = category_for_file(category_file, models)
    category_name = _category_name(category_file)
    cog.outl("private int pass_{0}_filter(".format(category_name))
    cog.outl("    struct {0} * pptr,struct {1} * fptr) {{".format(category_name, category_name))
    for name, type_name in category.backend_type_list():
        _check_pass_filter(name, type_name)
        
def load_attributes(category_file):
    category = category_for_file(category_file, models)
    for name, type_name in category.backend_type_list():
        cog.outl("if ((aptr = document_atribut( vptr, \"{0}\" )) != (struct xml_atribut *) 0)".format(name))
        if (type_name == "string"):
            cog.outl("    {0}->{1} = document_atribut_string(aptr);".format(_name_root(category_file), name))
        elif(type_name == "int"):
            cog.outl("    {0}->{1} = document_atribut_value(aptr);".format(_name_root(category_file), name))
        else:
            raise(ValueError('Unexpected type {0}'.format(type_name)))

def save_attributes(category_file):
    category = category_for_file(category_file, models)
    for name, type_name in category.backend_type_list():
        cog.outl("fprintf(h,\" {0}=%c\",0x0022);".format(name))
        if (type_name == "string"):
            cog.outl("fprintf(h,\"%s\",(pptr->{0}?pptr->{1}:\"\"));".format(name, name))
        elif(type_name == "int"):
            cog.outl("fprintf(h,\"%u\",pptr->{0});".format(name))
        else:
            raise(ValueError('Unexpected type {0}'.format(type_name)))
        cog.outl("fprintf(h,\"%c\",0x0022);")
        
def clone(category_file):
    category = category_for_file(category_file, models)
    for name, type_name in category.backend_type_list():
        cog.outl("if (original->{0}) {{".format(name))
        if (type_name == "string"):
            cog.outl("    success = success && (copy->{0} = allocate_string(original->{1}));".format(name, name))
        elif(type_name == "int"):
            cog.outl("    copy->{0} = original->{1};".format(name, name))
        else:
            raise(ValueError('Unexpected type {0}'.format(type_name)))
        cog.outl("}")
        
    
        
