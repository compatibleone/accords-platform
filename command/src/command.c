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
private	int	noauth=0;

private	char *	agent="CO-COMMAND/1.0";
private	char *	publisher="http://127.0.0.1:8086";
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

#include "cspi.c"

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

	if (!( sptr = occi_resolve_category_provider( _CORDS_SERVICE, agent, default_tls() ) ))
		return( 500 );
	else	sprintf(buffer,"%s/%s/%s",sptr,_CORDS_SERVICE,id );

	if (!( zptr =  cords_invoke_action( buffer, action, agent, default_tls() ) ))
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

	if (!( noauth ))
	{
		if (!( auth = login_occi_user( "test-broker","co-system",agent, default_tls() ) ))
			return(403);
		else 	(void) occi_client_authentication( auth );
	}

	status = ll_cords_service_action( id, action );

	if (!( noauth ))
	{
		(void) logout_occi_user( "test-broker","co-system",agent, auth, default_tls() );	
	}

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

	if (!( sptr = occi_resolve_category_provider( _CORDS_SERVICE, agent, default_tls() ) ))
		return( 500 );
	else	sprintf(buffer,"%s/%s/%s",sptr,_CORDS_SERVICE,id );

	if (!( zptr =  occi_simple_delete( buffer, agent, default_tls() ) ))
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

	if (!( noauth ))
	{
		if (!( auth = login_occi_user( "test-broker","co-system",agent, default_tls() ) ))
			return(403);
		else 	(void) occi_client_authentication( auth );
	}

	status = ll_cords_service_delete( id );

	if (!( noauth ))
	{
		(void) logout_occi_user( "test-broker","co-system",agent, auth, default_tls() );	
	}

	return( status );
}

/*	-----------------------------------------------------	*/
/*		c o s a c s _ c o m m a n d _ t o o l		*/
/*	-----------------------------------------------------	*/
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
	sprintf(buffer,"%s://%s:%u/%s/%s",rest_http_prefix(),host,_COSACS_PORT,category,item);

	if ( verbose )
		printf("   %s %s\n",command,buffer);

	if (!( strcasecmp( command, "LIST" ) ))
	{
		if (!( rptr = occi_simple_get( buffer, agent, default_tls() ) ))
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
		if (!( rptr = occi_simple_get( buffer, agent, default_tls() ) ))
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
/*		c l e a n _ c o m m a n d _ l i n e		*/
/*	-----------------------------------------------------	*/
private	int	clean_command_line( char * buffer, int length, FILE * h )
{
	int	i;
	if (!( fgets( buffer, length, h ) ))
		return( 0 );
	else
	{
		for ( i=0; buffer[i] != 0; i++ )
		{
			if ( buffer[i] == '\n' )
				buffer[i] =0;
			else if ( buffer[i] == '\r' )
				buffer[i] =0;
		}
		return( i );
	}
}

/*	-----------------------------------------------------	*/
/*	    o c c i _ s e r v i c e _ o p e r a t i o n		*/
/*	-----------------------------------------------------	*/
private	int	occi_service_operation( char * filename, char * body )
{
	FILE *	h;
	int	n;
	int	i;
	char	buffer[4096];
	char *	method=(char *) 0;
	char *	object=(char *) 0;
	char *	version=(char *) 0;
	char *	wptr;
	char *	xptr;
	struct	rest_header * root=(struct rest_header * ) 0;
	struct	rest_header * foot=(struct rest_header * ) 0;
	struct	rest_header * hptr=(struct rest_header * ) 0;
	struct	rest_response * rptr;

	if ( verbose )
		printf("\nOCCI infile=%s\n",filename);

	/* ------------------- */
	/* open the input file */
	/* ------------------- */
	if (!( h = fopen( filename, "r" ) ))
		return( 0 );

	/* --------------------- */
	/* read the request line */
	/* --------------------- */
	else if (!( clean_command_line( buffer, 4096, h ) ))
	{
		fclose(h);
		return(0);
	}
	/* ------------------------ */
	/* analyse the request line */
	/* ------------------------ */
	else
	{
		for ( xptr=wptr=buffer; *wptr != 0; wptr++ )
		{
			if ( *wptr == ' ' )
			{
				*(wptr++) = 0;
				if (!( method ))
				{
					method = allocate_string( xptr );
					xptr = wptr;
				}
				else if (!( object ))
				{
					object = allocate_string( xptr );
					xptr = wptr;
				}
				else	break;
			}
		}
		version = allocate_string( xptr );
		if ( verbose )
			printf("\nmethod=%s\nobject=%s\nversion=%s\n",
				(method  ? method : "<null method>"), 
				(object  ? object : "<null object>"),
				(version ? version : "<null version>"));
	}
	/* ------------------------ */
	/* read the request headers */
	/* ------------------------ */
	while ((n = clean_command_line( buffer, 4096, h )) > 1 )
	{
		for ( xptr=wptr=buffer; *wptr != 0; wptr++ )
			if ( *wptr == ':' )
				break;
		if ( *wptr != ':' )
			break;
		else
		{
			*(wptr++) = 0;
			while ( *wptr == ' ' )
				wptr++;
			if (!( hptr = rest_postfix_header( foot, xptr, wptr ) ))
				break;
			else 	foot = hptr;
			if (!( root ))
				root = hptr;
			if ( verbose )
				printf("header(%s=%s)\n",hptr->name,hptr->value);
		}
	}

	if (!( strcasecmp( method, "GET" ) ))
		rptr = rest_client_get_request( object, default_tls(), agent, root );
	else if (!( strcasecmp( method, "POST" ) ))
	 	rptr = rest_client_post_request( object, default_tls(), agent, body, root );
	else if (!( strcasecmp( method, "DELETE" ) ))
		rptr = rest_client_delete_request( object, default_tls(), agent, root );
	else if (!( strcasecmp( method, "PUT" ) ))
		rptr = rest_client_put_request( object, default_tls(), agent, body, root );
	else if (!( strcasecmp( method, "HEAD" ) ))
		rptr = rest_client_head_request( object, default_tls(), agent, root );
	else	rptr = (struct rest_response *) 0;

	fclose(h);

	if (!( rptr ))
	{
		if ( verbose )
			printf("request failure\n");
		return(0);
	}
	else
	{
		sprintf(buffer,"%s.response",filename);
		if ( verbose )
			printf("OCCI outfile=%s\n",buffer);
		if (!( h = fopen( buffer, "w" ) ))
			return( 0 );
		else
		{
			if ( rptr->status > 299 )
				fprintf(h,"%s %u %s\r\n",rptr->version,rptr->status,rptr->message);
			else
			{
				for ( 	hptr=rptr->first;
					hptr != (struct rest_header *) 0;
					hptr = hptr->next )
					fprintf(h,"%s: %s\r\n",hptr->name,hptr->value);
			}
			fprintf(h,"\r\n");
			fclose(h);
			return(0);
		
		}
	}
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
/*	      l l _ i n v o i c e _ o p e r a t i o n		*/
/*	-----------------------------------------------------	*/
private	int	ll_invoice_operation( char * account, char * other )
{
	char *	ihost;
	char * 	accountid;
	struct	occi_element * eptr;
	struct	occi_response * zptr;
	struct	occi_response * yptr;
	char *	sptr;
	char	buffer[2048];
	initialise_occi_resolver( publisher, (char *) 0, (char *) 0, (char *) 0 );

	if (!( accountid = occi_resolve_account( account, agent, default_tls() ) ))
		return(failure(40,"unknown account",account));
	else if (!( eptr = occi_create_element( "occi.invoice.account", accountid ) ))
		return(55);

	if (!( sptr = occi_resolve_category_provider( _CORDS_INVOICE, agent, default_tls() ) ))
		return( 500 );
	else	sprintf(buffer,"%s/%s/",sptr,_CORDS_INVOICE );

	if (!( zptr = occi_simple_post( buffer, eptr, agent, default_tls() )))
		return(56);
	else if (!( ihost = occi_extract_location( zptr ) ))
	{
		zptr = occi_remove_response( zptr );
		return( 118 );
	}
	else
	{
		if (!( yptr = occi_simple_get( ihost, agent, default_tls() ) ))
		{
			zptr = occi_remove_response( zptr );
			return( 119 );
		}
		else if (!( eptr = occi_locate_element( yptr->first, "occi.invoice.document" ) ))
		{
			yptr = occi_remove_response( yptr );
			zptr = occi_remove_response( zptr );
			return(120);
		}
		else
		{
			printf("invoice: %s\n",eptr->value);
			yptr = occi_remove_response( yptr );
			zptr = occi_remove_response( zptr );
			return(0);
		}
	}
}

/*	-----------------------------------------------------	*/
/*		c o r d s _ i n v o i c e _ a c t i o n		*/
/*	-----------------------------------------------------	*/
private	int	cords_invoice_action( char * account, char * other )
{
	int	status;
	char *	auth;

	initialise_occi_resolver( _DEFAULT_PUBLISHER, (char *) 0, (char *) 0, (char *) 0 );

	if (!( noauth ))
	{
		if (!( auth = login_occi_user( "test-broker","co-system",agent, default_tls() ) ))
			return(403);
		else 	(void) occi_client_authentication( auth );
	}

	status = ll_invoice_operation( account, other );

	if (!( noauth ))
	{
		(void) logout_occi_user( "test-broker","co-system",agent, auth, default_tls() );	
	}

	return( status );
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
	else if (!( strcasecmp( command, "INVOICE"  ) ))
		return( cords_invoice_action( filename, syntax ) );
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
		else if (!( strcasecmp( command, "RESTART" ) ))
			return( cords_service_action( id, "restart" ) );
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
		else if (!( strcasecmp( command, "OCCI"  ) ))
			return( occi_service_operation( filename, syntax ) );
		else	return( failure( 30,"incorrect command", command ) );
	}	
}

/*	---------------------------------------------------	*/
/*	    l l _ c o m m a n d _ t r a n s a c t i o n		*/
/*	---------------------------------------------------	*/
private	int	ll_command_transaction( char * account, char * price, char * reference, char * action, char * description )
{
	int	status=0;
	char *	accountid=(char *) 0;
	char *	priceid=(char *) 0;

	initialise_occi_resolver( publisher, (char *) 0, (char *) 0, (char *) 0 );

	if (!( accountid = occi_resolve_account( account, agent, default_tls() ) ))
		return(failure(78,"unknown account",account));
	else if (!( priceid = occi_resolve_price( price, agent, default_tls() ) ))
	{
		accountid = liberate( accountid );
		return(failure(78,"unknown price",price));
	}
	else if (!( occi_send_transaction( reference, priceid, action, accountid, description )))
	{
		accountid = liberate( accountid );
		priceid = liberate( priceid );
		return(failure(55,"transaction failure",reference));
	}
	else
	{
		accountid = liberate( accountid );
		priceid = liberate( priceid );
		return( 0 );
	}
}

/*	---------------------------------------------------	*/
/*		c o m m a n d _ t r a n s a c t i o n		*/
/*	---------------------------------------------------	*/
private	int	command_transaction( char * account, char * price, char * reference, char * action, char * description )
{
	int		status=0;
	char 	*	auth=(char *) 0;
	if (!( account ))
		return( 30 );
	else if (!( price ))
		return( 31 );
	else if (!( reference ))
		return( 32 );
	else if (!( description ))
		return( 33 );
	else
	{
		initialise_occi_resolver( _DEFAULT_PUBLISHER, (char *) 0, (char *) 0, (char *) 0 );

		if (!( noauth ))
		{
			if (!( auth = login_occi_user( "test-broker","co-system",agent, default_tls() ) ))
				return(403);
			else 	(void) occi_client_authentication( auth );
		}

		status = ll_command_transaction( account ,price, reference, action, description );

		if (!( noauth ))
		{
			(void) logout_occi_user( "test-broker","co-system",agent, auth, default_tls() );	
		}

		return( status );

	}
}


/*	---------------------------	*/
/*	c r e a t e _ a c c o u n t	*/
/*	---------------------------	*/
private	int	create_account( char * account )
{
	return( 0 );
}

/*	---------------------------	*/
/*	   c r e a t e _ u s e r	*/
/*	---------------------------	*/
private	int	create_user( char * account, char * name, char * pass, char * role, char * email, char * permission )
{
	return( 0 );
}

/*	---------------------------------------------------	*/
/*		c o m m a n d _ t r a n s a c t i o n		*/
/*	---------------------------------------------------	*/
/*	invoke the indicated occi 'action' on the required 	*/
/*	resource instance.					*/
/*	---------------------------------------------------	*/
private	int	invoke_action( char * action, char * instance )
{
	int			status=0;
	char 			* auth=(char *) 0;
	struct occi_response 	* zptr;
	if (!( action ))
		return( 30 );
	else if (!( instance ))
		return( 31 );
	else
	{
		initialise_occi_resolver( _DEFAULT_PUBLISHER, (char *) 0, (char *) 0, (char *) 0 );

		if (!( noauth ))
		{
			if (!( auth = login_occi_user( "test-broker","co-system",agent, default_tls() ) ))
				return(403);
			else 	(void) occi_client_authentication( auth );
		}

		if (!( zptr =  cords_invoke_action( instance, action, agent, default_tls() ) ))
			status = 501;
		else
		{
			zptr = occi_remove_response( zptr );
			status = 0;
		}
		
		if (!( noauth ))
		{
			(void) logout_occi_user( "test-broker","co-system",agent, auth, default_tls() );	
		}

		return( status );

	}
}

/*	---------------------------------------------------	*/
/*	r u n _ c o r d s c r i p t _ i n t e r p r e t e r	*/
/*	---------------------------------------------------	*/
private	int	run_cordscript_interpreter( char * filename, int argc, char * argv[] )
{
	int			status=0;
	char 			* auth=(char *) 0;
	struct occi_response 	* zptr;

	initialise_occi_resolver( publisher, (char *) 0, (char *) 0, (char *) 0 );

	if (!( noauth ))
	{
		if (!( auth = login_occi_user( "test-broker","co-system",agent, default_tls() ) ))
			return(403);
		else 	(void) occi_client_authentication( auth );
	}

	cordscript_interpreter( filename, argc, argv );

	if (!( noauth ))
	{
		(void) logout_occi_user( "test-broker","co-system",agent, auth, default_tls() );	
	}

	return( status );

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
			else if (!( strcasecmp( command, "INVOKE" ) ))
				return( invoke_action( aptr, argv[argi] ) );
			else if (!( strcasecmp( command, "RUN" ) ))
				return( run_cordscript_interpreter( aptr, argc-argi, &argv[argi] ) );
			else if (!( strcasecmp( command, "ACCOUNT" ) ))
				return( create_account( aptr ) );
			else if (!( strcasecmp( command, "USER" ) ))
				return( create_user( aptr, argv[argi], argv[argi+1], argv[argi+2 ], argv[argi+3], argv[argi+4] ) );
			else if (!( strcasecmp( command, "TRANSACTION" ) )) 
				return( command_transaction( aptr, argv[argi], argv[argi+1], argv[argi+2 ], argv[argi+3] ) );

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
				else if (!( strcmp( aptr, "noauth" ) ))
					noauth = 1;
				else if (!( strcmp( aptr, "echo") ))
					csp_set_echo(1);
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
	printf("\n   CompatibleOne Command Line Tool : Version 1.0b.0.03");
	printf("\n   Beta Version : 09/11/2012 ");
	printf("\n   Copyright (c) 2011,2012 Iain James Marshall ");
	printf("\n   Usage : ");
	printf("\n         command <options> START       <service_file> ");
	printf("\n         command <options> RESTART     <service_file> ");
	printf("\n         command <options> STOP        <service_file> ");
	printf("\n         command <options> SAVE        <service_file> ");
	printf("\n         command <options> SNAPSHOT    <service_file> ");
	printf("\n         command <options> DELETE      <service_file> ");
	printf("\n         command <options> COSACS      <service_file> <instruction> ");
	printf("\n         command <options> OCCI        [body] <request> ");
	printf("\n         command <options> INVOICE     <account>      ");
	printf("\n         command <options> ACCOUNT     <name> ");
	printf("\n         command <options> USER        <account> <name> <pass> <role> <email> <permission> ");
	printf("\n         command <options> INVOKE      <action> <instance> ");
	printf("\n         command <options> RUN         <script> ");
	printf("\n         command <options> TRANSACTION <account> <price> <reference> <action> <description> ");
	printf("\n   Options: ");
	printf("\n         --publisher <publisher>      specify publisher identity ");
	printf("\n         --agent     <agent>          specify agent identity ");
	printf("\n         --tls       <security>       specify security configuration ");
	printf("\n         --verbose                    activate verbose messages");
	printf("\n         --noauth                     inhibit authentication for test purposes");
	printf("\n         --echo                       activate source echo ");
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

