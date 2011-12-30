/* ---------------------------------------------------------------------------- */
/* Advanced Capabilities for Compatible One Resources Delivery System - ACCORDS	*/
/* (C) 2011 by Iain James Marshall <ijm667@hotmail.com>				*/
/* ---------------------------------------------------------------------------- */
/*										*/
/* This is free software; you can redistribute it and/or modify it		*/
/* under the terms of the GNU Lesser General Public License as			*/
/* published by the Free Software Foundation; either version 2.1 of		*/
/* the License, or (at your option) any later version.				*/
/*										*/
/* This software is distributed in the hope that it will be useful,		*/
/* but WITHOUT ANY WARRANTY; without even the implied warranty of		*/
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU		*/
/* Lesser General Public License for more details.				*/
/*										*/
/* You should have received a copy of the GNU Lesser General Public		*/
/* License along with this software; if not, write to the Free			*/
/* Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA		*/
/* 02110-1301 USA, or see the FSF site: http://www.fsf.org.			*/
/*										*/
/* ---------------------------------------------------------------------------- */
#ifndef	_occiserver_c
#define _occiserver_c

/*	---------------------------------------------------------	*/
/*			O C C I _ S E R V E R				*/
/*	---------------------------------------------------------	*/
/*	this module offers a complete and extensible OCCI server 	*/
/*	implementation based on an underlying HTTP / REST server 	*/
/*	the server will be activated for a particular collection	*/
/*	of OCCI classes passed as a parameter to the operational	*/
/*	public function of the same name.				*/
/*	---------------------------------------------------------	*/

#include "standard.h"
#include "rest.h"
#include "occi.h"
#include "xlink.h"

private	struct	occi_category * OcciServerLinkManager=(struct occi_category *) 0;
private	struct	occi_category * OcciServerMixinManager=(struct occi_category *) 0;
public	struct	occi_category * occi_cords_xlink_builder( char * domain, char * name );

/*	---------------------------------------------------------	*/
/*			o c c i _ f a i l u r e				*/
/*	---------------------------------------------------------	*/
/*	this local function hands all errors that occur prior to 	*/
/*	allocation of a rest response structure.			*/
/*	---------------------------------------------------------	*/
private	struct rest_response * occi_failure(struct rest_client * cptr,  int status, char * message )
{
	struct	rest_response * aptr;
	if (!( aptr = rest_allocate_response(cptr)))
		return( aptr );
	else	return( rest_html_response( aptr, status, message ) );
}

/*	---------------------------------------------------------	*/
/*			o c c i _ s u c c e s s				*/
/*	---------------------------------------------------------	*/
public	int	occi_success( struct rest_response * aptr )
{
	char			* nptr;
	struct	rest_header 	* hptr;

	if (!( hptr = rest_response_header( aptr, "Content-Type", "text/occi" ) ))
		return( 0 );

	else if (!( hptr = rest_response_header( aptr, "Content-Length", "4" ) ))
		return( 0 );

	else if (!( nptr = allocate_string( "OK\r\n" ) ))
		return( 0 );
	else
	{
		rest_response_body( aptr, nptr, 1 );
		return( 1 );
	}
}

/*	---------------------------------------------	*/  
/*	    o c c i _ a l l o c a t e _ u u i d		*/
/*	---------------------------------------------	*/  
public	char *	occi_allocate_uuid()
{
	return( rest_allocate_uuid() );
}

/*	---------------------------------------------------------	*/
/*			o c c i _ i n i t i a l i s e 			*/
/*	---------------------------------------------------------	*/
/*	this function handles the initialisation call from the		*/
/*	rest server prior to going online and processing messages	*/
/*	---------------------------------------------------------	*/
private	char *	occi_athorization=(char *) 0;
private	struct rest_server * occi_initialise( void * vptr, struct rest_server * sptr )
{
	struct	occi_category * optr;
	struct	rest_interface * iptr;
	struct	rest_header * hptr;

	if ( check_debug() )
		printf("   OCCI Server Initialisation \n");

	if ( occi_athorization )
	{
		if (!( hptr = rest_create_header( _OCCI_AUTHORIZE, occi_athorization ) ))
			return( sptr );
		else	sptr->headers = hptr;
	}

	if (!( optr = vptr ))
		return( sptr );
	else if (!( iptr = optr->interface ))
		return( sptr );
	else if (!( iptr->initialise ))
		return( sptr );
	else	return( (*iptr->initialise)( vptr, sptr ) );
}

/*	---------------------------------------------------------	*/
/*			o c c i _ a u t h o r i s e			*/
/*	---------------------------------------------------------	*/
/*	this function is called to perform request authentication	*/
/*	---------------------------------------------------------	*/
private	int	occi_authorise( void * vptr, struct rest_client * cptr, char * username, char * password )
{
	return(1);
}

/*	---------------------------------------------------------	*/
/*		o c c i _ s t a n d a r d _ s e c u r i t y		*/
/*	---------------------------------------------------------	*/
/*	this function is the default standard security policy.  	*/
/*	---------------------------------------------------------	*/
private	int	occi_standard_security( 
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr )
{
	return(1);
}

/*	---------------------------------------------------------	*/
/*			o c c i _ s e c u r i t y			*/
/*	---------------------------------------------------------	*/
/*	this function is called to ensure security is enforced.  	*/
/*	---------------------------------------------------------	*/
private	int	occi_security( void * vptr, struct rest_client * cptr, struct rest_request * rptr )
{
	struct	occi_category * optr;
	struct	rest_interface * iptr;

	if ( check_debug() )
		printf("   OCCI Server Security \n");

	if (!( optr = vptr ))
		return( 0 );
	else if (!( iptr = optr->interface ))
		return( 0 );
	else if (!( optr->access & _OCCI_SECURE ))
		return( 1 );
	else if (!( iptr->security ))
		return( occi_standard_security( optr, cptr, rptr ) );
	else	return( (*iptr->security) (optr, cptr, rptr ) );
}

/*	---------------------------------------------------------	*/
/*			o c c i _ e x t e n s i o n			*/
/*	---------------------------------------------------------	*/
/*	this function is called when new socket activity has been	*/
/*	detected for a socket of the indicated socerver extension	*/
/*	---------------------------------------------------------	*/
private	struct rest_extension * occi_extension( void * vptr, struct rest_server * sptr, 
struct rest_extension * xptr)
{
	struct	occi_category * optr;
	struct	rest_interface * iptr;

	if ( check_debug() )
		printf("   OCCI Server Extension \n");

	if (!( optr = vptr ))
		return( xptr );
	else if (!( iptr = optr->interface ))
		return( xptr );
	else if (!( iptr->initialise ))
		return( xptr );
	else	return( (*iptr->extension)( vptr, sptr, xptr ) );
}

/*	---------------------------------------------------	*/
/*		o c c i _ r e n d e r _ l i n k s		*/
/*	---------------------------------------------------	*/
/*	this function is called at the end rendering a GET	*/
/*	response to allow any links from the instance to be	*/
/*	appended to the response.				*/
/*	---------------------------------------------------	*/
public	int	occi_render_links( struct rest_response * aptr, char * id )
{
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	struct	rest_header 	* hptr;
	char			* mptr;
	char 			* wptr;
	if (!( aptr ))
		return(0);
	else if (!( id ))
		return(0);
	for (	nptr=occi_first_link_node();
		nptr != (struct occi_link_node *) 0;
		nptr = nptr->next )
	{
		if (!( lptr = nptr->contents ))
			continue;
		else if (!( lptr->source ))
			continue;
		else if (!( wptr = occi_category_id( lptr->source ) ))
			continue;
		else if ( strcmp( wptr, id ) != 0)
		{
			liberate( wptr );
			continue;
		}
		else	liberate( wptr );
		if (!( mptr = occi_http_link( OcciServerLinkManager, lptr->target, lptr->id ) ))
			continue;
		else if (!( hptr = rest_response_header( aptr, _OCCI_LINKHEAD, mptr )))
		{
			liberate(mptr);
			return(27);
		}
		else
		{
			liberate(mptr);
			continue;
		}
	}
	return(0);
}

/*	---------------------------------------------------	*/
/*		o c c i _ i n v o k e _ a c t i o n 		*/
/*	---------------------------------------------------	*/
/*	This function is called in order to invoke an occi	*/
/*	action through the provided optr,fptr combination	*/
/*	using the pptr, rptr, aptr information.			*/
/*	Actions are invoked in response to post methods	on	*/
/* 	specific instance identifiers.				*/
/*	---------------------------------------------------	*/
public	struct	rest_response * occi_invoke_action(
		struct occi_action * fptr,	/* the action description element 	*/
		struct occi_category * optr,	/* the category description		*/
		struct rest_client * cptr,	/* the calling rest client instance	*/
		struct rest_request * rptr,	/* the request received from the client	*/
		struct rest_response * aptr,	/* the response under preparation.	*/
		void * pptr )			/* the actual occi data structure	*/
{
	int	status;
	struct	rest_response * (*action)
			(
			struct occi_category * category, 
			struct rest_client * client, 
			struct rest_request * request,
			struct rest_response * response,
			void * data
			);

	/* -------------------------------------- */
	/* undefined actions are silently ignored */
	/* -------------------------------------- */
	if (!( action = fptr->action ))
		return( rest_response_status( aptr, 200, "OK" ) );

	/* ----------------------- */
	/* lanch the action target */
	/* ----------------------- */
	else 	return( (*action)( optr,cptr,rptr,aptr,pptr ) );
}

/*	---------------------------------------------------------	*/
/*	O C C I    s t a n d a r d    L I N K    m a n a g e m e n t	*/
/*	---------------------------------------------------------	*/

/*	---------------------------------------------------------	*/
/*		o c c i _ g e t _ c a p a c i t i e s			*/
/*	---------------------------------------------------------	*/
/*	this function is called by occi get handler to process an	*/
/*	eventual request for capacities. This function expects		*/
/*	the complete list of capacities to be generated.		*/ 
/*	---------------------------------------------------------	*/
private	struct rest_response * occi_get_capacities( 
		struct occi_category * optr,
		struct rest_client * cptr, 
		struct rest_request * rptr )
{
	struct rest_response * aptr;
	char	* mptr;
	char 	* ctptr;
	char	  clbuff[64];
	struct	rest_header * hptr;
	

	if (!( aptr = rest_allocate_response(cptr) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	else if (( hptr = rest_resolve_header( rptr->first, _HTTP_ACCEPT ))
			!= (struct rest_header *) 0)
		ctptr = hptr->value;
	else	ctptr = _OCCI_MIME_OCCI;

	if (!( strcasecmp( ctptr, _OCCI_MIME_JSON ) ))
	{
		if (!( mptr = occi_json_capacities( optr ) ))
			return( rest_response_status( aptr, 500, "Server Failure" ) );

		else	rest_response_body( aptr, mptr, _TEXT_BODY );

		sprintf(clbuff,"%u",strlen(mptr));

		if (!( hptr = rest_response_header( aptr, _HTTP_CONTENT_TYPE, _OCCI_MIME_JSON) ))
			return( rest_response_status( aptr, 500, "Server Failure" ) );
		else if (!( hptr = rest_response_header( aptr, _HTTP_CONTENT_LENGTH, clbuff )))
			return( rest_response_status( aptr, 500, "Server Failure" ) );
		else	return( rest_response_status( aptr, 200, "OK" ) );
	}
	else
	{
		for (	;
			optr != (struct occi_category *) 0;
			optr = optr->next )
		{
			if ( optr->access & _OCCI_SECRET )
				continue;
			else if (!( mptr = occi_http_capacity( optr ) ))
				continue;
			else if (!( hptr = rest_response_header( aptr, "Category", mptr ) ))
			{
				liberate( mptr );
				return( rest_html_response( aptr, 500, "Server Failure" ) );
			}
			else	liberate( mptr );
		}
	}

	if ((optr = OcciServerLinkManager) != (struct occi_category*) 0)
		if (!( optr->access & _OCCI_SECRET ))
			if (( mptr = occi_http_capacity( optr )) != (char *) 0)
				hptr = rest_response_header( aptr, "Category", mptr );

	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );

}


/*	---------------------------------------------------------	*/
/*		   o c c i _ s h o w _ r e q u e s t			*/
/*	---------------------------------------------------------	*/
private	void	occi_show_request( struct rest_request * rptr )
{
	struct	rest_header * hptr;
	if ( check_debug() )
	{
		printf("   OCCI Server Request  : %s %s?%s %s \n",rptr->method,
			rptr->object,
			( rptr->parameters ? rptr->parameters : "" ),
			rptr->version);
		if ( check_debug() )
		{
			for (	hptr=rptr->first;
				hptr != (struct rest_header *) 0;
				hptr = hptr->next )
				printf("   %s: %s \n",hptr->name,hptr->value);
		}
	}
	return;
}

/*	---------------------------------------------------------	*/
/*			o c c i _ i n v o k e _ g e t			*/
/*	---------------------------------------------------------	*/
/*	this function performs invocation of GET for a category		*/
/*	---------------------------------------------------------	*/
private	struct rest_response * occi_invoke_get(
	struct occi_category * optr,
	struct rest_client   * cptr, 
	struct rest_request  * rptr )
{
	struct	rest_response * aptr;
	struct	rest_interface * iptr;
	if (!( iptr = optr->interface ))
		return( occi_failure(cptr,  400, "Bad Request : No Methods" ) );
	else
	{	
		if ( iptr->before ) 	(*iptr->before)(optr,cptr,rptr);
		if ( iptr->get )	aptr = (*iptr->get)(optr,cptr,rptr);
		else			aptr = occi_failure(cptr,  405, "Bad Request : No Category Method" );
		if ( iptr->after )	(*iptr->after)(optr,cptr,rptr);
		return( aptr );
	}
}

/*	---------------------------------------------------------	*/
/*			o c c i _ i n v o k e _ p o s t 		*/
/*	---------------------------------------------------------	*/
/*	this function performs invocation of POST for a category	*/
/*	---------------------------------------------------------	*/
private	struct rest_response * occi_invoke_post(
	struct occi_category * optr,
	struct rest_client   * cptr, 
	struct rest_request  * rptr )
{
	struct	rest_response * aptr;
	struct	rest_interface * iptr;
	if (!( iptr = optr->interface ))
		return( occi_failure(cptr,  400, "Bad Request : No Methods" ) );
	else
	{	
		if ( iptr->before ) 	(*iptr->before)(optr,cptr,rptr);
		if ( iptr->post )	aptr = (*iptr->post)(optr,cptr,rptr);
		else			aptr = occi_failure(cptr,  405, "Bad Request : No Category Method" );
		if ( iptr->after )	(*iptr->after)(optr,cptr,rptr);
		return( aptr );
	}
}

/*	---------------------------------------------------------	*/
/*			o c c i _ i n v o k e _ p u t			*/
/*	---------------------------------------------------------	*/
/*	this function performs invocation of PUT for a category		*/
/*	---------------------------------------------------------	*/
private	struct rest_response * occi_invoke_put(
	struct occi_category * optr,
	struct rest_client   * cptr, 
	struct rest_request  * rptr )
{
	struct	rest_response * aptr;
	struct	rest_interface * iptr;
	if (!( iptr = optr->interface ))
		return( occi_failure(cptr,  400, "Bad Request : No Methods" ) );
	else
	{	
		if ( iptr->before ) 	(*iptr->before)(optr,cptr,rptr);
		if ( iptr->put )	aptr = (*iptr->put)(optr,cptr,rptr);
		else			aptr = occi_failure(cptr,  405, "Bad Request : No Category Method" );
		if ( iptr->after )	(*iptr->after)(optr,cptr,rptr);
		return( aptr );
	}
}

/*	---------------------------------------------------------	*/
/*		   o c c i _ i n v o k e _ d e l e t e			*/
/*	---------------------------------------------------------	*/
/*	this function performs invocation of DELETE of a category	*/
/*	---------------------------------------------------------	*/
private	struct rest_response * occi_invoke_delete(
	struct occi_category * optr,
	struct rest_client   * cptr, 
	struct rest_request  * rptr )
{
	struct	rest_response * aptr;
	struct	rest_interface * iptr;
	if (!( iptr = optr->interface ))
		return( occi_failure(cptr,  400, "Bad Request : No Methods" ) );
	else
	{	
		if ( iptr->before ) 	(*iptr->before)(optr,cptr,rptr);
		if ( iptr->delete )	aptr = (*iptr->delete)(optr,cptr,rptr);
		else			aptr = occi_failure(cptr,  405, "Bad Request : No Category Method" );
		if ( iptr->after )	(*iptr->after)(optr,cptr,rptr);
		return( aptr );
	}
}

/*	---------------------------------------------------------	*/
/*			o c c i _ i n v o k e _ h e a d			*/
/*	---------------------------------------------------------	*/
/*	this function performs invocation of HEAD for a category	*/
/*	---------------------------------------------------------	*/
private	struct rest_response * occi_invoke_head(
	struct occi_category * optr,
	struct rest_client   * cptr, 
	struct rest_request  * rptr )
{
	struct	rest_response * aptr;
	struct	rest_interface * iptr;
	if (!( iptr = optr->interface ))
		return( occi_failure(cptr,  400, "Bad Request : No Methods" ) );
	else
	{	
		if ( iptr->before ) 	(*iptr->before)(optr,cptr,rptr);
		if ( iptr->head )	aptr = (*iptr->head)(optr,cptr,rptr);
		else			aptr = occi_failure(cptr,  405, "Bad Request : No Category Method" );
		if ( iptr->after )	(*iptr->after)(optr,cptr,rptr);
		return( aptr );
	}
}

/*	---------------------------------------------------------	*/
/*		o c c i _ i n v o k e _ t r a n s a c t i o n		*/
/*	---------------------------------------------------------	*/
/*	this function launches financial transaction processing		*/
/*	---------------------------------------------------------	*/
private	struct	rest_response *	occi_invoke_transaction( 
	struct occi_category * optr,
	struct rest_client   * cptr, 
	struct rest_request  * rptr,
	struct rest_response * aptr )
{
	struct	rest_interface * iptr;

	/* --------------------------------------- */
	/* check if price handling is not required */
	/* --------------------------------------- */
	if ( optr->access & _OCCI_PRICING )
		return( aptr );
	else if (!( iptr = optr->interface ))
		return( aptr );
	else if (!( iptr->transaction ))
		return( aptr );
	else 	return( (*iptr->transaction) ( optr, cptr, rptr, aptr ) );
}

/*	---------------------------------------------------------	*/
/*				o c c i _ g e t				*/
/*	---------------------------------------------------------	*/
/*	this function is called by the rest server to handle http	*/
/*	get request processing.						*/
/*	---------------------------------------------------------	*/
private	struct rest_response * occi_get( 
		void * vptr,
		struct rest_client * cptr, 
		struct rest_request * rptr )
{
	struct	occi_category * optr;
	struct	rest_interface * iptr;

	occi_show_request( rptr );
	if (!( optr = vptr ))
		return( occi_failure(cptr,  400, "Bad Request : No Category" ) );
	else if (!( strcmp( rptr->object, "/-/" ) ))
		return( occi_get_capacities( optr, cptr, rptr ) );
	else if (((optr = OcciServerLinkManager) != (struct occi_category *) 0) 
	     &&  (!( strncmp( rptr->object, optr->location,strlen(optr->location)) )))
		return( occi_invoke_get( optr, cptr, rptr ) );
	else
	{
		for ( 	optr = vptr;
			optr != (struct occi_category *) 0;
			optr = optr->next )
			if (!( strncmp( rptr->object, optr->location, strlen(optr->location)) )) 
				return( occi_invoke_get( optr, cptr, rptr ) );
				
		return( occi_failure(cptr,  404, "Category Not Found" ) );
	}
}

/*	---------------------------------------------------------	*/
/*			     o c c i _ p o s t 				*/
/*	---------------------------------------------------------	*/
/*	this function is called by the rest server to handle http	*/
/*	post request processing by the OCCI server layers. This 	*/
/*	involves three types of possible operation:			*/
/*		Category Instance Creation				*/
/*		LINK processing						*/
/*		Action invocation					*/
/*	---------------------------------------------------------	*/
private	struct rest_response * occi_post( 
		void * vptr,
		struct rest_client * cptr, 
		struct rest_request * rptr )
{
	struct	occi_category 	* optr;
	struct	rest_interface 	* iptr;
	struct	rest_response 	* aptr;

	occi_show_request( rptr );

	if (!( optr = vptr ))
		return( occi_failure(cptr,  400, "Bad Request : No Category" ) );
	else if (!( strcmp( rptr->object, "/-/" ) ))
		return( occi_failure(cptr,  400, "Bad Request : Illegal Mixin Creation" ) );
	else if (((optr = OcciServerLinkManager) != (struct occi_category *) 0) 
	     &&  (!( strncmp( rptr->object, optr->location,strlen(optr->location)) )))
		return( occi_invoke_post( optr, cptr, rptr ) );
	else
	{
		/* -------------------------------------- */
		/* scan the list of associated categories */
		/* -------------------------------------- */
		for ( 	optr = vptr;
			optr != (struct occi_category *) 0;
			optr = optr->next )
		{
			/* ------------------------------------ */
			/* localise the requested category name */
			/* ------------------------------------ */
			if (!( strncmp( rptr->object, optr->location, strlen(optr->location)) ))
			{
				/* --------------------------------- */
				/* process the OCCI Category Request */
				/* --------------------------------- */
				if (!( aptr = occi_invoke_post( optr, cptr, rptr ) ))
					return( aptr );
				else if ( aptr->status >= 300 )
					return( aptr );
				else	return( occi_invoke_transaction( optr, cptr, rptr, aptr ) );
			}
		}
		return( occi_failure(cptr,  404, "Category Not Found" ) );
	}
}

/*	---------------------------------------------------------	*/
/*				o c c i _ p u t				*/
/*	---------------------------------------------------------	*/
/*	this function is called by the rest server to handle http	*/
/*	put request processing.						*/
/*	---------------------------------------------------------	*/
private	struct rest_response * occi_put( 
		void * vptr,
		struct rest_client * cptr, 
		struct rest_request * rptr )
{
	struct	occi_category   * optr;
	struct	rest_interface  * iptr;
	struct	rest_header	* hptr;

	occi_show_request( rptr );

	if (!( optr = vptr ))
		return( occi_failure(cptr,  400, "Bad Request : No Category" ) );
	else if ((hptr = rest_resolve_header( rptr->first, "Link" )))
		return( occi_failure(cptr,  400, "Bad Request : MUST not PUT Link" ) );
	else if (!( strcmp( rptr->object, "/-/" ) ))
		return( occi_failure(cptr,  400, "Bad Request : Illegal Mixin Update" ) );
	else if (((optr = OcciServerLinkManager) != (struct occi_category *) 0) 
	     &&  (!( strncmp( rptr->object, optr->location,strlen(optr->location)) )))
		return( occi_invoke_put( optr, cptr, rptr ) );
	{
		for ( 	optr = vptr;
			optr != (struct occi_category *) 0;
			optr = optr->next )
			if (!( strncmp( rptr->object, optr->location, strlen(optr->location)) )) 
				return( occi_invoke_put( optr, cptr, rptr ) );

		return( occi_failure(cptr,  404, "Category Not Found" ) );
	}
}

/*	---------------------------------------------------------	*/
/*			o c c i _ d e l e t e				*/
/*	---------------------------------------------------------	*/
/*	this function is called by the rest server to handle http	*/
/*	delete request processing.					*/
/*	---------------------------------------------------------	*/
private	struct rest_response * occi_delete(
		void * vptr,
		struct rest_client * cptr, 
		struct rest_request * rptr )
{
	struct	occi_category * optr;
	struct	rest_interface * iptr;

	occi_show_request( rptr );

	if (!( optr = vptr ))
		return( occi_failure(cptr,  400, "Bad Request : No Category" ) );

	else if (!( strcmp( rptr->object, "/-/" ) ))
		return( occi_failure(cptr,  400, "Bad Request : Illegal Mixin Removal" ) );
	else if (((optr = OcciServerLinkManager) != (struct occi_category *) 0) 
	     &&  (!( strncmp( rptr->object, optr->location,strlen(optr->location)) )))
		return( occi_invoke_delete( optr, cptr, rptr ) );
	else
	{
		for ( 	optr = vptr;
			optr != (struct occi_category *) 0;
			optr = optr->next )
			if (!( strncmp( rptr->object, optr->location, strlen(optr->location)) )) 
				return( occi_invoke_delete( optr, cptr, rptr ) );

		return( occi_failure(cptr,  404, "Category Not Found" ) );
	}
}

/*	---------------------------------------------------------	*/
/*			o c c i _ h e a d 				*/
/*	---------------------------------------------------------	*/
/*	this function is called by the rest server to handle http	*/
/*	head request processing.					*/
/*	---------------------------------------------------------	*/
private	struct rest_response * occi_head(
		void * vptr,
		struct rest_client * cptr, 
		struct rest_request * rptr )
{
	struct	occi_category * optr;
	struct	rest_interface * iptr;

	occi_show_request( rptr );

	if (!( optr = vptr ))
		return( occi_failure(cptr,  400, "Bad Request : No Category" ) );
	else if (((optr = OcciServerLinkManager) != (struct occi_category *) 0) 
	     &&  (!( strncmp( rptr->object, optr->location,strlen(optr->location)) )))
		return( occi_invoke_head( optr, cptr, rptr ) );
	else
	{
		for ( 	optr = vptr;
			optr != (struct occi_category *) 0;
			optr = optr->next )
			if (!( strncmp( rptr->object, optr->location, strlen(optr->location)) )) 
				return( occi_invoke_head( optr, cptr, rptr ) );

		return( occi_failure(cptr,  404, "Category Not Found" ) );
	}
}

/*	---------------------------------------------	*/  
/*	  o c c i _ p r o c e s s _ a t r i b u t s	*/
/*	---------------------------------------------	*/  
public	int	occi_process_atributs(
		struct occi_category * optr,
		struct rest_request * rptr,
		struct rest_response * aptr,
		void * pptr,
		void (*method)(struct occi_category *,void *,char *, char *)
		)

{
	char	*	mptr;
	char	*	nptr;
	char 	*	vptr;
	struct	rest_header * hptr;

	for ( 	hptr = rest_resolve_header( rptr->first, "X-OCCI-Attribute");
		hptr != (struct rest_header *) 0;
		hptr = hptr->next )
	{
		if ( strcasecmp(hptr->name,"X-OCCI-Attribute") != 0)
			continue;
		else if (!( hptr->value ))
			continue;
		else if (!( mptr = allocate_string( hptr->value ) ))
			return(0);
		else
		{
			for ( nptr=mptr; *nptr == ' '; nptr++);
			for ( vptr=nptr; *vptr != 0; vptr++ )
			{
				if ( *vptr == '=' )
				{
					*(vptr++) = 0;
					break;
				}
			}	
			for ( ; *vptr == ' '; vptr++);
			(*method)(optr,pptr, nptr, vptr);
			liberate( mptr );
		}
	}
	return(1);
}

/*	---------------------------------------------------------	*/
/*			o c c i _ s e r v e r				*/
/*	---------------------------------------------------------	*/
/*	this function is to be called to instance an OCCI server	*/
/*	operating over an underlying  REST server to provide the	*/
/*	processing functions needed for each HTTP method.		*/
/*	---------------------------------------------------------	*/
public	int	occi_server( char * nptr, int port, char * tls, int max, 
		struct occi_category * category, char * athorization )
{
	int	status=0;
	struct	rest_interface  Osi = 
	{
		(void * ) 0,
		occi_initialise,
		occi_authorise,
		occi_get,
		occi_post,
		occi_put,
		occi_delete,
		occi_head,
		occi_extension,
		(void *) 0,
		(void *) 0,
		occi_security,
		(void *) 0
	};

	if ( tls )
		if (!( strlen(tls) ))
			tls = (char *) 0;

	occi_athorization = athorization;
	Osi.authorise = (void *) 0;
	Osi.instance  = category;

	/* --------------------------------------------------------------- */
	/* all compatible one servers have a default external link handler */
	/* --------------------------------------------------------------- */
	if (!(OcciServerLinkManager = occi_cords_xlink_builder( "occi", "link" ) ))
		return( 27 );
	else	OcciServerLinkManager->access |= _OCCI_PRIVATE;

	/* ------------------------------------------ */
	/* this parameter now controls thread workers */
	/* ------------------------------------------ */
	if ( max )	rest_thread_control(1);
	else		rest_thread_control(0);

	/* --------------------------------- */
	/* launch the REST HTTP Server layer */
	/* --------------------------------- */
	return( rest_server(  nptr, port, tls, 0, &Osi ) );
}


#endif	/* _occiserver_c */
	/* ------------- */

