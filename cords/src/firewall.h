
#ifndef	_cords_firewall_h
#define	_cords_firewall_h

struct	cords_firewall
{
	char *	id;
	char *	name;
	char *	description;
	char *	provider;
	char *	node;
	char *	network;
	char *	image;
	int	ports;
	int	state;
};

#endif	


