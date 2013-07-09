#ifndef	_os_quantum_c
#define	_os_quantum_c

/*	-------------------------------------------	*/
/*	    O S  Q U A N T U M / N E U T R O N		*/
/*	-------------------------------------------	*/
/*	This file provides the high level contract	*/
/*	support functions for use when the quantum	*/
/*	networking interface is detected and is to 	*/
/*	be used instead of the essex floating ips	*/
/*	API which is no longer operational.		*/
/*	-------------------------------------------	*/

/*	-----------------------------------		*/
/*	   r e s o l v e _ j s o n _ a t b 		*/
/*	-----------------------------------		*/
private	char * resolve_json_atb( struct os_response * zptr, char * nptr )
{
	char * result=(char *) 0;
	if (!( zptr ))
		return( (char *) 0 );
	else if (!( zptr->jsonroot ))
		return( (char *) 0 );
	else if (!( result = json_atribut( zptr->jsonroot, nptr ) ))
		return( (char *) 0 );
	else	return( allocate_string( result ) );
}

/*	-----------------------------------		*/
/*	   r e s o l v e _ j s o n _ i d 		*/
/*	-----------------------------------		*/
private	char * resolve_json_id( struct os_response * zptr )
{
	return( resolve_json_atb( zptr, "id" ) );
}

/*	-----------------------------------------	*/
/*	r e s o l v e _ n a m e d _ n e t w o r k 	*/
/*	-----------------------------------------	*/
private	char *	resolve_named_network( struct os_response * zptr, char * nptr )
{
	struct data_element * dptr;
	struct data_element * eptr;
	char * vptr;

	if (!( zptr ))
		return( (char *) 0 );
	else if (!( zptr->jsonroot ))
		return( (char *) 0 );
	else if (!( eptr = json_element( zptr->jsonroot, "networks" ) ))
		return( (char *) 0 );
	else
	{
		for ( 	dptr=eptr->first;
			dptr != ( struct data_element *) 0;
			dptr = dptr->next )
		{	
			if (!( vptr = json_atribut( dptr, "name" ) ))
				continue;
			else if ( strcmp( vptr, nptr ) != 0 )
				continue;
			else if (!( vptr = json_atribut( dptr, "id" ) ))
				continue;
			else 	return( allocate_string( vptr ) );
		}
		return((char *) 0 );
	}
}

/*	------------------------------------	*/
/*	r e s o l v e _ s e r ve r _ p o r t 	*/
/*	------------------------------------	*/
private	char *	resolve_server_port( struct os_response * zptr, char * id )
{
	struct data_element * dptr;
	struct data_element * eptr;
	char * vptr;
	if (!( zptr ))
		return( (char *) 0 );
	else if (!( zptr->jsonroot ))
		return( (char *) 0 );
	else if (!( eptr = json_element( zptr->jsonroot, "networks" ) ))
		return( (char *) 0 );
	else
	{
		for ( 	dptr=eptr->first;
			dptr != ( struct data_element *) 0;
			dptr = dptr->next )
		{	
			if (!( vptr = json_atribut( dptr, "device_id" ) ))
				continue;
			else if ( strcmp( vptr, id ) != 0 )
				continue;
			else if (!( vptr = json_atribut( dptr, "id" ) ))
				continue;
			else 	return( allocate_string( vptr ) );
		}
		return((char *) 0 );
	}
}

/*	-----------------------------------		*/
/*	i s _ q u a n t u m _ n e t w o r k 	*/
/*	-----------------------------------		*/
public	is_quantum_network( struct os_subscription * sptr )
{
	return( sptr->KeyStone.quantum );
}
	
/*	-------------------------------------------	*/
/*	 r e s e t _ q u a  t u m  _ n e t w o r k	*/
/*	-------------------------------------------	*/
public	int reset_quantum_network( struct openstack * pptr )
{
	if ( pptr->port )
		pptr->port = liberate( pptr->port );
	if ( pptr->subnet )
		pptr->subnet = liberate( pptr->subnet );
	if ( pptr->address )
		pptr->address = liberate( pptr->address );
	return(0);
}

/*	-------------------------------------------	*/
/*	c r e a t e _ q u a  t u m  _ n e t w o r k	*/
/*	-------------------------------------------	*/
public	int create_quantum_network( struct os_subscription * sptr, struct openstack * pptr )
{
	struct	os_response * zptr=(struct os_response *) 0;

	char *	sid =(char *) 0;
	char *	rid =(char *) 0;

	/* -------------------------- */
	/* create the account network */
	/* -------------------------- */
	if (!( zptr = os_create_network( sptr, pptr->accountname, 1, 0 ) ))
		return(0);

	else if (!( pptr->privatenet = resolve_json_id( zptr ) ))
	{
		zptr = liberate_os_response( zptr );
		return(0);
	}
	else	zptr = liberate_os_response( zptr );

	/* --------------------- */
	/* create account subnet */
	/* --------------------- */
	if (!( zptr = os_create_subnet( sptr, pptr->accountname, pptr->privatenet, 4, "192.168.0.0/24" )))
		return(0);

	else if (!( sid = resolve_json_id( zptr )))
	{
		zptr = liberate_os_response( zptr );
		return(0);
	}
	else	zptr = liberate_os_response( zptr );

	/* --------------------- */
	/* create account router */
	/* --------------------- */
	if (!( zptr = os_create_router( sptr, pptr->accountname, pptr->publicnet, 1 ) ))
	{
		sid = liberate( sid );
		return(0);
	}
	else if (!( rid = resolve_json_id( zptr ) ))
	{
		zptr = liberate_os_response( zptr );
		sid = liberate( sid );
		return(0);
	}
	else	zptr = liberate_os_response( zptr );

	/* ----------------------------------------- */
	/* connect account router to account network */
	/* ----------------------------------------- */
	if (!( zptr = os_create_interface( sptr, rid, sid ) ))
	{
		sid = liberate( sid );
		rid = liberate( rid );
		return(0);
	}
	else
	{
		zptr = liberate_os_response( zptr );
		sid = liberate( sid );
		rid = liberate( rid );
		return( 1 );
	}
}

/*	---------------------------------------------	*/
/*	r e s o l v e _ q u a n t u m _ n e t w o r k	*/
/*	---------------------------------------------	*/
/*	 this can be called from the create contract	*/
/*	 function prior to the resolution of flavors	*/
/*	 and images.					*/
/*	---------------------------------------------	*/

public	int resolve_quantum_network( struct os_subscription * sptr, struct openstack * pptr )
{

	struct	os_response * zptr=(struct os_response *) 0;

	if (!( pptr->publicnet ))
	{
		/* --------------------------- */
		/* resolve network identifiers */
		/* --------------------------- */
		if (!( zptr = os_list_networks( sptr )))
			return( 0 );

		/* ------------------------ */
		/* first the public network */
		/* ------------------------ */
		else if (!( pptr->publicnet = resolve_named_network( zptr, "public" ) ))
		{
			zptr = liberate_os_response( zptr );
			return( 0 );
		}

		if (!( pptr->privatenet ))
		{
			/* ------------------------------------------ */
			/* then resolve or create the private network */
			/* ------------------------------------------ */
			if (!( pptr->privatenet = resolve_named_network( zptr, pptr->accountname ) ))
			{
				if (!( create_quantum_network( sptr, pptr ) ))
				{
					zptr = liberate_os_response( zptr );
					return( 0 );
				}
			}	
			else	zptr = liberate_os_response( zptr );
		}
		else	zptr = liberate_os_response( zptr );
	}

	/* ----------------------------------------- */
	/* private and public network identifiers ok */
	/* ----------------------------------------- */
	return( 1 );

}

/*	---------------------------------------------	*/
/*	c o n n e c t _ q u a n t u m _ n e t w o r k	*/
/*	---------------------------------------------	*/
public	int	connect_quantum_network( struct os_subscription * sptr, struct openstack * pptr )
{
	struct	os_response * zptr;
	int	status=0;
	
	if (!( zptr = os_list_ports( sptr )))
		return( 0 );
	else if (!( pptr->port = resolve_server_port( zptr, pptr->number ) ))
	{
		/* --------------------------- */
		/* create local port to server */
		/* --------------------------- */
		if (!( zptr = os_create_port(sptr, pptr->id, pptr->privatenet, pptr->number, pptr->group, 1 )))
			return( 0 );
		else if (!( pptr->port = resolve_json_id( zptr ) ))
		{
			zptr = liberate_os_response( zptr );
			return( 0 );
		}
		else	zptr = liberate_os_response( zptr );
	}
	else	zptr = liberate_os_response( zptr );

	/* --------------------------- */
	/* create ip on public netowrk */
	/* --------------------------- */
	if (!( zptr = os_create_floatingip( sptr, pptr->publicnet, pptr->port ) ))
		return( 0 );

	else if (!( pptr->privateaddr = resolve_json_atb( zptr, "fixed_ip_address" )))
	{
		zptr = liberate_os_response( zptr );
		return( 0 );
	}

	else if (!( pptr->publicaddr = resolve_json_atb( zptr, "floating_ip_address" )))
	{
		zptr = liberate_os_response( zptr );
		return( 0 );
	}

	else if ((status = os_set_host_address( pptr )) != 0)
	{
		zptr = liberate_os_response( zptr );
		return( 0 );
	}

	else if (!( pptr->address = resolve_json_id( zptr ) ))
	{
		zptr = liberate_os_response( zptr );
		return( 0 );
	}
	else	zptr = liberate_os_response( zptr );

	return( 1 );

}

/*	---------------------------------------------------	*/
/*	d i s c o n n e c t _ q u a n t u m _ n e t w o r k	*/
/*	---------------------------------------------------	*/
public	int	disconnect_quantum_network( struct os_subscription * sptr, struct openstack * pptr )
{
	struct	os_response * zptr;
	int	status=0;

	if ( pptr->address )
	{
		/* ----------------------------- */
		/* delete ip from public netowrk */
		/* ----------------------------- */
		if (( zptr = os_delete_floatingip( sptr, pptr->address )) != (struct os_response*) 0)
			zptr = liberate_os_response( zptr );

		if ( pptr->address )
			pptr->address = liberate( pptr->address );

		if ( pptr->privateaddr )
			pptr->privateaddr = liberate( pptr->privateaddr );

		if ( pptr->publicaddr )
			pptr->publicaddr = liberate( pptr->publicaddr );

		if ( pptr->hostname )
			pptr->hostname = liberate( pptr->hostname );
	}

	if ( pptr->port )
	{

		/* --------------------------- */
		/* delete local port to server */
		/* --------------------------- */
		if (( zptr = os_delete_port(sptr, pptr->port)) != (struct os_response *) 0)
			zptr = liberate_os_response( zptr );


		if ( pptr->port )
			pptr->port = liberate( pptr->port );
	}

	return( 1 );
}

	/* ------------- */
#endif	/* _os_quantum_c */
	/* ------------- */
