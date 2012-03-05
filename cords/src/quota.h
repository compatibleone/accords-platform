
#ifndef	_cords_quota_h
#define	_cords_quota_h

struct	cords_quota
{
	char *	id;
	char *	name;
	char *	description;
	char *	operator;
	char *	price;
	int	ceiling;
	int	floor;
	int	reserved;
	int	consumed;
	int	status;
};

#endif	

