###############################################################################
# copyright 2013, Hamid MEDJAHED  (hmedjahed@prologue.fr) Prologue            #
# Licensed under the Apache License, Version 2.0 (the "License");             #
# you may not use this file except in compliance with the License.            #
# You may obtain a copy of the License at                                     #
#                                                                             #
#       http://www.apache.org/licenses/LICENSE-2.0                            #
#                                                                             #
# Unless required by applicable law or agreed to in writing, software         #
# distributed under the License is distributed on an "AS IS" BASIS,           #
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.    #
# See the License for the specific language governing permissions and         #
# limitations under the License.                                              #
###############################################################################
import sys
import time
import pypacksrc
srcdirectory=pypacksrc.srcpydir+"/pyaccords/pygen/"
srcdirectoryc=pypacksrc.srcpydir+"/pyaccords/pysrc/"
sys.path.append(srcdirectoryc)
sys.path.append(srcdirectory)
import boto
import boto.ec2
from ec2instanceinfo import *

class Flavor:
        def __init__(self,name,memory,computeunits,storage,architecture,ioperf,maxips,apiname,unixcosth,wincosth):
                self.name = name
                self.memory = memory
                self.computeunits = computeunits
                self.storage = storage
                self.architecture = architecture
                self.ioperf = ioperf
                self.maxips = maxips
                self.apiname = apiname
                self.unixcosth = unixcosth
                self.wincosth = wincosth

def my_range(start, end, step):
	while start <= end:
		yield start
		start+=step

def convertTovalue(strvalue):
	l=strvalue.split(" ")
	value = float(l[0])
	if(l[1]=="TB"):
		factor = 1000000000
	elif (l[1] == "GB"):
		factor = 1000000
	elif (l[1] == "MB"):
		factor = 1000
	else :
		factor = 1
	return (value * factor)

def ec2_flavors():
	p = TableParser()
	p.feed(data)
	listFlavors = []
	for i in my_range(0,len(p.flavors)-10,10):
		myFlavor = Flavor(p.flavors[i],p.flavors[i+1],p.flavors[i+2],p.flavors[i+3],p.flavors[i+4],p.flavors[i+5],p.flavors[i+6],p.flavors[i+7],p.flavors[i+8],p.flavors[i+9])
		listFlavors.append(myFlavor)
	return listFlavors
		

def ec2_get_flavor(memory,cores,speed,storage,architecture):
	listFlavors = ec2_flavors()
	for i in my_range(0,len(listFlavors)-1,1):
		ec2memory = convertTovalue(listFlavors[i].memory)
		ec2speed = int(float(listFlavors[i].computeunits))
		ec2storage = convertTovalue(listFlavors[i].storage)
		ec2architecture = listFlavors[i].architecture
		if((ec2memory <= memory) and (ec2speed <= speed) and (ec2storage <= storage)):
			return listFlavors[i].apiname
	return "t1.micro"


def ec2_get_zone(accesskey, secretkey, zone):
	regions = boto.ec2.regions(aws_access_key_id=accesskey,aws_secret_access_key=secretkey)
	for my_region in regions:
		regioninfos = str(my_region)
		regioninfo = regioninfos.split(":")
		ec2zone = regioninfo[1]
		if(ec2zone == zone):
			return ec2zone
	return "eu-west-1"

def ec2_get_os(imgname):
	if("win" in imgname):
		return "windows"
	elif("ubu" in imgname):
		return "ubuntu"
	elif("deb" in imgname):
		return "debian"
	elif("red" in imgname):
		return "redhat"
	elif("cent" in imgname):
		return "centos"
	elif("sus" in imgname):
		return "suse"
	else:
		return "ubuntu"
	
def ec2_get_image(accesskey, secretkey,imgname, zone):
	EC2_IMAGE_TO_AMI = {
		"ubuntu" : "ami-ad184ac4",
		"debian" : "ami-3428ba5d",
		"suse" : "ami-e8084981",
		"redhat" : "ami-a25415cb",
		"centos" : "ami-07b73c6e",
		"windows" : "ami-7527031c",
	}
	imgos = ec2_get_os(imgname)
	conn = boto.ec2.connect_to_region(zone,aws_access_key_id=accesskey,aws_secret_access_key=secretkey)
	image = conn.get_all_images(filters={'name' : imgname})
	if (image):
		if (image[0]):
			imagestr = str(image[0]).split(":")
			ec2imageid = imagestr[1]
			return ec2imageid
		else:
			images = conn.get_all_images()
			for myimage in images:
				if(myimage.name):
					if imgname in myimage.name:
						imagestr = str(myimage).split(":")
						ec2imageid = imagestr[1]
						return ec2imageid
	if(EC2_IMAGE_TO_AMI.has_key(imgos)):
			return EC2_IMAGE_TO_AMI[imgos]



def ec2_create_secgroup(accesskey,secretkey,zone,secname):
	secgname = secname.split("/")
	sgname = secgname[4]
	descrip = "This is a new security group: "+sgname
	conn = boto.ec2.connect_to_region(zone,aws_access_key_id=accesskey,aws_secret_access_key=secretkey)
	secgroup = conn.get_all_security_groups()
	for group in secgroup:
		if(group.name):
			if sgname == group.name:
				return sgname
	ngroup = conn.create_security_group(sgname,descrip)
	while True:
		secgroups = conn.get_all_security_groups()
		for group in secgroups:
			if sgname == group.name:
				return sgname
			else:
				time.sleep(2.0)
	return sgname

def ec2_delete_secgroup(accesskey, secretkey, zone, secname):
	secgname = secname.split("/")
	sgname = secgname[4]
	filtersg = {'group-name' : sgname}
	conn = boto.ec2.connect_to_region(zone,aws_access_key_id=accesskey,aws_secret_access_key=secretkey)
	try:
		for mysg in conn.get_all_security_groups(filters=filtersg):
			mysg.delete()
	except:
		return "exception"
	return "succeeded"

	
def ec2_add_rule(accesskey,secretkey,zone,group,rname,fport,tport,protocol):
	conn = boto.ec2.connect_to_region(zone,aws_access_key_id=accesskey,aws_secret_access_key=secretkey)
	conn.revoke_security_group(group,ip_protocol=protocol, from_port=str(fport),to_port=str(tport),cidr_ip='0.0.0.0/0')

	auth = conn.authorize_security_group(group,ip_protocol=protocol, from_port=str(fport),to_port=str(tport),cidr_ip='0.0.0.0/0')
	return "succeeded"
