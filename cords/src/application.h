#ifndef	_cords_application_h
#define	_cords_application_h
struct	cords_application
{
	char	*	id;
	int		state;
	char	*	image;
	char	*	provider;
	char	*	price;
	char 	*	url;
	char 	*	account;
	char 	*	provision;
	int		created;
	int		started;
	int		completed;
	int		duration;
};

#endif

