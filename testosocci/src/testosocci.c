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
#ifndef	_test_os_occi_c
#define	_test_os_occi_c

#include "standard.h"
#include "rest.h"
#include "occi.h"
#include "accords.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"
#include "cordslang.h"

struct	accords_configuration TestOsOcci = {
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
	"testosocci.xml",
	"europe",
	"storage",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(TestOsOcci.debug);		}
public	int	check_verbose()		{	return(TestOsOcci.verbose);		}
public	char *	default_publisher()	{	return(TestOsOcci.publisher);	}
public	char *	default_operator()	{	return(TestOsOcci.operator);	}
public	char *	default_tls()		{	return(TestOsOcci.tls);		}
public	char *	default_zone()		{	return(TestOsOcci.zone);		}

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
/*		t e s t o s o c c i _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads procci    configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	test_os_occi_load()
{
	load_accords_configuration( &TestOsOcci, "testosocci" );
	return;
}

private	int	banner()
{
	printf("\n   CompatibleOne Test OCCI Server : Version 1.0a.0.02");
	printf("\n   Beta Version : 25/05/2012");
	printf("\n   Copyright (c) 2012 Iain James Marshall, Prologue");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*	------------------------------------------------------------------	*/
/*		t e s t o s o c c i _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * test_os_occi_initialise(  void * v,struct rest_server * sptr )
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
/*		t e s t o s o c c i _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	test_os_occi_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, TestOsOcci.user ) )
		return(0);
	else if ( strcmp( password, TestOsOcci.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*		t e s t o s o c c i _ e x t e n s i o n 			*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * test_os_occi_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	-------------------------------------------------------------------	*/
/*		     	s t a r t _ o c c i _ c o m p u t e			*/
/*	-------------------------------------------------------------------	*/
private	struct	rest_response * start_occi_compute(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_compute * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state != _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );
	else
	{
		pptr->state = _OCCI_ACTIVE;
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------------------------------	*/
/*		     	s t o p _ o c c i _ c o m p u t e			*/
/*	-------------------------------------------------------------------	*/
private	struct	rest_response * stop_occi_compute(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_compute * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state == _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );
	else
	{
		pptr->state = _OCCI_IDLE;
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------------------------------	*/
/*		     	s a v e _ o c c i _ c o m p u t e			*/
/*	-------------------------------------------------------------------	*/
private	struct	rest_response * save_occi_compute(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_compute * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------------------------------	*/
/*		     s n a p s h o t _ o c c i _ c o m p u t e			*/
/*	-------------------------------------------------------------------	*/
private	struct	rest_response * snapshot_occi_compute(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_compute * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 		a l l o c _ f l o a t _ i p		*/
/*	-------------------------------------------	*/
private	struct	rest_response * alloc_float_ip(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_compute * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	----------------------------------------------	*/
/* 		d e a l l o c _ f l o a t _ i p		*/
/*	----------------------------------------------	*/
private	struct	rest_response * dealloc_float_ip(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_compute * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	------------------------------------------------------------------	*/
/*		t e s t o s o c c i _ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	test_os_occi_operation( char * nptr )
{

	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_testosocci.xml");

	set_autosave_cords_compute_name("osocci_compute.xml");
	set_autosave_cords_network_name("osocci_network.xml");
	set_autosave_cords_storage_name("osocci_storage.xml");

	if (!( optr = occi_cords_compute_builder( TestOsOcci.domain,"compute" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	optr->access |= _OCCI_PRIVATE;
	last = optr;

	if (!( optr = occi_add_action( optr,_CORDS_START,"",start_occi_compute)))
		return( 27 );
	else if (!( optr = occi_add_action( optr,_CORDS_SAVE,"",save_occi_compute)))
		return( 27 );
	else if (!( optr = occi_add_action( optr,_CORDS_SNAPSHOT,"",snapshot_occi_compute)))
		return( 27 );
	else if (!( optr = occi_add_action( optr,_CORDS_STOP,"",stop_occi_compute)))
		return( 27 );
	else if (!( optr = occi_add_action( optr,"alloc_float_ip","",alloc_float_ip)))
		return( 27 );
	else if (!( optr = occi_add_action( optr,"dealloc_float_ip","",dealloc_float_ip)))
		return( 27 );

	if (!( optr = occi_cords_storage_builder( TestOsOcci.domain,"storage" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	optr->access |= _OCCI_PRIVATE;
	last = optr;

	if (!( optr = occi_cords_network_builder( TestOsOcci.domain,"network" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	optr->access |= _OCCI_PRIVATE;
	last = optr;

	if (!( optr = occi_create_resource_template( TestOsOcci.domain, "small", "http://www.compatibleone.fr/template/resource#small", "small compute" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_create_resource_template( TestOsOcci.domain, "medium", "http://www.compatibleone.fr/template/resource#medium", "medium compute" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_create_resource_template( TestOsOcci.domain, "large", "http://www.compatibleone.fr/template/resource#large", "large compute" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_create_os_template( TestOsOcci.domain, "debian", "http://www.compatibleone.fr/template/os#debian", "system debian" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_create_os_template( TestOsOcci.domain, "ubuntu", "http://www.compatibleone.fr/template/os#ubuntu", "system ubuntu" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_create_os_template( TestOsOcci.domain, "redhat", "http://www.compatibleone.fr/template/os#redhat", "system redhat" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_create_os_template( TestOsOcci.domain, "suze", "http://www.compatibleone.fr/template/os#suze", "system suze" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_create_os_template( TestOsOcci.domain, "centos", "http://www.compatibleone.fr/template/os#centos", "system centos" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_create_os_template( TestOsOcci.domain, "fedora", "http://www.compatibleone.fr/template/os#fedora", "system fedora" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	rest_initialise_log( TestOsOcci.monitor );

	if (!( TestOsOcci.identity ))
		return( occi_server(  nptr, TestOsOcci.restport, TestOsOcci.tls, TestOsOcci.threads, first, (char *) 0 ) );
	else
	{
		initialise_occi_publisher( TestOsOcci.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			TestOsOcci.user, TestOsOcci.password,
			TestOsOcci.identity,  nptr, 
			TestOsOcci.restport, TestOsOcci.tls, 
			TestOsOcci.threads, first ) );
	}
}


/*	------------------------------------------------------------------	*/
/*			t e s t o s o c c i 					*/
/*	------------------------------------------------------------------	*/
private	int	testosocci(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	test_os_occi_load();
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
				TestOsOcci.verbose=1;
				continue;
			case	'd'	:
				TestOsOcci.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = test_os_occi_operation(aptr) ))
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
	else	return( testosocci( argc, argv ) );
}


	/* --------------- */
#endif	/* _test_os_occi_c */
	/* --------------- */

