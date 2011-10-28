#ifndef	_broker_h
#define	_broker_h


#include "rest.h"

struct	broker_configuration
{
	int	monitor;
	int	autopub;
	int	verbose;
	int	debug;
	int	threads;

	char *  tls;
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
#endif	/* _broker_h */
	/* --------- */

