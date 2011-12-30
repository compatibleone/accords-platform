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
#ifndef	_occipublisher_c
#define	_occipublisher_c

#include "occipublisher.h"
#include "occipub.c"
#include "autopub.c"
#include "document.h"
#include "cp.h"
#include "occilogin.h"
#include "tlsload.h"

private	struct	occi_publisher Publisher = {
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(struct occi_publication *) 0,
	(struct occi_publication *) 0
	};

/*	---------------------------------------------------------	*/
/*		d e f a u l t _ o p e r a t o r				*/
/*	---------------------------------------------------------	*/
/*	this is a temporary measure and will be provided by the		*/
/*	configuration of a service provider module.			*/
/*	---------------------------------------------------------	*/
private	char *	default_operator()
{
	return("accords");
}

/*	---------------------------------------------------------	*/
/*	     o c c i _ l o c a t e _ p u b l i c a t i o n		*/
/*	---------------------------------------------------------	*/
/*	this function attempts to locate a publication record for	*/
/*	use by the unpublish operation. These records are created	*/
/*	during the publication of categories phase.			*/
/*	---------------------------------------------------------	*/
private	struct	occi_publication * occi_locate_publication( struct occi_category * cptr )
{
	struct	occi_publication * pptr;
	for (	pptr=Publisher.first;
		pptr != (struct occi_publication *) 0;
		pptr = pptr->next)
	{
		if (!( pptr->category ))
			continue;
		else if (!( pptr->id ))
			continue;
		else if (!( strcmp( cptr->id, pptr->category ) ))
			break;
	}
	return( pptr );
}

/*	---------------------------------------------------------	*/
/*	   i n i t i a l i s e _ o c c i _ p u b l i s h e r		*/
/*	---------------------------------------------------------	*/
public	int	initialise_occi_publisher( char * host, char * room, char * publication, char * enquiry )
{
	char	*	sptr;
	if ( host )
	{
		if (!( sptr = allocate_string( host ) ))
			return( 27 );
		if ( Publisher.host )
			Publisher.host = liberate( Publisher.host );
		Publisher.host = sptr;
	}
	if ( room )
	{
		if (!( sptr = allocate_string( room ) ))
			return( 27 );
		if ( Publisher.room )
			Publisher.room = liberate( Publisher.room );
		Publisher.room = sptr;
	}
	if ( publication )
	{
		if (!( sptr = allocate_string( publication ) ))
			return( 27 );
		if ( Publisher.publication )
			Publisher.publication = liberate( Publisher.publication );
		Publisher.publication = sptr;
	}
	if ( enquiry )
	{
		if (!( sptr = allocate_string( enquiry ) ))
			return( 27 );
		if ( Publisher.enquiry )
			Publisher.enquiry = liberate( Publisher.enquiry );
		Publisher.enquiry = sptr;
	}
	return( 0 );
}

/*	---------------------------------------------------------	*/
/*	     t e r m i n a t e _ o c c i _ p u b l i s h e r		*/
/*	---------------------------------------------------------	*/
public	int	terminate_occi_publisher()
{
	char	*	sptr;
	if ( Publisher.host )
		Publisher.host = liberate( Publisher.host );
	if ( Publisher.room )
		Publisher.room = liberate( Publisher.room );
	if ( Publisher.publication )
		Publisher.publication = liberate( Publisher.publication );
	if ( Publisher.enquiry )
		Publisher.enquiry = liberate( Publisher.enquiry );
	if ( Publisher.uri )
		Publisher.uri = liberate( Publisher.uri );
	return(0);
}

/*	---------------------------------------------------------	*/
/*		    p u b l i s h e r _ d e f a u l t 			*/
/*	---------------------------------------------------------	*/
public	int	occi_publisher_default()
{
	if (!( Publisher.host ))
		Publisher.host = allocate_string( _DEFAULT_HOST );

	if (!( Publisher.tls ))
		if ( _DEFAULT_TLS )
			Publisher.tls = allocate_string( _DEFAULT_TLS );

	if (!( Publisher.room ))
		Publisher.room = allocate_string( _DEFAULT_ROOM );

	if (!( Publisher.publication ))
		Publisher.publication = allocate_string( _DEFAULT_PUBLICATION );

	if (!( Publisher.enquiry ))
		Publisher.enquiry = allocate_string( _DEFAULT_ENQUIRY );

	if (!( Publisher.uri ))
	{
		if (!( Publisher.uri = allocate(strlen(Publisher.host)+strlen(Publisher.publication)+16) ))
			return(27);
		else	sprintf(Publisher.uri,"%s/%s/",Publisher.host,Publisher.publication);
	}
	return(0);
}

/*	---------------------------------------------------------	*/
/*	      u n p u b l i s h _ o c c i _ c a t e g o r y 		*/
/*	---------------------------------------------------------	*/
public	int	unpublish_occi_category( 
		int item,
		char * user,	char * password,
		char * url,	char * agent, 
		struct occi_category * category	 )
{
	struct	occi_client   *	cptr;
	struct	occi_request  *	rptr;
	struct	occi_response * aptr;
	struct	occi_publication * pptr;
	char *	uri;
	char	username[256];
	sprintf(username,"%s%u",user,item);

	if (!( pptr = occi_locate_publication( category )))
		return( 78 );
	else if (!( uri = allocate( strlen( Publisher.host ) + strlen( Publisher.publication ) + strlen( pptr->id ) + 16 ) ))
		return( 27 );
	else	sprintf(uri,"%s/%s/%s",Publisher.host,Publisher.publication,pptr->id);

	if (!( cptr = occi_create_client( uri, agent, Publisher.tls ) ))
	{
		liberate( uri );
		return( 27 );
	}

	else if (!(rptr = occi_create_request( cptr, Publisher.publication, _OCCI_NORMAL )))
	{
		liberate( uri );
		cptr = occi_remove_client( cptr );
		return( 27 );
	}
	else if (!( aptr = occi_client_delete( cptr, rptr ) )) 
	{
		liberate( uri );
		rptr = occi_remove_request( rptr );
		cptr = occi_remove_client( cptr );
		return( 27 );
	}
	else
	{
		pptr = drop_occi_publication( pptr );
		aptr = occi_remove_response( aptr );
		rptr = occi_remove_request( rptr );
		cptr = occi_remove_client( cptr );
		liberate( uri );
		return(0);
	}
}

/*	---------------------------------------------------------	*/
/*	    	n u l l _ o c c i _ t r a n s a c t i o n		*/
/*	---------------------------------------------------------	*/
private	int	null_occi_transaction(
		struct	occi_category * optr,
		struct	occi_client * cptr,
		struct  occi_request * rptr,
		struct 	occi_reponse * aptr
		)
{
	return( aptr );
}

/*	---------------------------------------------------------	*/
/*	    p r o c e s s _ o c c i _ t r a n s a c t i o n		*/
/*	---------------------------------------------------------	*/
/*	this relay interface method is called back after the post	*/
/*	method invocation allowing for priced transaction events	*/
/*	to be generated for priced categories.				*/
/*	---------------------------------------------------------	*/
private	int	process_occi_transaction(
		struct	occi_category * optr,
		struct	occi_client * cptr,
		struct  occi_request * rptr,
		struct 	occi_reponse * aptr
		)
{
	/* ---------------------------------------------------- */
	/* TODO							*/
	/* ---------------------------------------------------- */
	/* 							*/
	/* this method needs to be implemented and must: 	*/
	/* 							*/
	/* resolve the price information from the category	*/
	/* resolve the account information from the request	*/
	/* the method from the request				*/
	/* 							*/
	/* resolve a transaction service provider via publisher	*/
	/* send a transaction message to provider		*/
	/* 							*/
	/* ---------------------------------------------------- */
	/* currently it only performs a NULL transaction	*/
	/* ---------------------------------------------------- */
	return( null_occi_transaction( optr, cptr, rptr, aptr ) );
}

/*	---------------------------------------------------------	*/
/*	      p u b l i s h _ o c c i _ c a t e g o r y 		*/
/*	---------------------------------------------------------	*/
public	int	publish_occi_category( 
		int item,
		char * user,	char * password,
		char * url,	char * agent, 
		struct occi_category * category )
{
	struct	occi_element  *	eptr;
	struct	occi_client   *	cptr;
	struct	occi_request  *	rptr;
	struct	occi_response * aptr;
	struct	occi_publication * pptr;
	char 	*	uri;
	char	username[256];
	
	/* ------------------------------------------------------------------------ */
	/* resolve an eventual price for the provision of instances of the category */
	/* ------------------------------------------------------------------------ */
	if (!( category->price ))
	{
		if ( category->access & _OCCI_PRICING )
		{
			if (!( category->price = allocate_string("") ))
				return(27);
		}
		else if (!( category->price = occi_resolve_category_price( category->id, url, agent, Publisher.tls ) ))
		{	
			if (!( category->price = allocate_string("") ))
				return(27);
		}
		else	category->interface.transaction = process_occi_transaction;
	}
	else	category->interface.transaction = null_transaction;

	/* ------------------------------------- */
	/* generate a user identification string */
	/* ------------------------------------- */
	sprintf(username,"%s%u",user,item);

	/* --------------------------------------- */
	/* create the provider uri for publication */
	/* --------------------------------------- */
	if (!( uri = allocate( strlen( Publisher.host ) + strlen( Publisher.publication ) + 16 ) ))
		return( 27 );
	else	sprintf(uri,"%s/%s/",Publisher.host,Publisher.publication);

	/* -------------------------------------------------------------- */
	/* create an OCCI client request with the appropriate information */
	/* -------------------------------------------------------------- */
	if (!( cptr = occi_create_client( uri, agent, Publisher.tls ) ))
	{
		liberate( uri );	
		return( 27 );
	}
	else if (!(rptr = occi_create_request( cptr, Publisher.publication, _OCCI_NORMAL )))
	{
		liberate( uri );	
		cptr = occi_remove_client( cptr );
		return( 27 );
	}
	else if ((!( eptr = occi_request_element( rptr, "occi.publication.who",username		)))
	     ||  (!( eptr = occi_request_element( rptr, "occi.publication.pass",password 	)))
	     ||  (!( eptr = occi_request_element( rptr, "occi.publication.where",Publisher.room	)))
	     ||  (!( eptr = occi_request_element( rptr, "occi.publication.what",category->id 	)))
	     ||  (!( eptr = occi_request_element( rptr, "occi.publication.operator",default_operator() )))
	     ||  (!( eptr = occi_request_element( rptr, "occi.publication.price",category->price)))
	     ||  (!( eptr = occi_request_element( rptr, "occi.publication.why",url 		))))
	{
		liberate( uri );	
		rptr = occi_remove_request( rptr );
		cptr = occi_remove_client( cptr );
		return( 27 );
	}
	/* ----------------------------------------------------------- */
	/* issue the publication POST request to the Accords Publisher */
	/* ----------------------------------------------------------- */
	else if (!( aptr = occi_client_post( cptr, rptr ) )) 
	{
		liberate( uri );	
		rptr = occi_remove_request( rptr );
		cptr = occi_remove_client( cptr );
		return( 27 );
	}
	else if ( aptr->response->status >= 300 )
	{
		if ( check_verbose() )
			printf("   OCCI Client : %s %u %s \n",
				aptr->response->version,
				aptr->response->status,
				aptr->response->message );
		liberate( uri );	
		aptr = occi_remove_response( aptr );
		rptr = occi_remove_request( rptr );
		cptr = occi_remove_client( cptr );
		return( 27 );
	}

	/* -------------------------------------------------------------- */
	/* store the publication record for removal at shutdown of server */
	/* -------------------------------------------------------------- */
	else if ((!( pptr = add_occi_publication( &Publisher ) ))
	     ||  (!( pptr->category = allocate_string( category->id ) )))
	{
		liberate( uri );	
		pptr = drop_occi_publication( pptr );
		aptr = occi_remove_response( aptr );
		rptr = occi_remove_request( rptr );
		cptr = occi_remove_client( cptr );
		return(27);
	}
	else if (!( eptr = occi_locate_element( aptr->first, Publisher.publication ) ))
	{
		liberate( uri );	
		pptr = drop_occi_publication( pptr );
		aptr = occi_remove_response( aptr );
		rptr = occi_remove_request( rptr );
		cptr = occi_remove_client( cptr );
		return(40);
	}
	else if (!( pptr->id = occi_category_id( eptr->value ) ))
	{
		liberate( uri );	
		pptr = drop_occi_publication( pptr );
		aptr = occi_remove_response( aptr );
		rptr = occi_remove_request( rptr );
		cptr = occi_remove_client( cptr );
		return(27);
	}
	else
	{
		liberate( uri );	
		aptr = occi_remove_response( aptr );
		rptr = occi_remove_request( rptr );
		cptr = occi_remove_client( cptr );
		return(0);
	}	
}

/*	---------------------------------------------------------	*/
/*		o c c i _ a u t o _ p u b l i s h			*/
/*	---------------------------------------------------------	*/
private	int 	occi_auto_publish( 
		void * vptr,
		struct rest_client * cptr, 
		struct rest_request * rptr )
{
	struct	occi_category * optr;
	struct	rest_interface * iptr;
	struct	auto_publication * pptr;
	int	status;
	if (!( optr = vptr ))
	  return(118);
	else if (!( pptr = optr->payload ))
	  return(118);
	else 	return( publish_occi_category( 
				pptr->item, pptr->user, pptr->password, 
				pptr->url, pptr->agent, optr ));
}

/*	---------------------------------------------------------	*/
/*		o c c i _ a u t o _ u n p u b l i s h			*/
/*	---------------------------------------------------------	*/
private	int 	occi_auto_unpublish( 
		void * vptr,
		struct rest_client * cptr, 
		struct rest_request * rptr )
{
	struct	occi_category * optr;
	struct	rest_interface * iptr;
	struct	auto_publication * pptr;
	int	status;
	if (!( optr = vptr ))
	  return(118);
	else if (!( pptr = optr->payload ))
	  return(118);
	else 	return( unpublish_occi_category( 
				pptr->item, pptr->user, pptr->password, 
				pptr->url, pptr->agent, optr ));
}

/*	---------------------------------------------------------	*/
/*	      p u b l i s h _ o c c i _ c a t e g o r i e s		*/
/*	---------------------------------------------------------	*/
public	int	occi_auto_publication( 
		int	item,
		char * user,	char * password,
		char * url,	char * agent, 
		struct occi_category * optr )
{
	struct	rest_interface * iptr;
	struct	auto_publication * pptr;
	if (!( iptr = optr->interface ))
		return(56);
	else if (!( pptr = allocate_auto_publication()))
		return(27);
	else if (!( pptr->user = allocate_string( user )))
	{
		pptr = liberate_auto_publication( pptr );
		return( 27 );
	}
	else if (!( pptr->password = allocate_string( password )))
	{
		pptr = liberate_auto_publication( pptr );
		return( 27 );
	}
	else if (!( pptr->url = allocate_string( url )))
	{
		pptr = liberate_auto_publication( pptr );
		return( 27 );
	}
	else if (!( pptr->agent = allocate_string( agent )))
	{
		pptr = liberate_auto_publication( pptr );
		return( 27 );
	}
	else
	{
		pptr->item = item;
		optr->payload= pptr;
		iptr->before = occi_auto_unpublish;
		iptr->after  = occi_auto_publish;
		return( 0 );
	}
}

/*	---------------------------------------------------------	*/
/*	      p u b l i s h _ o c c i _ c a t e g o r i e s		*/
/*	---------------------------------------------------------	*/
public	int	publish_occi_categories( 
		char * user,	char * password,
		char * url,	char * agent, 
		struct occi_category * category )
{
	int	items=0;
	struct	occi_category *	optr;
	int	status=0;
	for (	optr=category;
		optr != (struct occi_category *) 0;
		optr = optr->next )
	{
		if ( optr->access & _OCCI_PRIVATE )
			continue;
		else if ((status = publish_occi_category( ++items, user, password, url, agent, optr )) != 0)
			break;
		else if (!( optr->access & _OCCI_AUTO_PUBLISH ))
			continue;
		else if (!(status = occi_auto_publication( items, user, password, url, agent, optr )))
			continue;
		else	break;
	}
	return( status );
}

/*	---------------------------------------------------------	*/
/*	    u n p u b l i s h _ o c c i _ c a t e g o r i e s		*/
/*	---------------------------------------------------------	*/
public	int	unpublish_occi_categories( 
		char * user,	char * password,
		char * url,	char * agent, 
		struct occi_category * category )
{
	int	items=0;
	struct	occi_category *	optr;
	int	status=0;
	for (	optr=category;
		optr != (struct occi_category *) 0;
		optr = optr->next )
	{
		if ( optr->access & _OCCI_PRIVATE )
			continue;
		else if ((status = unpublish_occi_category( ++items, user, password,url,agent,optr)) != 0)
			break;
	}
	return( status );
}

/*	---------------------------------------------------------	*/
/*		p u b l i s h i n g _ o c c i _ s e r v e r		*/
/*	---------------------------------------------------------	*/
/*	this encapsulation function provides the compatible one 	*/
/*	occi server with automatic publishing of all categories		*/
/*	prior to server startup. 					*/
/*	The publications will be cancelled on return from the 		*/
/*	occi server prior to exit from the function.			*/
/*	---------------------------------------------------------	*/
public	int	publishing_occi_server( 
		char * user,	char * password,
		char * url,	char * agent, 
		int port, char * tls, int max, struct occi_category * category )
{
	int	status;
	int	result;
	struct	tls_configuration * tlsconf=(struct tls_configuration *) 0;

	/* --------------------------------------------- */
	/* initialise the resolver and publisher default */
	/* --------------------------------------------- */
	initialise_occi_resolver( _DEFAULT_PUBLISHER, (char *) 0, (char *) 0, (char *) 0 );

	if ((status = occi_publisher_default()) != 0 )
		return( status );

	/* -------------------------------------------- */
	/* handle transport layer security, if required */
	/* -------------------------------------------- */
	if ( tls )
	{
		if (!( strlen(tls) ))
			tls = (char *) 0;
		else if (!( tlsconf = tls_configuration_load( tls ) ))
			return( 40 );
		else if ( tlsconf->authenticate )
		{
			if ((user) && (password))
			{
				if (!(Publisher.authorization = login_occi_user( user, password, agent, tls )))
					return( 403 );
				else 	(void) occi_client_authentication( Publisher.authorization );
			}
		}
	}

	/* ----------------------------------------- */
	/* publish the collection of OCCI categories */
	/* ----------------------------------------- */
	if (( Publisher.host ) && ( Publisher.publication ))
		if (( url ) && ( category ))
			if ((status = publish_occi_categories( user, password, url, agent, category )) != 0)
				return( status );

	/* ---------------------------------------------------------- */
	/* activate the OCCI server layer for the required categories */
	/* ---------------------------------------------------------- */
	result = occi_server( agent, port, tls, max, category, Publisher.authorization );

	/* --------------------------------------------------------- */
	/* withdraw publication of the collection of OCCI categories */
	/* --------------------------------------------------------- */
	if (( Publisher.host ) && ( Publisher.publication ))
		if (( url ) && ( category ))
			if ((status = unpublish_occi_categories(user, password, url, agent, category)) != 0)
				return( status );

	/* -------------------------------------------- */
	/* terminate security session, if one is active */
	/* -------------------------------------------- */
	if (( tls ) && ( tlsconf ))
	{
		if ( Publisher.authorization )
			Publisher.authorization = logout_occi_user( 
				user, password, agent, Publisher.authorization, tls );
		if ( tlsconf )
			tlsconf = release_tls_configuration(tlsconf );
	}
	return( result );
}

	/* ---------------- */
#endif	/* _occipublisher_c */
	/* ---------------- */



