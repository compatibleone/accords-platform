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
#include "occibody.h"
#include "json.h"
#include "document.h"
#include "occicat.h"
#include "restlog.h"
#include "stdnode.h"

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
		_OCCI_MIME_JSON,
		0,
		1	/* optimised */
	};

#include "occiauth.c"

/*	------------------------------------------------------------	*/
/*		   o c c i _ a p p e n d _ h e a d e r			*/
/*	------------------------------------------------------------	*/
private	int	py_ocni_bugfix=1;

private	struct	rest_header * occi_append_header( struct rest_header * hptr, char * nptr, char * vptr )
{
	struct	rest_header * wptr;
	struct	rest_header * xptr;

	if (!( wptr = rest_create_header( nptr, vptr ) ))
		return( hptr );
	else if (!( xptr = hptr ))
		return( wptr );
	else
	{
		while ( xptr->next )
			xptr = xptr->next;
		xptr->next = wptr;
		wptr->previous = xptr;
		return( hptr );
	}
}

/*	------------------------------------------------------------	*/
/*	     o c c i _ s p e c i a l _ a u t h o r i s a t i o n	*/
/*	------------------------------------------------------------	*/
/*	the pyocni module on which coes is built needs a special id	*/
/*	or it wont work, this is a work around till we deal with it	*/
/*	later on ....							*/
/*	------------------------------------------------------------	*/
private	struct	rest_header * occi_special_authorisation( struct rest_header * hptr )
{
	if (!( py_ocni_bugfix ))
		return( hptr );
	else if (!( hptr = occi_append_header( hptr, _HTTP_ACCEPT, "text/occi" ) ))
		return( hptr );
	else if (!( hptr = occi_append_header( hptr, _HTTP_CONTENT_TYPE, "text/occi" ) ))
		return( hptr );
	else	return( hptr );
}

/*	---	*/
/*	GET	*/
/*	---	*/
public	struct	rest_response * 
	occi_client_get_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	hptr = occi_special_authorisation( hptr );
	return( rest_client_get_request( target, tls, nptr, hptr ) );
}


/*	------	*/
/*	DELETE	*/
/*	------	*/
public	struct	rest_response * 
	occi_client_delete_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	hptr = occi_special_authorisation( hptr );
	return( rest_client_delete_request( target, tls, nptr, hptr ) );
}


/*	----	*/
/*	HEAD	*/
/*	----	*/
public	struct	rest_response * 
	occi_client_head_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	hptr = occi_special_authorisation( hptr );
	return( rest_client_head_request( target, tls, nptr, hptr ) );
}


/*	----	*/
/*	POST	*/
/*	----	*/
public	struct	rest_response * 
	occi_client_post_request(
		char * target, char * tls, char * nptr, char * filename, struct rest_header * hptr )
{
	hptr = occi_special_authorisation( hptr );
	return( rest_client_post_request( target, tls, nptr, filename, hptr ) );
}


/*	---	*/
/*	PUT	*/
/*	---	*/
public	struct	rest_response * 
	occi_client_put_request(
		char * target, char * tls, char * nptr, char * filename, struct rest_header * hptr )
{
	hptr = occi_special_authorisation( hptr );
	return( rest_client_put_request( target, tls, nptr, filename, hptr ) );
}

#define	rest_client_get_request occi_client_get_request
#define	rest_client_head_request occi_client_head_request
#define	rest_client_put_request occi_client_put_request
#define	rest_client_post_request occi_client_post_request
#define	rest_client_delete_request occi_client_delete_request

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
public 	struct	rest_header * occi_add_default_header(struct rest_header  * hptr)
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
/*		   o c c i _ a d d _ d e f a u l t _ h e a d e r	*/
/*	------------------------------------------------------------	*/
public	void	occi_drop_default_headers()
{
	struct	rest_header * rptr;

	while (( rptr = OcciManager.headers ) != (struct rest_header *) 0)
	{
		OcciManager.headers = rptr->next;
		rptr->next = rptr->previous = (struct rest_header *) 0;
		liberate_rest_header( rptr );
	}
	return;
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
			hptr->value = liberate( hptr->value );
		hptr->value = allocate_string( aptr );
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
			hptr->value = liberate( hptr->value );
		hptr->value = allocate_string( aptr );
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
			hptr->value = liberate( hptr->value );
		hptr->value = allocate_string( aptr );
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
		else if (!( cptr->location ))
			continue;
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

/*	-------------------------------------------------------		*/
/*		o c c i _ e x t r a c t _ a t r i b u t			*/
/*	-------------------------------------------------------		*/
public	char *	occi_extract_atribut( 
		struct occi_response * zptr, 
		char * domain,
		char * category,
		char * nptr )
{
	char	buffer[2048];
	sprintf(buffer,"%s.%s.%s",domain,category,nptr);
	struct	occi_element * eptr;
	for (	eptr = zptr->first;
		eptr != (struct occi_element *) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if ( strcmp( eptr->name, buffer ) )
			continue;
		else if (!( eptr->value ))
			break;
		else if (!( strcmp( eptr->value, _CORDS_NULL ) ))
			break;
		else	return( occi_unquoted_value( eptr->value ) );
	}
	return((char*) 0);
}

/*	---------------------------------------------------	*/
/*	     o c c i _ e x t r a c t _ l o c a t i o n		*/
/*	---------------------------------------------------	*/
/*	this function will scan the element list of an occi	*/
/*	response and will extract the first "location" item	*/
/*	encountered. If no element list is available then	*/
/*	the header list of the eventual rest response will	*/
/*	be scanned for X-OCCI-Location fields. The first	*/
/*	one encountered will be returned.			*/
/*	---------------------------------------------------	*/
public  char *	occi_extract_location( struct occi_response * rptr )
{
	struct	rest_response * aptr;
	struct	occi_element  * eptr;
	struct	rest_header   * hptr;
	if (( aptr = rptr->response ) != (struct rest_response *) 0)
	{
		for (	eptr = rptr->first;
			eptr != (struct occi_element *) 0;
			eptr = eptr->next )
			if (!( strcasecmp( eptr->name, _HTTP_LOCATION )))
				return( eptr->value );
		return((char *) 0);
	}
	else if (!( hptr = rest_resolve_header( aptr->first, _OCCI_LOCATION ) ))
		return((char *) 0);
	else	return( hptr->value );
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
/*	  o c c i _ c r e a t e _ d e f a u l t _ r e s p o n s e	*/
/*	------------------------------------------------------------	*/
public	struct	occi_response *	occi_create_default_response( 
	struct occi_response * aptr,
	struct occi_request * rptr, 
	struct rest_response * zptr )
{
	struct	rest_header * hptr;
	struct	occi_element * eptr;
	char	*	nptr;
	char	*	vptr;
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
				eptr=occi_response_element(aptr,nptr,vptr);
				nptr = liberate( nptr );
				if (!( eptr ))
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

/*	------------------------------------------------------------	*/
/*	     o c c i _ c r e a t e _ t e x t _ r e s p o n s e		*/
/*	------------------------------------------------------------	*/
public	struct	occi_response *	occi_create_text_response( 
	struct occi_response * aptr,
	struct occi_request * rptr, 
	struct rest_response * zptr )
{
	struct	occi_element * eptr;
	char	*	nptr;
	char	*	vptr;

	return( occi_create_default_response( aptr, rptr, zptr ) );
}

/*	------------------------------------------------------------	*/
/*	     o c c i _ c r e a t e _ p h p _ r e s p o n s e		*/
/*	------------------------------------------------------------	*/
public	struct	occi_response *	occi_create_php_response( 
	struct occi_response * aptr,
	struct occi_request * rptr, 
	struct rest_response * zptr )
{
	struct	occi_element * eptr;
	char	*	nptr;
	char	*	vptr;

	return( aptr );
}

/*	------------------------------------------------------------	*/
/*	     o c c i _ c r e a t e _ x m l _ r e s p o n s e		*/
/*	------------------------------------------------------------	*/
public	struct	occi_response *	occi_create_xml_response( 
	struct occi_response * aptr,
	struct occi_request * rptr, 
	struct rest_response * zptr )
{
	struct	occi_element * eptr;
	char	*	nptr;
	char	*	vptr;

	return( aptr );
}
/*	------------------------------------------------------------	*/
/*	     o c c i _ c r e a t e _ j s o n _ r e s p o n s e		*/
/*	------------------------------------------------------------	*/
/* 	this function expects the JSON message to be formated using	*/
/*	the new OCCI JSON format in a standard JSON object, as the 	*/
/*	name of the format implies, and not just as a dummy container 	*/
/*	for an OCCI message as the old format represented.		*/
/*									*/
/* 	{								*/
/* 		domain : {						*/
/* 			category : {					*/
/* 				attribute : value [,] 			*/
/* 				}					*/
/* 			}						*/
/* 	}								*/
/*									*/
/*	the equivalent of the occi header fields:			*/
/*									*/
/*		occi.core.id=ed3345-edf667-ab8aee31-bcd33		*/
/*		occi.compute.memory=5;					*/
/*		occi.compute.speed=2;					*/
/*		occi.compute.cores=1;					*/
/*									*/
/*	would be:							*/
/*									*/
/*	{ 								*/
/*	"occi" : { 							*/
/*		"core" : { "id" : "ed3345-edf667-ab8aee31-bcd33" } ,	*/
/*		"compute" : {						*/
/*			"memory" : 5, 					*/
/*			"speed"  : 2,					*/
/*			"cores"  : 1 					*/
/*			}						*/
/*		}							*/
/*	}								*/
/*									*/
/*	------------------------------------------------------------	*/
public	struct	occi_response *	occi_create_json_response( 
	struct occi_response * aptr,
	struct occi_request * rptr, 
	struct rest_response * zptr )
{
	struct	occi_element * eptr;
	char	*	nptr;
	char	*	vptr;
	char	*	domain;
	char 	*	category;
	struct	data_element * dptr;
	struct	data_element * root;
	struct	data_element * cptr;
	struct	data_element * bptr;
	char	buffer[2048];

	if (!( dptr = json_parse_file( zptr->body ) ))
		return( aptr );
	else	root = dptr;
	/* ------------------------------------------ */
	/* step over the first one : its the filename */
	/* ------------------------------------------ */
	if (!( dptr = dptr->first ))
	{
		drop_data_element( root );
		return( aptr );
	}
	else if (!( domain = dptr->name ))
	{
		drop_data_element( root );
		return( aptr );
	}
	else if (!( dptr = dptr->first ))
	{
		drop_data_element( root );
		return( aptr );
	}
	else if (!( category = dptr->name ))
	{
		drop_data_element( root );
		return( aptr );
	}
	else
	{
		for (	cptr = dptr->first;
			cptr != (struct data_element *) 0;
			cptr = cptr->next )
		{
			if (!( cptr->name ))
				continue;
			else if (!( nptr = occi_unquoted_value( cptr->name ) ))
				continue;
			else if (!( strcasecmp( nptr, _HTTP_LOCATION ) ))
			{
				for (	bptr = cptr->first;
					bptr != (struct data_element *) 0;
					bptr = bptr->next )
				{
					if (!( vptr = occi_unquoted_value( bptr->value ) ))
						break;
					else if (!(eptr=occi_response_element(aptr,rptr->name,vptr)))
						break;
					else	liberate( vptr );
				}
				liberate( nptr );
				break;					
			}
			else if (!( vptr = occi_unquoted_value( cptr->value ) ))
				continue;
			else
			{
				sprintf(buffer,"%s.%s.%s",domain,category,nptr);
				if (!(eptr=occi_response_element(aptr,buffer,vptr)))
					break;
				liberate( nptr );
				liberate( vptr );
				continue;
			}
		}
		drop_data_element( root );
		return( aptr );
	}
}

/*	------------------------------------------------------------	*/
/*	 o c c i _ c r e a t e _ o l d _ j s o n _ r e s p o n s e 	*/
/*	------------------------------------------------------------	*/
/*	this function consume the old, non standard, JSON rendering 	*/
/*	produced by the pyOCNI library					*/ 
/*	{								*/
/*		"occi.core.id" : "value",				*/
/*		"occi.core.title" : "value",				*/
/*		"occi.core.summary" : "value",				*/
/*		"attributes" : 	{					*/
/*			"occi.compute.memory" : 5,			*/
/*			"occi.compute.speed" : 2,			*/
/*			"occi.compute.cores" : 1			*/
/*			}						*/
/*	}								*/
/*	------------------------------------------------------------	*/
public	struct	occi_response *	occi_create_old_json_response( 
	struct occi_response * aptr,
	struct occi_request * rptr, 
	struct rest_response * zptr )
{
	struct	occi_element * eptr;
	char	*	nptr;
	char	*	vptr;
	char	*	domain;
	char 	*	category;
	struct	data_element * dptr;
	struct	data_element * root;
	struct	data_element * cptr;
	struct	data_element * bptr;
	char	buffer[2048];

	if (!( dptr = json_parse_file( zptr->body ) ))
		return( aptr );
	else	root = dptr;

	for (	cptr = dptr->first;
		cptr != (struct data_element *) 0;
		cptr = cptr->next )
	{
		if (!( cptr->name ))
			continue;
		else if (!( nptr = occi_unquoted_value( cptr->name ) ))
			continue;
		else if (!( strcasecmp( nptr, _HTTP_LOCATION ) ))
		{
			for (	bptr = cptr->first;
				bptr != (struct data_element *) 0;
				bptr = bptr->next )
			{
				if (!( vptr = occi_unquoted_value( bptr->value ) ))
					break;
				else if (!(eptr=occi_response_element(aptr,rptr->name,vptr)))
					break;
				else	liberate( vptr );
			}
			liberate( nptr );
			break;					
		}
		else if (!( strncmp( nptr, "occi.core", strlen("occi.core") ) ))
		{
			if (!( vptr = occi_unquoted_value( cptr->value )))
				continue;
			else if (!(eptr=occi_response_element(aptr,nptr,vptr)))
				break;
			else
			{
				liberate( nptr );
				liberate( vptr );
				continue;
			}
		}
		else if (!( strcmp( nptr, "attributes" ) ))
		{
			liberate( nptr );
			for (	bptr = cptr->first;
				bptr != (struct data_element *) 0;
				bptr = bptr->next )
			{
				if (!( bptr->name ))
					continue;
				else if (!( nptr = occi_unquoted_value( bptr->name )))
					continue;
				else if (!( vptr = occi_unquoted_value( bptr->value )))
					continue;
				else if (!(eptr=occi_response_element(aptr,nptr,vptr)))
					break;
				else
				{
					liberate( nptr );
					liberate( vptr );
					continue;
				}
			}
		}
	}
	drop_data_element( root );
	return( aptr );
}

/*	------------------------------------------------------------	*/
/*		 o c c i _ c r e a t e _ r e s p o n s e		*/
/*	------------------------------------------------------------	*/
public	struct	occi_response *	occi_create_response( 
	struct occi_request * rptr, 
	struct rest_response * zptr )
{
	struct	rest_header * hptr;
	struct	occi_response * aptr;

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
		if (!( hptr = rest_resolve_header( zptr->first, _HTTP_CONTENT_LENGTH ) ))
			return( occi_create_default_response( aptr, rptr,  zptr ) );
		else if (!( hptr = rest_resolve_header( zptr->first, _HTTP_CONTENT_TYPE ) ))
			return( occi_create_default_response( aptr, rptr,  zptr ) );
		else if (!( strcasecmp( hptr->value, _OCCI_TEXT_OCCI ) ))
			return( occi_create_text_response( aptr, rptr,  zptr ) );
		else if ((!( strcasecmp( hptr->value, _OCCI_OCCI_PHP ) ))
		     ||  (!( strcasecmp( hptr->value, _OCCI_APP_PHP  ) ))
		     ||  (!( strcasecmp( hptr->value, _OCCI_TEXT_PHP ) )))
			return( occi_create_php_response( aptr, rptr,  zptr ) );
		else if ((!( strcasecmp( hptr->value, _OCCI_OCCI_JSON ) ))
		     ||  (!( strcasecmp( hptr->value, _OCCI_APP_JSON  ) ))
		     ||  (!( strcasecmp( hptr->value, _OCCI_TEXT_JSON ) )))
			return( occi_create_json_response( aptr, rptr,  zptr ) );
		else if (!( strcasecmp( hptr->value, _OCCI_OLD_JSON ) ))
			return( occi_create_old_json_response( aptr, rptr,  zptr ) );
		else if ((!( strcasecmp( hptr->value, _OCCI_MIME_XML  ) ))
		     ||  (!( strcasecmp( hptr->value, _OCCI_APP_XML   ) ))
		     ||  (!( strcasecmp( hptr->value, _OCCI_TEXT_XML  ) )))
			return( occi_create_xml_response( aptr, rptr,  zptr ) );
		else	return( occi_create_default_response( aptr, rptr,  zptr ) );
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
	struct	occi_category * xptr;
	if (!( cptr ))
		return((struct occi_category *) 0);
	else if (!( sptr ))
		return((struct occi_category *) 0);
	else if (!( optr = occi_parse_category( sptr ) ))
		return( optr );
	else
	{
		xptr = optr;
		cptr->categories++;
		while ( xptr->next )
		{
			cptr->categories++;
			xptr = xptr->next;
		}
		if (!( optr->previous = cptr->lastcat ))
			cptr->firstcat = optr;
		else	optr->previous->next = optr;
		return((cptr->lastcat = xptr));
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

/*	-----------------------------------------------------------	*/
/*	  o c c i _ a n a l y s e _ h t t p _ c a t e g o r i e s	*/
/*	-----------------------------------------------------------	*/
private	struct	occi_client * occi_analyse_http_categories( struct occi_client * cptr, struct rest_response * rptr ){
	struct	rest_header * hptr=(struct rest_header *) 0;

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
	return( cptr );
}

/*	-----------------------------------------------------------	*/
/*	  o c c i _ a n a l y s e _ t e x t _ c a t e g o r i e s	*/
/*	-----------------------------------------------------------	*/
private	struct	occi_client * occi_analyse_text_categories( struct occi_client * cptr, struct rest_response * rptr )
{
	char	buffer[8192];
	struct	rest_header * hptr=(struct rest_header *) 0;
	char *	nptr;
	FILE *	h;
	if (!( hptr = rest_resolve_header( rptr->first, _HTTP_CONTENT_LENGTH ) ))
		return( occi_analyse_http_categories( cptr, rptr ) );
	else if (!( rptr->body ))
		return( occi_analyse_http_categories( cptr, rptr ) );
	else if (!( h= fopen( rptr->body, "r" ) ))
		return( occi_analyse_http_categories( cptr, rptr ) );
	else
	{
		while ((nptr = fgets(buffer,8000,h)) != (char *) 0)
		{
			while ( *nptr == ' ' ) nptr++;
			if (!( strncasecmp( nptr, _OCCI_CATEGORY, strlen( _OCCI_CATEGORY ) ) ))
			{
				nptr += strlen( _OCCI_CATEGORY );
				while ( *nptr == ' ' ) nptr++;
				if ( *nptr != ':' )				
					continue;
				else 	nptr++;
				while ( *nptr == ' ' ) nptr++;
				if (!( occi_client_add_category( cptr, nptr ) ))
				{
					cptr = occi_delete_client( cptr );
					break;
				}
			}
		}
		fclose(h);
		return( cptr );
	}
}

/*	-----------------------------------------------------------	*/
/*	   o c c i _ a n a l y s e _ x m l _ c a t e g o r i e s	*/
/*	-----------------------------------------------------------	*/
private	struct	occi_client * occi_analyse_xml_categories( struct occi_client * cptr, struct rest_response * rptr )
{
	char	buffer[8192];
	struct	rest_header * hptr=(struct rest_header *) 0;
	struct	xml_element * document;
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;
	if (!( hptr = rest_resolve_header( rptr->first, _HTTP_CONTENT_LENGTH ) ))
		return( occi_analyse_http_categories( cptr, rptr ) );
	else if (!( rptr->body ))
		return( occi_analyse_http_categories( cptr, rptr ) );
	else if (!( document = document_parse_file( rptr->body ) ))
		return( occi_analyse_http_categories( cptr, rptr ) );
	else
	{

		document = document_drop( document );
		return( cptr );
	}
}

/*	-----------------------------------------------------------	*/
/*	  o c c i _ a n a l y s e _ j s o n _ c a t e g o r i e s	*/
/*	-----------------------------------------------------------	*/
private	struct	occi_client * occi_analyse_json_categories( struct occi_client * cptr, struct rest_response * rptr )
{
	char	buffer[8192];
	struct	occi_category *	optr;
	struct	occi_attribute *aptr;
	struct	occi_action * mptr;
	struct	rest_header * hptr=(struct rest_header *) 0;
	struct	data_element * qptr;
	struct	data_element * bptr;
	struct	data_element * dptr;
	struct	data_element * eptr;
	struct	data_element * sptr;
	char *	vptr;
	if (!( hptr = rest_resolve_header( rptr->first, _HTTP_CONTENT_LENGTH ) ))
		return( occi_analyse_http_categories( cptr, rptr ) );
	else if (!( rptr->body ))
		return( occi_analyse_http_categories( cptr, rptr ) );
	else if (!( dptr = json_parse_file( rptr->body  ) ))
		return( occi_analyse_http_categories( cptr, rptr ) );
	else if (!( eptr = json_element( dptr, "categories") ))
		return( occi_analyse_http_categories( cptr, rptr ) );
	else
	{
		/* ----------------- */
		/* for each category */
		/* ----------------- */
		for (	sptr=eptr->first;
			sptr !=( struct data_element *) 0;
			sptr = sptr->next )
		{
			/* ------------------------- */
			/* add a new client category */
			/* ------------------------- */
			if (!( optr = allocate_occi_category() ))
				break;
			else if (!( optr->previous = cptr->lastcat ))
				cptr->firstcat = optr;
			else	optr->previous->next = optr;

			cptr->lastcat = optr;

			/* -------------------------------------- */
			/* collect and store the category details */
			/* -------------------------------------- */
			if (!( vptr = json_atribut( sptr->first, "term" ) ))
				break;
			else if (!( optr->id = occi_unquoted_value( vptr ) ))
				break;

			if (!( vptr = json_atribut( sptr->first, "scheme" )))
				break;
			else if (!( optr->scheme=occi_unquoted_value( vptr )))
				break;

			if (!( vptr = json_atribut( sptr->first, "class" )))
				break;
			else if (!( optr->class=occi_unquoted_value( vptr ) ))
				break;

			if (!( vptr = json_atribut( sptr->first, "rel" )))
				break;
			else if (!( optr->rel=occi_unquoted_value( vptr ) ))
				break;

			if (!( vptr = json_atribut( sptr->first, "title" )))
				break;
			else if (!( optr->title=occi_unquoted_value( vptr ) ))
				break;

			if (!( vptr = json_atribut( sptr->first, "location" )))
				break;
			else if (!( optr->location=occi_unquoted_value( vptr ) ))
				break;

			/* ----------------------------------------- */
			/* collect and store the category attributes */
			/* ----------------------------------------- */
			if (!( qptr = json_element( sptr->first, "attributes" )))
				break;
			else
			{
				for (	bptr=qptr->first;
					bptr !=( struct data_element *) 0;
					bptr = bptr->next )
				{

					if (!( vptr = json_atribut( bptr->first, "name" ) ))
						break;
					else if (!( aptr = add_occi_attribute( optr ) ))
						break;
					else
					{
						sprintf(buffer,"%s.%s.%s","occi",optr->id,vptr);
						 if (!( aptr->name = occi_unquoted_value( buffer ) ))
							break;
					}
				}
			}

			/* -------------------------------------- */
			/* collect and store the category actions */
			/* -------------------------------------- */
			if (!( qptr = json_element( sptr->first, "actions" )))
				break;
			else
			{
				for (	bptr=qptr->first;
					bptr !=( struct data_element *) 0;
					bptr = bptr->next )
				{

					if (!( vptr = json_atribut( bptr->first, "name" ) ))
						break;
					else if (!( mptr = add_occi_action( optr ) ))
						break;
					else if (!( mptr->name = occi_unquoted_value( vptr ) ))
						break;
				}
			}
		}
		dptr = drop_data_element( dptr );
		return( cptr );
	}
}

/*	-----------------------------------------------------------	*/
/*		o c c i _ a n a l y s e _ c a t e g o r i e s		*/
/*	-----------------------------------------------------------	*/
private	struct	occi_client * occi_analyse_categories( struct occi_client * cptr, struct rest_response * rptr )
{
	struct	rest_header * hptr=(struct rest_header *) 0;
	if (!( hptr = rest_resolve_header( rptr->first, _HTTP_CONTENT_TYPE ) ))
		return( occi_analyse_http_categories( cptr, rptr ) );
	else if (!( hptr->value ))
		return( occi_analyse_http_categories( cptr, rptr ) );
	else if (!( strcasecmp( hptr->value, _OCCI_TEXT_OCCI ) ))
		return( occi_analyse_http_categories( cptr, rptr ) );
	else if (!( strcasecmp( hptr->value, _OCCI_TEXT_PLAIN ) ))
		return( occi_analyse_text_categories( cptr, rptr ) );
	else if ((!( strcasecmp( hptr->value, _OCCI_OCCI_JSON ) ))
	     ||  (!( strcasecmp( hptr->value, _OCCI_OLD_JSON  ) ))
	     ||  (!( strcasecmp( hptr->value, _OCCI_APP_JSON  ) ))
	     ||  (!( strcasecmp( hptr->value, _OCCI_TEXT_JSON ) )))
		return( occi_analyse_json_categories( cptr, rptr ) );
	else if ((!( strcasecmp( hptr->value, _OCCI_MIME_XML  ) ))
	     ||  (!( strcasecmp( hptr->value, _OCCI_APP_XML   ) ))
	     ||  (!( strcasecmp( hptr->value, _OCCI_TEXT_XML  ) )))
		return( occi_analyse_xml_categories( cptr, rptr ) );
	else	return( occi_analyse_http_categories( cptr, rptr ) );
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

	cptr = occi_analyse_categories( cptr, rptr );

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
/*			o c c i _ r e l e a s e _  c l i e n t		*/
/*	------------------------------------------------------------	*/
/*	this function is required when an OCCI component such as a	*/
/*	COSACS server is private and will no longer need to be used	*/
/*	the client must be released in order to avoid problems due	*/
/*	to incorrect use of the OCCI Client structure. 			*/
/*	Many thanks to Thanks Jean Michel Leonard for finding this	*/
/*	------------------------------------------------------------	*/
public	int	occi_release_client( char * host )
{
	struct	occi_client *	cptr;
	if (( cptr = occi_resolve_client( host )) != (struct occi_client *) 0)
		occi_delete_client( cptr );
	return(0);
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
	else if (!( strcasecmp( wptr->value, _OCCI_TEXT_OCCI ) ))
		return( body );
	else if (!( strcasecmp( wptr->value, _OCCI_MIME_OCCI  ) ))
		return( body );
	else	return( occi_response_body( wptr->value, rptr->category , hptr ) );
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
/*		     o c c i _ s i m p l e _ p o s t 			*/
/*	------------------------------------------------------------	*/
public	struct	occi_response *	occi_simple_post( 
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

	if (!( zptr = occi_client_post ( cptr, qptr ) ))
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


/*	-------------------------------------------------------		*/
/*		    c o r d s _ p o s t _ e v e n t			*/
/*	-------------------------------------------------------		*/
public	int	cords_post_event( char * message, char * nature, char * agent, char * tls )
{
	char	*	ihost;
	struct	occi_client * kptr;
	struct	occi_request * qptr;
	struct	occi_response * yptr;
	struct	occi_response * zptr;
	struct	occi_element * dptr;
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;
	struct	xml_atribut * bptr;
	struct	cordscript_element * lptr;
	struct	cordscript_element * rvalue;
	char	buffer[2048];

	if (!( ihost = rest_log_comons_identity(_CORDS_EVENT,agent, tls) ))
		return(46);

	sprintf(buffer,"%s/%s/",ihost,_CORDS_EVENT);

	liberate( ihost );

	if (!( kptr = occi_create_client( buffer, agent, tls ) ))
		return(46);
	else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
	{
		kptr = occi_remove_client( kptr );
		return(50);
	}
	else if ((!(dptr=occi_request_element(qptr,"occi.event.description" , message ) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.event.nature"      , nature  ) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.event.source"      , agent   ) )))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return(51);
	}
	else if (!( yptr = occi_client_post( kptr, qptr ) ))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return(52);
	}
	else
	{
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return(0);
	}
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

/*	---------------------------------------------------	*/
/*		c o r d s _ l i n k _ e l e m e n t   		*/
/*	---------------------------------------------------	*/
/*	this function appends a well formed OCCI element to	*/
/*	the element list of the OCCI request. The element	*/
/*	will describe a simple named xlink attribute for use 	*/
/*	by link expressions.					*/
/*	--------------------------------------------------	*/
private	struct occi_element * cords_link_element( 
		struct	occi_request * rptr,
		char * 	nptr,
		char * 	vptr )
{
	char	buffer[8192];
	if (!( nptr ))
		return((struct occi_element *) 0);
	else	sprintf(buffer,"occi.link.%s",nptr);
	if (!( vptr ))
		return((struct occi_element *) 0);
	else	return( occi_request_element( rptr, buffer, vptr ));
}

/*	---------------------------------------------------	*/
/*	 	  o c c i _ c r e a t e _ l i n k		*/
/*	---------------------------------------------------	*/
/*	issues a POST request for the creation of a link  	*/
/*	"from" a category instance "to" the indicated other	*/
/*	category instance.					*/
/*	---------------------------------------------------	*/	
public	struct	occi_response * occi_create_link( char * from, char * to, char * agent, char * tls )
{
	struct	occi_element  	* fptr;
	struct	occi_response 	* zptr;
	struct	occi_request 	* rptr;
	struct	occi_client	* cptr;
	struct	url		*  lptr;
	char 			* linkhost;

	if ( check_debug() )
		printf("occi_create_link(%s,%s)\n",from,to);
	if (!( lptr = analyse_url( from )))
		return((struct occi_response *) 0);
	else if (!( lptr = validate_url( lptr )))
		return((struct occi_response *) 0);
	else 	lptr->object = liberate( lptr->object );
	if (!( lptr->object = allocate_string( _OCCI_XLINK )))
		return((struct occi_response *) 0);
	else if (!( linkhost = serialise_url( lptr,"" )))
		return((struct occi_response *) 0);
	else if (!( cptr = occi_create_client( linkhost, agent, tls ) ))
		return((struct occi_response *) 0);
	else if (!(rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
		return((struct occi_response *) 0);
	else if (!( fptr = cords_link_element( rptr, "source", from ) ))
		return((struct occi_response *) 0);
	else if (!( fptr = cords_link_element( rptr, "target", to ) ))
		return((struct occi_response *) 0);
	else if (!( zptr = occi_client_post( cptr, rptr ) ))
		return(zptr);
	else	return(zptr);
}

/*	---------------------------------------------------	*/
/*	 	  o c c i _ d e l e t e _ l i n k s		*/
/*	---------------------------------------------------	*/
/*	issues a DELETE request to perform the deletion of 	*/
/*	all links of a category instance by specifying the 	*/
/*	"from" attribute of the parent category instance.	*/
/*	---------------------------------------------------	*/	
public	struct	occi_response * occi_delete_links( char * from, char * agent, char * tls )
{
	struct	occi_element  	* fptr;
	struct	occi_response 	* zptr;
	struct	occi_request 	* rptr;
	struct	occi_client	* cptr;
	struct	url		*  lptr;
	char 			* linkhost;

	if ( check_debug() )
		printf("occi_delete_links(%s)\n",from);
	if (!( lptr = analyse_url( from )))
		return((struct occi_response *) 0);
	else if (!( lptr = validate_url( lptr )))
		return((struct occi_response *) 0);
	else 	lptr->object = liberate( lptr->object );
	if (!( lptr->object = allocate_string( _OCCI_XLINK )))
		return((struct occi_response *) 0);
	else if (!( linkhost = serialise_url( lptr,"" )))
		return((struct occi_response *) 0);
	else if (!( cptr = occi_create_client( linkhost, agent, tls ) ))
		return((struct occi_response *) 0);
	else if (!(rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
		return((struct occi_response *) 0);
	else if (!( fptr = cords_link_element( rptr, "source", from ) ))
		return((struct occi_response *) 0);
	else if (!( zptr = occi_client_delete( cptr, rptr ) ))
		return(zptr);
	else	return(zptr);
}
/*	---------------------------------------------------	*/
/*	 	  o c c i _ d e l e t e _ l i n k s		*/
/*	---------------------------------------------------	*/
/*	issues a DELETE request to perform the deletion of 	*/
/*	the link  of a category instance by specifying the 	*/
/*	"from" and "to" attributes of parent category.		*/
/*	---------------------------------------------------	*/	
public	struct	occi_response * occi_delete_link( char * from, char * to, char * agent, char * tls )
{
	struct	occi_element  	* fptr;
	struct	occi_response 	* zptr;
	struct	occi_request 	* rptr;
	struct	occi_client	* cptr;
	struct	url		*  lptr;
	char 			* linkhost;

	if ( check_debug() )
		printf("occi_delete_link(%s)\n",from);
	if (!( lptr = analyse_url( from )))
		return((struct occi_response *) 0);
	else if (!( lptr = validate_url( lptr )))
		return((struct occi_response *) 0);
	else 	lptr->object = liberate( lptr->object );
	if (!( lptr->object = allocate_string( _OCCI_XLINK )))
		return((struct occi_response *) 0);
	else if (!( linkhost = serialise_url( lptr,"" )))
		return((struct occi_response *) 0);
	else if (!( cptr = occi_create_client( linkhost, agent, tls ) ))
		return((struct occi_response *) 0);
	else if (!(rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
		return((struct occi_response *) 0);
	else if (!( fptr = cords_link_element( rptr, "source", from ) ))
		return((struct occi_response *) 0);
	else if (!( fptr = cords_link_element( rptr, "target", to ) ))
		return((struct occi_response *) 0);
	else if (!( zptr = occi_client_delete( cptr, rptr ) ))
		return(zptr);
	else	return(zptr);
}

#include "stdnode.c"

#endif 	/* _occi_client_c */


