
#ifndef	_deltacloud_h
#define	_deltacloud_h

struct	deltacloud
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

	int	status;
};

#endif	


