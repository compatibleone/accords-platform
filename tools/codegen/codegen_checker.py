#!/usr/bin/python
# encoding: utf-8
'''
codegen_checker.codegen -- shortdesc

codegen_checker.codegen is a description

It defines classes_and_methods

@author:     user_name
        
@copyright:  2013 organization_name. All rights reserved.
        
@license:    license

@contact:    user_email
@deffield    updated: Updated
'''

import sys
import os
from argparse import ArgumentParser
from argparse import RawDescriptionHelpFormatter
import glob
import re

# Local imports
import logger

__all__ = []
__version__ = 0.1
__date__ = '2013-03-28'
__updated__ = '2013-03-28'

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

def readVars(filename):
    result = {}
    
    # Prepare a whitespace remover
    wsr = re.compile(r'\s+')
    
    # Read the file in a line at a time
    for line in open(filename):
        m = re.match("^\s(.+)\s(\S+);$", line)
        if m:
            result[m.group(2)] = re.sub(wsr, "", m.group(1))
    
    return result
    

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

EXAMPLE
  ./tools/codegen/codegen_checker.py -m ./build/tools/strukt_autogen/ -s ./

USAGE
''' % (program_shortdesc, str(__date__))

    try:
        # Setup argument parser
        parser = ArgumentParser(description=program_license, formatter_class=RawDescriptionHelpFormatter)
        parser.add_argument("-v", "--verbose", dest="verbose", action="count", help="set verbosity level [default: %(default)s]")
        parser.add_argument("-m", "--model", dest="model", required=True, help="Model directory")
        parser.add_argument("-s", "--src", dest="src", required=True, help="Source directory")
        parser.add_argument('-V', '--version', action='version', version=program_version_message)
        
        # Process arguments
        args = parser.parse_args()
        
        log = logger.logger(args.verbose)
        modelDir = args.model
        srcDir = args.src
        log.write("Verbose mode on")
        
        # Get the directories in canonical form
        modelDir = os.path.abspath(modelDir) + "/"
        srcDir = os.path.abspath(srcDir) + "/"
        
        # Find all the model files
        modelFiles = []
        modelFiles.extend(glob.glob(modelDir+"*.h"))

        for modelFile in modelFiles:
            # Get the basename
            filename = os.path.basename(modelFile)

            # Try to find the existing header file in the usual place.
            potentialMatches = []
            potentialMatches.extend(glob.glob(srcDir + "*/src/" + filename))
            
            # Try to find the existing header file in the usual place but with underscores removed from the file name.
            if (len(potentialMatches) == 0):
                potentialMatches.extend(glob.glob(srcDir + "*/src/" + re.sub('_', '', filename)))
            
            if (len(potentialMatches) == 0):
                print "No matches for " + modelFile
            
            for potentialMatch in potentialMatches:
                output = []
                
                # Parse the generated model file
                modelVars = readVars(modelFile)
            
                # Parse the header file
                headerVars = readVars(potentialMatch)
                
                # Compare variables, first starting with ones that are in the model file.
                keysToRemove = []
                for modelVar in modelVars:
                    # Is it in the header file?
                    if (modelVar in headerVars):
                        if (modelVars[modelVar] != headerVars[modelVar]):
                            output.append("    " + "Difference: " + modelFile + ":" + modelVar + " is of type " + modelVars[modelVar] + " but " + potentialMatch + ":" + modelVar + " is of type " + headerVars[modelVar])
                            
                    keysToRemove.append(modelVar)
                    
                # Remove keys that we have processed
                for key in keysToRemove:
                    if (key in modelVars):
                        del modelVars[key]
                    if (key in headerVars):
                        del headerVars[key]
                    
                # Output missing vars
                for modelVar in modelVars:
                    output.append("    " + modelFile + ":" + modelVar + " is not in " + potentialMatch)
                    
                for headerVar in headerVars:
                    output.append("    " + potentialMatch + ":" + headerVar + " is not in " + modelFile)
                    
                if (len(output) > 0):
                    print "Comparing " + modelFile + " with " + potentialMatch
                    
                    for line in output:
                        print line

        return 0
    except KeyboardInterrupt:
        ### handle keyboard interrupt ###
        return 0
#    except Exception, e:
#        indent = len(program_name) * " "
#        sys.stderr.write(program_name + ": " + repr(e) + "\n")
#        sys.stderr.write(indent + "  for help use --help")
#        return 2

if __name__ == "__main__":
    sys.exit(main())