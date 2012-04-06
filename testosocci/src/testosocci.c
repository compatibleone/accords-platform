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

struct	accords_configuration TestOsOcci = {
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
	"testosocci.xml",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(TestOsOcci.debug);		}
public	int	check_verbose()		{	return(TestOsOcci.verbose);		}
public	char *	default_publisher()	{	return(TestOsOcci.publisher);	}
public	char *	default_operator()	{	return(TestOsOcci.operator);	}
public	char *	default_tls()		{	return(TestOsOcci.tls);		}

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
	printf("\n   CompatibleOne Test OCCI Server : Version 1.0a.0.01");
	printf("\n   Beta Version : 06/04/2012");
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

/*	------------------------------------------------------------------	*/
/*		t e s t o s o c c i _ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	test_os_occi_operation( char * nptr )
{

	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_testosocci.xml");

	if (!( optr = occi_cords_compute_builder( TestOsOcci.domain,"compute" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	optr->access |= _OCCI_PRIVATE;
	last = optr;

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

	if (!( optr = occi_create_os_template( TestOsOcci.domain, "debian", "http://www.compatibleone.fr/template/os#debian", "system debian" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_create_os_template( TestOsOcci.domain, "ubuntu", "http://www.compatibleone.fr/template/os#ubuntu", "syste ubuntu" ) ))
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

