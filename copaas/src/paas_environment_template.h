#ifndef	_paas_environment_template_h
#define	_paas_environment_template_h
struct	paas_environment_template
{
	char *	id;
	char *	name;
	char *	memory;
	char *	disk;
	char *	description;
	char *	paas_environment_relation;
	char *	paas_environment_configuration;
	int	state;
};
#endif

