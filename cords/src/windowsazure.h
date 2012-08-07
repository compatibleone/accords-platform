
#ifndef	_windowsazure_h
#define	_windowsazure_h

struct	windowsazure
{
	char *	id;

	char *	name;
	char *	flavor;
	char *	image;
	char *	profile;
	char *	media;

	char *	number;
	char *	access;
	char *	rootpass;
	char *	reference;
	char *	publicaddr;
	char *	privateaddr;

	char *	firewall;
	char *	hostname;
	char *	workload;

	char *	node;
	char *	architecture;
	char *	driver;
	char *	price;
	char *	original;
	char *	publicnetwork;

	char *	account;
	char *	hostingservice;
	char *	storageaccount;
	char *	deployment;
	char *	location;

	int	when;

	int	state;
};

#endif	


