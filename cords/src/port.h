
#ifndef	_cords_port_h
#define	_cords_port_h

struct	cords_port
{
	char *	id;
	char *	name;
	char *	description;
	char *	protocol;
	char *	direction;
	char *	address;
	int	number;
	int	target;
	int	status;
};

#endif	

