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
# Implementation of category actions
import sys
import pycompdev
import pypacksrc
srcdirectory=pypacksrc.srcpydir+"/pyaccords/pysrc/"
srcdirectoryc=pypacksrc.srcpydir+"/cocarrier/src/"
sys.path.append(srcdirectory)
sys.path.append(srcdirectoryc)
from intercloudGWClass import *
from actionClass import *
from client import OCCIclient
""" Note:respAction is a python class to describe the occi response with the status and the message
	intercloudGW is a python class to interface the accords category :intercloudGW.
	-Attributes of this category are members of this class.
	-List of attributes:

		- name 
		- node 
		- account 
		- price 
		- state 
"""

def intercloudGW_start(intercloudGW):
	response=respAction("200","ok")
	"""Implement here your function"""
	print('$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$')
	print('                Receiving action START intercloudGW                 ')


	#---------------------------------------------------intercloudGW---------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	""" Determine server which handle intercloudGW category """
	attributePUB= {
		'what': 'intercloudGW'
	}

	client = OCCIclient('127.0.0.1', '8086', 'CO-PUB', 'publication', attributePUB)
	publication = client.GetElement_pathuuid(client.Get()[0])
	client.host = publication['host']
	client.port = publication['port']
	publication = client.GetElement_pathuuid(client.GetElement(publication['uuid'])['occi.publication.why'])

	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#

	tempintercloudGW={
		'name':intercloudGW.name,
		'node':intercloudGW.node,
		'account':intercloudGW.account,
		'price': intercloudGW.price,
		'state':intercloudGW.state,
	}

	gwOCCI = OCCIclient(publication['host'], publication['port'], 'CO-NETS-PAAS', 'intercloudGW', tempintercloudGW)

	uuidintercloudGW=gwOCCI.Get()[0].replace(' ','')
	
	#-------------------------------------------------contract---------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	""" Determine server which handle contract category """
	attributePUB= {
		'what': 'contract'
	}

	client = OCCIclient('127.0.0.1', '8086', 'CO-PUB', 'publication', attributePUB)
	publication = client.GetElement_pathuuid(client.Get()[0])
	client.host = publication['host']
	client.port = publication['port']
	publication = client.GetElement_pathuuid(client.GetElement(publication['uuid'])['occi.publication.why'])


	attribute = {
		'provider': uuidintercloudGW,
		'profile': 'intercloudGW',
		'node':intercloudGW.node,
	} 
	client = OCCIclient(publication['host'], publication['port'], 'CO-PARSER', 'contract', attribute)
	listinstruction=client.GetLinks(client.GetElement_pathuuid(client.Get()[0])['uuid'])
	#print listinstruction


	#------------------------------------------------Instruction-------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#    
	""" Determine server which handle instruction category """
	attributePUB= {
		'what': 'instruction'
	}

	client = OCCIclient('127.0.0.1', '8086', 'CO-PUB', 'publication', attributePUB)
	publication = client.GetElement_pathuuid(client.Get()[0])
	client.host = publication['host']
	client.port = publication['port']
	publication = client.GetElement_pathuuid(client.GetElement(publication['uuid'])['occi.publication.why'])

	attribute = {}
	client = OCCIclient(publication['host'], publication['port'], 'CO-PARSER', 'instruction', attribute)

	listNodeToConnect = []
	for item in listinstruction:
		nodetoConnect = client.GetElement_pathuuid(item)
		client.host = nodetoConnect['host']
		client.port = nodetoConnect['port']
		client.category = nodetoConnect['category']
		listNodeToConnect.append(client.GetElement(nodetoConnect['uuid'])['occi.instruction.source'])


	listlinkgw = []
	for i in range(len(listNodeToConnect)):
		for j in range(i+1,len(listNodeToConnect)):
			listlinkgw.append( {
				'name': intercloudGW.name,
				'intercloudGW': intercloudGW.node,
				'account': intercloudGW.account,
				'gwsrc': '',
				'gwdst': '',
				'tunnelproto': 'gre',
				'addressgresrc': '10.3.3.1',
				'addressgredst': '10.3.3.2',
				'prefix': '30',
				'authenticationkey': 'test_key_1',
				'endpointsrc': listNodeToConnect[i],
				'endpointdst': listNodeToConnect[j],
				'state': '0',
			})



	#------------------------------------------------linkgw------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#	
	""" Determine server which handle linkgw category """
	attributePUB= {
		'what': 'linkgw'
	}
	client = OCCIclient('127.0.0.1', '8086', 'CO-PUB', 'publication', attributePUB)
	publication = client.GetElement_pathuuid(client.Get()[0])
	client.host = publication['host']
	client.port = publication['port']
	publication = client.GetElement_pathuuid(client.GetElement(publication['uuid'])['occi.publication.why'])
	

	listuuidlinkgw = []
	for i in listlinkgw:
		conetsOCCI = OCCIclient(publication['host'], publication['port'], 'CO-NETS-PAAS', 'linkgw', i)
		uuidlinkgw = conetsOCCI.GetElement_pathuuid(conetsOCCI.Post())['uuid']
		listuuidlinkgw.append(uuidlinkgw)
		print('Sending Post to linkgw')

	for item in listuuidlinkgw:
		print('Sending START to linkgw')
		conetsOCCI.action(item, 'start')

	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	tempintercloudGW={
		'name':intercloudGW.name,
		'node':intercloudGW.node,
		'account':intercloudGW.account,
		'price':intercloudGW.price,
		'state': intercloudGW.state,
	}
	attribute = {
		'state': '1',
	}
	conetsOCCI.category = 'intercloudGW'
        conetsOCCI.attributes = tempintercloudGW
        
	uuidintercloudGW=conetsOCCI.Get()
	conetsOCCI.attributes=attribute
	conetsOCCI.Put(conetsOCCI.GetElement_pathuuid(uuidintercloudGW[0])['uuid'])
	print('Changing the state of the intercloudGW to 1')

	return response

def intercloudGW_stop(intercloudGW):
	response=respAction("200","ok")
	"""Implement here your function"""
	print('$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$')
	print('                Receiving action STOP intercloudGW                  ')
	#------------------------------------------------linkgw------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#	
	""" Determine server which handle linkgw category """
	attributePUB= {
		'what': 'linkgw'
	}
	client = OCCIclient('127.0.0.1', '8086', 'CO-PUB', 'publication', attributePUB)
	publication = client.GetElement_pathuuid(client.Get()[0])
	client.host = publication['host']
	client.port = publication['port']
	publication = client.GetElement_pathuuid(client.GetElement(publication['uuid'])['occi.publication.why'])
	

	attribute = {
		'intercloudGW': intercloudGW.node,
		'state': '1',
	}
	conetsOCCI = OCCIclient(publication['host'], publication['port'], 'CO-NETS-PAAS', 'linkgw', attribute)
	listInstance = conetsOCCI.Get()

	for item in listInstance:
        	linkgws = conetsOCCI.GetElement_pathuuid(item)
        	conetsOCCI.host = linkgws['host']
        	conetsOCCI.port = linkgws['port']
        	conetsOCCI.category = linkgws['category']
        	linkgws = conetsOCCI.GetElement(linkgws['uuid'])
		print('Sending STOP to linkgw')
		conetsOCCI.action(linkgws['occi.core.id'], 'stop')

	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	tempintercloudGW={
		'name':intercloudGW.name,
		'node':intercloudGW.node,
		'account':intercloudGW.account,
		'price':intercloudGW.price,
		'state': intercloudGW.state,
	}
	attribute = {
		'state': '0',
	}
	conetsOCCI.category = 'intercloudGW'
        conetsOCCI.attributes = tempintercloudGW
        
	uuidintercloudGW=conetsOCCI.Get()
	conetsOCCI.attributes=attribute
	conetsOCCI.Put(conetsOCCI.GetElement_pathuuid(uuidintercloudGW[0])['uuid'])
	print('Changing the state of the intercloudGW to 0')

	return response

