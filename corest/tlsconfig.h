#ifndef	_tls_config_h
#define	_tls_config_h


struct	tls_configuration
{
	struct	tls_configuration * previous;
	struct	tls_configuration * next;
	char *	filename;
	char *	id;
	char *	name;
	char *	key;
	char *	certificate;
	char *	passphrase;
	char *	authority;
	int	option;
	int	authenticate;
};


#endif


