#ifndef	_publication_h
#define	_publication_h

struct	publication
{
	struct	publication	* previous;
	struct	publication	* next;
	char *	id;
	char *	remote;
	char *	what;
	char *	where;
	char *	why;
	int	when;
	int	uptime;
	char *	who;
	char *	pass;
	char * 	identity;
	char *  zone;
	char *	price;
	char *	rating;
	char *	operator;
	int	pid;
	int	state;
};

#endif
