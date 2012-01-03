
#ifndef	_cords_transaction_h
#define	_cords_transaction_h

struct	cords_transaction
{
	char	*	id;
	char	*	authorization;
	char	*	source;
	char	*	account;
	char	*	description;
	char	*	reference;
	char	*	price;
	int		when;
	int		validity;
	int		type;
	int		state;
};

#endif

