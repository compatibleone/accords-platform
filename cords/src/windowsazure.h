
#ifndef	_windowsazure_h
#define	_windowsazure_h

struct	windowsazure
{
	char *	id;

	char *	name;
	char *	flavor;
	char *	image;
	char *	profile;

	char *	number;
	char *	access;
	char *	rootpass;
	char *	reference;
	char *	publicaddr;
	char *	privateaddr;

	char *	firewall;
	char *	hostname;

	int	when;

	int	state;
};

#endif	


