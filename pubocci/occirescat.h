
#ifndef	_occi_res_cat_h
#define	_occi_res_cat_h

struct	occi_resolved_category
{
	struct	occi_resolved_category 	* previous;
	struct	occi_resolved_category 	* next;
	struct	occi_resolver_manager 	* parent;
	char 				* category;
	struct	occi_resolved_agency 	* first;
	struct	occi_resolved_agency 	* last;
};

#endif

