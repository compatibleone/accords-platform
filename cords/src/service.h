
#ifndef	_cords_service_h
#define	_cords_service_h

struct	cords_service
{
	char *	id;
	char *	name;
	char *	manifest;
	char *	plan;
	char *	account;
	char *	sla;
	char *	price;
	char *	session;
	char *	initiation;
	char *	expiration;
	int	when;
	int	instructions;
	int	contracts;
	int	state;
};

#endif




