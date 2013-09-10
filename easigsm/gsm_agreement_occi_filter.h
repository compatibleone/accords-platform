#ifndef _gsm_agreement_occi_filter_h
#define _gsm_agreement_occi_filter_h

struct	gsm_agreement_occi_filter
{
	struct gsm_agreement	*attributes;
	int	id;
	int	account;
	int	name;
	int	verify;
	int	accept;
	int	state;
	int	template;
};

#endif
