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
srcdirectory=pypacksrc.srcpydir+"/pyaccords"
sys.path.append(srcdirectory)
from intercloudGWInterface import *

def create(categStr):
	l=categStr.split(",")
	categInterf = CintercloudGW(l[0],l[1],l[2],l[3],l[4] )
	resCateg = intercloudGW_create(categInterf)
	categStrR = [ str(resCateg.name),str(resCateg.node),str(resCateg.account),str(resCateg.price),str(resCateg.state) ]
	categStrNew = ",".join(categStrR)
	return categStrNew

def retrieve(categStr):
	l=categStr.split(",")
	categInterf = CintercloudGW(l[0],l[1],l[2],l[3],l[4] )
	resCateg = intercloudGW_retrieve(categInterf)
	categStrR = [ str(resCateg.name),str(resCateg.node),str(resCateg.account),str(resCateg.price),str(resCateg.state) ]
	categStrNew = ",".join(categStrR)
	return categStrNew

def update(categStr):
	l=categStr.split(",")
	categInterf = CintercloudGW(l[0],l[1],l[2],l[3],l[4] )
	resCateg = intercloudGW_update(categInterf)
	categStrR = [ str(resCateg.name),str(resCateg.node),str(resCateg.account),str(resCateg.price),str(resCateg.state) ]
	categStrNew = ",".join(categStrR)
	return categStrNew

def delete(categStr):
	l=categStr.split(",")
	categInterf = CintercloudGW(l[0],l[1],l[2],l[3],l[4] )
	resCateg = intercloudGW_delete(categInterf)
	categStrR = [ str(resCateg.name),str(resCateg.node),str(resCateg.account),str(resCateg.price),str(resCateg.state) ]
	categStrNew = ",".join(categStrR)
	return categStrNew

