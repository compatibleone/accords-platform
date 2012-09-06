##############################################################################
#copyright 2012,Hamid MEDJAHED  (hmedjahed@prologue.fr) Prologue             #
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

# Implementation of category actions


import sys
import pycompdev
import pypacksrc
srcdirectory=pypacksrc.srcpydir+"/pyaccords/pysrc/"
srcdirectoryc=pypacksrc.srcpydir+"/cocarrier/src/"
sys.path.append(srcdirectory)
sys.path.append(srcdirectoryc)
from ec2client import *  
from amazonEc2Class import *
from actionClass import *
""" Note:respAction is a python class to describe the occi response with the status and the message
	amazonEc2 is a python class to interface the accords category :amazonEc2.
	-Attributes of this category are members of this class.
	-List of attributes:

		- name 
		- flavor 
		- image 
		- original 
		- profile 
		- node 
		- price 
		- account 
		- number 
		- rootpass 
		- reference 
		- network 
		- access 
		- accessip 
		- floating 
		- floatingid 
		- publicaddr 
		- privateaddr 
		- firewall 
		- group 
		- zone 
		- hostname 
		- workload 
		- when 
		- state 
"""

def amazonEc2_start(amazonEc2):
		
	response=respAction("200","ok")
	

	respboto = ec2_start(amazonEc2.when, amazonEc2.state, "europe" , amazonEc2.name, amazonEc2.image,amazonEc2.flavor)
	if (respboto == "OK"):
		response.message="OK"
		response.status="200"
	else :
		response.message="NOT_OK"
		response.status="400"
		
	return response
		


def amazonEc2_save(amazonEc2):
	response=respAction("200","ok")
	"""Implement here your function"""

	return response

def amazonEc2_snapshot(amazonEc2):
	response=respAction("200","ok")
	"""Implement here your function"""

	return response

def amazonEc2_stop(amazonEc2):
	response=respAction("200","ok")
	"""Implement here your function"""

	return response

def amazonEc2_suspend(amazonEc2):
	response=respAction("200","ok")
	"""Implement here your function"""

	return response

def amazonEc2_restart(amazonEc2):
	response=respAction("200","ok")
	"""Implement here your function"""

	return response

