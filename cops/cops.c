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
#ifndef	_cops_c
#define	_cops_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occiresolver.h"
#include "occibuilder.h"

struct	accords_configuration Cops = {
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
	"cops.xml",
	"europe",
	"storage",
	"cops",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(Cops.debug);		}
public	int	check_verbose()		{	return(Cops.verbose);		}
public	char *	default_publisher()	{	return(Cops.publisher);		}
public	char *	default_operator()	{	return(Cops.operator);		}
public	char *	default_tls()		{	return(Cops.tls);		}
public	char *	default_zone()		{	return(Cops.zone);		}

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
/*		c o p s _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads cops  configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	cops_load()
{
	load_accords_configuration( &Cops, "cops" );
	return;
}

#include "occibuilder.h"

private	int	banner()
{
	printf("\n   CompatibleOne Placement Services COPS : Version 1.0b.0.01");
	printf("\n   Beta Version : 27/07/2012");
	printf("\n   Copyright (c) 2011, 2012 Iain James Marshall, Prologue");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*	------------------------------------------------------------------	*/
/*			c o p s _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * cops_initialise(  void * v,struct rest_server * sptr )
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
/*			c o p s _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	cops_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, Cops.user ) )
		return(0);
	else if ( strcmp( password, Cops.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			c o p s _ e x t e n s i o n 				*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * cops_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

#include "comonsconnection.c"
#include "copsquantity.c"
#include "copsoperation.c"
#include "copsplacement.c"

#include "federation.h"
#include "federation.c"
#include "occifederation.c"

/*	------------------------------------------------------------------	*/
/*			c o p s _ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	cops_operation( char * nptr )
{
	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_cops.xml");

	if (!( optr = occi_cords_placement_builder( Cops.domain, "placement" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = &placement_interface;

	if (!( optr = occi_add_action( optr,_CORDS_CHOOSE,"",choose_placement)))
		return( 28 );
	else if (!( optr = occi_add_action( optr,"consume","",consume_placement)))
		return( 28 );
	else if (!( optr = occi_add_action( optr,"restore","",restore_placement)))
		return( 28 );
	else if (!( optr = occi_add_action( optr,"release","",release_placement)))
		return( 28 );

	if (!( optr = occi_cords_quantity_builder( Cops.domain, "quantity" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_add_action( optr,"reserve","",reserve_quantity)))
		return( 28 );
	else if (!( optr = occi_add_action( optr,"consume","",consume_quantity)))
		return( 28 );
	else if (!( optr = occi_add_action( optr,"restore","",restore_quantity)))
		return( 28 );
	else if (!( optr = occi_add_action( optr,"release","",release_quantity)))
		return( 28 );

	if (!( optr = occi_cords_federation_builder( Cops.domain, "federation" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = comons_connection_builder( Cops.domain ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = comons_packet_builder( Cops.domain, "packet_cops.xml" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_cords_algorithm_builder( Cops.domain, "algorithm" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if (!( optr = occi_cords_quota_builder( Cops.domain, "quota" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	rest_initialise_log( Cops.monitor );

	if (!( Cops.identity ))
		return( occi_server(  nptr, Cops.restport, Cops.tls, Cops.threads, first, (char *) 0 ) );
	else
	{
		initialise_occi_publisher( Cops.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			Cops.user, Cops.password,
			Cops.identity,  nptr, 
			Cops.restport, Cops.tls, 
			Cops.threads, first ) );
	}
}

/*	------------------------------------------------------------------	*/
/*				c o p s 					*/
/*	------------------------------------------------------------------	*/
private	int	cops(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	cops_load();
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
				Cops.verbose=1;
				continue;
			case	'd'	:
				Cops.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = cops_operation(aptr) ))
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
	else	return( cops( argc, argv ) );
}


	/* --------- */
#endif	/* _cops_c */
	/* --------- */

