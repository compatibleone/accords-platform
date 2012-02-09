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
	0,0,0,
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
		printf("\n*** failure %u",e);
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
	char *	vptr;
	char *	ihost;
	char	buffer[2048];
	char *	tls=default_tls();
	char *	agent=_CORDS_CONTRACT_AGENT;

	printf("coips:build application_node(%s)\n",image);
	printf("coips:retrieve infrastructure(%s)\n",_COIPS_MODEL);

	/* retrieve the COIPS infrastructure description */
	/* --------------------------------------------- */
	if (!( yptr = cords_retrieve_named_instance_list( 
		_CORDS_INFRASTRUCTURE, "occi.infrastructure.name", _COIPS_MODEL, agent, tls ) ))
	{
		printf("no list\n");
		return( (char *) 0 );
	}
	else if (!( uptr = cords_retrieve_named_instance( yptr, agent, tls )))
	{
		printf("no instance\n");
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

	printf("coips:build temporary node (%s)\n",infrastructure);

	if (!( ihost = occi_resolve_category_provider(_CORDS_NODE,agent, tls) ))
		return((char *) 0);
	else
	{
		sprintf(buffer,"%s/%s/",ihost,_CORDS_NODE);
		liberate( ihost );
	}

	printf("using(%s)\n",buffer);

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
	else if ((!(dptr=occi_request_element(qptr,"occi.node.name", 		image 		) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.node.provider",    	provider	) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.node.profile",    	_COIPS_ACCOUNT	) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.node.image",    	image		) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.node.infrastructure", 	infrastructure  ) )))
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
	else if (!( vptr = cords_extract_location( yptr ) ))
	{
		uptr = occi_remove_response( uptr );
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		return((char *) 0);
	}
	else if (!( vptr = occi_category_id( vptr ) ))
	{
		uptr = occi_remove_response( uptr );
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		return((char *) 0);
	}
	else
	{
		uptr = occi_remove_response( uptr );
		sprintf(buffer,"%s/%s/%s",yptr->host,_CORDS_NODE,vptr);
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		return( allocate_string( buffer ) );
	}		

	return((char *) 0);
}

/* ------------------------- */
/* negotiate the contracts   */
/* ------------------------- */
private	char *	negotiate_application_contract(char * node)
{
	char *	contract=(char *) 0;
	struct	xml_element * document=(struct xml_element *) 0;
	struct	xml_atribut * aptr;
	if (!( document = cords_instance_node(
		node, node, _CORDS_CONTRACT_AGENT, default_tls(), "coips", "coips", "coips") ))
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
		return(contract);
	}
}


/* ------------------------- */
/* provision the contract    */
/* ------------------------- */
private char * 	provision_application_contract(char *contract)
{
	cords_invoke_action( contract, "start", _CORDS_SERVICE_AGENT, default_tls() );
	return(contract);
}


/* ------------------------- */
/* Install the Package       */
/* ------------------------- */
private	char * 	install_application_package( char * cosacs , char * package )
{
	char *	action="cosacs:run";
	char *	command=(char *) 0;
	char *	type="command";
	struct	occi_response * zptr;
	char *	vptr;

	/* retrieve the package details */
	/* ---------------------------- */
	if (!( zptr = occi_simple_get( package, _CORDS_SERVICE_AGENT, default_tls() ) ))
		return( (char * )0 );
	else if (!( vptr = cords_extract_atribut( zptr, "occi","package", "installation" )))
		return( (char * )0 );
	else if (!( cosacs_create_script( cosacs, "cosacs:run", vptr, type ) ))
		return( (char * )0 );
	else if (!( vptr = cords_extract_atribut( zptr, "occi", "package", "configuration" )))
		return( (char * )0 );
	else if (!( cosacs_create_script( cosacs, "cosacs:run", vptr, type ) ))
		return( (char *) 0 );
	else if (!( cosacs_create_script( cosacs, "cosacs:start", "", type ) ))
		return((char *) 0) ;
	else	return( package );
}

/* ------------------------- */
/* Save Image 		     */
/* ------------------------- */
char *	save_application_image( char * contract )
{
	if ( check_debug() ) rest_log_message("coips:save_image");
	cords_invoke_action( contract, "save", _CORDS_SERVICE_AGENT, default_tls() );
	if ( check_debug() ) rest_log_message("coips:save_image:done");
	return(contract);
}

/* ------------------------- */
/* Stop Provisioning 	     */
/* ------------------------- */
private	void 	stop_application_provisioning(  char * contract )
{
	if ( check_debug() ) rest_log_message("coips:stop_server");
	cords_invoke_action( contract, "stop", _CORDS_SERVICE_AGENT, default_tls() );
	if ( check_debug() ) rest_log_message("coips:stop_server:done");
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
	return;
}


/* ------------------------- */
/* Update Image name 	     */
/* ------------------------- */
private	void	update_ezvm_image( struct cords_application * aptr )
{
	if ( check_debug() ) rest_log_message("coips:update_ezvm");
	return;
}

/*	------------------------------------------------------------------	*/
/* 	  actions and methods required for the coips instance category		*/
/*	------------------------------------------------------------------	*/
private	int	build_application( struct occi_category * optr, struct cords_application * aptr)
{
	char *	node;
	char *	contract;
	char *	package;
	char *	image;
	int	packages=0;

	/* ------------------------- */
	/* build a provisioning node */
	/* ------------------------- */

	if ( check_debug() ) rest_log_message("coips:build_application");

	if (!( node = build_application_node(aptr->image, aptr->provider) ))
		return( 800 );

	/* ------------------------- */
	/* negotiate the contracts   */
	/* ------------------------- */
	if (!( contract = negotiate_application_contract(node)))
		return( 801 );


	/* ------------------------- */
	/* provision the contract    */
	/* ------------------------- */
	if (!( contract = provision_application_contract(contract)))
		return( 801 );

	/* ------------------------- */
	/* For Each Package 	     */
	/* ------------------------- */
	while ( packages )
	{
		/* --------------------------------- */
		/* Install and configure the Package */
		/* --------------------------------- */
		if (!( package = install_application_package( contract , package ) ))
			return( 802 );
	}

	/* ------------------------- */
	/* Save Image 		     */
	/* ------------------------- */
	if (!( image = save_application_image( contract ) ))
		return( 803 );

	/* ------------------------- */
	/* Stop Provisioning 	     */
	/* ------------------------- */
	stop_application_provisioning( contract );

	/* ------------------------- */
	/* Delete Provisioning 	     */
	/* ------------------------- */
	delete_application_provisioning( contract );

	/* ------------------------- */
	/* Delete Provisioning 	     */
	/* ------------------------- */
	delete_application_node( node );

	/* ------------------------- */
	/* Update Image name 	     */
	/* ------------------------- */
	update_ezvm_image( aptr );

	if ( check_debug() ) rest_log_message("coips:build_application:done");
	return(0);
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
	else	return(build_application(optr,pptr));
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

	if (!( optr = occi_cords_application_builder( Coips.domain, "application" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = &cords_application_interface;

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

