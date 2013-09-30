#ifndef	_occihm_h
#define	_occihm_h

#ifndef	_occi_header_manager

struct	occi_header_manager	
{
	struct	occi_header_manager 	* previous;
	struct	occi_header_manager 	* next;
	pthread_t			  tid;
	pid_t				  pid;
	struct	rest_header		* headers;
};

#endif

#endif	

