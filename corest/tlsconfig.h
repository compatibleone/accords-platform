#ifndef	_tls_config_h
#define	_tls_config_h

#define _SSL_DEBUG              0x000002
#define _USE_SSL                0x000004
#define _REQUEST_PEER           0x000008
#define _REQUIRE_PEER           0x000010
#define _DER_KEY                0x000020
#define _DER_CERTIFICATE        0x000040
#define _SSL_COMPATIBLE         0x000080
#define _SSL_INTERNAL           0x000100
#define _OPENSSL_ENGIN          0x000200

#define _OPTIMISE_OCCI_LOCAL    0x001000
#define _OPTIMISE_OCCI_CLIENT   0x002000

#define _SSL_ACCEPT_INVALID     0x004000
#define _SSL_SELF_SIGNED        0x008000
#define _SSL_VALID_CERT         0x010000
#define _SSL_SAME_CA            0x020000

#define _SSL_IS_ISSUER          0x400000
#define _SSL_CERT_MATCH         0x800000

#define _SSL_MODES              (_SSL_ACCEPT_INVALID|_SSL_SELF_SIGNED|_SSL_VALID_CERT|_SSL_SAME_CA)

#define _SSL_RESULTS            (_SSL_IS_ISSUER|_SSL_CERT_MATCH)

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
	char *	peercert;
	int	option;
	int	authenticate;
};


#endif


