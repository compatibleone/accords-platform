
#ifndef	_cords_firewall_h
#define	_cords_firewall_h

struct	cords_firewall
{
	char *	id;
	char *	name;
	char *	description;
	char *	contract;
	char *	provider;
	char *	node;
	int	ports;
	int	status;
};

#endif	


