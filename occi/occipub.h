#ifndef	_occipub_h
#define _occipub_h

struct	occi_publication
{
	struct	occi_publication * previous;
	struct	occi_publication * next;
	struct	occi_publisher	 * parent;
	char	*	category;
	char	*	id;
};

#endif	

