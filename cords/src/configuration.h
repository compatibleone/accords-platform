
#ifndef	_cords_configuration_h
#define	_cords_configuration_h

struct	cords_configuration
{
	char *	id;
	char *	name;
	int	actions;
	struct	cords_action *	firstact;
	struct	cords_action *	lastact;
	int	state;
};

#endif

