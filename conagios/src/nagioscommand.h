#ifndef	_nagios_command_h
#define	_nagios_command_h

struct	nagios_command
{
	char *	id;
	char *	name;
	char *	syntax;
	int	status;
};

#endif

