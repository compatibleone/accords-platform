#ifndef	_nagios_service_dependency
#define	_nagios_service_dependency
struct	nagios_service_dependency
{
	char *	id;
	char *	name;
	char *	dependent_host_name;
	char *	dependent_hostgroup_name;
	char *	dependent_service_description;
	char *	host_name;
	char *	hostgroup_name;
	char *	service_description;
	int	inherits_parent;
	char *	execution_failure_criteria;
	char *	notification_failure_criteria;
	char *	dependancy_period;
	int	status;
};

#endif

