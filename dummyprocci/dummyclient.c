#ifndef	_dummyclient_c
#define	_dummyclient_c

#include "dummyclient.h"

public struct	dummy_response * dummy_liberate_response( struct dummy_response * rptr )
{
	if ( rptr )
	{
		rptr = liberate( rptr );
	}
	return((struct dummy_response *) 0);
}

/*	------------------------------------------------------------	*/
/*		d u m m y _ a l l o c a t e _ c o n f i g		*/
/*	------------------------------------------------------------	*/
public	struct	dummy_config * dummy_allocate_config()
{
	struct	dummy_config * sptr;
	if (!( sptr = (struct dummy_config *) allocate( sizeof( struct dummy_config ) ) ))
		return( sptr );
	else
	{
	 	memset( sptr, 0, sizeof( struct dummy_config ));
		return( sptr );
	}
}

public struct	dummy_config * dummy_initialise_client( char * user, char * pass, char * host, char * agent, char * version, char * tls )
{
	struct dummy_config * sptr=(struct dummy_config *) 0;

//	char *	tls;
//	char *	name;
//	char *	host;
//	char *	agent;
//	int	current;
//	char *	authentication;
//	char *	version;
//	char *	user;
//	char *	base;
//	char *	password;
//	char *	description;

	if (!( sptr = dummy_allocate_config()))
		return( sptr );
//	if (!( sptr->apikey = allocate_string( apikey )))
//		return(dummy_liberate_config( sptr ) );
//	if (!( sptr->apisec = allocate_string( apisec )))
//		return(dummy_liberate_config( sptr ));
//	if (!( sptr->host = allocate_string( host )))
//		return(dummy_liberate_config( sptr ));
//	else if (!( sptr->agent = allocate_string( agent )))
//		return(dummy_liberate_config( sptr ));
//	else if (!( sptr->version = allocate_string( version )))
//		return(dummy_liberate_config( sptr ));

	sptr->authentication = (char *) 0;
	if (!( tls ))
		sptr->tls = (char *) 0;
	else if ((sptr->tls = allocate_string(tls)) != (char *) 0)
		if ( (!( strlen( sptr->tls ) )) || ( *(sptr->tls) == '0' ) )
			sptr->tls = liberate( sptr->tls );

	return( sptr );

//	return((struct dummy_config *) 0); // TODO
}


	/* ----------- */
#endif 	/* _dummyclient_c */
	/* ----------- */



