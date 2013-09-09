
#ifndef	_linknode_h
#define	_linknode_h

struct occi_link_node
{
	struct occi_link_node  	* previous;
	struct occi_link_node  	* next;
	void 			* contents;
};

public  struct  occi_link_node * allocate_occi_link_node();
public  struct  occi_link_node * liberate_occi_link_node(struct occi_link_node * n);

#endif

