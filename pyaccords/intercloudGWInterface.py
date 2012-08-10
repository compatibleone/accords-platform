###############################################################################
# copyright 2012, Marouen Mechtri (Marouen.Mechtri@it-sudparis.eu)            #
# Institut Mines-Telecom - TELECOM & Management SudParis                      #
# Licensed under the Apache License, Version 2.0 (the "License");             #
# you may not use this file except in compliance with the License.            #
# You may obtain a copy of the License at                                     #
#                                                                             #
#       http://www.apache.org/licenses/LICENSE-2.0                            #
#                                                                             #
# Unless required by applicable law or agreed to in writing, software         #
# distributed under the License is distributed on an "AS IS" BASIS,           #
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.    #
# See the License for the specific language governing permissions and         #
# limitations under the License.                                              #
###############################################################################

#!/usr/bin/env python
# -*- coding: latin-1 -*-
# Implementation of category CRUD functions
import sys
import pycompdev
import pypacksrc
srcdirectory=pypacksrc.srcpydir+"/pyaccords/pysrc/"
sys.path.append(srcdirectory)
from intercloudGWClass import *
from client import OCCIclient
""" Note: intercloudGW is a python class to interface the accords category :intercloudGW.
	-Attributes of this category are members of this class.
	-List of attributes:

		- name 
		- node 
		- account 
		- price 
		- state 
"""

def intercloudGW_create(intercloudGW):
	"""Implement here your function"""
	print('$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$')
	print('                   Receiving POST intercloudGW                      ')
	intercloudGW.state = '0'

	return intercloudGW

def intercloudGW_retrieve(intercloudGW):
	"""Implement here your function"""

	return intercloudGW

def intercloudGW_update(intercloudGW):
	"""Implement here your function"""

	return intercloudGW

def intercloudGW_delete(intercloudGW):
	"""Implement here your function"""

	return intercloudGW

