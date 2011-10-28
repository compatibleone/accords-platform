
#ifndef	_cords_transaction_h
#define	_cords_transaction_h

struct	cords_transaction
{
	char	*	id;
	char	*	authorisation;
	char	*	source;
	char	*	target;
	char	*	description;
	char	*	value;
	char	*	currency;
	int		when;
	int		validity;
	int		type;
	int		state;
};

#endif

