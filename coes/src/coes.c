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
#ifndef	_coes_c
#define	_coes_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occiresolver.h"
#include "occibuilder.h"

struct	accords_configuration Coes = {
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
	"coes.xml",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(Coes.debug);		}
public	int	check_verbose()		{	return(Coes.verbose);		}
public	char *	default_publisher()	{	return(Coes.publisher);		}
public	char *	default_operator()	{	return(Coes.operator);		}
public	char *	default_tls()		{	return(Coes.tls);		}

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
/*		c o e s _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads coes  configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	coes_load()
{
	load_accords_configuration( &Coes, "coes" );
	return;
}

#include "occibuilder.h"

private	int	banner()
{
	printf("\n   CompatibleOne Elasticity Services COES : Version 1.0a.0.06");
	printf("\n   Beta Version : 05/03/2012");
	printf("\n   Copyright (c) 2011, 2012 Iain James Marshall, Prologue");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*	------------------------------------------------------------------	*/
/*			c o e s _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * coes_initialise(  void * v,struct rest_server * sptr )
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
/*			c o e s _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	coes_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, Coes.user ) )
		return(0);
	else if ( strcmp( password, Coes.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			c o e s _ e x t e n s i o n 				*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * coes_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	-------------------------------------------------------------------	*/
/*			r e s o l v e _ p l a c e m e n t			*/
/*	-------------------------------------------------------------------	*/
/*	this is the rudimentary placement algorithm on the first come basis	*/
/*	and will be extended to take into consideration zone and price info	*/
/*	-------------------------------------------------------------------	*/
private	char *	resolve_placement( char * provider, char * zone, char * agent, char * tls )
{
/*	------------------------------------------------------------------	*/
/* 	  actions and methods required for the coes instance category		*/
/*	------------------------------------------------------------------	*/
	struct	occi_response 	* zptr;
	struct	occi_response 	* yptr;
	struct	occi_element 	* eptr;
	char			* solution;

	/* ------------------------------------------------------ */
	/* attempt to resolve agencys of the "provider" category */
	/* ------------------------------------------------------ */
	if ( zone )
	{
		if (!( zptr = occi_resolve_by_zone( provider, zone, agent ) ))
			return( (char *) 0 );
	}
	else if (!( zptr = occi_resolver( provider, agent ) ))
		return( (char *) 0 );
	
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
		else 	return( allocate_string( eptr->value ) );
	}
	return((char *) 0);
}

/*	--------------------------------------------------	*/
/*	c r e a t e _ p l a c e m e n t _ s o l u t i o n	*/
/*	--------------------------------------------------	*/
/*	this will analyse the placement request and choose	*/
/*	an appropriate placement based on the node, zone &	*/
/*	provider information.					*/
/*	--------------------------------------------------	*/
private	int	create_placement_solution(
		struct occi_category * optr, 
		struct cords_placement * pptr,
		char * agent,
		char * tls )
{
	if ( pptr->solution )
	{
		liberate( pptr->solution );
		pptr->solution = (char *) 0;
	}
	pptr->solution = resolve_placement( pptr->provider, pptr->zone, agent, tls );
	return(0);
}

/*	-------------------------------------------	*/
/* 	      c r e a t e _ p l a c e m e n t  		*/
/*	-------------------------------------------	*/
private	int	create_placement(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_placement * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( pptr->node ))
		return( 0 ); 
	else	return(create_placement_solution(optr,pptr, _CORDS_CONTRACT_AGENT, Coes.tls));
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ p l a c e m e n t  	*/
/*	-------------------------------------------	*/
private	int	retrieve_placement(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_placement * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ p l a c e m e n t 	 	*/
/*	-------------------------------------------	*/
private	int	update_placement(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_placement * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ p l a c e m e n t  		*/
/*	-------------------------------------------	*/
private	int	delete_placement(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_placement * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

private	struct	occi_interface	placement_interface = {
	create_placement,
	retrieve_placement,
	update_placement,
	delete_placement
	};


/*	------------------------------------------------------------------	*/
/*			c o e s _ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	coes_operation( char * nptr )
{
	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_coes.xml");

	if (!( optr = occi_cords_placement_builder( Coes.domain, "placement" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = &placement_interface;

	if (!( optr = occi_cords_connection_builder( Coes.domain, "connection" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback = (void *) 0;
	optr->access |= (_OCCI_PRIVATE | _OCCI_CONSUMER);

	if (!( optr = occi_cords_algorithm_builder( Coes.domain, "algorithm" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if (!( optr = occi_cords_quota_builder( Coes.domain, "quota" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	rest_initialise_log( Coes.monitor );

	if (!( Coes.identity ))
		return( occi_server(  nptr, Coes.restport, Coes.tls, Coes.threads, first, (char *) 0 ) );
	else
	{
		initialise_occi_publisher( Coes.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			Coes.user, Coes.password,
			Coes.identity,  nptr, 
			Coes.restport, Coes.tls, 
			Coes.threads, first ) );
	}
}

/*	------------------------------------------------------------------	*/
/*				c o e s 					*/
/*	------------------------------------------------------------------	*/
private	int	coes(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	coes_load();
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
				Coes.verbose=1;
				continue;
			case	'd'	:
				Coes.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = coes_operation(aptr) ))
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
	else	return( coes( argc, argv ) );
}


	/* --------- */
#endif	/* _coes_c */
	/* --------- */

