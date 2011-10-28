#ifndef	_worker_h
#define	_worker_h

struct	rest_worker
{
	struct	rest_server * parent;
	struct	rest_worker * previous;
	struct	rest_worker * next;
	int	pid;
	int	status;
	long	started;
};


#endif	
	
	
