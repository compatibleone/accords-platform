/* ------------------------------------------------------------------------------------	*/
/*				 CompatibleOne Cloudware				*/
/* ------------------------------------------------------------------------------------ */
/*											*/
/* Ce fichier fait partie de ce(tte) oeuvre de Iain James Marshall et est mise a 	*/
/* disposition selon les termes de la licence Creative Commons Paternit‚ : 		*/
/*											*/
/*			 	Pas d'Utilisation Commerciale 				*/
/*				Pas de Modification 					*/
/*				3.0 non transcrit.					*/
/*											*/
/* ------------------------------------------------------------------------------------ */
/* 			Copyright (c) 2011 Iain James Marshall for Prologue 		*/
/*				   All rights reserved					*/
/* ------------------------------------------------------------------------------------ */
#ifndef	_cords_broker_c
#define	_cords_broker_c

#include "cb.h"
#include "csp.c"
#include "restlog.h"

/*	-------------------------------------------------------		*/
/*	t e r m i n a t e _ c o r d s _ p r o v i s i o n i n g		*/
/*	-------------------------------------------------------		*/
private	int	cords_terminate_provisioning( 
		int status, 
		struct cords_provisioning * pptr )
{
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

	return( status );
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
		else if (!( strcmp( eptr->name, buffer ) ))
			return( occi_unquoted_value( eptr->value ) );
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
		else if (!( strcmp( bptr->value , coreappname  ) ))
			break;
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

/*	-------------------------------------------------------		*/
/*	  c o r d s _ a c t i o n _ i n s t r u c t i o n		*/
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
	char	buffer[2048];

	if (!( lptr = action->lvalue ))
		return( 78 );
	else if (!( lptr->prefix ))
		return( 30 );


	if (!( aptr = cords_resolve_contract_id( document, lptr->prefix ) ))
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
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.member"  	, "configure" 	) ))
		     ||  (!(dptr=occi_request_element(qptr,"occi.instruction.type"  	, "method" 	) ))
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

/*	-------------------------------------------------------		*/
/*	  c o r d s _ b r o k e r _ c o n f i g u r a t i o n		*/
/*	-------------------------------------------------------		*/
/*	this function is called from the service instance build		*/
/*	once the contract instances have been created and their		*/
/*	details are resolved and established. 				*/
/*	Here we will parse the collection of action statements		*/
/*	to produce the corresponding concrete configuration 		*/
/*	actions required to communicate the run time parameter		*/
/*	values to the provider instantiation mechanisms.		*/
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

	/* -------------------------------- */
	/* select a list of provider records */
	/* -------------------------------- */
	else if (!( yptr = cords_retrieve_named_instance_list( 
		cptr->value, "occi.contract.name", nptr->value, agent,tls )))
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

/*	-------------------------------------------------------		*/
/*		c o r d s _ b u i l d _ c o n t r a c t  			*/
/*	-------------------------------------------------------		*/
/*	builds an XML document form of a contract instance of a		*/
/*	cords request/plan.						*/
/*	-------------------------------------------------------		*/
private	struct	xml_element * 	cords_build_contract( 
		char * core, 	char * name, 
		char * provider )
{
	struct	xml_element * eptr;
	struct	xml_element * xptr;
	struct	xml_atribut * aptr;

	if (!( eptr = allocate_element()))
		return((struct xml_element *) 0);
	else if (!( eptr->name = allocate_string( _CORDS_CONTRACT ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_NAME, name ) ))
		return(document_drop( eptr ));
	else if (!( aptr = document_add_atribut( eptr, _CORDS_CORE, core ) ))
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


/*	-------------------------------------------------------		*/
/*	   c o r d s _ t e r m i n a t e _ a p p l i a n c e		*/
/*	-------------------------------------------------------		*/
private	void 	cords_terminate_core( struct cords_core_descriptor * dptr )
{
	if ( dptr->nameApp )	dptr->nameApp = liberate( dptr->nameApp );
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
	if ( dptr->core )	dptr->core = occi_remove_response( dptr->core );
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
	printf("   Cords Default Provider : %s \n",defaut);
	return( allocate_string( defaut ) );
}

/*	-------------------------------------------------------		*/
/*		c o r d s _ r e s o l v e _ p r o v i d e r		*/
/*	-------------------------------------------------------		*/
private	char *	cords_resolve_provider( struct occi_response * core, char * defaut, char * agent,char * tls )
{
	char	*	value;
	/* ----------------------------------------- */
	/* retrieve the provider information instance */
	/* ----------------------------------------- */
	if (!( value = cords_extract_atribut( core, "occi", _CORDS_CORE, _CORDS_PROVIDER ))) 
		return( cords_research_provider(defaut,agent,tls) );
	else if (!( strcmp(value,_CORDS_NULL) ))
		return( cords_research_provider(defaut,agent,tls) );
	else if (!( strcmp(value,_CORDS_ANY) ))
		return( cords_research_provider(defaut,agent,tls) );
	else
	{
		printf("   Cords Explicite Provider : %s \n",value);
		return( value );
	}
}

/*	-------------------------------------------------------		*/
/*		c o r d s _ r e s o l v e _ p r o f i l e		*/
/*	-------------------------------------------------------		*/
private	char *	cords_resolve_profile( struct occi_response * core, char * defaut )
{
	char	*	value;
	/* ----------------------------------------- */
	/* retrieve the provider information instance */
	/* ----------------------------------------- */
	if (!( value = cords_extract_atribut( core, "occi", _CORDS_CORE, _CORDS_PROFILE ))) 
		return( allocate_string( defaut ) );
	else if (!( strcmp(value,_CORDS_NULL) ))
		return( allocate_string( defaut ) );
	else	return( value );
}


/*	-------------------------------------------------------		*/
/*	   c o r d s _ i n s t a n c e _ a p p l i a n c e		*/
/*	-------------------------------------------------------		*/
private	struct	xml_element * cords_instance_core( 
		char * host,
		char * id,
		char * agent,
		char * tls,
		char * namePlan )
{
	int	status;
	struct	xml_element 	*	xptr;
	struct	xml_element 	* 	document=(struct xml_element *) 0;
	struct	xml_atribut	*	aptr;

	struct	cords_core_descriptor App = {
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

	if ( check_verbose() )
		printf("   CORDS Core \n");

	/* ------------------------------- */
	/* retrieve the core instance */
	/* ------------------------------- */
	if (!( App.core = cords_retrieve_instance( host, id, agent,tls)))
		return((struct xml_element *) 0);

	else if (!(App.nameApp = cords_extract_atribut(App.core,"occi",_CORDS_CORE,_CORDS_NAME)))
	{
		cords_terminate_core( &App );
		return((struct xml_element *) 0);
	}
	else if (!( App.provider = cords_resolve_provider( App.core, "openstack", agent,tls )))
	{
		cords_terminate_core( &App );
		return((struct xml_element *) 0);
	}
	else if (!( App.profile = cords_resolve_profile( App.core, namePlan )))
	{
		cords_terminate_core( &App );
		return((struct xml_element *) 0);
	}
	else if (!( document = cords_build_contract( id, App.nameApp, App.provider )))
	{
		cords_terminate_core( &App );
		return((struct xml_element *) 0);
	}
	else if (!( xptr = cords_add_provider( document, App.provider, App.nameApp, App.profile ) ))
	{
		cords_terminate_core( &App );
		return((struct xml_element *) 0);
	}
	else if (!( App.providerid = cords_create_provider( xptr, agent,tls ) ))
	{
		cords_terminate_core( &App );
		return((struct xml_element *) 0);
	}
	else if (!( aptr = document_add_atribut( xptr, _CORDS_ID, App.providerid ) ))
	{
		cords_terminate_core( &App );
		return((struct xml_element *) 0);
	}

	/* ------------------------------------ */
	/* retrieve the infrastructure instance */
	/* ------------------------------------ */
	if (( App.hid  = cords_extract_atribut( App.core, "occi", 
			_CORDS_CORE, _CORDS_INFRASTRUCTURE )) != (char *) 0)
	{
		if (!( App.hardware = cords_retrieve_instance( host, App.hid, agent,tls )))
		{
			cords_terminate_core( &App );
			return(document_drop(document));
		}
		else	App.hid = liberate( App.hid );

		/* retrieve the compute instance */
		/* ----------------------------- */
		if (( App.hid  = cords_extract_atribut( App.hardware, "occi", 
				_CORDS_INFRASTRUCTURE, _CORDS_COMPUTE )) != (char *) 0)
		{
			if (!( App.compute = cords_retrieve_instance( host, App.hid, agent,tls )))
			{
				cords_terminate_core( &App );
				return(document_drop(document));
			}
			else	App.hid = liberate( App.hid );
		}
		else	App.compute = (struct occi_response *) 0;

		/* retrieve the storage instance */
		/* ----------------------------- */
		if (( App.hid  = cords_extract_atribut( App.hardware, "occi", 
				_CORDS_INFRASTRUCTURE, _CORDS_STORAGE )) != (char *) 0)
		{
			if (!( App.storage = cords_retrieve_instance( host, App.hid, agent,tls )))
			{
				cords_terminate_core( &App );
				return(document_drop(document));
			}
			else	App.hid = liberate( App.hid );
		}
		else	App.storage = (struct occi_response *) 0;

		/* retrieve the network instance */
		/* ----------------------------- */
		if (( App.hid  = cords_extract_atribut( App.hardware, "occi", 
				_CORDS_INFRASTRUCTURE, _CORDS_NETWORK )) != (char *) 0)
		{
			if (!( App.network = cords_retrieve_instance( host, App.hid, agent,tls )))
			{
				cords_terminate_core( &App );
				return(document_drop(document));
			}
			else	App.hid = liberate( App.hid );
		}
		else	App.network = (struct occi_response *) 0;
	}
	else	App.hardware = (struct occi_response *) 0;

	/* ------------------------------------ */
	/* retrieve the infrastructure instance */
	/* ------------------------------------ */
	if (( App.sid  = cords_extract_atribut( App.core, "occi", 
			_CORDS_CORE, _CORDS_IMAGE )) != (char *) 0)
	{
		if (!( App.software = cords_retrieve_instance( host, App.sid, agent,tls )))
		{
			cords_terminate_core( &App );
			return(document_drop(document));
		}
		else	App.sid = liberate( App.sid );

		if (( App.sid  = cords_extract_atribut( App.software, "occi", 
				_CORDS_IMAGE, _CORDS_SYSTEM )) != (char *) 0)
		{
			if (!( App.system = cords_retrieve_instance( host, App.sid, agent,tls )))
			{
				cords_terminate_core( &App );
				return(document_drop(document));
			}
			else	App.sid = liberate( App.sid );
		}
		else	App.system = (struct occi_response *) 0;
	}
	else	App.software = (struct occi_response *) 0;
	
	if (!( App.contract = cords_create_category( document, agent,tls ) ))
	{
		cords_terminate_core( &App );
		return(document_drop(document));
	}
	else if ((status = cords_resolve_location( App.contract, document )) != 0)
	{
		cords_terminate_core( &App );
		return(document_drop(document));
	}
	else if ((status = cords_terminate_level( document, agent,tls )) != 0)
	{
		cords_terminate_core( &App );
		return((struct xml_element *) 0);
	}

	cords_terminate_core( &App );

	return( document );
}



/*	-------------------------------------------------------		*/
/*		c o r d s _ b u i l d _ i n s t a n c e			*/
/*	-------------------------------------------------------		*/
/*	builds an XML document form of a provisioning instance		*/
/*	of a cords request/plan.					*/
/*	-------------------------------------------------------		*/
private	struct	xml_element * 	cords_build_instance(
		char * plan, 	char * name, 
		char * request,
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
	else if (!( aptr = document_add_atribut( eptr, _CORDS_MANIFEST , request ) ))
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
/*	the fiinal step on this long and winding road. here we		*/
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
		return(911);
	else if (!( zptr =  cords_invoke_action( instance, "start", agent,tls ) ))
		return(912);
	else if (!( aptr = document_add_atribut( document, "instance", instance )))
		return(913);
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
#ifndef	_STAND_ALONE_BROKER
public	int cords_manifest_broker(
	char * host, char * plan, char * nameplan, char * manifest, char * agent, char * tls )
#else
public	int cords_manifest_broker(
	char * host, char * plan, char * agent, char * tls, char * result )
#endif
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

#ifdef	_STAND_ALONE_BROKER
	initialise_occi_resolver( host, (char *) 0, (char *) 0, (char *) 0 );
#endif

	if ( check_verbose() )
		printf("   CORDS Request Broker ( %s ) Phase 1 : Preparation \n", agent);

#ifndef	_STAND_ALONE_BROKER
	if (!( CbC.planID = allocate_string( plan )))
		return( cords_terminate_provisioning( 900, &CbC ) );
	else if (!( CbC.namePlan = allocate_string( nameplan ) ))
		return( cords_terminate_provisioning( 902, &CbC ) );
	if (!( CbC.reqID  = allocate_string( manifest ) ))
		return( cords_terminate_provisioning( 903, &CbC ) );
	else
#else
	/* --------------------------------------- */
	/* retrieve the provisioning plan instance */
	/* --------------------------------------- */
	if (!( CbC.planID = occi_unquoted_value( plan ) ))
		return( cords_terminate_provisioning( 900, &CbC ) );
	else if (!( CbC.plan = cords_retrieve_instance( host, CbC.planID, agent )))
		return( cords_terminate_provisioning( 901, &CbC ) );
	else if (!( CbC.namePlan = cords_extract_atribut( CbC.plan, "occi", _CORDS_PLAN, _CORDS_NAME ) ))
		return( cords_terminate_provisioning( 902, &CbC ) );
	/* ----------------------------- */
	/* retrieve the request instance */
	/* ----------------------------- */
	if (!( CbC.reqID  = cords_extract_atribut( CbC.plan, "occi", _CORDS_PLAN, _CORDS_MANIFEST ) ))
		return( cords_terminate_provisioning( 903, &CbC ) );
	else
#endif
	if (!( CbC.manifest = cords_retrieve_instance( host, CbC.reqID, agent,tls )))
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

	if (!( CbC.document = cords_build_instance( CbC.planID, CbC.namePlan, CbC.reqID, CbC.accID, CbC.accID ) ))
		return( cords_terminate_provisioning( 909, &CbC ) );

	else if (!( CbC.instance = cords_create_category( CbC.document, agent,tls ) ))
		return( cords_terminate_provisioning( 910, &CbC ) );

	else if ((status = cords_resolve_location( CbC.instance, CbC.document )) != 0)
		return( cords_terminate_provisioning( status, &CbC ) );
	else if (!( aptr = document_atribut( CbC.document, "id" )))
		return( cords_terminate_provisioning( status, &CbC ) );
	else if (!( CbC.instID = occi_unquoted_value( aptr->value ) ))
		return( cords_terminate_provisioning( status, &CbC ) );

	for (	eptr=cords_first_link( CbC.manifest );
		eptr != (struct occi_element *) 0;
		eptr = eptr->next )
	{
		if (!( eptr->value ))
			continue;
		if (!( id =  occi_unquoted_link( eptr->value ) ))
			continue;
		else if (!( mptr = cords_instance_core( host, id, agent,tls, CbC.namePlan ) ))
			return( cords_terminate_provisioning( 911, &CbC ) );
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
			CbC.cores++;
			continue;
		}
	}

	(void) cords_terminate_level( CbC.document, agent,tls );

	if ( check_verbose() )
		printf("   CORDS Request Broker ( %s ) Provisioned %u Cores \n",agent,CbC.cores);

	if ( check_verbose() )
		printf("   CORDS Request Broker ( %s ) Phase 3 : Configuration \n",agent);

	if (( status = cords_broker_configuration( host, CbC.document, CbC.configuration, agent,tls )) != 0)
		return( cords_terminate_provisioning( status, &CbC ) );


#ifndef	_STAND_ALONE_BROKER
	else if (!( CbC.plan =  cords_invoke_action( CbC.instID, "start", agent,tls ) ))
		return( cords_terminate_provisioning( 912, &CbC ) );

#else
	else if ((status = cords_instance_plan( CbC.document, CbC.planID, CbC.instID, agent,tls )) != 0)
		return( cords_terminate_provisioning( status, &CbC ) );
	else if ((status = cords_recover_results( CbC.document, agent,tls )) != 0)
		return( cords_terminate_provisioning( status, &CbC ) );

	if ( check_verbose() )
		printf("   CORDS Request Broker ( %s ) Terminated (%s) \n",agent,tls,result);

	if ( result )
		document_serialise_file(CbC.document, result );
#endif	
	return( cords_terminate_provisioning( 0, &CbC ) );
}

	/* --------------- */
#endif	/* _cords_broker_c */
	/* --------------- */


