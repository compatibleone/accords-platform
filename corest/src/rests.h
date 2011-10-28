#ifndef	_rest_server
#define _rest_server

struct	rest_server
{
	int	port;
	char *	name;
	char *	user;
	char *	password;
	int	seconds;
	int	wakeup;
	int	fraction;
	int	workers;
	int	threads;
	struct	rest_header * headers;
	struct	rest_client * first;
	struct	rest_client * last;
	struct	rest_client * active;

};

#endif	
	


