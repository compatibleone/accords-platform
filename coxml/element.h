#ifndef	_element_h
#define	_element_h

struct	xml_atribut
{
	struct	xml_atribut * previous;
	struct	xml_atribut * next;
	char	*	name;
	char	*	value;
};

struct	xml_element 
{
	struct	xml_element * previous;
	struct	xml_element * next;
	struct	xml_element * parent;
	struct	xml_element * first;
	struct	xml_element * last;
	struct	xml_atribut * firstatb;
	struct	xml_atribut * lastatb;
	char *	name;
	char *	value;
	void *	payload;
};

struct	xml_atribut * allocate_atribut();
struct	xml_element * allocate_element();
struct	xml_atribut * liberate_atribut(struct xml_atribut * v);
struct	xml_element * liberate_element(struct xml_element * v);

#endif	/* _element_h */
	/* ---------- */

