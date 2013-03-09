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
#ifndef	_ezicontract_c	
#define	_ezicontract_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"
#include "cordslang.h"
#include "cb.h"

struct	cords_vector
{
	char *	id;
	struct occi_response * message;
};

struct	cords_easy_contract
{
	struct	cords_vector	node;
	struct	cords_vector	manifest;
	struct	cords_vector	application;
};

/* ---------------------------------------------------------------------------- */
/* 		r e s o l v e _ e z i  _ c o n f i g u r a t i o n		*/
/* ---------------------------------------------------------------------------- */
private	struct	ezi_config * resolve_ezi_configuration( char * sptr )
{
	struct	occi_kind_node * nptr;
	struct	ezi_config * pptr=(struct ezi_config *) 0;
	struct	occi_kind_node  * occi_first_ezi_config_node();
	rest_log_message("resolve_ezi_configuration");
	rest_log_message( sptr );
	for (	nptr = occi_first_ezi_config_node();
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next )
	{
		if (!( pptr = nptr->contents ))
			continue;
		else if (!( pptr->name ))
			continue;
		else if (!( strcmp( pptr->name, sptr ) ))
			return( pptr );
	}
	return((struct ezi_config *) 0);
}

/*	--------------------------------------------------------	*/
/* 	u s e _ e a s i c l o u d s _ c o n f i g u r a t i o n 	*/
/*	--------------------------------------------------------	*/
private	struct ezi_subscription * use_easiclouds_configuration( char * sptr )
{
	struct	ezi_config * pptr;

	if (!( pptr = resolve_ezi_configuration( sptr )))
	 	return((struct ezi_subscription *) 0);

	else 	return( ezi_initialise_client( 
			pptr->user, pptr->password, pptr->namespace,
			pptr->host, _CORDS_CONTRACT_AGENT, pptr->version, pptr->tls ));
}


/*	-------------------------------------------	*/
/* 	   	s t a r t _ e a s i c l o u d s     		*/
/*	-------------------------------------------	*/
public	struct	rest_response * start_easiclouds(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	easiclouds * pptr;
	struct	ezi_subscription * subptr;
	struct	ezi_response * zptr;
	rest_log_message("start_easiclouds_contract");
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if (!( pptr->profile ))
	 	return( rest_html_response( aptr, 403, "Forbidden" ) );
	else if (!( subptr = use_easiclouds_configuration( pptr->profile ) ))
	 	return( rest_html_response( aptr, 403, "Forbidden" ) );
	else if (!( pptr->filename ))
	 	return( rest_html_response( aptr, 500, "Application Message Failure" ) );
	else if (!( zptr = ezi_create_server( subptr, pptr->filename ) ))
	 	return( rest_html_response( aptr, 500, "Application Creation Failure" ) );
	else 
	{
		pptr->state = 1;
		pptr->hostname = allocate_string("vm.easiclouds.com");
		zptr = liberate_ezi_response( zptr );
		if ( rest_valid_string( pptr->price ) )
			occi_send_transaction( "easiclouds", pptr->price, "action=start", pptr->account, pptr->id );
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	   	s a v e _ e a s i c l o u d s     		*/
/*	-------------------------------------------	*/
public	struct	rest_response * save_easiclouds(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	easiclouds * pptr;
	struct	ezi_subscription * subptr;
	rest_log_message("save_easiclouds_contract");
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if (!( pptr->profile ))
	 	return( rest_html_response( aptr, 403, "Forbidden" ) );
	else if (!( subptr = use_easiclouds_configuration( pptr->profile ) ))
	 	return( rest_html_response( aptr, 403, "Forbidden" ) );
	else
	{
		/* ----------------------------------- */
		/* add code here to allocate resources */	
		/* ----------------------------------- */
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-----------------------------------------------		*/
/* 	s t o p _ e a s i c l o u d s _ c o n t r a c t 	*/
/*	-----------------------------------------------		*/
private	int	stop_easiclouds_contract( 
	struct	ezi_subscription * subptr,
	struct easiclouds * pptr )
{
	if ( pptr->state )
	{
		/* ---------------------------------- */
		/* add code here to release resources */	
		/* ---------------------------------- */
		pptr->state = 0;
		if ( pptr->application )
		{
			ezi_delete_server( subptr, pptr->application );
			if ( rest_valid_string( pptr->price ) )
				occi_send_transaction( "easiclouds", pptr->price, "action=stop", pptr->account, pptr->id );
			pptr->application = liberate( pptr->application );	
		}
	}
	return( 0 );
}

/*	-------------------------------------------	*/
/* 	   	s t o p _ e a s i c l o u d s     		*/
/*	-------------------------------------------	*/
public	struct	rest_response * stop_easiclouds(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	easiclouds * pptr;
	struct	ezi_subscription * subptr;
	rest_log_message("stop_easiclouds_contract");
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if (!( pptr->profile ))
	 	return( rest_html_response( aptr, 403, "Forbidden" ) );
	else if (!( subptr = use_easiclouds_configuration( pptr->profile ) ))
	 	return( rest_html_response( aptr, 403, "Forbidden" ) );
	else
	{
		stop_easiclouds_contract( subptr, pptr );
	 	return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	---------------------------------------------------	*/
/* 	   t e r m i n a t e _ e a s y _ c o n t r a c t	*/
/*	---------------------------------------------------	*/
private	int	terminate_ezi_contract( int status, struct cords_easy_contract * contract )
{
	if ( contract )
	{
		if ( contract->node.id )
			contract->node.id = liberate( contract->node.id );
		if ( contract->node.message )
			contract->node.message = occi_remove_response( contract->node.message );
		if ( contract->manifest.id )
			contract->manifest.id = liberate( contract->manifest.id );
		if ( contract->manifest.message )
			contract->manifest.message = occi_remove_response( contract->manifest.message );
		if ( contract->application.id )
			contract->application.id = liberate( contract->application.id );
		if ( contract->application.message )
			contract->application.message = occi_remove_response( contract->application.message );
	}
	return( status );
}

/*	---------------------------------------------------	*/
/*		e z i _ s e r i a l i s e _ e x t r a s		*/
/*	---------------------------------------------------	*/
/*	TODO	*/
/*	----	*/
private	int	ezi_serialise_extras( FILE * h, struct occi_response * zptr )
{
	fprintf(h,",%cextras%c : {}\n",0x0022,0x0022);
	return(0);
}

/*	---------------------------------------------------	*/
/*	 e z i _ s e r i a l i s e _ d e s c r i p t i o n	*/
/*	---------------------------------------------------	*/
/*	TODO	*/
/*	----	*/
private	int	ezi_serialise_description( FILE * h, struct occi_response * zptr )
{
	fprintf(h,",%cdescription%c : {}\n",0x0022,0x0022);
	return(0);
}

/*	---------------------------------------------------	*/
/*	    e z i _ s e r i a l i s e _ m e t a d a t a		*/
/*	---------------------------------------------------	*/
/*	TODO	*/
/*	----	*/
private	int	ezi_serialise_metadata( FILE * h, struct occi_response * zptr )
{
	fprintf(h,",%cmetadata%c : {}\n",0x0022,0x0022);
	return(0);
}

/*	---------------------------------------------------	*/
/* 	 c r e a t e _ e a s i c l o u d s _ m e s s a g e  	*/
/*	---------------------------------------------------	*/
private	char *	create_easiclouds_request( struct cords_easy_contract * contract, struct easiclouds * pptr )
{
	struct	occi_response * yptr;
	struct	occi_response * zptr;
	struct	occi_element * eptr;
	struct	occi_element * dptr;
	char * filename;
	char *	vptr;
	char *	sptr;
	char *	nptr;
	FILE * h;
	int	mode=0;
	int	servers=0;
	int	links=0;
	int	nodes=0;
	if (!( filename = rest_temporary_filename("json")))
		return( filename );
	else if (!( h = fopen( filename, "w") ))
		return( liberate( filename ) );

	fprintf(h,"{\n%ccompound_app%c : \n",0x0022,0x0022);

	fprintf(h,"{\n");

	if (( vptr = occi_extract_atribut( contract->manifest.message, "occi", "easiclouds_application", _CORDS_NAME )) != (char *) 0)
	{
		fprintf(h,"%cname%c : %c%s%c,\n",0x0022,0x0022,0x0022,vptr,0x0022);
	}


	/* --------------------------- */
	/* foreach node or link record */
	/* --------------------------- */
	for (	eptr = cords_first_link( contract->manifest.message );
		eptr != (struct occi_element *) 0;
		eptr = cords_next_link( eptr ))
	{
		if (!( eptr->value ))
			continue;
		else if (!( nptr =  occi_unquoted_link( eptr->value ) ))
			continue;
		else if (!( yptr = occi_simple_get( nptr, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		{
			liberate( nptr );
			continue;
		}
		else if (!( yptr->category ))
		{
			liberate( nptr );
			yptr = occi_remove_response( yptr );
			continue;
		}
		else if (!( yptr->category->id ))
		{
			liberate( nptr );
			yptr = occi_remove_response( yptr );
			continue;
		}
		else if (!( strcmp( yptr->category->id, "easiclouds_node" ) ))
		{
			if ( mode != 1 )
			{
				/* ------------------------ */
				/* open the node collection */
				/* ------------------------ */
				if ( mode ) fprintf(h,"],\n");
				fprintf(h,"%cnodes%c : [\n",0x0022,0x0022);
				mode = 1;
			}
			/* --------------------------- */
			/* open a named node structure */
			/* --------------------------- */
			if ( nodes++ )
				fprintf(h,",{\n");
			else	fprintf(h,"{\n");

			if (( vptr = occi_extract_atribut( yptr, "occi", "easiclouds_node", "name" )) != (char *) 0)
				fprintf(h,"%cname%c : %c%s%c\n",0x0022,0x0022,0x0022,vptr,0x0022);
			else 	fprintf(h,"%cname%c : %c%s%c\n",0x0022,0x0022,0x0022,"noname",0x0022);

			if (( vptr = occi_extract_atribut( yptr, "occi", "easiclouds_node", "tenant" )) != (char *) 0)
			{
				fprintf(h,"%c,os_tenant_id%c : %c%s%c\n",0x0022,0x0022,0x0022,vptr,0x0022);
			}
			/* -------------------- */
			/* open the server list */
			/* -------------------- */
			fprintf(h,",%cservers%c : [\n",0x0022,0x0022);

			for (	servers=0,dptr = cords_first_link( yptr );
				dptr != (struct occi_element *) 0;
				dptr = cords_next_link( dptr ))
			{
				if (!( dptr->value ))
					continue;
				else if (!( sptr =  occi_unquoted_link( dptr->value ) ))
					continue;
				else if (!( zptr = occi_simple_get( sptr, _CORDS_CONTRACT_AGENT, default_tls() ) ))
				{
					liberate( sptr );
					continue;
				}
				else if (!( yptr->category ))
				{
					liberate( sptr );
					zptr = occi_remove_response( zptr );
					continue;
				}
				else if (!( zptr->category->id ))
				{
					liberate( sptr );
					zptr = occi_remove_response( zptr );
					continue;
				}
				else if (!( strcmp( zptr->category->id, "easiclouds_server" ) ))
				{
					/* --------------------------------------------------- */
					/* open a named server structure with image and flavor */
					/* --------------------------------------------------- */
					if ( servers++ )
						fprintf(h,",{\n");
					else	fprintf(h,"{\n");
				
					if (( vptr = occi_extract_atribut( zptr, "occi", "easiclouds_server", "image" )) != (char *) 0)
					{
						fprintf(h,"%cimageRef%c : %c%s%c,\n",0x0022,0x0022,0x0022,vptr,0x0022);
					}
					if (( vptr = occi_extract_atribut( zptr, "occi", "easiclouds_server", "flavor" )) != (char *) 0)
					{
						fprintf(h,"%cflavorRef%c : %c%s%c,\n",0x0022,0x0022,0x0022,vptr,0x0022);
					}
					if (( vptr = occi_extract_atribut( zptr, "occi", "easiclouds_server", "name" )) != (char *) 0)
					{
						fprintf(h,"%cname%c : %c%s%c\n",0x0022,0x0022,0x0022,vptr,0x0022);
					}

					ezi_serialise_metadata( h, zptr );	

					fprintf(h,"}\n");
					zptr = occi_remove_response(zptr);
				}
				else	zptr = occi_remove_response(zptr);
			}
			fprintf(h,"]\n");

			ezi_serialise_extras( h, yptr );

			ezi_serialise_description( h, yptr );

			fprintf(h,"}\n");

			yptr = occi_remove_response(yptr);
		}
		else if (!( strcmp( yptr->category->id, "easiclouds_link" ) ))
		{
			if ( mode != 2 )
			{
				/* ------------------------ */
				/* open the node collection */
				/* ------------------------ */
				if ( mode ) fprintf(h,"],\n");
				fprintf(h,"%clinks%c : [\n",0x0022,0x0022);
				mode = 2;
			}
			/* --------------------------------------------------- */
			/* open a named link structure with source and target  */
			/* --------------------------------------------------- */
			if ( links++ )
				fprintf(h,",{\n");
			else	fprintf(h,"{\n");

			if (( vptr = occi_extract_atribut( yptr, "occi", "easiclouds_link", "name" )) != (char *) 0)
				fprintf(h,"%cname%c : %c%s%c,\n",0x0022,0x0022,0x0022,vptr,0x0022);
			else	fprintf(h,"%cname%c : %c%s%c,\n",0x0022,0x0022,0x0022,"noname",0x0022);
			if (( vptr = occi_extract_atribut( yptr, "occi", "easiclouds_link", "from" )) != (char *) 0)
				fprintf(h,"%cnodeA_name%c : %c%s%c,\n",0x0022,0x0022,0x0022,vptr,0x0022);
			else	fprintf(h,"%cnodeA_name%c : %c%s%c\n",0x0022,0x0022,0x0022,"nolink",0x0022);
			if (( vptr = occi_extract_atribut( yptr, "occi", "easiclouds_link", "to" )) != (char *) 0)
				fprintf(h,"%cnodeB_name%c : %c%s%c\n",0x0022,0x0022,0x0022,vptr,0x0022);
			else	fprintf(h,"%cnodeB_name%c : %c%s%c\n",0x0022,0x0022,0x0022,"nolink",0x0022);

			ezi_serialise_description( h, yptr );
			ezi_serialise_extras( h, yptr );

			fprintf(h,"}\n");
			yptr = occi_remove_response(yptr);
		}
		else	yptr = occi_remove_response( yptr );
	}

	if ( mode ) fprintf(h,"]\n");

	ezi_serialise_extras( h, contract->manifest.message );

	fprintf(h,"}\n");
	fprintf(h,"}\n");
	fclose( h );
	return( filename );
}

/*	---------------------------------------------------	*/
/* 	c r e a t e _ e a s i c l o u d s _ c o n t r a c t	*/
/*	---------------------------------------------------	*/
public	int	create_easiclouds_contract(
		struct occi_category * optr,
		struct easiclouds * pptr)
{
	struct	ezi_subscription * subptr;
	struct	occi_response 	* zptr;
	struct	occi_response 	* yptr;
	char *	vptr;
	struct	cords_easy_contract contract;
	
	memset(&contract,0,sizeof( struct cords_easy_contract ));

	rest_log_message("create_easiclouds_contract");

	/* ------------------------ */
	/* resolve the subscription */
	/* ------------------------ */
	if (!( pptr->profile ))
	 	return( 118 );
	else if (!( subptr = use_easiclouds_configuration( pptr->profile ) ))
	 	return( 118 );
	else if (!( pptr->node ))
		return(118);

	/* ------------------------------------- */
	/* collect the manifest node description */
	/* ------------------------------------- */
	else if (!( contract.node.id = allocate_string( pptr->node ) ))
		return(terminate_ezi_contract(118, &contract ));
	else if (!( contract.node.message = occi_simple_get( contract.node.id, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( terminate_ezi_contract( 1170, &contract ) );
	else if (!( vptr = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_TYPE ) ))
		return( terminate_ezi_contract( 1127, &contract ) );
	/* ------------------------------------- */
	/* collect abstract manifest name or url */
	/* ------------------------------------- */
	else if (!( contract.manifest.id = allocate_string( vptr ) ))
		return(terminate_ezi_contract( 118, &contract ));
	else if (!( contract.manifest.message = occi_simple_get( contract.manifest.id, _CORDS_CONTRACT_AGENT, default_tls() ) ))
	{
		/* ----------------------------- */
		/* resolve the reference by name */
		/* ----------------------------- */
		if (!( yptr = cords_retrieve_named_instance_list( "easiclouds_application", "occi.easiclouds_application.name", contract.manifest.id, _CORDS_CONTRACT_AGENT,default_tls() ) ))
			return( terminate_ezi_contract( 1404, &contract ) );
		else if (!( contract.manifest.message = cords_retrieve_named_instance( yptr, _CORDS_CONTRACT_AGENT, default_tls() )))
			return( terminate_ezi_contract( 1405, &contract ) );
		else	yptr = occi_remove_response( yptr );
	}
	/* ------------------------------------------------------------------------------ */
	/* arrival here the manifest element contains the application description message */
	/* ------------------------------------------------------------------------------ */
	if (!( pptr->filename = create_easiclouds_request( &contract, pptr ) ))
		return( terminate_ezi_contract( 1406, &contract ) );

	else
	{
		/* ----------------------------------------------- */
		/* resolve any price informatino for this category */
		/* ----------------------------------------------- */
		pptr->price = occi_resolve_category_price( "easiclouds", default_operator(), _CORDS_CONTRACT_AGENT, default_tls() );
		return( terminate_ezi_contract( 0, &contract ) );
	}
}

/*	---------------------------------------------	*/
/* 	d e l e t e _ e a s i c l o u d s _ c o n t r a c t	*/
/*	---------------------------------------------	*/
public	int	delete_easiclouds_contract(
		struct occi_category * optr,
		struct easiclouds * pptr)
{
	struct	ezi_subscription * subptr;
	/* ----------------------------------- */
	/* add code here for contract deletion */
	/* ----------------------------------- */
	rest_log_message("delete_easiclouds_contract");
	if (!( pptr->profile ))
	 	return( 118 );
	else if (!( subptr = use_easiclouds_configuration( pptr->profile ) ))
	 	return( 118 );
	else
	{
		stop_easiclouds_contract( subptr, pptr );
		return(0);
	}
}

	/* -------------- */
#endif	/* _ezicontract_c */
	/* -------------- */


