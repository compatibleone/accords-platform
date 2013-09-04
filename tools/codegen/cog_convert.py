import argparse
import string

parser = argparse.ArgumentParser(description='Convert a file into a cog template, by performing simple substitutions')
parser.add_argument('input', help='file to be converted')
parser.add_argument('output', help='name of new file')

args = parser.parse_args()

_category_replace = \
"""
[[[cog t.category_name()]]]
[[[end]]]
"""

_filename_replace = \
"""
[[[cog t.filename_root()]]]
[[[end]]]
"""

with open(args.input) as f:
    with open(args.output, 'w+') as output:
        source = f.read()
        converted = string.replace(source, 'CATEGORY_NAME',_category_replace)
        converted = string.replace(converted, 'FILENAME_ROOT', _filename_replace)
        output.write(converted)

