#!/usr/bin/env python
# -*- coding: latin-1 -*-
# Hamid MEDJAHED (c) Prologue

import sys
import pypacksrc
srcdirectory=pypacksrc.srcpydir+"/pyaccords"
sys.path.append(srcdirectory)
from amazonEc2Action import *
from actionClass import *

def start(categStr):
	
	l=categStr.split(",")
	categoryAtr = CamazonEc2(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],l[12],l[13],l[14],l[15],l[16],l[17],l[18],l[19],l[20],l[21],l[22],l[23],l[24] )
        
	restCateg = amazonEc2_start(categoryAtr)
		
	restResp = [str(restCateg.status),str(restCateg.message)]	
	restResponse = ",".join(restResp)
	
	return restResponse

def save(categStr):
	l=categStr.split(",")
	categoryAtr = CamazonEc2(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],l[12],l[13],l[14],l[15],l[16],l[17],l[18],l[19],l[20],l[21],l[22],l[23],l[24] )
	restCateg = amazonEc2_save(categoryAtr)
	restResp = [str(restCateg.status),str(restCateg.message)]	
	restResponse = ",".join(restResp)
	return restResponse

def snapshot(categStr):
	l=categStr.split(",")
	categoryAtr = CamazonEc2(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],l[12],l[13],l[14],l[15],l[16],l[17],l[18],l[19],l[20],l[21],l[22],l[23],l[24] )
	restCateg = amazonEc2_snapshot(categoryAtr)
	restResp = [str(restCateg.status),str(restCateg.message)]	
	restResponse = ",".join(restResp)
	return restResponse

def stop(categStr):
	l=categStr.split(",")
	categoryAtr = CamazonEc2(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],l[12],l[13],l[14],l[15],l[16],l[17],l[18],l[19],l[20],l[21],l[22],l[23],l[24] )
	restCateg = amazonEc2_stop(categoryAtr)
	restResp = [str(restCateg.status),str(restCateg.message)]	
	restResponse = ",".join(restResp)
	return restResponse

def suspend(categStr):
	l=categStr.split(",")
	categoryAtr = CamazonEc2(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],l[12],l[13],l[14],l[15],l[16],l[17],l[18],l[19],l[20],l[21],l[22],l[23],l[24] )
	resCateg = amazonEc2_suspend(categoryAtr)
	restResp = [str(restCateg.status),str(restCateg.message)]	
	restResponse = ",".join(restResp)
	return restResponse

def restart(categStr):
	l=categStr.split(",")
	categoryAtr = CamazonEc2(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],l[12],l[13],l[14],l[15],l[16],l[17],l[18],l[19],l[20],l[21],l[22],l[23],l[24] )
	resCateg = amazonEc2_restart(categoryAtr)
	restResp = [str(restCateg.status),str(restCateg.message)]	
	restResponse = ",".join(restResp)
	return restResponse

