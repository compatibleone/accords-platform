###############################################################################
# copyright 2012, Hamid MEDJAHED    (hmedjahed@prologue.fr) Prologue          #
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
import sys
import pypacksrc
srcdirectory=pypacksrc.srcpydir+"/pyaccords/pygen"
sys.path.append(srcdirectory)
from gwInterface import *

def create(categStr):
	l=categStr.split(",")
	categInterf = Cgw(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10] )
	resCateg = gw_create(categInterf)
	categStrR = [ str(resCateg.name),str(resCateg.publicaddr),str(resCateg.privateaddr),str(resCateg.ethername),str(resCateg.intercloudGW),str(resCateg.contract),str(resCateg.provider_type),str(resCateg.provider_platform),str(resCateg.connection),str(resCateg.account),str(resCateg.state) ]
	categStrNew = ",".join(categStrR)
	return categStrNew

def retrieve(categStr):
	l=categStr.split(",")
	categInterf = Cgw(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10] )
	resCateg = gw_retrieve(categInterf)
	categStrR = [ str(resCateg.name),str(resCateg.publicaddr),str(resCateg.privateaddr),str(resCateg.ethername),str(resCateg.intercloudGW),str(resCateg.contract),str(resCateg.provider_type),str(resCateg.provider_platform),str(resCateg.connection),str(resCateg.account),str(resCateg.state) ]
	categStrNew = ",".join(categStrR)
	return categStrNew

def update(categStr):
	l=categStr.split(",")
	categInterf = Cgw(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10] )
	resCateg = gw_update(categInterf)
	categStrR = [ str(resCateg.name),str(resCateg.publicaddr),str(resCateg.privateaddr),str(resCateg.ethername),str(resCateg.intercloudGW),str(resCateg.contract),str(resCateg.provider_type),str(resCateg.provider_platform),str(resCateg.connection),str(resCateg.account),str(resCateg.state) ]
	categStrNew = ",".join(categStrR)
	return categStrNew

def delete(categStr):
	l=categStr.split(",")
	categInterf = Cgw(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10] )
	resCateg = gw_delete(categInterf)
	categStrR = [ str(resCateg.name),str(resCateg.publicaddr),str(resCateg.privateaddr),str(resCateg.ethername),str(resCateg.intercloudGW),str(resCateg.contract),str(resCateg.provider_type),str(resCateg.provider_platform),str(resCateg.connection),str(resCateg.account),str(resCateg.state) ]
	categStrNew = ",".join(categStrR)
	return categStrNew

