/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>     */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License"); 	*/
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at 				*/
/*  									*/
/*  http://www.apache.org/licenses/LICENSE-2.0 				*/
/*  									*/
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 	*/
/* implied. 								*/
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License. 					*/
/* -------------------------------------------------------------------- */
#ifndef	_dcaction_c
#define	_dcaction_c

/*	------------------------------------------	*/
/*		d c _ v a l i d _ p r i c e		*/
/*	------------------------------------------	*/
private	int	dc_valid_price( char * price )
{
	if (!( price ))
		return(0);
	else if (!( strlen( price )))
		return( 0 );
	else if (!( strcmp( price, _CORDS_NULL ) ))
		return( 0 );
	else	return( 1 );
}	

/*	------------------------------------------	*/
/*		d c _ v a l i d _ a d d r e s s		*/
/*	------------------------------------------	*/
private	int	dc_valid_address( char * address )
{
	if (!( address ))
		return(0);
	else if (!( strlen( address )))
		return( 0 );
	else if (!( strcmp( address, _CORDS_NULL ) ))
		return( 0 );
	else	return( 1 );
}	

private	int	build_deltacloud_firewall( struct deltacloud * pptr )
{
	return(0);
}

/*	-------------------------------------------	*/
/* 	      s t a r t  _ d e l t a c l o u d	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * start_deltacloud(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	int	status=0;
	char *	filename=(char *) 0;
	struct	deltacloud * pptr;
	struct	dc_config *  kptr;
	struct	rest_response * zptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 400, "Invalid Request" ) );
	else if ( pptr->state != _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if (!( kptr = use_deltacloud_configuration( pptr->profile )))
	 	return( rest_html_response( aptr, 400, "Incorrect Configuration" ) );
	else if ((status = build_deltacloud_firewall( pptr )) != 0)
		return( rest_html_response( aptr, 4002, "Server Failure : Firewall Preparation" ) );
	else if (!( filename = dc_create_instance_message( pptr->name, pptr->image, pptr->flavor, pptr->firewall, pptr->zone ) ))
	 	return( rest_html_response( aptr, 4004, "Server Failure : Create Server Message" ) );
	else if (!( zptr = dc_create_instance( filename )))
	 	return( rest_html_response( aptr, 4008, "Server Failure : Create Server Request" ) );
	else if ( zptr->status >= 400 )
	{
		aptr = rest_html_response( aptr, zptr->status + 4000, "Bad Request : Create Server No Response" );
		zptr = liberate_rest_response( zptr );
		return( aptr );
	}
	else
	{
		liberate( filename );
		pptr->state = _OCCI_RUNNING;
		zptr = liberate_rest_response( zptr );
		if (!( status ))
		{
			if (!( dc_valid_price( pptr->price ) ))
				return( rest_html_response( aptr, 200, "OK" ) );
			else if ( occi_send_transaction( _CORDS_DELTACLOUD, pptr->price, "action=start", pptr->account, pptr->id ) )
				return( rest_html_response( aptr, 200, "OK" ) );
			else	return( rest_html_response( aptr, 200, "OK" ) );
		}
		else  	return( rest_html_response( aptr, 4256, "Server Failure : Connect DeltaCloud" ) );
	}
}

/*	-------------------------------------------	*/
/* 	      s t o p _ d e l t a c l o u d	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * stop_deltacloud(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	deltacloud * pptr;
	struct	dc_config *  kptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 400, "Invalid Request" ) );
	else if ( pptr->state == _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if (!( kptr = use_deltacloud_configuration( pptr->profile )))
	 	return( rest_html_response( aptr, 400, "Incorrect Configuration" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	      s a v e  _ d e l t a c l o u d	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * save_deltacloud(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	deltacloud * pptr;
	struct	dc_config *  kptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 400, "Invalid Request" ) );
	else if ( pptr->state == _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if (!( kptr = use_deltacloud_configuration( pptr->profile )))
	 	return( rest_html_response( aptr, 400, "Incorrect Configuration" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	    s n a p s h o t _ d e l t a c l o u d  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * snapshot_deltacloud(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	deltacloud * pptr;
	struct	dc_config *  kptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 400, "Invalid Request" ) );
	else if ( pptr->state == _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if (!( kptr = use_deltacloud_configuration( pptr->profile )))
	 	return( rest_html_response( aptr, 400, "Incorrect Configuration" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}


	/* ----------- */
#endif	/* _dcaction_c */
	/* ----------- */

