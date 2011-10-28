#ifndef	_rest_header
#define	_rest_header

struct	rest_header
{
	struct	rest_header	* previous;
	struct	rest_header	* next;
	struct	rest_request	* parent;
	char			* name;
	char 			* value;
};

#endif





