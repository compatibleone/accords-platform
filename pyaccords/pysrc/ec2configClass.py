##############################################################################
#copyright 2012, Hamid MEDJAHED  (hmedjahed@prologue.fr) Prologue            #
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

class Cec2config:
	"""Class to define the ec2config category structure"""
	def __init__( self,Id,name,description,user,password,accesskey,secretkey,authenticate,agent,host,version,location,namespace,base,tls,current):
		"""Constructor of the class"""
		self.Id = Id
		self.name = name
		self.description = description
		self.user = user
		self.password = password
		self.accesskey = accesskey
		self.secretkey = secretkey
		self.authenticate = authenticate
		self.agent = agent
		self.host = host
		self.version = version
		self.location = location
		self.namespace = namespace
		self.base = base
		self.tls = tls
		self.current = current

