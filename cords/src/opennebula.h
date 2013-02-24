
#ifndef	_opennebula_h
#define	_opennebula_h

struct	opennebula
{
	char *	id;
	char *	name;
	char *	number;
	char *	hostname;
	char *	flavor;
	char *	image;
	char *	architecture;
	char *	driver;
	char *	original;
	char *	node;
	char *	price;
	char *	account;
	char * 	access;
	char *	workload;
	char *	blob;
	char *	publicaddr;
	char *	privateaddr;
	char *	profile;
	char *	publicnetwork;
	char *	privatenetwork;
	char *	started;
	char *	created;
	char *	firewall;
	char *	group;
	char *	configuration;
	char *	agent;
	int	when;
	int	state;
};

#endif	


