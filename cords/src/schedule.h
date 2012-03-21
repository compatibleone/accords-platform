#ifndef	_cords_schedule_h
#define	_cords_schedule_h

struct	cords_schedule
{
	char *	id;
	char *	operation;
	char *	account;
	char *	price;
	int	requested;
	int	expected;
	int	started;
	int	completed;
	int 	priority;
	int	status;
};

#endif


