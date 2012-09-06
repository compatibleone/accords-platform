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
from linkgwClass import *
from actionClass import *
from client import OCCIclient
from Proccigw import *
""" Note:respAction is a python class to describe the occi response with the status and the message
	linkgw is a python class to interface the accords category :linkgw.
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

def linkgw_start(linkgw):
	response=respAction("200","ok")
	"""Implement here your function"""
	print('$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$')
	print('                 Receiving action START linkgw                      ')


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
	attribute = {}
	conetsOCCI = OCCIclient(publication['host'], publication['port'], 'CO-NETS-PAAS', 'gw', attribute)


	#--------------------------------------------uuid gw src-----------------------------------------------------------#
	gwsrc = conetsOCCI.GetElement_pathuuid(linkgw.gwsrc)
	addressgwsrc = conetsOCCI.GetElement(gwsrc['uuid'])
	publicaddrgwsrc = addressgwsrc['occi.gw.publicaddr']
	privateaddrgwsrc = addressgwsrc['occi.gw.privateaddr']
	ethernamegwsrc = addressgwsrc['occi.gw.ethername']

	#--------------------------------------------uuid gw dst-----------------------------------------------------------#
	gwdst = conetsOCCI.GetElement_pathuuid(linkgw.gwdst)
	addressgwdst = conetsOCCI.GetElement(gwdst['uuid'])
	publicaddrgwdst = addressgwdst['occi.gw.publicaddr']
	privateaddrgwdst = addressgwdst['occi.gw.privateaddr']
	ethernamegwdst = addressgwdst['occi.gw.ethername']
	#-----------------------------start gateway source and gateway destination-----------------------------------------#

	print('Sending START to gateway source')
	conetsOCCI.action(gwsrc['uuid'], 'start')
	print('Sending START to gateway destination')
	conetsOCCI.action(gwdst['uuid'], 'start')



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

	endpointsrc = client.GetElement(client.GetElement_pathuuid(linkgw.endpointsrc)['uuid'])
	endpointdst = client.GetElement(client.GetElement_pathuuid(linkgw.endpointdst)['uuid'])


	#-------------------------------------------------provider gwsrc---------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	""" Determine server which handle procci provider category """
	attributePUB= {
		'what': endpointsrc['occi.contract.profile']
	}

	client = OCCIclient('127.0.0.1', '8086', 'CO-PUB', 'publication', attributePUB)
	publication = client.GetElement_pathuuid(client.Get()[0])
	client.host = publication['host']
	client.port = publication['port']
	publication = client.GetElement_pathuuid(client.GetElement(publication['uuid'])['occi.publication.why'])


	attribute = {}
	client = OCCIclient(publication['host'], publication['port'], 'CO-PARSER', endpointsrc['occi.contract.profile'], attribute)

	#print endpointsrc['occi.contract.provider']
	privateaddrvmsrc = client.GetElement(client.GetElement_pathuuid(endpointsrc['occi.contract.provider'])['uuid'])
	privateaddrvmsrc = privateaddrvmsrc['occi.'+ endpointsrc['occi.contract.profile'] +'.privateaddr']

	#-------------------------------------------------provider gwdst---------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	""" Determine server which handle procci provider category """
	attributePUB= {
		'what': endpointdst['occi.contract.profile']
	}

	client = OCCIclient('127.0.0.1', '8086', 'CO-PUB', 'publication', attributePUB)
	publication = client.GetElement_pathuuid(client.Get()[0])
	client.host = publication['host']
	client.port = publication['port']
	publication = client.GetElement_pathuuid(client.GetElement(publication['uuid'])['occi.publication.why'])


	attribute = {}
	client = OCCIclient(publication['host'], publication['port'], 'CO-PARSER', endpointdst['occi.contract.profile'], attribute)

	#print endpointdst['occi.contract.provider']
	privateaddrvmdst = client.GetElement(client.GetElement_pathuuid(endpointdst['occi.contract.provider'])['uuid'])
	privateaddrvmdst = privateaddrvmdst['occi.'+ endpointdst['occi.contract.profile'] +'.privateaddr']




	gwsrc = gwProcci()
	gwdst = gwProcci()

	templist = privateaddrvmsrc.split('.')
	templist[3] = '0/24'
	privateaddrvmsrc = '.'.join(templist)
	#print privateaddrvmsrc

	templist = privateaddrvmdst.split('.')
	templist[3] = '0/24'
	privateaddrvmdst = '.'.join(templist)
	#print privateaddrvmdst

	''' Configuration des gateways'''
	gwsrc.configure_protocol_IPSEC(gwsrc, publicaddrgwsrc, ethernamegwsrc, 'IKEgwsrc', 'ESPgwsrc')
	gwdst.configure_protocol_IPSEC(gwdst, publicaddrgwdst, ethernamegwdst, 'IKEgwdst', 'ESPgwdst')

	''' Connecte les deux gateways'''
	gwsrc.connect_gw(gwsrc, linkgw.addressgresrc, linkgw.prefix, linkgw.tunnelproto, publicaddrgwsrc, publicaddrgwdst, linkgw.authenticationkey, linkgw.addressgredst, privateaddrvmdst)
	gwsrc.connect_gw(gwsrc, linkgw.addressgredst, linkgw.prefix, linkgw.tunnelproto, publicaddrgwdst, publicaddrgwsrc, linkgw.authenticationkey, linkgw.addressgresrc, privateaddrvmsrc)


	return response

def linkgw_stop(linkgw):
	response=respAction("200","ok")
	"""Implement here your function"""
	print('$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$')
	print('                   Receiving action STOP linkgw                     ')

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
	attribute = {}
	gwOCCI = OCCIclient(publication['host'], publication['port'], 'CO-PARSER', 'gw', attribute)

	#--------------------------------------------uuid gw src-----------------------------------------------------------#
	gwsrc = gwOCCI.GetElement_pathuuid(linkgw.gwsrc)
	#--------------------------------------------uuid gw dst-----------------------------------------------------------#
	gwdst = gwOCCI.GetElement_pathuuid(linkgw.gwdst)
	#------------------------------stop gateway source and gateway destination-----------------------------------------#
	print('Sending STOP to gateway source')
	gwOCCI.action(gwsrc['uuid'], 'stop')
	print('Sending STOP to gateway destination')
	gwOCCI.action(gwdst['uuid'], 'stop')
	

	templinkgw={
		'name':linkgw.name,
		'intercloudGW':linkgw.intercloudGW,
		'account':linkgw.account,
		'gwsrc':linkgw.gwsrc,
		'gwdst': linkgw.gwdst,
		'tunnelproto':linkgw.tunnelproto,
		'addressgresrc':linkgw.addressgresrc,
		'addressgredst':linkgw.addressgredst,
		'prefix':linkgw.prefix,
		'authenticationkey':linkgw.authenticationkey,
		'endpointsrc':linkgw.endpointsrc,
		'endpointdst':linkgw.endpointdst,
		'state':linkgw.state,
	}

	gwOCCI.category = 'linkgw'
	gwOCCI.attributes=templinkgw

	uuidlinkgw=gwOCCI.Get()
	uuidlinkgw=gwOCCI.GetElement_pathuuid(uuidlinkgw[0])['uuid']

	attribute = {
		'state': '0',
	}

	gwOCCI.attributes=attribute
	gwOCCI.Put(uuidlinkgw)
	print('Changing the state of the link gateway to 0')



	return response

