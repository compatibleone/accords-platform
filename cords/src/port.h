
#ifndef	_cords_port_h
#define	_cords_port_h

struct	cords_port
{
	char *	id;
	char *	name;
	char *	description;
	char *	protocol;
	char *	range;
	char *	from;
	char *	to;
	char *	direction;
	char *	target;
	int	state;
};

#endif	

