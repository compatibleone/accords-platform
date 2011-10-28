#ifndef	_fileserver_h
#define	_fileserver_h


#include "rest.h"

struct	fileserver_configuration
{
	int	autopub;
	int	verbose;
	int	debug;
	int	threads;
	int	authorise;

	char *  tls;
	char *	publisher;

	char *	identity;
	char *	user;
	char *	password;

	char *	resthost;
	int	restport;

	char *	domain;
	char *	config;

};



	/* --------- */
#endif	/* _fileserver_h */
	/* --------- */

