
#ifndef	_paas_application_deployable_h
#define	_paas_application_deployable_h
struct	paas_application_deployable
{
	char *	id;
	char *	name;
	char *	location;
	char *	content_type;
	char *	multitenancy_level;
	int	state;
};
#endif

