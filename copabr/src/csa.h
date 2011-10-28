#ifndef	_csa_h
#define	_csa_h

struct	cordscript_action
{
	struct	cordscript_action * previous;
	struct	cordscript_action * next;
	int	type;
	struct	cordscript_element * lvalue;
	struct	cordscript_element * rvalue;
	int	status;
};


#endif	

