
#ifndef	_jpaas_h
#define	_jpaas_h

struct	jpaas
{
	char *	id;

	char *	name;
	char *	environment;
	char *	application;
	char *	version;
	char *	instance;
	char *	profile;

	char *	node;
	char *	price;
	char *	account;

	char *	number;
	char *	rootpass;
	char *	reference;

	char *	access;

	char *	publicaddr;
	char *	privateaddr;

	char *	zone;
	char *	hostname;
	char *	workload;

	char *	agent;

	int	when;
	int	state;
};

#endif	


