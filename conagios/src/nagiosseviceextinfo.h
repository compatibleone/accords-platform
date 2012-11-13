#ifndef	_nagios_service_ext_info
#define	_nagios_service_ext_info

struct 	nagios_service_ext_info
{
	char *	id;
	char *	name;
	char *	host_name;
	char *	service_description;
	char *	notes;
	char *	notes_url;
	char *	action_url;
	char *	icon_image;
	char *	icon_image_alt;
	int	status;
};

#endif

