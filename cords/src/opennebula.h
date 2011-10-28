
#ifndef	_opennebula_h
#define	_opennebula_h

struct	opennebula
{
	char *	id;
	char *	name;
	char *	hostname;
	char *	flavour;
	char *	image;
	char *	publicaddr;
	char *	privateaddr;
	char *	started;
	char *	created;
	char *	configuration;
	int	status;
};

#endif	


