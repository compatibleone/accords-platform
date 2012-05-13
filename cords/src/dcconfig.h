#ifndef	_dc_config_h
#define	_dc_config_h

struct	dc_config
{
	char	*	id;
	char	*	name;
	char 	*	description;
	char	*	user;
	char	*	password;
	char	*	authenticate;
	char	*	agent;
	char 	*	host;
	char	*	version;
	char 	*	namespace;
	char	*	base;
	char 	*	tls;
	int		current;
};

#endif	/* _dc_config_h */


