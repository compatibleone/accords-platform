
#ifndef	_cords_probe_h
#define	_cords_probe_h

struct	cords_probe
{
	char *	id;
	char *	name;
	char *	metric;
	char *	connection;
	char *	expression;
	char *	timer;
	int	samples;
	int	period;
	int	pid;
	int	packets;
	int	state;
};

#endif





