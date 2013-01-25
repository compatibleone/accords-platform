
#ifndef	_cords_quota_h
#define	_cords_quota_h

struct	cords_quota
{
	char *	id;
	char *	name;
	char *	property;
	char *	description;
	char *	operator;
	char *	provision;
	char *	price;
	char *	zone;
	char *	opinion;
	char *	units;
	int	ceiling;
	int	offered;
	int	reserved;
	int	consumed;
	int	granularity;
	int	state;
};

#endif	

