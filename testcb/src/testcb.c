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
#ifndef	_test_cords_broker_c
#define	_test_cords_broker_c

#include "standard.h"
#include "cb.h"
#include "occilogin.h"

struct	cords_broker_config
{
	char *	publisher;
	char *	host;
	char *	agent;
	char *	result;
	char *	tls;
} Cb = 	{
	_CORDS_DEFAULT_PUBLISHER,
	_CORDS_DEFAULT_PUBLISHER,
	_CORDS_BROKER_AGENT,
	(char *) 0,
	(char *) 0
	};

private	int	debug=0;
private	int	verbose=0;
public	int	check_debug()		{	return(debug);		}
public	int	check_verbose()		{	return(verbose);	}
public	char *	default_publisher()	{	return(Cb.publisher);	}
public	char *	default_tls()		{	return(Cb.tls);		}

public	int 	failure( int v, char * mptr, char * aptr )
{
	printf("\ncords: error(%u): %s %s\n",v,(mptr ? mptr : "" ), (aptr ? aptr : "" ) );
	return(v);
}

/*	-----------------------------------------------------	*/
/*		c o r d s _ i n s t a n c e _ p l a n		*/
/*	-----------------------------------------------------	*/
private	int	cords_instance_plan( char * host, char * plan, char * agent, char * result )
{
	struct	occi_response * zptr;
	char	*	sptr;
	initialise_occi_resolver( host, (char *) 0, (char *) 0, (char *) 0 );
	if (!( sptr = occi_unquoted_value( plan )))
		return(500);
	else if (!( zptr =  cords_invoke_action( sptr, _CORDS_INSTANCE, agent, Cb.tls ) ))
		return(501);
	else
	{
		zptr = occi_remove_response( zptr );
		return( 0 );
	}
}

/*	-----------------------------------------------------	*/
/*	t e s t _ c o r d s _ b r o k e r _ o p e r a t i o n	*/
/*	-----------------------------------------------------	*/
private	int	ll_test_cords_broker_operation( char * filename )
{
	struct	xml_element * dptr;
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;
	char *	nptr;
	char	nameplan[512];
	int	status;
	if (!(nptr = Cb.result))
		sprintf((nptr=nameplan),"instance_%s",filename);
	if (!( Cb.host ))
		return( failure(1,"requires","publication host"));
	else if (!( Cb.agent ))
		return( failure(2,"requires","parser agent name"));
	else if (!( filename ))
		return( failure(3,"requires","cords filename"));
	else if (!( dptr = document_parse_file( filename ) ))
		return( failure(4,"parse error",filename));
	else if (!( eptr = document_element( dptr, _CORDS_MANIFEST ) ))
		return( failure(5,"incorrect document",filename));
	else if (!( aptr = document_atribut( eptr, _CORDS_PLAN ) ))
		return( failure(6,"incorrect request",filename));
	else if ((status = cords_instance_plan( Cb.host, aptr->value, Cb.agent, nptr )) != 0)
		return( failure(status,"provisioning plan",aptr->value));
	else	return( 0 );
}

/*	-----------------------------------------------------	*/
/*	t e s t _ c o r d s _ b r o k e r _ o p e r a t i o n	*/
/*	-----------------------------------------------------	*/
private	int	test_cords_broker_operation( char * filename )
{
	int	status;
	char *	auth;

	initialise_occi_resolver( _DEFAULT_PUBLISHER, (char *) 0, (char *) 0, (char *) 0 );

	if (!( auth = login_occi_user( "test-broker","co-system",Cb.agent, Cb.tls ) ))
		return(403);
	else 	(void) occi_client_authentication( auth );

	status = ll_test_cords_broker_operation( filename );

	(void) logout_occi_user( "test-broker","co-system",Cb.agent, auth, Cb.tls );	

	return( status );
}

/*	-----------------------------------------------------	*/
/*	   t e s t _ c o r d s _ b r o k e r _ c o m a n d 	*/
/*	-----------------------------------------------------	*/
private int	test_cords_broker_command( int	argc, char * argv[] )
{
	int	argi=1;
	char *	aptr;

	while (argi < argc)
	{
		if (!( aptr = argv[argi++] ))
			break;
		else if ( *aptr !=  '-')
			return( test_cords_broker_operation( aptr ) );
		else 
		{
			aptr++;
			if ( *aptr ==  '-')
			{
				aptr++;
				if (!( strcmp( aptr, "verbose" ) ))
				{	verbose=1; continue;	}
				else if (!( strcmp( aptr, "debug" ) ))
				{	debug=1; continue;	}
				else if (!( strcmp( aptr, "tls" ) ))
				{
					Cb.tls = argv[argi++];
					continue;
				}
				else if (!( strcmp( aptr, "publisher" ) ))
				{
					Cb.publisher = argv[argi++];
					continue;
				}
				else if (!( strcmp( aptr, "host" ) ))
				{
					Cb.host = argv[argi++];
					continue;
				}
				else if (!( strcmp( aptr, "agent" ) ))
				{
					Cb.agent = argv[argi++];
					continue;
				}
				else if (!( strcmp( aptr, "result" ) ))
				{
					Cb.result = argv[argi++];
					continue;
				}
				else
				{
					printf("incorrect option : %s\n",aptr);
					break;
				}
			
			}
			else
			{
				switch( *(aptr+1) )
				{
				case	'e'	:
					set_xml_echo(1);
					continue;
				case	'd'	:
					debug = 1;
					continue;
				case	'v'	:
					verbose = 1;
					continue;
				case	'h'	:
					Cb.host = (aptr+2);
					continue;
				case	'a'	:
					Cb.agent = (aptr+2);
					continue;
				case	'r'	:
					Cb.result = (aptr+2);
					continue;
				default		:
					printf("incorrect option : %s \n",aptr);
					break;
				}
			}
		}
	}
	return(0);
}

/*	-----------------------------------------------------	*/
/*	   t e s t _ c o r d s _ b r o k e r _ b a n n e r 	*/
/*	-----------------------------------------------------	*/
private	int	test_cords_broker_banner(char * n)
{
	printf("\n   Cords Broker : Version 1.0.a.0.01 ");
	printf("\n   Beta Version 27/07/2011 \n");
	printf("\n   Copyright (c) 2011 Iain James Marshall, Prologue ");
	printf("\n   Usage : \n");
	printf("\n   --tls  <name>        specify the tls configuration  ");
	printf("\n   --host <host>        specify the publisher hostname ");
	printf("\n   --agent <name>       specify the name of the agent ");
	printf("\n   --result <filename>  specify the output plan filename ");
	printf("\n   --verbose            activate verbose messages ");
	printf("\n   --debug              activate debug messages \n");
	printf("\n   Example : \n");
	printf("\n   %s --host http://127.0.0.1:8086 cords.xml \n\n",n);
	return(0);
}

/*	-----------------------------------------------------	*/
/*	     t e s t _ c o r d s _ b r o k e r _ m a i n	*/
/*	-----------------------------------------------------	*/
public	int	main(int argc, char * argv[])
{
	if ( argc == 1 )
		return( test_cords_broker_banner(argv[0]) );
	else	return( test_cords_broker_command( argc, argv ) );
}


#endif

