
#ifndef	_data_element_h
#define	_data_element_h

struct	data_element
{
	struct	data_element 	* previous;
	struct	data_element 	* next;
	struct	data_element 	* parent;
	struct	data_element 	* first;
	struct	data_element 	* last;
	char 			* name;
	char 			* value;
};
	

#endif	

