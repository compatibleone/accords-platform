#ifndef	cords_federation_h
#define	cords_federation_h

struct	cords_federation
{
	char *	id;
	char *	name;
	char *	description;
	char *	publisher;
	char *	user;
	char *	password;
	char *	security;
	char *	tls;
	char *	authorization;
	int	status;
};

#endif

