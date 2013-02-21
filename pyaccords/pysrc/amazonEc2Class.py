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

class CamazonEc2:
	"""Class to define the amazonEc2 category structure"""
	def __init__( self,Id, name,flavor,image,original,profile,node,price,account,number,rootpass,reference,network,access,accessip,keypair,placementgroup,publicaddr,privateaddr,firewall,group,zone,hostname,workload,agent,when,state ):
		"""Constructor of the class"""
		self.Id = Id
		self.name = name
		self.flavor = flavor
		self.image = image
		self.original = original
		self.profile = profile
		self.node = node
		self.price = price
		self.account = account
		self.number = number
		self.rootpass = rootpass
		self.reference = reference
		self.network = network
		self.access = access
		self.accessip = accessip
		self.keypair = keypair
		self.placementgroup = placementgroup
		self.publicaddr = publicaddr
		self.privateaddr = privateaddr
		self.firewall = firewall
		self.group = group
		self.zone = zone
		self.hostname = hostname
		self.workload = workload
                self.agent = agent
		self.when = when
		self.state = state

