import os

import cog

from codegen import parse_and_generate_code as parse
from codegen import category_for_file

import codegen_types.ctypes as ctypes

#TODO Get the input path and output path from cog globals
_output_dir = "./tmp"
_input_path = ['../model']

def init():
    global models
    models = parse(_input_path, None, _output_dir, None)
        
init()


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
