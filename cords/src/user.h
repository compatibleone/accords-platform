
#ifndef	_cords_user_h
#define	_cords_user_h

struct	cords_user
{
	struct	cords_user * previous;
	struct	cords_user * next;
	char *	id;
	char *	name;
	char *	password;
	char *	role;
	char *	authorization;
	int	when;
	int	state;
};

#endif	


