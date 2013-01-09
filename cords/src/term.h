
#ifndef	_cords_term_h
#define	_cords_term_h

struct	cords_term
{
	char *	id;	
	char *	name;	
	char * 	description;
	char * 	type;	
	char *	manifest;
	char *	provider;
	int	variables;
	int	guarantees;
	int	state;	
};

#endif	


