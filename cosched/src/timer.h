#ifndef	_cords_timer_h
#define	_cords_timer_h

struct	cords_timer
{
	char *	id;
	char *	name;
	char *	description;
	char *	account;
	char *	price;

	char *	period;
	char *	nature;

	char *	created;
	char *	activated;
	char *	activation;

	char *	source;
	char *	target;

	int	reference;
	int	hitcount;
	int	state;
};

#endif


