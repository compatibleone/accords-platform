
#ifndef	_oa_config_h
#define	_oa_config_h

struct	oa_config
{
	char	*	id;
	char	*	name;
	char 	*	description;
	char	*	user;
	char	*	password;
	char    *  	authentication;
	char	*	agent;
	char 	*	host;
	char	*	version;
	char	*	base;
	char 	*	tls;
	int		current;
};

#endif

