#!/usr/bin/env python
# -*- coding: latin-1 -*-
# Hamid MEDJAHED (c) Prologue
import sys
import pypacksrc
srcdirectory=pypacksrc.srcpydir+"/pyaccords"
sys.path.append(srcdirectory)
from amazonEc2Interface import *

def create(categStr):
	l=categStr.split(",")
	categInterf = CamazonEc2(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],l[12],l[13],l[14],l[15],l[16],l[17],l[18],l[19],l[20],l[21],l[22],l[23],l[24] )
	resCateg = amazonEc2_create(categInterf)
	categStrR = [ str(resCateg.name),str(resCateg.flavor),str(resCateg.image),str(resCateg.original),str(resCateg.profile),str(resCateg.node),str(resCateg.price),str(resCateg.account),str(resCateg.number),str(resCateg.rootpass),str(resCateg.reference),str(resCateg.network),str(resCateg.access),str(resCateg.accessip),str(resCateg.floating),str(resCateg.floatingid),str(resCateg.publicaddr),str(resCateg.privateaddr),str(resCateg.firewall),str(resCateg.group),str(resCateg.zone),str(resCateg.hostname),str(resCateg.workload),str(resCateg.when),str(resCateg.state) ]
	categStrNew = ",".join(categStrR)
	return categStrNew

def retrieve(categStr):
	l=categStr.split(",")
	categInterf = CamazonEc2(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],l[12],l[13],l[14],l[15],l[16],l[17],l[18],l[19],l[20],l[21],l[22],l[23],l[24] )
	resCateg = amazonEc2_retrieve(categInterf)
	categStrR = [ str(resCateg.name),str(resCateg.flavor),str(resCateg.image),str(resCateg.original),str(resCateg.profile),str(resCateg.node),str(resCateg.price),str(resCateg.account),str(resCateg.number),str(resCateg.rootpass),str(resCateg.reference),str(resCateg.network),str(resCateg.access),str(resCateg.accessip),str(resCateg.floating),str(resCateg.floatingid),str(resCateg.publicaddr),str(resCateg.privateaddr),str(resCateg.firewall),str(resCateg.group),str(resCateg.zone),str(resCateg.hostname),str(resCateg.workload),str(resCateg.when),str(resCateg.state) ]
	categStrNew = ",".join(categStrR)
	return categStrNew

def update(categStr):
	l=categStr.split(",")
	categInterf = CamazonEc2(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],l[12],l[13],l[14],l[15],l[16],l[17],l[18],l[19],l[20],l[21],l[22],l[23],l[24] )
	resCateg = amazonEc2_update(categInterf)
	categStrR = [ str(resCateg.name),str(resCateg.flavor),str(resCateg.image),str(resCateg.original),str(resCateg.profile),str(resCateg.node),str(resCateg.price),str(resCateg.account),str(resCateg.number),str(resCateg.rootpass),str(resCateg.reference),str(resCateg.network),str(resCateg.access),str(resCateg.accessip),str(resCateg.floating),str(resCateg.floatingid),str(resCateg.publicaddr),str(resCateg.privateaddr),str(resCateg.firewall),str(resCateg.group),str(resCateg.zone),str(resCateg.hostname),str(resCateg.workload),str(resCateg.when),str(resCateg.state) ]
	categStrNew = ",".join(categStrR)
	return categStrNew

def delete(categStr):
	l=categStr.split(",")
	categInterf = CamazonEc2(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],l[12],l[13],l[14],l[15],l[16],l[17],l[18],l[19],l[20],l[21],l[22],l[23],l[24] )
	resCateg = amazonEc2_delete(categInterf)
	categStrR = [ str(resCateg.name),str(resCateg.flavor),str(resCateg.image),str(resCateg.original),str(resCateg.profile),str(resCateg.node),str(resCateg.price),str(resCateg.account),str(resCateg.number),str(resCateg.rootpass),str(resCateg.reference),str(resCateg.network),str(resCateg.access),str(resCateg.accessip),str(resCateg.floating),str(resCateg.floatingid),str(resCateg.publicaddr),str(resCateg.privateaddr),str(resCateg.firewall),str(resCateg.group),str(resCateg.zone),str(resCateg.hostname),str(resCateg.workload),str(resCateg.when),str(resCateg.state) ]
	categStrNew = ",".join(categStrR)
	return categStrNew

