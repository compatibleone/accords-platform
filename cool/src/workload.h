#ifndef _cords_workload_h
#define	_cords_workload_h

struct	cords_workload
{
	char *	id;
	char *	name;
	char *	description;
	char *	contract;
	int	nature;
	int	threads;
	int	average;
	int	total;
	int	samples;
	int	timestamp;
	int	period;
	int	state;
};

#endif


