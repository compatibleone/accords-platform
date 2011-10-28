#ifndef	_occiatb_h
#define _occiatb_h

struct	occi_attribute
{
	struct	occi_attribute 	* previous;
	struct	occi_attribute 	* next;
	struct	occi_category  	* parent;
	char		    	* name;
	int			  mandatory;
	int			  immutable;
};

#endif 	

