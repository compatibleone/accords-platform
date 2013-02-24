
#ifndef	_windowsazure_h
#define	_windowsazure_h

struct	windowsazure
{
	char *	id;

	char *	name;
	char *	profile;

	char *	number;
	char *	access;
	char *	rootpass;
	char *	reference;
	char *	publicaddr;
	char *	privateaddr;

	char *	firewall;
	char *	hostname;
	char *	workload;
	char *	blob;

	char *	node;
	char *	architecture;
	char *	driver;
	char *	price;
	char *	original;
	char *	account;

	char *	location;
	char *	group;
	char *	hostedservice;
	char *	storageaccount;
	int	iteration;

	char *	image;
	char *	media;
	char *	flavor;
	char *	publicnetwork;

	char *	agent;

	int	when;

	int	state;
};

#endif	


