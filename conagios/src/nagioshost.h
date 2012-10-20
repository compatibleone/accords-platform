#ifndef	_nagios_host
#define	_nagios_host

struct 	nagios_host
{
	char *	id;
	char *	name;
	char *	alias;
	char *	display_name;
	char *	address;
	int	parents;
	int	hostgroups;
	char *	check_command;
	char *	initial_state;
	int	max_check_attempts;
	int	check_interval;
	int	retry_interval;
	int	active_checks_enabled;
	int	passive_checks_enabled;
	char *	check_period;
	int	obsess_over_host;
	int	check_freshness;
	int	freshness_threshold;
	char *	event_handler;
	int	event_handler_enabled;
	int	low_flap_threshold;
	int	high_flap_threshold;
	int	flap_detection_enabled;
	char *	flap_detection_options;
	int	process_perf_data;
	int	retain_status_information;
	int	retain_nonstatus_information;
	int	contacts;
	int	contact_groups;
	int	notification_interval;
	int	first_notification_delay;
	char *	notification_period;
	char *	notification_options;
	int	notifications_enabled;
	char *	stalking_options;
	char *	notes;
	char *	notes_url;
	char *	action_url;
	char *	icon_image;
	char *	icon_image_alt;
	char *	vrml_image;
	char *	statusmap_image;
	char *	twod_coords;
	char *	threed_coords;
	int	status;
};

#endif

