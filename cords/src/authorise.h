
#ifndef	_cords_authorisation_h
#define	_cords_authorisation_h

struct	cords_authorisation
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

