#ifndef	_occipar_h
#define _occipar_h

struct	occi_parameter
{
	struct	occi_parameter 	* previous;
	struct	occi_parameter 	* next;
	struct	occi_action  	* parent;
	char		    	* name;
};

#endif 	

