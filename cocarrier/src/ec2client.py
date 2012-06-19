import boto
from boto.ec2.connection import EC2Connection

accesskey=""
privatekey=""
agent=""
tls=""
zone=""

def ec2_initialise_client(p_accesskey, p_privatekey, p_agent, p_tls, p_zone):
	global accesskey	
	accesskey=p_accesskey
	global privatekey	
	privatekey=p_privatekey
	global agent
	agent=p_agent
	global tls
	tls=p_tls
	global zone
	


def ec2_zone_resolver(zone):
	if zone=='europe': 
	  aws_zone='eu-west-1'
	elif zone=='usa': 
	  aws_zone='us-west-1'
	else :
	  aws_zone='eu-west-1'
	return aws_zone


def ec2_connect(access_key, secret_key, zone):
        aws_zone=ec2_zone_resolver(zone)	
 	conn=boto.ec2.connect_to_region(aws_zone, aws_access_key_id=access_key, aws_secret_access_key=secret_key)
 	return conn
	
def ec2_list_servers(ec2):
	instances=ec2.get_all_instances()
	return instances
	
def ec2_resolve_image(ec2,amiID):
	response="NOT_OK"	
	l=[amiID]
	try:	
		images=ec2.get_all_images(image_ids=l)
	except:
		reponse="NOT_OK"	
	return response		


def ec2_resolve_flavor():
	return "t1.micro"

def ec2_list_regions (ec2):
	regions=ec2.get_all_regions()
	return regions		

def ec2_create_server(ec2, identity, ami, flavour):
	response="NOT_OK"
	try:	
		print ec2		
		reservation= ec2.run_instances(image_id=ami, instance_type=flavour)		
		print reservation		
		for r in ec2.get_all_instances():
	  		if (r.id == reservation.id):
	    			break
		reservation.instances[0].add_tag('Name',identity)
		response="OK"
	except:
		response="NOT_OK"
		
	return response


def ec2_create_certificate(ec2, certificate,path):	
	key_pair=ec2.get_key_pair(certificate)
	if key_pair!=None:
	  return key_pair
	key_pair = ec2.create_key_pair(certificate)
	#key_pair.save(path)
	return key_pair
		


def ec2_resolve_image_g(accesskey, secretkey, zone, amiID):
		
	ec2=ec2_connect(accesskey, secretkey, zone)
	return ec2_resolve_image(ec2,amiID)
	
def ec2_start(accesskey, secretkey, zone, identity, amiID, flavour):
		
	ec2=ec2_connect(accesskey, secretkey, zone)
	return ec2_create_server(ec2,identity,amiID, flavour)

'''
def main():
	#ec2=ec2_connect('AKIAJ2IKM6O55O7PPOIQ', 'IOKhyz1V+AqFlKpPiIH0msdns5yXoYC0WqKa4RP6', 'europe')
	#print ec2	
	#image=ec2_resolve_image(ec2,'ami-023f0d11')
	
	#print image
	#msg=ec2_create_server(ec2, "amazon", "ami-03c2f677", "t1.micro")
        #print msg	
	#images=ec2_list_images(ec2)
	#certif=ec2_create_certificate(ec2,'certif1','/home/ezekri/')
	#print certif
	#ec2_create_server(ec2, 'server1', 'ami-023f0d76' , 't1.micro', 'certif1')
	

if __name__=="__main__":
	main()
'''
