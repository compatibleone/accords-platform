#ifndef	_occicat_h
#define _occicat_h

struct	occi_category
{
	struct	occi_category  	* previous;
	struct	occi_category  	* next;
	struct	occi_attribute 	* first;
	struct	occi_attribute 	* last;
	struct	occi_action	* firstact;
	struct	occi_action	* lastact;
	char			* domain;
	char		    	* id;
	char			* scheme;
	char			* class;
	char			* title;
	char 			* rel;
	char			* location;
	int			  access;
	void			* interface;
	void			* callback;
	void			* payload;
};

#endif 	

