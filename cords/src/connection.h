
#ifndef	_cords_connection_h
#define	_cords_connection_h

struct	cords_connection
{
	char *	id;
	char *	name;
	char *	start;
	char *	finish;
	char *	account;
	char *	session;
	char *	monitor;
	int	pid;
	int	probes;
	int	state;
};

#endif





