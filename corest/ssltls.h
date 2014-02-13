#ifndef	_ABALSSL_H
#define	_ABALSSL_H

#ifdef SYSTEM_SSL
public	int	https_use_encryption(char *);
public	int	https_use_certificate(char *);
public	int	https_use_password(char *);
public	int	https_use_peer_cert(char *);

public	int	sslsocketwriter( SSL *, char *, int );
public	int	ssl_tcp_read( SSL *, char *, int );
public	int	ssl_tcp_readb( SSL * );
public	int	ssl_tcp_readw( SSL * );
#endif

#endif
