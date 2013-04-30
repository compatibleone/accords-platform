#ifndef	_item_h
#define	_item_h

struct	item {
	struct	item * previous;
	struct	item * next;
	char * 	name;
	char * 	basic;
	char * 	type;
	int	dimension;
	int	indirection;
	};

#endif	/* _item_h */


