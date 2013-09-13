import argparse
import string
import os

parser = argparse.ArgumentParser(description='Generate a file listing commands to cog to generate auto-generated code')
parser.add_argument('filename', help='file to be created')
parser.add_argument('src_dir', help='top-level source directory')
parser.add_argument('categories', help='a category to generate commands for', nargs ='+')

args = parser.parse_args()

_model_dir = os.path.join(args.src_dir, 'model')
_tools_dir = os.path.join(args.src_dir, 'tools')
_template_dir = os.path.join(os.path.join(_tools_dir, 'cog'), 'templates')
_category_c_template = os.path.join(_template_dir, 'template_category.c')
_category_h_template = os.path.join(_template_dir, 'template_category.h')
_node_backend_c_template = os.path.join(_template_dir, 'template_node_backend.c')
_node_backend_h_template = os.path.join(_template_dir, 'template_node_backend.h')
_schema_template = os.path.join(_template_dir, 'template_schema.xsd')
_occi_rest_c_template = os.path.join(_template_dir, 'template_occi.c')
_occi_rest_h_template = os.path.join(_template_dir, 'template_occi.h')
_backend_interface_template = os.path.join(_template_dir, 'template_category_backend_interface.h')
_filter_template = os.path.join(_template_dir, 'template_category_filter.h')
_occi_include_template = os.path.join(_template_dir, 'template_occi_include.c')


templates = [_category_c_template, _category_h_template, _node_backend_c_template, _node_backend_h_template, _schema_template, \
             _occi_rest_c_template, _occi_rest_h_template, _backend_interface_template, _filter_template, _occi_include_template]
name_patterns = ["{0}.c", "{0}.h", "{0}_node_backend.c", "{0}_node_backend.h", "{0}.xsd", \
                 "occi_rest_{0}.c", "occi_rest_{0}.h", "{0}_backend_interface.h", "{0}_occi_filter.h", \
                 "{0}_occi_include.c"]

with open(args.filename, 'w+') as output:
    output.write("# This is a generated list of commands to generate model .c and .h files\n")
    for category in args.categories:
        if category.endswith('.h'):
            category = category[:-2]
        for template, pattern in zip(templates, name_patterns):
            filename = pattern.format(category)
            output.write("{0} -I {2} -D cog_category_file={1}.h -D model_dir={3} -o {4}\n".format(template, category, _tools_dir, _model_dir, filename))
        
    

