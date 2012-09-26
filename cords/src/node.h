
#ifndef	_cords_node_h
#define	_cords_node_h

struct	cords_node
{
	char *				id;
	char *				name;
	char *				provider;
	char *				profile;
	char *				infrastructure;
	char *				image;
	char *				category;
	char *				access;
	char *				common;
	char *				scope;
	char *				type;
	char *				firewall;
	int	state;
};

#endif

