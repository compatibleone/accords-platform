#ifndef	_paas_environment_node_h
#define	_paas_environment_node_h
struct	paas_environment_node
{
	char *	id;
	char *	name;
	char *	version;
	char *	provider;
	char *	content_type;
	int	state;
};
#endif

