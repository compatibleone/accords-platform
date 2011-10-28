#ifndef	_rest_request
#define	_rest_request

struct	rest_request
{
	struct	rest_header	* first;
	struct	rest_header	* last;
	char			* method;
	char 			* object;
	char			* parameters;
	char			* version;
	int			type;
	char			* body;
};

#endif





