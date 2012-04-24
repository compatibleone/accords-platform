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
#ifndef	_paprocci_c
#define	_paprocci_c

#include "standard.h"
#include "procci.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"

struct	accords_configuration WpaProcci = {
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
	"paprocci.xml",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(WpaProcci.debug);	}
public	int	check_verbose()		{	return(WpaProcci.verbose);	}
public	char *	default_publisher()	{	return(WpaProcci.publisher);	}
public	char *	default_tls()		{	return(WpaProcci.tls);		}
public	char *	default_operator()	{	return(WpaProcci.operator);	}

/*!
 * Called in any case of failure in this procci. */
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
/*		 p a p r o c c i _ l o a d 		            */
/*	---------------------------------------------	*/
/*	this function loads paprocci configuration	    */
/*	from the xml configuration file.		        */
/*	---------------------------------------------	*/  
private	void	paprocci_load()
{
	load_accords_configuration( &WpaProcci, "paprocci" );
	return;
}

private	int	banner()
{
	printf("\n   CompatibleOne ProActive Procci : Version 1.0a.0.03");
	printf("\n   Beta Version : 28/11/2011");
	printf("\n   Copyright (c) 2011 Iain James Marshall, Prologue ");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*	------------------------------------------------------------------	*/
/*			p a p r o c c i _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * paprocci_initialise(  void * v,struct rest_server * sptr )
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
/*			p a p r o c c i _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	paprocci_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, WpaProcci.user ) )
		return(0);
	else if ( strcmp( password, WpaProcci.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			p a p r o c c i _ e x t e n s i o n 			*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * paprocci_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

#include "proccipa.c"

/*	------------------------------------------------------------------	*/
/*			p a p r o c c i _ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	paprocci_operation( char * nptr )
{

	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_proactive.xml");

	if (!( optr = build_proactive( WpaProcci.domain ) ))
		return( 27 );

	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;


	if (!( optr = build_proactive_configuration( WpaProcci.domain ) ))
		return( 27 );

	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	rest_initialise_log( WpaProcci.monitor );

	if (!( WpaProcci.identity ))
		return( occi_server(  nptr, WpaProcci.restport, WpaProcci.tls, WpaProcci.threads, first, (char *) 0 ) );
	else
	{
		initialise_occi_publisher( WpaProcci.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			WpaProcci.user, WpaProcci.password,
			WpaProcci.identity,  nptr, 
			WpaProcci.restport, WpaProcci.tls, 
			WpaProcci.threads, first ) );
	}
}


/*	------------------------------------------------------------------	*/
/*				p a p r o c c i 					*/
/*	------------------------------------------------------------------	*/
private	int	paprocci(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	paprocci_load();
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
				WpaProcci.verbose=1;
				continue;
			case	'd'	:
				WpaProcci.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv) ))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = paprocci_operation(aptr) ))
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
	else	return( paprocci( argc, argv ) );
}


	/* ----------- */
#endif	/* _paprocci_c */
	/* ----------- */

