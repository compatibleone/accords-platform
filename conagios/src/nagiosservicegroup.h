#ifndef	_nagios_service_group
#define	_nagios_service_group

struct	nagios_service_group
{
	char *	id;
	char *	name;
	char *	alias;
	int	members;
	int	servicegroup_members;
	char *	notes;
	char *	notes_url;
	char *	action_url;
};

#endif

