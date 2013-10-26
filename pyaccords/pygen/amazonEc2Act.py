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

import sys
import pypacksrc
srcdirectory=pypacksrc.srcpydir+"/pyaccords/pygen"
sys.path.append(srcdirectory)
from amazonEc2Action import *
from actionClass import *

def start(accesskey,secretkey,zone,keypair,categStr):
	l=categStr.split(",")
	categoryAtr = CamazonEc2(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],l[12],l[13],l[14],l[15],l[16],l[17],l[18],l[19],l[20],l[21],l[22],l[23],l[24],l[25],l[26],l[27],l[28])
	resCateg = amazonEc2_start(accesskey,secretkey,zone,keypair,categoryAtr)
	categStrR = [ str(resCateg.Id),str(resCateg.name),str(resCateg.flavor),str(resCateg.image),str(resCateg.original),str(resCateg.profile),str(resCateg.node),str(resCateg.price),str(resCateg.account),str(resCateg.number),str(resCateg.rootpass),str(resCateg.reference),str(resCateg.network),str(resCateg.access),str(resCateg.accessip),str(resCateg.keypair),str(resCateg.placementgroup),str(resCateg.publicaddr),str(resCateg.privateaddr),str(resCateg.firewall),str(resCateg.group),str(resCateg.zone),str(resCateg.hostname),str(resCateg.workload),str(resCateg.agent),str(resCateg.when),str(resCateg.state),str(resCateg.keyfile),str(resCateg.agentstatus) ]
	categStrNew = ",".join(categStrR)
	return categStrNew
	

def stop(accesskey,secretkey,zone,categStr):
	l=categStr.split(",")
	categoryAtr = CamazonEc2(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],l[12],l[13],l[14],l[15],l[16],l[17],l[18],l[19],l[20],l[21],l[22],l[23],l[24],l[25],l[26],l[27],l[28])
	resCateg = amazonEc2_stop(accesskey,secretkey,zone,categoryAtr)
	categStrR = [ str(resCateg.Id),str(resCateg.name),str(resCateg.flavor),str(resCateg.image),str(resCateg.original),str(resCateg.profile),str(resCateg.node),str(resCateg.price),str(resCateg.account),str(resCateg.number),str(resCateg.rootpass),str(resCateg.reference),str(resCateg.network),str(resCateg.access),str(resCateg.accessip),str(resCateg.keypair),str(resCateg.placementgroup),str(resCateg.publicaddr),str(resCateg.privateaddr),str(resCateg.firewall),str(resCateg.group),str(resCateg.zone),str(resCateg.hostname),str(resCateg.workload),str(resCateg.agent),str(resCateg.when),str(resCateg.state),str(resCateg.keyfile),str(resCateg.agentstatus) ]
	categStrNew = ",".join(categStrR)
	return categStrNew

def restart(accesskey,secretkey,zone,categStr):
	l=categStr.split(",")
	categoryAtr = CamazonEc2(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],l[12],l[13],l[14],l[15],l[16],l[17],l[18],l[19],l[20],l[21],l[22],l[23],l[24],l[25],l[26],l[27],l[28])
	resCateg = amazonEc2_restart(accesskey,secretkey,zone,categoryAtr)
	categStrR = [ str(resCateg.Id),str(resCateg.name),str(resCateg.flavor),str(resCateg.image),str(resCateg.original),str(resCateg.profile),str(resCateg.node),str(resCateg.price),str(resCateg.account),str(resCateg.number),str(resCateg.rootpass),str(resCateg.reference),str(resCateg.network),str(resCateg.access),str(resCateg.accessip),str(resCateg.keypair),str(resCateg.placementgroup),str(resCateg.publicaddr),str(resCateg.privateaddr),str(resCateg.firewall),str(resCateg.group),str(resCateg.zone),str(resCateg.hostname),str(resCateg.workload),str(resCateg.agent),str(resCateg.when),str(resCateg.state),str(resCateg.keyfile),str(resCateg.agentstatus) ]
	categStrNew = ",".join(categStrR)
	return categStrNew




def snapshot(accesskey,secretkey,zone,imgname,categStr):
	l=categStr.split(",")
	categoryAtr = CamazonEc2(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],l[12],l[13],l[14],l[15],l[16],l[17],l[18],l[19],l[20],l[21],l[22],l[23],l[24],l[25],l[26],l[27],l[28] )
	resCateg = amazonEc2_snapshot(accesskey,secretkey,zone,imgname,categoryAtr)
	categStrR = [ str(resCateg.Id),str(resCateg.name),str(resCateg.flavor),str(resCateg.image),str(resCateg.original),str(resCateg.profile),str(resCateg.node),str(resCateg.price),str(resCateg.account),str(resCateg.number),str(resCateg.rootpass),str(resCateg.reference),str(resCateg.network),str(resCateg.access),str(resCateg.accessip),str(resCateg.keypair),str(resCateg.placementgroup),str(resCateg.publicaddr),str(resCateg.privateaddr),str(resCateg.firewall),str(resCateg.group),str(resCateg.zone),str(resCateg.hostname),str(resCateg.workload),str(resCateg.agent),str(resCateg.when),str(resCateg.state),str(resCateg.keyfile),str(resCateg.agentstatus) ]
	categStrNew = ",".join(categStrR)
	return categStrNew

def suspend(accesskey,secretkey,zone,categStr):
	l=categStr.split(",")
	categoryAtr = CamazonEc2(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],l[12],l[13],l[14],l[15],l[16],l[17],l[18],l[19],l[20],l[21],l[22],l[23],l[24],l[25],l[26],l[27],l[28])
	resCateg = amazonEc2_suspend(accesskey,secretkey,zone,categoryAtr)
	categStrR = [ str(resCateg.Id),str(resCateg.name),str(resCateg.flavor),str(resCateg.image),str(resCateg.original),str(resCateg.profile),str(resCateg.node),str(resCateg.price),str(resCateg.account),str(resCateg.number),str(resCateg.rootpass),str(resCateg.reference),str(resCateg.network),str(resCateg.access),str(resCateg.accessip),str(resCateg.keypair),str(resCateg.placementgroup),str(resCateg.publicaddr),str(resCateg.privateaddr),str(resCateg.firewall),str(resCateg.group),str(resCateg.zone),str(resCateg.hostname),str(resCateg.workload),str(resCateg.agent),str(resCateg.when),str(resCateg.state),str(resCateg.keyfile),str(resCateg.agentstatus)]
	categStrNew = ",".join(categStrR)
	return categStrNew


