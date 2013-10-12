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

private	struct	accords_configuration 	* configuration 		= (struct accords_configuration *) 0;
private	char				* section 			= (char *) 0;
private	char				* identity			= (char *) 0;
private	char				* component			= (char *) 0;
private	char				* default_account		= (char *) 0;
private	char				* default_account_id		= (char *) 0;
private	char				* default_agent			= (char *) 0;
private	char				* default_operator_profile	= (char *) 0;
private	int				  alert_status=600;


/*	---------------------------------------------------------	*/
/*	  	    i s _ a l e r t _ s t a t u s 			*/
/*	---------------------------------------------------------	*/
public	int	is_alert_status( int status )
{
	if ( status >= alert_status )
		return( 1 );
	else	return( 0 );
}

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
/*			g e t _ c o m p o n e n t			*/
/*	---------------------------------------------------------	*/
public	char *	get_component()
{
	if (!( configuration ))
		return((char *) 0);
	else 	return( configuration->component ); 
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
/*		s e t _ o p e r a t o r _ p r o f i l e 		*/
/*	---------------------------------------------------------	*/
public	void	set_operator_profile(char * sptr)
{
	if ( default_operator_profile )
		default_operator_profile = liberate( default_operator_profile );
	if ( sptr )
		default_operator_profile = allocate_string( sptr );
	return;
}

/*	---------------------------------------------------------	*/
/*		g e t _ o p e r a t o r _ p r o f i l e 		*/
/*	---------------------------------------------------------	*/
public	char * get_operator_profile()
{
	if (!( default_operator_profile ))
		set_operator_profile( "accords" );
	return( default_operator_profile );
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
	else if (!( strcmp( aptr, "component" ) ))
		component = configuration->component = allocate_string( argv[++argi] );
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
	printf("\n   --component  <name>        specify name of component");
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

	if (!( configuration ))
		configuration = cptr;
	if (!( section ))
		section	      = sptr;

	set_xml_echo(cptr->debug);

	if (( document = document_parse_file( cptr->config )) != (struct xml_element *) 0)
	{
		if ( check_verbose() )
			printf("   Loading configuration section : %s, from : %s \n",sptr,cptr->config);

		if (( eptr = document_element( document, sptr )) != (struct xml_element *) 0)
		{
			if ((aptr = document_atribut( eptr, "identity" )) != (struct xml_atribut *) 0)
				cptr->identity = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "component" )) != (struct xml_atribut *) 0)
				cptr->component = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "publisher")) != (struct xml_atribut *) 0)
				cptr->publisher = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "operator")) != (struct xml_atribut *) 0)
				cptr->operator = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "verbose")) != (struct xml_atribut *) 0)
				cptr->verbose = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "ipv6")) != (struct xml_atribut *) 0)
				cptr->ipv6 = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "debug")) != (struct xml_atribut *) 0)
				cptr->debug = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "threads")) != (struct xml_atribut *) 0)
				cptr->threads = document_atribut_value( aptr );

			if (( vptr = document_element( eptr, "rest" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "host" )) != (struct xml_atribut *) 0)
					cptr->resthost = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "port" )) != (struct xml_atribut *) 0)
					cptr->restport = document_atribut_value( aptr );

				if ((aptr = document_atribut( vptr, "alert" )) != (struct xml_atribut *) 0)
					if (!( alert_status = document_atribut_value( aptr ) ))
						alert_status = 600;

				if ((aptr = document_atribut( vptr, "storage" )) != (struct xml_atribut *) 0)
					cptr->storage = document_atribut_string( aptr );
			}

			if (( vptr = document_element( eptr, "security" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "user" )) != (struct xml_atribut *) 0)
					cptr->user = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "password"))  != (struct xml_atribut *) 0)
					cptr->password = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "tls" )) != (struct xml_atribut *) 0)
					if ((cptr->tls = document_atribut_string( aptr )) != 0)
						if (!( strlen(cptr->tls) ))
							cptr->tls = (char *) 0;
				if ((aptr = document_atribut( vptr, "monitor")) != (struct xml_atribut *) 0)
					cptr->monitor = document_atribut_value( aptr );
			}

			if (( vptr = document_element( eptr, "domain" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "name" )) != (struct xml_atribut *) 0)
					cptr->domain = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "zone" )) != (struct xml_atribut *) 0)
					cptr->zone = document_atribut_string( aptr );
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
		if ( is_valid_value( cptr->identity ) )
		{
			cptr->identity = service_prefix_url( 
				cptr->identity,
				( security ? "https" : "http" )
				);
		}

		/* ---------------------------------------- */
		/* ensure service prefixed publisher string */
		/* ---------------------------------------- */
		if ( is_valid_value( cptr->publisher ) )
		{
			cptr->publisher = service_prefix_url( 
				cptr->publisher,
				( security ? "https" : "http" )
				);
		}
		if ( configuration->ipv6 )
			set_socket_ipv6();
		else	set_socket_ipv4();

	}
	else if ( check_verbose() )
		printf("   Failed to load configuration section : %s, from : %s \n",sptr,cptr->config);
	set_xml_echo(0);
	return;
}

/*	----------------------------------------------------	*/
/*	l o a d _ a c c o r d s _ c o n f i g u r a t i o n	*/
/*	----------------------------------------------------	*/
/*	this function provides configuration loading for all	*/
/*	modules of the accords platform except the publisher	*/
/*	----------------------------------------------------	*/

public	int	save_accords_configuration( struct accords_configuration * cptr, char * sptr )
{
	int	security=0;
	struct	xml_element * document;
	struct	xml_element * eptr;
	struct	xml_element * vptr;
	struct	xml_atribut * aptr;
	FILE *	h;
	if (!( h = fopen( sptr, "w" ) ))
		return( 46 );

	else
	{
		fprintf(h,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
		fprintf(h,"<%s\n",cptr->component);
			fprintf(h,"\tpublisher=\"%s\"\n",cptr->publisher);
			fprintf(h,"\tidentity=\"%s\"\n",cptr->identity);
			fprintf(h,"\toperator=\"%s\"\n",cptr->operator);
			fprintf(h,"\tcomponent=\"%s\"\n",cptr->component);
			fprintf(h,"\tverbose=\"%u\"\n",cptr->verbose);
			fprintf(h,"\tipv6=\"%u\"\n",cptr->ipv6);
			fprintf(h,"\tdebug=\"%u\"\n",cptr->debug);
			fprintf(h,"\tthreads=\"%u\">\n",cptr->threads);
			fprintf(h,"<rest host=\"%s\" port=\"%u\" storage=\"%s\"/>\n",
				cptr->resthost,cptr->restport,cptr->storage);
			fprintf(h,"<security user=\"%s\" password=\"%s\" tls=\"%s\" monitor=\"%u\"/>\n",
				cptr->user,cptr->password,cptr->tls,cptr->monitor);
			fprintf(h,"<domain name=\"%s\" zone=\"%s\"/>\n",
				cptr->domain,cptr->zone);
		fprintf(h,"</%s>\n",cptr->component);
		fclose(h);
		return(0);
	}
}

/*	-----------------------------------------------------------------	*/
/*		s e r i a l i s e _ s t r i n g _ v a l u e 			*/
/*	-----------------------------------------------------------------	*/
private	char *	serialise_string_value( char * result, char * nptr, char * vptr )
{
	char	buffer[2048];
	if ( result )
	{
		sprintf(buffer,"%s %s=%c%s%c",result,nptr,0x0022,(vptr ? vptr : ""),0x0022);
		liberate( result );
	}
	else	sprintf(buffer,"%s=%c%s%c",nptr,0x0022,(vptr ? vptr : ""),0x0022);
	return( allocate_string( buffer ) );
}

/*	-----------------------------------------------------------------	*/
/*		s e r i a l i s e _ i n t e g e r _ v a l u e 			*/
/*	-----------------------------------------------------------------	*/
private	char *	serialise_integer_value( char * result, char * nptr, int vptr )
{
	char	buffer[2048];
	if ( result )
	{
		sprintf(buffer,"%s %s=%c%u%c",result,nptr,0x0022,vptr,0x0022);
		liberate( result );
	}
	else	sprintf(buffer,"%s=%c%u%c",nptr,0x0022,vptr,0x0022);
	return( allocate_string( buffer ) );
}

/*	-----------------------------------------------------------------	*/
/*	s e r i a l i s e _ c o m p o n e n t _ c o n f i g u r a t i o n	*/
/*	-----------------------------------------------------------------	*/
public	char *	serialise_component_configuration(struct accords_configuration * cptr)
{
	char *	result=(char *) 0;
	if (!( cptr ))
		if (!( cptr = configuration ))
			return( result );
	if (!( result = serialise_string_value( result, "name", cptr->component )))
		return( result );
	else if (!( result = serialise_string_value( result, "identity", cptr->identity )))
		return( result );
	else if (!( result = serialise_string_value( result, "publisher", cptr->publisher )))
		return( result );
	else if (!( result = serialise_string_value( result, "operator", cptr->operator )))
		return( result );
	else if (!( result = serialise_string_value( result, "storage", cptr->storage )))
		return( result );
	else if (!( result = serialise_string_value( result, "domain", cptr->domain )))
		return( result );
	else if (!( result = serialise_string_value( result, "zone", cptr->zone )))
		return( result );
	else if (!( result = serialise_string_value( result, "tls", cptr->tls )))
		return( result );
	else if (!( result = serialise_string_value( result, "user", cptr->user )))
		return( result );
	else if (!( result = serialise_string_value( result, "password", cptr->password )))
		return( result );
	else if (!( result = serialise_string_value( result, "host", cptr->resthost )))
		return( result );
	else if (!( result = serialise_integer_value( result, "port", cptr->restport )))
		return( result );
	else if (!( result = serialise_integer_value( result, "verbose", cptr->verbose )))
		return( result );
	else if (!( result = serialise_integer_value( result, "debug", cptr->debug )))
		return( result );
	else if (!( result = serialise_integer_value( result, "ipv6", cptr->ipv6 )))
		return( result );
	else if (!( result = serialise_integer_value( result, "threads", cptr->threads )))
		return( result );
	else if (!( result = serialise_integer_value( result, "monitor", cptr->monitor )))
		return( result );
	else	return( result );
}

/*	---------------------------------------------------------------------	*/
/*	d e s e r i a l i s e _ c o m p o n e n t _ c o n f i g u r a t i o n	*/
/*	---------------------------------------------------------------------	*/
public	void	deserialise_component_configuration( 
		struct accords_configuration * configuration,
		struct xml_element * eptr )
{
	struct	xml_atribut * aptr;
	if ((aptr = document_atribut( eptr, "identity" )) != (struct xml_atribut *) 0)
		configuration->identity = document_atribut_string( aptr );
	if ((aptr = document_atribut( eptr, "name" )) != (struct xml_atribut *) 0)
		configuration->component = document_atribut_string( aptr );
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

	if ((aptr = document_atribut( eptr, "host" )) != (struct xml_atribut *) 0)
		configuration->resthost = document_atribut_string( aptr );
	if ((aptr = document_atribut( eptr, "port" )) != (struct xml_atribut *) 0)
		configuration->restport = document_atribut_value( aptr );

	if ((aptr = document_atribut( eptr, "storage" )) != (struct xml_atribut *) 0)
		configuration->storage = document_atribut_string( aptr );

	if ((aptr = document_atribut( eptr, "user" )) != (struct xml_atribut *) 0)
		configuration->user = document_atribut_string( aptr );
	if ((aptr = document_atribut( eptr, "password"))  != (struct xml_atribut *) 0)
		configuration->password = document_atribut_string( aptr );
	if ((aptr = document_atribut( eptr, "tls" )) != (struct xml_atribut *) 0)
		configuration->tls = document_atribut_string( aptr );
	if ((aptr = document_atribut( eptr, "monitor")) != (struct xml_atribut *) 0)
		configuration->monitor = document_atribut_value( aptr );
	if ((aptr = document_atribut( eptr, "domain" )) != (struct xml_atribut *) 0)
		configuration->domain = document_atribut_string( aptr );
	if ((aptr = document_atribut( eptr, "zone" )) != (struct xml_atribut *) 0)
		configuration->zone = document_atribut_string( aptr );
	return;
}

#endif	/* _accords_c */
	/* ---------- */





