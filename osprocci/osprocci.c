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
#ifndef	_osprocci_c
#define	_osprocci_c

#include "standard.h"
#include "procci.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"

#include "osconfig.h"
#include "openstack.h"
#include "openstack.c"
#include "occiopenstack.c"
#include "osconfig.c"
#include "occiosconfig.c"

struct	accords_configuration OsProcci = {
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
	"osprocci.xml",
	"europe",
	"storage",
	"osprocci",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(OsProcci.debug);		}
public	int	check_verbose()		{	return(OsProcci.verbose);	}
public	char *	default_publisher()	{	return(OsProcci.publisher);	}
public	char *	default_operator()	{	return(OsProcci.operator);	}
public	char *	default_tls()		{	return(OsProcci.tls);		}
public	char *	default_zone()		{	return(OsProcci.zone);		}

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
/*		 o s p r o c c i _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads osprocci    configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	osprocci_load()
{
	load_accords_configuration( &OsProcci, "osprocci" );
	return;
}

private	int	banner()
{
	printf("\n   CompatibleOne Open Stack OsProcci : Version 1.0c.0.01");
	printf("\n   Beta Version : 08/07/2013");
	printf("\n   Copyright (c) 2011, 2013 Iain James Marshall, Prologue ");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*	------------------------------------------------------------------	*/
/*			o s p r o c c i _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * osprocci_initialise(  void * v,struct rest_server * sptr )
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
/*			o s p r o c c i _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	osprocci_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, OsProcci.user ) )
		return(0);
	else if ( strcmp( password, OsProcci.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			o s p r o c c i _ e x t e n s i o n 			*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * osprocci_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

#include "proccios.c"


/*	------------------------------------------------------------------	*/
/*		o s p r o c c i _ a l e r t _ r e l a y				*/
/*	------------------------------------------------------------------	*/
private	struct rest_response *  osprocci_alert_relay(
		void * i, 
		struct rest_client * cptr, 
		struct rest_response * aptr, 
		int status, char * message, 
		char * nature, 
		char * agent, char * tls)
{
	struct	rest_request * rptr;
	char subject[2048];

	/* detect and convert REST Alerts */
	/* ------------------------------ */
	if ((!( agent )) || ( strcmp( agent, "REST" ) != 0 ))
		return( occi_alert( i, cptr, aptr, status, message, nature, agent, tls ) );
	else if ((!( aptr )) || (!( rptr = aptr->request )))
		return( occi_alert( i, cptr, aptr, status, message, nature, agent, tls ) );
	else
	{
		sprintf(subject,"%s %s %s",rptr->method,rptr->object,message);
		return( occi_alert( i, cptr, aptr, status, subject, nature, "openstack", tls ) );
	}
}

/*	------------------------------------------------------------------	*/
/*			o s p r o c c i _ o p e r a t i o n			*/
/*	------------------------------------------------------------------	*/
private	int	osprocci_operation( char * nptr )
{

	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_openstack.xml");

	if (!( optr = build_openstack( OsProcci.domain ) ))
		return( 27 );

	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;


	if (!( optr = build_openstack_configuration( OsProcci.domain ) ))
		return( 27 );

	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	rest_initialise_log( OsProcci.monitor );

	set_occi_alert_relay( osprocci_alert_relay );

	if (!( OsProcci.identity ))
		return( occi_server(  nptr, OsProcci.restport, OsProcci.tls, OsProcci.threads, first, (char *) 0 ) );
	else
	{
		initialise_occi_publisher( OsProcci.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			OsProcci.user, OsProcci.password,
			OsProcci.identity,  nptr, 
			OsProcci.restport, OsProcci.tls, 
			OsProcci.threads, first ) );
	}
}


/*	------------------------------------------------------------------	*/
/*				o s p r o c c i 					*/
/*	------------------------------------------------------------------	*/
private	int	osprocci(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	osprocci_load();
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
				OsProcci.verbose=1;
				continue;
			case	'd'	:
				OsProcci.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = osprocci_operation(aptr) ))
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
	else	return( osprocci( argc, argv ) );
}


	/* ----------- */
#endif	/* _osprocci_c */
	/* ----------- */

