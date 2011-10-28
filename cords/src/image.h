
#ifndef	_cords_image_h
#define	_cords_image_h

struct	cords_image
{
	char *	id;
	char *	name;
	char *	system;
	int	packages;
	char *	created;
	char *	updated;
	struct	cords_package * firstpack;
	struct	cords_package * lastpack;
	int	state;
};

#endif

