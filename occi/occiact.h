#ifndef	_occiact_h
#define _occiact_h

typedef struct  rest_response * (*occi_action_func)(
        struct occi_category * optr, 
        struct rest_client * cptr, 
        struct rest_request * rptr, 
        struct rest_response * aptr, 
        void * vptr );

struct	occi_action
{
	struct	occi_action 	* previous;
	struct	occi_action 	* next;
	struct	occi_category  	* parent;
	struct	occi_category  	* binding;
	struct	occi_parameter	* first;
	struct	occi_parameter	* last;
	char *	(*html_rendering)(struct occi_category * cptr,struct rest_header  * hptr );
	char		    	* name;
	occi_action_func          action;
};

public struct occi_action * add_occi_action(struct occi_category * pptr);

#endif 	

