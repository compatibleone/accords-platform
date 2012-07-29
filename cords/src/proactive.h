
#ifndef	_proactive_h
#define	_proactive_h

struct	proactive
{
	char *	id;    

	char *	name;       
	char *	flavor;
	char *	image;
	char *	profile;

	char *	number;     
	char *	access;
	char *	rootpass;   
	char *	reference;  
	char *	publicaddr;
	char *	privateaddr;

	char *	hostname;
	char *	workload;
	char *	account;

	int	when;

	int	status;

};

#endif	


