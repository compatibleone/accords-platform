#ifndef	_paasconfig_h
#define	_paasconfig_h

struct	paas_config
{
	char *	id;
	char *	name;
	char *	description;
	char *	host;
	char *	user;
	char *	password;
	char *	subscription;
	char *	tenant;
	char *	version;
	char *	tls;
	int	port;
	int	state;	
};

#endif

