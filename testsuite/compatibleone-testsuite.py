#!/usr/bin/env python
# vim: set et sw=4 ts=4 ai:

from utils import * 

# Ensure correct directories
from dirs import *

# Basic checks on all binaries
from basic.accords import *
from basic.cobroker import *
from basic.cocheck import *
from basic.cocommand import *
from basic.coparser import *
from basic.coplatform import *
from basic.coprovider import *
from basic.coprovision import *
from basic.coresolver import *
from basic.costart import *
from basic.costatus import *
from basic.costop import *
from basic.command import *
from basic.parser import *
from basic.azprocci import *
from basic.broker import *
from basic.coees import *
from basic.cops import *
from basic.coips import *
from basic.comons import *
from basic.conets import *
from basic.conagios import *
from basic.coobas import *
from basic.cool import *
from basic.cosacs import *
from basic.cosched import *
from basic.coss import *
from basic.dcprocci import *
from basic.jpaasprocci import *
from basic.copaas import *
from basic.ezvm import *
from basic.example import *
from basic.fileserver import *
from basic.onprocci import *
from basic.osocciprocci import *
from basic.osprocci import *
from basic.paprocci import *
from basic.procci import *
from basic.publisher import *
from basic.slam import *
from basic.testaz import *
from basic.testcb import *
from basic.testdc import *
from basic.teston import *
from basic.testos import *
from basic.testosocci import *
from basic.testresolver import *

# Check platform basic running
from platform.start import *
from platform.status import *
from platform.stop import *

import unittest
if __name__ == '__main__':
    unittest.main()
