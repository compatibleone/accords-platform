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
private	int	port=80;
private	int	threads=1;
private	int	authorise=0;
private	char *	user=(char *) 0;
private	char *	password=(char *) 0;
private	int	echo=0;

public	char *	default_operator()	{	return( operator );	}
public	char *	default_zone()		{	return( zone );		}
public	char *	default_publisher()	{	return( publisher );	}
public	char *	default_tls()		{	return( tls );		}
public	int	check_verbose()		{	return( verbose );	}
public	int	check_debug()		{	return( debug );	}

public	struct occi_request * cords_account_request( struct occi_client * kptr, char * object, int type );

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

#include "cspi.h"
#include "colog.c"

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

private	int	run_json_verification( char * filename )
{
	struct	data_element * dptr;

	if (!( dptr = json_parse_file( filename ) ))
		return( 40 );
	else
	{
		printf("\n");
		json_show( dptr );
		printf("\n");
		dptr = drop_data_element( dptr );
		return(0);
	}
}

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

/*	-----------------------------------------------------	*/
/*	t e s t _ c o r d s _ p a r s e r _ o p e r a t i o n	*/
/*	-----------------------------------------------------	*/
private	int	ll_cords_parser_operation( char * filename )
{
	char *dirc, *basec, *bname, *dname;
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
/*	t e s t _ c o r d s _ p a r s e r _ o p e r a t i o n	*/
/*	-----------------------------------------------------	*/
private	int	cords_parser_operation( char * filename )
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

	status = ll_cords_parser_operation( filename );

	(void) logout_occi_user( "test-parser","co-system",Cp.agent, auth, Cp.tls );	

	return( status );
}

/*	-----------------------------------	*/
/*	   c o m m a n d _ p a r s e  r		*/
/*	-----------------------------------	*/
private	int	command_parser( char * filename, char * instance )
{
	return( cords_parser_operation( filename ) );
}

struct	cords_broker_config
{
	char *	accept;
	char *	publisher;
	char *	host;
	char *	agent;
	char *	result;
	char *	tls;
	char *	zone;
	char *	operator;
	char *	security;
	int	deployment;
} Cb = 	{
	(char * ) 0,
	_CORDS_DEFAULT_PUBLISHER,
	_CORDS_DEFAULT_PUBLISHER,
	_CORDS_BROKER_AGENT,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	1
	};



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
/*	   c o r d s _ i n s t a n c e _ a g r e e m e n t	*/
/*	-----------------------------------------------------	*/
private	int	cords_instance_agreement( char * host, char * name, char * sla, char * manifest, char * plan,  char * agent, char * result )
{
	struct	occi_response * zptr;
	char	*	namev;
	char	*	planv;
	char 	*	slav;
	char  	*	manv;
	char 	*	ihost;
	char 	*	aptr;
	int		i;
	int		j;
	FILE	*	h;
	struct	occi_client * kptr;
	struct	occi_request* qptr;
	struct	occi_element* dptr;
	struct	occi_response* yptr;
	char	buffer[1024];

	/* ------------------- */
	/* prepare environment */
	/* ------------------- */
	initialise_occi_resolver( host, (char *) 0, (char *) 0, (char *) 0 );

	/* ------------------- */
	/* validate parameters */
	/* ------------------- */
	if (!( manv = occi_unquoted_value( manifest )))
		return(531);
	else if (!( slav = occi_unquoted_value( sla  )))
		return(532);
	else if (!( planv = occi_unquoted_value( plan )))
		return(533);
	else if (!( namev = occi_unquoted_value( name )))
		return(533);

	/* ------------------------------------------- */
	/* resolve a service category instance manager */
	/* ------------------------------------------- */
	if (!( ihost = occi_resolve_category_provider( _CORDS_SERVICE, agent, default_tls() ) ))
		return(478);

	/* ------------------------------------------ */
	/* url for service category instance creation */ 
	/* ------------------------------------------ */
	else
	{
		sprintf(buffer,"%s/%s/",ihost,_CORDS_SERVICE);
		liberate( ihost );
	}

	/* ------------------------------------------ */
	/* create the SLA controlled service instance */
	/* ------------------------------------------ */
	if (!( kptr = occi_create_client( buffer, agent, default_tls() ) ))
		return(546);
	else if (!( qptr = cords_account_request( kptr, kptr->target->object, _OCCI_NORMAL )))
	{
		kptr = occi_remove_client( kptr );
		return(550);
	}
	else if ((!(dptr=occi_request_element(qptr,"occi.service.plan"  	, planv ) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.service.manifest"   	, manv  ) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.service.name"   	, namev ) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.service.sla"  		, slav 	) )))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return(551);
	}
	else if (!( yptr = occi_client_post( kptr, qptr ) ))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return(552);
	}
	else if (!( ihost = occi_extract_location( yptr ) ))
	{
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return(553);
	}
	/* -------------------------------------- */
	/* duplicate the host before the clean up */
	/* -------------------------------------- */
	else if (!( ihost = allocate_string( ihost ) ))
	{
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return(554);
	}
	else
	{
		/* ------------------------------- */
		/* clean up after service creation */
		/* ------------------------------- */
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
	
		/* -------------------------------- */
		/* start the SLA controlled service */
		/* -------------------------------- */
		rest_add_http_prefix(buffer,1024,ihost);

		if (!( Cb.deployment ))
		{
			for ( j=0,i=0; *(ihost+i) != 0; i++)
				if ( *(ihost+i) == '/' )
					j = (i+1);
			sprintf(buffer,"service/%s",(ihost+j));
			printf("%s\n",buffer);
			if ((h = fopen(buffer,"w")) != (FILE *) 0)
			{
				fprintf(h,"{}\n");
				fclose(h);
			}
			ihost = liberate( ihost );
			return( 0 );
		}
		else 
		{
			ihost = liberate( ihost );
			if (!( zptr =  cords_invoke_action( buffer, _CORDS_START, agent, default_tls() ) ))
				return(503);
			else
			{
				zptr = occi_remove_response( zptr );
				return( 0 );
			}
		}
	}
}

/*	-----------------------------------------------------	*/
/*	   l l _ s l a _ b r o k e r _ o p e r a t i o n	*/
/*	-----------------------------------------------------	*/
/*	this function will provision an instance of service	*/
/*	as described by the input document which will provide 	*/
/*	plan identifier that will be used to resolve to the	*/
/*	manifest description of the service. The operation	*/
/*	can be initated on either a manifest document type or	*/
/*	for the new agreement document type under sla control	*/
/*	-----------------------------------------------------	*/
private	int	ll_sla_broker_operation( char * filename )
{
	struct	occi_response * zptr;
	struct	xml_element * dptr;
	struct	xml_element * eptr;
	struct	xml_element * fptr;
	struct	xml_element * tptr;
	struct	xml_element * xptr;
	struct	xml_atribut * aptr;
	struct	xml_atribut * gptr;
	struct	xml_atribut * mptr;
	struct	xml_atribut * pptr;
	char *	nptr;
	char	nameplan[512];
	int	status;

	/* ------------------- */
	/* validate parameters */
	/* ------------------- */
	if (!(nptr = Cb.result))
		sprintf((nptr=nameplan),"instance_%s",filename);
	if (!( Cb.host ))
		return( failure(1,"requires","publication host"));
	else if (!( Cb.agent ))
		return( failure(2,"requires","parser agent name"));
	else if (!( filename ))
		return( failure(3,"requires","cords filename"));

	/* -------------------------- */
	/* process the input document */
	/* -------------------------- */
	else if (!( dptr = document_parse_file( filename ) ))
		return( failure(4,"parse error",filename));

	/* ----------------------------- */
	/* detect manifest document type */
	/* ----------------------------- */
	if (( eptr = document_element( dptr, _CORDS_MANIFEST )) != (struct xml_element *) 0)
	{
		if (!( aptr = document_atribut( eptr, _CORDS_PLAN ) ))
			return( failure(6,"failure resolving plan",filename));
		else if ((status = cords_instance_plan( Cb.host, aptr->value, Cb.agent, nptr )) != 0)
			return( failure(status,"failure provisioning plan",aptr->value));
		else	return( 0 );
	}

	/* ------------------------------ */
	/* detect agreement document type */
	/* ------------------------------ */
	else if (!( fptr = document_element( dptr, _CORDS_AGREEMENT ) ))
		return( failure(5,"expected manifest or agreement document",filename));
	else if (!( gptr = document_atribut( fptr, _CORDS_ID ) ))
		return( failure(5,"missing agreement identifier",filename));
	else if (!( tptr = document_element( fptr, _CORDS_TERMS ) ))
		return( failure(5,"missing agreement terms",filename));
	else
	{
		for ( 	eptr = (struct xml_element *) 0,
			xptr = document_element( tptr, _CORDS_TERM );
			xptr != (struct xml_element *) 0;
			tptr = xptr->next )
		{
			if (!( eptr = document_element( xptr, _CORDS_MANIFEST ) ))
				continue;
			else	break;
		}
		if (!( eptr ))
			return( failure(5,"missing manifest element",filename));
		else if (!( mptr = document_atribut( eptr, _CORDS_ID ) ))
			return( failure(5,"missing manifest identifier",filename));
		else if (!( pptr = document_atribut( eptr, _CORDS_ID ) ))
			return( failure(5,"missing manifest name",filename));
		else if (!( aptr = document_atribut( eptr, _CORDS_PLAN ) ))
			return( failure(6,"missing plan identifier",filename));
		else if ((status = cords_instance_agreement( Cb.host, pptr->value, gptr->value, mptr->value, aptr->value, Cb.agent, nptr )) != 0)
			return( failure(status,"failure to provision plan",aptr->value));
		else	return( 0 );
	}
}

/*	-------------------------------------------	*/
/*	c o r d s _ p a r s e r _ o p e r a t i o n	*/
/*	-------------------------------------------	*/
private	int	cords_script_interpreter( char * filename )
{
	char *	argv[10];
	int	argc=0;
	argv[0] = (char *) 0;
	run_cordscript_interpreter( filename, argc, argv );
	return( 200 );
}

/*	------------------------------------------------	*/
/*	c o r d s _ r e s o l v e r _ o p e r a t i o n		*/
/*	------------------------------------------------	*/
private	int	cords_resolver_operation( char * category )
{
	int	status;
	char *	auth;

	occi_client_accept( Cb.accept );

	initialise_occi_resolver( _DEFAULT_PUBLISHER, (char *) 0, (char *) 0, (char *) 0 );

	if (!( auth = login_occi_user( "test-broker","co-system",Cb.agent, Cb.tls ) ))
		return(403);
	else 	(void) occi_client_authentication( auth );

	(void) logout_occi_user( "test-broker","co-system",Cb.agent, auth, Cb.tls );	

	return( status );
}

/*	------------------------------------------------	*/
/*	c o r d s _ s e r v i c e  _ o p e r a t i o n		*/
/*	------------------------------------------------	*/
private	int	cords_service_operation( char * command, char * service )
{
	int	status;
	char *	auth;

	occi_client_accept( Cb.accept );

	initialise_occi_resolver( _DEFAULT_PUBLISHER, (char *) 0, (char *) 0, (char *) 0 );

	if (!( auth = login_occi_user( "test-command","co-system",Cb.agent, Cb.tls ) ))
		return(403);
	else 	(void) occi_client_authentication( auth );

	(void) logout_occi_user( "test-command","co-system",Cb.agent, auth, Cb.tls );	

	return( status );
}

/*	-------------------------------------------	*/
/*	c o r d s _ b r o k e r _ o p e r a t i o n	*/
/*	-------------------------------------------	*/
private	int	cords_broker_operation( char * filename )
{
	int	status;
	char *	auth;

	occi_client_accept( Cb.accept );

	initialise_occi_resolver( _DEFAULT_PUBLISHER, (char *) 0, (char *) 0, (char *) 0 );

	if (!( auth = login_occi_user( "test-broker","co-system",Cb.agent, Cb.tls ) ))
		return(403);
	else 	(void) occi_client_authentication( auth );

	status = ll_sla_broker_operation( filename );

	(void) logout_occi_user( "test-broker","co-system",Cb.agent, auth, Cb.tls );	

	return( status );
}

/*	-----------------------------------	*/
/*	   c o m m a n d _ b r o k e  r		*/
/*	-----------------------------------	*/
private	int	command_broker( char * filename, char * other )
{
	return(cords_broker_operation( filename ));
}

/*	------------------------------------------------------------------	*/
/*		c o m m a n d s e r v e r _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * commandserver_initialise(  void * v,struct rest_server * sptr )
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
/*		c o m m a n d s e r v e r _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	commandserver_authorise(  void * v,struct rest_client * cptr, char * username, char * password)
{
	if ( check_verbose )
		printf("   REST Authentication of %s \n",username); 
	if (!( authorise ))
		return(1);
	else if (!( user ))
		return(1);
	else if ( strcmp( username, user ) )
		return(0);
	else if ( strcmp( password, password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*		c o m m a n d s e r v e r_ e x t e n s i o n 			*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * commandserver_extension( void * v,struct rest_server * sptr, 
struct rest_extension * xptr)
{
	return( xptr );
}


/*	---------------------------------------------------------	*/
/*		d e t e c t _ c o m m a n d _ f i l e 			*/
/*	---------------------------------------------------------	*/
private	char *	detect_command_file( char * stub, char * ext )
{
	char	buffer[1024];
	FILE *	h;
	sprintf(buffer,"%s.%s",stub,ext);
	if (!( h = fopen( buffer,"r" )))
		return((char *) 0);
	else
	{
		fclose(h);
		return( allocate_string( buffer ) );
	}
}	

/*	--------------------------------------------------------	*/
/*		c o m m a n d _ g e t _ f i l e n a m e 		*/
/*	--------------------------------------------------------	*/
private	char *	default_get_filename( char * command )
{
	FILE *	h;
	char	buffer[1024];
	char *	filename;
	if ((!( strcasecmp( command, "parser" 	) ))
	||  (!( strcasecmp( command, "broker" 	) ))
	||  (!( strcasecmp( command, "resolver" ) ))
	||  (!( strcasecmp( command, "service" 	) ))
	||  (!( strcasecmp( command, "" 	) ))
	||  (!( strcasecmp( command, "script" 	) )))
	{
		sprintf(buffer,"%s.html",command);
		if (!( h = fopen( buffer, "w") ))
			return( (char *) 0 );
		else
		{
			fprintf(h,"<html><head><title>%s:%s</title>\n",agent,command);

			fprintf(h,"</head><body><div align=center><h1>%s:%s</h1>\n",agent,command);
			if ( strcasecmp( command, "" ) )
				fprintf(h,"<form method=POST action=%s enctype='multipart/form-data'>\n",command);
			fprintf(h,"<table class=%s>\n",command);
			if (!( strcasecmp( command, "parser" ) ))
			{
				fprintf(h,"<tr><th>Select Manifest or SLA file<th><input type=file name=filename width=48></tr>\n");
				fprintf(h,"<tr><th>Parse Document<th><input type=submit name=%s value=%s></tr>\n","command",command);

			}
			else if (!( strcasecmp( command, "broker" ) ))
			{
				fprintf(h,"<tr><th>Select Plan File<th><input type=file name=filename width=48></tr>\n");
				fprintf(h,"<tr><th>Broker Service<th><input type=submit name=%s value=%s></tr>\n","command",command);
			}		
			else if (!( strcasecmp( command, "resolver" ) ))
			{
				fprintf(h,"<tr><th>Specify Category<th><input type=text name=category width=48></tr>\n");
				fprintf(h,"<tr><th>Resolve Category Endpoints<th><input type=submit name=%s value=%s></tr>\n","command",command);
			}		
			else if (!( strcasecmp( command, "script" ) ))
			{
				fprintf(h,"<tr><th>Select Script File<th><input type=file name=filename width=48></tr>\n");
				fprintf(h,"<tr><th>Run Script<th><input type=submit name=%s value=%s></tr>\n","command",command);
			}
			else if (!( strcasecmp( command, "service" ) ))
			{
				fprintf(h,"<tr><th>Specify Service ID<th><input type=text name=service width=48></tr>\n");
				fprintf(h,"<tr><th>Start Service<th><input type=submit name=%s value=%s></tr>\n","service","start");
				fprintf(h,"<tr><th>Stop  Service<th><input type=submit name=%s value=%s></tr>\n","service","stop");
				fprintf(h,"<tr><th>Save  Service<th><input type=submit name=%s value=%s></tr>\n","service","save");
				fprintf(h,"<tr><th>Snapshot Service<th><input type=submit name=%s value=%s></tr>\n","service","snapshot");
				fprintf(h,"<tr><th>Delete Service<th><input type=submit name=%s value=%s></tr>\n","service","delete");
			}
			else
			{
				fprintf(h,"<tr><th colspan=2><a href=\"/resolver\">Command Resolver</a></th></tr>\n");
				fprintf(h,"<tr><th colspan=2><a href=\"/parser\">Command Parser</a></th></tr>\n");
				fprintf(h,"<tr><th colspan=2><a href=\"/broker\">Command Broker</a></th></tr>\n");
				fprintf(h,"<tr><th colspan=2><a href=\"/service\">Command Service</a></th></tr>\n");
				fprintf(h,"<tr><th colspan=2><a href=\"/script\">Command Script</a></th></tr>\n");
			}		
			fprintf(h,"</table>\n");
			if ( strcasecmp( command, "" ) )
				fprintf(h,"</form>\n");
			fprintf(h,"</div></body></html>\n");
			fclose( h );
			return( allocate_string( buffer ) );
		}
	}
	else	return((char *) 0);
}

/*	--------------------------------------------------------	*/
/*		c o m m a n d _ g e t _ f i l e n a m e 		*/
/*	--------------------------------------------------------	*/
private	char *	command_get_filename( char * command )
{
	char *	filename;
	if ((!( strcasecmp( command, "parser" 	) ))
	||  (!( strcasecmp( command, "broker" 	) ))
	||  (!( strcasecmp( command, "resolver"	) ))
	||  (!( strcasecmp( command, "service" 	) ))
	||  (!( strcasecmp( command, "script" 	) )))
	{
		if ((filename = detect_command_file( command, "html" ) ))
			return( filename );
		else if ((filename = detect_command_file( command, "htm" ) ))
			return( filename );
	}
	return((char *) 0);
}


/*	------------------------------------------------------------------	*/
/*			c o m m a n d s e r v e r _ g e t _ f i l e 		*/
/*	------------------------------------------------------------------	*/
private	struct rest_response * commandserver_get_file( 
		struct rest_client * cptr, 
		char *filename,
		struct rest_response * aptr )
{
	struct	stat info;
	char	length[64];
	char	buffer[1024];
	char *	nptr;
	if ( stat( filename,&info ) < 0 )
		return( rest_html_response( aptr, 404, "File Not Found" ) );
	else
	{
		sprintf(length,"%u",info.st_size);
		if (!( rest_response_header(aptr,"Content-Length",length) ))
			return( rest_html_response( aptr, 500, "Server Failure" ) );
		else if (!( rest_response_header(aptr,"Content-Type","text/html") ))
			return( rest_html_response( aptr, 500, "Server Failure" ) );
		else if (!( nptr = allocate_string( filename ) ))
			return( rest_html_response( aptr, 500, "Server Failure" ) );
		else
		{
			rest_response_body(aptr,nptr,0 );
			return( rest_response_status( aptr, 200, "OK" ) );
		}
	}
}


/*	------------------------------------------------------------------	*/
/*		c o m m a n d s e r v e r_ g e t 				*/
/*	------------------------------------------------------------------	*/
private	struct rest_response * commandserver_get( void * v,struct rest_client * cptr, struct rest_request * rptr )
{
	char *	filename;
	char *	command;
	char *	sptr;
	struct	rest_response * aptr;
	printf("   %s GET Request : %s %s %s \n",agent,rptr->method,rptr->object,rptr->version);
	if (!( aptr = rest_allocate_response(cptr) ))
		return( aptr );
	else if ( rptr->body  )
		return( rest_html_response( aptr, 400, "Unexpected Request Body" ) );
	else
	{
		if (!( command = allocate_string( rptr->object ) ))
			return( rest_html_response( aptr, 500, "Incorrect Request Body" ) );
		else if ( *command == '/' )
			command++;
		sptr = command;
		while( *sptr )
		{
			if ( *sptr == '/' ) break;
			else sptr++;
		}
		*(sptr++) = 0;
		if (( filename = command_get_filename( command )) != (char *) 0)
			return( commandserver_get_file( cptr, filename, aptr ) );
		else if (!( filename = default_get_filename( command )))
			return( rest_html_response( aptr, 400, "Incorrect Request" ) );
		else	return( commandserver_get_file( cptr, filename, aptr ) );
	}
}

/*	------------------------------------------------------------------	*/
/*				f i l e g e t l i n e				*/
/*	------------------------------------------------------------------	*/
private	char *	filegetline( FILE * h, char * buffer, int bufmax )
{
	int	c;
	int	i=0;
	while (1)
	{
		switch (( c = fgetc(h) ))
		{
		case	0	:
		case	-1	:
			if (!( i )) 
				return((char *) 0);
			else	break;
		case	'\r'	:
			continue;
		case	'\n'	:
			break;
		default		:
			if ( i < bufmax )
			{
				buffer[i++] = c;
				continue;
			}
			else	return((char *) 0);
		}
		buffer[i++] = 0;
		return( buffer );
	}
}

private	int	check_boundary( char * sptr, char * rptr )
{
	while ( *sptr == '-' ) sptr++;
	while ( *rptr == '-' ) rptr++;
	return( strcmp( sptr, rptr ) );
}

/*	------------------------------------------------------------------	*/
/*			g e t _ m u l t i p a r t _ f o r m			*/
/*	------------------------------------------------------------------	*/
private	char * get_multipart_data( struct rest_header * form, char * nptr )
{
	struct	rest_header * hptr;
	for (	hptr = form;
		hptr != (struct rest_header * ) 0;
		hptr = hptr->next )
	{
		if (!( hptr->name ))
			continue;
		else if (!( strcmp( hptr->name, nptr ) ))
			return( hptr->value );
		else	continue;
	}
	return( (char *) 0) ;

}

/*	------------------------------------------------------------------	*/
/*		s c a n _ t o _ m u l t i p a r t _ w h i t e			*/
/*	------------------------------------------------------------------	*/
private	int	scan_to_multipart_white( FILE * h, char * buffer )
{
	char * sptr;
	while (1)
	{
		if (!( sptr = filegetline( h, buffer, 8192 ) ))
			return( 0 );
		else if (!( strlen( sptr ) ))
			return(1);
		else	continue;
	}
}

/*	------------------------------------------------------------------	*/
/*		c o n s u m e _ t o _ b o u n d a r y				*/
/*	------------------------------------------------------------------	*/
private	int	consume_to_boundary( FILE * h, struct rest_header * hptr, char * buffer, char * boundary )
{
	int	status=0;
	char *	sptr;
	while ((sptr = filegetline( h, buffer, 8192 )) != (char *) 0)
	{
		if (!( check_boundary( sptr, boundary ) ))
			break;
		else if (!( status ))
		{
			hptr->value = allocate_string( sptr );
			status=1;
		}
	}
	return( status );
}

/*	------------------------------------------------------------------	*/
/*			g e t _ m u l t i p a r t _ f o r m			*/
/*	------------------------------------------------------------------	*/
private	struct rest_header * get_multipart_form( struct rest_request * rptr )
{
	struct	rest_header * root=(struct rest_header *) 0;
	struct	rest_header * foot=(struct rest_header *) 0;
	struct	rest_header * hptr=(struct rest_header *) 0;

	char *	boundary;
	FILE *	h=(FILE *) 0;
	FILE *	th=(FILE*) 0;
	char	buffer[8193];
	char *	sptr;
	char *	nptr;
	char *	target;
	if (!( rptr ))
		return((struct rest_header *) 0);
	else if ( rptr->type != _FILE_BODY )
		return((struct rest_header *) 0);
	else if (!( hptr = rest_resolve_header( rptr->first, _HTTP_CONTENT_TYPE ) ))
		return((struct rest_header *) 0);
	else if (!( boundary = hptr->value ))
		return((struct rest_header *) 0);
	else if ( strncmp( boundary, "multipart/form-data; boundary=", strlen( "multipart/form-data; boundary=" )) != 0 )
		return((struct rest_header *) 0);
	else if (!( h = fopen( rptr->body , "r" ) ))
		return((struct rest_header *) 0);
	else
	{
		boundary += strlen( "multipart/form-data; boundary=" );
		boundary += strlen( "multipart/form-data; boundary=" );
		while (1)
		{
			if (!( sptr = filegetline( h, buffer, 8192 ) ))
			{
				fclose(h);
				return((struct rest_header *) 0);
			}
			else if (!( check_boundary( sptr, boundary ) ))
				break;
			else	continue;
		}
		while (1)
		{
			/* ------------------------------------ */
			/* for each multipart form data element */
			/* ------------------------------------ */
			if (!( sptr = filegetline( h, buffer, 8192 ) ))
			{
				fclose(h);
				return(root);
			}
			else if ( strncasecmp( sptr, "Content-Disposition:", strlen("Content-Disposition:" )) != 0 )
			{
				fclose(h);
				return(root);
			}
			else	sptr += strlen("Content-Disposition:" );
			while ( *sptr == ' ' ) sptr++;
			if ( strncasecmp( sptr, "form-data;", strlen("form-data;" )) != 0 )
			{
				fclose(h);
				return(root);
			}
			else	sptr += strlen("form-data;" );
			while ( *sptr == ' ' ) sptr++;


			/* ------------------------------ */
			/* detect the name of the element */
			/* ------------------------------ */
			if ( strncasecmp( sptr, "name=", strlen("name=" )) != 0 )
			{
				fclose(h);
				return(root);
			}
			else	sptr += strlen("name=" );

			/* ------------------------------------ */
			/* step over string to semicolon or end */
			/* ------------------------------------ */
			while ( *sptr == ' ' ) sptr++;
			nptr = sptr;
			while ( *sptr ) { if ( *sptr == ';' ) break; else sptr++; }
			if ( *sptr ) 
			{
				*(sptr++) = 0;
				while ( *sptr == ' ' ) sptr++;
			}
			/* ---------------------------------------- */
			/* add a multipart form element to the list */
			/* ---------------------------------------- */
			if ((!( hptr = allocate_rest_header() ))
			||  (!( hptr->name = allocate_string( nptr ) ))
			||  (!( hptr->name = occi_unquoted_value( hptr->name ) )) )
			{
				fclose(h);
				return( root );
			}
			else if (!( hptr->previous = foot ))
				root = hptr;
			else	foot->next = hptr;
			foot = hptr;

			/* ------------------------ */
			/* detect the variable name */
			/* ------------------------ */
			if (!( strcmp( hptr->name, "filename" ) ))
			{
				if ( strncasecmp( sptr, "filename=", strlen("filename=" )) != 0 )
				{
					fclose(h);
					return(root);
				}
				else	sptr += strlen("filename=" );
				if (!( sptr = allocate_string( sptr )))
				{
					fclose(h);
					return(root);
				}
				else if (!( sptr = occi_unquoted_value( sptr ) ))
				{
					fclose(h);
					return(root);
				}
				else	target = sptr;

				if (!( scan_to_multipart_white(h,buffer)))
				{
					fclose(h);
					return( root );
				}
				/* start of the target file */
				/* ------------------------ */
				else if (!( th = fopen( target, "w" ) ))
				{
					fclose(h);
					return(root);
				}
				else
				{
					while ((sptr = filegetline( h, buffer, 8192 )) != (char *) 0)
					{
						if (!( check_boundary( sptr, boundary ) ))
							break;
						else	fprintf(th,"%s\n",sptr);
					}
					fclose(th);
					if (!( hptr->value = allocate_string( target ) ))
					{
						fclose(h);
						return( root );
					}
					else	continue;
				}		
			}
			else if (!( strcmp( hptr->name, "command" ) ))
			{
				if (!( scan_to_multipart_white(h,buffer)))
				{
					fclose(h);
					return( root );
				}
				else if (!( consume_to_boundary( h, hptr, buffer, boundary ) ))
				{
					fclose(h);
					return( root );
				}
			}

			else if (!( strcmp( hptr->name, "category" ) ))
			{
				if (!( scan_to_multipart_white(h,buffer)))
				{
					fclose(h);
					return( root );
				}
				else if (!( consume_to_boundary( h, hptr, buffer, boundary ) ))
				{
					fclose(h);
					return( root );
				}
			}
			else if (!( strcmp( hptr->name, "service" ) ))
			{
				if (!( scan_to_multipart_white(h,buffer)))
				{
					fclose(h);
					return( root );
				}
				else if (!( consume_to_boundary( h, hptr, buffer, boundary ) ))
				{
					fclose(h);
					return( root );
				}
			}
			else
			{
				if (!( scan_to_multipart_white(h,buffer)))
				{
					fclose(h);
					return( root );
				}
				else if (!( consume_to_boundary( h, hptr, buffer, boundary ) ))
				{
					fclose(h);
					return( root );
				}
			}
		}
	}
}

/*	------------------------------------------------------------------	*/
/*		c o r d s _ p a r s e r _ r e s p o n s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_response * cords_parser_response( struct rest_response * aptr, char * filename )
{
	struct	rest_header * hptr;
	char	buffer[2048];
	char	filesize[256];

	sprintf(buffer,"plan_%s",filename);
	return( rest_file_response( aptr, buffer, "application/xml" ) );
}

/*	------------------------------------------------------------------	*/
/*		c o r d s _ b r o k e r _ r e s p o n s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_response * cords_broker_response( struct rest_response * aptr, char * filename )
{
	struct	rest_header * hptr;
	char	buffer[2048];
	char	filesize[256];

	return( rest_file_response( aptr, filename, "application/json" ) );
}

/*	------------------------------------------------------------------	*/
/*		c o r d s _ r e s o l v e r _ r e s p o n s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_response * cords_resolver_response( struct rest_response * aptr, char * filename )
{
	struct	rest_header * hptr;
	char	buffer[2048];
	char	filesize[256];

	return( rest_file_response( aptr, filename, "application/json" ) );
}

/*	------------------------------------------------------------------	*/
/*		c o r d s _ s e r v i c e _ r e s p o n s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_response * cords_service_response( struct rest_response * aptr, char * filename )
{
	struct	rest_header * hptr;
	char	buffer[2048];
	char	filesize[256];

	return( rest_file_response( aptr, filename, "application/json" ) );
}

/*	------------------------------------------------------------------	*/
/*		c o m m a n d s e r v e r_ p o s t 				*/
/*	------------------------------------------------------------------	*/
private	struct rest_response * invoke_rest_command(struct rest_response * aptr, struct rest_client * cptr, struct rest_request * rptr )
{
	int	status;
	char *	filename;
	char *	service;
	char *	category;
	char *	command;
	char *	sptr;
	struct	rest_header * form;
	if (!( command = allocate_string( rptr->object ) ))
		return( rest_html_response( aptr, 500, "Incorrect Request Body" ) );
	else if ( *command == '/' )
		command++;
	sptr = command;
	while( *sptr )
	{
		if ( *sptr == '/' ) break;
		else sptr++;
	}
	*(sptr++) = 0;

	if (!( strcasecmp( command, "parser" ) ))
	{
		if (!( form = get_multipart_form( rptr ) ))
			return( rest_html_response( aptr, 400, "Incorrect request" ) );
		else if (!( filename = get_multipart_data( form, "command" ) ))
			return( rest_html_response( aptr, 400, "Incorrect request" ) );
		else if ( strcmp( filename, command ) )
			return( rest_html_response( aptr, 400, "Incorrect request" ) );
		else if (!( filename = get_multipart_data( form, "filename" ) ))
			return( rest_html_response( aptr, 400, "Incorrect request" ) );
		else if ((status = cords_parser_operation( filename )) != 0)
			return( rest_html_response( aptr, status, "Incorrect request" ) );
		else	return( cords_parser_response( aptr, filename ) );
	}
	else if (!( strcasecmp( command, "broker" ) ))
	{
		if (!( form = get_multipart_form( rptr ) ))
			return( rest_html_response( aptr, 400, "Incorrect request" ) );
		else if (!( filename = get_multipart_data( form, "command" ) ))
			return( rest_html_response( aptr, 400, "Incorrect request" ) );
		else if ( strcmp( filename, command ) )
			return( rest_html_response( aptr, 400, "Incorrect request" ) );
		else if (!( filename = get_multipart_data( form, "filename" ) ))
			return( rest_html_response( aptr, 400, "Incorrect request" ) );
		else if ((status = cords_broker_operation( filename )) != 200)
			return( rest_html_response( aptr, status, "Incorrect request" ) );
		else	return( cords_broker_response( aptr, filename ) );
	}
	else if (!( strcasecmp( command, "resolver" ) ))
	{
		if (!( form = get_multipart_form( rptr ) ))
			return( rest_html_response( aptr, 400, "Incorrect request" ) );
		else if (!( filename = get_multipart_data( form, "command" ) ))
			return( rest_html_response( aptr, 400, "Incorrect request" ) );
		else if ( strcmp( filename, command ) )
			return( rest_html_response( aptr, 400, "Incorrect request" ) );
		else if (!( category = get_multipart_data( form, "category" ) ))
			return( rest_html_response( aptr, 400, "Incorrect request" ) );
		else if ((status = cords_resolver_operation( category )) != 200)
			return( rest_html_response( aptr, status, "Incorrect request" ) );
		else	return( cords_resolver_response( aptr, filename ) );
	}
	else if (!( strcasecmp( command, "service" ) ))
	{
		if (!( form = get_multipart_form( rptr ) ))
			return( rest_html_response( aptr, 400, "Incorrect request" ) );
		else if (!( command = get_multipart_data( form, "command" ) ))
			return( rest_html_response( aptr, 400, "Incorrect request" ) );
		else if (!( service = get_multipart_data( form, "service" ) ))
			return( rest_html_response( aptr, 400, "Incorrect request" ) );
		else if ((status = cords_service_operation( command, service )) != 200)
			return( rest_html_response( aptr, status, "Incorrect request" ) );
		else	return( cords_service_response( aptr, filename ) );
	}
	else if (!( strcasecmp( command, "script" ) ))
	{
		if (!( form = get_multipart_form( rptr ) ))
			return( rest_html_response( aptr, 400, "Incorrect request" ) );
		else if (!( filename = get_multipart_data( form, "command" ) ))
			return( rest_html_response( aptr, 400, "Incorrect request" ) );
		else if ( strcmp( filename, command ) )
			return( rest_html_response( aptr, 400, "Incorrect request" ) );
		else if (!( filename = get_multipart_data( form, "filename" ) ))
			return( rest_html_response( aptr, 400, "Incorrect request" ) );
		else if ((status = cords_script_interpreter( filename )) != 200)
			return( rest_html_response( aptr, status, "Incorrect request" ) );
		else	return( rest_html_response( aptr, 200, "OK" ) );
	}
	else	return( rest_html_response( aptr, 400, "Incorrect Request" ) );

}

private	struct rest_response * commandserver_post(  void * v,struct rest_client * cptr, 
		struct rest_request * rptr )
{
	struct	rest_response * aptr;
	struct	rest_header   * hptr;
	struct	xml_element   * document;
	FILE 		      * target;
	
	printf("   %s POST Request : %s %s %s \n",agent,rptr->method,rptr->object,rptr->version);
	if (!( aptr = rest_allocate_response(cptr) ))
		return( aptr );
	else if (!( rptr->body  ))
		return( rest_html_response( aptr, 404, "Expected Body" ) );
	else	return( invoke_rest_command( aptr, cptr, rptr ) );
}

/*	------------------------------------------------------------------	*/
/*		c o m m a n d s e r v e r_ p u t				*/
/*	------------------------------------------------------------------	*/	
private	struct rest_response * commandserver_put(  void * v,struct rest_client * cptr, 
		struct rest_request * rptr )
{
	struct	rest_response * aptr;
	printf("   %s PUT Request : %s %s %s \n",agent,rptr->method,rptr->object,rptr->version);
	if (!( aptr = rest_allocate_response(cptr) ))
		return( aptr );
	else if (!( rptr->body ))
		return( rest_html_response( aptr, 400, "Put Requires a Body" ) );
	else	return( rest_html_response( aptr, 400, "Incorrect Request" ) );
}

/*	------------------------------------------------------------------	*/
/*		c o m m a n d s e r v e r_ d e l e t e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_response * commandserver_delete(  void * v,struct rest_client * cptr, 
		struct rest_request * rptr )
{
	struct	rest_response * aptr;
	printf("   %s DELETE Request : %s %s %s \n",agent,rptr->method,rptr->object,rptr->version);
	if (!( aptr = rest_allocate_response(cptr) ))
		return( aptr );
	else if ( rptr->body  )
		return( rest_html_response( aptr, 400, "Unexpected Request Body" ) );
	else	return( rest_html_response( aptr, 400, "Incorrect Request" ) );
}

/*	------------------------------------------------------------------	*/
/*		c o m m a n d s e r v e r_ h e a d				*/
/*	------------------------------------------------------------------	*/
private	struct rest_response * commandserver_head(  void * v,struct rest_client * cptr, struct rest_request * rptr )
{
	struct	rest_response * aptr;
	printf("   %s HEAD Request : %s %s %s \n",agent,rptr->method,rptr->object,rptr->version);
	if (!( aptr = rest_allocate_response(cptr) ))
		return( aptr );
	else if ( rptr->body  )
		return( rest_html_response( aptr, 400, "Unexpected Request Body" ) );
	else	return( rest_html_response( aptr, 400, "Incorrect Request" ) );
}

/*	-----------------------------------	*/
/*	   c o m m a n d _ o n l i n e 		*/
/*	-----------------------------------	*/
private	int	command_online( char * aname, char * other )
{
	int	status=0;
	struct	rest_interface  CsI = 
	{
		(void *) 0,
		commandserver_initialise,
		commandserver_authorise,
		commandserver_get,
		commandserver_post,
		commandserver_put,
		commandserver_delete,
		commandserver_head,
		commandserver_extension,
		(void *) 0,
		(void *) 0,
		(void *) 0,
		(void *) 0

	};

	if (!( authorise ))
		CsI.authorise = (void *) 0;

	return( rest_server(  (agent=aname), port, tls, threads, & CsI ) );
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
			else if (!( strcasecmp( command, "PARSER" ) ))
				return( command_parser( aptr, argv[argi] ) );
			else if (!( strcasecmp( command, "BROKER" ) ))
				return( command_broker( aptr, argv[argi] ) );
			else if (!( strcasecmp( command, "ONLINE" ) ))
				return( command_online( aptr, argv[argi] ) );
			else if (!( strcasecmp( command, "INVOKE" ) ))
				return( invoke_action( aptr, argv[argi] ) );
			else if (!( strcasecmp( command, "ANALYSE" ) ))
				return( colog_analysis( aptr ) );
			else if (!( strcasecmp( command, "RUN" ) ))
				return( run_cordscript_interpreter( aptr, argc-argi, &argv[argi] ) );
			else if (!( strcasecmp( command, "JSON" ) ))
			{
				if (!( run_json_verification( aptr ) ))
					continue;
				else	break;
			}
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
				else if (!( strcmp( aptr, "port" ) ))
					port = atoi(argv[argi++]);
				else if (!( strcmp( aptr, "threads" ) ))
					threads = atoi(argv[argi++]);
				else if (!( strcmp( aptr, "tls" ) ))
					tls = argv[argi++];
				else if (!( strcmp( aptr, "user" ) ))
					user = argv[argi++];
				else if (!( strcmp( aptr, "password" ) ))
					password = argv[argi++];
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
	printf("\n   CompatibleOne Command Line Tool : Version 1.0c.0.01");
	printf("\n   Beta Version : 12/06/2013 ");
	printf("\n   Copyright (c) 2011,2013 Iain James Marshall ");
	printf("\n   Usage : ");
	printf("\n         command <options> PARSER      <xml_file> ");
	printf("\n         command <options> BROKER      <xml_file> ");
	printf("\n         command <options> ONLINE      <hostinfo> ");
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
	printf("\n         command <options> JSON        <script> ");
	printf("\n         command <options> TRANSACTION <account> <price> <reference> <action> <description> ");
	printf("\n   Options: ");
	printf("\n         --publisher <publisher>      specify publisher identity ");
	printf("\n         --agent     <agent>          specify agent identity ");
	printf("\n         --tls       <security>       specify security configuration ");
	printf("\n         --verbose                    activate verbose messages");
	printf("\n         --noauth                     inhibit authentication for test purposes");
	printf("\n         --echo                       activate source echo ");
	printf("\n         --port <number>              set online port number \n");
	printf("\n         --user <name>                set online user name \n");
	printf("\n         --paasword <value>           set online password \n");
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
