#ifndef	_publication_h
#define	_publication_h

struct	publication
{
	struct	publication	* previous;
	struct	publication	* next;
	char *	id;
	char *	what;
	char *	where;
	char *	why;
	int	when;
	int	uptime;
	char *	who;
	char *	pass;
	char * 	identity;
	char *  zone;
	int	pid;
	int	state;
};

#endif
