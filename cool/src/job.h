#ifndef _cords_job_h
#define	_cords_job_h

struct	cords_job
{
	char *	id;
	char *	name;
	char *	description;
	char *	contract;
	int	floor;
	int	ceiling;
	int	threshold;
	int	strategy;
	int	workloads;
	int	state;
};


#endif

