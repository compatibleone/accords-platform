#ifndef	_occic_h
#define	_occic_h

#ifndef	_occi_client

struct	occi_client	
{
	struct	occi_client 	* previous;
	struct	occi_client 	* next;
	struct	occi_manager	* parent;

	char 			* host;
	char 			* agent;
	char 			* tls;
	char			* mimetype;

	int			  categories;
	int			  started;
	int			  requests;
	int			  failures;

};

#endif

public struct occi_client * liberate_occi_client(struct occi_client * sptr);

#endif	

