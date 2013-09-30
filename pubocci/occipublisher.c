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
#ifndef	_occipublisher_c
#define	_occipublisher_c

#include "occipublisher.h"
#include "occipub.c"
#include "autopub.c"
#include "document.h"
#include "cp.h"
#include "cb.h"
#include "occilogin.h"
#include "tlsload.h"

private	struct	occi_publisher Publisher = {
	0,
	(char *) 0,
	(char *) 0,
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
/*		d e f a u l t _ c o n n e c t i o n			*/
/*	---------------------------------------------------------	*/
/*	this is a temporary measure and will be provided by the		*/
/*	configuration of a service provider module.			*/
/*	---------------------------------------------------------	*/
private	char *	default_connection()
{
	return("normal");
}

/*	---------------------------------------------------------	*/
/*	c o r d s _ c o n s u m e r _ p u b l i c a t i o n		*/
/*	---------------------------------------------------------	*/
private	char * 	cords_consumer_publication( 
		char * name,
		char * price,
		char * nature,
		char * operator,
		char * identity, 
		char * agent, char * tls )
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
	char	buffer[2048];

	if (!( ihost = occi_resolve_category_provider( _CORDS_CONSUMER, agent, tls ) ))
		return((char *) 0);
	else
	{
		sprintf(buffer,"%s/%s/",ihost,_CORDS_CONSUMER);
		liberate( ihost );
	}

	if (!( kptr = occi_create_client( buffer, agent, tls ) ))
		return((char *) 0);
	else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
	{
		kptr = occi_remove_client( kptr );
		return((char *) 0);
	}
	else if ((!(dptr=occi_request_element(qptr,"occi.consumer.name"  	, ( name ? name : name )       ) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.consumer.price" 	, ( price ? price : "" )       ) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.consumer.nature" 	, ( nature ? nature : "" )     ) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.consumer.operator" 	, ( operator ? operator : "" ) ) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.consumer.identity"	, ( identity ? identity : "" ) ) )))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return((char *) 0);
	}
	else if (!( yptr = occi_client_post( kptr, qptr ) ))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return((char *) 0);
	}
	else if (!( ihost = occi_extract_location( yptr ) ))
	{
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return((char *) 0);
	}
	else
	{
		rest_add_http_prefix(buffer,2048,ihost);
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return( allocate_string( buffer ) );
	}

}

/*	---------------------------------------------------------	*/
/*		o c c i _ r e s o l v e _ a g r e e m e n t		*/
/*	---------------------------------------------------------	*/
/*	resolve the first agreement for the indicated initiator		*/
/*	---------------------------------------------------------	*/
private	char * occi_resolve_agreement( char * accid, char * agent, char * tls )
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
	char	buffer[2048];

	if (!( ihost = occi_resolve_category_provider( _CORDS_AGREEMENT, agent, tls ) ))
		return((char *) 0);
	else
	{
		sprintf(buffer,"%s/%s/",ihost,_CORDS_AGREEMENT);
		liberate( ihost );
	}

	if (!( kptr = occi_create_client( buffer, agent, tls ) ))
		return((char *) 0);
	else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
	{
		kptr = occi_remove_client( kptr );
		return((char *) 0);
	}
	else if (!(dptr=occi_request_element(qptr,"occi.agreement.initiator" , ( accid ? accid : accid ) ) ))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return((char *) 0);
	}
	else if (!( yptr = occi_client_get( kptr, qptr ) ))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return((char *) 0);
	}
	else if (!( ihost = occi_extract_location( yptr ) ))
	{
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return((char *) 0);
	}
	else
	{
		rest_add_http_prefix(buffer,2048,ihost);
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return( allocate_string( buffer ) );
	}
}

/*	---------------------------------------------------------	*/
/*		c o r d s _ u p d a t e _ p r o v i d e r		*/
/*	---------------------------------------------------------	*/
private	char *	cords_update_provider( char * vptr, char * identity, char * category, char * agent, char * tls )
{
	struct	occi_client * kptr;
	struct	occi_request * qptr;
	struct	occi_element * dptr;
	struct	occi_response * yptr;

	if (!( kptr = occi_create_client( vptr, agent, tls ) ))
		return((char *) 0);

	else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
	{
		kptr = occi_remove_client( kptr );
		return((char *) 0);
	}
	else if ((!(dptr=occi_request_element(qptr,"occi.provider.identity" , (identity ? identity : "" ) ) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.provider.category", (category ? category : "" ) ) )))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return((char *) 0);
	}
	else if (!( yptr = occi_client_put( kptr, qptr ) ))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return((char *) 0);
	}
	else
	{
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return( vptr );
	}
}

/*	---------------------------------------------------------	*/
/*	   	r e s o l v e _ t e r m _ p r o v i d e r		*/
/*	---------------------------------------------------------	*/
private	char *	resolve_term_provider( struct occi_response * term, char * agent, char * tls )
{
	struct	occi_element * dptr;

	if (!( term )) 	
		return((char *) 0);
	else if (!( dptr = occi_locate_element( term->first, "occi.term.provider" )))
		return( (char *) 0 );
	else if (!( rest_valid_string( dptr->value ) ))
		return( (char *) 0 );
	else	return( allocate_string( dptr->value ) );
}

/*	---------------------------------------------------------	*/
/*	   r e s o l v e _ a g r e e m e n t _ t e r m 			*/
/*	---------------------------------------------------------	*/
private	char *	resolve_agreement_term( struct occi_response * agreement, char * agent, char * tls )
{
	struct	occi_response * zptr;
	struct	occi_element * eptr;
	struct	occi_element * dptr;
	char *	id;
	char *	result=(char *) 0;

	if (!( agreement )) 	
		return(0);

	/* --------------------------------------------------- */
	/* for all "term" items attached to the terms instance */
	/* --------------------------------------------------- */
	for (	eptr=cords_first_link( agreement );
		eptr != (struct occi_element *) 0;
		eptr = eptr->next )
	{
		if (!( eptr->value ))
			continue;
		else if (!( id =  occi_unquoted_link( eptr->value ) ))
			continue;
		else if (!( zptr = occi_simple_get( id, agent, tls )))
			continue;
		else if (!( result = resolve_term_provider( zptr, agent, tls ) ))
		{
			zptr = occi_remove_response( zptr );
			continue;
		}
		else
		{
			zptr = occi_remove_response( zptr );
			break;
		}
	}
	return( result );
}

/*	---------------------------------------------------------	*/
/*	   r e s o l v e _ a g r e e m e n t _ s e r v i c e 		*/
/*	---------------------------------------------------------	*/
private	char *	resolve_agreement_service( struct occi_response * agreement, char * agent, char * tls )
{
	struct	occi_response * zptr;
	struct	occi_element * eptr;
	struct	occi_element * dptr;
	char *	id;
	char *	vptr;
	char *	result=(char *) 0;

	if (!( agreement )) 	
		return(0);

	/* -------------------------------------------- */
	/* for all "terms" attached to the sla instance */
	/* -------------------------------------------- */
	for (	eptr=cords_first_link( agreement );
		eptr != (struct occi_element *) 0;
		eptr = eptr->next )
	{
		if (!( eptr->value ))
			continue;
		else if (!( id =  occi_unquoted_link( eptr->value ) ))
			continue;
		else if (!( zptr = occi_simple_get( id, agent, tls )))
			continue;
		else if (!( dptr = occi_locate_element( zptr->first, "occi.terms.type" )))
		{
			zptr = occi_remove_response( zptr );
			continue;
		}
		else if (!( rest_valid_string( dptr->value ) ))
		{
			zptr = occi_remove_response( zptr );
			continue;
		}
		else if ( strcmp( dptr->value, _CORDS_SERVICES ) != 0)
		{
			zptr = occi_remove_response( zptr );
			continue;
		}
		else if (!( result = resolve_agreement_term( zptr, agent, tls )))
		{
			zptr = occi_remove_response( zptr );
			break;
		}
		else
		{
			zptr = occi_remove_response( zptr );
			break;
		}
	}
	return( result );
}

/*	---------------------------------------------------------	*/
/*	     c o r d s _ p r o v i d e r _ a g r e e m e n t		*/
/*	---------------------------------------------------------	*/
/*	resolve the provider account, if any, and the sla, if any	*/
/*	and process the quota information that it describes.		*/
/*	---------------------------------------------------------	*/
private	char * 	cords_provider_agreement( char * operator, char * agent, char * tls )
{
	char *	accountid;
	struct	occi_response * account;
	char *	agreementid;
	struct	occi_response * agreement;
	char *	result=(char *) 0;

	/* ----------------------------------------------------- */
	/* resolve the account id from the operator account name */
	/* ----------------------------------------------------- */
	if (!( accountid = occi_resolve_account( operator, agent, tls ) ))
		return( (char *) 0 );

	/* ----------------------------------------------------- */
	/* resolve the agreement id from the account identifier  */
	/* ----------------------------------------------------- */
	else if (!( agreementid = occi_resolve_agreement( accountid, agent, tls ) ))
	{
		liberate( accountid );
		return( (char *) 0 );
	}

	/* ----------------------------------------------------- */
	/* retrieve the agreement description record information */
	/* ----------------------------------------------------- */
	else if (!( agreement = occi_simple_get( agreementid, agent, tls ) ))
	{
		liberate( accountid );
		liberate( agreementid );
		return( (char *) 0 );
	}
	/* -------------------------- */
	/* retrieve the provider term */
	/* -------------------------- */
	else if (!( result = resolve_agreement_service( agreement, agent, tls ) ))
	{
		agreement = occi_remove_response( agreement );
		liberate( accountid );
		liberate( agreementid );
		return( (char *) 0 );
	}

	/* ----------------------------------------------------- */
	/* clean it all up now before return to the publication  */
	/* ----------------------------------------------------- */
	else
	{
		agreement = occi_remove_response( agreement );
		liberate( accountid );
		liberate( agreementid );
		return( result );
	}
}

/*	---------------------------------------------------------	*/
/*	c o r d s _ p r o v i d e r _ p u b l i c a t i o n		*/
/*	---------------------------------------------------------	*/
/*	Their might exist an SLA controlling the publication of		*/
/*	resource quota by the provider.					*/
/*	---------------------------------------------------------	*/
private	char * 	cords_provider_publication( 
		char * name,
		char * category,
		char * price,
		char * operator,
		char * identity, 
		char * agent, char * tls )
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
	char  *	result=(char *) 0;
	char	buffer[2048];

	if (!( ihost = occi_resolve_category_provider( _CORDS_PROVIDER, agent, tls ) ))
		return((char *) 0);
	else
	{
		sprintf(buffer,"%s/%s/",ihost,_CORDS_PROVIDER);
		liberate( ihost );
	}

	/* ------------------------------------------------------ */
	/* attempt to localise provider record by account and sla */
	/* ------------------------------------------------------ */
	if (( result = cords_provider_agreement( operator, agent, tls )) != (char *) 0)
	{
		Publisher.sla = 1;
		return ( cords_update_provider( result, identity, category, agent, tls ) );
	}
	/* ---------------------------------------------------- */
	/* enforce the service level agreement in security mode */
	/* ---------------------------------------------------- */
	else if ( rest_valid_string( default_tls() ) != 0 )
	{
		rest_log_message("An Account and SLA is required for provider");
		return((char *) 0);
	}
	/* ---------------------------------- */
	/* didnt find one so create a new one */
	/* ---------------------------------- */
	else
	{
		if (!( kptr = occi_create_client( buffer, agent, tls ) ))
			return((char *) 0);
		else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
		{
			kptr = occi_remove_client( kptr );
			return((char *) 0);
		}
		else if ((!(dptr=occi_request_element(qptr,"occi.provider.name"  	, ( name ? name : name )       ) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.provider.price" 	, ( price ? price : "" )       ) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.provider.category" 	, ( category ? category : "" )     ) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.provider.operator" 	, ( operator ? operator : "" ) ) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.provider.identity"	, ( identity ? identity : "" ) ) )))
		{
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			return((char *) 0);
		}
		else if (!( yptr = occi_client_post( kptr, qptr ) ))
		{
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			return((char *) 0);
		}
		else if (!( ihost = occi_extract_location( yptr ) ))
		{
			yptr = occi_remove_response( yptr );
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			return((char *) 0);
		}
		else
		{
			rest_add_http_prefix(buffer,2048,ihost);
			yptr = occi_remove_response( yptr );
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			if (!( result = allocate_string( buffer ) ))
				return( result );
			else	return( result );
		}
	}	
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
	char	username[1024];

	sprintf(username,"%s",user);

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
private	struct	rest_response * null_occi_transaction(
		void *	vptr,
		struct	rest_client * cptr,
		struct  rest_request * rptr,
		struct 	rest_response * aptr
		)
{
	return( aptr );
}

/*	--------------------------------------------------------	*/
/*		o c c i _ s e n d _ t r a n s a c t i o n 		*/
/*	--------------------------------------------------------	*/
public	int	occi_send_transaction( char * source, char * price, char * description, char * account, char * reference )
{
	char 	buffer[1024];
	char *	host;
	struct	occi_element 	*	eptr;
	struct	occi_client	*	kptr;
	struct	occi_request	*	qptr;
	struct	occi_response	*	zptr;
	
	/* ---------------------------------------------------- */
	/* resolve a transaction service provider via publisher	*/
	/* ---------------------------------------------------- */
	if (!( host = occi_resolve_category_provider( _CORDS_TRANSACTION, _CORDS_CONTRACT_AGENT,  Publisher.tls ) ))
		return(0);
	else
	{
		/* ------------------------------------------------------ */
		/* build a transaction category instance creation request */
		/* ------------------------------------------------------ */
		sprintf(buffer,"%s/%s/",host,_CORDS_TRANSACTION);
		host = liberate( host );
		if (!( kptr = occi_create_client( buffer, _CORDS_CONTRACT_AGENT,  Publisher.tls )))
			return( 0 );
		else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL ) ))
			return( 0 );
		else if ((!( eptr = occi_request_element( qptr, "occi.transaction.source", source )))
		||       (!( eptr = occi_request_element( qptr, "occi.transaction.price", price )))
		||       (!( eptr = occi_request_element( qptr, "occi.transaction.description", description )))
		||       (!( eptr = occi_request_element( qptr, "occi.transaction.reference", reference )))
		||       (!( eptr = occi_request_element( qptr, "occi.transaction.account", account ))))
		{
			qptr = occi_remove_request( qptr );
			return( 0 );
		}
		/* ---------------------------------------------------- */
		/* post the request to the transaction category manager */
		/* ---------------------------------------------------- */
		else if (!( zptr = occi_client_post( kptr, qptr ) ))
		{
			qptr = occi_remove_request( qptr );
			return( 0 );
		}
		else
		{
			/* ------- */
			/* success */
			/* ------- */
			zptr = occi_remove_response( zptr );
			qptr = occi_remove_request( qptr );
			return( 1 );
		}
	}
}

/*	---------------------------------------------------------	*/
/*	    p r o c e s s _ o c c i _ t r a n s a c t i o n		*/
/*	---------------------------------------------------------	*/
/*	this relay interface method is called back after the post	*/
/*	method invocation allowing for priced transaction events	*/
/*	to be generated for priced categories.				*/
/*	---------------------------------------------------------	*/
private	struct 	rest_response * process_occi_transaction(
		void *	vptr,
		struct	rest_client * cptr,
		struct  rest_request * rptr,
		struct 	rest_response * aptr
		)
{
	struct	rest_header	*	hptr;
	struct	occi_category 	*	optr;
	char *	price;
	char *	host;

	/* ------------------------------------------------------- */
	/* This method must resolve the category price information */
	/* ------------------------------------------------------- */
	if (!( optr = vptr ))
		return( null_occi_transaction( vptr, cptr, rptr, aptr ) );
	else if (!( price = optr->price ))
		return( null_occi_transaction( vptr, cptr, rptr, aptr ) );
	else if (!( strlen( price ) ))
		return( null_occi_transaction( vptr, cptr, rptr, aptr ) );

	/* -------------------------------------------------*/
	/* resolve the account information from the request */
	/* the method from the request			    */
	/* ------------------------------------------------ */
	else if (!( rptr ))
		return( null_occi_transaction( vptr, cptr, rptr, aptr ) );
	else if (!( hptr = rest_resolve_header( rptr->first, _OCCI_ACCOUNT ) ))
		return( null_occi_transaction( vptr, cptr, rptr, aptr ) );
	else if (!( hptr->value ))
		return( null_occi_transaction( vptr, cptr, rptr, aptr ) );
	else if (!( occi_send_transaction( optr->id, price, "method=create", hptr->value, _CORDS_NULL ) ))
		return( null_occi_transaction( vptr, cptr, rptr, aptr ) );
	else	return( aptr );
}

/*	---------------------------------------------------------	*/
/*	      p u b l i s h _ o c c i _ c a t e g o r y 		*/
/*	---------------------------------------------------------	*/
public	int	publish_occi_category( 
		int	item,
		char * user,	char * password,
		char * url,	char * agent, 
		struct occi_category * category,
		char * contract
		)
{
	struct	rest_interface * iptr;
	struct	occi_element  *	eptr;
	struct	occi_client   *	cptr;
	struct	occi_request  *	rptr;
	struct	occi_response * aptr;
	struct	occi_publication * pptr;
	char 	*	uri;
	char	username[1024];
	
	/* ------------------------------------------------------------------------ */
	/* resolve an eventual price for the provision of instances of the category */
	/* ------------------------------------------------------------------------ */
	if (!( category->price ))
	{
		if ( category->access & _OCCI_NO_PRICING )
		{
			if (!( category->price = allocate_string("") ))
				return(27);
		}
		else if (!( category->price = occi_resolve_category_price( category->id, default_operator(), agent, Publisher.tls ) ))
		{	
			if (!( category->price = allocate_string("") ))
				return(27);
		}
		else if ((iptr = category->interface) != (struct rest_interface *) 0)
			iptr->transaction = process_occi_transaction;
	}
	else if ((iptr = category->interface) != (struct rest_interface *) 0)
		iptr->transaction = null_occi_transaction;

	/* ------------------------------------- */
	/* generate a user identification string */
	/* ------------------------------------- */
	sprintf(username,"%s",user);

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
	     ||  (!( eptr = occi_request_element( rptr, "occi.publication.where",
			( category->access & _OCCI_CONTRACT ? 
			( contract ? contract : Publisher.room ) : Publisher.room)	)))
	     ||  (!( eptr = occi_request_element( rptr, "occi.publication.what",category->id 	)))
	     ||  (!( eptr = occi_request_element( rptr, "occi.publication.operator",default_operator() )))
	     ||  (!( eptr = occi_request_element( rptr, "occi.publication.zone",default_zone() )))
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
	char 	*	contract=(char *) 0;
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
				pptr->url, pptr->agent, optr, contract ));
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
		struct occi_category * category, char * contract )
{
	char *	vptr;
	int	items=0;
	struct	occi_category *	optr;
	int	status=0;
	for (	optr=category;
		optr != (struct occi_category *) 0;
		optr = optr->next )
	{
		if ( optr->access & _OCCI_CONSUMER )
		{
			if (!( vptr = cords_consumer_publication( 
				user,
				category->price,
				default_connection(),
				default_operator(),
				url,
				agent,
				default_tls() ) ))
				return(27);
			else if (!( Publisher.consumer = vptr ))
				return(27);
		}

		if ( optr->access & _OCCI_PROVIDER )
		{
			if (!( vptr = cords_provider_publication( 
				user,
				optr->id,
				category->price,
				default_operator(),
				url,
				agent,
				default_tls() ) ))
				return(27);
			else if (!( Publisher.provider = vptr ))
				return(27);
		}

		if ( optr->access & _OCCI_PRIVATE )
			continue;
		else if ((status = publish_occi_category( ++items, user, password, url, agent, optr, contract )) != 0)
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
/*		   o c c i _ s e c u r e _ A A A 			*/
/*	---------------------------------------------------------	*/
public	int	occi_secure_AAA( char * user, char * password, char * agent, char * tls )
{
	struct	rest_header * hptr;
	if ( Publisher.authorization )
		return( 0 );
	else if (!(Publisher.authorization = login_occi_user( user, password, agent, tls )))
		return( 403 );
	else if (!( hptr = occi_server_authentication( Publisher.authorization )))
		return( 503 );
	else	return( 0 );
}

/*	---------------------------------------------------------	*/
/*		   o c c i _ r el e a s e _ A A A 			*/
/*	---------------------------------------------------------	*/
public	int	occi_release_AAA( char * user, char * password, char * agent, char * tls )
{
	if ( Publisher.authorization )
		Publisher.authorization = logout_occi_user( 
			user, password, agent, Publisher.authorization, tls );
	return( 0 );
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
	char *	contract=(char *) 0;
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
	if (!( rest_valid_string( tls ) ))
		tls = (char *) 0;
	else if (!( tlsconf = tls_configuration_load( tls ) ))
		return( 40 );
	else if ( tlsconf->authenticate )
	{
		if ((user) && (password))
		{
			if ((status = occi_secure_AAA( user, password, agent, tls )) != 0)
				return( status );
		}
	}
	activate_event_manager();

	/* ----------------------------------------- */
	/* publish the collection of OCCI categories */
	/* ----------------------------------------- */
	if (( Publisher.host ) && ( Publisher.publication ))
		if (( url ) && ( category ))
			if ((status = publish_occi_categories( user, password, url, agent, category, contract )) != 0)
				return( status );

	/* ---------------------------------------------------------- */
	/* activate the OCCI server layer for the required categories */
	/* ---------------------------------------------------------- */
	result = occi_server( agent, port, tls, max, category, Publisher.authorization );

	/* ------------------------------------------------- */
	/* check for and remove monitoring consumer identity */
	/* ------------------------------------------------- */
	if ( Publisher.consumer )
	{
		occi_simple_delete( Publisher.consumer, agent, default_tls() );
		Publisher.consumer = liberate( Publisher.consumer );
	}

	/* ------------------------------------------------- */
	/* check for and remove monitoring provider identity */
	/* ------------------------------------------------- */
	if ( Publisher.provider )
	{
		if (!( Publisher.sla ))
		{
			occi_simple_delete( Publisher.provider, agent, default_tls() );
			Publisher.sla = 0;
		}
		else	cords_update_provider( Publisher.provider,"", "", agent, default_tls() );

		Publisher.provider = liberate( Publisher.provider );
	}

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
		occi_release_AAA( user, password, agent, tls );
		if ( tlsconf )
			tlsconf = release_tls_configuration(tlsconf );
	}
	return( result );
}


	/* ---------------- */
#endif	/* _occipublisher_c */
	/* ---------------- */



