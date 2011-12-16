
#ifndef	_cords_authorization_h
#define	_cords_authorization_h

struct	cords_authorization
{
	char	*	id;
	char	*	account;	
	char	*	user;
	char	*	pass;
	int		from;
	int		till;
	int		ttl;
	int		state;
};

#endif

