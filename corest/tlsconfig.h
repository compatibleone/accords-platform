#ifndef	_tls_config_h
#define	_tls_config_h

#define _SSL_DEBUG		0x00002
#define _USE_SSL		0x00004
#define _REQUEST_PEER		0x00008	
#define _REQUIRE_PEER		0x00010
#define _DER_KEY		0x00020
#define _DER_CERTIFICATE	0x00040
#define _SSL_COMPATIBLE		0x00080
#define _SSL_INTERNAL		0x00100
#define _OPENSSL_ENGINE         0x00200

#define _OPTIMISE_OCCI_LOCAL	0x01000
#define _OPTIMISE_OCCI_CLIENT	0x02000

#define _SSL_ACCEPT_INVALID	0x04000
#define _SSL_SELF_SIGNED	0x08000
#define _SSL_VALID_CERT		0x10000
#define _SSL_SAME_CA		0x20000
#define _SSL_IS_ISSUER		0x40000

#define _SSL_MODES				(_SSL_ACCEPT_INVALID|_SSL_SELF_SIGNED|_SSL_VALID_CERT|_SSL_SAME_CA)

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


