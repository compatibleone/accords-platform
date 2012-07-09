
#ifndef	_proactive_h
#define	_proactive_h

struct	proactive
{
	char *	id;    

	char *	name;       
	char *	flavor;
	char *	image;
	char *	profile;

	char *	node;
	char *	price;
	char *	account;

	char *	number;     // uuid of the ProActive node request? 
	char *	access;
	char *	rootpass;   
	char *	reference;  
	char *	publicaddr;
	char *	privateaddr;

	char *	hostname;
	char *	workload;

	int	when;

	int	status;

};

#endif	


