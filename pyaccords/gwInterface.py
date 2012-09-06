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
from gwClass import *
from client import OCCIclient
""" Note: gw is a python class to interface the accords category :gw.
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

def gw_create(gw):
	"""Implement here your function"""
	"""Implement here your function"""
	print('$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$')
	print('                         Receiving POST gw                          ')

	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	attribute = {}
	client = OCCIclient('', '', 'CO-PROCCI', '', attribute)
	node = client.GetElement_pathuuid(gw.intercloudGW)
	client.host = node['host']
	client.port = node['port']
	client.category = node['category']
	node = client.GetElement(node['uuid'])
	infrastructure = node['occi.node.infrastructure']
	image = node['occi.node.image']
	firewall = node['occi.node.firewall']
	accsessnode = node['occi.node.access']
	scopenode = node['occi.node.scope']
	typenode = node['occi.node.type']

	infra = client.GetElement_pathuuid(infrastructure)
	client.host = infra['host']
	client.port = infra['port']
	client.category = infra['category']
	infra = client.GetElement(infra['uuid'])

	img = client.GetElement_pathuuid(image)
	client.host = img['host']
	client.port = img['port']
	client.category = img['category']
	img = client.GetElement(img['uuid'])

	fire = client.GetElement_pathuuid(firewall)
	client.host = fire['host']
	client.port = fire['port']
	client.category = fire['category']
	fire = client.GetElement(fire['uuid'])

	#-------------------------------------------Infrastructure---------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	compute = infra['occi.infrastructure.compute']
	compute = client.GetElement_pathuuid(compute)
	client.host = compute['host']
	client.port = compute['port']
	client.category = compute['category']
	compute = client.GetElement(compute['uuid'])
	#------------------------------------------------------------------------------------------------------------------#
	""" Determine server which handle compute category """
	attributePUB= {
		'what': 'compute'
	}
	client = OCCIclient('127.0.0.1', '8086', 'CO-PUB', 'publication', attributePUB)
	publication = client.GetElement_pathuuid(client.Get()[0])
	client.host = publication['host']
	client.port = publication['port']
	publication = client.GetElement_pathuuid(client.GetElement(publication['uuid'])['occi.publication.why'])

	archi = compute['occi.compute.architecture']
	tempdict = {
		'name': gw.name,
		'architecture': archi,
		'cores': compute['occi.compute.cores'],
		'speed': compute['occi.compute.speed'],
		'memory': compute['occi.compute.memory']
	}
	testPost = OCCIclient(publication['host'], publication['port'], 'CO-PROCCI', 'compute', tempdict)
	compute=testPost.Post()
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	network = infra['occi.infrastructure.network']
	network = client.GetElement_pathuuid(network)
	client.host = network['host']
	client.port = network['port']
	client.category = network['category']
	network = client.GetElement(network['uuid'])
	#------------------------------------------------------------------------------------------------------------------#
	""" Determine server which handle network category """
	attributePUB= {
		'what': 'network'
	}
	client = OCCIclient('127.0.0.1', '8086', 'CO-PUB', 'publication', attributePUB)
	publication = client.GetElement_pathuuid(client.Get()[0])
	client.host = publication['host']
	client.port = publication['port']
	publication = client.GetElement_pathuuid(client.GetElement(publication['uuid'])['occi.publication.why'])

	tempdict = {
		'name': network['occi.network.name'],
		'label': network['occi.network.label'],
		'vlan': network['occi.network.vlan']
	}
	testPost = OCCIclient(publication['host'], publication['port'], 'CO-NETS', 'network', tempdict)
	network=testPost.Post()
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	storage = infra['occi.infrastructure.storage']
	storage = client.GetElement_pathuuid(storage)
	client.host = storage['host']
	client.port = storage['port']
	client.category = storage['category']
	storage = client.GetElement(storage['uuid'])
	#------------------------------------------------------------------------------------------------------------------#
	""" Determine server which handle storage category """
	attributePUB= {
		'what': 'storage'
	}
	client = OCCIclient('127.0.0.1', '8086', 'CO-PUB', 'publication', attributePUB)
	publication = client.GetElement_pathuuid(client.Get()[0])
	client.host = publication['host']
	client.port = publication['port']
	publication = client.GetElement_pathuuid(client.GetElement(publication['uuid'])['occi.publication.why'])

	tempdict = {
		'name': gw.name,
		'size': storage['occi.storage.size']
	}
	testPost = OCCIclient(publication['host'], publication['port'], 'CO-PROCCI', 'storage', tempdict)
	storage=testPost.Post()
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	""" Determine server which handle infrastructure category """
	attributePUB= {
		'what': 'infrastructure'
	}
	client = OCCIclient('127.0.0.1', '8086', 'CO-PUB', 'publication', attributePUB)
	publication = client.GetElement_pathuuid(client.Get()[0])
	client.host = publication['host']
	client.port = publication['port']
	publication = client.GetElement_pathuuid(client.GetElement(publication['uuid'])['occi.publication.why'])

	tempdict = {
		'name': gw.name,
		'compute': compute,
		'network': network,
		'storage': storage
	}
	testPost = OCCIclient(publication['host'], publication['port'], 'CO-PROCCI', 'infrastructure', tempdict)
	infrastructure=testPost.Post()



	#--------------------------------------------------Image-----------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	system = img['occi.image.system']
	system = client.GetElement_pathuuid(system)
	client.host = system['host']
	client.port = system['port']
	client.category = system['category']
	system = client.GetElement(system['uuid'])
	#------------------------------------------------------------------------------------------------------------------#

	""" Determine server which handle system category """
	attributePUB= {
		'what': 'system'
	}
	client = OCCIclient('127.0.0.1', '8086', 'CO-PUB', 'publication', attributePUB)
	publication = client.GetElement_pathuuid(client.Get()[0])
	client.host = publication['host']
	client.port = publication['port']
	publication = client.GetElement_pathuuid(client.GetElement(publication['uuid'])['occi.publication.why'])


	""" Post a system category """
	tempdict = {
		'name': system['occi.system.name']
	}
	testPost = OCCIclient(publication['host'], publication['port'], 'CO-EZVM', 'system', tempdict)
	system=testPost.Post()
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	""" Determine server which handle image category """
	attributePUB= {
		'what': 'image'
	}
	client = OCCIclient('127.0.0.1', '8086', 'CO-PUB', 'publication', attributePUB)
	publication = client.GetElement_pathuuid(client.Get()[0])
	client.host = publication['host']
	client.port = publication['port']
	publication = client.GetElement_pathuuid(client.GetElement(publication['uuid'])['occi.publication.why'])


	""" Post a image category """
	tempdict = {
		'name': gw.name,
		'system': system
	}
	testPost = OCCIclient(publication['host'], publication['port'], 'CO-EZVM', 'image', tempdict)
	image=testPost.Post()


	#-------------------------------------------------Firewall---------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	""" Determine server which handle firewall category """
	attributePUB= {
		'what': 'firewall'
	}
	client = OCCIclient('127.0.0.1', '8086', 'CO-PUB', 'publication', attributePUB)
	publication = client.GetElement_pathuuid(client.Get()[0])
	client.host = publication['host']
	client.port = publication['port']
	publication = client.GetElement_pathuuid(client.GetElement(publication['uuid'])['occi.publication.why'])


	""" Post a firewall category """
	tempdict = {}
	OCCIfirewall = OCCIclient(publication['host'], publication['port'], 'CO-NETS', 'firewall', tempdict)
	firewall=OCCIfirewall.Post()


	#---------------------------------------------------node-----------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	""" Determine server which handle node category """
	attributePUB= {
		'what': 'node'
	}
	client = OCCIclient('127.0.0.1', '8086', 'CO-PUB', 'publication', attributePUB)
	publication = client.GetElement_pathuuid(client.Get()[0])
	client.host = publication['host']
	client.port = publication['port']
	publication = client.GetElement_pathuuid(client.GetElement(publication['uuid'])['occi.publication.why'])


	tempdict = {
		'name': gw.name,
		'provider': gw.provider_type,
		'infrastructure': infrastructure,
		'image': image,
		'access': accsessnode,
		'scope': scopenode,
		'type': typenode,
		'firewall': firewall
	}
	testPost = OCCIclient(publication['host'], publication['port'], 'CO-PROCCI', 'node', tempdict)
	node=testPost.Post()

	""" Update firewall """
	OCCIfirewall.attributes={'node':node}
	OCCIfirewall.Put(OCCIfirewall.GetElement_pathuuid(firewall)['uuid'])


	#------------------------------------------------Provisionning-----------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	#------------------------------------------------------------------------------------------------------------------#
	""" Determine server which handle opennebula category """
	attributePUB= {
		'what': gw.provider_type
	}
	client = OCCIclient('127.0.0.1', '8086', 'CO-PUB', 'publication', attributePUB)
	publication = client.GetElement_pathuuid(client.Get()[0])
	client.host = publication['host']
	client.port = publication['port']
	publication = client.GetElement_pathuuid(client.GetElement(publication['uuid'])['occi.publication.why'])


	profile = client.GetElement_pathuuid(gw.account)
	client.host = profile['host']
	client.port = profile['port']
	client.category = profile['category']
	profile = client.GetElement(profile['uuid'])['occi.account.name']
	tempdict = {
		'name': gw.name,
		'flavor': 'small',
		'architecture': archi,
		'node': node,
		'account': gw.account,
		'profile': profile
	}
	testPost = OCCIclient(publication['host'], publication['port'], 'CO-PROCCI', gw.provider_type, tempdict)
	provider=testPost.Post()



	#--------------------------------------------------Contract--------------------------------------------------------#
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


	tempdict = {
		'name': gw.name,
		'node': node,
		'provider': provider,
		'profile': gw.provider_type,
		'access': accsessnode,
		'scope': scopenode,
		'type': typenode
	}
	testPost = OCCIclient(publication['host'], publication['port'], 'CO-PROCCI', 'contract', tempdict)
	contract=testPost.Post()
	gw.contract = contract
	print('Creating a contract for this gateway')

	return gw

def gw_retrieve(gw):
	"""Implement here your function"""

	return gw

def gw_update(gw):
	"""Implement here your function"""

	return gw

def gw_delete(gw):
	"""Implement here your function"""

	return gw

