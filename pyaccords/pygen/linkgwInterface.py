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
srcdirectory=pypacksrc.srcpydir+"/pyaccords/pygen/"
srcdirectoryc=pypacksrc.srcpydir+"/pyaccords/pysrc/"
sys.path.append(srcdirectoryc)
sys.path.append(srcdirectory)
from linkgwClass import *
from client import OCCIclient
""" Note: linkgw is a python class to interface the accords category :linkgw.
	-Attributes of this category are members of this class.
	-List of attributes:

		- name 
		- intercloudGW 
		- account 
		- gwsrc 
		- gwdst 
		- tunnelproto 
		- addressgresrc 
		- addressgredst 
		- prefix 
		- authenticationkey 
		- endpointsrc 
		- endpointdst 
		- state 
"""

def linkgw_create(linkgw):
	"""Implement here your function"""
	print('$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$')
	print('                       Receiving POST linkgw                        ')

	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#    
	attribute = {}
	client = OCCIclient(' ', ' ', ' ', ' ', attribute)
	parametersrc = client.GetElement_pathuuid(linkgw.endpointsrc)
	client.host = parametersrc['host']
	client.port = parametersrc['port']
	client.category = parametersrc['category']
	client.GetElement(parametersrc['uuid'])
	providergwsrc = client.GetElement(parametersrc['uuid'])['occi.contract.profile']
	print('The gw source provider is: '+providergwsrc)

	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#    
	parameterdst = client.GetElement_pathuuid(linkgw.endpointdst)
	client.host = parameterdst['host']
	client.port = parameterdst['port']
	client.category = parameterdst['category']
	client.GetElement(parameterdst['uuid'])
	providergwdst = client.GetElement(parameterdst['uuid'])['occi.contract.profile']
	print('The gw destination provider is: '+providergwdst)
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
  
	#----------------------------------------------------gw------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#	
	""" Determine server which handle linkgw category """
	attributePUB= {
		'what': 'gw'
	}
	client = OCCIclient('127.0.0.1', '8086', 'CO-PUB', 'publication', attributePUB)
	publication = client.GetElement_pathuuid(client.Get()[0])
	client.host = publication['host']
	client.port = publication['port']
	publication = client.GetElement_pathuuid(client.GetElement(publication['uuid'])['occi.publication.why'])
  
	attribute={
		'provider_type': providergwsrc
	}
	client = OCCIclient(publication['host'], publication['port'], 'CO-NET-PAAS', 'gw', attribute)
	listInstance = client.Get()
	if len(listInstance)==0:
		gw={
			'name': linkgw.name,
			'publicaddr': '',
			'privateaddr': '',
			'ethername': 'eth0',
			'intercloudGW': linkgw.intercloudGW,
			'contract': '',
			'provider_type': providergwsrc,
			'provider_platform': '',
			'connection': '1',
			'account': linkgw.account,
			'state': '0',
        	}
		client.attributes = gw
		print('Creating the first gateway in the ' + providergwsrc + ' provider')
		linkgw.gwsrc=client.Post()
	else:
		#---------------------------Incremente the number of connection in gwsrc-------------------------------------------#
		#------------------------------------------------------------------------------------------------------------------#
		linkgw.gwsrc = listInstance[0]
		gwsrc = client.GetElement_pathuuid(listInstance[0])
		client.host = gwsrc['host']
		client.port = gwsrc['port']
		client.category = gwsrc['category']
		client.GetElement(gwsrc['uuid'])
		nbconnection = client.GetElement(gwsrc['uuid'])['occi.gw.connection']
		print('The number of connection in gateway source is: '+str(int(nbconnection)+1))
		attributes = {
			'connection': str(int(nbconnection)+1)
		}
		client.attributes = attributes
		client.Put(gwsrc['uuid'])
		print('Incrementing the number of connection in gateway source')
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#    
	attribute={
		'provider_type': providergwdst
	}
	client = OCCIclient(publication['host'], publication['port'], 'CO-NET-PAAS', 'gw', attribute)
	listInstance = client.Get()
	if len(listInstance)==0:
		gw={
			'name': linkgw.name,
			'publicaddr': '',
			'privateaddr': '',
			'ethername': 'eth0',
			'intercloudGW': linkgw.intercloudGW,
			'contract': '',
			'provider_type': providergwdst,
			'provider_platform': '',
			'connection': '1',
			'account': linkgw.account,
			'state': '0',
		}
		client.attributes = gw
		linkgw.gwdst=client.Post()
		print('Creating the first gateway in the ' + providergwdst + ' provider')

	else:
		#---------------------------Incremente the number of connection in gwdst-------------------------------------------#
		#------------------------------------------------------------------------------------------------------------------#
		linkgw.gwdst = listInstance[0]
		gwdst = client.GetElement_pathuuid(listInstance[0])
		client.host = gwdst['host']
		client.port = gwdst['port']
		client.category = gwdst['category']
		client.GetElement(gwdst['uuid'])
		nbconnection = client.GetElement(gwdst['uuid'])['occi.gw.connection']
		print('The number of connection in gateway destination is: '+str(int(nbconnection)+1))
		attributes = {
			'connection': str(int(nbconnection)+1)
		}
		client.attributes = attributes
		client.Put(gwdst['uuid'])
		print('Incrementing the number of connection in gateway destination')
	linkgw.state='1'



	return linkgw

def linkgw_retrieve(linkgw):
	"""Implement here your function"""

	return linkgw

def linkgw_update(linkgw):
	"""Implement here your function"""

	return linkgw

def linkgw_delete(linkgw):
	"""Implement here your function"""

	return linkgw

