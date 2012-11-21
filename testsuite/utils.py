# vim: set et sw=4 ts=4 ai:

import os
import sys

devnull = open(os.devnull,"w") 

sys.path.append('/tmp')

from cocredentials import User, Pass, Host
