#ifndef	_tls_config_h
#define	_tls_config_h

#define _SSL_DEBUG				(1<<1)
#define _USE_SSL				(1<<2)
#define _REQUEST_PEER			(1<<3)
#define _REQUIRE_PEER			(1<<4)
#define _DER_KEY				(1<<5)
#define _DER_CERTIFICATE		(1<<6)
#define _SSL_COMPATIBLE			(1<<7)
#define _SSL_INTERNAL			(1<<8)
#define _OPENSSL_ENGINE         (1<<9)


#define _OPTIMISE_OCCI_LOCAL	(1<<12)
#define _OPTIMISE_OCCI_CLIENT	(1<<13)


#define _SSL_ACCEPT_INVALID		(1<<14)
#define _SSL_SELF_SIGNED		(1<<15)
#define _SSL_VALID_CERT			(1<<16)
#define _SSL_SAME_CA			(1<<17)

#define _SSL_MODES				(_SSL_ACCEPT_INVALID|_SSL_SELF_SIGNED|_SSL_VALID_CERT|_SSL_SAME_CA)

#define _SSL_IS_ISSUER			(1<<20)

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


