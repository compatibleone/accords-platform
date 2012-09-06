###############################################################################
# copyright 2012, Hamid MEDJAHED (hmedjahed@prologue.fr) Prologue             #
# Licensed under the Apache License, Version 2.0 (the "License");           #
# you may not use this file except in compliance with the License.            #
# You may obtain a copy of the License at                                     #
#                                                                             #
#       http://www.apache.org/licenses/LICENSE-2.0                            #
#                                                                             #
# Unless required by applicable law or agreed to in writing, software         #
# distributed under the License is distributed on an "AS IS" BASIS,         #
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.    #
# See the License for the specific language governing permissions and         #
# limitations under the License.                                              #
###############################################################################
#!/usr/bin/env python
# -*- coding: latin-1 -*-
class Clinkgw:
	"""Class to define the linkgw category structure"""
	def __init__( self,name,intercloudGW,account,gwsrc,gwdst,tunnelproto,addressgresrc,addressgredst,prefix,authenticationkey,endpointsrc,endpointdst,state ):
		"""Constructor of the class"""
		self.name = name
		self.intercloudGW = intercloudGW
		self.account = account
		self.gwsrc = gwsrc
		self.gwdst = gwdst
		self.tunnelproto = tunnelproto
		self.addressgresrc = addressgresrc
		self.addressgredst = addressgredst
		self.prefix = prefix
		self.authenticationkey = authenticationkey
		self.endpointsrc = endpointsrc
		self.endpointdst = endpointdst
		self.state = state

