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
#ifndef	_coips_c
#define	_coips_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"
#include "occiclient.h"
#include "cosacsctrl.h"
#include "cordslang.h"
#include "cp.h"
#include "cb.h"

#define	_COIPS_MODEL "coips:model"
#define _COIPS_ACCOUNT "coips"

struct	accords_configuration Coips = {
	0,0,
	0,0,0,0,
	(char *) 0,
	(char *) 0,
	_CORDS_DEFAULT_PUBLISHER,
	_CORDS_DEFAULT_OPERATOR,
	_CORDS_DEFAULT_USER,
	_CORDS_DEFAULT_PASSWORD,
	"http",  80,
	"xmpp",  8000,
	"domain",
	"coips.xml",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(Coips.debug);		}
public	int	check_verbose()		{	return(Coips.verbose);		}
public	char *	default_publisher()	{	return(Coips.publisher);	}
public	char *	default_operator()	{	return(Coips.operator);	}
public	char *	default_tls()		{	return(Coips.tls);		}

public	int	failure( int e, char * m1, char * m2 )
{
	if ( e )
	{
		printf("\n***(%u) failure %u",getpid(),e);
		if ( m1 )
			printf(" : %s",m1);
		if ( m2 )
			printf(" : %s",m2);
		printf(" **`\n");
	}
	return( e );
}

/*	---------------------------------------------	*/  
/*	 	   c o i p s _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads coips  configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	coips_load()
{
	load_accords_configuration( &Coips, "coips" );
	return;
}

private	int	banner()
{
	printf("\n   CompatibleOne Image Production Services : Version 1.0a.0.01");
	printf("\n   Beta Version : 07/02/2012");
	printf("\n   Copyright (c) 2012 Iain James Marshall, Prologue");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*	------------------------------------------------------------------	*/
/*			c o i p s _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * coips_initialise(  void * v,struct rest_server * sptr )
{
	struct	rest_extension * xptr;
	if (!( xptr = rest_add_extension( sptr ) ))
		return((struct rest_server *) 0);

	else
	{
		xptr->net = (struct connection *) 0;
		return( sptr );
	}
}

/*	------------------------------------------------------------------	*/
/*			c o i p s _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	coips_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, Coips.user ) )
		return(0);
	else if ( strcmp( password, Coips.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			c o i p s _ e x t e n s i o n 				*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * coips_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	------------------------------------------------------------------	*/
/*			c o i p s _ s yn c h r o n i s e 			*/
/*	------------------------------------------------------------------	*/
private	void	coips_synchronise()
{
	sleep(1);
}

/* ------------------------- */
/* build a provisioning node */
/* ------------------------- */
private char *	build_application_node(char * image, char * provider )
{
	struct	occi_response 	* yptr;
	struct	occi_response 	* uptr;
	struct	occi_response 	* zptr;
	struct	occi_client 	* kptr;
	struct	occi_request 	* qptr;
	struct	occi_element  	* dptr;
	char *	infrastructure;
	char *	iname;
	char *	vptr;
	char *	ihost;
	char	buffer[2048];
	char *	tls=default_tls();
	char *	agent=_CORDS_CONTRACT_AGENT;

	if ( check_debug() ) rest_log_message("coips:build application_node");

	/* retrieve the COIPS infrastructure description */
	/* --------------------------------------------- */
	if (!( yptr = cords_retrieve_named_instance_list( 
		_CORDS_INFRASTRUCTURE, "occi.infrastructure.name", _COIPS_MODEL, agent, tls ) ))
	{
		return( (char *) 0 );
	}
	else if (!( uptr = cords_retrieve_named_instance( yptr, agent, tls )))
	{
		return( (char *) 0 );
	}
	else
	{
		sprintf(buffer,"%s%s",uptr->host,uptr->name);

		if (!( infrastructure = allocate_string( buffer ) ))
			return((char *) 0);

		yptr = occi_remove_response( yptr );
		uptr = occi_remove_response( uptr );
	}

	if (!( ihost = occi_resolve_category_provider(_CORDS_NODE,agent, tls) ))
		return((char *) 0);
	else
	{
		sprintf(buffer,"%s/%s/",ihost,_CORDS_NODE);
		liberate( ihost );
	}

	/* ------------------------------------ */
	/* build the temporary node description */
	/* ------------------------------------ */
	if (!( kptr = occi_create_client( buffer, agent, tls ) ))
		return((char *) 0);

	else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
	{
		kptr = occi_remove_client( kptr );
		return((char *) 0);
	}
	else if (!( iname = occi_category_id( image ) ))
	{
		kptr = occi_remove_client( kptr );
		return((char *) 0);
	}
	else if ((!(dptr=occi_request_element(qptr,"occi.node.name", 		iname 		) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.node.provider",    	provider	) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.node.profile",    	_COIPS_ACCOUNT	) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.node.image",    	image		) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.node.infrastructure", 	infrastructure  ) )))
	{
		liberate( iname );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return((char *) 0);
	}
	else if (!( yptr = occi_client_post( kptr, qptr ) ))
	{
		liberate( iname );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return((char *) 0);
	}
	else if (!( vptr = occi_extract_location( yptr ) ))
	{
		liberate( iname );
		uptr = occi_remove_response( uptr );
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		return((char *) 0);
	}
	else if (!( vptr = occi_category_id( vptr ) ))
	{
		liberate( iname );
		uptr = occi_remove_response( uptr );
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		return((char *) 0);
	}
	else
	{
		liberate( iname );
		uptr = occi_remove_response( uptr );
		sprintf(buffer,"%s/%s/%s",yptr->host,_CORDS_NODE,vptr);
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		if ( check_debug() ) rest_log_message("coips:build application_node:done");
		coips_synchronise();
		return( allocate_string( buffer ) );
	}		
}

/* ------------------------- */
/* negotiate the contracts   */
/* ------------------------- */
private	char *	negotiate_application_contract(char * node,struct cords_placement_criteria * selector)
{
	char *	contract=(char *) 0;
	struct	xml_element * document=(struct xml_element *) 0;
	struct	xml_atribut * aptr;
	if ( check_debug() ) rest_log_message("coips:negotiate_application_contract");
	if (!( document = cords_instance_node(
		selector, node, node, _CORDS_CONTRACT_AGENT, default_tls(), "coips", "coips", "coips") ))
		return( (char *) 0 );
	else if (!( aptr = document_atribut( document, _CORDS_ID ) ))
	{
		document = document_drop( document );
		return((char * ) 0);
	}
	else if (!( contract = allocate_string( aptr->value ) ))
	{
		document = document_drop( document );
		return((char * ) 0);
	}
	else
	{
		document = document_drop( document );
		if ( check_debug() ) rest_log_message("coips:negotiate_application_contract:done");
		coips_synchronise();
		return(contract);
	}
}


/* ------------------------- */
/* provision the contract    */
/* ------------------------- */
private char * 	provision_application_contract(char *contract)
{
	if ( check_debug() ) rest_log_message("coips:provision_application_contract");
	cords_invoke_action( contract, "start", _CORDS_SERVICE_AGENT, default_tls() );
	if ( check_debug() ) rest_log_message("coips:provision_application_contract:done");
	coips_synchronise();
	return(contract);
}


/* ------------------------- */
/* Install the Package       */
/* ------------------------- */
private	char * 	install_application_package( char * cosacs , char * package )
{
	char *	action=_COSACS_RUN;
	char *	command=(char *) 0;
	char *	type="command";
	struct	occi_response * zptr;
	char *	vptr;
	int	status=0;

	if ( check_debug() )
	{
		rest_log_message("coips:install_application_package");
		rest_log_message( package );
	}

	/* retrieve the package details */
	/* ---------------------------- */
	if (!( zptr = occi_simple_get( package, _CORDS_SERVICE_AGENT, default_tls() ) ))
		return( (char * )0 );
	else if (!( vptr = occi_extract_atribut( zptr, "occi","package", "installation" )))
		return( (char * )0 );
	else if ((status = cosacs_create_script( cosacs, _COSACS_RUN, vptr, type )) != 0)
		return( (char * )0 );
	else if (!( vptr = occi_extract_atribut( zptr, "occi", "package", "configuration" )))
		return( (char * )0 );
	else if ((status = cosacs_create_script( cosacs, _COSACS_RUN, vptr, type )) != 0)
		return( (char *) 0 );
	else if ((status = cosacs_create_script( cosacs, _COSACS_RUN, "sync", type )) != 0)
		return( (char *) 0 );
	else if ((status = cosacs_create_script( cosacs, _COSACS_START, "production", type )) != 0)
		return((char *) 0) ;
	else
	{
		if ( check_debug() ) rest_log_message("coips:install_application_package:done");
		coips_synchronise();
		return( package );
	}
}

/* ------------------------- */
/* Save Image 		     */
/* ------------------------- */
private void	save_application_image( char * contract )
{
	if ( check_debug() ) rest_log_message("coips:save_image");
	cords_invoke_action( contract, "save", _CORDS_SERVICE_AGENT, default_tls() );
	if ( check_debug() ) rest_log_message("coips:save_image:done");
	coips_synchronise();
	return;
}

/* ------------------------- */
/* Stop Provisioning 	     */
/* ------------------------- */
private	void 	stop_application_provisioning(  char * contract )
{
	if ( check_debug() ) rest_log_message("coips:stop_server");
	cords_invoke_action( contract, "stop", _CORDS_SERVICE_AGENT, default_tls() );
	if ( check_debug() ) rest_log_message("coips:stop_server:done");
	coips_synchronise();
	return;
}


/* ------------------------- */
/* Delete Provisioning 	     */
/* ------------------------- */
private	void	delete_application_provisioning(  char * contract )
{
	if ( check_debug() ) rest_log_message("coips:delete_server");
	occi_simple_delete( contract, _CORDS_SERVICE_AGENT, default_tls() );
	if ( check_debug() ) rest_log_message("coips:delete_server:done");
	coips_synchronise();
	return;
}


/* ------------------------- */
/* Delete Provisioning 	     */
/* ------------------------- */
private void	delete_application_node(  char * node )
{
	if ( check_debug() ) rest_log_message("coips:delete_node");
	occi_simple_delete( node, _CORDS_SERVICE_AGENT, default_tls() );
	if ( check_debug() ) rest_log_message("coips:delete_node:done");
	coips_synchronise();
	return;
}


/* ------------------------- */
/* Update Image name 	     */
/* ------------------------- */
private	void	update_ezvm_image( struct cords_application * aptr )
{
	if ( check_debug() ) rest_log_message("coips:update_ezvm");
	/* TODO */
	if ( check_debug() ) rest_log_message("coips:update_ezvm:done");
	return;
}


/*	------------------------------------------------------------------	*/
/* 	  actions and methods required for the coips instance category		*/
/*	------------------------------------------------------------------	*/

/*	------------------------------------------------------------------	*/
/*			c o i p s _ l i n k _ v a l u e				*/
/*	------------------------------------------------------------------	*/
private	char *	coips_link_value( char * sptr )
{
	char *	rptr;
	if (!( sptr ))
		return( sptr );
	else if ( *sptr != '<' )
		return( sptr );
	else if (!( rptr = allocate_string( (sptr+1) )))
		return( rptr );
	else 
	{
		for ( sptr=rptr; *sptr != 0; sptr++ )
		{
			if ( *sptr == '>' )
			{
				*sptr = 0;
				break;
			}
		}
		return( rptr );
	}
}

/*	-----------------------------------------------------------	*/
/*	f i r s t _ a p p l i c a t i o n _ p a c k a g e _ l i n k	*/
/*	-----------------------------------------------------------	*/
private	struct occi_element * 	first_application_package_link( struct occi_response * zptr )
{
	struct	occi_element  * eptr;
	for (	eptr=zptr->first;
		eptr != (struct occi_element *) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;
		else if ( strcasecmp(eptr->name,"LINK") != 0 )
			continue;
		else	break;
	}
	return( eptr );
}

/*	------------------------------------------------------------------	*/
/*			l l _ b u i l d _ a p p l i c a t i o n			*/
/*	------------------------------------------------------------------	*/
private	int	ll_build_application( struct occi_category * optr, struct cords_application * aptr)
{
	char *	node;
	char *	contract;
	char *	linkvalue;
	char *	package;
	char *	cosacs=(char *) 0;
	char *	vptr;
	int	packages=0;
	struct	occi_response * zptr;
	struct	occi_response * wptr;
	struct	occi_element  * eptr;
	struct	cords_placement_criteria selector;

	memset( &selector, 0, sizeof( struct cords_placement_criteria ));

	/* ---------------------------------------- */
	/* check first for packages to be installed */
	/* ---------------------------------------- */
	if (!( zptr = occi_simple_get( aptr->image, _CORDS_SERVICE_AGENT, default_tls() ) ))
	{
		aptr->state = 10;
		return( 0 );
	}
	else if (!( eptr = first_application_package_link( zptr ) ))
	{
		zptr = occi_remove_response( zptr );
		aptr->state = 10;
		return( 0 );
	}

	/* ------------------------- */
	/* build a provisioning node */
	/* ------------------------- */

	if ( check_debug() ) rest_log_message("coips:build_application");

	aptr->state = 1;

	if (!( node = build_application_node(aptr->image, aptr->provider) ))
		return( 800 );

	/* ------------------------- */
	/* negotiate the contracts   */
	/* ------------------------- */
	aptr->state = 2;

	if (!( contract = negotiate_application_contract(node,&selector)))
		return( 801 );


	/* ------------------------- */
	/* provision the contract    */
	/* ------------------------- */
	aptr->state = 3;

	if (!( contract = provision_application_contract(contract)))
		return( 801 );
	
	else 
	{
		aptr->state = 4;
		/* ------------------------- */
		/* For Each Package 	     */
		/* ------------------------- */
		for (	eptr = first_application_package_link( zptr );
			eptr != (struct occi_element *) 0;
			eptr = eptr->next )
		{
			if (!( eptr->name ))
				continue;
			else if (!( eptr->value ))
				continue;
			else if ( strcasecmp(eptr->name,"LINK") != 0 )
				continue;
			else if (!( linkvalue = coips_link_value( eptr->value )))
				continue;
			else
			{
				if (!( cosacs ))
				{
					/* --------------------------- */
					/* retrieve the cosacs address */
					/* --------------------------- */
					if (!( wptr = occi_simple_get( contract, _CORDS_SERVICE_AGENT, default_tls() ) ))
						return( 803 );
					else if (!( vptr = occi_extract_atribut( wptr, "occi","contract", "hostname" )))
						return( 804 );
					else if (!( cosacs = allocate_string( vptr ) ))
						return( 805 );
					else	
					{
						wptr = occi_remove_response( wptr );
						coips_synchronise();
					}
				}
				if (!( package = install_application_package( cosacs, linkvalue ) ))
					return( 806 );
				else
				{
					linkvalue = liberate( linkvalue );
					coips_synchronise();
				}
			}
		}
		zptr = occi_remove_response( zptr );
		occi_flush_client( cosacs, _COSACS_PORT );
		if ( cosacs ) cosacs = liberate( cosacs );
	}

	/* ------------------------- */
	/* Save Image 		     */
	/* ------------------------- */
	aptr->state = 5;
	save_application_image( contract );

	/* ------------------------- */
	/* Stop Provisioning 	     */
	/* ------------------------- */
	aptr->state = 6;
	stop_application_provisioning( contract );

	/* ------------------------- */
	/* Delete Provisioning 	     */
	/* ------------------------- */
	aptr->state = 7;
	delete_application_provisioning( contract );

	/* ------------------------- */
	/* Delete Provisioning 	     */
	/* ------------------------- */
	aptr->state = 8;
	delete_application_node( node );

	/* ------------------------- */
	/* Update Image name 	     */
	/* ------------------------- */
	aptr->state = 9;
	update_ezvm_image( aptr );

	aptr->state = 10;
	if ( check_debug() ) rest_log_message("coips:build_application:done");
	return(0);
}


/*	-------------------------------------------	*/
/* 	      b u i l d _ a p p l i c a t i o n  	*/
/*	-------------------------------------------	*/
private	struct rest_response * build_application(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_application * pptr;
	char	buffer[1024];
	int	status;
	if (!( pptr = vptr ))
		return(0);
	else if ( pptr->state > 0 )
		return(0);
	else
	{
		pptr->started = time((long *) 0);
		if ((status = ll_build_application (optr, pptr )) != 0)
		{
			pptr->completed = time((long *) 0);
			pptr->duration  = (pptr->completed - pptr->started);
			sprintf(buffer,"Application Build Failure Phase #%u",pptr->state);
			return( rest_html_response( aptr, status, buffer ) );
		}
		else
		{
			pptr->completed = time((long *) 0);
			pptr->duration  = (pptr->completed - pptr->started);
			return( rest_html_response( aptr, 200, "OK" ) );
		}
	}
}

/*	-------------------------------------------	*/
/* 	      c r e a t e _ a p p l i c a t i o n  	*/
/*	-------------------------------------------	*/
private	int	create_cords_application(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_application * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else
	{
		pptr->created = time((long *) 0);
		pptr->state  = 0;
		return(0);
	}
}

/*	-------------------------------------------	*/
/* 	  r e t r i e v e _ a p p l i c a t i o n  	*/
/*	-------------------------------------------	*/
private	int	retrieve_cords_application(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_application * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ a p p l i c a t i o n  	*/
/*	-------------------------------------------	*/
private	int	update_cords_application(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_application * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ a p p l i c a t i o n  	*/
/*	-------------------------------------------	*/
private	int	delete_cords_application(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_application * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

private	struct	occi_interface	cords_application_interface = {
	create_cords_application,
	retrieve_cords_application,
	update_cords_application,
	delete_cords_application
	};

/*	------------------------------------------------------------------	*/
/*			c o i p s _ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	coips_operation( char * nptr )
{
	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_coips.xml");

	if (!( optr = occi_cords_application_builder( Coips.domain, _CORDS_APPLICATION ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = &cords_application_interface;

	if (!( optr = occi_add_action( optr,_CORDS_BUILD,"",build_application)))
		return( 28 );

	rest_initialise_log(Coips.monitor);

	if (!( Coips.identity ))
		return( occi_server(  nptr, Coips.restport, Coips.tls, Coips.threads, first,(char *) 0 ) );
	else
	{
		initialise_occi_publisher( Coips.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			Coips.user, Coips.password,
			Coips.identity,  nptr, 
			Coips.restport, Coips.tls, 
			Coips.threads, first ) );
	}
}

/*	------------------------------------------------------------------	*/
/*				c o i p s 					*/
/*	------------------------------------------------------------------	*/
private	int	coips(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	coips_load();
	while ( argi < argc )
	{
		if (!( aptr = argv[++argi] ))
			break;
		else if ( *aptr == '-' )
		{
			aptr++;
			switch( *(aptr++) )
			{
			case	'v'	:
				Coips.verbose=1;
				continue;
			case	'd'	:
				Coips.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = coips_operation(aptr) ))
			continue;
		else	break;
	}
	return(status);
}

/*	------------------------------------------------------------------	*/
/*					m a i n 				*/
/*	------------------------------------------------------------------	*/
public	int	main(int argc, char * argv[] )
{
	if ( argc == 1 )
		return( banner() );
	else	return( coips( argc, argv ) );
}


	/* --------- */
#endif	/* _coips_c */
	/* --------- */

