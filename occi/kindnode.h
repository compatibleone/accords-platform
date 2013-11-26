
#ifndef	_kindnode_h
#define	_kindnode_h

struct occi_kind_node
{
	struct occi_kind_node  	* previous;
	struct occi_kind_node  	* next;
	void			* contents;
	char			* filter;
	struct occi_mixin 	* firstmixin;
	struct occi_mixin 	* lastmixin;
	struct occi_link  	* firstlink;
	struct occi_link  	* lastlink;
};

#endif

