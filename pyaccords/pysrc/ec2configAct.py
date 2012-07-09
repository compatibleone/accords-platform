#!/usr/bin/env python
# -*- coding: latin-1 -*-
# Hamid MEDJAHED (c) Prologue

import sys
import pypacksrc
srcdirectory=pypacksrc.srcpydir+"/pyaccords"
sys.path.append(srcdirectory)
from ec2configAction import *
from actionClass import *

def current(categStr):
	l=categStr.split(",")
	categoryAtr = Cec2config(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11] )
	resCateg = ec2config_current(categoryAtr)
	restResp = [str(restCateg.status),str(restCateg.message)]	restResponse = ",".join(restResp)
	return restResponse

