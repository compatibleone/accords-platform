#ifndef	_paas_application_h
#define	_paas_application_h
struct	paas_application
{
	char *	id;
	char *	name;
	char *	description;
	char *	environment;
	char *	paas_application_version;
	int	state;
};
#endif

