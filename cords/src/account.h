#ifndef	_cords_account_h
#define	_cords_account_h

struct	cords_account
{
	char *	id;
	char *	number;
	char *	name;
	char *	date;
	char *	security;
	char *	legal;
	char *	nature;
	char *	invoicing;
	int	users;
	int	state;
};

#endif

