#ifndef	_nagios_host_group
#define	_nagios_host_group

struct	nagios_host_group
{
	char *	id;
	char *	name;
	char *	alias;
	char *	notes;
	char *	notes_url;
	char *	action_url;
	int	members;
	int	statis;
};

#endif

