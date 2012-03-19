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
#ifndef	_test_cords_parser_c
#define	_test_cords_parser_c

#include "standard.h"
#include "cp.h"
#include "occilogin.h"

struct	cords_parser_config
{
	char *	accept;
	char *	publisher;
	char *	host;
	char *	agent;
	char *	result;
	char *	tls;
	int	xsd;
} Cp = 	{
	(char *) 0,
	_CORDS_DEFAULT_PUBLISHER,
	_CORDS_DEFAULT_PUBLISHER,
	_CORDS_PARSER_AGENT,
	(char *) 0,
	(char *) 0,
	1
	};

private	int	debug=0;
private	int	verbose=0;
public	int	check_debug()		{	return(debug);		}
public	int	check_verbose()		{	return(verbose);	}
public	char *	default_publisher()	{	return(Cp.publisher);	}
public	char *	default_tls()		{	return(Cp.tls);		}

public	int 	failure( int v, char * mptr, char * aptr )
{
	printf("\ncords: error(%u): %s %s\n",v,(mptr ? mptr : "" ), (aptr ? aptr : "" ) );
	return(v);
}

/*	-----------------------------------------------------	*/
/*	t e s t _ c o r d s _ p a r s e r _ o p e r a t i o n	*/
/*	-----------------------------------------------------	*/
private	int	ll_test_cords_parser_operation( char * filename )
{
	struct	xml_element * dptr;
	char	nameplan[512];
	if (!( Cp.host ))
		return( failure(1,"requires","publication host"));
	else if (!( Cp.agent ))
		return( failure(2,"requires","parser agent name"));
	else if (!( filename ))
		return( failure(3,"requires","cords filename"));
	else if (!( dptr = cords_document_parser( Cp.host, filename, Cp.agent, Cp.tls, Cp.xsd ) ))
		return( failure(4,"parse error",filename));
	else if (!( Cp.result ))
	{
		sprintf(nameplan,"plan_%s",filename);
		if (!( Cp.result = allocate_string( nameplan ) ))
			return( failure(4,"allocation","result filename"));
	}
	dptr = cords_serialise_document( dptr, Cp.result );
	return( 0 );
}

/*	-----------------------------------------------------	*/
/*	t e s t _ c o r d s _ p a r s e r _ o p e r a t i o n	*/
/*	-----------------------------------------------------	*/
private	int	test_cords_parser_operation( char * filename )
{
	int	status;
	char *	auth;

	if ( Cp.accept )
	{
		occi_client_accept( Cp.accept );
	}

	initialise_occi_resolver( _DEFAULT_PUBLISHER, (char *) 0, (char *) 0, (char *) 0 );

	if (!( auth = login_occi_user( "test-parser","co-system",Cp.agent, Cp.tls ) ))
		return(403);
	else 	(void) occi_client_authentication( auth );

	status = ll_test_cords_parser_operation( filename );

	(void) logout_occi_user( "test-parser","co-system",Cp.agent, auth, Cp.tls );	

	return( status );
}

/*	-----------------------------------------------------	*/
/*	   t e s t _ c o r d s _ p a r s e r _ c o m a n d 	*/
/*	-----------------------------------------------------	*/
private int	test_cords_parser_command( int	argc, char * argv[] )
{
	int	argi=1;
	char *	aptr;
	while (argi < argc)
	{
		if (!( aptr = argv[argi++] ))
			break;
		else if ( *aptr !=  '-')
			return( test_cords_parser_operation( aptr  ) );
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
				else if (!( strcmp( aptr, "xsd" ) ))
				{	Cp.xsd=0; continue;	}
				else if (!( strcmp( aptr, "tls" ) ))
				{
					Cp.tls = argv[argi++];
					continue;
				}
				else if (!( strcmp( aptr, "publisher" ) ))
				{
					Cp.publisher = argv[argi++];
					continue;
				}
				else if (!( strcmp( aptr, "mime" ) ))
				{
					Cp.accept = argv[argi++];
					continue;
				}
				else if (!( strcmp( aptr, "host" ) ))
				{
					Cp.host = argv[argi++];
					continue;
				}
				else if (!( strcmp( aptr, "agent" ) ))
				{
					Cp.agent = argv[argi++];
					continue;
				}
				else if (!( strcmp( aptr, "result" ) ))
				{
					Cp.result = argv[argi++];
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
					Cp.host = (aptr+2);
					continue;
				case	'a'	:
					Cp.agent = (aptr+2);
					continue;
				case	'r'	:
					Cp.result = (aptr+2);
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
/*	   t e s t _ c o r d s _ p a r s e r _ b a n n e r 	*/
/*	-----------------------------------------------------	*/
private	int	test_cords_parser_banner(char * n)
{
	printf("\n   Cords Parser : Version 1.0.b.0.01 ");
	printf("\n   Beta Version 19/03/2012 \n");
	printf("\n   Copyright (c) 2011, 2012 Iain James Marshall, Prologue ");
	printf("\n   Usage : \n");
	printf("\n   --xsd                dis-activate xsd validation ");
	printf("\n   --tls  <name>        specify the tls configuration  ");
	printf("\n   --mime <accept>      specify the OCCI mime type ");
	printf("\n   --publisher <host>   specify the publisher hostname ");
	printf("\n   --accept <type>      specify ACCEPT MIME type ");
	printf("\n   --agent <name>       specify the name of the agent ");
	printf("\n   --result <filename>  specify the output plan filename ");
	printf("\n   --verbose            activate verbose messages ");
	printf("\n   --debug              activate debug messages \n");
	printf("\n   Example : \n");
	printf("\n   %s --publisher http://127.0.0.1:8086 cords.xml \n\n",n);
	return(0);
}

/*	-----------------------------------------------------	*/
/*	     t e s t _ c o r d s _ p a r s e r _ m a i n	*/
/*	-----------------------------------------------------	*/
public	int	main(int argc, char * argv[])
{
	if ( argc == 1 )
		return( test_cords_parser_banner(argv[0]) );
	else	return( test_cords_parser_command( argc, argv ) );
}


#endif

