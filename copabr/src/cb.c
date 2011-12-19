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
#ifndef	_cords_broker_c
#define	_cords_broker_c

#include "cb.h"
#include "csp.c"
#include "restlog.h"

struct	operator_preferences
{
	char *	provider;
	char *	account;
	char * 	tarif;
} Operator = { 
	"openstack",
	"accords",
	"standard"
	};

private	struct	xml_element * 	cords_instance_contract(
	struct cords_node_descriptor * App,
	char *	host,
	char *	id,
	char *	agent,
	char *	tls,
	char * namePlan );

private	struct	xml_element * 	cords_complete_contract(
	struct 	cords_node_descriptor * App,
	struct	xml_element * document,
	char *	agent,
	char *	tls );

private	int	provisioning_status=0;

/*	--------------------------------------------------------	*/
/*	    g e t _ p r o v i s i o n i n g _ s t a t u s		*/
/*	--------------------------------------------------------	*/
/*	retrieve the status code of the most recent provisioning	*/
/*	operation performed.						*/
/*	--------------------------------------------------------	*/
public	int	get_provisioning_status()
{
	return( provisioning_status );
}

/*	-------------------------------------------------------		*/
/*	t e r m i n a t e _ c o r d s _ p r o v i s i o n i n g		*/
/*	-------------------------------------------------------		*/
/*	returns the instance ID for return to the calling layer		*/
/*	-------------------------------------------------------		*/
private	char * 	cords_terminate_provisioning( 
		int status, 
		struct cords_provisioning * pptr )
{
	char * service=(char *) 0;

	if ( pptr->confID )
		pptr->confID = liberate( pptr->confID );
	if ( pptr->configuration )
		pptr->configuration = occi_remove_response( pptr->configuration );
	if ( pptr->secID )
		pptr->secID = liberate( pptr->secID );
	if ( pptr->security )
		pptr->security = occi_remove_response( pptr->security );
	if ( pptr->accID )
		pptr->accID = liberate( pptr->accID );
	if ( pptr->account )
		pptr->account = occi_remove_response( pptr->account );
	if ( pptr->reqID )
		pptr->reqID = liberate( pptr->reqID );
	if ( pptr->manifest )
		pptr->manifest = occi_remove_response( pptr->manifest );
	if ( pptr->planID )
		pptr->planID = liberate( pptr->planID );
	if ( pptr->namePlan )
		pptr->namePlan = liberate( pptr->namePlan );
	if ( pptr->plan )
		pptr->plan = occi_remove_response( pptr->plan );
	if ( pptr->instance )
		pptr->instance = occi_remove_response( pptr->instance );
	if ( pptr->document )
		pptr->document = document_drop( pptr->document );

	if (!( provisioning_status = status ))
		service = pptr->instID;
	else if ( pptr->instID )
		pptr->instID = liberate( pptr->instID );
	
	return( service );
}

/*	-------------------------------------------------------		*/
/*	     c o r d s _ r e t r i e v e _ i n s t a n c e		*/
/*	-------------------------------------------------------		*/
private	struct	occi_response * cords_retrieve_instance(
		char * host,
		char * id,
		char * agent,
		char * tls
		 )
{
	return( occi_simple_get( id, _CORDS_CONTRACT_AGENT, tls ) );
}

/*	-------------------------------------------------------		*/
/*		c o r d s _ e x t r a c t _ a t r i b u t		*/
/*	-------------------------------------------------------		*/
private	char *	cords_extract_atribut( 
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
	
/*	-------------------------------------------------------		*/
/*		    c o r d s _ e x t r a c t _ l i n k			*/
/*	-------------------------------------------------------		*/
private	struct	occi_element * 	cords_extract_link( struct occi_element * eptr )
{
	for (	;
		eptr != (struct occi_element *) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( strcasecmp( eptr->name, "LINK" ) ))
			break;
	}
	return( eptr );
}
	
/*	-------------------------------------------------------		*/
/*		    c o r d s _ f i r s t _ l i n k 			*/
/*	-------------------------------------------------------		*/
public 	struct	occi_element * cords_first_link( struct occi_response * zptr )
{
	return( cords_extract_link( zptr->first ) );
}

/*	-------------------------------------------------------		*/
/*		    c o r d s _ n e x t _ l i n k 			*/
/*	-------------------------------------------------------		*/
public 	struct occi_element * cords_next_link( struct occi_element * eptr )
{
	return( cords_extract_link( eptr->next ) );
}

/*	-------------------------------------------------------		*/
/*	  c o r d s _ r e s o l v e _ c o n t r a c t _ i d 		*/
/*	-------------------------------------------------------		*/
private	struct xml_atribut *	cords_resolve_contract_id( struct xml_element * document, char * coreappname )
{
	struct	xml_element * eptr;
	struct	xml_atribut * bptr;
	int	l;

	/* --------------------------------------------------- */ 
	/* resolve the contract id of the target of the action */
	/* --------------------------------------------------- */ 
	for (	eptr = document_element( document, _CORDS_CONTRACT );
		eptr != (struct xml_element *) 0;
		eptr = eptr->next )
	{
		if (!( bptr = document_atribut( eptr, _CORDS_NAME ) ))
			continue;
		else if (!( bptr->value ))
			continue;
		/* ---------------------------------------------------- */
		/* an exact match is easy so we have found the contract */
		/* ---------------------------------------------------- */
		else if (!( strcmp( bptr->value , coreappname  ) ))
			break;
		else if (!( l = strlen( bptr->value ) ))
			continue;
		else if (!( strncmp( bptr->value, coreappname, l )))
		{
			/* ------------------------------------------- */
			/* perhaps we have found a complex node prefix */
			/* ------------------------------------------- */
			if (!( bptr = document_atribut( eptr, _CORDS_TYPE ) ))
				continue;
			else if (!( bptr->value ))
				continue;
			else if (!( strcmp( bptr->value , _CORDS_SIMPLE ) ))
				continue;
			else if ( *(coreappname+l) != '.' )
				continue;
			else if (!( eptr->first ))
				continue;
			else	return( cords_resolve_contract_id( eptr->first, (coreappname+l+1) ) );
		}
		else	continue;
	}
	if (!( eptr ))
		return((struct xml_atribut *) 0);
	else if (!( bptr = document_atribut( eptr, _CORDS_ID ) ))
		return((struct xml_atribut *) 0);
	else if (!( bptr->value ))
		return((struct xml_atribut *) 0);
	else	return( bptr );
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

	if (!( ihost = rest_log_comons_identity(_CORDS_EVENT,agent,tls) ))
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

/*	---------------------------------------------------------	*/
/*	c o r d s _ a f f e c t a t i o n _ i n s t r u c t i o n	*/
/*	---------------------------------------------------------	*/
/*	here postscript configuration actions are transformed		*/
/*	to produce the configuration and monitoring instruction		*/
/*	set used by the combined broker, procci and providers  		*/
/*	during the actual provisioning for meta data and probe		*/
/*	channel communication and delivery.				*/
/*	---------------------------------------------------------	*/
private	int	cords_affectation_instruction( 
		char * host,
		struct xml_element * document,
		struct cordscript_action * action,
		char * agent,
		char * tls )
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
	char *	avalue=(char *) 0;
	char	buffer[2048];

	if (!( lptr = action->lvalue ))
		return( 78 );

	else if (!( lptr->prefix ))
		return( 30 );

	else if (!( aptr = cords_resolve_contract_id( document, lptr->prefix ) ))
		return( 78 );

	if (!( ihost = occi_resolve_category_provider( _CORDS_INSTRUCTION, agent, tls ) ))
	{
		liberate_cordscript_action( action );
		return(46);
	}

	sprintf(buffer,"%s/%s/",ihost,_CORDS_INSTRUCTION);

	liberate( ihost );

	for (	rvalue=action->rvalue;
		rvalue != (struct cordscript_element *) 0;
		rvalue = rvalue->next )
	{
		if (!( kptr = occi_create_client( buffer, agent, tls ) ))
		{
			liberate_cordscript_action( action );
			return(46);
		}
		else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
		{
			kptr = occi_remove_client( kptr );
			liberate_cordscript_action( action );
			return(50);
		}
		else if (!( avalue = occi_unquoted_value( rvalue->value ) ))
		{
			kptr = occi_remove_client( kptr );
			liberate_cordscript_action( action );
			return(27);
		}
		else if ((!(dptr=occi_request_element(qptr,"occi.instruction.target"  	, aptr->value 	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.member"  	, "configure" 	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.type"  	, "method"  	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.provision" , "" 		) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.symbol" 	, "self"        ) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.source" 	, aptr->value 	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.property"	, lptr->value	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.value"   	, avalue       	) )))
		{
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			liberate( avalue );
			liberate_cordscript_action( action );
			return(51);
		}
		else if (!( yptr = occi_client_post( kptr, qptr ) ))
		{
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			liberate( avalue );
			liberate_cordscript_action( action );
			return(52);
		}
		else if (!( ihost = cords_extract_location( yptr ) ))
		{
			yptr = occi_remove_response( yptr );
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			liberate( avalue );
			liberate_cordscript_action( action );
			return(53);
		}
		else if (!( zptr =  cords_create_link( aptr->value,  ihost, agent,tls ) ))
		{
			yptr = occi_remove_response( yptr );
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			liberate( avalue );
			liberate_cordscript_action( action );
			return(54);
		}
		else
		{
			zptr = occi_remove_response( zptr );
			yptr = occi_remove_response( yptr );
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			liberate( avalue );
		}


	}
	liberate_cordscript_action( action );
	return( 0 );

}

/*	-------------------------------------------------------		*/
/*	  c o r d s _ a c t i o n _ i n s t r u c t i o n		*/
/*	-------------------------------------------------------		*/
/*	here postscript configuration actions are transformed		*/
/*	to produce the configuration and monitoring instruction		*/
/*	set used by the combined broker, procci and providers  		*/
/*	during the actual provisioning for meta data and probe		*/
/*	channel communication and delivery.				*/
/*	-------------------------------------------------------		*/
private	int	cords_action_instruction( 
		char * host,
		struct xml_element * document,
		struct cordscript_action * action,
		char * agent,
		char * tls )
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
	char *	mname;
	char	buffer[2048];

	if (!( lptr = action->lvalue ))
		return( 78 );

	else if (!( lptr->prefix ))
		return( 30 );

	else if (!( mname = cordscript_method( action->type ) ))
		return(30);

	else if (!( strcasecmp( mname, "none" ) ))
		return( cords_affectation_instruction( host, document, action, agent, tls ) );

	else if (!( aptr = cords_resolve_contract_id( document, lptr->prefix ) ))
		return( 78 );

	if (!( ihost = occi_resolve_category_provider( _CORDS_INSTRUCTION, agent, tls ) ))
	{
		liberate_cordscript_action( action );
		return(46);
	}

	sprintf(buffer,"%s/%s/",ihost,_CORDS_INSTRUCTION);

	liberate( ihost );

	for (	rvalue=action->rvalue;
		rvalue != (struct cordscript_element *) 0;
		rvalue = rvalue->next )
	{
		if (!( bptr = cords_resolve_contract_id( document, rvalue->prefix ) ))
			continue;

		else if (!( kptr = occi_create_client( buffer, agent, tls ) ))
		{
			liberate_cordscript_action( action );
			return(46);
		}
		else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
		{
			kptr = occi_remove_client( kptr );
			liberate_cordscript_action( action );
			return(50);
		}
		else if ((!(dptr=occi_request_element(qptr,"occi.instruction.target"  	, aptr->value 	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.member"  	, mname 	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.type"  	, "method"  	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.provision" , "" 		) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.value"  	, "" 		) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.symbol" 	, rvalue->prefix) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.source" 	, bptr->value 	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.property"	, rvalue->value	) )))
		{
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			liberate_cordscript_action( action );
			return(51);
		}
		else if (!( yptr = occi_client_post( kptr, qptr ) ))
		{
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			liberate_cordscript_action( action );
			return(52);
		}
		else if (!( ihost = cords_extract_location( yptr ) ))
		{
			yptr = occi_remove_response( yptr );
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			liberate_cordscript_action( action );
			return(53);
		}
		else if (!( zptr =  cords_create_link( aptr->value,  ihost, agent,tls ) ))
		{
			yptr = occi_remove_response( yptr );
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			liberate_cordscript_action( action );
			return(54);
		}
		else
		{
			zptr = occi_remove_response( zptr );
			yptr = occi_remove_response( yptr );
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
		}
	}
	liberate_cordscript_action( action );
	return( 0 );
};

/*	-------------------------------------------------------		*/
/*	  c o r d s _ c o n f i g u r a t i o n	_ a c t i o n		*/
/*	-------------------------------------------------------		*/
private	int	cords_configuration_action( 
		char * host,
		struct xml_element * document,
		struct occi_response * zptr,
		char * agent,
		char * tls )
{
	char *	type=(char *) 0;
	char *	statement=(char *) 0;
	struct	cordscript_action * aptr=(struct cordscript_action*) 0;

	if (!( statement = cords_extract_atribut( zptr, "occi",_CORDS_ACTION,_CORDS_EXPRESSION ) ))
	{
		return( 78 );
	}
	else if (!( type = cords_extract_atribut( zptr, "occi",_CORDS_ACTION,_CORDS_TYPE ) ))
		type = allocate_string("cordscript");

	if (!( strcmp( type, "cordscript" )))
		if ((aptr = cordscript_parse_statement( statement )) != (struct cordscript_action *) 0)
			cords_action_instruction( host, document, aptr, agent, tls );

	type = liberate( type );
	statement = liberate( statement );	 

	return(0);
}

/*	-----------------------------------------------		*/
/*	c o r d s _ u p d a t e _ c o m m o n _ n o d e 	*/
/*	-----------------------------------------------		*/
/*	updates the common field of a node instance		*/
/*	-----------------------------------------------		*/
private	struct occi_response * cords_update_common_node( 
		char * id, 
		struct occi_response * zptr, 
		char * common, 
		char * agent, 
		char * tls )
{
	struct	occi_client * kptr;
	struct	occi_request * qptr;
	struct	occi_element * fptr;
	struct	occi_element * eptr;
	struct	occi_response * yptr;

	/* ----------------------------------------------------- */
	/* build a client and request, the ncopy response fields */
	/* ----------------------------------------------------- */
	if (!( kptr = occi_create_client( id, agent, tls ) ))
		return((struct occi_response *) 0);
	else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
		return((struct occi_response *) 0);
	else if (!( fptr = occi_request_from_response( qptr, zptr )))
		return((struct occi_response *) 0);
	else
	{
		for ( 	eptr=fptr;
			eptr != (struct occi_element *) 0;
			eptr = eptr->next )
			if (!( strcmp( eptr->name, "occi.node.common" ) ))
				break;
		if ( eptr )
		{
			if ( eptr->value )
				eptr->value = liberate( eptr->value );
			if (!( eptr->value = allocate_string( common )))
				return((struct occi_response *) 0);
		}
		else if (!( fptr = occi_request_element( qptr, "occi.node.common", common )))
			return((struct occi_response *) 0);
	}

	/* ---------------------------- */
	/* put the node data collection */
	/* ---------------------------- */
	if (!( yptr = occi_client_put( kptr, qptr ) ))
		return((struct occi_response *) 0);
	else	return( yptr );
}

/*	-------------------------------------------------------		*/
/*	  c o r d s _ b r o k e r _ c o n f i g u r a t i o n		*/
/*	-------------------------------------------------------		*/
/*	this function is called from the service instance build		*/
/*	once the contract instances have been created and their		*/
/*	details are resolved and established. 				*/
/*									*/
/*	Here two things will happen:					*/
/*									*/
/*	1) we will parse the collection of action statements		*/
/*	to produce the corresponding concrete configuration 		*/
/*	actions required to communicate the run time parameter		*/
/*	values to the provider instantiation mechanisms.		*/
/*									*/
/*	2) we will parse the collection of action statements to		*/
/*	produce the collection of monitoring channels that are 		*/
/*	to be set up for the control of the service.			*/
/*									*/
/*	These two will occur at the same, differentiated by the		*/
/*	cordscript keywords "configure" and "monitor".			*/
/*	-------------------------------------------------------		*/

private	int	cords_broker_configuration( 
		char * host,
		struct xml_element * document,
		struct occi_response * zptr,
		char * agent, char * tls )
{
	struct	occi_element *	eptr;
	struct	occi_response * aptr;
	char	*	id;
	int		status;
	if (!( zptr )) return(0);
	for (	eptr=cords_first_link( zptr );
		eptr != (struct occi_element *) 0;
		eptr = eptr->next )
	{
		if (!( eptr->value ))
			continue;
		if (!( id =  occi_unquoted_link( eptr->value ) ))
			continue;
		else if (!( aptr = cords_retrieve_instance( host, id, agent, tls ) ))
		{
			return( 908 );
		}
		else
		{
			cords_configuration_action( host, document, aptr, agent, tls );
			aptr = occi_remove_response( aptr );
			id = liberate( id );
			continue;
		}
	}
	return( 0 );
}

/*	--------------------------------------------------------	*/
/*		c o r d s _ c r e a t e _ p r o v i d e r 		*/
/*	--------------------------------------------------------	*/
private	char * 	cords_create_provider( struct xml_element * dptr , char * agent, char * tls )
{
	struct	occi_client   * kptr;
	struct	occi_response * yptr;
	struct	occi_response * zptr;
	struct	occi_request  * qptr;
	struct	occi_element  * fptr;
	struct	xml_atribut  * cptr;
	struct	xml_atribut  * pptr;
	struct	xml_atribut  * nptr;
	char *	sptr;
	char *	host;
	char 	buffer[2048];
	char 	filter[1024];

	/* -------------------------------------------- */
	/* retrieve the provider identification category */
	/* -------------------------------------------- */
	if (!( cptr = document_atribut( dptr, _CORDS_CATEGORY ) ))
		return((char *) 0);

	/* -------------------------------------------- */
	/* retrieve the provider identification name     */
	/* -------------------------------------------- */
	else if (!( nptr = document_atribut( dptr, _CORDS_NAME ) ))
		return((char *) 0);

	/* -------------------------------------------- */
	/* retrieve the provider profile name string     */
	/* -------------------------------------------- */
	else if (!( pptr = document_atribut( dptr, _CORDS_PROFILE ) ))
		return((char *) 0);

	/* --------------------------------- */
	/* select a list of provider records */
	/* --------------------------------- */
	sprintf(filter,"occi.%s.name",cptr->value);

	if (!( yptr = cords_retrieve_named_instance_list( 
		cptr->value, filter, nptr->value, agent,tls )))
		return((char *) 0);

	/* --------------------------------- */
	/* get the first of the matching set */
	/* --------------------------------- */
	else if (!( zptr = cords_retrieve_named_instance( yptr, agent,tls )))
		return((char *) 0);
	else	yptr = occi_remove_response( yptr );

	/* ---------------------------------- */
	/* duplicate the collection of values */
	/* ---------------------------------- */
	sprintf(buffer,"%s/%s/",zptr->host,cptr->value);
	if (!( kptr = occi_create_client( buffer, agent, tls ) ))
		return((char *) 0);
	else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
		return((char *) 0);
	else if (!( fptr = occi_request_from_response( qptr, zptr )))
		return((char *) 0);
	/* ----------------------------------- */
	/* set profile and reset result values */
	/* ----------------------------------- */
	sprintf(buffer,"occi.%s.",cptr->value);
	for (	fptr = qptr->first;
		fptr != (struct occi_element *) 0;
		fptr = fptr->next )
	{
		if (!( sptr = fptr->name ))
			continue;
		else if ( strncmp( sptr, buffer, strlen(buffer) ) != 0 )
			continue;
		else	sptr += strlen( buffer );
		if ((!( strcmp( sptr,"hostname"  ) ))
		||  (!( strcmp( sptr,"rootpass"  ) ))
		||  (!( strcmp( sptr,"reference" ) )))
		{
			if ( fptr->value ) 
				fptr->value = liberate( fptr->value );
			if (!( fptr->value = allocate_string("") ))
				return((char *) 0);
		}
		else if (!( strcmp( sptr,"profile"  ) ))
		{
			if ( fptr->value ) 
				fptr->value = liberate( fptr->value );
			if (!( fptr->value = allocate_string(pptr->value) ))
				return((char *) 0);
		}
		else if (!( strcmp( sptr,"name"  ) ))
		{
			if ( fptr->value ) 
				fptr->value = liberate( fptr->value );
			if (!( fptr->value = allocate_string(nptr->value) ))
				return((char *) 0);
		}
	}

	/* ------------------------ */
	/* post the data collection */
	/* ------------------------ */
	if (!( yptr = occi_client_post( kptr, qptr ) ))
		return((char *) 0);

	/* --------------------- */
	/* return the identifier */
	/* --------------------- */
	else if (!( host = cords_extract_location( yptr ) ))
		return((char *) 0);
	else if (!( host = occi_category_id( host ) ))
		return( host );
	else
	{
		sprintf(buffer,"%s/%s/%s",zptr->host,cptr->value,host);
		return(allocate_string(buffer) );
	}
}

/*	-------------------------------------------------------		*/
/*		c o r d s _ a d d  _ p r o v i d e r  			*/
/*	-------------------------------------------------------		*/
/*	builds an XML element  form of a provider instance of a		*/
/*	cords instance contract						*/
/*	-------------------------------------------------------		*/
private	struct	xml_element * 	cords_add_provider( 
			struct xml_element * dptr,
			char * provider, char * name, char * profile  )
{
	struct	xml_atribut * aptr;
	struct	xml_element * eptr;
	if (!( eptr = document_add_element(dptr,_CORDS_PROVIDER)))
		return((struct xml_element *) 0);
	else if (!( aptr = document_add_atribut( eptr, _CORDS_CATEGORY, provider ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_NAME, name ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_PROFILE, profile ) ))
		return(document_drop( eptr ));
	else	return(eptr);
}

/*	---------------------------------------------------------	*/
/*	c o r d s _ t e r m i n a t e _ i n s t a n c e _ n o d e	*/
/*	---------------------------------------------------------	*/
private	void 	cords_terminate_instance_node( struct cords_node_descriptor * dptr )
{
	if ( dptr->nameApp )	dptr->nameApp = liberate( dptr->nameApp );
	if ( dptr->typeApp )	dptr->typeApp = liberate( dptr->typeApp );
	if ( dptr->accessApp )	dptr->accessApp = liberate( dptr->accessApp );
	if ( dptr->scopeApp )	dptr->scopeApp = liberate( dptr->scopeApp );
	if ( dptr->hid )	dptr->hid = liberate( dptr->hid );
	if ( dptr->sid )	dptr->sid = liberate( dptr->sid );
	if ( dptr->provider )	dptr->provider = liberate( dptr->provider );
	if ( dptr->providerid )	dptr->providerid = liberate( dptr->providerid );
	if ( dptr->profile )	dptr->profile = liberate( dptr->profile );
	if ( dptr->system )	dptr->system = occi_remove_response( dptr->system );
	if ( dptr->software )	dptr->software = occi_remove_response( dptr->software );
	if ( dptr->network )	dptr->network = occi_remove_response( dptr->network );
	if ( dptr->storage )	dptr->storage = occi_remove_response( dptr->storage );
	if ( dptr->compute )	dptr->compute = occi_remove_response( dptr->compute );
	if ( dptr->hardware )	dptr->hardware = occi_remove_response( dptr->hardware );
	if ( dptr->node )	dptr->node = occi_remove_response( dptr->node );
	if ( dptr->contract )	dptr->contract = occi_remove_response( dptr->contract );
	return;
}


/*	-------------------------------------------------------		*/
/*		c o r d s _ a c c e p t _ p r o v i d e r 		*/
/*	-------------------------------------------------------		*/
/*	this function is called to enforce provider filtering		*/
/*	rules that may have been established concerning nature		*/
/*	of provider category, tarification or energy consumption		*/
/*	-------------------------------------------------------		*/
private	char * cords_accept_provider( char * category, struct occi_response * zptr )
{
	/* --------------- */
	/* nothing for now */
	/* so any are good */
	/* --------------- */
	return( category );
}

/*	-------------------------------------------------------		*/
/*	      c o r d s _ r e s e a r c h _ p r o v i d e r 		*/
/*	-------------------------------------------------------		*/
/*	This function will seek to find a valid provider service		*/
/*	to fulfill the needs of an "any", "null" or missing 		*/
/*	provider atribut for an infrastructure provisioning.		*/
/*	The resolver will be used to localise agencys which 		*/
/*	will be tried in turn to localise provider descriptions		*/
/*	-------------------------------------------------------		*/
private	char *	cords_research_provider( char * defaut, char * agent, char * tls )
{
	struct	occi_response * zptr;
	struct	occi_response * yptr;
	struct	occi_response * xptr;
	struct	occi_element  * eptr;
	struct	occi_element  * fptr;
	struct	occi_element  * gptr;
	char *	sptr;
	char 	buffer[2048];

	/* ------------------------------------------------------ */
	/* attempt to resolve agencys of the "provider" category */
	/* ------------------------------------------------------ */
	if (!( zptr = occi_resolver( _CORDS_PROVIDER, agent ) ))
		return( allocate_string( defaut ) );

	/* ------------------------------------------------------ */
	/*  scan the list to find their list of providers offered  */
	/* ------------------------------------------------------ */
	for (	eptr = zptr->first;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;

		sprintf(buffer,"%s/%s/",eptr->value,_CORDS_PROVIDER);
		if (!( yptr = occi_simple_get( buffer, agent,tls ) ))
			continue;

		/* ------------------------------------------------------ */
		/*  scan their list of offered providers to localise one   */
		/* ------------------------------------------------------ */
		for (	fptr = yptr->first;
			fptr != (struct occi_element*) 0;
			fptr = fptr->next )
		{
			if (!( fptr->name ))
				continue;
			else if (!( fptr->value ))
				continue;
			else if (!( sptr = occi_category_id( fptr->value ) ))
				continue;

			/* ------------------------------------------------------ */
			/*  scan the returned atributs to localise the category   */
			/* ------------------------------------------------------ */
			sprintf(buffer,"%s/%s/%s",eptr->value,_CORDS_PROVIDER,sptr);
			sptr = liberate( sptr );
			if (!( xptr = occi_simple_get( buffer, agent,tls ) ))
				continue;

			sprintf(buffer,"occi.%s.",_CORDS_PROVIDER);
			for (	gptr = xptr->first;
				gptr != (struct occi_element*) 0;
				gptr = gptr->next )
			{
				if (!( sptr = gptr->name ))
					continue;
				else if ( strncmp( sptr, buffer, strlen( buffer ) ) != 0 )
					continue;
				else 	sptr += strlen( buffer );
				if ( strcmp( sptr, _CORDS_CATEGORY ) != 0)
					continue;
				else
				{
					sptr = occi_unquoted_value( gptr->value );
					if (!(sptr = cords_accept_provider( sptr, xptr  ) ))
						continue;
					else
					{
						xptr = occi_remove_response( xptr );
						yptr = occi_remove_response( yptr );
						zptr = occi_remove_response( zptr );
						if ( check_verbose() )
							printf("   Cords Resolved Provider : %s \n",sptr);
						return( sptr );
					}
				}
			}
			xptr = occi_remove_response( xptr );
			continue;
		}				
		yptr = occi_remove_response( yptr );
		continue;
	}		
	zptr = occi_remove_response( zptr );
	if ( check_verbose() )
		printf("   Cords Default Provider : %s \n",defaut);
	return( allocate_string( defaut ) );
}

/*	-------------------------------------------------------		*/
/*		c o r d s _ r e s o l v e _ p r o v i d e r		*/
/*	-------------------------------------------------------		*/
private	char *	cords_resolve_provider( struct occi_response * node, char * defaut, char * agent,char * tls )
{
	char	*	value;
	/* ----------------------------------------- */
	/* retrieve the provider information instance */
	/* ----------------------------------------- */
	if (!( value = cords_extract_atribut( node, "occi", _CORDS_NODE, _CORDS_PROVIDER ))) 
		return( cords_research_provider(defaut,agent,tls) );
	else if (!( strcmp(value,_CORDS_NULL) ))
		return( cords_research_provider(defaut,agent,tls) );
	else if (!( strcmp(value,_CORDS_ANY) ))
		return( cords_research_provider(defaut,agent,tls) );
	else
	{
		if ( check_verbose() )
			printf("   Cords Explicite Provider : %s \n",value);
		return( value );
	}
}

/*	-------------------------------------------------------		*/
/*		c o r d s _ r e s o l v e _ p r o f i l e		*/
/*	-------------------------------------------------------		*/
private	char *	cords_resolve_profile( struct occi_response * node, char * defaut )
{
	char	*	value;
	/* ----------------------------------------- */
	/* retrieve the provider information instance */
	/* ----------------------------------------- */
	if (!( value = cords_extract_atribut( node, "occi", _CORDS_NODE, _CORDS_PROFILE ))) 
		return( allocate_string( defaut ) );
	else if (!( strcmp(value,_CORDS_NULL) ))
		return( allocate_string( defaut ) );
	else	return( value );
}


/*	-------------------------------------------------------		*/
/*		c o r d s _ b u i l d _ c o n t r a c t  		*/
/*	-------------------------------------------------------		*/
/*	builds an XML document form of a contract instance of a		*/
/*	cords request/plan.						*/
/*	-------------------------------------------------------		*/
public	struct	xml_element * 	cords_build_contract( 
		char * node, 	char * name, 
		char * provider )
{
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;

	if (!( eptr = allocate_element()))
		return((struct xml_element *) 0);
	else if (!( eptr->name = allocate_string( _CORDS_CONTRACT ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_NAME, name ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_NODE, node ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_SERVICE, "" ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_REFERENCE, "" ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_HOSTNAME, "" ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_ROOTPASS, "" ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_PROFILE, provider ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_TARIFICATION, "" ) ))
		return(document_drop( eptr ));
	else	return( eptr );
}

/*	----------------------------------------------------------	*/
/*		c o r d s _ i n s t a n c e _ s e r v i c e		*/
/*	----------------------------------------------------------	*/
/*	builds a nested service instance graph for complex nodes	*/
/*	----------------------------------------------------------	*/
private	char *	cords_instance_service( char * host, char * planid, char * agent, char * tls, struct xml_element ** root )
{
	struct	occi_response * zptr=(struct occi_response *) 0;
	char *	service=(char *) 0;
	char *	manifest=(char *) 0;
	char *	plan=(char *) 0;
	char *	name=(char *) 0;
	char *	sptr;

	/* --------------------------------------- */
	/* retrieve the provisioning plan instance */
	/* --------------------------------------- */
	if (!( plan = occi_unquoted_value( planid ) ))
		service = (char *) 0;
	else if (!( zptr = cords_retrieve_instance( host, plan, agent, tls )))
		service = (char *) 0;
	else if (!( name = cords_extract_atribut( zptr, "occi", _CORDS_PLAN, _CORDS_NAME ) ))
		service = (char *) 0;
	else if (!( manifest = cords_extract_atribut( zptr, "occi", _CORDS_PLAN, _CORDS_MANIFEST ) ))
		service = (char *) 0;
	else	service = cords_manifest_broker( host, plan, name, manifest, agent, tls, root );

	/* ----------------------------- */
	/* liberate response and strings */
	/* ----------------------------- */
	if ( zptr ) 	zptr = occi_remove_response (zptr );
	if ( name )	name = liberate( name );
	if ( manifest )	manifest = liberate( manifest );
	if ( plan )	plan = liberate( plan );

	return( service );
}

/*	------------------------------------------------------------	*/
/*	c o r d s _ i n s t a n c e _ c o m p l e x _c o n t r a c t	*/
/*	------------------------------------------------------------	*/
private	struct	xml_element * 	cords_instance_complex_contract(
	struct cords_node_descriptor * App,
	char *	host,
	char *	id,
	char *	agent,
	char *	tls,
	char * namePlan )
{
	int	status;
	struct	xml_element 	*	xroot=(struct xml_element *) 0;
	struct	xml_element 	*	xptr;
	struct	xml_element 	* 	document=(struct xml_element *) 0;
	struct	xml_atribut	*	aptr;
	char 			*	service;

	/* ----------------------------------------------------------- */
	/* not a simple type node so instance a service graph for node */
	/* ----------------------------------------------------------- */
	if (!( service = cords_instance_service( host, App->typeApp, agent, tls, &xroot ) ))
	{
		cords_terminate_instance_node( App );
		return((struct xml_element *) 0);
	}
	/* --------------------------------------------------- */
	/* then create the contract document for the node here */
	/* --------------------------------------------------- */
	else if (!( document = cords_build_contract( id, App->nameApp, _CORDS_ANY ) ))
	{
		cords_terminate_instance_node( App );
		return((struct xml_element *) 0);
	}
	/* --------------------------------------------------- */
	/* now add the service instance identifier to contract */
	/* --------------------------------------------------- */
	else if (!( document_add_atribut( document, _CORDS_SERVICE, service ) ))
	{
		cords_terminate_instance_node( App );
		return(document_drop(document));
	}
	else 	document->first = document->last = xroot;

	return( document );

}

/*	------------------------------------------------------------	*/
/*		c o r d s _ n e g o c i a t e _ p r o v i d e r		*/
/*	------------------------------------------------------------	*/
private	char *	cords_negociate_provider( 
		struct cords_node_descriptor * App,
		struct xml_element * xptr,
		char *	agent,
		char * tls )
{
	/* ---------------------------------------------------- */
	/* this will use an algorithm to negociate the contract */
	/* ---------------------------------------------------- */
	/* TODO : But  for now ....				*/
	/* ---------------------------------------------------- */
	return( cords_create_provider( xptr, agent,tls ) );
}

/*	------------------------------------------------------------	*/
/*	c o r d s _ i n s t a n c e _ s i m p l e _ c o n t r a c t	*/
/*	------------------------------------------------------------	*/
private	struct	xml_element * 	cords_instance_simple_contract(
	struct cords_node_descriptor * App,
	char *	host,
	char *	id,
	char *	agent,
	char *	tls,
	char * namePlan )
{
	int	status;
	struct	xml_element 	*	xroot=(struct xml_element *) 0;
	struct	xml_element 	*	xptr;
	struct	xml_element 	* 	document=(struct xml_element *) 0;
	struct	xml_atribut	*	aptr;
	char 			*	service;

	if (!( document = cords_build_contract( id, App->nameApp, App->provider )))
	{
		cords_terminate_instance_node( App );
		return((struct xml_element *) 0);
	}
	else if (!( xptr = cords_add_provider( document, App->provider, App->nameApp, App->profile ) ))
	{
		cords_terminate_instance_node( App );
		return((struct xml_element *) 0);
	}
	else if (!( App->providerid = cords_negociate_provider( App, xptr, agent,tls ) ))
	{
		cords_terminate_instance_node( App );
		return((struct xml_element *) 0);
	}
	else if (!( aptr = document_add_atribut( xptr, _CORDS_ID, App->providerid ) ))
	{
		cords_terminate_instance_node( App );
		return((struct xml_element *) 0);
	}
	
	/* ------------------------------------ */
	/* retrieve the infrastructure instance */
	/* ------------------------------------ */
	if (( App->hid  = cords_extract_atribut( App->node, "occi", 
			_CORDS_NODE, _CORDS_INFRASTRUCTURE )) != (char *) 0)
	{
		if (!( App->hardware = cords_retrieve_instance( host, App->hid, agent,tls )))
		{
			cords_terminate_instance_node( App );
			return(document_drop(document));
		}
		else	App->hid = liberate( App->hid );

		/* retrieve the compute instance */
		/* ----------------------------- */
		if (( App->hid  = cords_extract_atribut( App->hardware, "occi", 
				_CORDS_INFRASTRUCTURE, _CORDS_COMPUTE )) != (char *) 0)
		{
			if (!( App->compute = cords_retrieve_instance( host, App->hid, agent,tls )))
			{
				cords_terminate_instance_node( App );
				return(document_drop(document));
			}
			else	App->hid = liberate( App->hid );
		}
		else	App->compute = (struct occi_response *) 0;

		/* retrieve the storage instance */
		/* ----------------------------- */
		if (( App->hid  = cords_extract_atribut( App->hardware, "occi", 
				_CORDS_INFRASTRUCTURE, _CORDS_STORAGE )) != (char *) 0)
		{
			if (!( App->storage = cords_retrieve_instance( host, App->hid, agent,tls )))
			{
				cords_terminate_instance_node( App );
				return(document_drop(document));
			}
			else	App->hid = liberate( App->hid );
		}
		else	App->storage = (struct occi_response *) 0;

		/* retrieve the network instance */
		/* ----------------------------- */
		if (( App->hid  = cords_extract_atribut( App->hardware, "occi", 
				_CORDS_INFRASTRUCTURE, _CORDS_NETWORK )) != (char *) 0)
		{
			if (!( App->network = cords_retrieve_instance( host, App->hid, agent,tls )))
			{
				cords_terminate_instance_node( App );
				return(document_drop(document));
			}
			else	App->hid = liberate( App->hid );
		}
		else	App->network = (struct occi_response *) 0;
	}
	else	App->hardware = (struct occi_response *) 0;

	/* ------------------------------------ */
	/* retrieve the infrastructure instance */
	/* ------------------------------------ */
	if (( App->sid  = cords_extract_atribut( App->node, "occi", 
			_CORDS_NODE, _CORDS_IMAGE )) != (char *) 0)
	{
		if (!( App->software = cords_retrieve_instance( host, App->sid, agent,tls )))
		{
			cords_terminate_instance_node( App );
			return(document_drop(document));
		}
		else	App->sid = liberate( App->sid );

		if (( App->sid  = cords_extract_atribut( App->software, "occi", 
				_CORDS_IMAGE, _CORDS_SYSTEM )) != (char *) 0)
		{
			if (!( App->system = cords_retrieve_instance( host, App->sid, agent,tls )))
			{
				cords_terminate_instance_node( App );
				return(document_drop(document));
			}
			else	App->sid = liberate( App->sid );
		}
		else	App->system = (struct occi_response *) 0;
	}
	else	App->software = (struct occi_response *) 0;

	return( document );
}

/*	------------------------------------------------------------------------------	*/
/*	c o r d s _ t e r m i n a t e _ p r i v a t e _ c o m m o n  _ c o n t r a c t	*/
/*	------------------------------------------------------------------------------	*/
/*	used by private common contract handlers for the creation of the node element	*/
/*	referencing the common instance for use by the parent service contract		*/
/*	------------------------------------------------------------------------------	*/
private	struct	xml_element * cords_terminate_private_common_contract(
	struct 	cords_node_descriptor * App,
	char *	id,
	char *	agent,
	char *	tls,
	char * common )
{
	struct	occi_response * yptr;
	struct	xml_element * document;
	struct	xml_atribut * aptr;

	if (!( document = cords_build_contract( id, App->nameApp, App->provider )))
	{
		cords_terminate_instance_node( App );
		return((struct xml_element *) 0);
	}
	else if (!( aptr = document_add_atribut( document, _CORDS_COMMON, common ) ))
	{
		cords_terminate_instance_node( App );
		return(document_drop(document));
	}
	else if (!( yptr = cords_update_common_node( id, App->node, common, agent, tls ) ))
	{
		cords_terminate_instance_node( App );
		return(document_drop(document));
	}
	else
	{
		yptr = occi_remove_response( yptr );
		return( document );
	}
}

/*	----------------------------------------------------------------------------	*/
/*		    c o r d s _ u p d a t e _ p u b l i c _ c o m m o n			*/
/*	----------------------------------------------------------------------------	*/
private	struct	occi_response * cords_update_public_common(
	struct cords_node_descriptor * App,
	char *	id,
	char *	common,
	char *	agent,
	char *	tls )
{
	char	*	ihost;
	struct	occi_client * kptr;
	struct	occi_request * qptr;
	struct	occi_response * yptr;
	struct	occi_response * zptr;
	struct	occi_element * dptr;
	struct	xml_element * eptr;
	char	buffer[2048];

	if (!( ihost = rest_log_comons_identity(_CORDS_INSTANCE,agent,tls) ))
		return((struct occi_response *) 0);

	sprintf(buffer,"%s/%s/",ihost,_CORDS_INSTANCE);

	liberate( ihost );

	if (!( kptr = occi_create_client( buffer, agent, tls ) ))
		return((struct occi_response *) 0);
	else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
	{
		kptr = occi_remove_client( kptr );
		return((struct occi_response *) 0);
	}
	else if ((!(dptr=occi_request_element(qptr,"occi.instruction.name"        , App->nameApp) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.common"      , common      ) )))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return((struct occi_response *) 0);
	}
	else if (!( yptr = occi_client_post( kptr, qptr ) ))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return((struct occi_response *) 0);
	}
	else
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return(yptr);
	}
}

/*	----------------------------------------------------------------------------	*/
/*	  	  c o r d s _ r e s o l v e _ p u b l i c _ c o m m o n			*/
/*	----------------------------------------------------------------------------	*/
private	char * cords_resolve_public_common(
	struct cords_node_descriptor * App,
	char *	host,
	char *	id,
	char *	agent,
	char *	tls,
	int  *	idptr )
{
	struct	occi_response * yptr;
	struct	occi_response * uptr;
	struct	occi_element  * eptr;
	char *	location=(char *) 0;
	char *	common=(char *) 0;
	*idptr = 0;
	if (!( yptr = cords_retrieve_named_instance_list( _CORDS_INSTANCE, "occi.instance.name", App->nameApp, agent, tls ) ))
		return( (char *) 0 );

	else if (!( uptr = cords_retrieve_named_instance( yptr, agent, tls )))
	{
		yptr = occi_remove_response ( yptr );
		return( (char *) 0 );
	}
	else
	{
		yptr = occi_remove_response ( yptr );
		for (	eptr = uptr->first;
			eptr != (struct occi_element *) 0;
			eptr = eptr->next )
		{
			if (!( strcmp( eptr->name, "occi.instance.common"  )))
			{
				if (!( common = allocate_string( eptr->value ) ))
					return((char *) 0 );
				else	break;
			}
		}
		uptr = occi_remove_response ( uptr );
		if ( common ) *idptr = 1;
		return( common );
	}
}

/*	------------------------------------------------------------------------------	*/
/*	 c o r d s _ t e r m i n a t e _ p u b l i c _ c o m m o n  _ c o n t r a c t	*/
/*	------------------------------------------------------------------------------	*/
/*	used by private common contract handlers for the creation of the node element	*/
/*	referencing the common instance for use by the parent service contract		*/
/*	------------------------------------------------------------------------------	*/
private	struct	xml_element * cords_terminate_public_common_contract(
	struct 	cords_node_descriptor * App,
	char *	id,
	char *	agent,
	char *	tls,
	int	location,
	char *	common )
{
	struct	occi_response * yptr;
	struct	xml_element * document;
	struct	xml_atribut * aptr;

	if (!( document = cords_build_contract( id, App->nameApp, App->provider )))
	{
		liberate( common );
		cords_terminate_instance_node( App );
		return((struct xml_element *) 0);
	}
	else if (!( aptr = document_add_atribut( document, _CORDS_COMMON, common ) ))
	{
		liberate( common );
		cords_terminate_instance_node( App );
		return(document_drop(document));
	}
	else if ( location )
	{
		liberate( common );
		yptr = occi_remove_response( yptr );
		return( document );
	}
 	else if (!( yptr = cords_update_public_common( App, id, common, agent, tls ) ))
	{
		liberate( common );
		cords_terminate_instance_node( App );
		return(document_drop(document));
	}
 	else
	{
		liberate( common );
		yptr = occi_remove_response( yptr );
		return( document );
	}
}

/*	----------------------------------------------------------------------------	*/
/*		      c o r d s _ b u i l d _ c o m p l e x _ c o m m o n		*/
/*	----------------------------------------------------------------------------	*/
private	char * 	cords_build_simple_common(
	struct cords_node_descriptor * App,
	char *	host,
	char *	id,
	char *	agent,
	char *	tls,
	char * namePlan )
{
	struct	xml_element * document;
	struct	xml_atribut * aptr;
	char *	common;
	/* ----------------------------------- */
	/* build a new simple service contract */
	/* ----------------------------------- */
	if (!( document = cords_instance_simple_contract( App, host, id, agent, tls, namePlan ) ))
		return((char *) 0);

	else if (!( document = cords_complete_contract( App, document, agent, tls ) ))
	{
		cords_terminate_instance_node( App );
		return((char *) 0);
	}
	/* ------------------ */
	/* recover identifier */
	/* ------------------ */
	else if (!( aptr = document_atribut( document, _CORDS_ID ) ))
	{
		cords_terminate_instance_node( App );
		document = document_drop( document );
		return((char *) 0);
	}
	else if (!( common = allocate_string( aptr->value ) ))
	{
		cords_terminate_instance_node( App );
		document = document_drop( document );
		return((char *) 0);
	}
	else
	{
		document = document_drop( document );
		return( common );
	}
}

/*	--------------------------------------------------------------------------	*/
/*	c o r d s _ s i m p l e _ p r i v a t e _ c o m m o n  _ c o n t r a c t	*/
/*	--------------------------------------------------------------------------	*/
private	struct	xml_element * 	cords_simple_private_common_contract(
	struct cords_node_descriptor * App,
	char *	host,
	char *	id,
	char *	agent,
	char *	tls,
	char * namePlan )
{
	struct	xml_element * document;
	struct	xml_atribut * aptr;
	char *	common;

	/* ------------------------------------------ */
	/* retrieve the common instance from the node */
	/* ------------------------------------------ */
	if (!( common = cords_extract_atribut(App->node,"occi",_CORDS_NODE,_CORDS_COMMON)))
		if (!( common = cords_build_simple_common( App, host, id, agent, tls, namePlan ) ))
			return( (struct xml_element *) 0 );

	return( cords_terminate_private_common_contract( App, id, agent, tls, common ) );
}

/*	----------------------------------------------------------------------------	*/
/*		      c o r d s _ b u i l d _ c o m p l e x _ c o m m o n		*/
/*	----------------------------------------------------------------------------	*/
private	char *	cords_build_complex_common(
	struct cords_node_descriptor * App,
	char *	host,
	char *	id,
	char *	agent,
	char *	tls,
	char * namePlan )
{
	struct	xml_element * document;
	struct	xml_atribut * aptr;
	char *	common;

	/* ----------------------------------- */
	/* build a new simple service contract */
	/* ----------------------------------- */
	if (!( document = cords_instance_complex_contract( App, host, id, agent, tls, namePlan ) ))
		return((char *) 0);

	else if (!( document = cords_complete_contract( App, document, agent, tls ) ))
	{
		cords_terminate_instance_node( App );
		return((char *) 0);
	}

	/* ------------------ */
	/* recover identifier */
	/* ------------------ */
	else if (!( aptr = document_atribut( document, _CORDS_ID ) ))
	{
		cords_terminate_instance_node( App );
		document = document_drop( document );
		return((char *) 0);
	}
	else if (!( common = allocate_string( aptr->value ) ))
	{
		cords_terminate_instance_node( App );
		document = document_drop( document );
		return((char *) 0);
	}
	else
	{
		document = document_drop( document );
		return( common );
	}
}

/*	----------------------------------------------------------------------------	*/
/*	c o r d s _ c o m p l e x _ p r i v a t e _ c o m m o n  _ c o n t r a c t	*/
/*	----------------------------------------------------------------------------	*/
private	struct	xml_element * 	cords_complex_private_common_contract(
	struct cords_node_descriptor * App,
	char *	host,
	char *	id,
	char *	agent,
	char *	tls,
	char * namePlan )
{
	struct	xml_element * document;
	struct	xml_atribut * aptr;
	char *	common;
	/* ------------------------------------------ */
	/* retrieve the common instance from the node */
	/* ------------------------------------------ */
	if (!( common = cords_extract_atribut(App->node,"occi",_CORDS_NODE,_CORDS_COMMON)))
		if (!( common = cords_build_complex_common( App, host, id, agent, tls, namePlan ) ))
			return( (struct xml_element *) 0 );

	return( cords_terminate_private_common_contract( App, id, agent, tls, common ) );
}

/*	----------------------------------------------------------------------------	*/
/*	  c o r d s _ c o m p l e x _ p u b l i c _ c o m m o n  _ c o n t r a c t	*/
/*	----------------------------------------------------------------------------	*/
private	struct	xml_element * 	cords_complex_public_common_contract(
	struct cords_node_descriptor * App,
	char *	host,
	char *	id,
	char *	agent,
	char *	tls,
	char * 	namePlan )
{
	char * 	common=(char *) 0;
	int	location=0;
	if (!( common = cords_resolve_public_common(App,host,id,agent,tls,&location) ))
	{
		if (!( common = cords_build_complex_common( App, host, id, agent, tls, namePlan ) ))
			return( (struct xml_element *) 0 );
		else if (!( common = allocate_string( common ) ))
			return( (struct xml_element *) 0 );
	}
	return( cords_terminate_public_common_contract( App, id, agent, tls, location, common ) );
}

/*	----------------------------------------------------------------------------	*/
/*	   c o r d s _ s i m p l e _ p u b l i c _ c o m m o n  _ c o n t r a c t	*/
/*	----------------------------------------------------------------------------	*/
private	struct	xml_element * 	cords_simple_public_common_contract(
	struct cords_node_descriptor * App,
	char *	host,
	char *	id,
	char *	agent,
	char *	tls,
	char * namePlan )
{
	char * 	common=(char *) 0;
	int	location=0;
	if (!( common = cords_resolve_public_common(App,host,id,agent,tls,&location) ))
	{
		if (!( common = cords_build_simple_common( App, host, id, agent, tls, namePlan ) ))
			return( (struct xml_element *) 0 );
		else if (!( common = allocate_string( common ) ))
			return( (struct xml_element *) 0 );
	}
	return( cords_terminate_public_common_contract( App, id, agent, tls, location, common ) );
}

/*	------------------------------------------------------------	*/
/*		c o r d s _ c o m p l e t e _ c o n t r a c t		*/
/*	------------------------------------------------------------	*/
private	struct	xml_element * 	cords_complete_contract(
	struct 	cords_node_descriptor * App,
	struct	xml_element * document,
	char *	agent,
	char *	tls )
{
	int	status;
	/* -------------------------------------------------- */
	/* Join the common trunk for Simple and Complex Nodes */
	/* -------------------------------------------------- */
	if (!( document_add_atribut( document, _CORDS_TYPE, App->typeApp ) ))
		return(document_drop(document));

	/* -------------------------------------- */
	/* Ensure the Access Atribute is in place */
	/* -------------------------------------- */
	else if (!( document_add_atribut( document, _CORDS_ACCESS,
		 ( App->scope & _ACCESS_PUBLIC ? _CORDS_PUBLIC : _CORDS_PRIVATE ) ) ))
		return(document_drop(document));

	/* ------------------------------------- */
	/* Ensure the Scope Atribute is in place */
	/* ------------------------------------- */
	else if (!( document_add_atribut( document, _CORDS_SCOPE, 
		 ( App->scope & _SCOPE_COMMON ? _CORDS_COMMON : _CORDS_NORMAL ) ) ))
		return(document_drop(document));

	/* -------------------------------- */
	/* build the OCCI contract instance */
	/* -------------------------------- */
	else if (!( App->contract = cords_create_category( document, agent,tls ) ))
		return(document_drop(document));

	/* ----------------------------------------- */
	/* recover the resulting contract identifier */
	/* ----------------------------------------- */
	else if ((status = cords_resolve_location( App->contract, document )) != 0)
		return(document_drop(document));

	/* ----------------------------------------- */
	/* terminate the service contract processing */
	/* ----------------------------------------- */
	else if ((status = cords_terminate_level( document, agent,tls )) != 0)
		return(document_drop(document));

	/* ------------------------------------------- */
	/* successful completion returnes the document */
	/* ------------------------------------------- */
	else	return( document );
}

/*	------------------------------------------------------------	*/
/*		c o r d s _ i n s t a n c e _ c o n t r a c t		*/
/*	------------------------------------------------------------	*/
private	struct	xml_element * 	cords_instance_contract(
	struct cords_node_descriptor * App,
	char *	host,
	char *	id,
	char *	agent,
	char *	tls,
	char * namePlan )
{
	/* ----------------------------------------------------------------- */
	/* detect and handle simple type nodes and build a provider contract */
	/* ----------------------------------------------------------------- */
	if (!( strcmp( App->typeApp, _CORDS_SIMPLE ) ))
	{
		if (!( App->scope & _SCOPE_COMMON ))
			return( cords_instance_simple_contract( App, host, id, agent, tls, namePlan ) );
		else if (!( App->scope & _ACCESS_PRIVATE ))
			return( cords_simple_private_common_contract( App, host, id, agent, tls, namePlan ) );
		else	return( cords_simple_public_common_contract( App, host, id, agent, tls, namePlan ) );
	}
	else
	{
		if (!( App->scope & _SCOPE_COMMON ))
			return( cords_instance_complex_contract( App, host, id, agent, tls, namePlan ) );
		else if (!( App->scope & _ACCESS_PRIVATE ))
			return( cords_complex_private_common_contract( App, host, id, agent, tls, namePlan ) );
		else	return( cords_complex_public_common_contract( App, host, id, agent, tls, namePlan ) );
	}
}

/*	-------------------------------------------------------		*/
/*	   c o r d s _ i n s t a n c e _ a p p l i a n c e		*/
/*	-------------------------------------------------------		*/
/*	here we instance a node and create a contract instance		*/
/*	that will ne added to the service of the parent manifest	*/
/*	-------------------------------------------------------		*/
private	struct	xml_element * cords_instance_node( 
		char * host,
		char * id,
		char * agent,
		char * tls,
		char * namePlan )
{
	int	status;
	struct	xml_element 	*	xroot=(struct xml_element *) 0;
	struct	xml_element 	*	xptr;
	struct	xml_element 	* 	document=(struct xml_element *) 0;
	struct	xml_atribut	*	aptr;
	char 			*	service;

	struct	cords_node_descriptor App = {
		_SCOPE_NORMAL | _ACCESS_PRIVATE,
		(char *) 0,
		(char *) 0,
		(char *) 0,
		(char *) 0,
		(char *) 0,
		(char *) 0,
		(char *) 0,
		(char *) 0,
		(char *) 0,
		(struct	occi_response *) 0, 
		(struct	occi_response *) 0, 
		(struct	occi_response *) 0, 
		(struct	occi_response *) 0, 
		(struct	occi_response *) 0, 
		(struct	occi_response *) 0, 
		(struct	occi_response *) 0, 
		(struct	occi_response *) 0,
		(struct	occi_response *) 0
	};

	if ( check_verbose() )	printf("   CORDS Node \n");

	/* ------------------------------- */
	/* retrieve the node instance */
	/* ------------------------------- */
	if (!( App.node = cords_retrieve_instance( host, id, agent,tls)))
		return((struct xml_element *) 0);

	/* ------------------------------------------ */
	/* retrieve the node name and type attributes */
	/* ------------------------------------------ */
	else if (!(App.nameApp = cords_extract_atribut(App.node,"occi",_CORDS_NODE,_CORDS_NAME)))
	{
		cords_terminate_instance_node( &App );
		return((struct xml_element *) 0);
	}
	else if (!(App.typeApp = cords_extract_atribut(App.node,"occi",_CORDS_NODE,_CORDS_TYPE)))
	{
		if (!(App.typeApp = allocate_string(_CORDS_SIMPLE)))
		{
			cords_terminate_instance_node( &App );
			return((struct xml_element *) 0);
		}
	}
	/* --------------------------------------------- */
	/* ensure default simple type if no type present */
	/* --------------------------------------------- */
	else if (!( strcmp(App.typeApp, _CORDS_NULL )))
	{
		if (!(App.typeApp = allocate_string(_CORDS_SIMPLE)))
		{
			cords_terminate_instance_node( &App );
			return((struct xml_element *) 0);
		}
	}
	/* -------------------------------- */
	/* recover provider information now */
	/* -------------------------------- */
	else if (!( App.provider = cords_resolve_provider( App.node, Operator.provider, agent,tls )))
	{
		cords_terminate_instance_node( &App );
		return((struct xml_element *) 0);
	}
	else if (!( App.profile = cords_resolve_profile( App.node, namePlan )))
	{
		cords_terminate_instance_node( &App );
		return((struct xml_element *) 0);
	}

	/* ----------------------------------------------------- */
	/* check for scope COMMON and then access PUBLIC/PRIVATE */
	/* ----------------------------------------------------- */
	if ((App.scopeApp = cords_extract_atribut(App.node,"occi",_CORDS_NODE,_CORDS_SCOPE)) != (char *) 0)
	{
		if (!( strcasecmp( App.scopeApp, _CORDS_COMMON )))
			App.scope = _SCOPE_COMMON;
		else if (!( strcasecmp( App.scopeApp, _CORDS_NORMAL )))
			App.scope = _SCOPE_NORMAL;
		else	App.scope = _SCOPE_NORMAL;
		
		if ((App.accessApp = cords_extract_atribut(App.node,"occi",_CORDS_NODE,_CORDS_ACCESS)) != (char *) 0)
		{
			if (!( strcasecmp( App.accessApp, _CORDS_PUBLIC )))
				App.scope |= _ACCESS_PUBLIC;
			else	App.scope |= _ACCESS_PRIVATE;
		}
		else	App.scope |= _ACCESS_PRIVATE;
	}

	/* ---------------------------------------------------- */
	/* instance or share the service contract for this node */
	/* ---------------------------------------------------- */
	if (!( document = cords_instance_contract( &App, host, id, agent, tls, namePlan ) ))
		return( document );
	else 
	{
		document = cords_complete_contract( &App, document, agent, tls );
		cords_terminate_instance_node( &App );
		return(document);
	}		
}


/*	-------------------------------------------------------		*/
/*		c o r d s _ b u i l d _ s e r v i c e 			*/
/*	-------------------------------------------------------		*/
/*	builds an XML document form of a provisioning instance		*/
/*	of a cords request/plan.					*/
/*	-------------------------------------------------------		*/
public	struct	xml_element * 	cords_build_service(
	char * plan, 	char * name, 
	char * manifest,
	char * account,	char * tarif)
{
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;

	if (!( eptr = allocate_element()))
		return((struct xml_element *) 0);
	else if (!( eptr->name = allocate_string( _CORDS_SERVICE ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_NAME , name ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_PLAN , plan ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_MANIFEST , manifest ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_ACCOUNT , account ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_TARIFICATION , tarif ) ))
		return(document_drop( eptr ));
	else	return( eptr );
}

/*	-------------------------------------------------------		*/
/*		c o r d s _ i n s t a n c e _ p l a n			*/
/*	-------------------------------------------------------		*/
/*	the final step on this long and winding road. here we		*/
/*	record the new plan instance identifier as a link from		*/
/*	the parent plan to the newly created instance instance		*/
/*	we then issue the action "start" to commence deployment		*/
/*	and configuration of the required infrastructure.		*/
/*	-------------------------------------------------------		*/
private	int	cords_instance_plan( 
			struct xml_element * document, 
			char * plan, char * instance,
			char * agent,
			char * tls )
{
	struct	xml_atribut *	aptr;
	struct	occi_response * zptr;
	if (!( zptr =  cords_create_link( plan, instance, agent,tls ) ))
		return(914);
	else if (!( zptr =  cords_invoke_action( instance, _CORDS_START, agent,tls ) ))
		return(915);
	else if (!( aptr = document_add_atribut( document, "instance", instance )))
		return(916);
	else	return(0);
}

/*	-------------------------------------------------------		*/
/*	      c o r d s _ r e c o v e r _ r e s u l t s			*/
/*	-------------------------------------------------------		*/
private	int	cords_recover_results(struct xml_element * document, char * agent, char * tls )
{
	struct	xml_element   * eptr;
	struct	occi_element  * fptr;
	struct	xml_atribut   * aptr;
	struct	xml_atribut   * bptr;
	struct	occi_response * zptr;
	char *	sptr;
	char *	prefix = "occi.contract.";
	if (!( document ))
		return(118);
	else if (!( document->name ))
		return(118);
	else if ( strcmp( document->name, _CORDS_CONTRACT ) != 0)
		return( 56 );
	else
	{
		for (	eptr=document->first;
			eptr != (struct xml_element *) 0;
			eptr = eptr->next )
		{
			if (!( eptr->name ))
				continue;
			else if ( strcmp( eptr->name, _CORDS_CONTRACT ) != 0 )
				continue;
			else if (!( aptr = document_atribut( eptr, _CORDS_ID ) ))
				continue;
			else if (!( zptr = occi_simple_get( aptr->value, agent, tls ) ))
				continue;
			for (	fptr = zptr->first;
				fptr != (struct occi_element *) 0;
				fptr = fptr->next )
			{
				if (!( sptr = fptr->name ))
					continue;
				else if ( strncmp( sptr, prefix, strlen(prefix) ) != 0 )
					continue;
				else	sptr += strlen( prefix );
				if ((!( strcmp( sptr,"hostname"  ) ))
				||  (!( strcmp( sptr,"rootpass"  ) ))
				||  (!( strcmp( sptr,"reference" ) )))
				{
					if (!( bptr = document_atribut( eptr, sptr ) ))
						continue;
					if ( bptr->value ) 
						bptr->value = liberate( bptr->value );
					if (!( bptr->value = allocate_string( fptr->value ) ))
						return( 27 );
				}
			}
			zptr = occi_remove_response ( zptr );
		}
	}
	return( 0 );
}


/*	-------------------------------------------------------		*/
/*		c o r d s _ r e q u e s t _ b r o k e r			*/
/*	-------------------------------------------------------		*/
public	char *	cords_manifest_broker(
	char * 	host, char * plan, char * nameplan, char * manifest, char * agent, char * tls, struct xml_element ** root )
{
	int	status;
	char	*	id;
	struct	occi_element * eptr;
	struct	xml_atribut * aptr;
	struct	xml_element  * mptr;
	struct cords_provisioning CbC = {
		(char *) 0,
		(char *) 0,
		(struct occi_response *) 0,
		(char *) 0,
		(struct occi_response *) 0,
		(char *) 0,
		(struct occi_response *) 0,
		(char *) 0,
		(struct occi_response *) 0,
		(char *) 0,
		(struct occi_response *) 0,
		(char *) 0,
		(struct occi_response *) 0,
		(struct xml_element *) 0,
		0
		};

	if ( check_verbose() )
		printf("   CORDS Request Broker ( %s ) Phase 1 : Preparation \n", agent);

	if (!( CbC.planID = allocate_string( plan )))
		return( cords_terminate_provisioning( 900, &CbC ) );
	else if (!( CbC.namePlan = allocate_string( nameplan ) ))
		return( cords_terminate_provisioning( 901, &CbC ) );
	else if (!( CbC.reqID  = allocate_string( manifest ) ))
		return( cords_terminate_provisioning( 902, &CbC ) );
	else if (!( CbC.manifest = cords_retrieve_instance( host, CbC.reqID, agent,tls )))
		return( cords_terminate_provisioning( 904, &CbC ) );

	/* ----------------------------------------- */
	/* retrieve the account information instance */
	/* ----------------------------------------- */
	if (( CbC.accID  = cords_extract_atribut( CbC.manifest, "occi", _CORDS_MANIFEST, _CORDS_ACCOUNT )) 
			!= (char *) 0)
	{
		if (!( CbC.account = cords_retrieve_instance( host, CbC.accID, agent,tls )))
			return( cords_terminate_provisioning( 905, &CbC ) );
	}

	/* ---------------------------------------- */
	/* retrieve the security procedure instance */
	/* ---------------------------------------- */
	if (( CbC.secID  = cords_extract_atribut( CbC.manifest, "occi", _CORDS_MANIFEST, _CORDS_SECURITY )) 
		!= (char *) 0)
	{
		if (!( CbC.security = cords_retrieve_instance( host, CbC.secID, agent,tls )))
			return( cords_terminate_provisioning( 906, &CbC ) );
	}

	/* ----------------------------------- */
	/* retrieve the configuration instance */
	/* ----------------------------------- */
	if (!( CbC.confID = cords_extract_atribut( CbC.manifest,"occi",_CORDS_MANIFEST,_CORDS_CONFIGURATION)))
		return( cords_terminate_provisioning( 907, &CbC ) );
	else if (!( CbC.configuration = cords_retrieve_instance( host, CbC.confID, agent,tls)))
		return( cords_terminate_provisioning( 908, &CbC ) );

	if ( check_verbose() )
		printf("   CORDS Request Broker ( %s ) Phase 2 : Provisioning \n",agent);

	if (!( CbC.document = cords_build_service( CbC.planID, CbC.namePlan, CbC.reqID, CbC.accID, CbC.accID ) ))
		return( cords_terminate_provisioning( 909, &CbC ) );

	else if (!( CbC.instance = cords_create_category( CbC.document, agent, tls ) ))
		return( cords_terminate_provisioning( 910, &CbC ) );

	else if ((status = cords_resolve_location( CbC.instance, CbC.document )) != 0)
		return( cords_terminate_provisioning( status, &CbC ) );
	else if (!( aptr = document_atribut( CbC.document, "id" )))
		return( cords_terminate_provisioning( 911, &CbC ) );
	else if (!( CbC.instID = occi_unquoted_value( aptr->value ) ))
		return( cords_terminate_provisioning( 912, &CbC ) );

	for (	eptr=cords_first_link( CbC.manifest );
		eptr != (struct occi_element *) 0;
		eptr = eptr->next )
	{
		if (!( eptr->value ))
			continue;
		if (!( id =  occi_unquoted_link( eptr->value ) ))
			continue;
		else if (!( mptr = cords_instance_node( host, id, agent,tls, CbC.namePlan ) ))
			return( cords_terminate_provisioning( 913, &CbC ) );
		else	
		{
			/* ----------------------- */
			/* add to list of contracts */
			/* ----------------------- */
			if (!( mptr->previous = CbC.document->last ))
				CbC.document->first = mptr;
			else 	mptr->previous->next = mptr;
			CbC.document->last = mptr;
			id = liberate( id );
			CbC.nodes++;
			continue;
		}
	}

	/* ----------------------------------- */
	/* terminate the service element level */
	/* ----------------------------------- */
	(void) cords_terminate_level( CbC.document, agent,tls );

	if ( check_verbose() )
		printf("   CORDS Request Broker ( %s ) Provisioned %u Nodes \n",agent,CbC.nodes);

	if ( check_verbose() )
		printf("   CORDS Request Broker ( %s ) Phase 3 : Configuration \n",agent);

	/* -------------------------------------------- */
	/* perform configuration instruction processing */
	/* -------------------------------------------- */
	if (( status = cords_broker_configuration( host, CbC.document, CbC.configuration, agent,tls )) != 0)
		return( cords_terminate_provisioning( status, &CbC ) );
	
	/* ------------------------------------ */
	/* check if document return is required */
	/* ------------------------------------ */
	if ( root )
	{
		*root = CbC.document;
		CbC.document = (struct xml_element *) 0;
	}

	return( cords_terminate_provisioning( 0, &CbC ) );
}

	/* --------------- */
#endif	/* _cords_broker_c */
	/* --------------- */


