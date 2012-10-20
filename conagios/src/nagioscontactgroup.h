#ifndef	_nagios_contact_group
#define _nagios_contact_group

struct	nagios_contact_group
{
	char *	id;
	char *	name;
	char *	alias;
	int	contacts;
	int	status;
};

#endif


