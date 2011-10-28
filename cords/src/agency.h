
#ifndef	_agency_h
#define	_agency_h

struct	agency
{
	char	*	id;
	char	*	servicename;
	char	*	description;
	char	*	identity;
	char	*	port;
	char 	*	domain;
	char	*	publisher;
	char	*	user;
	char	*	password;
	int		process;
	int		start;
	int		uptime;
	int		state;
};


#endif


