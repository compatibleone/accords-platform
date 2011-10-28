#ifndef	_cords_invoice_h
#define	_cords_invoice_h

struct	cords_invoice
{
	char *	id;
	char *	number;
	char *	account;
	char *	date;
	char *	authorisation;
	char *	total;
	char *	taxe;
	char *	reduction;
	char *	rate;
	int	lines;
	int	status;
};

#endif

