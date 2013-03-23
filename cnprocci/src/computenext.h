#ifndef _computenext_h
#define _computenext_h

struct  computenext
{
	char *  id;
	char *  profile;
	char *  description;
	char *  hostname;

	int     state;

	char *	node;
	char *	access;
	char *	agent;
	char *	account;
	char *	firewall;

	char *  workload;
	char *  transaction;
	char *  image;
	char *  instancetype;
	char *  volumestorage;

	char *  securitygroup;
	char *  keypair;
	char *  password;
	char *  username;

	char *  ipaddress;
	char *  status;

	char *  provider;
	char *  region;
	char *  zone;
	char *  price;
	char *  location;

	char *  apikey;
	char *  apisec;
};

#endif

