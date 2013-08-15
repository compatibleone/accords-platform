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
#include "url.h"
#include "urlpublic.h"
#include "occiauth.h"
#include "cordslang.h"
#include "occiclient.h"
#include "occiresolver.h"
#include "occibody.h"
#include "occilogin.h"
#include "json.h"
#include "restclient.h"

struct	accords_authorization
{
	struct	accords_authorization * previous;
	struct	accords_authorization * next;
	char *	user;
	char *	pass;
	char *	authorization;
};

private	struct	occi_category * OcciLocalCategory=(struct occi_category *) 0;
private	struct	occi_category * OcciServerLinkManager=(struct occi_category *) 0;
private	struct	occi_category * OcciServerMixinManager=(struct occi_category *) 0;
private	struct	accords_authorization * AccordsAuthorization=(struct accords_authorization *) 0;
public	struct	occi_category * occi_cords_xlink_builder( char * domain, char * name );
private	char *	occi_authorization=(char *) 0;

private	struct rest_response * occi_get( 
		void * vptr,
		struct rest_client * cptr, 
		struct rest_request * rptr );

private	struct rest_response * occi_put( 
		void * vptr,
		struct rest_client * cptr, 
		struct rest_request * rptr );

private	struct rest_response * occi_post( 
		void * vptr,
		struct rest_client * cptr, 
		struct rest_request * rptr );

private	struct rest_response * occi_delete( 
		void * vptr,
		struct rest_client * cptr, 
		struct rest_request * rptr );

private	struct rest_response * occi_head( 
		void * vptr,
		struct rest_client * cptr, 
		struct rest_request * rptr );


public	struct rest_response * occi_alert(
		void * vptr,
		struct rest_client * cptr, 
		struct rest_response * rptr,
		char * name,
		int status, 
		char * message, char * nature, 
		char * agent, char * tls );

private	struct rest_response * occi_alert_relay_impl(
		void * vptr,
		struct rest_client * cptr,
		struct rest_response * rptr,
		char * name,
		int status,
		char * message, char * nature,
		char * agent, char * tls );

private	struct rest_response *  (*occi_alert_relay)(
		void * i, 
		struct rest_client * cptr, 
		struct rest_response * rptr, 
		char * name,
		int status, char * message, 
		char * nature, 
		char * agent, char * tls)=&occi_alert_relay_impl;


/*	---------------------------------------------------------	*/
/*		s e t _ o c c i _ a l e r t _ r e l a y 		*/
/*	---------------------------------------------------------	*/
public	void	set_occi_alert_relay( struct rest_response *  (*relay)(
		void * i, 
		struct rest_client * cptr, 
		struct rest_response * rptr, 
		char * name,
		int status, char * message, 
		char * nature, 
		char * agent, char * tls))
{
	occi_alert_relay = relay;
	return;
}

/*	---------------------------------------------------------	*/
/*			o c c i _ f a i l u r e				*/
/*	---------------------------------------------------------	*/
/*	this local function hands all errors that occur prior to 	*/
/*	allocation of a rest response structure.			*/
/*	---------------------------------------------------------	*/
private	struct rest_response * occi_failure(struct rest_client * cptr,  int status, char * message )
{
	struct	rest_response * aptr;
	struct	rest_header * hptr;
	if (!( aptr = rest_allocate_response(cptr)))
		return( aptr );
	else 
	{
		switch ( status )
		{
		case	401	:
			hptr = rest_response_header( aptr, _HTTP_WWW_AUTHENTICATE, _HTTP_BASIC );
			break;
		case	407	:
			hptr = rest_response_header( aptr, _HTTP_PROXY_AUTHENTICATE, _HTTP_BASIC );
			break;
		}
		return( rest_html_response( aptr, status, message ) );
	}
}

/*	---------------------------------------------------------	*/
/*			o c c i _ r e d i r e c t			*/
/*	---------------------------------------------------------	*/
/*	this local function redirects to the resource specified		*/
/*	in the fourth parameter.						*/
/*	---------------------------------------------------------	*/
private	struct rest_response * occi_redirect(struct rest_client * cptr,  int status, char * message, char * target )
{
	struct	rest_response * aptr;
	struct	rest_header * hptr;
	if (!( aptr = rest_allocate_response(cptr)))
		return( aptr );
	else if (!( hptr = rest_response_header( aptr, _HTTP_LOCATION, target ) ))
		return( rest_html_response( aptr, 500, message ) );
	else	return( rest_html_response( aptr, status, message ) );
}

/*	---------------------------------------------------------	*/
/*			o c c i _ s u c c e s s				*/
/*	---------------------------------------------------------	*/
public	int	occi_success( struct rest_response * aptr )
{
	char			* nptr;
	struct	rest_header 	* hptr;

	if (!( hptr = rest_resolve_header( aptr->first, _HTTP_ACCEPT ) ))
		if (!( hptr = occi_accept_header( aptr ) ))
			return( 0 );
	if ((hptr = rest_resolve_header( aptr->first, _HTTP_CONTENT_TYPE )) != (struct rest_header *) 0)
		return( 0 );	
	else if (!( hptr = rest_response_header( aptr, _HTTP_CONTENT_TYPE, "text/occi" ) ))
		return( 0 );

	else if (!( hptr = rest_response_header( aptr, _HTTP_CONTENT_LENGTH, "4" ) ))
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
private	struct rest_server * occi_initialise( void * vptr, struct rest_server * sptr )
{
	struct	occi_category * optr;
	struct	rest_interface * iptr;
	struct	rest_header * hptr;

	if ( check_debug() )
		printf("   OCCI Server Initialisation \n");

	if ( occi_authorization )
	{
		if (!( hptr = rest_create_header( _OCCI_AUTHORIZE, occi_authorization ) ))
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

/*	---------------------------------------------	*/
/*	o c c i _ n e x t _ c a t e g o r y _ l i n k	*/
/*	---------------------------------------------	*/
/*	category link filtering to select a specific	*/
/*	category instance collection of links.		*/
/*	---------------------------------------------	*/
public struct  occi_link_node * occi_next_category_link( 
		struct occi_link_node * nptr, 
		char * category, 
		char * id )
{
	struct	cords_xlink	* lptr;
	struct	url * uptr;
	char	buffer[1024];
	/* -------------------- */
	/* check the parameters */
	/* -------------------- */
	if (!( nptr ))
		return( nptr );
	else if (!( category ))
		return( nptr );
	else if (!( id ))
		return( nptr );
	{
		/* --------------------------------------------- */
		/* build the source category instance identifier */
		/* --------------------------------------------- */
		sprintf(buffer,"/%s/%s",category,id);

		/* ---------------------------------------- */
		/* scan the list of category instance links */
		/* ---------------------------------------- */
		while ( nptr )
		{
			if (( lptr = nptr->contents ) != (struct cords_xlink *) 0)
			{
				if (( uptr = analyse_url( lptr->source ) ) != (struct url *) 0)
				{
					if (!( strcmp( uptr->object, buffer ) ))
						break;
					uptr = liberate_url( uptr );
				}
			}
			nptr = nptr->next;
		}
		if ( uptr )
			uptr = liberate_url( uptr );

		return( nptr );
	}
}

/*	-----------------------------------------------		*/
/*	o c c i _ f i r s t _ c a t e g o r y _ l i n k		*/
/*	-----------------------------------------------		*/
public struct  occi_link_node * occi_first_category_link( 
		char * category, 
		char * id )
{
	return( occi_next_category_link( occi_first_link_node(), category, id ) );
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

/*	---------------------------------------------------	*/
/*	This function is called in order to invoke an occi	*/
/*	action through the provided optr,fptr combination	*/
/*	using the pptr, rptr, aptr information.			*/
/*	Actions are invoked in response to post methods	on	*/
/* 	specific instance identifiers.				*/
/*	---------------------------------------------------	*/
public	struct	rest_response * occi_invoke_action_with_parameters(
		struct occi_action * fptr,	/* the action description element 	*/
		struct rest_header * hptr,	/* the parameter value attributes 	*/
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
	struct	occi_action * actptr;
	struct	occi_category * foot;
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

	if  ((!( strcasecmp( ctptr, _OCCI_OCCI_JSON ) ))
	||   (!( strcasecmp( ctptr, _OCCI_APP_JSON  ) ))
	||   (!( strcasecmp( ctptr, _OCCI_OLD_JSON  ) ))
	||   (!( strcasecmp( ctptr, _OCCI_TEXT_JSON ) )))
	{
		if (!( mptr = occi_json_capacities( optr ) ))
			return( rest_response_status( aptr, 500, "Server Failure" ) );

		else	rest_response_body( aptr, mptr, _TEXT_BODY );

		sprintf(clbuff,"%u",strlen(mptr));

		if (!( hptr = occi_accept_header( aptr ) ))
			return( rest_response_status( aptr, 500, "Server Failure" ) );
		else if (!( hptr = rest_response_header( aptr, _HTTP_CONTENT_TYPE, ctptr) ))
			return( rest_response_status( aptr, 500, "Server Failure" ) );
		else if (!( hptr = rest_response_header( aptr, _HTTP_CONTENT_LENGTH, clbuff )))
			return( rest_response_status( aptr, 500, "Server Failure" ) );
		else	return( rest_response_status( aptr, 200, "OK" ) );
	}
	else if ( accept_string_includes( ctptr, _OCCI_TEXT_HTML ) )
	{
		if (!( hptr = rest_response_header( aptr, _HTTP_CONTENT_TYPE, _OCCI_TEXT_HTML ) ))
			return( rest_response_status( aptr, 500, "Server Failure" ) );
		else if (!( mptr = occi_html_capacities( optr, aptr ) ))
			return( rest_response_status( aptr, 500, "Server Failure" ) );
		else
		{
			rest_response_body( aptr, mptr, _FILE_BODY );
			return( rest_response_status( aptr, 200, "OK" ) );
		}
	}
	else if (( accept_string_includes( ctptr, _OCCI_TEXT_XML ) )
	     ||  ( accept_string_includes( ctptr, _OCCI_APPLICATION_XML ) )
	     ||  ( accept_string_includes( ctptr, _OCCI_X_APPLICATION_XML ) ))
	{
		if (!( hptr = rest_response_header( aptr, _HTTP_CONTENT_TYPE, _OCCI_APPLICATION_XML ) ))
			return( rest_response_status( aptr, 500, "Server Failure" ) );
		else if (!( mptr = occi_xml_capacities( optr, aptr ) ))
			return( rest_response_status( aptr, 500, "Server Failure" ) );
		else
		{
			rest_response_body( aptr, mptr, _FILE_BODY );
			return( rest_response_status( aptr, 200, "OK" ) );
		}
	}
	else if ( accept_string_includes( ctptr, _OCCI_TEXT_OCCI ) )
	{
		for (	foot=optr;
			optr != (struct occi_category *) 0;
			optr = optr->next )
		{
			/* ------------------------------- */
			/* append the category description */
			/* ------------------------------- */
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
			/* --------------------------------------- */
			/* append the action category descriptions */
			/* --------------------------------------- */
			for (	actptr=optr->firstact;
				actptr != (struct occi_action *) 0;
				actptr = actptr->next )
			{
				if (!( actptr->binding ))
					continue;
				else if (!( mptr = occi_http_capacity( actptr->binding ) ))
					continue;
				else if (!( hptr = rest_response_header( aptr, "Category", mptr ) ))
				{
					liberate( mptr );
					return( rest_html_response( aptr, 500, "Server Failure" ) );
				}
			}
		}
		optr = foot;
		if (!( hptr = occi_accept_header( aptr ) ))
			return( rest_response_status( aptr, 500, "Server Failure" ) );

	}
	else if (( accept_string_includes( ctptr, _OCCI_TEXT_PLAIN ) )
	     ||  ( accept_string_includes( ctptr, "*/*" ) ))
	{
		if (!( hptr = rest_response_header( aptr, _HTTP_CONTENT_TYPE, _OCCI_TEXT_PLAIN ) ))
			return( rest_response_status( aptr, 500, "Server Failure" ) );
		else if (!( mptr = occi_text_capacities( optr, aptr ) ))
			return( rest_response_status( aptr, 500, "Server Failure" ) );
		else
		{
			rest_response_body( aptr, mptr, _FILE_BODY );
			return( rest_response_status( aptr, 200, "OK" ) );
		}
	}

#ifdef	_need_to_add_link_manager
	if ((optr = OcciServerLinkManager) != (struct occi_category*) 0)
		if (!( optr->access & _OCCI_SECRET ))
			if (( mptr = occi_http_capacity( optr )) != (char *) 0)
				hptr = rest_response_header( aptr, "Category", mptr );
#endif
	if (!( aptr = occi_content_type( optr, rptr, aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	if (!( occi_success( aptr ) ))
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
/*			o c c i _ c o n t e n t _ t y p e		*/
/*	---------------------------------------------------------	*/
public 	struct rest_response *	occi_content_type( 
	struct occi_category * cptr,
	struct rest_request * qptr, 
	struct rest_response * rptr )
{
	char *	body;
	struct	rest_header * hptr;
	if (!( rptr ))
		return( rptr );
	else if (!( hptr = rest_resolve_header( qptr->first, _HTTP_ACCEPT ) ))
		return( rptr );	
	else if (!( strcasecmp( hptr->value, _OCCI_TEXT_OCCI ) ))
		return( rptr );
	else if (!( body = occi_response_body( hptr->value, cptr, rptr->first ) ))
		return( rptr );
	else 	
	{
		rest_response_body( rptr, body, _FILE_BODY );
		return( rptr );
	}
}

/*	---------------------------------------------------------	*/
/*		o c c i _ t e x t _ r e q u e s t _ b o d y 		*/
/*	---------------------------------------------------------	*/
private	struct	rest_request *	occi_text_request_body(
	struct occi_category * optr, 
	struct rest_request * rptr)
{
	return( rptr );
}

/*	---------------------------------------------------------	*/
/*		o c c i _ h t m l _ r e q u e s t _ b o d y 		*/
/*	---------------------------------------------------------	*/
private	struct	rest_request *	occi_html_request_body(
	struct occi_category * optr, 
	struct rest_request * rptr)
{
	return( rptr );
}

/*	---------------------------------------------------------	*/
/*		o c c i _ x m l _ r e q u e s t _ b o d y 		*/
/*	---------------------------------------------------------	*/
private	struct	rest_request *	occi_xml_request_body(
	struct occi_category * optr, 
	struct rest_request * rptr)
{
	return( rptr );
}

/*	---------------------------------------------------------	*/
/*		o c c i _ p h p _ r e q u e s t _ b o d y 		*/
/*	---------------------------------------------------------	*/
private	struct	rest_request *	occi_php_request_body(
	struct occi_category * optr, 
	struct rest_request * rptr)
{
	return( rptr );
}

/*	---------------------------------------------------------	*/
/*		o c c i _ f o r m _ r e q u e s t _ t o k e n 		*/
/*	---------------------------------------------------------	*/
private	char *	occi_form_request_token( FILE * h, int t )
{
	char 	buffer[8192];
	int	i=0;
	int	c;
	while ((c = fgetc( h )) > 0 )
	{
		if ( c == t )
			break;
		else if ( i < 8190 )
			buffer[i++] = c;
		else	break;
	}
	if (!( i ))
		return((char *) 0);
	else
	{
		buffer[i++] = 0;
		return( allocate_string( buffer ) );
	}
}

/*	---------------------------------------------------------	*/
/*		o c c i _ f o r m _ r e q u e s t _ b o d y 		*/
/*	---------------------------------------------------------	*/
private	struct	rest_request *	occi_form_request_body(
	struct occi_category * optr, 
	struct rest_request * rptr)
{
	FILE * h;
	char *	domain=(char *) 0;
	char *	category=(char *) 0;
	char *	name=(char *) 0;
	char *	value=(char *) 0;

	struct rest_header * hptr=(struct rest_header *) 0;

	char 	buffer[4048];

	if (!( rptr->body ))
		return( rptr );
	else if (!( h = fopen( rptr->body,"r" ) ))
		return( rptr );
	else
	{
		while ((name = occi_form_request_token( h,'=' )) != (char *) 0)
		{
			if (!(value = occi_form_request_token( h, '&' ) ))
			{
				liberate( name );
				continue;
			}
			else if (!( strcmp( name, "domain" ) ))
			{
				liberate( name );
				domain=value;
				continue;
			}
			else if (!( strcmp( name, "category" ) ))
			{
				liberate( name );
				category=value;
				continue;
			}
			else
			{
				sprintf(buffer,"%s.%s.%s=%s",
					(domain ? domain : "occi" ),
					(category ? category : "category" ),
					name,
					(value ? value : "" ));
				if (!( hptr = rest_request_header( rptr, _OCCI_ATTRIBUTE, buffer ) ))
					break;
				else	liberate( value );
			}
		}
		fclose(h);
		if ( domain ) liberate( domain );
		if ( category ) liberate( category );
		return( rptr );
	}

}

/*	---------------------------------------------------------	*/
/*		o c c i _ j s o n _ r e q u e s t _ b o d y 		*/
/*	---------------------------------------------------------	*/
private	struct	rest_request *	occi_json_request_body(
	struct occi_category * optr, 
	struct rest_request * rptr)
{
	struct	rest_header * eptr;
	char	*	nptr;
	char	*	vptr;
	char	*	domain;
	char 	*	category;
	struct	data_element * dptr;
	struct	data_element * root;
	struct	data_element * cptr;
	struct	data_element * bptr;
	char	buffer[2048];

	if (!( dptr = json_parse_file( rptr->body ) ))
		return( rptr );
	else	root = dptr;
	/* ------------------------------------------ */
	/* step over the first one : its the filename */
	/* ------------------------------------------ */
	if (!( dptr = dptr->first ))
	{
		drop_data_element( root );
		return( rptr );
	}
	else if (!( domain = dptr->name ))
	{
		drop_data_element( root );
		return( rptr );
	}
	else if (!( dptr = dptr->first ))
	{
		drop_data_element( root );
		return( rptr );
	}
	else if (!( category = dptr->name ))
	{
		drop_data_element( root );
		return( rptr );
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
			else if (!( strcmp( nptr, "location" ) ))
			{
				for (	bptr = cptr->first;
					bptr != (struct data_element *) 0;
					bptr = bptr->next )
				{
					if (!( vptr = occi_unquoted_value( bptr->value ) ))
						break;
					else if (!(eptr=rest_request_header(rptr,optr->id,vptr)))
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
				if (!(eptr=rest_request_header(rptr,buffer,vptr)))
					break;
				else
				{
					liberate( nptr );
					liberate( vptr );
					continue;
				}
			}
		}
		drop_data_element( root );
		return( rptr );
	}
}

/*	---------------------------------------------------------	*/
/*	   o c c i _ o l d _ j s o n _ r e q u e s t _ b o d y 		*/
/*	---------------------------------------------------------	*/
private	struct	rest_request *	occi_old_json_request_body(
	struct occi_category * optr, 
	struct rest_request * rptr)
{
	struct	rest_header * eptr;
	char	*	nptr;
	char	*	vptr;
	struct	data_element * dptr;
	struct	data_element * root;
	struct	data_element * cptr;
	struct	data_element * bptr;

	if (!( dptr = json_parse_file( rptr->body ) ))
		return( rptr );
	else	root = dptr;

	/* ------------------------------------------ */
	/* step over the first one : its the filename */
	/* ------------------------------------------ */
	if (!( dptr = dptr->first ))
	{
		drop_data_element( root );
		return( rptr );
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
			else if (!( strcmp( nptr, "location" ) ))
			{
				for (	bptr = cptr->first;
					bptr != (struct data_element *) 0;
					bptr = bptr->next )
				{
					if (!( vptr = occi_unquoted_value( bptr->value ) ))
						break;
					else if (!(eptr=rest_request_header(rptr,optr->id,vptr)))
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
				else if (!(eptr=rest_request_header(rptr,nptr,vptr)))
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
					else if (!(eptr=rest_request_header(rptr,nptr,vptr)))
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
		return( rptr );
	}
}

/*	---------------------------------------------------------	*/
/*		    o c c i _ r e q u e s t _ b o d y 			*/
/*	---------------------------------------------------------	*/
private	struct	rest_request *	occi_request_body(
	struct occi_category * optr, 
	struct rest_request * rptr)
{
	struct	rest_header * hptr;
	if (!( hptr = rest_resolve_header( rptr->first, _HTTP_CONTENT_LENGTH ) ))
		return( rptr );
	else if (!( hptr = rest_resolve_header( rptr->first, _HTTP_CONTENT_TYPE ) ))
		return( rptr );
	else if (!( strcasecmp( hptr->value, _OCCI_TEXT_OCCI ) ))
		return( occi_text_request_body( optr, rptr ) );
	else if ((!( strcasecmp( hptr->value, _OCCI_OCCI_PHP ) ))
	     ||  (!( strcasecmp( hptr->value, _OCCI_APP_PHP  ) ))
	     ||  (!( strcasecmp( hptr->value, _OCCI_TEXT_PHP ) )))
		return( occi_php_request_body( optr, rptr ) );
	else if ((!( strcasecmp( hptr->value, _OCCI_OCCI_JSON ) ))
	     ||  (!( strcasecmp( hptr->value, _OCCI_APP_JSON  ) ))
	     ||  (!( strcasecmp( hptr->value, _OCCI_TEXT_JSON ) )))
		return( occi_json_request_body( optr, rptr ) );
	else if (!( strcasecmp( hptr->value, _OCCI_OLD_JSON ) ))
		return( occi_old_json_request_body( optr, rptr ) );
	else if (!( strcasecmp( hptr->value, _OCCI_TEXT_FORM ) ))
		return( occi_form_request_body( optr, rptr ) );
	else if ((!( strcasecmp( hptr->value, _OCCI_MIME_XML  ) ))
	     ||  (!( strcasecmp( hptr->value, _OCCI_APP_XML   ) ))
	     ||  (!( strcasecmp( hptr->value, _OCCI_TEXT_XML  ) )))
		return( occi_xml_request_body( optr, rptr ) );
	else	return( rptr );
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
		return( occi_content_type( optr, rptr, aptr ) );
	}
}

/*	---------------------------------------------------------	*/
/*	    	o c c i _ i n v o k e _ p s e u d o 			*/
/*	---------------------------------------------------------	*/
private	struct rest_response * occi_invoke_pseudo(
	char *	action,
	struct rest_header   * hptr,
	struct occi_category * optr,
	struct rest_client   * cptr, 
	struct rest_request  * rptr )
{
	struct rest_header   * xptr;
	int	l;
	int	i;
	char *	sptr;
	char *	wptr;
	char *	tptr;

	/* ------------------------------------- */
	/* substitute the method with the action */
	/* ------------------------------------- */
	if ( rptr->method )	rptr->method = liberate( rptr->method );
	rptr->method = action;

	/* ---------------------------------------- */
	/* if it is a POST : remove the instance id */
	/* ---------------------------------------- */
	if (!( strcmp( action, "POST" ) ))
	{
		for ( 	sptr=rptr->object, l=0, i=0; 
			*(sptr+i) != 0;
			i++ )
			if ( *(sptr+i) == '/' )
				l = (i+1);
		*(sptr+l) = 0;
	}

	/* ----------------------------- */
	/* rebuild the parameters string */
	/* ----------------------------- */
	if ( rptr->parameters )
	{
		rptr->parameters = liberate( rptr->parameters );
		for (	wptr=(char *) 0, xptr=hptr;
			xptr != (struct rest_header *) 0;
			xptr = xptr->next )
		{
			if (!( sptr = allocate(
				strlen( ( xptr->name  ? xptr->name  : "\0" ) ) + 
				strlen( ( xptr->value ? xptr->value : "\0" ) ) + 2 ) ))
			{
				break;
			}
			else
			{
				sprintf(sptr,"%s=%s",
					( xptr->name  ? xptr->name  : "\0" ),
					( xptr->value ? xptr->value : "\0" ));
				if (!( wptr ))
					wptr = sptr;
				else if (!( tptr = allocate( strlen( wptr ) + strlen( sptr ) + 2 ) ))
					break;
				else
				{
					sprintf(tptr,"%s&%s",wptr,sptr);
					sptr = liberate( sptr );
					wptr = liberate( wptr );
					wptr = tptr;
				}
			}
		}
		rptr->parameters = wptr;
	}
	if ( hptr )	hptr = liberate_rest_headers( hptr );

	/* ------------------------------------ */
	/* dispatch the adjusted action request */
	/* ------------------------------------ */
	if (!( strcmp( action,"POST" ) ))
		return( occi_post( optr, cptr, rptr ) );
	else if (!( strcmp( action,"GET" ) ))
		return( occi_get( optr, cptr, rptr ) );
	else if (!( strcmp( action,"DELETE" ) ))
		return( occi_delete( optr, cptr, rptr ) );
	else if (!( strcmp( action,"PUT" ) ))
		return( occi_put( optr, cptr, rptr ) );
	else if (!( strcmp( action,"HEAD" ) ))
		return( occi_head( optr, cptr, rptr ) );
	else	return( occi_failure(cptr,  500, "Server Failure" ) );
}

/*	---------------------------------------------------------	*/
/*		o c c i _ e x t r a c t _ p a r a m e t e r s		*/
/*	---------------------------------------------------------	*/
/*	converts a "name=value&name=value" type url string to a		*/
/*	list of name value header pairs.				*/
/*	---------------------------------------------------------	*/
private	struct	rest_header * occi_extract_parameters( char * sptr )
{
	char *	wptr;
	char *	nptr;
	char *	vptr;
	struct	rest_header * root=(struct rest_header *) 0;
	struct	rest_header * foot=(struct rest_header *) 0;
	struct	rest_header * hptr=(struct rest_header *) 0;

	if (!( sptr ))
		return( root );
	else if (!( wptr = allocate_string( sptr ) ))
		return( root );
	else	sptr = wptr;

	while (*wptr)
	{
		/* ----------------------- */
		/* step over name to value */
		/* ----------------------- */
		for (	nptr=wptr; *wptr != 0; wptr++ )	
			if (( *wptr == '=' ) || ( *wptr == '&' ))
				break;

		if ( *wptr == '=' )
		{
			*(wptr++) = 0;
			/* ----------------------- */
			/* step over value to name */
			/* ----------------------- */
			for (	vptr=wptr; *wptr != 0; wptr++ )
				if ( *wptr == '&' )
					break;
			if ( *wptr == '&' )
				*(wptr++) = 0;
		}		
		/* ---------- */
		/* null value */
		/* ---------- */
		else
		{
			vptr = wptr;
			 if ( *wptr == '&' )						
				*(wptr++) = 0;
		}

		/* ----------------- */
		/* allocate a header */
		/* ----------------- */
		if (!( hptr = rest_create_header( nptr, vptr ) ))
			break;
		else if (!( hptr->previous = foot ))
			root = hptr;
		else	hptr->previous->next = hptr;
		foot = hptr;
	}
	liberate( sptr );
	return( root );
}

/*	---------------------------------------------------------	*/
/*		o c c i _ e x t r a c t _ a c t i o n			*/
/*	---------------------------------------------------------	*/
private	char *	occi_extract_action( struct rest_header ** hhptr )
{
	struct	rest_header * hptr;
	char *	result=(char *) 0;
	if (!( hptr = *hhptr ))
		return( (char *) 0);

	while ( hptr )
	{
		if (( hptr->name )
		&&  (!( strcmp( hptr->name, "action" ) )))
		{
			/* -------------------------- */
			/* duplicate the header value */
			/* -------------------------- */
			if (!( result = allocate_string( hptr->value ) ))
				break;

			/* -------------------------------------- */
			/* remove the action value from the chain */
			/* -------------------------------------- */
			else if (!( hptr->previous ))
			{
				*hhptr = hptr->next;
				hptr->next->previous = (struct rest_header * )0;
				break;
			}
			else
			{
			 	hptr->previous->next = hptr->next;
				if ( hptr->next )
					hptr->next->previous = hptr->previous;
				hptr->next = hptr->previous = (struct rest_header * ) 0;
				liberate_rest_header( hptr );
				break;
			}
		}
		else	hptr = hptr->next;
	}
	return( result );
}

/*	---------------------------------------------------------	*/
/*		   o c c i _ p s e u d o _ a c t i o n			*/
/*	---------------------------------------------------------	*/
/*	this function performs invocation of POST for a category	*/
/*	---------------------------------------------------------	*/
private	struct rest_response * occi_pseudo_action(
	struct occi_category * optr,
	struct rest_client   * cptr, 
	struct rest_request  * rptr )
{
	struct	rest_header * hptr;
	char *	action=(char *) 0;
	if (!( action ))
		return((struct rest_response *) 0);
	else if (!( rptr ))
		return((struct rest_response *) 0);
	else if (!( rptr->parameters ))
		return((struct rest_response *) 0);
	else if (!( hptr = occi_extract_parameters( rptr->parameters ) ))
		return((struct rest_response *) 0);
	else if (!(action = occi_extract_action( &hptr ) ))
	{
		hptr = liberate_rest_headers( hptr );
		return((struct rest_response *) 0);
	}
	else if ((!( strcmp( action, "POST"   ) ))
	     ||  (!( strcmp( action, "GET"    ) ))
	     ||  (!( strcmp( action, "DELETE" ) ))
	     ||  (!( strcmp( action, "PUT"    ) )))
		return( occi_invoke_pseudo( action, hptr, optr, cptr, rptr ) );
	else
	{
		hptr = liberate_rest_headers( hptr );
		return((struct rest_response *) 0);
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
	struct	rest_response * aptr=(struct rest_response *) 0;
	struct	rest_interface * iptr;
	if (( aptr = occi_pseudo_action( optr, cptr, rptr )) != (struct rest_response *) 0)
		return( aptr );
	else if (!( iptr = optr->interface ))
		return( occi_failure(cptr,  400, "Bad Request : No Methods" ) );
	else if (!( rptr = occi_request_body(optr, rptr) ))
		return( occi_failure(cptr,  400, "Bad Request : Incorrect Body" ) );
	else
	{
		if ( iptr->before ) 	(*iptr->before)(optr,cptr,rptr);
		if ( iptr->post )	aptr = (*iptr->post)(optr,cptr,rptr);
		else			aptr = occi_failure(cptr,  405, "Bad Request : No Category Method" );
		if ( iptr->after )	(*iptr->after)(optr,cptr,rptr);
		return( occi_content_type( optr,rptr, aptr ) );
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
	else if (!( rptr = occi_request_body(optr, rptr) ))
		return( occi_failure(cptr,  400, "Bad Request : Incorrect Body" ) );
	else
	{	
		if ( iptr->before ) 	(*iptr->before)(optr,cptr,rptr);
		if ( iptr->put )	aptr = (*iptr->put)(optr,cptr,rptr);
		else			aptr = occi_failure(cptr,  405, "Bad Request : No Category Method" );
		if ( iptr->after )	(*iptr->after)(optr,cptr,rptr);
		return( occi_content_type( optr, rptr, aptr ) );
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
		return( occi_content_type( optr, rptr, aptr ) );
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
		return( occi_content_type( optr, rptr, aptr ) );
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
	if ( optr->access & _OCCI_NO_PRICING )
		return( aptr );
	else if (!( iptr = optr->interface ))
		return( aptr );
	else if (!( iptr->transaction ))
		return( aptr );
	else 	return( (*iptr->transaction) ( optr, cptr, rptr, aptr ) );
}

/*	----------------------------------------------------------------	*/
/*		o c c i _ a u t h o r i z a t i o n _ f a i l u r e		*/
/*	----------------------------------------------------------------	*/
private	struct rest_response * occi_authorization_failure( 
		struct rest_client * cptr, 
		struct rest_request * rptr )
{
	struct	rest_header * hptr;
	if (!( hptr = rest_resolve_header( rptr->first, _HTTP_ACCEPT ) ))
		return( occi_failure(cptr,  403, "Bad Request : Forbidden" ) );
	else if (!( hptr->value ))
		return( occi_failure(cptr,  403, "Bad Request : Forbidden" ) );
	else if (!( accept_string_includes( hptr->value, _OCCI_TEXT_HTML ) ))
		return( occi_failure(cptr,  403, "Bad Request : Forbidden" ) );
	else	return( occi_failure(cptr,  401, "Authorization required" ) );
}

/*	----------------------------------------------------------------	*/
/*	   a c c o r d s _ l o c a t e _ a u t h o r i z a t i o n 		*/
/*	----------------------------------------------------------------	*/
private	struct accords_authorization * accords_locate_authorization( char * username, char * password )
{
	struct	accords_authorization * aptr;
	for (	aptr=AccordsAuthorization;
		aptr != (struct accords_authorization *) 0;
		aptr = aptr->next )
	{
		if ( strcmp( aptr->user, username ) != 0)
			continue;
		else if ( strcmp( aptr->pass, password ) != 0)
			continue;
		else	break;
	}
	return( aptr );
}

/*	----------------------------------------------------------------	*/
/*	   a c c o r d s _ r e m o v e _ a u t h o r i z a t i o n 		*/
/*	----------------------------------------------------------------	*/
private	struct accords_authorization * accords_remove_authorization( struct accords_authorization * aptr )
{
	if ( aptr )
	{
		if ( aptr->user )
			aptr->user = liberate( aptr->user );
		if ( aptr->pass )
			aptr->pass = liberate( aptr->pass );
		if ( aptr->authorization )
			aptr->authorization = liberate( aptr->authorization );
		aptr = liberate( aptr );
	}
	return((struct accords_authorization *) 0);
}

/*	----------------------------------------------------------------	*/
/*		a c c o r d s _ d r o p _ a u t h o r i z a t i o n 		*/
/*	----------------------------------------------------------------	*/
private	struct accords_authorization * accords_drop_authorization( struct accords_authorization * aptr )
{
	if (!( aptr->previous ))
	{
		if ((AccordsAuthorization = aptr->next) != (struct accords_authorization *) 0)
			AccordsAuthorization->previous = (struct accords_authorization *) 0;
	}
	else if (( aptr->previous->next = aptr->next ) != (struct accords_authorization *) 0)
		aptr->next->previous = aptr->previous;

	return( accords_remove_authorization( aptr ) );
}

/*	----------------------------------------------------------------	*/
/*	   a c c o r d s _ c r e a t e _ a u t h o r i z a t i o n 		*/
/*	----------------------------------------------------------------	*/
private	struct accords_authorization * accords_create_authorization( char * username, char * password, char * authorization )
{
	struct	accords_authorization * aptr;
	if (!( aptr = (struct accords_authorization *) allocate( sizeof( struct accords_authorization ) ) ))
		return( aptr );
	else	memset( aptr, 0, sizeof( struct accords_authorization ) );
	if (!( aptr->user = allocate_string( username ) ))
		return( accords_remove_authorization( aptr ) );
	else if (!( aptr->pass = allocate_string( password ) ))
		return( accords_remove_authorization( aptr ) );
	else if (!( aptr->authorization = allocate_string( authorization ) ))
		return( accords_remove_authorization( aptr ) );
	else
	{
		if ((aptr->next = AccordsAuthorization) != (struct accords_authorization *) 0)
			aptr->next->previous = aptr;
		return ((AccordsAuthorization = aptr));
	}
}

/*	----------------------------------------------------------------	*/
/*	   a c c o r d s _ r e s o l v e _ a u t h o r i z a t i o n		*/
/*	----------------------------------------------------------------	*/
private	int	accords_resolve_authorization( char * sptr, char * agent, char * tls )
{
	struct	accords_authorization * aptr;
	char 	username[1024];
	char 	password[1024];
	char *	authorization;
	if (!( rest_decode_credentials( sptr, username, password ) ))
		return( 0 );
	else if ((aptr = accords_locate_authorization( username, password )) != (struct accords_authorization *) 0)
		return( 1 );
	else if (!( authorization = login_occi_user( username, password, agent, tls )))
		return( 0 );
	else if (!(aptr = accords_create_authorization( username, password, authorization ) ))
		return( 0 );
	else	return( 1 );
}

/*	----------------------------------------------------------------	*/
/*	o c c i _ c h e c k _ r e q u e s t _ a u t h o r i z a t i o n		*/
/*	----------------------------------------------------------------	*/
/*	Here we check the request authorization header value in respect		*/
/*	with the current authorization status of the server connection.		*/
/*	If the server provides, and consequently requires authorization		*/
/*	then this function will fail if authorization is not provided.		*/
/*	otherwise the function will only fail if the provided token is		*/
/*	not a valid authorization credential. Authorization is always		*/
/*	required when security has been activated, otherwise it is not		*/
/*	expected to be provided. The provision of authorization when it		*/
/*	is not needed is not aproblem, but, provision of an illicite		*/
/*	authorization credential is indicative of something doubtful.		*/
/*	----------------------------------------------------------------	*/
private	int	occi_check_request_authorization( 
		struct rest_request * rptr,
		struct occi_category * optr )
{
	struct	rest_header * hptr;

	/* ----------------------------------------------- */
	/* test first for OCCI Authorization token present */
	/* ----------------------------------------------- */
	if (( hptr = rest_resolve_header( rptr->first, _OCCI_AUTHORIZE )) != (struct rest_header *) 0)
	{
		if (!( hptr->value ))
			return(( occi_authorization ? ( optr->access & _OCCI_NO_AUTHORIZE ? 1 : 0) : 1 ));
		else 	return( occi_resolve_authorization( hptr->value ) );
	}
	/* ------------------------------------- */
	/* test for an HTTP AUTHORIZATION header */
	/* ------------------------------------- */
	else if (!( hptr = rest_resolve_header( rptr->first, _HTTP_AUTHORIZATION ) ))
		return(( occi_authorization ? ( optr->access & _OCCI_NO_AUTHORIZE ? 1 : 0) : 1 ));
	else if (!( hptr->value ))
		return(( occi_authorization ? ( optr->access & _OCCI_NO_AUTHORIZE ? 1 : 0) : 1 ));
	else 	return( accords_resolve_authorization( hptr->value, _CORDS_CONTRACT_AGENT, default_tls()  ) );
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
	else if (!( occi_check_request_authorization( rptr, optr ) ))
		return( occi_authorization_failure( cptr, rptr ) );
	else if (!( strcmp( rptr->object, "/-/" ) ))
		return( occi_get_capacities( optr, cptr, rptr ) );
	else if (!( strcmp( rptr->object, "/" ) ))
		return( occi_redirect(cptr,  301, "Redirected to Capacities", "/-/" ) );
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
	else if (!( occi_check_request_authorization( rptr, optr ) ))	
		return( occi_authorization_failure( cptr, rptr ) );
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
	else if (!( occi_check_request_authorization( rptr, optr ) ))	
		return( occi_authorization_failure( cptr, rptr ) );
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
	else if (!( occi_check_request_authorization( rptr, optr ) ))	
		return( occi_authorization_failure( cptr, rptr ) );
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
	else if (!( occi_check_request_authorization( rptr, optr ) ))	
		return( occi_authorization_failure( cptr, rptr ) );
	else if (!( strcmp( rptr->object, "/-/" ) ))
		return( occi_get_capacities( optr, cptr, rptr ) );
	else if (!( strcmp( rptr->object, "/" ) ))
		return( occi_redirect(cptr,  301, "Redirected to Capacities", "/-/" ) );
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
	char	*	full_attribute;
	char	*	attribute_value ;
	char 	*	start ;
	struct	rest_header * hptr;

	for ( 	hptr = rest_resolve_header( rptr->first, _OCCI_ATTRIBUTE );
		hptr != (struct rest_header *) 0;
		hptr = hptr->next )
	{
		if ( strcasecmp(hptr->name, _OCCI_ATTRIBUTE) != 0)
			continue;
		else if (!( hptr->value ))
			continue;
		else if (!( full_attribute = allocate_string( hptr->value ) ))
			return(0);
		else
		{
			for ( attribute_value = full_attribute; *attribute_value  == ' '; attribute_value ++);
			for ( start = attribute_value ; *start  != 0; start ++ )
			{
				if ( *start  == '=' )
				{
					*(start ++) = 0;
					break;
				}
			}	
			for ( ; *start  == ' '; start ++);
			if ((!( start  = allocate_string( start  ) ))
			||  (!( start  = occi_unquoted_value( start  ) )))
				return(0);
			else
			{
				(*method)(optr,pptr, attribute_value , start );
				liberate( full_attribute );
				liberate( start  );
			}
		}
	}
	return(1);
}

/*	-------------------------------------------------------		*/
/*		    	o c c i _ a l e r t				*/
/*	-------------------------------------------------------		*/
public	struct rest_response * occi_alert(
		void * vptr,
		struct rest_client * cptr, 
		struct rest_response * rptr,
		char * name,
		int status, 
		char * message, char * nature, 
		char * agent, char * tls )
{
	struct	occi_category 	* optr;

	char	ecode[32];
	char	etime[32];
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

	if (name == NULL)
	{
		name = "";
	}

	if (!( optr = vptr ))
		return(rptr);
	else
	{
		sprintf(ecode,"%u",status);
		sprintf(etime,"%u",time((long *) 0));
	}

	if (!( ihost = occi_resolve_category_provider( _CORDS_ALERT, agent, tls ) ))
		return(rptr);
	else
	{
		sprintf(buffer,"%s/%s/",ihost,_CORDS_ALERT);
		liberate( ihost );
	}

	if (!( kptr = occi_create_client( buffer, agent, tls ) ))
		return(rptr);
	else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
	{
		kptr = occi_remove_client( kptr );
		return(rptr);
	}

	else if ((!(dptr=occi_request_element(qptr,"occi.alert.source"      , agent   ) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.alert.name"        , name  ) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.alert.nature"      , nature  ) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.alert.status"      , ecode   ) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.alert.created"     , etime   ) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.alert.subject"     , message ) )))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return(rptr);
	}
	else if (!( yptr = occi_client_post( kptr, qptr ) ))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return(rptr);
	}
	else
	{
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return(rptr);
	}
}

/*	-------------------------------------------------------		*/
/*		    	o c c i _ a l e r t				*/
/*	-------------------------------------------------------		*/
private	struct rest_response * occi_alert_relay_impl(
		void * vptr,
		struct rest_client * cptr,
		struct rest_response * rptr,
		char * name,
		int status,
		char * message, char * nature,
		char * agent, char * tls )
{
	return occi_alert(vptr, cptr, rptr, name, status, message, nature, agent, tls);
}

private	struct occi_category * append_category_list( struct occi_category * lptr, struct occi_category * cptr )
{
	struct	occi_category * root;
	if (!( root = lptr ))
		return( cptr );
	else
	{
		while ( lptr->next )
			lptr = lptr->next;

		lptr->next = cptr;
		cptr->previous = lptr;
		return( root );
	}
}

private	struct occi_category * check_occi_conformity( struct occi_category * category )
{
	struct	occi_category * cptr;
	struct	occi_category * optr;
	int	core=0;
	for ( cptr=category;
		cptr != (struct occi_category *) 0;
		cptr = cptr->next )
	{
		if (!( strcmp( cptr->id, "entity" ) ))
			core |= 1;
		else if (!( strcmp( cptr->id, "resource" ) ))
			core |= 2;
		else if (!( strcmp( cptr->id, "link" ) ))
			core |= 4;
	}
	if (!( core & 1 ))
	{
		if (!( optr = occi_create_category(
			"occi",
			"entity",
			"http://schemas.ogf.org/core#",
			"kind",
			"http://scheme.ogf.org/occi/entity#",
			"standard OCCI entity" ) )) 
			return( category );
		if (!( optr = occi_add_attribute( optr, "id", 0, 0 ) ))
			return( category );
		else if (!( optr = occi_add_attribute( optr, "title", 0, 0 ) ))
			return( category );
		else
		{
			optr->access = _OCCI_PRIVATE;
			category = append_category_list( category, optr );
		}
	}
	if (!( core & 2 ))
	{
		if (!( optr = occi_create_category(
			"occi",
			"resource",
			"http://schemas.ogf.org/core#",
			"kind",
			"http://scheme.ogf.org/occi/resource#",
			"standard OCCI resource" ) )) 
			return( category );
		if (!( optr = occi_add_attribute( optr, "summary", 0, 0 ) ))
			return( category );
		else
		{
			optr->access = _OCCI_PRIVATE;
			category = append_category_list( category, optr );
		}
	}
	if (!( core & 4 ))
	{
		if (!( optr = occi_create_category(
			"occi",
			"link",
			"http://schemas.ogf.org/core#",
			"kind",
			"http://scheme.ogf.org/occi/link#",
			"standard OCCI link" ) )) 
			return( category );
		if (!( optr = occi_add_attribute( optr, "source", 0, 0 ) ))
			return( category );
		else if (!( optr = occi_add_attribute( optr, "target", 0, 0 ) ))
			return( category );
		else
		{
			optr->access = _OCCI_PRIVATE;
			category = append_category_list( category, optr );
		}
	}
	return( category );
}

/*	--------------------------------------------------	*/
/*		o c c i _ l o c a l _ s e r v e r		*/
/*	--------------------------------------------------	*/
/*	called back from the OCCI client when the identity	*/
/*	and target verification indicates local service is	*/
/*	useable for the request thus allowing optimisation	*/
/*	--------------------------------------------------	*/
public	struct	rest_response * occi_local_server( char * method, char * target, char * tls, char * agent, struct rest_header * hptr, char * body )
{
	struct 	rest_client * cptr=(struct rest_client *) 0;
	struct 	rest_request * rptr=(struct rest_request *) 0;
	struct	rest_response * aptr=(struct rest_response *) 0;
	struct	url *	uptr;
	/* --------------------------- */
	/* build a pseudo rest request */
	/* --------------------------- */
	rest_log_message("occi_local_server");
	rest_log_message( target );

	if (!( uptr = analyse_url( target ) ))
		return(occi_failure(cptr,930,"incorrect url"));

	else if (!( rptr = allocate_rest_request() ))
		return(occi_failure(cptr,927,"allocation failure"));

	else if (!( rptr->object = allocate_string( uptr->object ) ))
	{
		rptr = liberate_rest_request( rptr );
		return(occi_failure(cptr,927,"allocation failure"));
	}
	else if (!( rptr = rest_parse_object( (struct rest_client *) 0, rptr) ))
	{
		rptr = liberate_rest_request( rptr );
		return(occi_failure(cptr,927,"allocation failure"));
	}
	else if (!( rptr->host = allocate_string( uptr->host ) ))
	{
		rptr = liberate_rest_request( rptr );
		return(occi_failure(cptr,927,"allocation failure"));
	}
	else
	{
		rptr->port = uptr->port;
	}
	if (!( cptr = allocate_rest_client() ))
	{
		rptr = liberate_rest_request( rptr );
		return(occi_failure(cptr,927,"allocation failure"));
	}
	else if (!( cptr->buffer = allocate((cptr->buffersize = _MAX_RESTBUFFER)) ))
	{
		rptr = liberate_rest_request( rptr );
		return(occi_failure(cptr,927,"allocation failure"));
	}
	else if (!( rptr->method = allocate_string( method ) ))
	{
		rptr = liberate_rest_request( rptr );
		return(occi_failure(cptr,927,"allocation failure"));
	}
	else if (!( hptr = rest_duplicate_headers( hptr )))
	{
		rptr = liberate_rest_request( rptr );
		return(occi_failure(cptr,927,"allocation failure"));
	}
	else
	{
		/* ----------------------- */
		/* add the request headers */
		/* ----------------------- */

		if ((rptr->first = hptr) != (struct rest_header *) 0)
		{
			while ( hptr->next )
				hptr = hptr->next;
			rptr->last = hptr;
		}

		/* ---------------- */
		/* process the body */
		/* ---------------- */
		if (( body )
		&&  ((body = allocate_string(body)) != (char *) 0))
			rest_request_body( rptr, body, _FILE_BODY );
		
		/* ------------------- */
		/* process the request */
		/* ------------------- */
		if (!( strcasecmp( method, "GET" ) ))
			aptr = occi_get( OcciLocalCategory, cptr, rptr );
		else if (!( strcasecmp( method, "PUT" ) ))
			aptr = occi_put( OcciLocalCategory, cptr, rptr );
		else if (!( strcasecmp( method, "POST" ) ))
			aptr = occi_post( OcciLocalCategory, cptr, rptr );
		else if (!( strcasecmp( method, "DELETE" ) ))
			aptr = occi_delete( OcciLocalCategory, cptr, rptr );
		else if (!( strcasecmp( method, "HEAD" ) ))
			aptr = occi_head( OcciLocalCategory, cptr, rptr );

		/* ----------------------- */
		/* release the request now */
		/* ----------------------- */
		rptr = liberate_rest_request( rptr );
		cptr = liberate_rest_client( cptr );
		return( aptr );
	}
}

/*	---------------------------------------------------------	*/
/*			o c c i _ s e r v e r				*/
/*	---------------------------------------------------------	*/
/*	this function is to be called to instance an OCCI server	*/
/*	operating over an underlying  REST server to provide the	*/
/*	processing functions needed for each HTTP method.		*/
/*	---------------------------------------------------------	*/
public	int	occi_server( char * nptr, int port, char * tls, int max, 
		struct occi_category * category, char * authorization )
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
		(void *) 0,
		occi_alert_relay
	};

	set_default_agent( nptr );

	if (!( rest_valid_string( tls ) ))
		tls = (char *) 0;

	/* -------------------------------------------------- */
	/* ensure that the basic core information is provided */
	/* -------------------------------------------------- */
	if (!( category = check_occi_conformity( category ) ))
 		return( 55 );

	occi_authorization = authorization;
	Osi.authorise = (void *) 0;
	Osi.instance  = category;
	OcciLocalCategory = category;

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

