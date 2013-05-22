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
srcdirectory=pypacksrc.srcpydir+"/pyaccords/pygen/"
srcdirectoryc=pypacksrc.srcpydir+"/pyaccords/pysrc/"
sys.path.append(srcdirectory)
sys.path.append(srcdirectoryc)
from gwClass import *
from actionClass import *
from client import OCCIclient
""" Note:respAction is a python class to describe the occi response with the status and the message
	gw is a python class to interface the accords category :gw.
	-Attributes of this category are members of this class.
	-List of attributes:

		- name 
		- publicaddr 
		- privateaddr 
		- ethername 
		- intercloudGW 
		- contract 
		- provider_type 
		- provider_platform 
		- connection 
		- account 
		- state 
"""

def gw_start(gw):
	response=respAction("200","ok")
	"""Implement here your function"""
	print('$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$')
	print('                     Receiving action START gw                      ')

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


	attribute = {}
	client = OCCIclient(publication['host'], publication['port'], 'CO-PARSER', 'contract', attribute)

	#----------------------------------------------start Node----------------------------------------------------------#
	gwcontract = client.GetElement_pathuuid(gw.contract)
	print('Sending START to the contract of the gateway source')
	client.action(gwcontract['uuid'], 'start')
	gwcontract = client.GetElement(gwcontract['uuid'])

	#-------------------------------------------------provider gw------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	""" Determine server which handle procci provider category """
	attributePUB= {
		'what': gwcontract['occi.contract.profile']
	}

	client = OCCIclient('127.0.0.1', '8086', 'CO-PUB', 'publication', attributePUB)
	publication = client.GetElement_pathuuid(client.Get()[0])
	client.host = publication['host']
	client.port = publication['port']
	publication = client.GetElement_pathuuid(client.GetElement(publication['uuid'])['occi.publication.why'])


	attribute = {}
	client = OCCIclient(publication['host'], publication['port'], 'CO-PARSER', gwcontract['occi.contract.profile'], attribute)

	addressgw = client.GetElement(client.GetElement_pathuuid(gwcontract['occi.contract.provider'])['uuid'])
	gwprivateaddr = addressgw['occi.'+ gwcontract['occi.contract.profile'] +'.privateaddr']
	gwpublicaddr = addressgw['occi.'+ gwcontract['occi.contract.profile'] +'.publicaddr']

	print('The private address of the gateway is: '+gwprivateaddr)
	print('The public address of the gateway is: '+gwpublicaddr)

	#---------------------------------------------------gw-------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	""" Determine server which handle gw category """
	attributePUB= {
		'what': 'gw'
	}

	client = OCCIclient('127.0.0.1', '8086', 'CO-PUB', 'publication', attributePUB)
	publication = client.GetElement_pathuuid(client.Get()[0])
	client.host = publication['host']
	client.port = publication['port']
	publication = client.GetElement_pathuuid(client.GetElement(publication['uuid'])['occi.publication.why'])

	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	tempgw={
		'name':gw.name,
		'publicaddr':gw.publicaddr,
		'privateaddr':gw.privateaddr,
		'ethername':gw.ethername,
		'intercloudGW': gw.intercloudGW,
		'contract':gw.contract,
		'provider_type':gw.provider_type,
		'provider_platform':gw.provider_platform,
		'connection':gw.connection,
		'account':gw.account,
		'state':gw.state,
	}
	attribute = {
		'publicaddr': gwpublicaddr,
		'privateaddr': gwprivateaddr,
		'state': '1',
	}
	gwOCCI = OCCIclient(publication['host'], publication['port'], 'CO-PARSER', 'gw', tempgw)

	uuidgw=gwOCCI.Get()
	gwOCCI.attributes=attribute
	gwOCCI.Put(gwOCCI.GetElement_pathuuid(uuidgw[0])['uuid'])
	print('updating the gateway category with the public and private address')


	return response

def gw_stop(gw):
	response=respAction("200","ok")
	"""Implement here your function"""
	print('$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$')
	print('                     Receiving action STOP gw                       ')

	#---------------------------------------------------gw-------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	""" Determine server which handle gw category """
	attributePUB= {
		'what': 'gw'
	}

	client = OCCIclient('127.0.0.1', '8086', 'CO-PUB', 'publication', attributePUB)
	publication = client.GetElement_pathuuid(client.Get()[0])
	client.host = publication['host']
	client.port = publication['port']
	publication = client.GetElement_pathuuid(client.GetElement(publication['uuid'])['occi.publication.why'])
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#

	tempgw={
		'name':gw.name,
		'publicaddr':gw.publicaddr,
		'privateaddr':gw.privateaddr,
		'ethername':gw.ethername,
		'intercloudGW': gw.intercloudGW,
		'contract':gw.contract,
		'provider_type':gw.provider_type,
		'provider_platform':gw.provider_platform,
		'connection':gw.connection,
		'account':gw.account,
		'state':gw.state,
	}

	gwOCCI = OCCIclient(publication['host'], publication['port'], 'CO-PARSER', 'gw', tempgw)
	uuidgw=gwOCCI.Get()
	uuidgw=gwOCCI.GetElement_pathuuid(uuidgw[0])['uuid']
	nbconnection = gwOCCI.GetElement(uuidgw)['occi.gw.connection']

	if int(nbconnection) > 0:
		attributes = {
			'connection': str(int(nbconnection)-1)
		}
		gwOCCI.attributes = attributes
		gwOCCI.Put(uuidgw)
		print('decrement the number of connection to :'+attributes['connection'])


	if int(nbconnection) == 1:
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


		attribute = {}
		client = OCCIclient(publication['host'], publication['port'], 'CO-PARSER', 'contract', attribute)

		#----------------------------------------------stop Node----------------------------------------------------------#
		gwcontract = client.GetElement_pathuuid(gw.contract)
		print('the number of connection = 0 ==> stopping the gateway  ')

		client.action(gwcontract['uuid'], 'stop')
		gwcontract = client.GetElement(gwcontract['uuid'])


		attribute = {
			'publicaddr': '',
			'privateaddr': '',
			'state': '0',
		}

		gwOCCI.attributes=attribute
		gwOCCI.Put(uuidgw)
		print('Changing the state of the gateway to 0')

	return response

