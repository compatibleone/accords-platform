
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
public	char *	occi_resolve_category_provider( char * category, char * agent, char * tls );
public	char *	occi_resolve_category_price( char * category, char * operator, char * agent, char * tls );


	/* ---------------- */
#endif	/* _occi_resolver_h */
	/* ---------------- */

