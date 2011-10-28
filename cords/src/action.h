#ifndef	_cords_action_h
#define	_cords_action_h

struct	cords_action
{
	struct	cords_action * previous;
	struct	cords_action * next;
	char *	id;
	char *	name;
	char *	type;
	char *	expression;
	int	state;
};

#endif

