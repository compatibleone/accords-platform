
#ifndef	_cords_quota_h
#define	_cords_quota_h

struct	cords_quota
{
	char *	id;
	char *	name;
	char *	description;
	char *	operator;
	char *	price;
	char *	zone;
	char *	opinion;
	int	ceiling;
	int	offered;
	int	reserved;
	int	consumed;
	int	status;
};

#endif	

