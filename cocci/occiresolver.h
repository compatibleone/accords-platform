
#ifndef	_occiresolver_h
#define	_occiresolver_h

#include "occiclient.h"
#include "occipublisher.h"
#include "occipub.h"
#include "cordslang.h"

struct	occi_resolver_manager;
struct	occi_resolved_category;
struct	occi_resolved_agency;

#include "occirespro.h"
#include "occirescat.h"

#define	_OCCI_TTL	60

struct	occi_resolver_manager
{
	int				 ttl;
	int				 optimised;
	struct	occi_resolved_category * first;
	struct	occi_resolved_category * last;
};
	

public	int	initialise_occi_resolver( char * host, char * room, char * publication, char * enquiry );
public	int	terminate_occi_resolver();
public	int	occi_resolver_default();
public	struct	occi_response * occi_resolver( char * category, char * agent );

public	struct	occi_response * occi_resolve_by_operator( char * category, char * operator, char * agent );
public	struct	occi_response * occi_resolve_by_score( char * category, char * score, char * agent );
public	struct	occi_response * occi_resolve_by_security( char * category, char * security, char * agent );
public	struct	occi_response * occi_resolve_by_price( char * category, char * price, char * agent );
public	struct	occi_response * occi_resolve_by_energy( char * category, char * energy, char * agent );
public	struct	occi_response * occi_resolve_by_zone( char * category, char * zone, char * agent );

public	char *	occi_resolve_category_provider( char * category, char * agent, char * tls );
public	char *	occi_resolve_category_price( char * category, char * operator, char * agent, char * tls );
public	char *	occi_resolve_consumer( char * consumer, char * agent, char * tls );
public	char *	occi_resolve_consumer_identity( char * consumer, char * agent, char * tls );
public	char *	occi_resolve_metric( char * metric, char * agent, char * tls );
public	char *	occi_resolve_account( char * name, char * agent, char * tls );
public	char *	occi_resolve_price( char * name, char * agent, char * tls );


	/* ---------------- */
#endif	/* _occi_resolver_h */
	/* ---------------- */

