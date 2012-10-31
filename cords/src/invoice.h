#ifndef	_cords_invoice_h
#define	_cords_invoice_h

struct	cords_invoice
{
	char *	id;
	char *	number;
	char *	account;
	char *	date;
	char *	authorization;
	char *	total;
	char *	taxe;
	char *	grandtotal;
	char *	reduction;
	char *	rate;
	char *	document;
	int	transactions;
	int	state;
};

#endif

