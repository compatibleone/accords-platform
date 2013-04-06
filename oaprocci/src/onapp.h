#ifndef _onapp_h
#define _onapp_h

struct  onapp
{
	char *  id;
	char *	name;
	char *  profile;
	char *  description;

	int     state;

	char *	node;
	char *	access;
	char *	agent;
	char *	account;
	char *	firewall;

	char *  image;
	char *	original;
	char *  password;
	char *  username;
	char *  hostname;
	char *  ipaddress;
	char *  status;

	char *  provider;
	char *  region;
	char *  zone;
	char *  price;
	char *  location;

};

#endif

