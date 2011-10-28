#ifndef	_procci_h
#define	_procci_h


#include "rest.h"

struct	procci_configuration
{
	int	monitor;

	int	verbose;
	int	debug;
	int	threads;
	char *	tls;

	char *	publisher;
	char *	identity;

	char *	user;
	char *	password;

	char *	resthost;
	int	restport;

	char *	chathost;
	int	chatport;
	char *	domain;
	char *	config;
	struct	occi_category *	firstcat;
	struct	occi_category *	lastcat;

};



	/* --------- */
#endif	/* _procci_h */
	/* --------- */

