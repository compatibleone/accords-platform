#!/usr/bin/python2.7
# encoding: utf-8
# TODO update re: license / owner etc.
'''
codegen.codegen -- shortdesc

codegen.codegen is a description

It defines classes_and_methods

@author:     OnApp
        
@copyright:  2013 organization_name. All rights reserved.
        
@license:    license

@contact:    user_email
@deffield    updated: Updated
'''

# TODO - Need to add actions and designators for mixins and links

import sys
import os
import glob
from argparse import ArgumentParser
from argparse import RawDescriptionHelpFormatter

# Local imports
import logging
import OCCI.Parser
from stat import S_ISDIR, S_ISREG

__all__ = []
__version__ = "0.1.0"
__date__ = '2013-03-28'
__updated__ = '2013-04-04'

# TODO handle and check schema version's from meta

class CLIError(Exception):
    '''Generic exception to raise and log different fatal errors.'''
    def __init__(self, msg):
        super(CLIError).__init__(type(self))
        self.msg = "E: %s" % msg
    def __str__(self):
        return self.msg
    def __unicode__(self):
        return self.msg

def get_class( kls ):
    parts = kls.split('.')
    module = ".".join(parts[:-1])
    m = __import__( module )
    for comp in parts[1:]:
        m = getattr(m, comp)            
    return m


def _parse_all_input_files(input_paths):
    # Get all input files and pass to parser
    files = []
    for inpath in input_paths:
    # Get all files in this path
        if (os.path.exists(os.path.abspath(inpath))):
            mode = os.stat(inpath).st_mode
        # TODO consider directory recursion
        # Add all files in dir or specific file dependant on type
            if S_ISDIR(mode):
                files.extend(glob.glob(inpath + "/*"))
            if S_ISREG(mode):
                files.append(inpath)
        else:
            logging.warn("Ignoring (non-existent) input path " + inpath)
    
    op = OCCI.Parser.Parser(files)
# Do the parse
    models = op.parse()
    return models

def _check_output_file_exists(filename, models):
    if (filename != None):
        for model in models.list.values():
            for cat in model.list.values():
                if (cat.getOutputFilename() == filename):
                    return            
        logging.error("Error: File argument '" + filename + "' is not valid.")
        sys.exit(1)
            

def _check_output_type_and_get_class(output_type):
    try:
        output_class = get_class("OCCI." + output_type + "." + output_type)
# Note issubclass can raise an exception hence wrap in try and exception raised
# if issubclass is False
        if not issubclass(output_class, OCCI.Output.Output):
            raise Exception()
    except:
        logging.error("Error: Output type " + output_type + " is not valid")
        sys.exit(1)
    return output_class


def _check_output_directory(output_dir):
# Check the output directory and get it in canonical form
    if not os.path.exists(output_dir):
        logging.error("Error: Output directory {0} does not exist.".format(os.path.abspath(output_dir)))
        sys.exit(1)
    if not os.path.isdir(output_dir):
        logging.error("Error: Output directory {0} is not a directory.".format(os.path.abspath(output_dir)))
        sys.exit(1)


def _set_logging_level(logging_verbose):
    if (logging_verbose == True):
        logging.basicConfig(format='%(message)s', level=logging.DEBUG)
    else:
        logging.basicConfig(format='%(message)s')

def main(argv=None): # IGNORE:C0111
    '''Command line options.'''
    
    if argv is None:
        argv = sys.argv
    else:
        sys.argv.extend(argv)

    program_name = os.path.basename(sys.argv[0])
    program_version = "v%s" % __version__
    program_build_date = str(__updated__)
    program_version_message = '%%(prog)s %s (%s)' % (program_version, program_build_date)
    program_shortdesc = __import__('__main__').__doc__.split("\n")[1]
    program_license = '''%s

  Created by user_name on %s.
  Copyright 2013 organization_name. All rights reserved.
  
  Licensed under the Apache License 2.0
  http://www.apache.org/licenses/LICENSE-2.0
  
  Distributed on an "AS IS" basis without warranties
  or conditions of any kind, either express or implied.

USAGE
''' % (program_shortdesc, str(__date__))

    try:
        # Process arguments
        parser = ArgumentParser(description=program_license, formatter_class=RawDescriptionHelpFormatter)
        parser.add_argument("-v", "--verbose", dest="verbose", action="store_true", default=False, help="Select to enable info output")
        parser.add_argument("-t", "--type", dest="outputType", required=False, help="Choose output type")
        parser.add_argument('-V', '--version', action='version', version=program_version_message)
        parser.add_argument("-o", "--output", dest="outputDst", required=True, help="Choose output directory")
        parser.add_argument("-f", "--file", dest="file", help="Process a single specific category/file")
        parser.add_argument(dest="paths", help="paths to folder(s) with source file(s) [default: %(default)s]", metavar="path", nargs='+')
        args = parser.parse_args()

        logging_verbose = args.verbose
        output_dir = args.outputDst
        output_filename = args.file
        input_paths = args.paths
        output_type = args.outputType
        
        _set_logging_level(logging_verbose)
        _check_output_directory(output_dir)                
        # Get directory in canonical form to help code down the line.
        output_dir = os.path.abspath(output_dir) + "/"
                
        models = _parse_all_input_files(input_paths)

        _check_output_file_exists(output_filename, models)
        
        if output_type is not None:
            output_class = _check_output_type_and_get_class(output_type)
            generator = output_class(models, output_filename, output_dir)            
            # Generate output
            generator.go()
        
        return 0
    except KeyboardInterrupt:
        ### handle keyboard interrupt ###
        return 0
#    except Exception, e:
#        indent = len(program_name) * " "
#        sys.stderr.write(program_name + ": " + repr(e) + "\n")
#        sys.stderr.write(indent + "  for help use --help\n")
#        return 2

if __name__ == "__main__":
    sys.exit(main())
