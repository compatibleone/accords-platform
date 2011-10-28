#ifndef	_cse_h
#define	_cse_h

struct	cordscript_element
{
	struct	cordscript_element * previous;
	struct	cordscript_element * next;
	int	type;					
	char *	prefix;
	char *	value;					
};


#endif	

