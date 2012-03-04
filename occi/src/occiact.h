#ifndef	_occiact_h
#define _occiact_h

struct	occi_action
{
	struct	occi_action 	* previous;
	struct	occi_action 	* next;
	struct	occi_category  	* parent;
	struct	occi_parameter	* first;
	struct	occi_parameter	* last;
	char		    	* name;
	void			* action;
};

public struct occi_action * add_occi_action(struct occi_category * pptr);

#endif 	

