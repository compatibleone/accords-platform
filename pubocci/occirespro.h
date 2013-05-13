
#ifndef	_occi_res_pro_h
#define	_occi_res_pro_h

struct	occi_resolved_agency
{
	struct	occi_resolved_agency 	* previous;
	struct	occi_resolved_agency 	* next;
	struct	occi_resolved_category 	* parent;
	char				* id;
	char				* host;
	int				  ttl;
};

#endif



