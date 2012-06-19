#!/usr/bin/env python
# -*- coding: latin-1 -*-
# Hamid MEDJAHED (c) Prologue

# Implementation of category actions
import sys
import pycompdev
import pypacksrc
srcdirectory=pypacksrc.srcpydir+"/pyaccords/pysrc/"
sys.path.append(srcdirectory)
from ec2configClass import *
from actionClass import *
""" Note:respAction is a python class to describe the occi response with the status and the message
	ec2config is a python class to interface the accords category :ec2config.
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

def ec2config_current(ec2config):
	response=respAction("200","ok")
	"""Implement here your function"""

	return response

