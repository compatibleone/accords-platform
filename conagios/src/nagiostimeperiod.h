#ifndef	_nagios_time_period
#define	_nagios_time_period

struct	nagios_time_period
{
	char *	id;
	char *	name;
	char *	alias;
	char *	weekday;
	char *	exception;
	int	timeperiods;
};

#endif

