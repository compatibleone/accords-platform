
#ifndef	_cords_session_h
#define	_cords_session_h

struct	cords_session
{
	char *	id;
	char *	name;
	char *	contract;
	char *	account;
	char *	start;
	char *	finish;
	int	connections;
	int	state;
};

#endif





