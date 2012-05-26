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
#include "occiresolver.h"
#include "cordslang.h"

struct	accords_configuration Comons = {
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
	"comons.xml",
	"europe",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(Comons.debug);		}
public	int	check_verbose()		{	return(Comons.verbose);		}
public	char *	default_publisher()	{	return(Comons.publisher);	}
public	char *	default_operator()	{	return(Comons.operator);	}
public	char *	default_tls()		{	return(Comons.tls);		}
public	char *	default_zone()		{	return(Comons.zone);		}

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
	printf("\n   CompatibleOne Monitoring Services : Version 1.0a.0.05");
	printf("\n   Beta Version : 25/05/2012");
	printf("\n   Copyright (c) 2011, 2012 Iain James Marshall, Prologue");
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
/*	-------------------------------------------------------		*/
/*		    c o r d s _ p o s t _ e v e n t			*/
/*	-------------------------------------------------------		*/
public	int	comons_create_session( char * service, char * account, char * agent, char * tls )
{
	char	*	ihost;
	struct	occi_client * kptr;
	struct	occi_request * qptr;
	struct	occi_response * yptr;
	struct	occi_response * zptr;
	struct	occi_element * dptr;
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;
	struct	xml_atribut * bptr;
	struct	cordscript_element * lptr;
	struct	cordscript_element * rvalue;
	char	buffer[2048];

	if (!( ihost = occi_resolve_category_provider(_CORDS_SESSION,agent, tls) ))
		return(46);
	else
	{
		sprintf(buffer,"%s/%s/",ihost,_CORDS_SESSION);
		liberate( ihost );
	}

	if (!( kptr = occi_create_client( buffer, agent, tls ) ))
		return(46);
	else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
	{
		kptr = occi_remove_client( kptr );
		return(50);
	}
	else if ((!(dptr=occi_request_element(qptr,"occi.session.service" , service ) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.session.account" , account ) )))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return(51);
	}
	else if (!( yptr = occi_client_post( kptr, qptr ) ))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return(52);
	}
	else
	{
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return(0);
	}
}

/*	-------------------------------------------	*/
/* 	      c r e a t e _ s e s s i o n  		*/
/*	-------------------------------------------	*/
private	int	create_cords_session(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_session * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( pptr->name ))
		return( 0 ); 
	else	return( 0 );
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ s e s s i o n  		*/
/*	-------------------------------------------	*/
private	int	retrieve_cords_session(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_session * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ s e s s i o n 	 	*/
/*	-------------------------------------------	*/
private	int	update_cords_session(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_session * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ s e s s i o n  		*/
/*	-------------------------------------------	*/
private	int	delete_cords_session(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_session * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

private	struct	occi_interface	cords_session_interface = {
	create_cords_session,
	retrieve_cords_session,
	update_cords_session,
	delete_cords_session
	};


/*	-------------------------------------------------------		*/
/*		    c o r d s _ c r e a t e _ s t r e a m		*/
/*	-------------------------------------------------------		*/
public	int	comons_create_stream( char * service, char * consumer, char * agent, char * tls )
{
	char	*	ihost;
	struct	occi_client * kptr;
	struct	occi_request * qptr;
	struct	occi_response * yptr;
	struct	occi_response * zptr;
	struct	occi_element * dptr;
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;
	struct	xml_atribut * bptr;
	struct	cordscript_element * lptr;
	struct	cordscript_element * rvalue;
	char	buffer[2048];

	if (!( ihost = occi_resolve_category_provider(_CORDS_STREAM,agent, tls) ))
		return(46);
	else 
	{
		sprintf(buffer,"%s/%s/",ihost,_CORDS_STREAM);
		liberate( ihost );
	}

	if (!( kptr = occi_create_client( buffer, agent, tls ) ))
		return(46);
	else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
	{
		kptr = occi_remove_client( kptr );
		return(50);
	}
	else if ((!(dptr=occi_request_element(qptr,"occi.stream.service" , service  ) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.stream.consumer" , consumer) )))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return(51);
	}
	else if (!( yptr = occi_client_post( kptr, qptr ) ))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return(52);
	}
	else
	{
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return(0);
	}
}

/*	-------------------------------------------	*/
/* 	      c r e a t e _ s t r e a m  		*/
/*	-------------------------------------------	*/
private	int	create_cords_stream(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_stream * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( pptr->name ))
		return( 0 ); 
	else	return( 0 );
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ s t r e a m  		*/
/*	-------------------------------------------	*/
private	int	retrieve_cords_stream(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_stream * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ s t r e a m 	 	*/
/*	-------------------------------------------	*/
private	int	update_cords_stream(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_stream * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ s t r e a m  		*/
/*	-------------------------------------------	*/
private	int	delete_cords_stream(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_stream * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

private	struct	occi_interface	cords_stream_interface = {
	create_cords_stream,
	retrieve_cords_stream,
	update_cords_stream,
	delete_cords_stream
	};

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

	if (!( optr = occi_cords_session_builder( Comons.domain, "session" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = &cords_session_interface;
	optr->access |= _OCCI_NO_PRICING;

	if (!( optr = occi_cords_consumer_builder( Comons.domain, "consumer" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;
	optr->access |= _OCCI_NO_PRICING;

	if (!( optr = occi_cords_connection_builder( Comons.domain, "connection" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback = (void *) 0;
	optr->access |= ( _OCCI_NO_PRICING | _OCCI_PRIVATE );

	if (!( optr = occi_cords_stream_builder( Comons.domain, "stream" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = &cords_stream_interface;
	optr->access |= _OCCI_NO_PRICING;

	if (!( optr = occi_cords_metric_builder( Comons.domain, "metric" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;
	optr->access |= _OCCI_NO_PRICING;

	if (!( optr = occi_cords_alert_builder( Comons.domain, "alert" ) ))
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

