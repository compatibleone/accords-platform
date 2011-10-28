#ifndef	_rest_extension
#define _rest_extension

struct	rest_extension
{
	struct	rest_extension	* previous;
	struct	rest_extension	* next;
	void 			* payload;
	int			type;
};

#endif

