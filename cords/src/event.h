#ifndef	_cords_event_h
#define	_cords_event_h

struct	cords_event
{
	char *	id;
	char *	source;
	char *	target;
	char *	nature;
	char *	description;
	char *	compliment;
	int	timestamp;
	int	state;
};

#endif


