
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

	char *	accessip;
	char *	floating;
	char *	floatingid;

	char *	publicaddr;
	char *	privateaddr;

	char *	firewall;
	char *	group;
	char *	zone;
	char *	hostname;

	int	when;

	int	state;
};

#endif	


