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
#ifndef	_comons_c
#define	_comons_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"

struct	accords_configuration Comons = {
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
	"comons.xml",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(Comons.debug);		}
public	int	check_verbose()		{	return(Comons.verbose);		}
public	char *	default_publisher()	{	return(Comons.publisher);	}
public	char *	default_operator()	{	return(Comons.operator);	}
public	char *	default_tls()		{	return(Comons.tls);		}

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
/*	 	   c o m o n s _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads comons  configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	comons_load()
{
	load_accords_configuration( &Comons, "comons" );
	return;
}

#include "occibuilder.h"

private	int	banner()
{
	printf("\n   CompatibleOne Monitoring Services : Version 1.0a.0.03");
	printf("\n   Beta Version : 28/11/2011");
	printf("\n   Copyright (c) 2011 Iain James Marshall, Prologue");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*	------------------------------------------------------------------	*/
/*			c o m o n s _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * comons_initialise(  void * v,struct rest_server * sptr )
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
/*			c o m o n s _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	comons_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, Comons.user ) )
		return(0);
	else if ( strcmp( password, Comons.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			c o m o n s _ e x t e n s i o n 			*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * comons_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	------------------------------------------------------------------	*/
/* 	  actions and methods required for the comons instance category		*/
/*	------------------------------------------------------------------	*/
/* none for now */

/*	------------------------------------------------------------------	*/
/*			c o m o n s _ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	comons_operation( char * nptr )
{
	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_comons.xml");


	if (!( optr = occi_cords_monitor_builder( Comons.domain, "monitor" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;
	optr->access |= _OCCI_NO_PRICING;

	if (!( optr = occi_cords_event_builder( Comons.domain, "event" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;
	optr->access |= _OCCI_NO_PRICING;

	if (!( optr = occi_cords_report_builder( Comons.domain, "report" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;
	optr->access |= _OCCI_NO_PRICING;

	rest_initialise_log(Comons.monitor);

	if (!( Comons.identity ))
		return( occi_server(  nptr, Comons.restport, Comons.tls, Comons.threads, first,(char *) 0 ) );
	else
	{
		initialise_occi_publisher( Comons.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			Comons.user, Comons.password,
			Comons.identity,  nptr, 
			Comons.restport, Comons.tls, 
			Comons.threads, first ) );
	}
}

/*	------------------------------------------------------------------	*/
/*				c o m o n s 					*/
/*	------------------------------------------------------------------	*/
private	int	comons(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	comons_load();
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
				Comons.verbose=1;
				continue;
			case	'd'	:
				Comons.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = comons_operation(aptr) ))
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
	else	return( comons( argc, argv ) );
}


	/* --------- */
#endif	/* _comons_c */
	/* --------- */

