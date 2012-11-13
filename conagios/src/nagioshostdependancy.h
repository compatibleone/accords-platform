#ifndef	_nagios_host_dependency
#define	_nagios_host_dependency

struct	nagios_host_dependency
{
	char *	id;
	char *	 name;
	char *	dependent_host_name;
	char *	dependent_hostgroup_name;
	char *	host_name;
	char *	hostgroup_name;
	int	inherits_parent;
	char * 	execution_failure_criteria;
	char *	notification_failure_criteria;
	char *	dependency_period;
	int	status;
};

#endif

