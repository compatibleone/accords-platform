/* ---------------------------------------------------------------------------- */
/* Advanced Capabilities for Compatible One Resources Delivery System - ACCORDS	*/
/* (C) 2011 by Iain James Marshall <ijm667@hotmail.com>				*/
/* ---------------------------------------------------------------------------- */
/*										*/
/* This is free software; you can redistribute it and/or modify it		*/
/* under the terms of the GNU Lesser General Public License as			*/
/* published by the Free Software Foundation; either version 2.1 of		*/
/* the License, or (at your option) any later version.				*/
/*										*/
/* This software is distributed in the hope that it will be useful,		*/
/* but WITHOUT ANY WARRANTY; without even the implied warranty of		*/
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU		*/
/* Lesser General Public License for more details.				*/
/*										*/
/* You should have received a copy of the GNU Lesser General Public		*/
/* License along with this software; if not, write to the Free			*/
/* Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA		*/
/* 02110-1301 USA, or see the FSF site: http://www.fsf.org.			*/
/*										*/
/* ---------------------------------------------------------------------------- */
#ifndef	_accords_c
#define	_accords_c

#include "standard.h"
#include "accords.h"
#include "document.h"
#include "rest.h"

private	struct	accords_configuration 	* configuration = (struct accords_configuration *) 0;
private	char				* section 	= (char *) 0;

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
		configuration->identity = allocate_string( argv[++argi] );
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
	printf("\n   __config     <filename>    specify configuration filename ");
	printf("\n   --tls        <filename>    specify tls configuration filename ");
	printf("\n   --resthost   <url>         specify rest server host url");
	printf("\n   --restport   <number>      specify rest server port");
	printf("\n   --chathost   <url>         specify chat server host url");
	printf("\n   --chatport   <number>      specify chat server port");
	printf("\n   --publisher  <url>         specify url of publisher");
	printf("\n   --operator   <name>        specify name of operator");
	printf("\n   --identity   <url>         specify url for publication ");
	printf("\n   --user       <name>        specify user log name   ");
	printf("\n   --password   <value>       specify user password   ");
	return;
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
		if ( configuration->identity )
		{
			configuration->identity = service_prefix_url( 
				configuration->identity,
				( security ? "https" : "http" )
				);
		}

		/* ---------------------------------------- */
		/* ensure service prefixed publisher string */
		/* ---------------------------------------- */
		if ( configuration->publisher )
		{
			configuration->publisher = service_prefix_url( 
				configuration->publisher,
				( security ? "https" : "http" )
				);
		}
	}
	else if ( configuration->verbose )
		printf("   Failed to load configuration section : %s, from : %s \n",section,configuration->config);
	set_xml_echo(0);
	return;
}


#endif	/* _accords_c */
	/* ---------- */





