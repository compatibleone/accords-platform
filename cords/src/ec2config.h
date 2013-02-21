#ifndef _cords_ec2config_h_
#define _cords_ec2config_h_

struct ec2config
{
	char * id;
	char * name;
	char * description;
	char * user;
	char * password;
	char * accesskey;
	char * secretkey;
	char * authenticate;
	char * agent;
	char * host;
	char * version;
	char * location;
	char * namespace;
	char * base;
	char * tls;
	char * current;
};

#endif

