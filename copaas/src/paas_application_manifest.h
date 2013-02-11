#ifndef	_paas_application_manifest_h
#define	_paas_application_manifest_h
struct	paas_application_manifest
{
	char *	id;
	char *	name;
	char *	description;
	char *	paas_application;
	char *	paas_environment;
	char *	access;
	int	state;

};
#endif

