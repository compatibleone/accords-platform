#ifndef	_nagios_host_escalation
#define	_nagios_host_escalation

struct	nagios_host_escalation
{
	char *	id;
	char *	name;
	char *	host_name;
	char *	hostgroup_name;
	char *	escalation_period;
	char *	escalation_options;
	int	first_notification;
	int	last_notification;
	int	notification_interval;
};

#endif

