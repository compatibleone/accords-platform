
#ifndef	_cords_constraint_h
#define	_cords_constraint_h
struct	cords_constraint
{
	struct	cords_constraint * previous;
	struct	cords_constraint * next;
	char *	id;
	char *	description;
	char *	expression;
	char *	type;
	int	state;
};
#endif


