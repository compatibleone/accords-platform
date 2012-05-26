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
#ifndef	_command_c
#define	_command_c

#include "command.h"
#include "cordslang.h"
#include "occiresolver.h"
#include "occilogin.h"
#include "cp.h"
#include "json.h"

private	int	verbose=0;
private	int	debug=0;

private	char *	agent="CO-COMMAND/1.0";
private	char *	publisher=(char *) 0;
private	char *	operator="accords";
private	char *	zone="europe";
private	char *	tls=(char *) 0;

public	char *	default_operator()	{	return( operator );	}
public	char *	default_zone()		{	return( zone );		}
public	char *	default_publisher()	{	return( publisher );	}
public	char *	default_tls()		{	return( tls );		}
public	int	check_verbose()		{	return( verbose );	}
public	int	check_debug()		{	return( debug );	}

/*	-----------------------------------	*/
/*		f a i l u r e			*/
/*	-----------------------------------	*/
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

/*	-----------------------------------------------------	*/
/*	   l l _ c o r d s _ s e r v i c e _ a c t i o n	*/
/*	-----------------------------------------------------	*/
private	int	ll_cords_service_action( char * id, char * action )
{
	struct	occi_response * zptr;
	char 	buffer[2048];
	char *	result;
	char *	sptr;

	initialise_occi_resolver( publisher, (char *) 0, (char *) 0, (char *) 0 );

	if (!( sptr = occi_resolve_category_provider( _CORDS_SERVICE, agent, tls ) ))
		return( 500 );
	else	sprintf(buffer,"%s/%s/%s",sptr,_CORDS_SERVICE,id );

	if (!( zptr =  cords_invoke_action( buffer, action, agent, tls ) ))
		return(501);
	else
	{
		zptr = occi_remove_response( zptr );
		return( 0 );
	}
}

/*	-----------------------------------------------------	*/
/*		c o r d s _ s e r v i c e _ a c t i o n		*/
/*	-----------------------------------------------------	*/
private	int	cords_service_action( char * id, char * action )
{
	int	status;
	char *	auth;

	initialise_occi_resolver( _DEFAULT_PUBLISHER, (char *) 0, (char *) 0, (char *) 0 );

	if (!( auth = login_occi_user( "test-broker","co-system",agent, tls ) ))
		return(403);
	else 	(void) occi_client_authentication( auth );

	status = ll_cords_service_action( id, action );

	(void) logout_occi_user( "test-broker","co-system",agent, auth, tls );	

	return( status );
}

/*	-----------------------------------------------------	*/
/*	   l l _ c o r d s _ s e r v i c e _ d e l e t e	*/
/*	-----------------------------------------------------	*/
private	int	ll_cords_service_delete( char * id )
{
	struct	occi_response * zptr;
	char 	buffer[2048];
	char *	result;
	char *	sptr;

	initialise_occi_resolver( publisher, (char *) 0, (char *) 0, (char *) 0 );

	if (!( sptr = occi_resolve_category_provider( _CORDS_SERVICE, agent, tls ) ))
		return( 500 );
	else	sprintf(buffer,"%s/%s/%s",sptr,_CORDS_SERVICE,id );

	if (!( zptr =  occi_simple_delete( buffer, agent, tls ) ))
		return(501);
	else
	{
		zptr = occi_remove_response( zptr );
		return( 0 );
	}
}

/*	-----------------------------------------------------	*/
/*		c o r d s _ s e r v i c e _ d e l e t e		*/
/*	-----------------------------------------------------	*/
private	int	cords_service_delete( char * id )
{
	int	status;
	char *	auth;

	initialise_occi_resolver( _DEFAULT_PUBLISHER, (char *) 0, (char *) 0, (char *) 0 );

	if (!( auth = login_occi_user( "test-broker","co-system",agent, tls ) ))
		return(403);
	else 	(void) occi_client_authentication( auth );

	status = ll_cords_service_delete( id );

	(void) logout_occi_user( "test-broker","co-system",agent, auth, tls );	

	return( status );
}

private	int	cosacs_command_tool( char * host, char * command ,char * category, char * item )
{
	struct	occi_response * rptr;
	char buffer[2048];
	if ( verbose )
	{
		printf("\nCosacs { Host=%s,",host);
		printf(" Command=%s",command);
		printf(" Category=%s",category);
		printf(" Item=%s } \n ",item);
	}
	sprintf(buffer,"http://%s:%u/%s/%s",host,_COSACS_PORT,category,item);

	if ( verbose )
		printf("   %s %s\n",command,buffer);

	if (!( strcasecmp( command, "LIST" ) ))
	{
		if (!( rptr = occi_simple_get( buffer, agent, tls ) ))
			return( failure(  400,"OCCI LIST", "request failed" ) );
		else
		{
			occi_show_response( rptr );
			rptr = occi_remove_response( rptr );
			return(0);
		}
	}
	else if (!( strcasecmp( command, "GET" ) ))
	{
		if (!( rptr = occi_simple_get( buffer, agent, tls ) ))
			return( failure(  400,"OCCI client GET", "request failed" ) );
		else
		{
			occi_show_response( rptr );
			rptr = occi_remove_response( rptr );
			return(0);
		}
	}
	else if (!( strcasecmp( command, "DELETE" ) ))
		return(0);
	else if (!( strcasecmp( command, "POST" ) ))
		return( 0 );
	else	return(0);
}

/*	-----------------------------------------------------	*/
/*	   c o s a c s _ s e r v i c e _ o p e r a t i o n	*/
/*	-----------------------------------------------------	*/
private	int	cosacs_service_operation( char * service, char * syntax )
{
	struct	data_element * dptr;
	struct	data_element * eptr;
	char * vptr;
	char * hostname;
	char * command=syntax;
	char * category;
	if (!( service )) 
		return( failure(  30,"expected service filename", "" ) );
	else if (!( syntax ))
		return( failure(  30,"expected syntax", "" ) );

	else if ( verbose )
		printf("\n Cosacs Service %s { %s } \n",service, syntax );
	if (!( dptr = json_parse_file( service )))
		return( failure(  40,"parsing json", service ) );
	else if (!( eptr = json_element( dptr, "attributs" ) ))
	{
		dptr = drop_data_element( dptr );
		return( failure(  118, "missing attributes in", service ) );
	}
	else if (!( vptr = json_atribut( eptr, "occi.contract.hostname" ) ))
	{
		dptr = drop_data_element( dptr );
		return( failure(  118, "missing or null hostname in", service ) );
	}
	else if (!( hostname = allocate_string( vptr ) ))
	{
		dptr = drop_data_element( dptr );
		return( failure(  118, "allocation failure in", service ) );
	}
	else
	{
		dptr = drop_data_element( dptr );
		while ( (*syntax) && ( *syntax != ' '))
			syntax++;

		if ( *syntax == ' ' )
			*(syntax++) = 0; 

		while ( *syntax == ' ' )
			syntax++;

		category = syntax;

		while ( (*syntax) && ( *syntax != ' '))
			syntax++;

		if ( *syntax == ' ' )
			*(syntax++) = 0; 

		while ( *syntax == ' ' )
			syntax++;

		return( cosacs_command_tool( hostname, command, category, syntax ) );
	}
}

/*	-----------------------------------------------------	*/
/*		  s e r v i c e _ o p e r a t i o n		*/
/*	-----------------------------------------------------	*/
private	int	service_operation( char * command, char * service, char * syntax )
{
	char *	id;
	char *	filename=service;
	FILE *	h;
	if (!( command ))
		return( 31 );
	else if (!( service ))
		return( 32 );
	else if (!( h = fopen( service, "r" ) ))
		return( 40 );
	else if (!( publisher ))
		return( failure( 0, "publisher", "undefined" ) );
	{
		fclose(h);
		for (	id=service;
			*service != 0;
			service++ )
		{
			if ( *service == '/' )
				id = (service+1);
		}

		if ( verbose )
			printf("\n Service %s { %s } \n",command,id);

		if (!( strcasecmp( command, "START" ) ))
			return( cords_service_action( id, "start" ) );
		else if (!( strcasecmp( command, "STOP" ) ))
			return( cords_service_action( id, "stop" ) );
		else if (!( strcasecmp( command, "SAVE" ) ))
			return( cords_service_action( id, "save" ) );
		else if (!( strcasecmp( command, "SNAPSHOT" ) ))
			return( cords_service_action( id, "snapshot" ) );
		else if (!( strcasecmp( command, "DELETE" ) ))
			return( cords_service_delete( id ) );
		else if (!( strcasecmp( command, "COSACS" ) ))
			return( cosacs_service_operation( filename, syntax ) );
		else	return( failure( 30,"incorrect command", command ) );
	}	
}

/*	-----------------------------------	*/
/*		o p e r a t i o n		*/
/*	-----------------------------------	*/
private	int	operation( int argc, char * argv[] )
{
	int	argi=1;
	char *	aptr;
	int	status;
	char *	syntax=(char *) 0;
	char *	command=(char *) 0;
	while ( argi < argc )
	{
		if (!( aptr = argv[argi++] ))
			break;
		else if ( *aptr != '-' )
		{
			if (!( command ))
			{
				command = aptr;
				continue;
			}
			else if ((!( strcasecmp( command, "COSACS" ))) && (!( syntax )))
			{
				syntax = aptr;
				continue;
			}
			else if (!(status = service_operation( command, aptr, syntax ) ))
				continue;
			else	return( failure( status, command, aptr ) );
		}
		else
		{
			aptr++;
			if ( *aptr == '-' )
			{
				aptr++;
				if (!( strcmp( aptr, "verbose" ) ))
					verbose = 1;
				else if (!( strcmp( aptr, "debug" ) ))
					debug = 1;
				else if (!( strcmp( aptr, "tls" ) ))
					tls = argv[argi++];
				else if (!( strcmp( aptr, "publisher" ) ))
					publisher = argv[argi++];
				else if (!( strcmp( aptr, "agent" ) ))
					agent = argv[argi++];
				else	
				{
					return( failure( 30, "incorrect command option", aptr ) );
				}
			}
		}	
	}
	return( 0 );
}

/*	-----------------------------------	*/
/*		b a n n e r 			*/
/*	-----------------------------------	*/
private	int	banner()
{
	printf("\n   CompatibleOne Command Line Tool : Version 1.0a.0.04");
	printf("\n   Beta Version : 27/02/2012 ");
	printf("\n   Copyright (c) 2011,2012 Iain James Marshall ");
	printf("\n   Usage : ");
	printf("\n         command <options> START    <service_file> ");
	printf("\n         command <options> STOP     <service_file> ");
	printf("\n         command <options> SAVE     <service_file> ");
	printf("\n         command <options> SNAPSHOT <service_file> ");
	printf("\n         command <options> DELETE   <service_file> ");
	printf("\n         command <options> COSACS   <service_file> <instruction> ");
	printf("\n   Options: ");
	printf("\n         --publisher <publisher>      specify publisher identity ");
	printf("\n         --agent     <agent>          specify agent identity ");
	printf("\n         --tls       <security>       specify security configuration ");
	printf("\n         --verbose                    activate verbose messages");
	printf("\n         --debug                      activate debug messages \n");
	return( 0 );
}

/*	-----------------------------------	*/
/*			m a i n			*/
/*	-----------------------------------	*/
public	int	main( int argc, char * argv[] )
{
	if ( argc == 1 )
		return( banner() );
	else	return( operation( argc, argv ) );
}

	/* ---------- */
#endif	/* _command_c */
	/* ---------- */

