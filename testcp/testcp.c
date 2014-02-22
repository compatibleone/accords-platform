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
#ifndef	_test_cords_parser_c
#define	_test_cords_parser_c

#include <libgen.h>

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
	char *	zone;
	char *	operator;
	int	xsd;
} Cp = 	{
	(char *) 0,
	_CORDS_DEFAULT_PUBLISHER,
	_CORDS_DEFAULT_PUBLISHER,
	_CORDS_PARSER_AGENT,
	(char *) 0,
	(char *) 0,
	"europe",
	"accords",
	1
	};

private	int	debug=0;
private	int	echo=0;
private	int	verbose=0;
private	int	convert=0;
public	int	check_debug()		{	return(debug);		}
public	int	check_verbose()		{	return(verbose);	}
public	char *	default_publisher()	{	return(Cp.publisher);	}
public	char *	default_tls()		{	return(Cp.tls);		}
public	char *	default_operator()	{	return(Cp.operator);	}
public	char *	default_zone()		{	return(Cp.zone);	}
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
	char *dirc, *basec, *bname, *dname;
	struct	xml_element * dptr;
	char	nameplan[512];
	if (!( Cp.publisher ))
		return( failure(1,"requires","publication host"));
	else if (!( Cp.agent ))
		return( failure(2,"requires","parser agent name"));
	else if (!( filename ))
		return( failure(3,"requires","cords filename"));
	else if (!( dptr = cords_document_parser( Cp.publisher, filename, Cp.agent, Cp.tls, Cp.xsd ) ))
		return( failure(4,"parse error",filename));
	else if (!( Cp.result ))
	{
		if (!( dirc = allocate_string( filename ) ))
			return( failure(5,"allocation",filename));
		else if (!( basec = allocate_string( filename ) ))
			return( failure(6,"allocation",filename));

		dname = dirname(dirc);
		bname = basename(basec);
		sprintf(nameplan,"%s/plan_%s",dname, bname);
		dirc = liberate( dirc );
		basec = liberate( basec );
		if (!( Cp.result = allocate_string( nameplan ) ))
			return( failure(4,"allocation","result filename"));
	}
	dptr = cords_serialise_document( dptr, Cp.result );
	return( cords_check_parser_errors( Cp.result ) );
}

/*	-----------------------------------------------------	*/
/*	t e s t _ c o r d s _ p a r s e r _ m a n  f e s t	*/
/*	-----------------------------------------------------	*/
private	struct xml_element * test_cp_new_manifest( char * newname )
{
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;
	if (!( eptr = allocate_element() ))
		return( eptr );
	else if (!( eptr->name = allocate_string( "manifest" ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "name", newname ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "xmlns", "http://www.compatibleone.fr/schemes/manifest.xsd" ) ))
		return( liberate_element( eptr ) );
	else	return( eptr );
}

/*	-----------------------------------------------------	*/
/*	t e s t _ c o r d s _ p a r s e r _ n o d e       	*/
/*	-----------------------------------------------------	*/
private	struct xml_element * test_cp_add_node( struct xml_element * manifest, struct xml_element * services, struct xml_element * part )
{
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;
	char *	nodename;
	if (!( aptr = document_atribut( part, "composable" ) ))
		return((struct xml_element *) 0);
	else if (!( nodename = allocate_string( aptr->value ) ))
		return((struct xml_element *) 0);
	else if (!( eptr = allocate_element() ))
		return( eptr );
	else if (!( eptr->name = allocate_string( "node" ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "name", nodename ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "access", "public" ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "scope", "normal" ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "type", "simple" ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "category", "manifest" ) ))
		return( liberate_element( eptr ) );
	else
	{
		if (!( eptr->previous = manifest->last ))
			manifest->first = eptr;
		else	eptr->previous->next = eptr;
		manifest->last = eptr;
		return( eptr );
	}
}

/*	-----------------------------------------------------	*/
/*	t e s t _ c o r d s _ p a r s e r _ c o n v e r t o r 	*/
/*	-----------------------------------------------------	*/
private	int	test_cords_parser_convertor( char * filename )
{
	char *	newname="example";
	struct	xml_element * document;
	struct	xml_element * eptr;
	struct	xml_element * sptr;
	struct	xml_element * mptr;
	struct	xml_element * node=(struct xml_element *) 0;
	struct	xml_element * pptr;
	struct	xml_element * manifest=(struct xml_element *) 0;
	struct	xml_element * usdl;
	if (!( document = document_parse_file( filename ) ))
		return( 404 );
	else if (!( eptr = document_element( document, "ws:AgreementProperties" ) ))
	{
		document = document_drop( document );
		return( 405 );
	} 
	else if (!( eptr = document_element( eptr, "ws:Terms" ) ))
	{
		document = document_drop( document );
		return( 405 );
	} 
	else if (!( eptr = document_element( eptr, "ws:All" ) ))
	{
		document = document_drop( document );
		return( 405 );
	} 
	else if (!( eptr = document_element( eptr, "ws:ServiceDescriptionTerm" ) ))
	{
		document = document_drop( document );
		return( 405 );
	} 
	/* ----------------------------- */
	/* locate start of USDL document */
	/* ----------------------------- */
	else if (!( usdl = document_element( eptr, "ns2:USDL3Document" ) ))
	{
		document = document_drop( document );
		return( 405 );
	} 
	/* ---------------------------------- */
	/* locate service description section */
	/* ---------------------------------- */
	else if (!( sptr = document_element( usdl, "services" ) ))
	{
		document = document_drop( document );
		return( 405 );
	} 
	/* ---------------------------------- */
	/* locate service composition section */
	/* ---------------------------------- */
	else if (!( mptr = document_element( usdl, "compositeServices" ) ))
	{
		document = document_drop( document );
		return( 405 );
	} 
	else if (!( manifest = test_cp_new_manifest(newname) ))
	{
		document = document_drop( document );
		return( 405 );
	} 
	else
	{
		/* --------------------------------- */
		/* process service composition parts */
		/* --------------------------------- */
		for (	pptr = document_element( mptr, "parts" );
			pptr != (struct xml_element *) 0;
			pptr = pptr->next )
		{
			if (!( node = test_cp_add_node( manifest, sptr, pptr ) ))
				break;
		}
		document = document_drop( document );
		manifest = cords_serialise_document( manifest, newname );
		manifest = document_drop( manifest );
		return( 0 );
	}
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

	set_xml_echo(echo);

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
		{
			if (!( convert ))
				return( test_cords_parser_operation( aptr  ) );
			else	return( test_cords_parser_convertor( aptr  ) );
		}
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
				else if (!( strcmp( aptr, "convert" ) ))
				{	convert=1; continue;	}
				else if (!( strcmp( aptr, "echo" ) ))
				{	echo=1; continue;	}
				else if (!( strcmp( aptr, "xsd" ) ))
				{	Cp.xsd=0; continue;	}
				else if (!( strcmp( aptr, "force" ) ))
				{	set_force(1); continue;	}
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
				else if (!( strcmp( aptr, "account" ) ))
				{
					set_default_account( argv[argi++] );
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
	printf("\n   Cords Parser : Version 1.0.c.0.02 ");
	printf("\n   Beta Version 10/02/2014 \n");
	printf("\n   Copyright (c) 2011, 2014 Iain James Marshall, Prologue ");
	printf("\n   Usage : \n");
	printf("\n   --xsd                dis-activate xsd validation ");
	printf("\n   --tls  <name>        specify the tls configuration  ");
	printf("\n   --mime <accept>      specify the OCCI mime type ");
	printf("\n   --publisher <host>   specify the publisher hostname ");
	printf("\n   --accept <type>      specify ACCEPT MIME type ");
	printf("\n   --agent <name>       specify the name of the agent ");
	printf("\n   --account <name>     specify payment account name ");
	printf("\n   --result <filename>  specify the output plan filename ");
	printf("\n   --convert            convert document format   ");
	printf("\n   --verbose            activate verbose messages ");
	printf("\n   --debug              activate debug messages ");
	printf("\n   --force              force update of category \n");
	
	printf("\n   Example : \n");
	printf("\n   %s --publisher http://127.0.0.1:8086 cords.xml \n\n",n);
	return(0);
}

/*	-----------------------------------	*/
/*		o p e r a t i o n		*/
/*	-----------------------------------	*/
private	int	operation( int argc, char * argv[] )
{
	int	status;
	status = test_cords_parser_command( argc, argv );
	occi_flush_clients();
	return( status );
}

/*	-----------------------------------------------------	*/
/*	     t e s t _ c o r d s _ p a r s e r _ m a i n	*/
/*	-----------------------------------------------------	*/
public	int	main(int argc, char * argv[])
{
	if ( argc == 1 )
		return( test_cords_parser_banner(argv[0]) );
	else	return( operation( argc, argv ) );
}


#endif

