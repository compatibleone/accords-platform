#ifndef	_nagios_service_escalation
#define	_nagios_service_escalation

struct	nagios_service_escalation
{
	char *	id;
	char *	name;
	char *	host_name;
	char *	hostgroup_name;
	char *	service_description;
	int	first_notification;
	int	last_notification;
	int	notification_interval;
	char *	escalation_period;
	char *	escalation_options;
	int	contacts;
	int	status;
};

#endif

