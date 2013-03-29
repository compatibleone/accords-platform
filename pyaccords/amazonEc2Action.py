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

# Implementation of category actions


import sys
import os
import time
import ntpath
import pycompdev
import pypacksrc
srcdirectory=pypacksrc.srcpydir+"/pyaccords/pysrc/"
sys.path.append(srcdirectory)
from ec2client import *  
from amazonEc2Class import *
""" Note:amazonEc2 is a python class to interface the accords category :amazonEc2.
	-Attributes of this category are members of this class.
	-List of attributes:

		- name 
		- flavor 
		- image 
		- original 
		- profile 
		- node 
		- price 
		- account 
		- number 
		- rootpass 
		- reference 
		- network 
		- access 
		- accessip 
		- keypair 
		- placementgroup 
		- publicaddr 
		- privateaddr 
		- firewall 
		- group 
		- zone 
		- hostname 
		- workload 
		- agent
		- when 
		- state 
"""

def amazonEc2_start(accesskey,secretkey,zone,keypair,amazonEc2):
	conn = boto.ec2.connect_to_region(zone,aws_access_key_id=accesskey,aws_secret_access_key=secretkey)
	status = 1
        if(amazonEc2.state == 2):
		resinst = conn.start_instances(amazonEc2.reference)
		instance = resinst[0]
		while instance.state != "running":
			time.sleep(5)
			instance.update()
		amazonEc2.state = 1
		return amazonEc2
	else:
		mykeys = conn.get_all_key_pairs()
		if((keypair) and (keypair!= " ")):
			mykeyname = ntpath.basename(keypair)
			fp = open(os.path.expanduser(keypair))
			mykey = fp.read()
			fp.close()
			for k in mykeys:
				if(k.name == mykeyname):
					amazonEc2.keypair = k.name
					status = 2
					break
				else:
					status = 0
		if(status==0):
				key = conn.import_key_pair(mykeyname,mykey)
				amazonEc2.keypair = key.name			
		elif (status == 1):
        		if(mykeys):
				amazonEc2.keypair = mykeys[0].name
			else:
				amazonEc2.keypair = " "

		if(amazonEc2.keypair != " "):
			res = conn.run_instances(amazonEc2.image,instance_type=amazonEc2.flavor,min_count=1,max_count=1,key_name=amazonEc2.keypair,security_groups=[amazonEc2.group],user_data=None)
		else:
			res = conn.run_instances(amazonEc2.image,instance_type=amazonEc2.flavor,min_count=1,max_count=1,security_groups=[amazonEc2.group],user_data=None)		

		inst = res.instances[0]
		while inst.state != "running":
			time.sleep(5)
			inst.update()
		conn.create_tags([inst.id],{"Name": amazonEc2.name})
		amazonEc2.reference = inst.id
		amazonEc2.hostname = inst.dns_name
		amazonEc2.publicaddr = inst.public_dns_name
		amazonEc2.privateaddr = inst.private_dns_name
		if(inst.dns_name):
			ipaddr = inst.dns_name.split("-")
			ipaddr4 = ipaddr[4].split(".")
			amazonEc2.accessip = ipaddr[1]+"."+ipaddr[2]+"."+ipaddr[3]+"."+ipaddr4[0]
		amazonEc2.state = 1
		return amazonEc2
			

def amazonEc2_stop(accesskey,secretkey,zone,amazonEc2):
	conn = boto.ec2.connect_to_region(zone,aws_access_key_id=accesskey,aws_secret_access_key=secretkey)
	reserv = conn.get_all_instances(filters={'instance-id' : amazonEc2.reference})
	inst = reserv[0].instances[0]
	inst.terminate() 
	while inst.state != "terminated":
		time.sleep(1.0)
		inst.update()
	amazonEc2.state = 0
	return amazonEc2


def amazonEc2_restart(accesskey,secretkey,zone,amazonEc2):
	conn = boto.ec2.connect_to_region(zone,aws_access_key_id=accesskey,aws_secret_access_key=secretkey)
	reserv = conn.get_all_instances(filters={'instance-id' : amazonEc2.reference})
	inst = reserv[0].instances[0]
	inst.reboot()
	while inst.state != "running":
		time.sleep(5.0)
		inst.update() 
	amazonEc2.state = 2
	return amazonEc2




def amazonEc2_snapshot(accesskey,secretkey,zone,imgname,amazonEc2):
	conn = boto.ec2.connect_to_region(zone,aws_access_key_id=accesskey,aws_secret_access_key=secretkey)
	image_id = conn.create_image(amazonEc2.reference,imgname,'This image is created by accords platform')
	while True:
		image = conn.get_image(image_id)
		if image.state == 'available':
			break
		if image.state == "failed":
			abort("Error in Ec2 service Creating AMI image failure")
		time.sleep(5.0)
	conn.create_tags([image_id], {"Name" : imagname+"_image"})
	return amazonEc2

def amazonEc2_suspend(accesskey,secretkey,zone,amazonEc2):
	conn = boto.ec2.connect_to_region(zone,aws_access_key_id=accesskey,aws_secret_access_key=secretkey)
	reserv = conn.get_all_instances(filters={'instance-id' : amazonEc2.reference})
	inst = reserv[0].instances[0]
	inst.stop() 
	while inst.state != "stopped":
		time.sleep(5.0)
		inst.update()
	amazonEc2.state = 2
	return amazonEc2


