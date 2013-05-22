##############################################################################
#copyright 2012,Hamid MEDJAHED & Elyes ZEKRI (hmedjahed@prologue.fr)         #
# Prologue                                                                   #
#Licensed under the Apache License, Version 2.0 (the "License");             #
#you may not use this file except in compliance with the License.            #
#You may obtain a copy of the License at                                     #
#                                                                            #
#       http://www.apache.org/licenses/LICENSE-2.0                           #
#                                                                            #
#Unless required by applicable law or agreed to in writing, software         #
#distributed under the License is distributed on an "AS IS" BASIS,           #
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.    #
#See the License for the specific language governing permissions and         #
#limitations under the License.                                              #
##############################################################################
#!/usr/bin/env python
# -*- coding: latin-1 -*-
# Implementation of category CRUD functions
import sys
import pycompdev
import pypacksrc
srcdirectory=pypacksrc.srcpydir+"/pyaccords/pygen/"
sys.path.append(srcdirectory)
from ec2configClass import *
""" Note: ec2config is a python class to interface the accords category :ec2config.
	-Attributes of this category are members of this class.
	-List of attributes:

		- name 
		- description 
		- accesskey 
		- secretkey 
		- authenticate 
		- agent 
		- host 
		- version 
		- namespace 
		- base 
		- tls 
		- current 
"""

def ec2config_create(ec2config):
	"""Implement here your function"""

	return ec2config

def ec2config_retrieve(ec2config):
	"""Implement here your function"""

	return ec2config

def ec2config_update(ec2config):
	"""Implement here your function"""

	return ec2config

def ec2config_delete(ec2config):
	"""Implement here your function"""

	return ec2config

