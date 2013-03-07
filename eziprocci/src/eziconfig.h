#ifndef	_ezi_config_h
#define	_ezi_config_h

struct	ezi_config
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
	int	state;
};

#endif



