
#ifndef	_openstack_h
#define	_openstack_h

struct	openstack
{
	char *	id;

	char *	name;
	char *	flavor;
	char *	image;
	char *	profile;

	char *	number;
	char *	rootpass;
	char *	reference;
	char *	publicaddr;
	char *	privateaddr;

	char *	hostname;

	int	when;

	int	status;
};

#endif	


