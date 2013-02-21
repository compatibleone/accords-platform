/*-------------------------------------------------------------------------------*/
/* accords platform                                                              */
/* copyright 2013 ,Hamid MEDJAHED (hmedjahed@prologue.fr) prologue               */
/*-------------------------------------------------------------------------------*/
/* licensed under the apache license, version 2.0 (the "license");               */
/* you may not use this file except in compliance with the license.              */
/* you may obtain a copy of the license at                                       */
/*                                                                               */
/*       http://www.apache.org/licenses/license-2.0                              */
/*                                                                               */
/* unless required by applicable law or agreed to in writing, software           */
/* distributed under the license is distributed on an "as is" basis,             */
/* without warranties or conditions of any kind, either express or implied.      */
/* see the license for the specific language governing permissions and           */
/* limitations under the license.                                                */
/*-------------------------------------------------------------------------------*/

#ifndef _ec2client_h
#define _ec2client_h

struct  ec2_subscription 
{
	char * accesskey;
	char * secretkey;
	char * zone;
	char * keypair;	
};

char * get_ec2_imgname(struct ec2_subscription* subptr, char *imgname,char * zone);
char * get_ec2_zone(struct ec2_subscription* subptr, char * zone);
char * get_ec2_flavor(int memory, int cores, int speed, int storage, char * architecture);

char * build_ec2_firewall(struct ec2_subscription * subptr, struct amazonEc2 * pptr);
char * add_ec2_rule(struct ec2_subscription * subptr, char * group, char * rulename, int fport, int tport, char * protocol);
char * create_ec2_secgroup(struct ec2_subscription * subptr, struct amazonEc2 * pptr);


int	snapshot_ec2_instance( struct amazonEc2 * pptr );
int	suspend_ec2_instance( struct amazonEc2 * pptr );
int	restart_ec2_instance( struct amazonEc2 * pptr );
int 	start_ec2_instance(struct ec2_subscription * subptr, struct amazonEc2 * pptr);

int	reset_ec2_server( struct amazonEc2 * pptr );
int 	stop_ec2_provisioning( struct amazonEc2 * pptr );

#endif
