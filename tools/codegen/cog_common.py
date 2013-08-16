import os

import cog

from codegen import parse_and_generate_code as parse
from codegen import category_for_file

import codegen_types.ctypes as ctypes

# Example invocations from tools directiory:
# python -m cogapp -ed -D cog_category_file=publication.h -D model_dir="../model" -o tmp/output.xsd cog/templates/schema.xsd
# python -m cogapp -ed -D cog_category_file=publication.h -D model_dir='../model' -o tmp/output.h cog/templates/category.h 


def init_models(model_dir):
    global models
    models = parse([model_dir], None, None, None)

def _type_conversion(type_name):
    if type_name == "int":
        return "integer"
    return type_name

def category_xsd(category_file):
    category = category_for_file(category_file, models)   
    cog.outl('<xsd:complexType name="{0}">'.format(category.struct_name))
    for name, type_name in category.backend_type_list():
        cog.outl(r'    <xsd:attribute name="{0}" type="xsd:{1}"/>'.format(name, _type_conversion(type_name)))

def category_h_guard(category_file):
    category = category_for_file(category_file, models)
    
    guard = category.struct_name.lower() + "_h" # TODO Could just use the file name
    cog.outl('#ifndef {0}'.format(guard))    
    cog.outl('#define {0}'.format(guard))
    
def category_h_struct(category_file):
    category = category_for_file(category_file, models)
    
    cog.outl('struct {0}'.format(category.struct_name)) 
        
def category_h_members(category_file):
    category = category_for_file(category_file, models)
    
    for name, type_name in category.backend_type_list():
        cog.outl('{0} {1};'.format(ctypes.from_platform_type(type_name), name))
