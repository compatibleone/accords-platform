#ifndef	_nagios_host_ext_info
#define	_nagios_host_ext_info

struct	nagios_host_ext_info
{
	char *	id;
	char *	name;
	char *	host_name;
	char *	notes;
	char *	notes_url;
	char *	action_url;
	char *	icon_image;
	char * 	icon_image_alt;
	char *	vrml_image;
	char *	statusmap_image;
	char *	twod_coords;
	char *	threed_coords;
	int	status;
};

#endif


