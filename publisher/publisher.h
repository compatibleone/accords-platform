#ifndef	_publisher_h
#define	_publisher_h


#include "rest.h"

struct	publisher_configuration
{
	int	monitor;
	int	threads;
	int	verbose;
	int	debug;
	char *	tls;
	int	authorise;
	int	backup;
	int	autosave;

	char *	user;
	char *	password;

	char *	resthost;
	int	restport;

	char *	chathost;
	int	chatport;
	char *	chatagent;

	char *	domain;

	char *  config;
	char *	memory;

	struct	publication * first;
	struct	publication * last;

	struct	occi_category *	firstcat;
	struct	occi_category *	lastcat;

};



	/* ------------ */
#endif	/* _publisher_h */
	/* ------------ */

