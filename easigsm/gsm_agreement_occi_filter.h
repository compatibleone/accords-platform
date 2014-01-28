#ifndef _gsm_agreement_occi_filter_h
#define _gsm_agreement_occi_filter_h

struct	gsm_agreement_occi_filter
{
	struct gsm_agreement	*attributes;
	int	id;
	int	negotiation;
	int	account;
	int	name;
	int	verify;
	int	agreement;
	int	accept;
	int	manifest;
	int	refuse;
	int	state;
	int	plan;
	int	template;
};

#endif
