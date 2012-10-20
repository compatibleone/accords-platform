#ifndef	_nagios_service
#define	_nagios_service

struct	nagios_service
{
	char *	id;
	char *	name;
	char *	host_name;
	char *	hostgroup_name;
	char *	service_description;
	char *	display_name;
	int	servicegroups;
	int	is_volatile;
	char *	check_command;
	char *	initial_state;
	int	max_check_attempts;
	int	check_interval;
	int	retry_interval;
	int	active_checks_enabled;
	int	passive_checks_enabled;
	char *	check_period;
	int	obsess_over_service;
	int	check_freshness;
	int	freshness_threshold;
	char *	event_handler;
	int	event_handler_enabled;
	int	low_flap_threshold;
	int	high_flap_threshold;
	int	flap_detection_enabled;
	char *	flap_detection_options;
	int	process_perf_data ;
	int	retain_status_information;
	int	retain_nonstatus_information;
	int	notification_interval;
	int	first_notification_delay;
	char *	notification_period;
	char *	notification_options;
	int	notifications_enabled;
	int	contacts;
	int	contact_groups;
	char *	stalking_options;
	char *	notes;
	char *	notes_url;
	char *	action_url;
	char *	icon_image;
	char *	icon_image_alt;
	int	status;
};

#endif

