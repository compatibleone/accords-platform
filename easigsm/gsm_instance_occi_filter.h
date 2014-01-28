#ifndef _gsm_instance_occi_filter_h
#define _gsm_instance_occi_filter_h

struct	gsm_instance_occi_filter
{
	struct gsm_instance	*attributes;
	int	id;
	int	account;
	int	name;
	int	service;
	int	stop;
	int	scaleup;
	int	agreement;
	int	start;
	int	state;
	int	scaledown;
};

#endif
