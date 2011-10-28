
#ifndef	_occilink_h
#define	_occilink_h


struct	occi_link
{
	struct	occi_link 	* previous;
	struct	occi_link 	* next;
	void			* contents;
	char			* from;
	char			* to;
};



#endif	

