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
#ifndef	_conets_c
#define	_conets_c

#include <time.h>
#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"
#include "occiresolver.h"
#include "conetsengine.h"
#include "stdnode.h"

struct	accords_configuration Conets = {
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
	"conets.xml",
	"europe",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

static char * conets_agent;

public	int	check_debug()			{	return(Conets.debug);			}
public	int	check_verbose()			{	return(Conets.verbose);			}
public	char *	default_publisher()		{	return(Conets.publisher);		}
public	char *	default_operator()		{	return(Conets.operator);		}
public	char *	default_identity()		{	return(Conets.identity);		}
public	char *	default_tls()			{	return(Conets.tls);			}
public	char *	default_zone()			{	return(Conets.zone);			}

/*	-----------------------------------------------------------------	*/
/* 	 i p a d d r e s s  O C C I  i n t e r f a c e  f u n c t i o n s	*/
/*	-----------------------------------------------------------------	*/
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
/*		c o n e t s _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads conets  configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	conets_load()
{
	load_accords_configuration( &Conets, "conets" );
	return;
}

#include "occibuilder.h"

private	int	banner()
{
	printf("\n   CompatibleOne Networking Services : Version 1.0a.0.04");
	printf("\n   Beta Version : 25/05/2012");
	printf("\n   Copyright (c) 2011, 2012 Iain James Marshall, Prologue");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*	------------------------------------------------------------------	*/
/*			c o n e t s _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * conets_initialise(  void * v,struct rest_server * sptr )
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
/*			c o n e t s _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	conets_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, Conets.user ) )
		return(0);
	else if ( strcmp( password, Conets.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			c o n e t s _ e x t e n s i o n 			*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * conets_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	------------------------------------------------------------------	*/
/* 	  actions and methods required for the conets instance category		*/
/*	------------------------------------------------------------------	*/
#include "conetsdomain.c"
#include "conetsip.c"
#include "comonsconnection.c"

/*	-------------------------------------------	*/
/*	      a d d _ f i r e w a l l _ p o r t		*/
/*	-------------------------------------------	*/
private	int	add_firewall_port( struct cords_firewall * pptr, char * source, struct occi_element * eptr )
{
	struct	occi_response * zptr;
	char *	target;
	if ( eptr->value )
	{
		if ((target = standard_message_link_value( eptr->value )) != (char *) 0)
		{
			if ((zptr = cords_create_link( 
					source, target, 
					_CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
			{
				pptr->ports++;
				zptr = occi_remove_response( zptr );
			}
			liberate( target );
		}
	}
	return(0);
}

/*	-------------------------------------------	*/
/* 	      b u i l d _ f i r e w a l l		*/
/*	-------------------------------------------	*/
private	struct rest_response * build_firewall(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	occi_response * zptr;
	struct	occi_element * eptr;
	struct	occi_element * fptr;
	struct	cords_firewall * pptr;
	char *	sptr;
	char	buffer[1024];
	int	status;

	struct	standard_node * nptr;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Incorrect Request" ) );
	else if ( pptr->state > 0 )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if (!( pptr->node ))
		return( rest_html_response( aptr, 200, "OK" ) );
	else if (!( nptr = get_standard_node( pptr->node, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( rest_html_response( aptr, 400, "Incorrect Request ID" ) );
	else
	{
		/* ----------------------------------- */
		/* first process network element ports */
		/* ----------------------------------- */
		sprintf(buffer,"%s/firewall/%s",Conets.identity,pptr->id);
		pptr->ports=0;
		if ((zptr = cords_delete_links( buffer, _CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
			zptr = occi_remove_response( zptr );
		for (	fptr = first_standard_message_link( nptr->network.message );
			fptr != (struct occi_element *) 0;
			fptr = next_standard_message_link( fptr ) )
			add_firewall_port( pptr, buffer, fptr );			

		/* ------------------------------------ */
		/* then process the image package ports */
		/* ------------------------------------ */
		for (	eptr = first_standard_message_link( nptr->image.message );
			eptr != (struct occi_element *) 0;
			eptr = next_standard_message_link( eptr ) )
		{
			/* ---------------------------------- */
			/* retrieve the image package message */
			/* ---------------------------------- */
			if (!( sptr = standard_message_link_value( eptr->value ) ))
				continue;
			else 	status = get_standard_message( 
					&nptr->package, sptr,
					_CORDS_CONTRACT_AGENT, default_tls());
			liberate( sptr );
			if (status != 0)
				continue;

			/* ----------------------------------------- */
			/* process the port messages of this package */
			/* ----------------------------------------- */
			for (	fptr = first_standard_message_link( nptr->package.message );
				fptr != (struct occi_element *) 0;
				fptr = next_standard_message_link( fptr ) )
				add_firewall_port( pptr, buffer, fptr );			
		}
		pptr->state = 1;
		nptr = drop_standard_node( nptr );
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	------------------------------------------------------------------	*/
/*			c o n e t s _ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	conets_operation( char * nptr )
{
	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_conets.xml");

        conets_agent = allocate_string(nptr);    
   
	if (!( optr = occi_cords_ipaddress_builder( Conets.domain, "ipaddress" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = &ipaddress_interface;

	if (!( optr = occi_cords_domain_builder( Conets.domain, "domain" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if (!( optr = occi_cords_network_builder( Conets.domain,"network" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_cords_port_builder( Conets.domain, "port" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if (!( optr = occi_cords_firewall_builder( Conets.domain, "firewall" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if (!( optr = occi_add_action( optr,_CORDS_BUILD,"",build_firewall)))
		return( 28 );

	if (!( optr = comons_connection_builder( Conets.domain ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = comons_packet_builder( Conets.domain, "packet_conets.xml" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_cords_iprange_builder( Conets.domain, "iprange" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	rest_initialise_log( Conets.monitor );

	if (!( Conets.identity ))
		return( occi_server(  nptr, Conets.restport, Conets.tls, Conets.threads, first,(char *) 0 ) );
	else
	{
		initialise_occi_publisher( Conets.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			Conets.user, Conets.password,
			Conets.identity,  nptr, 
			Conets.restport, Conets.tls, 
			Conets.threads, first ) );
	}
}

/*	------------------------------------------------------------------	*/
/*				c o n e t s 					*/
/*	------------------------------------------------------------------	*/
private	int	conets(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	conets_load();
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
				Conets.verbose=1;
				continue;
			case	'd'	:
				Conets.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = conets_operation(aptr) ))
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
	else	return( conets( argc, argv ) );
}


	/* --------- */
#endif	/* _conets_c */
	/* --------- */

