
#ifndef	_openstack_h
#define	_openstack_h

struct	openstack
{
	char *	id;

	char *	name;
	char *	flavor;
	char *	original;
	char *	image;
	char *	profile;

	char *	node;
	char *	price;
	char *	account;

	char *	number;
	char *	rootpass;
	char *	reference;
	char *	network;

	char *	access;

	char *	publicaddr;
	char *	privateaddr;

	char *	hostname;

	int	when;

	int	status;
};

#endif	


