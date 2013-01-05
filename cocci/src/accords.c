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
#ifndef	_accords_c
#define	_accords_c

#include "standard.h"
#include "accords.h"
#include "document.h"
#include "rest.h"

private	struct	accords_configuration 	* configuration 	= (struct accords_configuration *) 0;
private	char				* section 		= (char *) 0;
private	char				* identity		= (char *) 0;
private	char				* default_account	= (char *) 0;
private	char				* default_account_id	= (char *) 0;
private	char				* default_agent		= (char *) 0;

/*	---------------------------------------------------------	*/
/*			g e t _ i d e n t i t y				*/
/*	---------------------------------------------------------	*/
public	char *	get_identity()
{
	if (!( configuration ))
		return((char *) 0);
	else 	return( configuration->identity ); 
}
	
/*	---------------------------------------------------------	*/
/*		g e t _ d e f a u l t _ a g e n t			*/
/*	---------------------------------------------------------	*/
public	char *	get_default_agent()
{
	return( default_agent );
}

/*	---------------------------------------------------------	*/
/*		s e t _ d e f a u l t _ a g e n t			*/
/*	---------------------------------------------------------	*/
public	int	set_default_agent(char * sptr)
{
	if ( default_agent )
		default_agent = liberate( default_agent );
	if (!( sptr ))
		return( 0 );
	else if (!( default_agent = allocate_string( sptr ) ))
		return(27);
	else	return(0);	
}
	
/*	---------------------------------------------------------	*/
/*		s e t _ d e f a u l t _ a c c o u n t			*/
/*	---------------------------------------------------------	*/
public	int	set_default_account(char * sptr)
{
	if (!( default_account = allocate_string( sptr ) ))
		return( 27 );
	else	return( 0 );
}

/*	---------------------------------------------------------	*/
/*		g e t _ d e f a u l t _ a c c o u n t			*/
/*	---------------------------------------------------------	*/
public	char *	get_default_account()
{
	if (!( default_account ))
		if ( set_default_account( "accords" ) != 0 )
			return( (char *) 0 );
	return( default_account );
}

/*	---------------------------------------------------------	*/
/*		s e t _ d e f a u l t _ a c c o u n t _ i d 		*/
/*	---------------------------------------------------------	*/
public	int	set_default_account_id(char * sptr)
{
	if (!( default_account_id = allocate_string( sptr ) ))
		return( 27 );
	else	return( 0 );
}

/*	---------------------------------------------------------	*/
/*		g e t _ d e f a u l t _ a c c o u n t _ i d 		*/
/*	---------------------------------------------------------	*/
public	char *	get_default_account_id()
{
	return( default_account_id );
}

/*	---------------------------------------------------------	*/
/*	 a c c o r d s _ c o n f i g u r a t i o n _ o p t i o n 	*/
/*	---------------------------------------------------------	*/
/*	analyse of an accords configuration option name and value	*/
/*	---------------------------------------------------------	*/
public	int	accords_configuration_option( char * aptr, int argi, char * argv[] )
{
	if (!( configuration ))
		return( 0 );
	else if (!( section ))
		return( 0 );

	else if (!( strcmp( aptr, "auto" ) ))
		configuration->autopub = 1;
	else if (!( strcmp( aptr, "verbose" ) ))
		configuration->verbose = 1;
	else if (!( strcmp( aptr, "debug" ) ))
		configuration->debug = 1;
	else if (!( strcmp( aptr, "ipv6" ) ))
		configuration->ipv6 = 1;
	else if (!( strcmp( aptr, "ipv4" ) ))
		configuration->ipv6 = 0;
	else if (!( strcmp( aptr, "config" ) ))
	{
		configuration->config = allocate_string( argv[++argi] );
		load_accords_configuration( configuration, section );
	}
	else if (!( strcmp( aptr, "tls" ) ))
		configuration->tls = allocate_string( argv[++argi] );
	else if (!( strcmp( aptr, "domain" ) ))
		configuration->domain = allocate_string( argv[++argi] );
	else if (!( strcmp( aptr, "publisher" ) ))
		configuration->publisher = allocate_string( argv[++argi] );
	else if (!( strcmp( aptr, "identity" ) ))
		identity = configuration->identity = allocate_string( argv[++argi] );
	else if (!( strcmp( aptr, "zone" ) ))
		configuration->zone = allocate_string( argv[++argi] );
	else if (!( strcmp( aptr, "storage" ) ))
		configuration->storage = allocate_string( argv[++argi] );
	else if (!( strcmp( aptr, "user" ) ))
		configuration->user = allocate_string( argv[++argi] );
	else if (!( strcmp( aptr, "password" ) ))
		configuration->password = allocate_string( argv[++argi] );
	else if (!( strcmp( aptr, "resthost" ) ))
		configuration->resthost = allocate_string( argv[++argi] );
	else if (!( strcmp( aptr, "restport" ) ))
		configuration->restport = atoi( argv[++argi] );
	else if (!( strcmp( aptr, "chathost" ) ))
		configuration->chathost = allocate_string( argv[++argi] );
	else if (!( strcmp( aptr, "chatport" ) ))
		configuration->chatport = atoi( argv[++argi] );
	else if (!( strcmp( aptr, "threads" ) ))
		configuration->threads = _REST_THREAD_WORKER;
	else	return( 0 );
	return( argi );
}

/*	---------------------------------------------------------	*/
/*	a c c o r d s _ c o n f i g u r a t i o n _ o p t i o n s	*/
/*	---------------------------------------------------------	*/
/*	displays the available options for construction of banner	*/
/*	---------------------------------------------------------	*/
public	void	accords_configuration_options()
{
	printf("\nOptions: ");
	printf("\n   --debug                    activates debug messages");
	printf("\n   --verbose                  activates info  messages");
	printf("\n   --threads                  activate thread handlers");
	printf("\n   --config     <filename>    specify configuration filename ");
	printf("\n   --tls        <filename>    specify tls configuration filename ");
	printf("\n   --ipv4                     activate use of IP V4 ( default ) ");
	printf("\n   --ipv6                     activate use of IP V6 ( default ) ");
	printf("\n   --resthost   <url>         specify rest server host url");
	printf("\n   --restport   <number>      specify rest server port");
	printf("\n   --chathost   <url>         specify chat server host url");
	printf("\n   --chatport   <number>      specify chat server port");
	printf("\n   --publisher  <url>         specify url of publisher");
	printf("\n   --operator   <name>        specify name of operator");
	printf("\n   --identity   <url>         specify url for publication ");
	printf("\n   --zone       <name>        specify location zone   ");
	printf("\n   --storage    <name>        specify data storage name");
	printf("\n   --user       <name>        specify user log name   ");
	printf("\n   --password   <value>       specify user password   ");
	return;
}

private	int	is_valid_value( char * sptr )
{
	if (!( sptr ))
		return( 0 );
	else if (!( strlen( sptr ) ))
		return(0);
	else if (!( strcmp( sptr, "(null)" ) ))
		return(0);
	else	return(1);
}

/*	----------------------------------------------------	*/
/*	l o a d _ a c c o r d s _ c o n f i g u r a t i o n	*/
/*	----------------------------------------------------	*/
/*	this function provides configuration loading for all	*/
/*	modules of the accords platform except the publisher	*/
/*	----------------------------------------------------	*/

public	void	load_accords_configuration( struct accords_configuration * cptr, char * sptr )
{
	int	security=0;
	struct	xml_element * document;
	struct	xml_element * eptr;
	struct	xml_element * vptr;
	struct	xml_atribut * aptr;

	configuration = cptr;
	section	      = sptr;

	set_xml_echo(configuration->debug);

	if (( document = document_parse_file( configuration->config )) != (struct xml_element *) 0)
	{
		if ( configuration->verbose )
			printf("   Loading configuration section : %s, from : %s \n",section,configuration->config);

		if (( eptr = document_element( document, section )) != (struct xml_element *) 0)
		{
			if ((aptr = document_atribut( eptr, "identity" )) != (struct xml_atribut *) 0)
				configuration->identity = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "publisher")) != (struct xml_atribut *) 0)
				configuration->publisher = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "operator")) != (struct xml_atribut *) 0)
				configuration->operator = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "verbose")) != (struct xml_atribut *) 0)
				configuration->verbose = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "ipv6")) != (struct xml_atribut *) 0)
				configuration->ipv6 = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "debug")) != (struct xml_atribut *) 0)
				configuration->debug = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "threads")) != (struct xml_atribut *) 0)
				configuration->threads = document_atribut_value( aptr );

			if (( vptr = document_element( eptr, "rest" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "host" )) != (struct xml_atribut *) 0)
					configuration->resthost = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "port" )) != (struct xml_atribut *) 0)
					configuration->restport = document_atribut_value( aptr );
				if ((aptr = document_atribut( vptr, "storage" )) != (struct xml_atribut *) 0)
					configuration->storage = document_atribut_string( aptr );
			}

			if (( vptr = document_element( eptr, "security" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "user" )) != (struct xml_atribut *) 0)
					configuration->user = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "password"))  != (struct xml_atribut *) 0)
					configuration->password = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "tls" )) != (struct xml_atribut *) 0)
					if ((configuration->tls = document_atribut_string( aptr )) != 0)
						if (!( strlen(configuration->tls) ))
							configuration->tls = (char *) 0;
				if ((aptr = document_atribut( vptr, "monitor")) != (struct xml_atribut *) 0)
					configuration->monitor = document_atribut_value( aptr );
			}

			if (( vptr = document_element( eptr, "domain" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "name" )) != (struct xml_atribut *) 0)
					configuration->domain = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "zone" )) != (struct xml_atribut *) 0)
					configuration->zone = document_atribut_string( aptr );
			}
		}
		document = document_drop( document );

		/* ---------------------------------------- */
		/* detect presence and validity of security */
		/* ---------------------------------------- */
		if (!( configuration->tls ))
			security = 0;
		else if (!( strlen( configuration->tls ) ))
			security = 0;
		else	security = 1;

		/* --------------------------------------- */
		/* ensure service prefixed identity string */
		/* --------------------------------------- */
		if ( is_valid_value( configuration->identity ) )
		{
			configuration->identity = service_prefix_url( 
				configuration->identity,
				( security ? "https" : "http" )
				);
		}

		/* ---------------------------------------- */
		/* ensure service prefixed publisher string */
		/* ---------------------------------------- */
		if ( is_valid_value( configuration->publisher ) )
		{
			configuration->publisher = service_prefix_url( 
				configuration->publisher,
				( security ? "https" : "http" )
				);
		}
		if ( configuration->ipv6 )
			set_socket_ipv6();
		else	set_socket_ipv4();

	}
	else if ( configuration->verbose )
		printf("   Failed to load configuration section : %s, from : %s \n",section,configuration->config);
	set_xml_echo(0);
	return;
}


#endif	/* _accords_c */
	/* ---------- */





