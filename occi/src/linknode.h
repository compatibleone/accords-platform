
#ifndef	_linknode_h
#define	_linknode_h

struct occi_link_node
{
	struct occi_link_node  	* previous;
	struct occi_link_node  	* next;
	void 			* contents;
	char			* source;
	char			* target;
};

#endif

