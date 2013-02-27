#ifndef	_paas_application_version_instance_h
#define	_paas_application_version_instance_h
struct	paas_application_version_instance
{
	char *	id;
	char *	name;
	char *	default_instance;
	char *	initial_stat;
	char *	memory;
	char *	disk;
	int	state;
};
#endif

