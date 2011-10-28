
#ifndef	_cords_requirement_h
#define	_cords_requirement_h
struct	cords_requirement
{
	struct	cords_requirement * previous;
	struct	cords_requirement * next;
	char *	id;
	char *	description;
	char *	expression;
	char *	type;
	int	state;
};
#endif


