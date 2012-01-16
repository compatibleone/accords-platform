#ifndef	_authcache_h
#define	_authcache_h

#define	_AUTH_TTL 300

struct	occi_authorization_item;

struct	occi_authorization_cache
{
	struct	occi_authorization_item * first;
	struct	occi_authorization_item * last;
	int				  ttl;
};

#endif	


