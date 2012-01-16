#ifndef	_authitem_h
#define	_authitem_h


struct	occi_authorization_item
{
	struct	occi_authorization_cache* parent;
	struct	occi_authorization_item * previous;
	struct	occi_authorization_item * next;
	char *	token;
	int	ttl;
};

#endif	

