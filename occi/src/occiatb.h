#ifndef	_occiatb_h
#define _occiatb_h

struct	occi_attribute
{
	struct	occi_attribute 	* previous;
	struct	occi_attribute 	* next;
	struct	occi_category  	* parent;
	char		    	* name;
	char		    	* type;
	char		    	* units;
	char		    	* validation;
	char		    	* defaultvalue;
	int			  mandatory;
	int			  indexed;
	int			  immutable;
};

public struct occi_attribute * add_occi_attribute(struct occi_category * pptr);

#endif 	

