
#ifndef	_cords_contract_h
#define	_cords_contract_h

struct	cords_contract
{
	char *	id;
	char *	name;
	char *	node;
	char *	provider;
	char *	profile;
	char *	reference;
	char *	hostname;
	char *	rootpass;
	char *	tarification;
	char *	type;
	char *	service;
	int	when;
	int	state;
};

#endif	

