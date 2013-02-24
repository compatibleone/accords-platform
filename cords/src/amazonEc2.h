#ifndef _amazonEc2_h_
#define _amazonEc2_h_

struct amazonEc2
{
	char * id;
	char * name;
	char * flavor;
	char * image;
	char * original;
	char * profile;
	char * node;
	char * price;
	char * account;
	char * number;
	char * rootpass;
	char * reference;
	char * network;
	char * access;
	char * accessip;
	char * keypair;
	char * placementgroup;
	char * publicaddr;
	char * privateaddr;
	char * firewall;
	char * group;
	char * zone;
	char * hostname;
	char * workload;
	char * blob;
        char * agent;
	int  when;
	int  state;
};

#endif

