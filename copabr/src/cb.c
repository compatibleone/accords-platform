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
#ifndef	_cords_broker_c
#define	_cords_broker_c

#include "cb.h"
#include "csp.c"
#include "restlog.h"
#include "cosacsctrl.c"
struct	operator_preferences
{
	int	negotiate;
	char *	provider;
	char *	account;
	char * 	pricelist;
	char *	domain;
} Operator = { 
	_BROKER_PROVIDER_CONTRACT,
	"openstack",
	"accords",
	"standard",
	"occi"
	};

private	struct	xml_element * 	cords_instance_contract(
	struct cords_node_descriptor * App,
	char *	host,
	char *	id,
	char *	agent,
	char *	tls,
	char *	sla,
	char * namePlan );

private	struct	xml_element * 	cords_complete_contract(
	struct 	cords_node_descriptor * App,
	struct	xml_element * document,
	char *	agent,
	char *	tls );

private	int	provisioning_status=0;


/*	---------------------------------------------------------------		*/
/*	c o r d s _ i n i t i a l i s e _ b r o k e r _ o p e r a t o r 	*/
/*	---------------------------------------------------------------		*/
public	int	cords_initialise_broker_operator( int mode, char * provider, char * account, char * pricelist, char * domain )
{
	Operator.negotiate = mode;
	if ( provider )
		Operator.provider = allocate_string( provider );
	if ( account )
		Operator.account = allocate_string( account );
	if ( pricelist )
		Operator.pricelist = allocate_string( pricelist );
	if ( domain )
		Operator.domain = allocate_string( domain );
	return(0);
}

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

/*	--------------------------------------------------------	*/
/*	l i b e r a t e _ p l a c e m e n t _ c o n d i t i o n		*/
/*	--------------------------------------------------------	*/
private	void	liberate_placement_conditions(
		struct cords_placement_criteria * placement )
{
	if ( placement )
	{
		if ( placement->algorithm )
			placement->algorithm = liberate( placement->algorithm );
		if ( placement->operator )
			placement->operator = liberate( placement->operator );
		if ( placement->zone )
			placement->zone = liberate( placement->zone );
		if ( placement->opinion )
			placement->opinion = liberate( placement->opinion );
		if ( placement->energy )
			placement->energy = liberate( placement->energy );
		if ( placement->security )
			placement->security = liberate( placement->security );
		if ( placement->price )
			placement->price = liberate( placement->price );
	}
	return;
}

/*	----------------------------------------------------------	*/
/*	   l i b e r t e _ g u a r a n t e e _ e l e m e n t 		*/
/*	----------------------------------------------------------	*/
private	struct cords_guarantee_element * liberate_guarantee_element( struct cords_guarantee_element * eptr )
{
	if ( eptr )
	{
		if ( eptr->reference )
			eptr->reference = liberate( eptr->reference );
		if ( eptr->obligated )
			eptr->obligated = liberate( eptr->obligated );
		if ( eptr->condition )
			eptr->condition = liberate( eptr->condition );
		if ( eptr->property )
			eptr->property = liberate( eptr->property );
		if ( eptr->objective )
			eptr->objective = liberate( eptr->objective );
		if ( eptr->scope )
			eptr->scope = liberate( eptr->scope );
		if ( eptr->importance )
			eptr->importance = liberate( eptr->importance );
		eptr = liberate( eptr );
	}
	return((struct cords_guarantee_element *) 0);
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
	struct cords_guarantee_element * gptr;

	if ( pptr->confID )
		pptr->confID = liberate( pptr->confID );
	if ( pptr->configuration )
		pptr->configuration = occi_remove_response( pptr->configuration );
	if ( pptr->interID )
		pptr->interID = liberate( pptr->interID );
	if ( pptr->interface )
		pptr->interface = occi_remove_response( pptr->interface );
	if ( pptr->releaseID )
		pptr->releaseID = liberate( pptr->releaseID );
	if ( pptr->release )
		pptr->release = occi_remove_response( pptr->release );
	if ( pptr->secID )
		pptr->secID = liberate( pptr->secID );
	if ( pptr->security )
		pptr->security = occi_remove_response( pptr->security );
	if ( pptr->accID )
		pptr->accID = liberate( pptr->accID );
	if ( pptr->accName )
		pptr->accName = liberate( pptr->accName );
	if ( pptr->account )
		pptr->account = occi_remove_response( pptr->account );
	if ( pptr->reqID )
		pptr->reqID = liberate( pptr->reqID );
	if ( pptr->manifest )
		pptr->manifest = occi_remove_response( pptr->manifest );
	if ( pptr->slaID )
		pptr->slaID = liberate( pptr->slaID );
	if ( pptr->sla )
		pptr->sla = occi_remove_response( pptr->sla );
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
	
	if ( pptr->placement )
		liberate_placement_conditions( pptr->placement );

	if ( pptr->warranty )
	{
		while ((gptr = pptr->warranty->first) != (struct cords_guarantee_element *) 0)
		{
			pptr->warranty->first = gptr->next;
			liberate_guarantee_element( gptr );
		}
		pptr->warranty->last = pptr->warranty->first;
	}

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
/*	  c o r d s _ r e s o l v e _ c o n s u m e r _ i d 		*/
/*	-------------------------------------------------------		*/
private	char *	cords_resolve_consumer_id( char * name, char * agent, char * tls )
{
	char	buffer[1024];
	sprintf(buffer,"service-%s",name);
	return( allocate_string(buffer) );
}

/*	---------------------------------------------------------------	*/
/*	c o r d s _ r e s o l v e _ c o m m o n _ c o n t r a c t _ i d	*/
/*	---------------------------------------------------------------	*/
private	char * cords_resolve_common_contract_id( char * contract, char * coreappname, char * tls )
{
	struct	occi_response * wptr;
	struct	occi_response * zptr;
	struct	occi_element * eptr;
	char *	sptr;
	char *	vptr;
	char *	service;
	char *	id;

	/* ------------------------------------- */
	/* attempt to locate the contract record */
	/* ------------------------------------- */
	if (!( zptr = occi_simple_get( contract, _CORDS_CONTRACT_AGENT, tls ) ))
		return((char *) 0);

	/* --------------------------------------------- */
	/* and resolve the corresponding service linkage */
	/* --------------------------------------------- */
	else if (!( sptr = occi_extract_atribut( zptr, Operator.domain, _CORDS_CONTRACT,_CORDS_SERVICE )))
	{
		zptr = occi_remove_response( zptr );
		return((char *) 0);
	}
	else if (!( service = allocate_string( sptr ) ))
	{
		zptr = occi_remove_response( zptr );
		return((char *) 0);
	}
	else if (!( service =  occi_unquoted_value( service ) ))
	{
		zptr = occi_remove_response( zptr );
		return((char *) 0);
	}
	else 	zptr = occi_remove_response( zptr );

	/* ------------------------------------- */
	/* attempt to locate the contract record */
	/* ------------------------------------- */
	if (!( wptr = occi_simple_get( service, _CORDS_CONTRACT_AGENT, tls ) ))
		return((char *) 0);

	/* -------------------------------------------- */
	/* recover the contracts linked to this service */
	/* -------------------------------------------- */
	for (	eptr = cords_first_link( wptr );
		eptr != (struct occi_element *) 0;
		eptr = eptr->next )
	{
		if (!( eptr->value ))
			continue;
		else if (!( sptr =  occi_unquoted_link( eptr->value ) ))
			continue;
		else if (!( zptr = occi_simple_get( sptr, _CORDS_CONTRACT_AGENT, tls ) ))
		{
			liberate( sptr );
			continue;
		}
		else if (!( vptr = occi_extract_atribut( zptr, Operator.domain, _CORDS_CONTRACT,_CORDS_NAME )))
		{
			liberate( sptr );
			continue;
		}
		else if (!( vptr = allocate_string( vptr ) ))
		{
			liberate( sptr );
			continue;
		}
		else if (!( vptr = occi_unquoted_value( vptr ) ))
		{
			liberate( sptr );
			continue;
		}
		else if (!( strcmp( vptr, coreappname ) ))
		{
			/* ------------------------ */
			/* found the contract match */
			/* ------------------------ */
			liberate( vptr );
			wptr = occi_remove_response( wptr );
			return( sptr );
		}
		else
		{
			/* ------------------------------------------------- */
			/* TODO : need to iterate on nested common instances */
			/* ------------------------------------------------- */
			liberate( sptr );
			liberate( vptr );
			continue;
		}
	}
	wptr = occi_remove_response( wptr );
	return( (char *) 0);
}

/*	-------------------------------------------------------		*/
/*	  c o r d s _ r e s o l v e _ c o n t r a c t _ i d 		*/
/*	-------------------------------------------------------		*/
private	char * cords_resolve_contract_id( struct xml_element * document, char * coreappname, char * tls )
{
	struct	xml_element * eptr=(struct xml_element *) 0;
	struct	xml_atribut * bptr;
	int	l;

	/* -------------------------- */
	/* ensure valid core app name */
	/* -------------------------- */
	if (!( coreappname ))
		return( coreappname );

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
		else if ( *(coreappname+l) != '.' )
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
			else if ( eptr->first )
				return( cords_resolve_contract_id( eptr->first, (coreappname+l+1), tls ) );
			/* -------------------------------------- */
			/* perhaps we have found a common complex */
			/* -------------------------------------- */
			else if (!( bptr = document_atribut( eptr, _CORDS_COMMON ) ))
				return( cords_resolve_contract_id( eptr->first, (coreappname+l+1), tls ) );
			else if (!( bptr->value ))
				return( cords_resolve_contract_id( eptr->first, (coreappname+l+1), tls ) );
			else	return( cords_resolve_common_contract_id( bptr->value, (coreappname+l+1), tls ) );
		}
		else	continue;
	}
	if (!( eptr ))
		return((char *) 0);
	else if (!( bptr = document_atribut( eptr, _CORDS_ID ) ))
		return((char *) 0);
	else	return( bptr->value );
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
		char * nature,
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
	char *  aptr;
	struct	xml_atribut * bptr;
	struct	cordscript_element * lptr;
	struct	cordscript_element * rvalue;
	char *	avalue=(char *) 0;
	char	buffer[2048];

	if (!( lptr = action->lvalue ))
		return( 78 );

	else if (!( lptr->prefix ))
		return( 30 );

	else if (!( aptr = cords_resolve_contract_id( document, lptr->prefix, tls ) ))
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
		else if ((!(dptr=occi_request_element(qptr,"occi.instruction.target"  	, aptr	 	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.nature"   	, nature 	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.method"  	, "configure" 	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.type"  	, "method"  	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.provision" , "" 		) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.symbol" 	, "self"        ) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.source" 	, aptr	 	) ))
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
		else if (!( ihost = occi_extract_location( yptr ) ))
		{
			yptr = occi_remove_response( yptr );
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			liberate( avalue );
			liberate_cordscript_action( action );
			return(53);
		}
		else if (!( zptr =  occi_create_link( aptr,  ihost, agent, tls ) ))
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

/*	---------------------------------------------------------	*/
/*	 c o r d s _ i n v o c a t i o n _ i n s t r u c t i o n	*/
/*	---------------------------------------------------------	*/
/*	here postscript configuration actions are transformed		*/
/*	to produce the configuration and monitoring instruction		*/
/*	set used by the combined broker, procci and providers  		*/
/*	during the actual provisioning for meta data and probe		*/
/*	channel communication and delivery.				*/
/*	---------------------------------------------------------	*/
private	int	cords_invocation_instruction( 
		char * host,
		struct xml_element * document,
		struct cordscript_action * action,
		char * command,
		char * nature,
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
	char * aptr;
	struct	xml_atribut * bptr;
	struct	cordscript_element * lptr;
	struct	cordscript_element * rvalue;
	char *	avalue=(char *) 0;
	char	buffer[2048];

	if (!( lptr = action->lvalue ))
		return( 78 );

	else if (!( lptr->prefix ))
		return( 30 );

	else if (!( aptr = cords_resolve_contract_id( document, lptr->prefix, tls ) ))
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
		else if ((!(dptr=occi_request_element(qptr,"occi.instruction.target"  	, aptr	 	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.nature"   	, nature 	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.method"  	, command 	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.type"  	, "method"  	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.provision" , "" 		) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.symbol" 	, "self"        ) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.source" 	, aptr	 	) ))
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
		else if (!( ihost = occi_extract_location( yptr ) ))
		{
			yptr = occi_remove_response( yptr );
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			liberate( avalue );
			liberate_cordscript_action( action );
			return(53);
		}
		else if (!( zptr =  occi_create_link( aptr,  ihost, agent, tls ) ))
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

/*	---------------------------------------------------------	*/
/*	   c o r d s _ i n te r f a c e _ i n s t r u c t i o n		*/
/*	---------------------------------------------------------	*/
/*	here post script configuration actions are transformed		*/
/*	to produce an interface method instruction			*/
/*	---------------------------------------------------------	*/
private	int	cords_interface_instruction( 
		char * host,
		struct xml_element * document,
		struct cordscript_action * action,
		char * command,
		char * nature,
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
	char * aptr;
	struct	xml_atribut * bptr;
	struct	cordscript_element * lptr;
	struct	cordscript_element * rvalue;
	char *	avalue=(char *) 0;
	char	buffer[2048];

	if (!( lptr = action->lvalue ))
		return( 78 );

	else if (!( lptr->prefix ))
		return( 30 );

	else if (!( aptr = cords_resolve_contract_id( document, lptr->prefix, tls ) ))
		return( 78 );

	if (!( ihost = occi_resolve_category_provider( _CORDS_INSTRUCTION, agent, tls ) ))
	{
		liberate_cordscript_action( action );
		return(46);
	}

	sprintf(buffer,"%s/%s/",ihost,_CORDS_INSTRUCTION);

	liberate( ihost );

	if (!( rvalue=action->rvalue ))
		avalue=allocate_string("");
	else if (!( avalue = occi_unquoted_value( rvalue->value ) ))
	{
		liberate_cordscript_action( action );
		return(27);
	}
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
	else if ((!(dptr=occi_request_element(qptr,"occi.instruction.target"  	, aptr	 	) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.nature"   	, nature 	) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.method"  	, command 	) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.type"  	, "method"  	) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.provision" , "" 		) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.symbol" 	, "self"        ) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.source" 	, aptr	 	) ))
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
	else if (!( ihost = occi_extract_location( yptr ) ))
	{
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		liberate( avalue );
		liberate_cordscript_action( action );
		return(53);
	}
	else if (!( zptr =  occi_create_link( aptr,  ihost, agent, tls ) ))
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

	liberate_cordscript_action( action );
	return( 0 );

}

/*	---------------------------------------------------------	*/
/*	c o r d s _ c o n t r a c t _ p r o p e r t y _ v a l u e	*/
/*	---------------------------------------------------------	*/
private	char *	cords_contract_property_value( char * contract, char * propname, char * agent, char * tls )
{
	struct	occi_response * zptr;
	char *	result;

	if (!( zptr = occi_simple_get( contract, agent, tls ) ))
		return( (char *) 0);

	else if (!( result = occi_extract_atribut( zptr, Operator.domain, _CORDS_CONTRACT, propname ) ))
	{
		zptr = occi_remove_response( zptr );
		return( (char *) 0);
	}
	else
	{
		result = allocate_string( result );
		zptr = occi_remove_response( zptr );
		return( result );
	}	
}

/*	-------------------------------------------------------		*/
/*		c o r d s _ a c t i o n _ t e r m			*/
/*	-------------------------------------------------------		*/
private	char *	cords_action_term( struct cordscript_element * term )
{
	char	*	result;
	switch ( term->type )
	{
	case	0 :
		if (!( term->value ))
			return( allocate_string("") );
		else if (!( result = allocate_string( term->value ) ))
			return( allocate_string("") );
		else if (!( result = occi_unquoted_value( result ) ))
			return( allocate_string("") );
		else	return( result );
	default	:
		return( allocate_string("") );
	}
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
		char * nature,
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
	char * 	aptr;
	char *	bptr;
	struct	cordscript_element * lptr;
	struct	cordscript_element * rvalue;
	char *	target;
	char *	source;
	char *	symbol;
	char *	mname;
	char *	ivalue="";
	char *	pvalue;
	char *	property="";
	char	buffer[2048];

	if (!( lptr = action->lvalue ))
		return( 78 );

	else if (!( lptr->prefix ))
		return( 30 );

	else if (!( mname = cordscript_method( action->type ) ))
		return(30);

	else if (!( strcasecmp( mname, "none" ) ))
		return( cords_affectation_instruction( host, document, action, nature,agent, tls ) );

	else if (!( strcasecmp( mname, "set" ) ))
		return( cords_affectation_instruction( host, document, action, nature,agent, tls ) );

	else if (!( strcasecmp( mname, "system" ) ))
		return( cords_invocation_instruction( host, document, action, mname,nature, agent, tls ) );

	else if (!( strcasecmp( mname, "fork" ) ))
		return( cords_invocation_instruction( host, document, action, mname,nature, agent, tls ) );

	else if (!( strcasecmp( mname, "kill" ) ))
		return( cords_invocation_instruction( host, document, action, mname,nature, agent, tls ) );

	else if (!( strcasecmp( mname, "invoke" ) ))
		return( cords_interface_instruction( host, document, action, lptr->value,nature, agent, tls ) );

	else if (!( aptr = cords_resolve_contract_id( document, lptr->prefix, tls ) ))
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
		property = rvalue->value;

		if (!( strcasecmp( mname, "monitor" ) ))
		{
			if (!( target = aptr ))
				continue;
			else if (!( source = aptr ))
				continue;
			else if (!(symbol = cords_resolve_consumer_id( rvalue->prefix, agent, tls ) ))
				continue;
			else if (!( rvalue->next ))
				ivalue = allocate_string("");
			else
			{
				rvalue  = rvalue->next;
				ivalue = cords_action_term( rvalue );
			}
		}
		else
		{
			if (!( target = aptr ))
				continue;
			else if (!( bptr = cords_resolve_contract_id( document, rvalue->prefix, tls ) ))
				continue;
			else if (!( source = bptr ))
				continue;
			else if (!( symbol = allocate_string( rvalue->prefix ) ))
				continue;
			else if (!( pvalue = cords_contract_property_value( source, rvalue->value, agent, tls ) ))
			{
				if (!( property ))
					ivalue = allocate_string("");
				else if (!( strcmp( property, "contract" ) ))
					ivalue = allocate_string( bptr ) ;
				else	ivalue = allocate_string("");
			}
			else	ivalue = pvalue;
		}

		if (!( kptr = occi_create_client( buffer, agent, tls ) ))
		{
			liberate_cordscript_action( action );
			symbol = liberate( symbol );
			return(46);
		}
		else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
		{
			kptr = occi_remove_client( kptr );
			liberate_cordscript_action( action );
			symbol = liberate( symbol );
			return(50);
		}
		else if ((!(dptr=occi_request_element(qptr,"occi.instruction.target"  	, target  	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.nature"   	, nature 	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.method"  	, mname 	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.type"  	, "method"  	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.provision" , "" 		) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.value"  	, ivalue	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.symbol" 	, symbol 	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.source" 	, source 	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.property"	, property	) )))
		{
			liberate( ivalue );
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			liberate_cordscript_action( action );
			symbol = liberate( symbol );
			return(51);
		}
		else if (!( yptr = occi_client_post( kptr, qptr ) ))
		{
			liberate( ivalue );
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			liberate_cordscript_action( action );
			symbol = liberate( symbol );
			return(52);
		}
		else if (!( ihost = occi_extract_location( yptr ) ))
		{
			liberate( ivalue );
			yptr = occi_remove_response( yptr );
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			liberate_cordscript_action( action );
			symbol = liberate( symbol );
			return(53);
		}
		else if (!( zptr =  occi_create_link( aptr,  ihost, agent, tls ) ))
		{
			liberate( ivalue );
			yptr = occi_remove_response( yptr );
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			liberate_cordscript_action( action );
			symbol = liberate( symbol );
			return(54);
		}
		else
		{
			liberate( ivalue );
			zptr = occi_remove_response( zptr );
			yptr = occi_remove_response( yptr );
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			symbol = liberate( symbol );
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

	if (!( statement = occi_extract_atribut( zptr, Operator.domain,_CORDS_ACTION,_CORDS_EXPRESSION ) ))
	{
		return( 78 );
	}
	else if (!( type = occi_extract_atribut( zptr, Operator.domain,_CORDS_ACTION,_CORDS_TYPE ) ))
		type = allocate_string("cordscript");
	else if (!( strlen( type ) ))
		type = allocate_string("cordscript");
	else if (!( strcmp( type, _CORDS_NULL ) ))
		type = allocate_string("cordscript");

	if ( type != (char *) 0 )
	{
		if (!( strcmp( type, "cordscript" )))
			if ((aptr = cordscript_parse_statement( statement )) != (struct cordscript_action *) 0)
				cords_action_instruction( host, document, aptr,_CORDS_CONFIGURATION, agent, tls );

		type = liberate( type );
	}

	statement = liberate( statement );	 

	return(0);
}

/*	-------------------------------------------------------		*/
/*	  	c o r d s _ i n t e r f a c e _ a c t i o n		*/
/*	-------------------------------------------------------		*/
private	int	cords_interface_action( 
		char * host,
		struct xml_element * document,
		struct occi_response * zptr,
		char * agent,
		char * tls )
{
	char *	type=(char *) 0;
	char *	name=(char *) 0;
	char *	statement=(char *) 0;
	struct	cordscript_action * aptr=(struct cordscript_action*) 0;

	if (!( statement = occi_extract_atribut( zptr, Operator.domain,_CORDS_ACTION,_CORDS_EXPRESSION ) ))
		return( 78 );
	if (!( type = occi_extract_atribut( zptr, Operator.domain,_CORDS_ACTION,_CORDS_TYPE ) ))
		type = allocate_string("cordscript");

	if (!( name = occi_extract_atribut( zptr, Operator.domain,_CORDS_ACTION,_CORDS_NAME ) ))
		name = allocate_string("method");

	if (!( strcmp( type, "cordscript" )))
		if ((aptr = cordscript_parse_statement( statement )) != (struct cordscript_action *) 0)
			cords_action_instruction( host, document, aptr,name, agent, tls );

	type = liberate( type );
	name = liberate( name );
	statement = liberate( statement );	 

	return(0);
}

/*	-------------------------------------------------------		*/
/*	  	c o r d s _ r e l e a s _ a c t i o n			*/
/*	-------------------------------------------------------		*/
private	int	cords_release_action( 
		char * host,
		struct xml_element * document,
		struct occi_response * zptr,
		char * agent,
		char * tls )
{
	char *	type=(char *) 0;
	char *	name=(char *) 0;
	char *	statement=(char *) 0;
	struct	cordscript_action * aptr=(struct cordscript_action*) 0;

	if (!( statement = occi_extract_atribut( zptr, Operator.domain,_CORDS_ACTION,_CORDS_EXPRESSION ) ))
		return( 78 );

	if (!( type = occi_extract_atribut( zptr, Operator.domain,_CORDS_ACTION,_CORDS_TYPE ) ))
		type = allocate_string("cordscript");

	if (!( strcmp( type, "cordscript" )))
		if ((aptr = cordscript_parse_statement( statement )) != (struct cordscript_action *) 0)
			cords_action_instruction( host, document, aptr,_CORDS_RELEASE, agent, tls );

	type = liberate( type );
	name = liberate( name );
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
	struct	xml_atribut * xptr;
	struct	occi_element *	eptr;
	struct	occi_response * aptr;
	char	*	id;
	int		status;
	int		instructions=0;
	char		buffer[64];
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
			instructions++;
			id = liberate( id );
			continue;
		}
	}
	sprintf(buffer,"%u",instructions);
	if (( xptr = document_atribut( document, "instructions" ) ) != (struct xml_atribut *) 0)
	{
		if ( xptr->value ) xptr->value = liberate( xptr->value );
		if (!( xptr->value = allocate_string( buffer ) ))
			return( 927 );
		else	return( 0 );
	}
	else if (!( xptr = document_add_atribut( document, "instructions", buffer ) ))
		return( 927 );
	else	return( 0 );
}

/*	-------------------------------------------------------		*/
/*		c o r d s _ b r o k e r _ r e l e a s e 		*/
/*	-------------------------------------------------------		*/
/*	the counterpart to configuration is release and will be		*/
/*	invoked through the COSACS interface just prior to the		*/
/*	release of provisioning.					*/
/*	-------------------------------------------------------		*/
private	int	cords_broker_release(
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
			cords_release_action( host, document, aptr, agent, tls );
			aptr = occi_remove_response( aptr );
			id = liberate( id );
			continue;
		}
	}
	return( 0 );
}

/*	-------------------------------------------------------		*/
/*	  c o r d s _ b r o k e r _ i n t e r f a c e			*/
/*	-------------------------------------------------------		*/
/*	this function is called from the service instance build		*/
/*	once the contract instances have been created and their		*/
/*	details are resolved and established. 				*/
/*									*/
/*	Here two things will happen:					*/
/*									*/
/*	1) we will parse the collection of action statements		*/
/*	to produce the corresponding concrete interface methods		*/
/*									*/
/*	2) we will parse the collection of action statements to		*/
/*	produce the collection of monitoring channels that are 		*/
/*	to be set up for the control of the service.			*/
/*									*/
/*	These two will occur at the same, differentiated by the		*/
/*	cordscript keywords "configure" and "monitor".			*/
/*	-------------------------------------------------------		*/

private	int	cords_broker_interface(
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
			cords_interface_action( host, document, aptr, agent, tls );
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
	sprintf(filter,"%s.%s.name",Operator.domain,cptr->value);

	if (!( yptr = cords_retrieve_named_instance_list( 
		cptr->value, filter, nptr->value, agent, tls )))
		return((char *) 0);

	/* --------------------------------- */
	/* get the first of the matching set */
	/* --------------------------------- */
	else if (!( zptr = cords_retrieve_named_instance( yptr, agent, tls )))
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
	sprintf(buffer,"%s.%s.",Operator.domain,cptr->value);
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
	else if (!( host = occi_extract_location( yptr ) ))
		return((char *) 0);
	else if (!( host = occi_category_id( host ) ))
		return( host );
	else
	{
		sprintf(buffer,"%s/%s/%s",zptr->host,cptr->value,host);
		return(allocate_string(buffer) );
	}
}

/*	--------------------------------------------------------	*/
/*	c o r d s _ a d d _ p r o v i d e r _ a t t r i b u t e		*/
/*	--------------------------------------------------------	*/
private	struct	occi_request  * cords_add_provider_attribute(
	struct	occi_request  * qptr,
	char *	provider,
	char *	atbname,
	char * 	atbvalue )
{
	struct	occi_element  * fptr;
	char 	filter[1024];

	if (!( qptr ))	return( qptr );

	sprintf(filter,"%s.%s.%s",Operator.domain,provider,atbname);

	if (!(fptr=occi_request_element(qptr,filter,atbvalue) ))
		return( occi_remove_request( qptr ) );

	else	return( qptr );		
}

/*	---------------------------------------------------------	*/
/*		c o r d s _ c o e s _ o p e r a t i o n			*/
/*	---------------------------------------------------------	*/
/*	this function provides linkage to COES placement engine		*/
/*	for the selection of the provider to be used by broker		*/		
/*	---------------------------------------------------------	*/
private	char *	cords_coes_operation( 
	struct cords_placement_criteria * selector, 
	char * agent, char * tls )
{
	struct	occi_action	* aptr;
	struct	occi_client 	* kptr;
	struct	occi_request	* qptr;
	struct	occi_response 	* zptr;
	struct	occi_response 	* yptr;
	struct	occi_element 	* eptr;
	struct	occi_element 	* dptr;
	char 			* result;
	char 			* id;
	char 			buffer[1024];

	if (!( zptr = occi_resolver( _CORDS_PLACEMENT, agent )))
		return( (char *) 0 );
	else
	{
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
			else	sprintf(buffer,"%s/%s/",eptr->value,_CORDS_PLACEMENT);

			if (!( kptr = occi_create_client( buffer, agent, tls ) ))
				continue;
			else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
			{
				kptr = occi_remove_client( kptr );
				continue;
			}
			else if ((!(dptr=occi_request_element(qptr,"occi.placement.provider" , selector->provider ) ))
			     ||  (!(dptr=occi_request_element(qptr,"occi.placement.name"     , "" 		  ) ))
			     ||  (!(dptr=occi_request_element(qptr,"occi.placement.solution" , "" 		  ) ))
			     ||  (!(dptr=occi_request_element(qptr,"occi.placement.node"     , selector->node     ) )))
			{
				qptr = occi_remove_request( qptr );
				kptr = occi_remove_client( kptr );
				continue;
			}

			if (( selector->algorithm )
			&&  (!(dptr=occi_request_element(qptr,"occi.placement.algorithm", selector->algorithm ) )))
			{
				qptr = occi_remove_request( qptr );
				kptr = occi_remove_client( kptr );
				continue;
			}
			else if (!(dptr=occi_request_element(qptr,"occi.placement.algorithm", "default" ) ))
			{
				qptr = occi_remove_request( qptr );
				kptr = occi_remove_client( kptr );
				continue;
			}

			if (( selector->zone )
			&&  (!(dptr=occi_request_element(qptr,"occi.placement.zone", selector->zone ) )))
			{
				qptr = occi_remove_request( qptr );
				kptr = occi_remove_client( kptr );
				continue;
			}
			else if (!(dptr=occi_request_element(qptr,"occi.placement.zone", "" ) ))
			{
				qptr = occi_remove_request( qptr );
				kptr = occi_remove_client( kptr );
				continue;
			}

			if (( selector->energy )
			&&  (!(dptr=occi_request_element(qptr,"occi.placement.energy", selector->energy ) )))
			{
				qptr = occi_remove_request( qptr );
				kptr = occi_remove_client( kptr );
				continue;
			}
			else if (!(dptr=occi_request_element(qptr,"occi.placement.energy", "" ) ))
			{
				qptr = occi_remove_request( qptr );
				kptr = occi_remove_client( kptr );
				continue;
			}


			if (( selector->opinion )
			&&  (!(dptr=occi_request_element(qptr,"occi.placement.opinion", selector->opinion ) )))
			{
				qptr = occi_remove_request( qptr );
				kptr = occi_remove_client( kptr );
				continue;
			}
			else if (!(dptr=occi_request_element(qptr,"occi.placement.opinion", "" ) ))
			{
				qptr = occi_remove_request( qptr );
				kptr = occi_remove_client( kptr );
				continue;
			}

			if (( selector->security )
			&&  (!(dptr=occi_request_element(qptr,"occi.placement.security", selector->security ) )))
			{
				qptr = occi_remove_request( qptr );
				kptr = occi_remove_client( kptr );
				continue;
			}
			else if (!(dptr=occi_request_element(qptr,"occi.placement.security", "" ) ))
			{
				qptr = occi_remove_request( qptr );
				kptr = occi_remove_client( kptr );
				continue;
			}

			if (( selector->operator )
			&&  (!(dptr=occi_request_element(qptr,"occi.placement.operator", selector->operator ) )))
			{
				qptr = occi_remove_request( qptr );
				kptr = occi_remove_client( kptr );
				continue;
			}

			else if (!(dptr=occi_request_element(qptr,"occi.placement.operator", "" ) ))
			{
				qptr = occi_remove_request( qptr );
				kptr = occi_remove_client( kptr );
				continue;
			}

			if (( selector->price )
			&&  (!(dptr=occi_request_element(qptr,"occi.placement.price", selector->price ) )))
			{
				qptr = occi_remove_request( qptr );
				kptr = occi_remove_client( kptr );
				continue;
			}
			else if (!(dptr=occi_request_element(qptr,"occi.placement.price", "" ) ))
			{
				qptr = occi_remove_request( qptr );
				kptr = occi_remove_client( kptr );
				continue;
			}

			if (!( yptr = occi_client_post( kptr, qptr ) ))
			{
				qptr = occi_remove_request( qptr );
				kptr = occi_remove_client( kptr );
				continue;
			}

			else if (!( id = occi_extract_location( yptr ) ))
			{
				yptr = occi_remove_response( yptr );
				qptr = occi_remove_request( qptr );
				kptr = occi_remove_client( kptr );
				continue;
			}
			else if (!( selector->solution = allocate_string( id )))
			{
				yptr = occi_remove_response( yptr );
				qptr = occi_remove_request( qptr );
				kptr = occi_remove_client( kptr );
				continue;
			}
			else
			{

				/* ------------------------------ */
				/* build the placement identifier */
				/* ------------------------------ */
				rest_add_http_prefix( buffer, 1024, id );
				yptr = occi_remove_response( yptr );
				qptr = occi_remove_request( qptr );

				/* ------------------------------------ */
				/* check for existance of choose method */
				/* ------------------------------------ */
				if ( cords_validate_action( 
					kptr, _CORDS_PLACEMENT, 
					_CORDS_CHOOSE, buffer ) )
				{
					/* --------------------------- */
					/* invoke the placement choice */
					/* --------------------------- */
					if (!( yptr = cords_invoke_action( 
							buffer, 
							_CORDS_CHOOSE, agent, tls ) ))
						continue;
					else	yptr = occi_remove_response( yptr );
				}

				kptr = occi_remove_client( kptr );

				/* --------------------- */
				/* retrieve the solution */
				/* --------------------- */
				if (!( yptr = occi_simple_get( buffer, agent, tls ) ))
					continue;
				/* ------------------------- */
				/* detect solution available */
				/* ------------------------- */
				else if (!( result = occi_extract_atribut( 
					yptr, Operator.domain, 
					_CORDS_PLACEMENT, _CORDS_SOLUTION ))) 
				{
					yptr = occi_remove_response( yptr );
					continue;
				}
				else
				{
					result = allocate_string( result );
					yptr = occi_remove_response( yptr );
					return( result );
				}
			}
		}
		return((char *) 0);
	}
}

/*	----------------------------------------------------------	*/
/*		c o r d s _ s e l e c t _ p r o v i d e r		*/
/*	----------------------------------------------------------	*/
private	char *	cords_select_provider( struct cords_placement_criteria * selector, char * agent, char * tls )
{
	struct	occi_response 	* zptr;
	struct	occi_response 	* yptr;
	struct	occi_element 	* eptr;
	char			* solution;

	/* ------------------------------------------------------ */
	/* allow the COES elastic placement engine to do its work */
	/* ------------------------------------------------------ */
	if (( solution = cords_coes_operation( selector, agent, tls )) != (char *) 0)
		return( solution );
	
	/* ------------------------------------------------------ */
	/* attempt to resolve agencys of the "provider" category */
	/* ------------------------------------------------------ */
	else if (!( zptr = occi_resolver( selector->provider, agent ) ))
		return( (char *) 0 );

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
		else 	return( allocate_string( eptr->value ) );
	}
	return((char *) 0);
}

/*	--------------------------------------------------------	*/
/*	    r e s e t _ p l a c e m e n t _ c r i t e r i a		*/
/*	--------------------------------------------------------	*/
private	void	reset_placement_criteria( struct cords_placement_criteria * selector )
{
	selector->node		=
	selector->price		=
	selector->opinion	=
	selector->provider	=
	selector->operator	=
	selector->zone		=
	selector->security	=
	selector->solution	= 
	selector->algorithm	= (char *) 0;	
	return;
}

/*	--------------------------------------------------------	*/
/*	      s e t _ p l a c e m e n t _ c r i t e r i a		*/
/*	--------------------------------------------------------	*/
private	void	set_placement_criteria( struct cords_placement_criteria * selector, char * provider, char * node )
{
	selector->node		= node;
	selector->provider	= provider;
	return;
}

/*	--------------------------------------------------------	*/
/*		c o r d s _ c r e a t e _ p r o v i d e r 		*/
/*	--------------------------------------------------------	*/
private	char * 	cords_contract_provider(
	struct cords_node_descriptor * App,
	char *	id,
	struct xml_element * dptr , 
	char * agent, char * tls )
{
	struct	occi_client   * kptr;
	struct	occi_response * yptr;
	struct	occi_request  * qptr;
	struct	occi_element  * fptr;

	struct	xml_atribut  * cptr;
	struct	xml_atribut  * pptr;
	struct	xml_atribut  * nptr;
	char *	sptr;
	char *	host;
	char 	buffer[2048];
	char *	zptr;

	/* --------------------------------------------- */
	/* retrieve the provider identification category */
	/* --------------------------------------------- */
	if (!( cptr = document_atribut( dptr, _CORDS_CATEGORY ) ))
		return((char *) 0);

	/* ----------------------------------------- */
	/* retrieve the provider identification name */
	/* ----------------------------------------- */
	else if (!( nptr = document_atribut( dptr, _CORDS_NAME ) ))
		return((char *) 0);

	/* ----------------------------------------- */
	/* retrieve the provider profile name string */
	/* ----------------------------------------- */
	else if (!( pptr = document_atribut( dptr, _CORDS_PROFILE ) ))
		return((char *) 0);

	/* ----------------------------------- */
	/* select a provider for this category */
	/* ----------------------------------- */
	else 
	{
		set_placement_criteria( &App->selector, cptr->value, id );
		if (!( zptr = cords_select_provider( &App->selector, agent, tls ) ))
			return( zptr );
	}

	/* --------------------------------- */
	/* create the client and the request */
	/* --------------------------------- */
	sprintf(buffer,"%s/%s/",zptr,cptr->value);

	if (!( kptr = occi_create_client( buffer, agent, tls ) ))
		return((char *) 0);
	else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
		return((char *) 0);

	/* ----------------------------------- */
	/* build the contract creation request */
	/* ----------------------------------- */
	else if (!( qptr = cords_add_provider_attribute( qptr, cptr->value, "name", nptr->value ) ))
		return((char *) 0);
	else if (!( qptr = cords_add_provider_attribute( qptr, cptr->value, "node", id ) ))
		return((char *) 0);
	else if (!( qptr = cords_add_provider_attribute( qptr, cptr->value, "account", App->account ) ))
		return((char *) 0);
	else if (!( qptr = cords_add_provider_attribute( qptr, cptr->value, "profile", pptr->value ) ))
		return((char *) 0);

	/* ------------------------ */
	/* post the data collection */
	/* ------------------------ */
	else if (!( yptr = occi_client_post( kptr, qptr ) ))
		return((char *) 0);

	/* --------------------- */
	/* return the identifier */
	/* --------------------- */
	else if (!( host = occi_extract_location( yptr ) ))
		return((char *) 0);
	else if (!( host = occi_category_id( host ) ))
		return( host );
	else
	{
		/* ---------------------------------------------------- */
		/* store the placement attribute in the parent contract */
		/* ---------------------------------------------------- */
		if (( App->selector.solution  )
		&&  ( dptr->parent )
		&&  ( dptr->parent->name )
		&&  (!( strcmp( dptr->parent->name, _CORDS_CONTRACT) )))
		{
			(void) document_add_atribut( dptr->parent, _CORDS_PLACEMENT, App->selector.solution );
		}
		sprintf(buffer,"%s/%s/%s",zptr,cptr->value,host);
		return(allocate_string(buffer) );
	}
}

/*	-------------------------------------------------------		*/
/*		c o r d s _ a d d  _ p r o v i d e r  			*/
/*	-------------------------------------------------------		*/
/*	builds an XML element  from of a provider instance of a		*/
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
	if ( dptr->account )	dptr->account = liberate( dptr->account );
	if ( dptr->accountName) dptr->accountName = liberate( dptr->accountName );
	if ( dptr->nameApp )	dptr->nameApp = liberate( dptr->nameApp );
	if ( dptr->typeApp )	dptr->typeApp = liberate( dptr->typeApp );
	if ( dptr->accessApp )	dptr->accessApp = liberate( dptr->accessApp );
	if ( dptr->scopeApp )	dptr->scopeApp = liberate( dptr->scopeApp );
	if ( dptr->category )	dptr->category = liberate( dptr->category );
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
private	char *	cords_research_provider( 
			char * defaut,
			char * agent, char * tls )
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
		if (!( yptr = occi_simple_get( buffer, agent, tls ) ))
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
			if (!( xptr = occi_simple_get( buffer, agent, tls ) ))
				continue;

			sprintf(buffer,"%s.%s.",Operator.domain,_CORDS_PROVIDER);
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
private	char *	cords_resolve_provider( 
			struct occi_response * node, 
			char * defaut,
			struct cords_placement_criteria * selector,
			char * agent,char * tls )
{
	char	*	value;
	/* ----------------------------------------- */
	/* retrieve the provider information instance */
	/* ----------------------------------------- */
	if (!( value = occi_extract_atribut( node, Operator.domain, _CORDS_NODE, _CORDS_PROVIDER ))) 
		return( cords_research_provider(defaut,agent, tls) );
	else if (!( strcmp(value,_CORDS_NULL) ))
		return( cords_research_provider(defaut,agent, tls) );
	else if (!( strcasecmp(value,_CORDS_ANY) ))
	{
		if ( selector->provider )
			return( selector->provider );
		else	return( cords_research_provider(defaut,agent, tls) );
	}
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
	if (!( value = occi_extract_atribut( node, Operator.domain, _CORDS_NODE, _CORDS_PROFILE ))) 
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
		char * node, 	
		char * name, 
		char * agreement,
		char * parentservice,
		char * provider,
		int	flags )
{
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;
	char	fbuffer[356];
	sprintf(fbuffer,"%u",flags);
	if (!( eptr = allocate_element()))
		return((struct xml_element *) 0);
	else if (!( eptr->name = allocate_string( _CORDS_CONTRACT ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_NAME, name ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_NODE, node ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_AGREEMENT, ( agreement ? agreement : "") ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_PARENTSERVICE, (parentservice ? parentservice : "") ) ))
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
	else if (!( aptr = document_add_atribut( eptr, _CORDS_FLAGS, fbuffer ) ))
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
private	char *	cords_instance_service( 
	char * host, 
	char * planid, 
	char * agent, 
	char * tls, 
	char * sla,
	struct xml_element ** root )
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
	else if (!( name = occi_extract_atribut( zptr, Operator.domain, _CORDS_PLAN, _CORDS_NAME ) ))
		service = (char *) 0;
	else if (!( manifest = occi_extract_atribut( zptr, Operator.domain, _CORDS_PLAN, _CORDS_MANIFEST ) ))
		service = (char *) 0;
	else	service = cords_manifest_broker(host, plan, name, manifest, agent, tls, sla, root );

	/* ----------------------------- */
	/* liberate response and strings */
	/* ----------------------------- */
	if ( zptr ) 	zptr = occi_remove_response (zptr );
	if ( name )	name = liberate( name );
	if ( manifest )	manifest = liberate( manifest );
	if ( plan )	plan = liberate( plan );

	return( service );
}

/*	-------------------------------------------------------------	*/
/*	c o r d s _ i n s t a n c e _ c o m p l e x _ c o n t r a c t	*/
/*	-------------------------------------------------------------	*/
private	struct	xml_element * 	cords_instance_complex_contract(
	struct cords_node_descriptor * App,
	char *	host,
	char *	id,
	char *	agent,
	char *	tls,
	char *	sla,
	char * 	namePlan )
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
	if (!( service = cords_instance_service( host, App->typeApp, agent, tls, sla, &xroot ) ))
	{
		cords_terminate_instance_node( App );
		return((struct xml_element *) 0);
	}
	/* --------------------------------------------------- */
	/* then create the contract document for the node here */
	/* --------------------------------------------------- */
	else if (!( document = cords_build_contract( id, App->nameApp, sla, App->service, _CORDS_ANY , App->flags) ))
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

/*	----------------------------------------------------------------	*/
/*	c o r d s _ i n s t a n c e _ a b s t r a c t _ c o n t r a c t	 	*/
/*	----------------------------------------------------------------	*/
private	struct	xml_element * 	cords_instance_abstract_contract(
	struct cords_node_descriptor * App,
	char *	host,
	char *	id,
	char *	agent,
	char *	tls,
	char *	sla,
	char * 	namePlan )
{
	int	status;
	struct	xml_element 	*	xroot=(struct xml_element *) 0;
	struct	xml_element 	*	xptr;
	struct	xml_element 	* 	document=(struct xml_element *) 0;
	struct	xml_atribut	*	aptr;

	/* --------------------------------------------------- */
	/* then create the contract document for the node here */
	/* --------------------------------------------------- */
	if (!( document = cords_build_contract( id, App->nameApp, sla, App->service, _CORDS_ANY , App->flags) ))
	{
		cords_terminate_instance_node( App );
		return((struct xml_element *) 0);
	}
	else	return( document );

}

/*	----------------------------------------------------------------------------	*/
/*		      c o r d s _ b u i l d _ a b s t r a c t _ c o m m o n		*/
/*	----------------------------------------------------------------------------	*/
private	char *	cords_build_abstract_common(
	struct cords_node_descriptor * App,
	char *	host,
	char *	id,
	char *	agent,
	char *	tls,
	char *	sla,
	char * namePlan,
	struct xml_element ** xroot )
{
	struct	xml_element * document;
	struct	xml_atribut * aptr;
	char *	common;

	/* ----------------------------------- */
	/* build a new simple service contract */
	/* ----------------------------------- */
	if (!( document = cords_instance_abstract_contract( App, host, id, agent, tls,sla, namePlan ) ))
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
	else if (!( xroot ))
	{
		document = document_drop( document );
		return( common );
	}
	else
	{
		*xroot = document;
		return( common );
	}
}

/*	------------------------------------------------------------	*/
/*		c o r d s _ n e g o t i a t e _ p r o v i d e r		*/
/*	------------------------------------------------------------	*/
private	char *	cords_negotiate_provider( 
		struct cords_node_descriptor * App,
		char * id,
		struct xml_element * xptr,
		char * agent,
		char * tls )
{
	/* ------------------------------------------------------- */
	/* request of the provider type the creation of a contract */
	/* ------------------------------------------------------- */
	if ( Operator.negotiate & _BROKER_PROVIDER_CONTRACT )
		return( cords_contract_provider( App, id, xptr, agent, tls ) );
	/* ---------------------------------------------------- */
	/*    this is the old way which we keep for failover	*/
	/* ---------------------------------------------------- */
	else
		return( cords_create_provider( xptr, agent, tls ) );
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
	char *	sla,
	char * namePlan )
{
	int	status;
	struct	xml_element 	*	xroot=(struct xml_element *) 0;
	struct	xml_element 	*	xptr;
	struct	xml_element 	* 	document=(struct xml_element *) 0;
	struct	xml_atribut	*	aptr;
	char 			*	service;

	if (!( document = cords_build_contract( id, App->nameApp, sla, App->service, App->provider, App->flags )))
	{
		cords_terminate_instance_node( App );
		return((struct xml_element *) 0);
	}
	else if (!( xptr = cords_add_provider( document, App->provider, App->nameApp, App->profile ) ))
	{
		cords_terminate_instance_node( App );
		return((struct xml_element *) 0);
	}
	else if (!( App->providerid = cords_negotiate_provider( App, id, xptr, agent, tls ) ))
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
	if (( App->hid  = occi_extract_atribut( App->node, Operator.domain, 
			_CORDS_NODE, _CORDS_INFRASTRUCTURE )) != (char *) 0)
	{
		if (!( App->hardware = cords_retrieve_instance( host, App->hid, agent, tls )))
		{
			cords_terminate_instance_node( App );
			return(document_drop(document));
		}
		else	App->hid = liberate( App->hid );

		/* retrieve the compute instance */
		/* ----------------------------- */
		if (( App->hid  = occi_extract_atribut( App->hardware, Operator.domain, 
				_CORDS_INFRASTRUCTURE, _CORDS_COMPUTE )) != (char *) 0)
		{
			if (!( App->compute = cords_retrieve_instance( host, App->hid, agent, tls )))
			{
				cords_terminate_instance_node( App );
				return(document_drop(document));
			}
			else	App->hid = liberate( App->hid );
		}
		else	App->compute = (struct occi_response *) 0;

		/* retrieve the storage instance */
		/* ----------------------------- */
		if (( App->hid  = occi_extract_atribut( App->hardware, Operator.domain, 
				_CORDS_INFRASTRUCTURE, _CORDS_STORAGE )) != (char *) 0)
		{
			if (!( App->storage = cords_retrieve_instance( host, App->hid, agent, tls )))
			{
				cords_terminate_instance_node( App );
				return(document_drop(document));
			}
			else	App->hid = liberate( App->hid );
		}
		else	App->storage = (struct occi_response *) 0;

		/* retrieve the network instance */
		/* ----------------------------- */
		if (( App->hid  = occi_extract_atribut( App->hardware, Operator.domain, 
				_CORDS_INFRASTRUCTURE, _CORDS_NETWORK )) != (char *) 0)
		{
			if (!( App->network = cords_retrieve_instance( host, App->hid, agent, tls )))
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
	if (( App->sid  = occi_extract_atribut( App->node, Operator.domain, 
			_CORDS_NODE, _CORDS_IMAGE )) != (char *) 0)
	{
		if (!( App->software = cords_retrieve_instance( host, App->sid, agent, tls )))
		{
			cords_terminate_instance_node( App );
			return(document_drop(document));
		}
		else	App->sid = liberate( App->sid );

		if (( App->sid  = occi_extract_atribut( App->software, Operator.domain, 
				_CORDS_IMAGE, _CORDS_SYSTEM )) != (char *) 0)
		{
			if (!( App->system = cords_retrieve_instance( host, App->sid, agent, tls )))
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
	char *	sla,
	char * common )
{
	struct	occi_response * yptr;
	struct	xml_element * document;
	struct	xml_atribut * aptr;

	if (!( document = cords_build_contract( id, App->nameApp, sla, App->service, App->provider, App->flags )))
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

	if (!( ihost = rest_log_comons_identity(_CORDS_INSTANCE,agent, tls) ))
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
	else if ((!(dptr=occi_request_element(qptr,"occi.instance.name"        , App->nameApp) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.instance.common"      , common      ) )))
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
	char *	sla,
	int	location,
	char *	common )
{
	struct	occi_response * yptr;
	struct	xml_element * document;
	struct	xml_atribut * aptr;

	if (!( document = cords_build_contract( id, App->nameApp, sla, App->service, App->provider, App->flags )))
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
/*	c o r d s _ a b s t r a c t _ p r i v a t e _ c o m m o n  _ c o n t r a c t	*/
/*	----------------------------------------------------------------------------	*/
private	struct	xml_element * 	cords_abstract_private_common_contract(
	struct cords_node_descriptor * App,
	char *	host,
	char *	id,
	char *	agent,
	char *	tls,
	char *	sla,
	char * 	namePlan )
{
	struct	xml_element * document=(struct xml_element *) 0;
	struct	xml_atribut * aptr;
	char *	common;
	/* ------------------------------------------ */
	/* retrieve the common instance from the node */
	/* ------------------------------------------ */
	if (!( common = occi_extract_atribut(App->node,Operator.domain,_CORDS_NODE,_CORDS_COMMON)))
		if (!( common = cords_build_abstract_common( App, host, id, agent, tls, sla, namePlan, &document ) ))
			return( (struct xml_element *) 0 );

	if ( document )
		document = document_drop( document );

	return( cords_terminate_private_common_contract( App, id, agent, tls,sla, common ) );
}

/*	----------------------------------------------------------------------------	*/
/*	  c o r d s _ a b s t r a c t _ p u b l i c _ c o m m o n  _ c o n t r a c t	*/
/*	----------------------------------------------------------------------------	*/
private	struct	xml_element * 	cords_abstract_public_common_contract(
	struct cords_node_descriptor * App,
	char *	host,
	char *	id,
	char *	agent,
	char *	tls,
	char *	sla,
	char * 	namePlan )
{
	struct	xml_element * document=(struct xml_element *) 0;
	char * 	common=(char *) 0;
	int	location=0;
	if (!( common = cords_resolve_public_common(App,host,id,agent, tls,&location) ))
	{
		if (!( common = cords_build_abstract_common( App, host, id, agent, tls, sla, namePlan, &document ) ))
			return( (struct xml_element *) 0 );
		else if (!( common = allocate_string( common ) ))
			return( (struct xml_element *) 0 );
	}

	if ( document )
		document = document_drop( document );

	return( cords_terminate_public_common_contract( App, id, agent, tls, sla, location, common ) );
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
	char *	sla,
	char * namePlan )
{
	struct	xml_element * document;
	struct	xml_atribut * aptr;
	char *	common;
	/* ----------------------------------- */
	/* build a new simple service contract */
	/* ----------------------------------- */
	if (!( document = cords_instance_simple_contract( App, host, id, agent, tls, sla, namePlan ) ))
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
	char *	sla,
	char * namePlan )
{
	struct	xml_element * document;
	struct	xml_atribut * aptr;
	char *	common;

	/* ------------------------------------------ */
	/* retrieve the common instance from the node */
	/* ------------------------------------------ */
	if (!( common = occi_extract_atribut(App->node,Operator.domain,_CORDS_NODE,_CORDS_COMMON)))
		if (!( common = cords_build_simple_common( App, host, id, agent, tls, sla, namePlan ) ))
			return( (struct xml_element *) 0 );

	return( cords_terminate_private_common_contract( App, id, agent, tls, sla, common ) );
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
	char *	sla,
	char * namePlan,
	struct xml_element ** xroot )
{
	struct	xml_element * document;
	struct	xml_atribut * aptr;
	char *	common;

	/* ----------------------------------- */
	/* build a new simple service contract */
	/* ----------------------------------- */
	if (!( document = cords_instance_complex_contract( App, host, id, agent, tls,sla, namePlan ) ))
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
	else if (!( xroot ))
	{
		document = document_drop( document );
		return( common );
	}
	else
	{
		*xroot = document;
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
	char *	sla,
	char * 	namePlan )
{
	struct	xml_element * document=(struct xml_element *) 0;
	struct	xml_atribut * aptr;
	char *	common;
	/* ------------------------------------------ */
	/* retrieve the common instance from the node */
	/* ------------------------------------------ */
	if (!( common = occi_extract_atribut(App->node,Operator.domain,_CORDS_NODE,_CORDS_COMMON)))
		if (!( common = cords_build_complex_common( App, host, id, agent, tls, sla, namePlan, &document ) ))
			return( (struct xml_element *) 0 );

	if ( document )
		document = document_drop( document );

	return( cords_terminate_private_common_contract( App, id, agent, tls,sla, common ) );
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
	char *	sla,
	char * 	namePlan )
{
	struct	xml_element * document=(struct xml_element *) 0;
	char * 	common=(char *) 0;
	int	location=0;
	if (!( common = cords_resolve_public_common(App,host,id,agent, tls,&location) ))
	{
		if (!( common = cords_build_complex_common( App, host, id, agent, tls, sla, namePlan, &document ) ))
			return( (struct xml_element *) 0 );
		else if (!( common = allocate_string( common ) ))
			return( (struct xml_element *) 0 );
	}

	if ( document )
		document = document_drop( document );

	return( cords_terminate_public_common_contract( App, id, agent, tls, sla, location, common ) );
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
	char *	sla,
	char * namePlan )
{
	char * 	common=(char *) 0;
	int	location=0;
	if (!( common = cords_resolve_public_common(App,host,id,agent, tls,&location) ))
	{
		if (!( common = cords_build_simple_common( App, host, id, agent, tls, sla, namePlan ) ))
			return( (struct xml_element *) 0 );
		else if (!( common = allocate_string( common ) ))
			return( (struct xml_element *) 0 );
	}
	return( cords_terminate_public_common_contract( App, id, agent, tls, sla, location, common ) );
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
	if (!( document_add_atribut( document, _CORDS_ACCOUNT, App->account) ))
		return(document_drop(document));
	else if (!( document_add_atribut( document, _CORDS_TYPE, App->typeApp ) ))
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
	else if (!( App->contract = cords_create_category( document, agent, tls ) ))
		return(document_drop(document));

	/* ----------------------------------------- */
	/* recover the resulting contract identifier */
	/* ----------------------------------------- */
	else if ((status = cords_resolve_location( App->contract, document )) != 0)
		return(document_drop(document));

	/* ----------------------------------------- */
	/* terminate the service contract processing */
	/* ----------------------------------------- */
	else if ((status = cords_terminate_level( document, agent, tls )) != 0)
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
	char *	sla,
	char * namePlan )
{
	/* ----------------------------------------------------------------- */
	/* detect and handle simple type nodes and build a provider contract */
	/* ----------------------------------------------------------------- */
	if (!( strcmp( App->typeApp, _CORDS_SIMPLE ) ))
	{
		if (!( App->scope & _SCOPE_COMMON ))
			return( cords_instance_simple_contract( App, host, id, agent, tls, sla, namePlan ) );
		else if ( App->scope & _ACCESS_PRIVATE )
			return( cords_simple_private_common_contract( App, host, id, agent, tls, sla, namePlan ) );
		else	return( cords_simple_public_common_contract( App, host, id, agent, tls, sla, namePlan ) );
	}
	/* --------------------------------- */
	/* detect an IAAS Complex types node */
	/* --------------------------------- */
	else if (!( strcmp( App->category, _CORDS_MANIFEST ) ))
	{
		if (!( App->scope & _SCOPE_COMMON ))
			return( cords_instance_complex_contract( App, host, id, agent, tls, sla, namePlan ) );
		else if ( App->scope & _ACCESS_PRIVATE )
			return( cords_complex_private_common_contract( App, host, id, agent, tls, sla, namePlan ) );
		else	return( cords_complex_public_common_contract( App, host, id, agent, tls, sla, namePlan ) );
	}
	else 
	{
		if (!( App->scope & _SCOPE_COMMON ))
			return( cords_instance_abstract_contract( App, host, id, agent, tls, sla, namePlan ) );
		else if ( App->scope & _ACCESS_PRIVATE )
			return( cords_abstract_private_common_contract( App, host, id, agent, tls, sla, namePlan ) );
		else	return( cords_abstract_public_common_contract( App, host, id, agent, tls, sla, namePlan ) );
	}
}

/*	-------------------------------------------------------		*/
/*	   	c o r d s _ i n s t a n c e _ n o d e 			*/
/*	-------------------------------------------------------		*/
/*	here we instance a node and create a contract instance		*/
/*	that will ne added to the service of the parent manifest	*/
/*	-------------------------------------------------------		*/
public	struct	xml_element * cords_instance_node( 
		struct cords_placement_criteria * selector,
		struct cords_guarantee_criteria * warranty,
		char * host,
		char * id,
		char * agent,
		char * tls,
		char * sla,
		char * namePlan,
		char * account,
		char * accountName )
{
	int	status;
	struct	xml_element 	*	xroot=(struct xml_element *) 0;
	struct	xml_element 	*	xptr;
	struct	xml_element 	* 	document=(struct xml_element *) 0;
	struct	xml_atribut	*	aptr;

	struct	cords_node_descriptor App;

	memset( &App, 0, sizeof( struct cords_node_descriptor ));
	memcpy( &App.selector, selector, sizeof( struct cords_placement_criteria ) );
	memcpy( &App.warranty, warranty, sizeof( struct cords_guarantee_criteria ) );
	App.scope = _SCOPE_NORMAL | _ACCESS_PRIVATE;
	App.flags = selector->flags;

	if ( warranty )
		App.service = warranty->service;

	if ( check_verbose() )	printf("   CORDS Node \n");

	/* ---------------------------- */
	/* retrieve the node instance 	*/
	/* ---------------------------- */
	if (!( App.node = cords_retrieve_instance( host, id, agent, tls)))
		return((struct xml_element *) 0);
	else if (!( App.account = allocate_string( account ) ))
	{
		cords_terminate_instance_node( &App );
		return((struct xml_element *) 0);
	}
	else if (!( App.accountName = allocate_string( accountName ) ))
	{
		cords_terminate_instance_node( &App );
		return((struct xml_element *) 0);
	}
		
	/* ------------------------------------------ */
	/* retrieve the node name and type attributes */
	/* ------------------------------------------ */
	else if (!(App.nameApp = occi_extract_atribut(App.node,Operator.domain,_CORDS_NODE,_CORDS_NAME)))
	{
		cords_terminate_instance_node( &App );
		return((struct xml_element *) 0);
	}
	else if (!(App.typeApp = occi_extract_atribut(App.node,Operator.domain,_CORDS_NODE,_CORDS_TYPE)))
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
	if (!( App.provider = cords_resolve_provider( App.node, Operator.provider, &App.selector, agent, tls )))
	{
		cords_terminate_instance_node( &App );
		return((struct xml_element *) 0);
	}
	else if (!( App.profile = cords_resolve_profile( App.node, ( App.accountName ? App.accountName : namePlan ) )))
	{
		cords_terminate_instance_node( &App );
		return((struct xml_element *) 0);
	}

	/* ----------------------------------------------------- */
	/* check for scope COMMON and then access PUBLIC/PRIVATE */
	/* ----------------------------------------------------- */
	if ((App.scopeApp = occi_extract_atribut(App.node,Operator.domain,_CORDS_NODE,_CORDS_SCOPE)) != (char *) 0)
	{
		if (!( strcasecmp( App.scopeApp, _CORDS_COMMON )))
			App.scope = _SCOPE_COMMON;
		else if (!( strcasecmp( App.scopeApp, _CORDS_NORMAL )))
			App.scope = _SCOPE_NORMAL;
		else	App.scope = _SCOPE_NORMAL;
		
		if ((App.accessApp = occi_extract_atribut(App.node,Operator.domain,_CORDS_NODE,_CORDS_ACCESS)) != (char *) 0)
		{
			if (!( strcasecmp( App.accessApp, _CORDS_PUBLIC )))
				App.scope |= _ACCESS_PUBLIC;
			else	App.scope |= _ACCESS_PRIVATE;
		}
		else	App.scope |= _ACCESS_PRIVATE;
	}

	/* ------------------------------ */
	/* collect the node type category */
	/* ------------------------------ */
	if (!(App.category = occi_extract_atribut(App.node,Operator.domain,_CORDS_NODE,_CORDS_CATEGORY)))
		App.category = allocate_string( _CORDS_MANIFEST );

	/* ---------------------------------------------------- */
	/* instance or share the service contract for this node */
	/* ---------------------------------------------------- */
	if (!( document = cords_instance_contract( &App, host, id, agent, tls, sla, namePlan ) ))
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
	char * manifest, char * sla,
	char * account,	char * tarif)
{
	struct	occi_response * zptr;
	char *	vptr;
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;

	if (!( eptr = allocate_element()))
		return((struct xml_element *) 0);
	else if (!( eptr->name = allocate_string( _CORDS_SERVICE ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_NAME , name ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_MANIFEST , manifest ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_ACCOUNT , account ) ))
		return(document_drop( eptr ));
	else if ((plan) && (!( aptr = document_add_atribut( eptr, _CORDS_PLAN , plan ) )))
		return(document_drop( eptr ));
	else if ((tarif) && (!( aptr = document_add_atribut( eptr, _CORDS_TARIFICATION , tarif ) )))
		return(document_drop( eptr ));
	else if (!( sla ))
		return( eptr );
	else if (!( aptr = document_add_atribut( eptr, _CORDS_SLA, sla ) ))
		return( eptr );
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

	if (!( zptr =  occi_create_link( plan, instance, agent, tls ) ))
		return(914);
	else	zptr = occi_remove_response( zptr );

	if (!( zptr =  cords_invoke_action( instance, _CORDS_START, agent, tls ) ))
		return(915);
	else	zptr = occi_remove_response( zptr );

	if (!( aptr = document_add_atribut( document, "instance", instance )))
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

/*	----------------------------------------------------------	*/
/*	   a l l o c a t e _ g u a r a n t e e _ e l e m e n t 		*/
/*	----------------------------------------------------------	*/
private	struct cords_guarantee_element * allocate_guarantee_element( struct cords_guarantee_criteria * gptr )
{
	struct	cords_guarantee_element * eptr;
	if (!( eptr = allocate( sizeof( struct cords_guarantee_element ) ) ))
		return( eptr );
	else
	{
		memset( eptr, 0, sizeof( struct cords_guarantee_element ) );
		if (!( eptr->parent = gptr ))
			return( eptr );
		else if (!( eptr->previous = gptr->last ))
			gptr->first = eptr;
		else	gptr->last->next = eptr;
		gptr->last = eptr;
		return( eptr );
	}
}

/*	-------------------------------------------------	*/
/*	c o r d s _ p l a c e m e n t _ c o n d i t i o n	*/
/*	-------------------------------------------------	*/
private	int	cords_placement_condition(
		struct cords_placement_criteria * placement,
		char * nptr, char * vptr )
{
	if (!( strcmp( nptr, "occi.placement.algorithm" ) ))
		placement->algorithm = allocate_string( vptr );
	else if (!( strcmp( nptr, "occi.placement.provider" ) ))
		placement->provider = allocate_string( vptr );
	else if (!( strcmp( nptr, "occi.placement.operator" ) ))
		placement->operator = allocate_string( vptr );
	else if (!( strcmp( nptr, "occi.placement.zone" ) ))
		placement->zone = allocate_string( vptr );
	else if (!( strcmp( nptr, "occi.placement.opinion" ) ))
		placement->opinion = allocate_string( vptr );
	else if (!( strcmp( nptr, "occi.placement.energy" ) ))
		placement->energy = allocate_string( vptr );
	else if (!( strcmp( nptr, "occi.placement.security" ) ))
		placement->security = allocate_string( vptr );
	else if (!( strcmp( nptr, "occi.placement.price" ) ))
		placement->price = allocate_string( vptr );
	return( 0 );
}

/*	-------------------------------------------------	*/
/*	 c o r d s _ w a r r a n t y _ c o n d i t i o n	*/
/*	-------------------------------------------------	*/
private	int	cords_warranty_condition(
		struct cords_guarantee_element * gptr,
		char * nptr, char * eptr, char * vptr )
{
	if (!( gptr ))
		return( 0 );
	/* --------------------------- */
	/* this is a metric identifier */
	/* --------------------------- */
	else if (!( gptr->property = allocate_string( nptr ) ))
		return( 27 );
	else if (!( gptr->objective = allocate_string( vptr ) ))
		return( 27 );
	else if (!( gptr->condition = allocate_string( eptr ) ))
		return( 27 );
	else	return( 0 );
}

/*	-------------------------------------------------	*/
/*	 c o r d s _ a n a l y s e _ c o n d i t i o n s	*/
/*	-------------------------------------------------	*/
/* 	this function retrieves placement conditions from	*/
/*	the sla terms defined conditions block.			*/
/*	-------------------------------------------------	*/
private	int	cords_analyse_warranty( 
		char * host,
		char * termsid,
		struct occi_response * zptr,
		struct cords_guarantee_criteria * warranty,
		char * agent, char * tls )
{
	struct	occi_element * eptr;
	struct	occi_element * fptr;
	char *	id;
	char *	vid;
	char *	vptr;
	char *	nptr;
	char *	cptr;
	struct	occi_response * aptr;
	struct	occi_response * bptr;
	struct	occi_response * xptr;
	int	status=0;
	struct	cords_guarantee_element * wptr;

	if (!( zptr )) 	
		return(0);

	/* ------------------------------------------------------- */
	/* for all "term" elements attached to the "terms" section */
	/* ------------------------------------------------------- */
	for (	eptr=cords_first_link( zptr );
		eptr != (struct occi_element *) 0;
		eptr = eptr->next )
	{
		if (!( eptr->value ))
			continue;
		else if (!( id =  occi_unquoted_link( eptr->value ) ))
			continue;
		else if (!( aptr = cords_retrieve_instance( host, id, agent, tls ) ))
			return( 908 );
		else
		{
			/* ----------------------------------------------------------- */
			/* for all "guarantee" elements attached to the "term" section */
			/* ----------------------------------------------------------- */
			for (	fptr=cords_first_link( aptr );
				fptr != (struct occi_element *) 0;
				fptr = fptr->next )
			{
				if (!( fptr->value ))
					continue;
				else if (!( vid =  occi_unquoted_link( fptr->value ) ))
					continue;
				else if (!( bptr = cords_retrieve_instance( host, vid, agent, tls ) ))
					return( 908 );
				/* -------------------------------------------------- */
				/* allocate a warranty element and append to criteria */
				/* -------------------------------------------------- */
				else if (!( wptr = allocate_guarantee_element( warranty ) ))
					return( 927 );
				else if (!( wptr->reference = allocate_string( vid ) ))
					return( 927 );
				else
				{
					/* ------------------------------ */
					/* retrieve the scope  identifier */
					/* ------------------------------ */
					if (( nptr = occi_extract_atribut( bptr, Operator.domain, _CORDS_GUARANTEE, _CORDS_SCOPE )) != (char *) 0)
						if (!( wptr->scope = allocate_string( nptr ) ))
							return( 927 );

					/* ------------------------------ */
					/* retrieve the obligated value   */
					/* ------------------------------ */
					if (( nptr = occi_extract_atribut( bptr, Operator.domain, _CORDS_GUARANTEE, _CORDS_OBLIGATED )) != (char *) 0)
						if (!( wptr->obligated = allocate_string( nptr ) ))
							return( 927 );

					/* ------------------------------- */
					/* retrieve the importance value   */
					/* ------------------------------- */
					if (( nptr = occi_extract_atribut( bptr, Operator.domain, _CORDS_GUARANTEE, _CORDS_IMPORTANCE )) != (char *) 0)
						if (!( wptr->importance = allocate_string( nptr ) ))
							return( 927 );

					/* ---------------------------------------------------- */
					/* retrieve the condition identifier and resolve fields */
					/* ---------------------------------------------------- */
					if (( nptr = occi_extract_atribut( bptr, Operator.domain, _CORDS_GUARANTEE, _CORDS_VARIABLE )) != (char *) 0)
					{
						if (!( xptr = occi_simple_get( nptr, agent, tls ) ))
							return( 908 );
						else
						{
							/* --------------------------------------------------- */
							/* extract name and values and set the criteria fields */
							/* --------------------------------------------------- */
							if (( nptr = occi_extract_atribut( xptr, Operator.domain, _CORDS_VARIABLE, _CORDS_PROPERTY )) != (char *) 0)
							{
								if (( vptr = occi_extract_atribut( xptr, Operator.domain, _CORDS_VARIABLE, _CORDS_VALUE )) != (char *) 0)
								{
									if (( cptr = occi_extract_atribut( xptr, Operator.domain, _CORDS_VARIABLE, _CORDS_CONDITION )) != (char *) 0)
									{
										cords_warranty_condition(wptr, nptr, cptr, vptr );
									}
								}
							}
							xptr = occi_remove_response( xptr );
						}
					}

					bptr = occi_remove_response( bptr );
					vid = liberate( vid );
					continue;
				}
			}
			aptr = occi_remove_response( aptr );
			id = liberate( id );
		}
	}
	return( 0 );
}

/*	-------------------------------------------------	*/
/*	 c o r d s _ a n a l y s e _ c o n d i t i o n s	*/
/*	-------------------------------------------------	*/
/* 	this function retrieves placement conditions from	*/
/*	the sla terms defined conditions block.			*/
/*	-------------------------------------------------	*/
private	int	cords_analyse_conditions( 
		char * host,
		char * termsid,
		struct occi_response * zptr,
		struct cords_placement_criteria * placement,
		char * agent, char * tls )
{
	struct	occi_element * eptr;
	struct	occi_element * fptr;
	char *	id;
	char *	vid;
	char *	vptr;
	char *	nptr;
	struct	occi_response * aptr;
	struct	occi_response * bptr;
	int	status=0;

	if (!( zptr )) 	
		return(0);

	/* ------------------------------------------------------- */
	/* for all "term" elements attached to the "terms" section */
	/* ------------------------------------------------------- */
	for (	eptr=cords_first_link( zptr );
		eptr != (struct occi_element *) 0;
		eptr = eptr->next )
	{
		if (!( eptr->value ))
			continue;
		else if (!( id =  occi_unquoted_link( eptr->value ) ))
			continue;
		else if (!( aptr = cords_retrieve_instance( host, id, agent, tls ) ))
			return( 908 );
		else
		{
			/* ---------------------------------------------------------- */
			/* for all "variable" elements attached to the "term" section */
			/* ---------------------------------------------------------- */
			for (	fptr=cords_first_link( aptr );
				fptr != (struct occi_element *) 0;
				fptr = fptr->next )
			{
				if (!( fptr->value ))
					continue;
				else if (!( vid =  occi_unquoted_link( fptr->value ) ))
					continue;
				else if (!( bptr = cords_retrieve_instance( host, vid, agent, tls ) ))
					return( 908 );
				else
				{
					/* --------------------------------------------------- */
					/* extract name and values and set the criteria fields */
					/* --------------------------------------------------- */
					if (( nptr = occi_extract_atribut( bptr, Operator.domain, _CORDS_VARIABLE, _CORDS_PROPERTY )) != (char *) 0)
					{
						if (( vptr = occi_extract_atribut( bptr, Operator.domain, _CORDS_VARIABLE, _CORDS_VALUE )) != (char *) 0)
						{
							cords_placement_condition(placement, nptr, vptr );
						}
					}
					bptr = occi_remove_response( bptr );
					vid = liberate( vid );
				}
			}
			aptr = occi_remove_response( aptr );
			id = liberate( id );
		}
	}
	return( 0 );
}

/*	-------------------------------------------------	*/
/*	c o r d s _ r e t r i e v e _ c o n d i t i o n s	*/
/*	-------------------------------------------------	*/
/* 	this function retrieves placement conditions from	*/
/*	the sla terms defined conditions block.			*/
/*	-------------------------------------------------	*/
private	int	cords_retrieve_conditions( 
		char * host,
		char * slaid, 
		struct occi_response * zptr,
		struct cords_placement_criteria * placement,
		struct cords_guarantee_criteria * warranty,
		char * agent, char * tls )
{
	struct	occi_element * eptr;
	char *	id;
	char *	vptr;
	struct	occi_response * aptr;
	int	status=0;

	if (!( zptr )) 	
		return(0);

	/* -------------------------------------------- */
	/* for all "terms" attached to the sla instance */
	/* -------------------------------------------- */
	for (	eptr=cords_first_link( zptr );
		eptr != (struct occi_element *) 0;
		eptr = eptr->next )
	{
		if (!( eptr->value ))
			continue;
		else if (!( id =  occi_unquoted_link( eptr->value ) ))
			continue;
		/* ------------------------- */
		/* retrieve the terms record */
		/* ------------------------- */
		else if (!( aptr = cords_retrieve_instance( host, id, agent, tls ) ))
			return( 908 );
		else if (( vptr = occi_extract_atribut( aptr, Operator.domain, _CORDS_TERMS, _CORDS_TYPE )) != (char *) 0)
		{
			if (!( strcmp( vptr, _CORDS_CONDITIONS ) ))
				status = cords_analyse_conditions( host, id, aptr, placement, agent, tls );
			else if (!( strcmp( vptr, _CORDS_GUARANTEES ) ))
				status = cords_analyse_warranty( host, id, aptr, warranty, agent, tls );
			aptr = occi_remove_response( aptr );
			id = liberate( id );
			continue;
		}
	}
	return( 0 );
}

/*	-------------------------------------------------------		*/
/*	     c o r d s _ b r o k e r i g _ a c c o u n t		*/
/*	-------------------------------------------------------		*/
private	char *	cords_brokering_account(
	struct  cords_provisioning * CbC,
	struct	cords_placement_criteria * CpC,
	struct	cords_guarantee_criteria * CgC,
	char *	host, 
	char * 	sla, 
	char * 	agent, 
	char * 	tls )
{
	int	status;

	if (!( sla ))
	{
		/* -------------------------------------------------- */
		/* retrieve the account information instance and name */
		/* -------------------------------------------------- */
		if (( CbC->accID  = occi_extract_atribut( CbC->manifest, Operator.domain, _CORDS_MANIFEST, _CORDS_ACCOUNT )) 
				!= (char *) 0)
		{
			if (!( CbC->account = cords_retrieve_instance( host, CbC->accID, agent, tls )))
				return( cords_terminate_provisioning( 905, CbC ) );
			else if (!( CbC->accName  = occi_extract_atribut( CbC->account, Operator.domain, _CORDS_ACCOUNT, _CORDS_NAME ))) 
				return( cords_terminate_provisioning( 905, CbC ) );
		}
	}
	else
	{
		/* -------------------------------------------------- */
		/* retrieve the S.L.A   information instance and name */
		/* -------------------------------------------------- */
		if (!( CbC->slaID  = allocate_string( sla ) ))
			return( cords_terminate_provisioning( 920, CbC ) );
		else if (!( CbC->sla = cords_retrieve_instance( host, CbC->slaID, agent, tls )))
			return( cords_terminate_provisioning( 930, CbC ) );
		else if ((status = cords_retrieve_conditions( host, CbC->slaID, CbC->sla, CpC, CgC, agent, tls )) != 0)
			return( cords_terminate_provisioning( 907, CbC ) );
	
		/* -------------------------------------------------- */
		/* retrieve the account information instance and name */
		/* -------------------------------------------------- */
		if (( CbC->accID  = occi_extract_atribut( CbC->sla, Operator.domain, _CORDS_AGREEMENT, _CORDS_INITIATOR ))
				!= (char *) 0)
		{
			if (!( CbC->account = cords_retrieve_instance( host, CbC->accID, agent, tls )))
				return( cords_terminate_provisioning( 905, CbC ) );
			else if (!( CbC->accName  = occi_extract_atribut( CbC->account, Operator.domain, _CORDS_ACCOUNT, _CORDS_NAME ))) 
				return( cords_terminate_provisioning( 905, CbC ) );
		}
	}
	return("OK");
}

/*	-------------------------------------------------------		*/
/*		c o r d s _ v a l i d _ i d e n t i f i e r		*/
/*	-------------------------------------------------------		*/
private	char *	cords_valid_identifier( char * id )
{
	if (!( id ))
		return( id );
	else if (!( strlen( id ) ))
		return((char *) 0);
	else 	return( id );
}

/*	---------------------------------------------------	*/
/*		c o r d s _ b u i l d _ c o n t r o l		*/
/*	---------------------------------------------------	*/
/*	builds a control element for sla guarantee monitors	*/
/*	that will perform probe data surveillance and issue	*/
/*	penalty or reward business values.			*/
/*	---------------------------------------------------	*/ 
private	char *	cords_build_control( char * contract, char * sla, struct cords_guarantee_element * gptr, char * agent, char * tls )
{
	struct	occi_client 	* kptr;
	struct	occi_request	* qptr;
	struct	occi_response 	* yptr;
	struct	occi_element 	* dptr;
	char	buffer[2048];
	char *	ihost;

	if (!( ihost = occi_resolve_category_provider( _CORDS_CONTROL, agent, tls ) ))
		return((char *) 0);
	else	
	{
		sprintf(buffer,"%s/%s/",ihost,_CORDS_CONTROL);
		liberate( ihost );
	}

	if (!( kptr = occi_create_client( buffer, agent, tls ) ))
		return((char *) 0);

	else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
	{
		kptr = occi_remove_client( kptr );
		return((char *) 0);
	}
	else if ((!(dptr=occi_request_element(qptr,"occi.control.contract"  , contract 	 	) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.control.name" 	    , contract	 	) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.control.agreement" , sla	 	) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.control.property"  , gptr->property 	) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.control.condition" , gptr->condition	) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.control.objective" , gptr->objective	) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.control.reference" , gptr->reference	) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.control.importance", gptr->importance	) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.control.obligated" , gptr->obligated	) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.control.scope"	    , gptr->scope	) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.control.monitor"   , ""	       	) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.control.connection", ""	       	) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.control.metric"    , ""	       	) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.control.probe"     , ""	       	) )))
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
	else if (!( ihost = allocate_string( ihost ) ))
	{
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return((char *) 0);
	}
	else
	{
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return( ihost );
	}
}

/*	-------------------------------------------------------		*/
/*		c o r d s _ c h e c k _ s c o p e			*/
/*	-------------------------------------------------------		*/
private	int	cords_check_scope( char * test, char * scope )
{
	if (!( strcasecmp( scope, "default") ))
		return(1);
	else if (!( strcasecmp( scope, "any") ))
		return(1);
	else if (!( strcasecmp( scope, "all") ))
		return(1);
	else if (!( strcmp( test, scope ) ))
		return(1);
	else	return(0);
}

/*	-------------------------------------------------------		*/
/*	    c o r d s _ v a l i d a t e _ s c o p e 			*/
/*	-------------------------------------------------------		*/
/*	we arrive here with a node name and a scope description		*/
/*	if the scope is null, default, any and all then OK		*/
/*	if the scope is an exact match with node name then OK		*/
/*	otherwise the complex scope string may specify a mix of		*/
/*	equations or negations where an exact match will respond	*/
/*	as required by its prefix or absence. The list of scope		*/
/*	elements may be comma, semicolon or space separated:		*/
/*									*/
/*		"=one,!two;!three =four"				*/
/*									*/
/*	-------------------------------------------------------		*/
 
private	int	cords_validate_scope( char * test, char * scope )
{
	int	result=0;
	int	negation=0;
	char *	root;
	char *	sptr;
	char *	wptr;
	/* --------------------------- */
	/* ensure scope : remove space */
	/* --------------------------- */
	if (!( scope ))
		return(1);
	else 
	{
		while ( *scope == ' ' ) scope++;
	}

	/* --------------------------- */
	/* handle the easy stuff first */
	/* --------------------------- */
	if ( cords_check_scope( test, scope ) )
		return( 1 );
	/* -------------------------- */
	/* handle the hard stuff next */		
	/* -------------------------- */
	else if (!( sptr = allocate_string( scope ) ))
		return(1);
	else	root = sptr;

	while ( *sptr )
	{
		switch ( *sptr )
		{
		case	'!'	:
			negation = 1;
			sptr++;
			while ( *sptr == ' ' ) sptr++;
			break;
		case	'='	:
			negation = 0;
			sptr++;
			while ( *sptr == ' ' ) sptr++;
			break;
		default		:
			negation=0;
			break;
		}

		for ( wptr=sptr; *wptr != 0; wptr++ )
			if (( *wptr == ',' )
			||  ( *wptr == ' ' )
			||  ( *wptr == ';' ))
				break;

		if ( *wptr )
			*(wptr++) = 0;
		if ((result = cords_check_scope( test, sptr )) != 0)
			break;
		else
		{
			result = negation = 0;
			sptr = wptr;
			continue;
		}
	}
	liberate( root );
	return( ( negation ? !result : result ) );
}

/*	-------------------------------------------------------		*/
/*	    c o r d s _ s e r v i c e _ g u a r a n t e e s		*/
/*	-------------------------------------------------------		*/
private	int	cords_service_guarantees( 
	char * host, 
	char *	sla,
	struct xml_element * document, 
	struct cords_guarantee_criteria * warranty, 
	char * agent, char * tls )
{
	char *	control;
	struct	xml_element * xptr;
	struct	xml_atribut * aptr;
	struct	xml_atribut * bptr;
	struct	cordscript_action * csptr=(struct cordscript_action*) 0;
	struct	cords_guarantee_element * gptr;
	char	statement[4096];
	/* ------------- */
	/* for all nodes */
	/* ------------- */
	for (	xptr = document->first;
		xptr != (struct xml_element *) 0;
		xptr = xptr->next )
	{
		/* detect a guarantee */
		/* ------------------ */
		for (	gptr=warranty->first;
			gptr != (struct cords_guarantee_element *) 0;
			gptr = gptr->next )
		{
			if (!( gptr->scope ))
				continue;

			else if (!( aptr = document_atribut( xptr, _CORDS_NAME ) ))
				continue;
			else if (!( cords_validate_scope( aptr->value, gptr->scope ) ))
				continue;
			else if (!( gptr->property ))
				continue;
			else if (!( bptr = document_atribut( xptr, _CORDS_ID ) ))
				continue;
			else if (!( rest_valid_string( bptr->value ) ))
				continue;
			else
			{
				 if (!( control = cords_build_control( bptr->value, sla, gptr, agent, tls )))
				{
					/* monitor without control */
					sprintf(statement,"%s.monitor(slam.%s);",
						aptr->value, gptr->property );
				}
				else
				{
					/* monitor with control */
					sprintf(statement,"%s.monitor(slam.%s,%c%s%c);",
						aptr->value, gptr->property,
						0x0022,control,0x0022 );
					control = liberate( control );
				}
				if ((csptr = cordscript_parse_statement( statement )) != (struct cordscript_action *) 0)
					cords_action_instruction( host, document, csptr,_CORDS_CONFIGURATION, agent, tls );
			}
		}  

	}
	return( 0 );
}

/*	-------------------------------------------------------		*/
/*		c o r d s _ s e r v i c e _ b r o k e r			*/
/*	-------------------------------------------------------		*/
/*	this function provides SLA driven service brokering for		*/
/*	fine control over the placement engine and sla accounts		*/
/*	handling over manifest described resources.			*/
/*	-------------------------------------------------------		*/
public	char *	cords_service_broker(
	char * 	host, 
	char * 	service,
	char * 	servicename,
	char *	plan,
	char * 	manifest, 
	char *	sla,
	char * 	agent, 
	char * 	tls, 
	struct xml_element ** root )
{
	char *	resultid=(char *) 0;
	int	status;
	char *	id;
	struct	occi_response * zptr;
	struct	occi_element * eptr;
	struct	xml_atribut * aptr;
	struct	xml_element  * mptr;
	struct	xml_atribut  * nptr;
	struct  cords_provisioning CbC; 
	struct	cords_placement_criteria CpC;
	struct cords_guarantee_criteria CgC;

	memset(&CbC,0, sizeof( struct cords_provisioning ) );
	memset(&CpC,0, sizeof( struct cords_placement_criteria ) );
	memset(&CgC,0, sizeof( struct cords_guarantee_criteria ) );
	CbC.placement = &CpC;
	CbC.warranty  = &CgC;
	CgC.service   = service;
	
	if ( check_verbose() )
		printf("   CORDS Service Broker ( %s ) Phase 1 : Preparation \n", agent);

	/* -------------------------------------------------- */
	/* retrieve the manifestinformation instance and name */
	/* -------------------------------------------------- */
	if (!( CbC.planID  = allocate_string( plan ) ))
		return( cords_terminate_provisioning( 900, &CbC ) );
	if (!( CbC.namePlan  = allocate_string( servicename ) ))
		return( cords_terminate_provisioning( 900, &CbC ) );
	if (!( CbC.reqID  = allocate_string( manifest ) ))
		return( cords_terminate_provisioning( 900, &CbC ) );
	else if (!( CbC.manifest = cords_retrieve_instance( host, CbC.reqID, agent, tls )))
		return( cords_terminate_provisioning( 910, &CbC ) );

	/* ----------------------------------------------- */
	/* handle the account from the sla or the manifest */
	/* ----------------------------------------------- */
	if (!( resultid = cords_brokering_account( &CbC, &CpC, &CgC, host, sla, agent, tls ) ))
		return( id );

	/* ----------------------------------- */
	/* retrieve the configuration instance */
	/* ----------------------------------- */
	else if (!( CbC.confID = occi_extract_atribut( CbC.manifest,Operator.domain,_CORDS_MANIFEST,_CORDS_CONFIGURATION)))
		return( cords_terminate_provisioning( 907, &CbC ) );
	else if (!( CbC.confID = cords_valid_identifier( CbC.confID ) ))
		return( cords_terminate_provisioning( 907, &CbC ) );
	else if (!( CbC.configuration = cords_retrieve_instance( host, CbC.confID, agent, tls)))
		return( cords_terminate_provisioning( 908, &CbC ) );

	/* ----------------------------- */
	/* retrieve the release instance */
	/* ----------------------------- */
	if ((( CbC.releaseID = occi_extract_atribut( CbC.manifest,Operator.domain,
		_CORDS_MANIFEST,_CORDS_RELEASE)) != (char *) 0)
	&& (( CbC.releaseID = cords_valid_identifier( CbC.releaseID )) != (char *) 0))
	{
		if (!( CbC.release = cords_retrieve_instance( host, CbC.releaseID, agent, tls)))
			return( cords_terminate_provisioning( 908, &CbC ) );
	}

	/* ------------------------------- */
	/* retrieve the interface instance */
	/* ------------------------------- */
	if ((( CbC.interID = occi_extract_atribut( CbC.manifest,Operator.domain,
		_CORDS_MANIFEST,_CORDS_INTERFACE)) != (char *) 0)
	&& (( CbC.interID = cords_valid_identifier( CbC.interID )) != (char *) 0))
	{
		if (!( CbC.interface = cords_retrieve_instance( host, CbC.interID, agent, tls)))
			return( cords_terminate_provisioning( 908, &CbC ) );
	}

	/* -------------------------------------- */
	/* build the service description document */
	/* -------------------------------------- */
	if (!( CbC.document = cords_build_service( CbC.planID, CbC.namePlan, CbC.reqID, CbC.slaID, CbC.accID, CbC.accID ) ))
		return( cords_terminate_provisioning( 909, &CbC ) );

	/* ------------------------ */
	/* add the service identity */
	/* ------------------------ */
	else if (!( aptr = document_add_atribut( CbC.document, _CORDS_ID, service ) ))
		return( cords_terminate_provisioning( 910, &CbC ) );

	/* --------------------------------------- */
	/* instance the contracts for this service */
	/* --------------------------------------- */
	for (	eptr=cords_first_link( CbC.manifest );
		eptr != (struct occi_element *) 0;
		eptr = eptr->next )
	{
		if (!( eptr->value ))
			continue;
		if (!( id =  occi_unquoted_link( eptr->value ) ))
			continue;
		else if (!( mptr = cords_instance_node( &CpC, &CgC, host, id, agent, tls, sla, CbC.namePlan, CbC.accID, CbC.accName ) ))
			return( cords_terminate_provisioning( 913, &CbC ) );
		else if (!( nptr = document_atribut( mptr, _CORDS_ID ) ))
			return( cords_terminate_provisioning( 914, &CbC ) );
		else if (!( zptr =  occi_create_link( service, nptr->value,  agent, tls ) ))
			return( cords_terminate_provisioning( 915, &CbC ) );
		{
			zptr = occi_remove_response( zptr );
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

	/* -------------------------------------------- */
	/* perform configuration instruction processing */
	/* -------------------------------------------- */
	if (( status = cords_broker_configuration( host, CbC.document, CbC.configuration, agent, tls )) != 0)
		return( cords_terminate_provisioning( status, &CbC ) );
	

	/* -------------------------------------------- */
	/* perform SLA guarantee instruction processing */
	/* -------------------------------------------- */
	if ((( sla ) && ( CgC.first ))
	&&  ((status = cords_service_guarantees( host, sla, CbC.document, &CgC, agent, tls )) != 0))
		return( cords_terminate_provisioning( status, &CbC ) );

	/* -------------------------------------- */
	/* perform release instruction processing */
	/* -------------------------------------- */
	if (( CbC.release )
	&&  (( status = cords_broker_release( host, CbC.document, CbC.release, agent, tls )) != 0))
		return( cords_terminate_provisioning( status, &CbC ) );
	

	/* -------------------------------------------- */
	/* perform interface methods action  processing */
	/* -------------------------------------------- */
	if (( CbC.interface )
	&&  ((status = cords_broker_interface( host, CbC.document, CbC.interface, agent, tls )) != 0))
		return( cords_terminate_provisioning( status, &CbC ) );

	else
	{
		cords_terminate_provisioning( 0, &CbC );
		return( resultid );
	}
}

/*	-------------------------------------------------------		*/
/*		c o r d s _ r e q u e s t _ b r o k e r			*/
/*	-------------------------------------------------------		*/
public	char *	cords_manifest_broker(
	char * 	host, 
	char * plan, char * nameplan, char * manifest, char * agent, char * tls, char * sla, struct xml_element ** root )
{
	int	status;
	char	*	id;
	struct	cords_placement_criteria CpC;
	struct	occi_element * eptr;
	struct	xml_atribut * aptr;
	struct	xml_element  * mptr;
	struct cords_provisioning CbC; 
	struct cords_guarantee_criteria CgC;

	memset(&CbC,0, sizeof( struct cords_provisioning ) );
	memset(&CpC,0, sizeof( struct cords_placement_criteria ) );
	memset(&CgC,0, sizeof( struct cords_guarantee_criteria ) );
	CbC.warranty = &CgC;

	if ( check_verbose() )
		printf("   CORDS Request Broker ( %s ) Phase 1 : Preparation \n", agent);

	if (!( CbC.planID = allocate_string( plan )))
		return( cords_terminate_provisioning( 900, &CbC ) );
	else if (!( CbC.namePlan = allocate_string( nameplan ) ))
		return( cords_terminate_provisioning( 901, &CbC ) );
	else if (!( CbC.reqID  = allocate_string( manifest ) ))
		return( cords_terminate_provisioning( 902, &CbC ) );
	else if (!( CbC.manifest = cords_retrieve_instance( host, CbC.reqID, agent, tls )))
		return( cords_terminate_provisioning( 904, &CbC ) );

	/* ----------------------------------------------- */
	/* handle the account from the sla or the manifest */
	/* ----------------------------------------------- */
	if (!( id = cords_brokering_account( &CbC, &CpC, &CgC, host, sla, agent, tls ) ))
		return( id );

	/* ---------------------------------------- */
	/* retrieve the security procedure instance */
	/* ---------------------------------------- */
	if (( CbC.secID  = occi_extract_atribut( CbC.manifest, Operator.domain, _CORDS_MANIFEST, _CORDS_SECURITY )) 
		!= (char *) 0)
	{
		if (!( CbC.security = cords_retrieve_instance( host, CbC.secID, agent, tls )))
			return( cords_terminate_provisioning( 906, &CbC ) );
	}

	/* ----------------------------------- */
	/* retrieve the configuration instance */
	/* ----------------------------------- */
	if (!( CbC.confID = occi_extract_atribut( CbC.manifest,Operator.domain,_CORDS_MANIFEST,_CORDS_CONFIGURATION)))
		return( cords_terminate_provisioning( 907, &CbC ) );
	else if (!( CbC.confID = cords_valid_identifier( CbC.confID ) ))
		return( cords_terminate_provisioning( 907, &CbC ) );
	else if (!( CbC.configuration = cords_retrieve_instance( host, CbC.confID, agent, tls)))
		return( cords_terminate_provisioning( 908, &CbC ) );

	/* ----------------------------- */
	/* retrieve the release instance */
	/* ----------------------------- */
	if ((( CbC.releaseID = occi_extract_atribut( CbC.manifest,Operator.domain,
		_CORDS_MANIFEST,_CORDS_RELEASE)) != (char *) 0)
	&& (( CbC.releaseID = cords_valid_identifier( CbC.releaseID )) != (char *) 0))
	{
		if (!( CbC.release = cords_retrieve_instance( host, CbC.releaseID, agent, tls)))
			return( cords_terminate_provisioning( 908, &CbC ) );
	}

	/* ------------------------------- */
	/* retrieve the interface instance */
	/* ------------------------------- */
	if ((( CbC.interID = occi_extract_atribut( CbC.manifest,Operator.domain,
		_CORDS_MANIFEST,_CORDS_INTERFACE)) != (char *) 0)
	&& (( CbC.interID = cords_valid_identifier( CbC.interID )) != (char *) 0))
	{
		if (!( CbC.interface = cords_retrieve_instance( host, CbC.interID, agent, tls)))
			return( cords_terminate_provisioning( 908, &CbC ) );
	}

	if ( check_verbose() )
		printf("   CORDS Request Broker ( %s ) Phase 2 : Provisioning \n",agent);

	/* ---------------------------------- */
	/* build the service control document */
	/* ---------------------------------- */
	if (!( CbC.document = cords_build_service( CbC.planID, CbC.namePlan, CbC.reqID, CbC.slaID, CbC.accID, CbC.accID ) ))
		return( cords_terminate_provisioning( 909, &CbC ) );

	else if (!( CbC.instance = cords_create_category( CbC.document, agent, tls ) ))
		return( cords_terminate_provisioning( 910, &CbC ) );

	else if ((status = cords_resolve_location( CbC.instance, CbC.document )) != 0)
		return( cords_terminate_provisioning( status, &CbC ) );
	else if (!( aptr = document_atribut( CbC.document, _CORDS_ID )))
		return( cords_terminate_provisioning( 911, &CbC ) );
	else if (!( CbC.instID = occi_unquoted_value( aptr->value ) ))
		return( cords_terminate_provisioning( 912, &CbC ) );
	else	CgC.service = CbC.instID;

	for (	eptr=cords_first_link( CbC.manifest );
		eptr != (struct occi_element *) 0;
		eptr = eptr->next )
	{
		if (!( eptr->value ))
			continue;
		if (!( id =  occi_unquoted_link( eptr->value ) ))
			continue;
		else if (!( mptr = cords_instance_node( &CpC, &CgC, host, id, agent, tls, sla, CbC.namePlan, CbC.accID, CbC.accName ) ))
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
	(void) cords_terminate_level( CbC.document, agent, tls );

	if ( check_verbose() )
		printf("   CORDS Request Broker ( %s ) Provisioned %u Nodes \n",agent,CbC.nodes);

	if ( check_verbose() )
		printf("   CORDS Request Broker ( %s ) Phase 3 : Configuration \n",agent);

	/* -------------------------------------------- */
	/* perform configuration instruction processing */
	/* -------------------------------------------- */
	if (( status = cords_broker_configuration( host, CbC.document, CbC.configuration, agent, tls )) != 0)
		return( cords_terminate_provisioning( status, &CbC ) );
	
	/* -------------------------------------------- */
	/* perform SLA guarantee instruction processing */
	/* -------------------------------------------- */
	if ((( sla ) && ( CgC.first ))
	&&  ((status = cords_service_guarantees( host, sla, CbC.document, &CgC, agent, tls )) != 0))
		return( cords_terminate_provisioning( status, &CbC ) );

	/* -------------------------------------- */
	/* perform release instruction processing */
	/* -------------------------------------- */
	if (( CbC.release )
	&&  (( status = cords_broker_release( host, CbC.document, CbC.release, agent, tls )) != 0))
		return( cords_terminate_provisioning( status, &CbC ) );
	
	/* -------------------------------------------- */
	/* perform interface methods action  processing */
	/* -------------------------------------------- */
	if (( CbC.interface )
	&&  ((status = cords_broker_interface( host, CbC.document, CbC.interface, agent, tls )) != 0))
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


