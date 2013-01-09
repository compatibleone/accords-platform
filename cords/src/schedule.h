#ifndef	_cords_schedule_h
#define	_cords_schedule_h

struct	cords_schedule
{
	char *	id;
	char *	name;
	char *	operation;
	char *	reference;
	char *	account;
	char *	price;
	int	requested;
	int	expected;
	int	started;
	int	completed;
	int	duration;
	int 	priority;
	int	state;
};

#endif


