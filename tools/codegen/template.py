import os

import cog

from codegen import parse_and_generate_code as parse
from codegen import category_for_file

_output_dir = "./tmp"
_input_path = ['../model']

def init():
    # TODO Get input parameters from cogs globals
    #input_file_paths = ["cords.xml", 'backend.xml', 'test_config.xml']
    global models
    models = parse(_input_path, None, _output_dir, None)
        
init()

def _type_conversion(type_name):
    if type_name == "int":
        return "integer"
    return type_name

def category_xsd(category_file):
    category = category_for_file(category_file, models)   
    cog.outl('<xsd:complexType name="{0}">'.format(category.struct_name))
    cog.outl(r'    <xsd:attribute name="{0}" type="xsd:{1}"/>'.format('id', 'string'))
    for name, attr in category.attrs.items():
        cog.outl(r'    <xsd:attribute name="{0}" type="xsd:{1}"/>'.format(name, _type_conversion(attr.attrtype)))
    for name in category.colls.keys():
        cog.outl(r'    <xsd:attribute name="{0}" type="xsd:{1}"/>'.format(name, 'string'))
