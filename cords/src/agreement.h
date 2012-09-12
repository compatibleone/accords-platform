
#ifndef	_cords_agreement_h
#define	_cords_agreement_h

struct	cords_agreement
{
	char * 	id;		
	char * 	name;		
	char * 	description;	
	char * 	initiator;	
	char * 	responder;	
	char * 	serviceprovider; 
	char *	initiation;
	char * 	expiration;	
	char * 	templateid;	
	char * 	templatename;	
	int	termss;		
	int	state;		
};

#endif	


