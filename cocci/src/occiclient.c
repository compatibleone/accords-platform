/* ------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                   */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>    */
/* --------------------------------------------------------------------*/
/*  This is free software; you can redistribute it and/or modify it    */
/*  under the terms of the GNU Lesser General Public License as        */
/*  published by the Free Software Foundation; either version 2.1 of   */
/*  the License, or (at your option) any later version.                */
/*                                                                     */
/*  This software is distributed in the hope that it will be useful,   */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of     */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   */
/*  Lesser General Public License for more details.                    */
/*                                                                     */
/*  You should have received a copy of the GNU Lesser General Public   */
/*  License along with this software; if not, write to the Free        */
/*  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA */
/*  02110-1301 USA, or see the FSF site: http://www.fsf.org.           */
/* --------------------------------------------------------------------*/
#ifndef	_occi_client_c
#define	_occi_client_c

#include "occiclient.h"
#include "occic.c"
#include "occim.c"
#include "occiauth.h"

/*	------------------------------------------------------------	*/
/*		o c c i    m a n a g e r    s t r u c t u r e		*/
/*	------------------------------------------------------------	*/
private	struct	occi_manager OcciManager = 
	{
		(struct rest_header *) 0,
		(struct occi_client *) 0,
		(struct occi_client *) 0,
		"CO-OCCI-MAN",
		_OCCI_MIME_OCCI,
		0,
		1	/* optimised */
	};

#include "occiauth.c"

/*	-----------------------------------------------------------------	*/
/*	o c c i _ c h e c k _ r e s p o n s e _ a u t h o r i z a t i o n	*/
/*	-----------------------------------------------------------------	*/
private	int	occi_check_response_authorization( struct rest_response * rptr )
{
	struct	rest_header * hptr;
	if (!( hptr = rest_resolve_header( rptr->first, _OCCI_AUTHORIZE )))
		return( 1 );
	else if (!( hptr->value ))
		return( 1 );
	else 	return( occi_resolve_authorization( hptr->value ) );
}

/*	------------------------------------------------------------	*/
/*		   o c c i _ a d d _ d e f a u l t _ h e a d e r	*/
/*	------------------------------------------------------------	*/
private	struct	rest_header * occi_add_default_header(struct rest_header  * hptr)
{
	struct	rest_header * rptr;

	if (!( rptr = OcciManager.headers ))
		OcciManager.headers = hptr;
	else
	{
		while ( rptr->next != (struct rest_header *) 0)
			rptr = rptr->next;

		rptr->next = hptr;
	}
	return( hptr );
}

/*	------------------------------------------------------------	*/
/*	    o c c i _ c l i e n t _ a u t h e n t i c a t i o n		*/
/*	------------------------------------------------------------	*/
public	struct	rest_header * occi_client_authentication( char * aptr )
{
	struct	rest_header * hptr;
	if (!( hptr = rest_resolve_header( OcciManager.headers, _OCCI_AUTHORIZE ) ))
	{
		if (!( hptr = rest_create_header( _OCCI_AUTHORIZE, aptr ) ))
			return( hptr );
		else	return( occi_add_default_header( hptr ) );
	}
	else
	{
		if ( hptr->value )
			liberate( hptr->value );
		else	hptr->value = allocate_string( aptr );
		return( hptr );
	}
}

/*	------------------------------------------------------------	*/
/*	    o c c i _ c l i e n t _ a c c e p t  			*/
/*	------------------------------------------------------------	*/
public	struct	rest_header * occi_client_accept( char * aptr )
{
	struct	rest_header * hptr;

	if (!( hptr = rest_resolve_header( OcciManager.headers, _HTTP_ACCEPT ) ))
	{
		if (!( hptr = rest_create_header( _HTTP_ACCEPT, aptr ) ))
			return( hptr );
		else	return( occi_add_default_header( hptr ) );
	}
	else
	{
		if ( hptr->value )
			liberate( hptr->value );
		else	hptr->value = allocate_string( aptr );
		return( hptr );
	}
}

/*	------------------------------------------------------------	*/
/*	    o c c i _ c l i e n t _ c o n t e n t _ t y p e		*/
/*	------------------------------------------------------------	*/
public	struct	rest_header * occi_client_content_type( char * aptr )
{
	struct	rest_header * hptr;

	if (!( hptr = rest_resolve_header( OcciManager.headers, _HTTP_CONTENT_TYPE ) ))
	{
		if (!( hptr = rest_create_header( _HTTP_CONTENT_TYPE, aptr ) ))
			return( hptr );
		else	return( occi_add_default_header( hptr ) );
	}
	else
	{
		if ( hptr->value )
			liberate( hptr->value );
		else	hptr->value = allocate_string( aptr );
		return( hptr );
	}
}

/*	------------------------------------------------------------	*/
/*		   o c c i _ r e m o v e _ e l e m e n t		*/
/*	------------------------------------------------------------	*/
public	struct	occi_element  *	occi_remove_element(struct occi_element * eptr)
{
	if ( eptr )
	{
		if ( eptr->name )
			liberate( eptr->name );
		if ( eptr->value )
			liberate( eptr->value );
		liberate( eptr );
	}
	return((struct occi_element *) 0);
}

/*	------------------------------------------------------------	*/
/*		   o c c i _ r e m o v e _ e l e m e n t s		*/
/*	------------------------------------------------------------	*/
public	struct	occi_element  *	occi_remove_elements(struct occi_element * eptr)
{
	struct	occi_element * fptr;
	while ((fptr = eptr) != (struct occi_element *) 0)
	{
		eptr = eptr->next;
		fptr = occi_remove_element( fptr );
	}
	return((struct occi_element *) 0);
}

/*	------------------------------------------------------------	*/
/*		a l l o c a t e _ o c c i _ e l e m e n t		*/
/*	------------------------------------------------------------	*/
public	struct	occi_element  *	allocate_occi_element()
{
	struct	occi_element * eptr;
	if (!( eptr = allocate( sizeof( struct occi_element ) )))
		return( eptr );
	else
	{
		eptr->name = eptr->value = (char *) 0;
		eptr->previous = eptr->next = (struct occi_element *) 0;
		return( eptr );
	}
}

/*	------------------------------------------------------------	*/
/*		   o c c i _ c r e a t e _ e l e m e n t		*/
/*	------------------------------------------------------------	*/
public	struct	occi_element  *	occi_create_element( char * nptr, char * vptr )
{
	struct	occi_element * eptr;
	if (!( eptr = allocate_occi_element() ))
		return( eptr );
	else if (!( eptr->name = allocate_string( nptr )))
		return( occi_remove_element( eptr ) );
	else if (!( eptr->value = allocate_string( vptr )))
		return( occi_remove_element( eptr ) );
	else	return( eptr );
}

/*	------------------------------------------------------------	*/
/*		o c c i _ r e q u e s t _ e l e m e n t			*/
/*	------------------------------------------------------------	*/
public	struct	occi_element  *	occi_request_element(struct occi_request * rptr, char * nptr, char * vptr )
{
	struct	occi_element * eptr;
	if (!( eptr = occi_create_element( nptr, vptr ) ))
		return( eptr );
	else	
	{
		if (!( eptr->previous = rptr->last ))
			rptr->first = eptr;
		else 	eptr->previous->next = eptr;
		return((rptr->last = eptr));
	}
}

/*	-----------------------------------------------------------	*/
/*	   o c c i _ r e q u e s t _ f r o m _ r e s p o n s e 		*/
/*	-----------------------------------------------------------	*/
public	struct	occi_element *	occi_request_from_response(
			struct	occi_request * qptr,
			struct 	occi_response * rptr)
{
	struct	occi_element * eptr=(struct occi_element *) 0;;
	struct	occi_element * fptr;
	for (	fptr=rptr->first;
		fptr != (struct occi_element *) 0;
		fptr = fptr->next )
	{
		if (!( fptr->name ))
			continue;
		else if (!( fptr->value ))
			continue;
		else if (!( eptr = occi_request_element( qptr, fptr->name, fptr->value )))
			break;
		else	continue;
	}
	return( qptr->first );
}

/*	------------------------------------------------------------	*/
/*		o c c i _ r e s p o n s e _ e l e m e n t		*/
/*	------------------------------------------------------------	*/
public	struct	occi_element  *	occi_response_element(struct occi_response * rptr, char * nptr, char * vptr )
{
	struct	occi_element * eptr;
	if (!( eptr = occi_create_element( nptr, vptr ) ))
		return( eptr );
	else	
	{
		if (!( eptr->previous = rptr->last ))
			rptr->first = eptr;
		else 	eptr->previous->next = eptr;
		return((rptr->last = eptr));
	}
}

/*	------------------------------------------------------------	*/
/*		o c c i _ r e s ol v e _ c a t e g o r y 		*/
/*	------------------------------------------------------------	*/
private	struct	occi_category * occi_resolve_category( struct occi_category * cptr, char * nptr )
{
	for (	;
		cptr != (struct occi_category *) 0;
		cptr = cptr->next )
	{
		if (!( cptr->id ))
			continue;
		else if (!( strcmp( cptr->id, nptr ) ))
			break;
		else if (!( strncmp( nptr, cptr->location, strlen(cptr->location) ) ))
			break;
	}
	return( cptr );
}

/*	------------------------------------------------------------	*/
/*		 o c c i _ r e m o v e _ r e q u e s t 			*/
/*	------------------------------------------------------------	*/
public	struct	occi_request  *	occi_remove_request( struct occi_request * rptr )
{
	struct	occi_element * eptr;
	if ( rptr )
	{
		while ((eptr = rptr->first) != (struct occi_element *) 0)
		{
			rptr->first = eptr->next;
			occi_remove_element( eptr );
		}
		if ( rptr->name )
			rptr->name = liberate( rptr->name );
		if ( rptr->host )
			rptr->host = liberate( rptr->host );
		liberate( rptr );
	}
	return((struct occi_request *) 0);
}

/*	------------------------------------------------------------	*/
/*	     o c c i _ s e t _ r e q u e s t _ a c c o u n t		*/
/*	------------------------------------------------------------	*/
/*	set the transaction account field of an OCCI request for use	*/
/*	as an X-OCCI-ACCOUNT field for the emission of transactions	*/
/*	------------------------------------------------------------	*/
public	struct	occi_request  *	occi_set_request_account( struct occi_request * rptr, char * account )
{
	if (!( rptr ))
		return( rptr );
	if ( rptr->account )
		rptr->account = liberate( rptr->account );
	if (!( account ))
		return( rptr );
	else if (!( rptr->account = allocate_string( account ) ))
		return( occi_remove_request( rptr ) );
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		 o c c i _ c r e a t e _ r e q u e s t 			*/
/*	------------------------------------------------------------	*/
public	struct	occi_request  *	occi_create_request( struct occi_client * cptr, char * category, int type )
{
	struct	occi_request * rptr;
	if (!( rptr = allocate( sizeof( struct occi_request ) )))
		return( rptr );
	{
		rptr->host = rptr->name = (char *) 0;
		rptr->first = rptr->last = (struct occi_element *) 0;
		rptr->category = (struct occi_category *) 0;
		rptr->account = (char *) 0;
		rptr->type = type;
		if (!( rptr->host = allocate_string( cptr->host )))
			return( occi_remove_request( rptr ) );
		else if (!( rptr->name = allocate_string( category ) ))
			return( occi_remove_request( rptr ) );
		else if (!( rptr->category = occi_resolve_category( cptr->firstcat, category ) ))
			return( occi_remove_request( rptr ) );
		return( rptr );
	}
}

/*	------------------------------------------------------------	*/
/*		 o c c i _ r e m o v e _ r e s p o n s e		*/
/*	------------------------------------------------------------	*/
public	struct	occi_response *	occi_remove_response( struct occi_response * aptr )
{
	struct	occi_element * eptr;
	if ( aptr )
	{
		while ((eptr = aptr->first) != (struct occi_element *) 0)
		{
			aptr->first = eptr->next;
			occi_remove_element( eptr );
		}
		if ( aptr->response )
			liberate_rest_response( aptr->response );
		if ( aptr->name )
			aptr->name = liberate( aptr->name );
		if ( aptr->host )
			aptr->host = liberate( aptr->host );
		liberate( aptr );
	}
	return((struct occi_response *) 0);
}

/*	------------------------------------------------------------	*/
/*		 a l l o c a t e _ o c c i _ r e s p o n s e		*/
/*	------------------------------------------------------------	*/
public	struct	occi_response * allocate_occi_response()
{
	struct	occi_response * aptr;
	if (!( aptr = allocate( sizeof( struct occi_response ) )))
		return( aptr );
	else
	{
		aptr->name = (char *) 0;
		aptr->host = (char *) 0;
		aptr->first = aptr->last = (struct occi_element *) 0;
		aptr->category = (struct occi_category *) 0;
		aptr->response = (struct rest_response *) 0;
		aptr->type = 0;
		return( aptr );
	}
}

/*	------------------------------------------------------------	*/
/*		 o c c i _ c r e a t e _ r e s p o n s e		*/
/*	------------------------------------------------------------	*/
public	struct	occi_response *	occi_create_response( 
	struct occi_request * rptr, 
	struct rest_response * zptr )
{
	struct	rest_header * hptr;
	struct	occi_element * eptr;
	struct	occi_response * aptr;
	char	*	nptr;
	char	*	vptr;

	if ((!( zptr )) || (!( rptr )))
		return((struct occi_response *) 0);
	else if (!( occi_check_response_authorization( zptr ) ))
		return((struct occi_response *) 0);
	else if (!( aptr = allocate_occi_response() ))
		return( aptr );
	else if (!( aptr->name = allocate_string( rptr->name ) ))
		return( occi_remove_response( aptr ) );
	else if (!( aptr->host = allocate_string( rptr->host ) ))
		return( occi_remove_response( aptr ) );
	else
	{
		aptr->type = rptr->type;
		aptr->response = zptr;
		aptr->category = rptr->category;
		for (	hptr=zptr->first;
			hptr != (struct rest_header *) 0;
			hptr = hptr->next )
		{
			if (!( hptr->name ))
				continue;
			else if (!( strcasecmp( hptr->name, _OCCI_ATTRIBUTE )))
			{
				if (!( nptr = allocate_string( hptr->value )))
					continue;
				else
				{
					for ( vptr=nptr; *vptr != 0; vptr++ )
						if ( *vptr == '=' )
							break;
					if ( *vptr == '=' )
						*(vptr++) = 0;
					if (!(eptr=occi_response_element(aptr,nptr,vptr)))
						break;
				}
			}
			else if (!( strcasecmp( hptr->name, _OCCI_LOCATION )))
			{
				if (!(eptr=occi_response_element(aptr,rptr->name,hptr->value)))
					break;
			}
			else if (!( strcasecmp( hptr->name, _OCCI_LINKHEAD )))
			{
				if (!(eptr=occi_response_element(aptr,"link",hptr->value)))
					break;
			}
		}
		return( aptr );
	}
}

/*	------------------------------------------------------------	*/
/*	    o c c i _ c l i e n t _ d r o p _ c a t e g o r i e s	*/
/*	------------------------------------------------------------	*/
public	struct	occi_category *	occi_client_drop_categories( struct occi_client * cptr )
{
	struct	occi_category * optr;
	while ((optr = cptr->firstcat) != (struct occi_category *) 0)
	{
		cptr->firstcat = optr->next;
		liberate_occi_category( optr );
	}
	return((cptr->firstcat = cptr->lastcat = (struct occi_category *) 0));
}


/*	------------------------------------------------------------	*/
/*	     o c c i _ c l i e n t _ a d d _ c a t e g o r i e s	*/
/*	------------------------------------------------------------	*/
private	struct occi_category * 	occi_client_add_category( 
		struct occi_client * cptr,
		char * sptr )
{
	struct	occi_category * optr;
	if (!( cptr ))
		return((struct occi_category *) 0);
	else if (!( sptr ))
		return((struct occi_category *) 0);
	else if (!( optr = occi_parse_category( sptr ) ))
		return( optr );
	else
	{
		cptr->categories++;
		if (!( optr->previous = cptr->lastcat ))
			cptr->firstcat = optr;
		else	optr->previous->next = optr;
		return((cptr->lastcat = optr));
	}
}

/*	------------------------------------------------------------	*/
/*			o c c i _ d e l e t e _ c l i e n t		*/
/*	------------------------------------------------------------	*/
public	struct	occi_client *	occi_delete_client( struct occi_client * cptr )
{
	occi_client_drop_categories( cptr );
	liberate_url( cptr->target );
	return( drop_occi_client( cptr ) );
}

/*	------------------------------------------------------------	*/
/*			o c c i _ f l u s h _ c l i e n t		*/
/*	------------------------------------------------------------	*/
public	struct	occi_client *	occi_flush_client( char * host, int port )
{
	struct	occi_client *	cptr;
	for (	cptr=OcciManager.first;
		cptr != (struct occi_client *) 0;
		cptr = cptr->next )
	{
		if ( cptr->target->port != port )
			continue;
		else if (!( strcmp( cptr->target->host, host ) ))
			break;
	}
	if (!( cptr ))
		return( cptr );
	else	return( occi_delete_client( cptr ) );
}

/*	------------------------------------------------------------	*/
/*			o c c i _ r e m o v e _ c l i e n t		*/
/*	------------------------------------------------------------	*/
public	struct	occi_client *	occi_remove_client( struct occi_client * cptr )
{
	if ( OcciManager.optimise )
		return((struct occi_client *) 0);
	else	return( occi_delete_client( cptr ) );
}

/*	------------------------------------------------------------	*/
/*			o c c i _ c l i e n t _ u r i			*/
/*	------------------------------------------------------------	*/
public	char * occi_client_uri( struct occi_client * cptr, char * object )
{
	sprintf(cptr->buffer,"%s://%s:%u%s",
		cptr->target->service,
		cptr->target->host,
		cptr->target->port,
		( object ? object : cptr->target->object ));
	return( allocate_string( cptr->buffer ) );
}

/*	------------------------------------------------------------	*/
/*		    o c c i _ a p p e n d _ d e f a u l t 		*/
/*	------------------------------------------------------------	*/
/*	this function appends the list of default headers that are	*/
/*	needed to transmit security and athorization information.	*/
/*	The headers have to be duplicated to be	sure that no cross 	*/	
/*	memory allocations occur !					*/
/*	------------------------------------------------------------	*/
public	struct	rest_header   * occi_append_default( 
			struct 	occi_client  * cptr,
			struct 	rest_header  * root,
			struct 	rest_header  * hptr )
{
	struct	rest_header * wptr;
	struct	rest_header * last;

	if ((last = root) != (struct rest_header *) 0)
		while ( last->next ) 
			last = last->next;

	for (	;
		hptr != (struct rest_header *) 0;
		hptr = hptr->next )
	{
		if (!( wptr = rest_create_header( hptr->name, hptr->value ) ))
			return( wptr );
		else if (!( wptr->previous = last ))
			root = wptr;
		else 	wptr->previous->next = wptr;
		last = wptr;
	}
	return( root );
}

/*	------------------------------------------------------------	*/
/*		    o c c i _ l o a d _ c a t e g o r i e s		*/
/*	------------------------------------------------------------	*/
public	struct	occi_client *	occi_load_categories( struct occi_client * cptr )
{
	char *	uri;
	struct	rest_header * hptr=(struct rest_header *) 0;
	struct	rest_response * rptr;

	if ( check_debug() )
		printf("   OCCI Client : Load Categories\n");

	if (!( uri = occi_client_uri( cptr, "/-/" ) ))
		return( occi_delete_client( cptr ) );

	else if ((OcciManager.headers)
	     &&  (!( hptr = occi_append_default( cptr, hptr, OcciManager.headers ) )))
		return( occi_delete_client( cptr ) );

	else if (!( rptr = rest_client_get_request(uri,cptr->tls,cptr->agent,hptr ) ))
	{
		liberate( uri );
		return( occi_delete_client( cptr ) );
	}
	else if ( rptr->status >= 300 )
	{
		liberate( uri );
		return( occi_delete_client( cptr ) );
	}
	else	liberate( uri );

	if ( check_debug() )
		printf("   OCCI Client : %s %u %s \n",rptr->version,rptr->status,rptr->message);

	for (	hptr=rptr->first;
		hptr != (struct rest_header *) 0;
		hptr = hptr->next )
	{
		if (!( hptr->name ))
			continue;
		else if (!( hptr->value ))
			continue;
		else if ( strcasecmp( hptr->name, _OCCI_CATEGORY ) != 0 )
			continue;
		else if (!( occi_client_add_category( cptr, hptr->value ) ))
		{
			cptr = occi_delete_client( cptr );
			break;
		}
	}
	rptr = liberate_rest_response( rptr );
	return( cptr );
}

/*	------------------------------------------------------------	*/
/*			o c c i _ r e d i r e c t _ c l i e n t		*/
/*	------------------------------------------------------------	*/
public	struct	occi_client   *	occi_redirect_client( struct occi_client * cptr, char * host )
{
	if ( cptr->target )
		cptr->target = liberate_url( cptr->target );

	if ( cptr->host )
		cptr->host = liberate( cptr->host );

	if (!( cptr->host = allocate_string( host ) ))
		return( occi_remove_client( cptr ) );

	else if (!( cptr->target = analyse_url( cptr->host ) ))
		return( occi_remove_client( cptr ) );

	else if (!( cptr->target = validate_url( cptr->target ) ))
		return( occi_remove_client( cptr ) );

	else	return( cptr );
}

/*	------------------------------------------------------------	*/
/*		    o c c i _ r e s o l v e _ c l i e n t		*/
/*	------------------------------------------------------------	*/
public	struct	occi_client *	occi_resolve_client( char * host )
{
	struct	url	*	uptr;
	struct	occi_client *	cptr;
	if (!( uptr = analyse_url( host )))
		return((struct occi_client *) 0);
	else if (!( uptr = validate_url( uptr )))
		return((struct occi_client *) 0);
	for (	cptr=OcciManager.first;
		cptr != (struct occi_client *) 0;
		cptr = cptr->next )
	{
		if ( cptr->target->port != uptr->port )
			continue;
		else if (!( strcmp( cptr->target->host, uptr->host ) ))
			break;
	}
	uptr = liberate_url( uptr );
	return( cptr );
}

/*	------------------------------------------------------------	*/
/*			o c c i _ c r e a t e _ c l i e n t		*/
/*	------------------------------------------------------------	*/
public	struct	occi_client *	occi_create_client( char * host, char * agent, char * tls )
{
	struct	url	    *	uptr;
	struct	occi_client *	cptr;

	if ( check_debug() )
	{
		printf("OCCI CREATE CLIENT { \n\thost=%s,\n\tagent=%s,\n\ttls=%s };\n",host,agent,(tls ? tls : ""));
	}

	if ( OcciManager.optimise )
		if ((cptr = occi_resolve_client( host )) != (struct occi_client *) 0)
			return( occi_redirect_client( cptr, host ) );

	if (!( cptr = add_occi_client( &OcciManager ) ))
		return( cptr );
	else
	{
		cptr->firstcat = cptr->lastcat = (struct occi_category *) 0;
		cptr->target = (struct url *) 0;
	}

	if (!( cptr->host = allocate_string( host ) ))
		return( occi_delete_client( cptr ) );

	else if ((tls)
	     &&  (!( cptr->tls = allocate_string( tls ) )))
		return( occi_delete_client( cptr ) );

	else if (!( cptr->agent = allocate_string( agent ) ))
		return( occi_delete_client( cptr ) );

	else if (!( cptr->target = analyse_url( cptr->host ) ))
		return( occi_delete_client( cptr ) );

	else if (!( cptr->target = validate_url( cptr->target ) ))
		return( occi_delete_client( cptr ) );

	else	return( occi_load_categories( cptr ) );
}

/*	------------------------------------------------------------	*/
/*			o c c i _ r e q u e s t _ h e a d e r s		*/
/*	------------------------------------------------------------	*/
/*	this function will return an eventual list of rest headers	*/
/*	prior to the rest method invocation. these headers will have 	*/
/*	been prepared on the occi element list first and last.  	*/
/*	------------------------------------------------------------	*/
private	struct	rest_header * occi_request_headers( 
		struct occi_client * cptr, 
		struct occi_request * rptr )
{
	struct	occi_element * eptr;
	struct	rest_header * hptr=(struct rest_header *) 0;
	struct	occi_attribute * aptr;
	struct	rest_header * root;
	struct	rest_header * foot;
	root = foot = (struct	rest_header *) 0;
	if (!( rptr ))
		return( hptr );

	if ( check_debug() )
	{
		if (!( rptr->category ))
			printf("occi_request_headers( **no category** )\n");
		else	printf("occi_request_headers( %s )\n",rptr->category->id);
	}

	if (!( hptr = allocate_rest_header()))
		return( hptr );
	else if (!( hptr->name = allocate_string( _OCCI_CATEGORY )))
		return( liberate_rest_header( hptr ) );
	else if (!( hptr->value = occi_http_category( rptr->category ) ))
		return( liberate_rest_header( hptr ) );
	else	root = foot = hptr;

	if ( check_debug() )
		printf("occi_request_header(%s: %s)\n",hptr->name,hptr->value);

	for (	eptr=rptr->first;
		eptr != (struct occi_element *) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( aptr = occi_resolve_attribute( rptr->category, eptr->name ) ))
			continue;
		else if (!( hptr = allocate_rest_header()))
			break;
		else if (!( hptr->name = allocate_string( _OCCI_ATTRIBUTE )))
		{
			liberate_rest_header( hptr );
			break;
		}
		else if (!( hptr->value = occi_http_attribute_value( rptr->category, aptr, eptr->value ) ))
		{
			liberate_rest_header( hptr );
			break;
		}
		else
		{
			if ( check_debug() )
				printf("occi_request_header(%s: %s)\n",hptr->name,hptr->value);
			if (!( hptr->previous = foot ))
				root = hptr;
			else 	hptr->previous->next = hptr;
			foot = hptr;
			continue;
		}
		
	}			
	return( root );
}

/*	------------------------------------------------------------	*/
/*		    o c c i _ a p p e n d _ h e a d e r s		*/
/*	------------------------------------------------------------	*/
/*	this function appends the list of headers provided as its 	*/
/*	second parameter to the list of headers of the request in	*/
/*	the first parameter. the headers have to be duplicated to be	*/
/*	sure that no cross memory allocations occur !			*/
/*	------------------------------------------------------------	*/
public	struct	occi_element  * occi_append_headers( 
			struct 	occi_request * rptr,
			struct 	occi_element  * hptr )
{
	struct	occi_element * root=hptr;
	struct	occi_element * wptr;
	for (	;
		hptr != (struct occi_element *) 0;
		hptr = hptr->next )
	{
		if (!( wptr = occi_create_element( hptr->name, hptr->value ) ))
			return(( rptr->first=rptr->last=occi_remove_elements( rptr->first )));
		else if (!( wptr->previous = rptr->last ))
			rptr->first = wptr;
		else 	wptr->previous->next = wptr;
		rptr->last = wptr;
	}
	return( root );
}

/*	------------------------------------------------------------	*/
/*		    o c c i _ a p p e n d _ a c c o u n t		*/
/*	------------------------------------------------------------	*/
private	struct	rest_header * occi_append_account( 
		struct occi_client * cptr, 
		struct rest_header * root, 
		char * account )
{
	struct	rest_header * wptr;
	struct	rest_header * last;

	if ((last = root) != (struct rest_header *) 0)
		while ( last->next ) 
			last = last->next;

	if (!( wptr = rest_create_header( _OCCI_ACCOUNT, account ) ))
		return( wptr );
	else if (!( wptr->previous = last ))
		root = wptr;
	else 	wptr->previous->next = wptr;
	last = wptr;

	return( root );
}
	
/*	------------------------------------------------------------	*/
/*		     o c c i _ j s o n _ b o d y 			*/
/*	------------------------------------------------------------	*/
private	char *	occi_json_body( 
		struct occi_client * cptr, 
		struct occi_request * rptr,
		struct rest_header  * hptr,
		char * body )
{
	FILE *	h;
	char *	filename;
	char *	message;

	if (!( filename = rest_temporary_filename( ".json" ) ))
		return( body );

	else  if (!( message = occi_json_category( rptr->category ) ))
		return( body );

	else if (!( h = fopen(filename,"w")))
	{
		liberate( message );
		return( body );
	}
	else
	{
		fprintf(h,"%s\r\n",message);
		liberate( message );
		fclose(h);
		return( filename );
	}
}

/*	------------------------------------------------------------	*/
/*		     o c c i _ x m l _ b o d y 				*/
/*	------------------------------------------------------------	*/
private	char * 	occi_xml_body( 
		struct occi_client * cptr, 
		struct occi_request * rptr,
		struct rest_header  * hptr,
		char * body )
{
	return( body );
}

/*	------------------------------------------------------------	*/
/*		     o c c i _ c l i e n t _ b o d y 			*/
/*	------------------------------------------------------------	*/
private	char *	occi_client_body( 
		struct occi_client * cptr, 
		struct occi_request * rptr,
		struct rest_header  * hptr,
		char * body )
{
	struct	rest_header * wptr;
	if (!( hptr ))
		return( body );
	else if (!( wptr = rest_resolve_header( hptr, _HTTP_CONTENT_TYPE ) ))
		return( body );
	else if (!( wptr->value ))
		return( body );
	else if (!( strcasecmp( wptr->value, _OCCI_TEXT_PLAIN ) ))
		return( body );
	else if (!( strcasecmp( wptr->value, _OCCI_MIME_OCCI ) ))
		return( body );
	else if (!( strcasecmp( wptr->value, _OCCI_MIME_JSON ) ))
		return( occi_json_body( cptr, rptr, hptr, body ) );
	else if (!( strcasecmp( wptr->value, _OCCI_MIME_XML  ) ))
		return( occi_xml_body( cptr, rptr, hptr, body ) );
	else	return( body );
}

/*	------------------------------------------------------------	*/
/*		     o c c i _ c l i e n t _ g e t 			*/
/*	------------------------------------------------------------	*/
public	struct	occi_response *	occi_client_get( struct occi_client * cptr, struct occi_request * rptr )
{
	struct	occi_response * aptr=(struct occi_response *) 0;
	struct	rest_header * hptr=(struct rest_header *) 0;
	struct	rest_response * zptr;
	char *	uri;
	if (!( uri = occi_client_uri( cptr, (char *) 0 ) ))
		return((struct occi_response *) 0);
	else if (( rptr->first ) 
	     &&  (!( hptr = occi_request_headers( cptr, rptr ) )))
		return((struct occi_response *) 0);
	else if ((OcciManager.headers)
	     &&  (!( hptr = occi_append_default( cptr, hptr, OcciManager.headers ) )))
		return((struct occi_response *) 0);
	else if (( rptr->account )
	     && (!( hptr = occi_append_account( cptr, hptr, rptr->account ) )))
		return((struct occi_response *) 0);
	else if (!( zptr = rest_client_get_request(uri,cptr->tls,cptr->agent, hptr) ))
		return((struct occi_response *) 0);
	else if (!( aptr = occi_create_response( rptr, zptr ) ))
		return( aptr );
	else if (!( aptr->host = serialise_url_host( cptr->target )))
		return( aptr );
	else	return( aptr );
}

/*	------------------------------------------------------------	*/
/*		     o c c i _ s i m p l e _ p u t 			*/
/*	------------------------------------------------------------	*/
public	struct	occi_response *	occi_simple_put( 
	char * reference, struct occi_element * eptr, char * agent, char * tls )
{	
	struct	occi_element 	* fptr;
	struct	occi_client  	* cptr;
	struct 	occi_request  	* qptr;
	struct 	occi_response 	* zptr=(struct occi_response *) 0;

	if (!( cptr = occi_create_client( reference, agent, tls ) ))
	 	return((struct occi_response *) 0);
	else if (!( qptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
	 	return((struct occi_response *) 0);

	for (	;
		eptr != (struct occi_element *) 0;
		eptr = eptr->next )
		if (!( fptr = occi_request_element( qptr, eptr->name, eptr->value ) ))
			break;

	if (!( zptr = occi_client_put ( cptr, qptr ) ))
	{
		qptr = occi_remove_request( qptr );
		return( zptr );
	}
	else
	{
		qptr = occi_remove_request( qptr );
		return( zptr );
	}
}

/*	------------------------------------------------------------	*/
/*		     o c c i _ s i m p l e _ g e t 			*/
/*	------------------------------------------------------------	*/
public	struct	occi_response *	occi_simple_get( char * reference, char * agent, char * tls )
{	
	struct	occi_client  	* cptr;
	struct 	occi_request  	* qptr;
	struct 	occi_response 	* zptr=(struct occi_response *) 0;

	if (!( cptr = occi_create_client( reference, agent, tls ) ))
	 	return((struct occi_response *) 0);
	else if (!( qptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
	 	return((struct occi_response *) 0);
	else if (!( zptr = occi_client_get ( cptr, qptr ) ))
	{
		qptr = occi_remove_request( qptr );
		return( zptr );
	}
	else
	{
		qptr = occi_remove_request( qptr );
		return( zptr );
	}
}

/*	------------------------------------------------------------	*/
/*		     o c c i _ c l i e n t _ p u t 			*/
/*	------------------------------------------------------------	*/
public	struct	occi_response *	occi_client_put( struct occi_client * cptr, struct occi_request * rptr )
{
	struct	occi_response * aptr=(struct occi_response *) 0;
	struct	rest_header * hptr=(struct rest_header *) 0;
	struct	rest_response * zptr;
	char *	body = "";
	char *	uri;
	if (!( uri = occi_client_uri( cptr, (char *) 0 ) ))
		return((struct occi_response *) 0);
	else if (( rptr->first ) 
	     &&  (!( hptr = occi_request_headers( cptr, rptr ))))
		return((struct occi_response *) 0);
	else if ((OcciManager.headers)
	     &&  (!( hptr = occi_append_default( cptr, hptr, OcciManager.headers ) )))
		return((struct occi_response *) 0);
	else if (( rptr->account )
	     && (!( hptr = occi_append_account( cptr, hptr, rptr->account ) )))
		return((struct occi_response *) 0);
	else if (!( body = occi_client_body( cptr, rptr, hptr, body ) ))
		return((struct occi_response *) 0);
	else if (!( zptr = rest_client_put_request(uri,cptr->tls,cptr->agent,body,hptr) ))
		return((struct occi_response *) 0);
	else if (!( aptr = occi_create_response( rptr, zptr ) ))
		return( aptr );
	else if (!( aptr->host = serialise_url_host( cptr->target )))
		return( aptr );
	else	return( aptr );
}

/*	------------------------------------------------------------	*/
/*		 	o c c i _ c l i e n t _ d e l e t e 		*/
/*	------------------------------------------------------------	*/
public	struct	occi_response *	occi_client_delete( struct occi_client * cptr, struct occi_request * rptr )
{
	struct	occi_response * aptr=(struct occi_response *) 0;
	struct	rest_header * hptr=(struct rest_header *) 0;
	struct	rest_response * zptr;
	char *	uri;
	if (!( uri = occi_client_uri( cptr, (char *) 0 ) ))
		return((struct occi_response *) 0);
	else if (( rptr->first ) 
	     &&  (!( hptr = occi_request_headers( cptr, rptr ))))
		return((struct occi_response *) 0);
	else if ((OcciManager.headers)
	     &&  (!( hptr = occi_append_default( cptr, hptr, OcciManager.headers ) )))
		return((struct occi_response *) 0);
	else if (( rptr->account )
	     && (!( hptr = occi_append_account( cptr, hptr, rptr->account ) )))
		return((struct occi_response *) 0);
	else if (!( zptr = rest_client_delete_request(uri,cptr->tls,cptr->agent,hptr) ))
		return((struct occi_response *) 0);
	else if (!( aptr = occi_create_response( rptr, zptr ) ))
		return( aptr );
	else if (!( aptr->host = serialise_url_host( cptr->target )))
		return( aptr );
	else	return( aptr );
}

/*	------------------------------------------------------------	*/
/*		     o c c i _ s i m p l e _ d e l e t e 		*/
/*	------------------------------------------------------------	*/
public	struct	occi_response *	occi_simple_delete( char * reference, char * agent, char * tls )
{	
	struct	occi_client  	* cptr;
	struct 	occi_request  	* qptr;
	struct 	occi_response 	* zptr=(struct occi_response *) 0;

	if (!( cptr = occi_create_client( reference, agent, tls ) ))
	 	return((struct occi_response *) 0);
	else if (!( qptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
	 	return((struct occi_response *) 0);
	else if (!( zptr = occi_client_delete( cptr, qptr ) ))
	{
		qptr = occi_remove_request( qptr );
		return( zptr );
	}
	else
	{
		qptr = occi_remove_request( qptr );
		return( zptr );
	}
}

/*	------------------------------------------------------------	*/
/*		 o c c i _ c l i e n t _ p o s t 			*/
/*	------------------------------------------------------------	*/
public	struct	occi_response *	occi_client_post( struct occi_client * cptr, struct occi_request * rptr )
{
	struct	occi_response * aptr=(struct occi_response *) 0;
	struct	rest_header * hptr=(struct rest_header *) 0;
	struct	rest_response * zptr;
	char *	body="";
	char *	uri;
	if (!( uri = occi_client_uri( cptr, (char *) 0 ) ))
		return((struct occi_response *) 0);
	else if (( rptr->first ) 
	     &&  (!( hptr = occi_request_headers( cptr, rptr ))))
		return((struct occi_response *) 0);
	else if ((OcciManager.headers)
	     &&  (!( hptr = occi_append_default( cptr, hptr, OcciManager.headers ) )))
		return((struct occi_response *) 0);
	else if (( rptr->account )
	     && (!( hptr = occi_append_account( cptr, hptr, rptr->account ) )))
		return((struct occi_response *) 0);
	else if (!( body = occi_client_body( cptr, rptr, hptr, body ) ))
		return((struct occi_response *) 0);
	else if (!( zptr = rest_client_post_request(uri,cptr->tls,cptr->agent,body,hptr) ))
		return((struct occi_response *) 0);
	else if (!( aptr = occi_create_response( rptr, zptr ) ))
		return( aptr );
	else if (!( aptr->host = serialise_url_host( cptr->target )))
		return( aptr );
	else	return( aptr );
}

/*	------------------------------------------------------------	*/
/*		     o c c i _ c l i e n t _ h e a d 			*/
/*	------------------------------------------------------------	*/
public	struct	occi_response *	occi_client_head( struct occi_client * cptr, struct occi_request * rptr )
{
	struct	occi_response * aptr=(struct occi_response *) 0;
	struct	rest_header * hptr=(struct rest_header *) 0;
	struct	rest_response * zptr;
	char *	uri;
	if (!( uri = occi_client_uri( cptr, (char *) 0 ) ))
		return((struct occi_response *) 0);
	else if (( rptr->first ) 
	     &&  (!( hptr = occi_request_headers( cptr, rptr ))))
		return((struct occi_response *) 0);
	else if ((OcciManager.headers)
	     &&  (!( hptr = occi_append_default( cptr, hptr, OcciManager.headers ) )))
		return((struct occi_response *) 0);
	else if (( rptr->account )
	     && (!( hptr = occi_append_account( cptr, hptr, rptr->account ) )))
		return((struct occi_response *) 0);
	else if (!( zptr = rest_client_head_request(uri,cptr->tls,cptr->agent,hptr) ))
		return((struct occi_response *) 0);
	else if (!( aptr = occi_create_response( rptr, zptr ) ))
		return( aptr );
	else if (!( aptr->host = serialise_url_host( cptr->target )))
		return( aptr );
	else	return( aptr );
}


/*	------------------------------------------------------------	*/
/*		   o c c i _ s h o w _ r e s p o n s e 			*/
/*	------------------------------------------------------------	*/
public	void	occi_show_response( struct occi_response * rptr )
{
	struct	occi_element * eptr;

	if ( rptr->response )
	{
		printf("   OCCI Client : %s %u %s \n",
			rptr->response->version,
			rptr->response->status,
			rptr->response->message );
	}
	for (	eptr = rptr->first;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;
		else	printf("   %s = %s \n",eptr->name,eptr->value );
	}
	return;
}

/*	------------------------------------------------------------	*/
/*		   o c c i _ l o c a t e _ e l e m e n t		*/
/*	------------------------------------------------------------	*/
public	struct	occi_element * occi_locate_element( struct occi_element * eptr, char * sptr )
{
	for (	;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( strcasecmp( eptr->name, sptr ) ))
			break;
	}
	return( eptr );
}

/*	------------------------------------------------------------	*/
/*			o c c i _ p o s t _ e v e n t 			*/
/*	------------------------------------------------------------	*/
/*	Attempts to post the contents of the buffer as an occi event	*/
/*	------------------------------------------------------------	*/
public	int	occi_post_event( char * buffer, char * nature, char * agent )
{
	return( cords_post_event( buffer, nature, agent, default_tls() ) );
}



#endif 	/* _occi_client_c */


