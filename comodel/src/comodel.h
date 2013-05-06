#ifndef	_comodel_h
#define	_comodel_h

struct	occi_component;
struct	occi_model;

struct	occi_component
{
	struct	occi_component 	*	previous;
	struct	occi_component 	*	next;
	struct	occi_category 	* 	first;
	struct	occi_category 	* 	last;
	char *	name;
	int	isvalid;
	struct	accords_configuration configuration;
};


	/* ---------- */
#endif	/* _comodel_h */
	/* ---------- */

