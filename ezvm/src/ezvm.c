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
#ifndef	_ezvm_c
#define	_ezvm_c

#include "standard.h"
#include "ezvm.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"
#include "cordslang.h"
#include "cp.h"

struct	accords_configuration Ezvm = {
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
	"ezvm.xml",
	"europe",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(Ezvm.debug);		}
public	int	check_verbose()		{	return(Ezvm.verbose);		}
public	char *	default_publisher()	{	return(Ezvm.publisher);	}
public	char *	default_operator()	{	return(Ezvm.operator);	}
public	char *	default_tls()		{	return(Ezvm.tls);		}
public	char *	default_zone()		{	return(Ezvm.zone);		}

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
/*			e z v m _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads ezvm    configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	ezvm_load()
{
	load_accords_configuration( &Ezvm, "ezvm" );
	return;
}

private	int	banner()
{
	printf("\n   CompatibleOne Ezvm : Version 1.0a.0.02");
	printf("\n   Beta Version : 25/05/2012");
	printf("\n   Copyright (c) 2012 Iain James Marshall, Prologue");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*	------------------------------------------------------------------	*/
/*			e z v m _ i n i t i a l i s e				*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * ezvm_initialise(  void * v,struct rest_server * sptr )
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
/*			e z v m _ a u t h o r i s e 				*/
/*	------------------------------------------------------------------	*/
private	int	ezvm_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, Ezvm.user ) )
		return(0);
	else if ( strcmp( password, Ezvm.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			e z v m _ e x t e n s i o n 					*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * ezvm_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	------------------------------------------------------------------	*/
/*	inclusion of the actions and methods required for vm       handling	*/
/*	------------------------------------------------------------------	*/

/*	-------------------------------------------	*/
/* 	      b u i l d _ a p p l i c a t i o n  	*/
/*	-------------------------------------------	*/
private	struct rest_response * build_vm(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_vm * pptr;
	struct	occi_response * zptr;
	struct	occi_response * yptr;
	char *	sptr;
	int	status;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Failure" ) );
	else if ( pptr->state != 0 )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if (!( pptr->image ))
		return( rest_html_response( aptr, 400, "incorrect image" ) );
	else if (!( pptr->provider ))
		return( rest_html_response( aptr, 400, "incorrect provider" ) );
	else if (!( pptr->application ))
		return( rest_html_response( aptr, 400, "incorrect application" ) );
	else if (!( zptr = cords_invoke_action( pptr->application, _CORDS_BUILD, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( rest_html_response( aptr, 400, "build failure" ) );
	else if (!( zptr->response ))
	{
		zptr = occi_remove_response( zptr );
		return( rest_html_response( aptr, 400, "build failure" ) );
	}
	else if (( pptr->state = zptr->response->status) > 299 )
	{
		zptr = occi_remove_response( zptr );
		return( rest_html_response( aptr, pptr->state, "build failure" ) );
	}
	else
	{
		zptr = occi_remove_response( zptr );
		if (!( zptr = occi_simple_get( pptr->application, _CORDS_CONTRACT_AGENT, default_tls() ) ))
			return( rest_html_response( aptr, 440, "build failure" ) );
		else if (!( sptr = occi_extract_atribut( zptr, "occi", _CORDS_APPLICATION, "url" )))
		{
			zptr = occi_remove_response( zptr );
			return( rest_html_response( aptr, 441, "build failure" ) );
		}
		else if (!( pptr->url = allocate_string( sptr ) ))
		{
			zptr = occi_remove_response( zptr );
			return( rest_html_response( aptr, 500, "build failure" ) );
		}
		else	autosave_cords_vm_nodes();
		zptr = occi_remove_response( zptr );
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	   	   c r e a t e _ v m   			*/
/*	-------------------------------------------	*/
private	int	create_cords_vm(struct occi_category * optr, void * vptr)
{
	char 	buffer[2048];
	struct	occi_element *	root=(struct occi_element *) 0;
	struct	occi_element *	last=(struct occi_element *) 0;
	struct	occi_element *	eptr=(struct occi_element *) 0;
	struct	occi_response * zptr=(struct occi_response*) 0;
	char *	sptr;
	struct	occi_kind_node * nptr;
	struct	cords_vm * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if ( pptr->state )
		return(0);
	else if (!( pptr->image ))
		return(0);
	else if (!( pptr->provider ))
		return(0);
	else if (!( eptr = occi_create_element( "occi.application.image", pptr->image ) ))
		return(27);
	else if (!( eptr->previous = last ))
		root = eptr;
	else 	last->next = eptr;
	last = eptr;
	if (!( eptr = occi_create_element( "occi.application.provider", pptr->provider ) ))
		return(27);
	else if (!( eptr->previous = last ))
		root = eptr;
	else 	last->next = eptr;
	last = eptr;

	if (!( zptr = cords_create_instance( _CORDS_APPLICATION, _CORDS_CONTRACT_AGENT, root, default_tls() ) ))
		return( 0 );
	else if (!( sptr = occi_extract_location( zptr ) ))
	{
		zptr = occi_remove_response( zptr );
		return(0);
	}
	else
	{
		rest_add_http_prefix(buffer,2048,sptr);
		if ( pptr->application ) pptr->application = liberate( pptr->application );
		pptr->application = allocate_string( buffer );
		zptr = occi_remove_response( zptr );
		if ( pptr->application )
			return(0);
		else	return(27);
	}
}

/*	-------------------------------------------	*/
/* 	 	 r e t r i e v e _ v m   		*/
/*	-------------------------------------------	*/
private	int	retrieve_cords_vm(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_vm * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	  	    u p d a t e _ v m   		*/
/*	-------------------------------------------	*/
private	int	update_cords_vm(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_vm * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	  	    d e l e t e _ v m  		 	*/
/*	-------------------------------------------	*/
private	int	delete_cords_vm(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_vm * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( pptr->application ))
		return(0);
	else	
	{
		occi_simple_delete( pptr->application, _CORDS_CONTRACT_AGENT, default_tls() );
		return(0);
	}
}

private	struct	occi_interface	cords_vm_interface = {
	create_cords_vm,
	retrieve_cords_vm,
	update_cords_vm,
	delete_cords_vm
	};

/*	------------------------------------------------------------------	*/
/*			e z v m _ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	ezvm_operation( char * nptr )
{

	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_ezvm.xml");

	if (!( optr = occi_cords_vm_builder( Ezvm.domain,"vm" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	optr->callback = &cords_vm_interface;

	if (!( optr = occi_add_action( optr,"build","",build_vm)))
		return( 28 );

	if (!( optr = occi_cords_image_builder( Ezvm.domain,"image" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_cords_system_builder( Ezvm.domain,"system" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	if (!( optr = occi_cords_package_builder( Ezvm.domain,"package" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	rest_initialise_log( Ezvm.monitor );

	if (!( Ezvm.identity ))
		return( occi_server(  nptr, Ezvm.restport, Ezvm.tls, Ezvm.threads, first, (char *) 0 ) );
	else
	{
		initialise_occi_publisher( Ezvm.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			Ezvm.user, Ezvm.password,
			Ezvm.identity,  nptr, 
			Ezvm.restport, Ezvm.tls, 
			Ezvm.threads, first ) );
	}
}


/*	------------------------------------------------------------------	*/
/*				e z v m 					*/
/*	------------------------------------------------------------------	*/
private	int	ezvm(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	ezvm_load();
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
				Ezvm.verbose=1;
				continue;
			case	'd'	:
				Ezvm.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = ezvm_operation(aptr) ))
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
	else	return( ezvm( argc, argv ) );
}


	/* --------- */
#endif	/* _ezvm_c */
	/* --------- */

