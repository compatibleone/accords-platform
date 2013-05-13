#ifndef	_oaclient_c
#define	_oaclient_c

#include "oaclient.h"

public struct	oa_response * oa_liberate_response( struct oa_response * rptr )
{
	if ( rptr )
	{
		rptr = liberate( rptr );
	}
	return((struct oa_response *) 0);
}

public struct	oa_config * oa_initialise_client( char * user, char * pass, char * host, char * agent, char * version, char * tls )
{
	return((struct oa_config *) 0);
}


	/* ----------- */
#endif 	/* _oaclient_c */
	/* ----------- */



