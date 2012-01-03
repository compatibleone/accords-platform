
#ifndef	_cords_package_h
#define	_cords_package_h
struct	cords_package
{
	struct	cords_package * previous;
	struct	cords_package * next;
	char *	id;
	char *	nature;
	char *	name;
	char *	version;
	char *	distribution;
	char *	description;
	char *	constraint;
	char *	requirement;
	char *	mirrors;
	char *	installation;
	char *	configuration;
	char *	price;
	char *	license;
	int	state;
};

#endif

