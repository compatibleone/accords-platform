#ifndef	_rest_client
#define	_rest_client

struct	rest_client
{
	struct	rest_client 	* previous;
	struct	rest_client 	* next;
	struct	rest_server 	* parent;
	char			* agent;	
	char			* buffer;
	int			buffersize;
	int			bytes;
	int			consumed;
	char			* user;
	char			* pass;
	struct	rest_header 	* headers;
};

	
#endif	
	


