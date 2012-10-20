#ifndef	_nagios_contact
#define _nagios_contact

struct	nagios_contact
{
	char *	id;
	char * 	name;
	char * 	alias;
	char *	host_notification_period;
	char *	service_notification_period;
	char * 	host_notification_options;
	char *	service_notification_options;
	char *	email;
	char *	pager;
	char *	addressx;
	int	host_notifications_enabled;
	int	service_notifications_enabled;
	int	can_submit_commands;
	int	retain_status_information;
	int	retain_nonstatus_information;
	int	status;
};

#endif

