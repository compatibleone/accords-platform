
#ifndef	_paas_h
#define	_paas_h

struct	paas
{
	char *	id;

	char *	name;
	char *	environment;
	char *	envfile;
	char *	application;
	char *	appfile;

	char *	profile;

	char *	node;
	char *	price;
	char *	account;
	char *	authorization;

	char *	number;
	char *	rootpass;
	char *	reference;

	char *	access;

	char *	publicaddr;
	char *	privateaddr;

	char *	zone;
	char *	hostname;
	char *	workload;
	char *	blob;

	char *	agent;

	int	when;
	int	state;
};

#endif	


