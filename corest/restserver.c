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
#ifndef	_restserver_c
#define	_restserver_c

#include "rest.h"
#include "restx.c"
#include "restc.c"
#include "restworkers.c"
#include "rests.c"
#include "restheader.c"
#include "restrequest.c"
#include "restresponse.c"
#include "restlog.h"
#include "tlsload.h"

public	char *	default_tls();
private	struct	rest_server * rest_liberate_server( struct rest_server * rptr );
private	struct	rest_server * rest_allocate_server();
private	struct	rest_client * rest_new_client(struct rest_server * sptr);
public 	struct	rest_client * rest_drop_client( struct rest_client * rptr );
private	struct	rest_response * rest_incorrect_method(void * vptr, struct rest_client * cptr, struct rest_request * rptr );

/*	-----------------------------------------------------------	*/
/*		   l o c k _ r e s t _ s e r v e r			*/
/*	-----------------------------------------------------------	*/
private	void	lock_rest_server(struct rest_server * tptr)
{
	pthread_mutex_lock( &tptr->lock );
	return;
}

/*	-----------------------------------------------------------	*/
/*		u n l o c k _ r e s t _ s e r v e r			*/
/*	-----------------------------------------------------------	*/
private	void	unlock_rest_server(struct rest_server * tptr)
{
	pthread_mutex_unlock( &tptr->lock );
	return;
}

/*	------------------------------------------------	*/
/*	     r e s t _ t h r e a d _ c o n t r o l		*/
/*	------------------------------------------------	*/
/*	called by encapsulating process to establish the	*/
/*	conditions of the rest server in terms of thread	*/
/*	usage and management.					*/
/*	------------------------------------------------	*/
private	int	thread_control=0;
public	int	rest_thread_control(int new_value)
{
	int	before=thread_control;
	thread_control = new_value;
	return( before );
}

/*	------------------------------------------------	*/
/*		r e s t _ v a l i d _ s t r i n g		*/
/*	------------------------------------------------	*/
public	int	rest_valid_string( char * vptr )
{
	if (!( vptr ))
		return( 0 );
	else if (!( strlen( vptr ) ))
		return( 0 );
	else if (!( strcmp( vptr, "(null)" ) ))
		return( 0 );
	else if (!( strcmp( vptr, "null" ) ))
		return( 0 );
	else if (!( strcmp( vptr, "(none)" ) ))
		return( 0 );
	else	return( 1 );
}

/*	------------------------------------------------	*/
/*	 r e s t _ v a l i d _ s t r i n g _ v a l u e 		*/
/*	------------------------------------------------	*/
public	int	rest_valid_string_value( char * sptr, char *vptr  )
{
	if (!( rest_valid_string( sptr ) ))
		return( 0 );
	else if (!( vptr ))
		return( 0 );
	else if (!( strcasecmp( sptr, vptr ) ))
		return( 1 );
	else	return( 0 );
}

/*	---------------------------------------------------	*/
/*		r e s t _ n o r m a l i s e _ v a l u e 	*/
/*	---------------------------------------------------	*/
/*	this function takes a value string as its parameter	*/
/*	which may or not be terminated by a T,G,M,K which	*/
/*	indicates an explicite quantity. A default quantity	*/
/*	type is passed as the second parameter to be used	*/
/*	in case no explicite value is present.			*/
/*	The function returns the normalisation of the value	*/
/*	---------------------------------------------------	*/
public	int	rest_normalise_value( char * sptr, int normal )
{
	int	factor=1;
	int	value=0;

	if ( normal == 'T' )
		factor = 1000000000;
	else if ( normal == 'G' )
		factor = 1000000;
	else if ( normal == 'M' )
		factor = 1000;
	else if ( normal == 'K' )
		factor = 1;
	else if ( normal == 'U' )
		factor = 1;
	else if ( normal == 0x0025 )
		factor = 1;
	else	factor = 1;

	value = atoi(sptr);

	while ( (( *sptr >= '0' ) && ( *sptr <= '9' )) || ( *sptr == '.') || (*sptr == ' ')) sptr++;

	if (( *sptr == 'T' ) || ( *sptr == 't' ))
		factor = 1000000000;
	else if (( *sptr == 'G' ) || ( *sptr == 'g' ))
		factor = 1000000;
	else if (( *sptr == 'M' ) || ( *sptr == 'm' ))
		factor = 1000;
	else if (( *sptr == 'K' ) || ( *sptr == 'k' ))
		factor = 1;
	else if (( *sptr == 'U' ) || ( *sptr == 'u' ))
		factor = 1;
	else if ( *sptr == 0x0025 )
		factor = 1;

	return( value * factor );
}

/*	---------------------------------------------------	*/
/*		r e s t _ r e d u c e d _ v a l u e 		*/
/*	---------------------------------------------------	*/
public	int	rest_reduced_value( int value , int normal )
{
	int	factor=1;

	if ( normal == 'T' )
		factor = 1000000000;
	else if ( normal == 'G' )
		factor = 1000000;
	else if ( normal == 'M' )
		factor = 1000;
	else if ( normal == 'K' )
		factor = 1;
	else if ( normal == 'U' )
		factor = 1;
	else if ( normal == 0x0025 )
		factor = 1;
	else	factor = 1;

	return( value / factor );
}

/*	------------------------------------------------	*/
/*		r e s t _ h t t p _ p r e f i x 		*/
/*	------------------------------------------------	*/
public	char *	rest_http_prefix()
{
	char *	prefix;
	if (!( prefix = default_tls() ))
		prefix = "http";
	else if (!( rest_valid_string( prefix ) ))
		prefix = "http";
	else	prefix = "https";
	return( prefix );
} 

/*	------------------------------------------------	*/
/*	    r e s t _ a d d _ h t t p _ p r e f i x 		*/
/*	------------------------------------------------	*/
public	void	rest_add_http_prefix(char * buffer, int buflen, char * host )
{
	if (!( strncmp(host,"http:",strlen("http:")) ))
		strcpy(buffer,host);
	else if (!( strncmp(host,"https:",strlen("https:")) ))
		strcpy(buffer,host);
	else	sprintf(buffer,"%s://%s",rest_http_prefix(),host);
	return;
}

/*	------------------------------------------------	*/
/*		r e s t _ o p e n _ s e r v e r			*/
/*	------------------------------------------------	*/
private	struct rest_server * 	rest_open_server( int port, char * tls, int max, struct rest_interface * iptr )	
{
	struct rest_server *  sptr;
 
	rest_debug_message( "rest_open_server", _DEBUG_SERVER);

	if (!( port ))
		return((struct rest_server*) 0); 
	else if (!( sptr = rest_allocate_server() ))
	{
		failure(27,"rest","allocate server");
		return( sptr );
	}
	else if ((sptr->net.socket = socket_create(get_socket_type(), SOCK_STREAM, 0  )) < 0)
	{
 	        failure(errno,"socket_create",strerror(errno));
		return( rest_liberate_server( sptr ) );
	}
	else if (!( socket_listen( sptr->net.socket, port, tcpmax ) ))
	{
	        failure(errno,"socket_listen",strerror(errno));
		return( rest_liberate_server( sptr ) );
	}
	else
	{
		if ( tls )
		{
			if (!( sptr->tlsconf = tls_configuration_load( tls ) ))
			{
				failure(errno,"tls configuration","errno");
				return( rest_liberate_server( sptr ) );
			}
			tls_configuration_use( sptr->tlsconf );
			tls_server_startup( &sptr->net, sptr->tlsconf->option );
		}
		sptr->port = port;
		sptr->seconds = 60;
		sptr->wakeup  = 5000;
		sptr->fraction = 0;
		if ( iptr->security )
			sptr->method.security = iptr->security;
		else	sptr->method.security = 0;
		if ( iptr->instance )
			sptr->method.instance = iptr->instance;
		else	sptr->method.instance = 0;
		if ( iptr->initialise )
			sptr->method.initialise = iptr->initialise;
		else	sptr->method.initialise = 0;
		if ( iptr->authorise )
			sptr->method.authorise = iptr->authorise;
		else	sptr->method.authorise = 0;
		if ( iptr->get )
			sptr->method.get = iptr->get;
		else	sptr->method.get = rest_incorrect_method;
		if ( iptr->post )
			sptr->method.post = iptr->post;
		else	sptr->method.post = rest_incorrect_method;
		if ( iptr->put )
			sptr->method.put = iptr->put;
		else	sptr->method.put = rest_incorrect_method;
		if ( iptr->delete )
			sptr->method.delete = iptr->delete;
		else	sptr->method.delete = rest_incorrect_method;
		if ( iptr->head)
			sptr->method.head = iptr->head;
		else	sptr->method.head = rest_incorrect_method;
		if ( iptr->extension )
			sptr->method.extension = iptr->extension;
		else	sptr->method.extension = 0;
		if ( iptr->before )
			sptr->method.before = iptr->before;
		else	sptr->method.before = 0;
		if ( iptr->after )
			sptr->method.after = iptr->after;
		else	sptr->method.after = 0;
		if ( iptr->alert )
			sptr->method.alert = iptr->alert;
		else	sptr->method.alert = 0;
		return( sptr );
		}
}

/*	------------------------------------------------	*/
/*	     r e s t _ c  l i e n t _ w r i t e			*/
/*	------------------------------------------------	*/
public	int	rest_client_write( struct rest_client * cptr )
{
	return ( connection_write( &cptr->net, cptr->buffer, cptr->bytes) );
}

/*	------------------------------------------------	*/
/*	     r e s t _ c  l i e n t _ r e a d       		*/
/*	------------------------------------------------	*/
private	int	rest_client_read( struct rest_client * cptr )
{
	cptr->consumed = 0;
	return (
		(cptr->bytes = connection_read( &cptr->net, cptr->buffer, cptr->buffersize) )
		);
}

/*	------------------------------------------------	*/
/*	   r e s t _ r e s p o n s e _ s t a r t
/*	------------------------------------------------	*/
private	void rest_response_start( struct rest_client * cptr )
{
	cptr->bytes = cptr->consumed = 0;
	return;
}

/*	------------------------------------------------	*/
/*	   r e s t _ r e s p o n s e _ f l u s h 
/*	------------------------------------------------	*/
private	void rest_response_flush( struct rest_client * cptr )
{
	if ( cptr->bytes )
	{
		rest_client_write( cptr );
		rest_response_start( cptr );
	}
	return;
}

/*	------------------------------------------------	*/
/*	   r e s t _ r e s p o n s e _ s t r i n g		*/
/*	------------------------------------------------	*/
private	void rest_response_string( struct rest_client * cptr, char * message, char * term )
{
	int	bytes;
	if (!( cptr ))
		return;
	else if ((!( message )) || (!( term )))
		return;
	else	bytes = strlen(message) + strlen(term);

	if ( ( cptr->bytes + bytes ) >= cptr->buffersize )
		rest_response_flush( cptr );
	sprintf(cptr->buffer+cptr->bytes,"%s%s",message,term);
	cptr->bytes += bytes;
	return;
}

/*	------------------------------------------------	*/
/*	   r e s t _ r e s p o n s e _ v a l u e		*/
/*	------------------------------------------------	*/
private	void rest_response_value(  struct rest_client * cptr, int value, char * term )
{
	char	buffer[64];
	sprintf(buffer,"%u",value);
	rest_response_string(cptr, buffer, term);
	return;
}

/*	------------------------------------------------	*/
/*	   r e s t _ r e s p o n s e _ b o d y  		*/
/*	------------------------------------------------	*/
public	void rest_response_body(  struct rest_response * aptr, char * body, int type )
{
	if ( aptr )
	{
		aptr->body = body;
		aptr->type = type;
	}
	return;
}

/*	------------------------------------------------	*/
/*		r e s t _ r e q u e s t _ b o d y  		*/
/*	------------------------------------------------	*/
public 	void rest_request_body(  struct rest_request * rptr, char * body, int type )
{
	if ( rptr )
	{
		rptr->body = body;
		rptr->type = type;
	}
	return;
}

/*	------------------------------------------------	*/
/*	   r e s t _ r e s p o n s e _ f i l e  		*/
/*	------------------------------------------------	*/
/*	the server will prepare a file for sending over		*/
/*	to the client in the body of the message.		*/
/*	------------------------------------------------	*/
private	void rest_response_file(  struct rest_client * cptr, char * filename )
{
	FILE * 	h;
	int	bytes=0;
	if (!( h = fopen( filename,"rb" ) ))
		return;
	{
		while (1)
		{
			if ( cptr->bytes < cptr->buffersize )
			{
				if (( bytes = fread(cptr->buffer+cptr->bytes,1,(cptr->buffersize-cptr->bytes),h)) > 0 )
					cptr->bytes += bytes;
				else	break;
			}
			else	rest_response_flush( cptr );
		}
		fclose(h);
	}
	return;
}

/*	---------------------------------------------------	*/
/*		r e s t _ r e p l a c e _ h e a d e r		*/
/*	---------------------------------------------------	*/
public	void	rest_replace_header( struct rest_header * hptr, char * vptr )
{
	if (!( hptr ))
		return;
	else if (!( vptr ))
		return;
	else
	{
		if ( hptr->value )
			hptr->value = liberate( hptr->value );
		hptr->value = allocate_string( vptr );
		return;
	}
}

/*	------------------------------------------------	*/
/*	   	r e s t _ s h o w _ r e q u e s t 		*/
/*	------------------------------------------------	*/
private	void	rest_show_request( struct rest_request * rptr)
{
	struct	rest_header * hptr;
	if ( check_verbose() )
	{
		printf("   Rest Server Request  : %s %s %s \n",rptr->method, rptr->object, rptr->version );
		if ( rest_check_debug() )
		{
			for (	hptr=rptr->first;
				hptr != (struct rest_header *) 0;
				hptr = hptr->next )
				if ( hptr->name )
					printf("   %s: %s",hptr->name,hptr->value);
		}
	}
	return;
}

/*	------------------------------------------------	*/
/*	   	r e s t _ s h o w _ r e s p o n s e 		*/
/*	------------------------------------------------	*/
private	void	rest_show_response( struct rest_response * aptr)
{
	struct	rest_header * hptr;
	if ( check_verbose() )
	{
		printf("   Rest Server Response : %s %u %s \n",aptr->version, aptr->status,aptr->message );
		if ( rest_check_debug() )
		{
			for (	hptr=aptr->first;
				hptr != (struct rest_header *) 0;
				hptr = hptr->next )
				if ( hptr->name )
					printf("   %s: %s \n",hptr->name,hptr->value);
		}
	}
	return;
}

/*	------------------------------------------------	*/
/*	   l i b e r a t e _ r e s t _ h e a d e r s		*/
/*	------------------------------------------------	*/
public	struct	rest_header * liberate_rest_headers( struct rest_header * hptr )
{
	struct	rest_header * sptr;
	while ((sptr = hptr) != (struct rest_header *) 0)
	{
		hptr = hptr->next;
		sptr = liberate_rest_header( sptr );
	}
	return((struct rest_header *) 0);
}

/*	------------------------------------------------	*/
/*	   r e s t _ t r a n s m i t _ r e s p o n s e 		*/
/*	------------------------------------------------	*/
private	struct rest_response * rest_transmit_response(
		struct rest_client  * cptr, 
		struct rest_response* aptr,
		struct rest_request * rptr)
{
	struct	rest_header * hptr;

	if (!( cptr ))	return( aptr );
	if (!( aptr ))	return( aptr );

	aptr->request = rptr;

	/* launch an alert if required and possible */
	/* ---------------------------------------- */
	if ( is_alert_status( aptr->status ) )
		if ( cptr->server != (struct rest_server *) 0)
			if ( cptr->server->method.alert )
				(void) (*cptr->server->method.alert )
				( cptr->server->method.instance, cptr, aptr,NULL,
				aptr->status, aptr->message, "ALERT","REST",default_tls());


	rest_log_send_response( cptr->server->name, aptr );
	rest_show_response( aptr );
	rest_response_start( cptr );

	/* Set CORS headers*/
	rest_set_cors( aptr, rptr );

	rest_response_string( cptr, aptr->version, " ");
	rest_response_value( cptr, aptr->status, " ");
	rest_response_string( cptr, aptr->message, "\r\n");

	if ( cptr->server )
	{
		for ( 	hptr = cptr->server->headers;
			hptr != (struct rest_header *) 0;
			hptr = hptr->next )
		{
			if ( hptr->name )
			{
				rest_response_string( cptr, hptr->name   , ": ");
				rest_response_string( cptr, hptr->value  , "\r\n");
			}
		}
	}

	for ( 	hptr = aptr->first;
		hptr != (struct rest_header *) 0;
		hptr = hptr->next )
	{
		if ( hptr->name )
		{
			rest_response_string( cptr, hptr->name   , ": ");
			rest_response_string( cptr, hptr->value  , "\r\n");
		}
	}

	rest_response_string( cptr, "\0" , "\r\n");

	if ( aptr->body )
	{
		switch ( aptr->type )
		{
		case	_FILE_BODY	:
			rest_response_file( cptr, aptr->body );		
			if (!( check_debug() ))
			{
				unlink( aptr->body );
			}
			break;
		case	_TEXT_BODY	:
			rest_response_string( cptr, aptr->body, "\0" );		
			break;
		}
	}

	rest_response_flush( cptr );
	return( aptr );
}


/*	------------------------------------------------	*/
/*	   r e s t _ s e t _ c o r s            		*/
/*	------------------------------------------------	*/
public	void rest_set_cors(
		struct rest_response* aptr,
		struct rest_request * rptr)
{  
  struct rest_header * hptr;

  printf("Check Origin header\n");
  if ( hptr = rest_resolve_header( rptr->first, _HTTP_ORIGIN ) )
    {
      printf("Found  Origin header: %s\n", hptr->value);
      rest_response_header( aptr, _HTTP_CORS_ORIGIN, hptr->value );
    }
}

/*	-----------------------------------------------------	*/
/*	l i b e r a t e _ r e s t _ r e s p o n s e _ b o d y	*/
/*	-----------------------------------------------------	*/
public	struct	rest_response * liberate_rest_response_body( struct rest_response * rptr )
{
	char *	filename;
	if (rptr->type == _FILE_BODY)
		filename = allocate_string(rptr->body);
	else	filename = (char *) 0;
	liberate_rest_response( rptr );
	if (!( check_debug() ))
	{
		if ( filename )
		{
			unlink( filename );
			liberate( filename );
		}
	}
	return((struct rest_response *) 0);	
}

/*	------------------------------------------------	*/
/*	   r e s t _ r e s p o n s e _ s t a t u s 		*/
/*	------------------------------------------------	*/
public	struct rest_response * rest_response_status(struct rest_response* aptr,int	status, char * message )
{
	if (!( aptr ))
		return( aptr );
	else if (!( aptr->message = allocate_string( message ) ))
		return( aptr );
	else 
	{
		aptr->status = status;
		return( aptr );
	}
}

/*	------------------------------------------------	*/
/*	     r e s t _ c r e a t e _ h e a d e r 		*/
/*	------------------------------------------------	*/
public	struct	rest_header * rest_create_header( char * nptr, char * vptr )
{
	struct	rest_header * hptr;
	if (!( hptr = allocate_rest_header()))
		return( hptr );
	else if (!( hptr->name = allocate_string( nptr ) ))
		return( liberate_rest_header( hptr ) );
	else if (!( hptr->value = allocate_string( vptr ) ))
		return( liberate_rest_header( hptr ) );
	else	return( hptr );
}

/*	------------------------------------------------	*/
/*	     r e s t _ p r e f i x _ h e a d e r 		*/
/*	------------------------------------------------	*/
public struct rest_header * rest_prefix_header( struct rest_header * root, char * nptr, char * vptr )
{
	struct	rest_header * hptr;
	if (!( hptr = rest_create_header( nptr, vptr ) ))
		return( hptr );
	else if (!( hptr->next = root ))
		return( hptr );
	else	return(( root->previous = hptr ));
}

/*	------------------------------------------------	*/
/*	     r e s t _ a p p e n d _ h e a d e r 		*/
/*	------------------------------------------------	*/
public struct rest_header * rest_postfix_header( struct rest_header * foot, char * nptr, char * vptr )
{
	struct	rest_header * hptr;
	if (!( hptr = rest_create_header( nptr, vptr ) ))
		return( hptr );
	else if (!( hptr->previous = foot ))
		return( hptr );
	else	return(( foot->next = hptr ));

}

/*	------------------------------------------------	*/
/*	  r e s t _ d u p l i c a t e _ h e a d e r s 		*/
/*	------------------------------------------------	*/
public	struct	rest_header * rest_duplicate_headers( struct rest_header * hptr )
{
	struct	rest_header * root=(struct rest_header *) 0;
	struct	rest_header * foot=(struct rest_header *) 0;
	struct	rest_header * wptr=(struct rest_header *) 0;
	for (	;
		hptr != (struct rest_header *) 0;
		hptr = hptr->next )
	{
		if (!( hptr->name ))
			continue;
		else if (!( wptr = rest_create_header( hptr->name, hptr->value ) ))
			break;
		else if (!( wptr->previous = foot ))
			root = wptr;
		else	wptr->previous->next = wptr;
		foot = wptr;
	}
	return( root );
}

/*	------------------------------------------------	*/
/*	   r e s t _ r e s p o n s e _ h e a d e r 		*/
/*	------------------------------------------------	*/
public	struct	rest_header * rest_response_header(struct rest_response * aptr, char * nptr, char * vptr )
{
	struct	rest_header * hptr;
	if (!( hptr = rest_create_header( nptr, vptr ) ))
		return( hptr );
	else if (!( hptr->name = allocate_string( nptr ) ))
		return( liberate_rest_header( hptr ) );
	else if (!( hptr->value = allocate_string( vptr ) ))
		return( liberate_rest_header( hptr ) );
	else
	{
		if (!( hptr->previous = aptr->last ))
			aptr->first = hptr;
		else 	hptr->previous->next = hptr;
		aptr->last = hptr;
		return( hptr );
	}
}

/*	------------------------------------------------	*/
/*	   r e s t _ r e q u e s t _ h e a d e r 		*/
/*	------------------------------------------------	*/
public	struct	rest_header * rest_request_header(struct rest_request * rptr, char * nptr, char * vptr )
{
	struct	rest_header * hptr;
	if (!( hptr = rest_create_header( nptr, vptr ) ))
		return( hptr );
	else if (!( hptr->name = allocate_string( nptr ) ))
		return( liberate_rest_header( hptr ) );
	else if (!( hptr->value = allocate_string( vptr ) ))
		return( liberate_rest_header( hptr ) );
	else
	{
		if (!( hptr->previous = rptr->last ))
			rptr->first = hptr;
		else 	hptr->previous->next = hptr;
		rptr->last = hptr;
		return( hptr );
	}
}

/*	------------------------------------------------	*/
/*		a d d _ r e s p o n s e _ h e a d e r 		*/
/*	------------------------------------------------	*/
public	struct	rest_header * add_response_header(struct rest_response * aptr )
{
	struct	rest_header * hptr;
	if (!( hptr = allocate_rest_header()))
		return( hptr );
	else
	{
		if (!( hptr->previous = aptr->last ))
			aptr->first = hptr;
		else 	hptr->previous->next = hptr;
		aptr->last = hptr;
		return( hptr );
	}
}

/*	------------------------------------------------	*/
/*	   r e s t _ r e q u e s t _ h o s t 			*/
/*	------------------------------------------------	*/
public	char * 	rest_request_host( struct rest_request * rptr )
{
	char 	buffer[2048];
	struct	rest_header * hptr;
	if (!( rptr ))
		return((char *) 0);
	else if ( rptr->host )
		return( rptr->host );
	else if (!( hptr = rest_resolve_header( rptr->first, _HTTP_HOST ) ))
		return((char *) 0);
	else if (!( hptr->value ))
		return((char *) 0);
	else
	{
		rest_add_http_prefix(buffer,2048,hptr->value);
		return( (rptr->host = allocate_string( buffer ) ) );
	}
}

/*	------------------------------------------------	*/
/*	   r e s t _ a l l o c a t e _ r e s p o n s e		*/
/*	------------------------------------------------	*/
public	struct	rest_response * rest_allocate_response(struct rest_client * cptr)
{
	struct	rest_server * sptr;
	struct 	rest_response* aptr;
	struct	rest_header * hptr;
	if (!( cptr ))
		return(allocate_rest_response());
	if (!( sptr = cptr->server ))
		return(allocate_rest_response());
	else if (!( aptr = allocate_rest_response() ))
		return( aptr );
	else if (!( hptr = rest_response_header( aptr, _HTTP_SERVER, sptr->name )))
		return( aptr );
	else	return( aptr );
}

/*	------------------------------------------------	*/
/*	   r e s t _ l i b e r a t e _ r e s p o n s e		*/
/*	------------------------------------------------	*/
public	struct	rest_response *	rest_liberate_response( struct rest_response * aptr)
{
	if ( aptr )
	{
		liberate_rest_response_body( aptr );
	}
	return((struct rest_response *) 0);
}		

/*	------------------------------------------------	*/
/*		r e s t _ h t m l _ f a i l u r e		*/
/*	------------------------------------------------	*/
private	char * rest_html_failure( int status, char * message )
{
	char 	buffer[8192];
	char * bptr=buffer;
	sprintf(buffer,"<html><body><div align=center><table width='80%c' border=1>",0x25);
	sprintf(bptr+strlen(bptr),"<tr><th colspan=2>Failure<tr><th>%u<th>%s</table></div></body></html>",
		status,message);
	return( allocate_string( buffer ) );
}

/*	------------------------------------------------	*/
/*	   r e s t _ h t m l _ r e s p o n s e 			*/
/*	------------------------------------------------	*/
public	struct rest_response * rest_file_response( struct rest_response  * aptr, char * filename, char * type )
{
	struct	stat info;
	char	buffer[64];
	struct	rest_header * hptr;

	if (!( filename ))
		return( aptr );
	else if (!( filename = allocate_string( filename ) ))
		return( aptr );

	if (!( aptr ))
		if (!( aptr = allocate_rest_response() ))
			return( aptr );
	if ( aptr->message )
		aptr->message = liberate( aptr->message );

	if (!( aptr->message = allocate_string( "OK" ) ))
		return( aptr );
	else	aptr->status = 200;

	if (!( aptr->version ))
		if (!( aptr->version = allocate_string("HTTP/1.1") ))
			return( aptr );

	if (!( hptr = rest_response_header( aptr, _HTTP_CONTENT_TYPE, type ) ))
		return( aptr );
	else if ( stat( filename,&info ) < 0 )
		return( aptr );
	else
	{
		sprintf(buffer,"%u",info.st_size);
		if (!( hptr = rest_response_header( aptr, _HTTP_CONTENT_LENGTH, buffer ) ))
			return( aptr );
		else 
		{
			rest_response_body( aptr, filename, _FILE_BODY );
			return( aptr );
		}
	}
}

/*	------------------------------------------------	*/
/*	   r e s t _ h t m l _ r e s p o n s e 			*/
/*	------------------------------------------------	*/
public	struct rest_response * rest_html_response( struct rest_response  * aptr, int status, char * message )
{
	struct	rest_header * hptr;
	char 	buffer[64];
	char *	htmlbody;

	if (!( aptr ))
		if (!( aptr = allocate_rest_response() ))
			return( aptr );

	if ( aptr->message )
		aptr->message = liberate( aptr->message );

	if (!( aptr->message = allocate_string( message ) ))
		return( aptr );
	else	aptr->status = status;

	if (!( aptr->version ))
		if (!( aptr->version = allocate_string("HTTP/1.1") ))
			return( aptr );

	if (!( hptr = rest_response_header( aptr, _HTTP_CONTENT_TYPE, "text/html" ) ))
		return( aptr );

	else if (!( htmlbody = rest_html_failure( status, message ) ))
		return( aptr );
	else
	{
		sprintf(buffer,"%u",strlen( htmlbody ));
		if (!( hptr = rest_response_header( aptr, _HTTP_CONTENT_LENGTH, buffer ) ))
			return( aptr );
		else 
		{
			rest_response_body( aptr, htmlbody, _TEXT_BODY );
			return( aptr );
		}
	}

}

/*	---------------------------------------------------	*/
/*		r e s t _ b a d _ r e q u e s t		*/
/*	---------------------------------------------------	*/
public	struct rest_response * rest_bad_request(void * vptr, struct rest_client * cptr,struct rest_request * rptr)
{
	struct	rest_response * aptr;
	if (!( aptr = rest_allocate_response( cptr ) ))
		return( aptr );
	else 	return( rest_html_response( aptr, 400, "Bad Request" ) );
}

/*	------------------------------------------------	*/
/*	   r e s t _ i n c o r r e c t _ m e t h o d		*/
/*	------------------------------------------------	*/
public	struct	rest_response * rest_incorrect_method(void * vptr,struct rest_client * cptr, struct rest_request * rptr)
{
	struct 	rest_response* aptr;
	if (!( aptr = rest_allocate_response(cptr) ))
		return( aptr );
	else 	return( rest_html_response( aptr, 405, "Incorrect Method" ) );
}

/*	------------------------------------------------	*/
/*	   r e s t _ r e s p o n s e _ f a i l u r e		*/
/*	------------------------------------------------	*/
public	struct rest_response * rest_response_failure(struct rest_client * cptr, int status, char * message )
{
	struct 	rest_response* aptr;
	if (!( aptr = rest_allocate_response(cptr) ))
		return( aptr );
	else if (!( aptr = rest_response_status( aptr, status, message )))
		return( aptr );
	else	return( rest_html_response( aptr, status, message ) );
}

/*	---------------------------------------------------------	*/
/*	r e s t _ a u t h e n t i c a t i o n _ c h a l l e n g e 
/*	---------------------------------------------------------	*/
public	struct rest_response * rest_authentication_challenge(struct rest_client * cptr, int status, char * message )
{
	struct 	rest_response * aptr;
	struct	rest_header   * hptr;
	if (!( aptr = rest_allocate_response(cptr) ))
		return( aptr );
	else if (!( aptr = rest_response_status( aptr, status, message )))
		return( aptr );
	else if (!( hptr = rest_response_header( aptr, "WWW-Authenticate","BASIC realm=*" ) ))
		return( aptr );	
	else	return( aptr );
}		

/*	---------------------------------------------------------	*/
/*		r e s t _ e n c o d e _ h t m l 			*/
/*	---------------------------------------------------------	*/
public	char *	rest_encode_html( char * sptr )
{
	char *	rptr;
	char *	wptr;
	int	c;
	if (!( sptr ))
		return( sptr );
	else if (!( wptr = allocate( strlen( sptr ) * 3 ) ))
		return( wptr );
	else	rptr = wptr;

	while( *sptr )
	{
		if (!( c = ( *(sptr++) & 0x00FF ) ))
			break;
		else if (( c >= '0' ) && ( c <= '9' ))
			*(wptr++) = c;
		else if (( c >= 'a' ) && ( c <= 'z' ))
			*(wptr++) = c;
		else if (( c >= 'A' ) && ( c <= 'Z' ))
			*(wptr++) = c;
		else if ( c == ' ' )
			*(wptr++) = '+';
		else if (( c == '.' ) || ( c == '_' ) || ( c == '-' ))
			*(wptr++) = c;
		else 
		{
			sprintf(wptr, "%c%02.2X",'%',c);
			wptr += strlen(wptr);
		}
	}
	*(wptr++) = 0;
	wptr = allocate_string( rptr );
	liberate( rptr );
	return( wptr );
}

/*	---------------------------------------------------------	*/
/*		r e s t _ d e c o d e _ h t m l 			*/
/*	---------------------------------------------------------	*/
public	char *	rest_decode_html( char * sptr )
{
	char *	rptr;
	char *	wptr;
	int	c;
	int	x;
	if (!( sptr ))
		return( sptr );
	else if (!( wptr = allocate_string( sptr ) ))
		return( wptr );
	else	rptr = wptr;

	while( *sptr )
	{
		if (!( c = ( *(sptr++) & 0x00FF ) ))
			break;
		else if ( c == '+' )
			*(wptr++) = ' ';
		else if ( c != '%' )
			*(wptr++) = c;
		else
		{
			c = *(sptr++);
			if ((c >= '0') && (c <= '9'))
				x = ((c - '0') * 16);
			else if ((c >= 'a') && (c <= 'f'))
				x = (((c - 'a') + 10) * 16);
			else if ((c >= 'A') && (c <= 'F'))
				x = (((c - 'A') + 10) * 16);
			c = *(sptr++);
			if ((c >= '0') && (c <= '9'))
				x += (c - '0');
			else if ((c >= 'a') && (c <= 'f'))
				x += ((c - 'a') + 10);
			else if ((c >= 'A') && (c <= 'F'))
				x += ((c - 'A') + 10);
			*(wptr++) = x;
		}
	}
	*(wptr++) = 0;
	wptr = allocate_string( rptr );
	liberate( rptr );
	return( wptr );
}

/*	---------------------------------------------------------	*/
/*		r e s t _ e n c o d e _ c r e d e n t i a l s		*/
/*	---------------------------------------------------------	*/
public	char *	rest_encode_credentials( char * username, char * password )
{
	char 	buffer[2048];
	char	work[1024]; 
	char	bwork[2048]; 
	strcpy( work, username);
	strcat( work, ":");
	strcat( work, password);
	EncodeBase64( bwork, work, strlen(work));
	sprintf(buffer,"BASIC %s",bwork);
	return( allocate_string( buffer ));
}

/*	---------------------------------------------------------	*/
/*		r e s t _ d e c o d e _ c r e d e n t i a l s		*/
/*	---------------------------------------------------------	*/
public	int	rest_decode_credentials( char * vptr, char * username, char * password )
{
	char	buffer[1024];
	char	httpAuth[1024];
	int	bytes=0;
	int	nature;
	int	value;

	strcpy(httpAuth,vptr);

	/* step over leading space */
	for (	nature=0;
		httpAuth[nature] == ' ';
		nature++);
	/* scan to space */
	for (	value=nature;
		httpAuth[value] != ' ';
		value++)
		if (!( httpAuth[value] ))
			break;
	/* terminate here for now */
	if ( httpAuth[value] )
		httpAuth[value++] = 0;
	/* step over space */
	for (	;
		httpAuth[value] == ' ';
		value++);
	/* ensure BASIC scheme */
	if (!( strcasecmp( &httpAuth[nature], "BASIC" )))
	{
		DecodeBase64( &httpAuth[value], buffer, &bytes );
		buffer[bytes] = 0;
		strcpy( httpAuth,buffer );
		if (!( strlen( buffer ) ))
			return(0);
		for (	nature=0;
			httpAuth[nature] == ' ';
			nature++);
		for (	value=nature;
			httpAuth[value] != ':';
			value++)
			if (!( httpAuth[value] ))
				break;
		if ( httpAuth[value] )
			httpAuth[value++] = 0;
		for (	;
			httpAuth[value] == ' ';
			value++);
		strcpy( username, &httpAuth[nature]);
		strcpy( password, &httpAuth[value]);
		return( 1);
	}
	else	return(0);
}

/*	------------------------------------------------	*/
/*	  r e s t _ l l _ p r o c e s s _ r e q u e s t		*/
/*	------------------------------------------------	*/
private	struct rest_response *	rest_ll_process_request( 
		struct rest_client  * cptr, 
		struct rest_request * rptr)
{
	struct	rest_header * hptr;
	char	username[256];
	char 	password[256];

	/* --------------------------------------- */
	/* check for external application security */
	/* --------------------------------------- */
	if ( cptr->server->method.security )
		if (!( (*cptr->server->method.security)
			( cptr->server->method.instance, cptr, rptr ) ))
			return((struct rest_response *) 0);

	/* validate request header for Host and Authentication */
	/* --------------------------------------------------- */
	if (!( hptr = rest_resolve_header( rptr->first, _HTTP_HOST ) ))
		return( rest_response_failure( cptr, 400, "Bad Request Expected Host" ) );
	else if (!( hptr = rest_resolve_header( rptr->first, _HTTP_AUTHORIZATION ) ))
	{
		if ( cptr->server->method.authorise )
			return( rest_authentication_challenge( cptr, 401, "Not Authorised" ) );
	}
	else if ( cptr->server->method.authorise )
	{
		if (!( rest_decode_credentials(hptr->value, username, password) ))
			return( rest_authentication_challenge( cptr, 401, "Not Authorised" ) );
		else if (!((*cptr->server->method.authorise)(cptr->server->method.instance,cptr, username,password)))
			return( rest_authentication_challenge( cptr, 401, "Not Authorised" ) );
	}

	/* dispatch the message to the appropriate handler */
	/* ----------------------------------------------- */
	if (!( strcasecmp( rptr->method, "GET" ) ))
	{
		if ( cptr->server->method.get )
			return((*cptr->server->method.get)(cptr->server->method.instance,cptr,rptr));
		else	return( rest_response_failure(cptr, 405, "Incorrect Method" ) );
	}
	else if (!( strcasecmp( rptr->method, "POST" ) ))
	{
		if ( cptr->server->method.post )
			return((*cptr->server->method.post)(cptr->server->method.instance,cptr,rptr));
		else	return( rest_response_failure(cptr, 405, "Incorrect Method" ) );
	}
	else if (!( strcasecmp( rptr->method, "PUT" ) ))
	{
		if ( cptr->server->method.put )
			return((*cptr->server->method.put)(cptr->server->method.instance,cptr,rptr));
		else	return( rest_response_failure(cptr, 405, "Incorrect Method" ) );
	}
	else if (!( strcasecmp( rptr->method, "DELETE" ) ))
	{
		if ( cptr->server->method.delete )
			return((*cptr->server->method.delete)(cptr->server->method.instance,cptr,rptr));
		else	return( rest_response_failure(cptr, 405, "Incorrect Method" ) );
	}
	else if (!( strcasecmp( rptr->method, "HEAD" ) ))
	{
		if ( cptr->server->method.head )
			return((*cptr->server->method.head)(cptr->server->method.instance,cptr,rptr));
		else	return( rest_response_failure(cptr, 405, "Incorrect Method" ) );
	}
	else	return( rest_response_failure(cptr, 405, "Incorrect Method" ) );
}

/*	------------------------------------------------	*/
/*	     r e s t _ p r o c e s s _ r e q u e s t		*/
/*	------------------------------------------------	*/
private	struct rest_response * rest_process_request( 
		struct rest_client  * cptr, 
		struct rest_request * rptr)
{
	struct	rest_response * aptr;
	rest_log_recv_request( cptr->server->name, rptr );
	if ( cptr->server->method.before )
		(*cptr->server->method.before)(cptr->server->method.instance,cptr,rptr);
	aptr = rest_ll_process_request( cptr, rptr );
	if ( cptr->server->method.after )
		(*cptr->server->method.after)(cptr->server->method.instance,cptr,rptr);
	return( aptr );
}

/*	------------------------------------------------	*/
/*		r e s t _ o o n s u m e _ b y t e		*/
/*	------------------------------------------------	*/
private	int	rest_consume_byte( struct rest_client * cptr )
{
	while ( cptr->consumed >= cptr->bytes )
		if ( rest_client_read( cptr ) == -1 )
			return( -1 );
	return( cptr->buffer[cptr->consumed++] );
}

/*	------------------------------------------------	*/
/*		r e s t _ o o n s u m e _ e o l 		*/
/*	------------------------------------------------	*/
private	int	rest_consume_eol( struct rest_client * cptr )
{
	int	c;
	while ((c = rest_consume_byte( cptr )) != -1 )
	{
		if ( c == '\n' )
			break;
		else	continue;
	}
	return( c );
}

/*	------------------------------------------------	*/
/*	 r e s t _ c o n s u m e _ c h u n k _ s i z e 		*/
/*	------------------------------------------------	*/
private	int	rest_consume_chunk_size( struct rest_client * cptr ) 
{
	char buffer[1024];
	int	c;
	int	i=0;
	while ((c = rest_consume_byte( cptr )) != -1)
	{
		if ( c == '\n' )
			break;
		else if ( c == '\r' )
			continue;
		else	buffer[i++] = c;
	}
	buffer[i] = 0;
	return( strtol( buffer,(char **) 0, 16 ) );
}

/*	------------------------------------------------	*/
/*	     r e s t _ c o n s u m e _ t o k e n 		*/
/*	------------------------------------------------	*/
private	char *	rest_consume_token( struct rest_client  * cptr, int terminator )	
{
	int	c;
	int	i=0;
	char *	work;
	char *	result=(char *) 0;
	if (!( work = allocate( (cptr->buffersize+1) )))
		return( work );
	else
	{
		while ((c = rest_consume_byte( cptr )) != -1)
		{
			if ( c == '\r' )
				continue;
			else if ( c == terminator )
				break;
			else if ( c == '\n' )
				break;
			else	work[i++] = c;
		}
		if ( i )
		{
			work[i] = 0;
			result = allocate_string( work );
		}
		liberate( work );
		return( result );
	}
}

/*	------------------------------------------------	*/
/*	     r e s t _ c o n s u m e _ l i n e   		*/
/*	------------------------------------------------	*/
private	char * rest_consume_line( struct rest_client  * cptr )
{
	return( rest_consume_token( cptr, '\n' ) );
}

/*	------------------------------------------------	*/
/*	     r e s t _ c o n s u m e _ l i n e   		*/
/*	------------------------------------------------	*/
private	char * rest_consume_value( struct rest_client  * cptr )
{
	int	c;
	while (1)
	{
		if ((c = rest_consume_byte( cptr )) == -1)
			return((char *) 0);
		else if ( c == ' ' )
			continue;
		else if ( c == '\t' )
			continue;
		else	break;
	}
	/* unget byte */
	cptr->consumed--;
	return( rest_consume_token( cptr, '\n' ) );
}

/*	------------------------------------------------	*/
/*	     r e s t _ p a r s e _ o b j e c t			*/
/*	------------------------------------------------	*/
public	struct rest_request *	rest_parse_object(
		struct rest_client  * cptr, 
		struct rest_request * rptr)
{
	char	*	sptr;
	if (!( sptr = rptr->object ))
		return( rptr );
	else
	{
		while ( *sptr )
		{
			if ( *sptr == '?' )
			{
				*(sptr++) = 0;
				if (!( rptr->parameters = allocate_string(sptr) ))
					break;
				else	break;
			}
			else	sptr++;
		}
		return( rptr );
	}
}

/*	------------------------------------------------	*/
/*	     r e s t _ c o n s u m e _ s t a t u s		*/
/*	------------------------------------------------	*/
private	struct rest_response *	rest_consume_status(
		struct rest_client  * cptr, 
		struct rest_response * rptr)
{
	if (!( rptr->status = atoi(rest_consume_token(cptr,' '))))
		return( rptr );
	else	return( rptr );
}

/*	------------------------------------------------	*/
/*	     r e s t _ c o n s u m e _ v e r s i o n 		*/
/*	------------------------------------------------	*/
private	struct rest_response *	rest_consume_version(
		struct rest_client  * cptr, 
		struct rest_response * rptr)
{
	if (!( rptr->version = rest_consume_token(cptr,' ')))
		return( rptr );
	else	return( rptr );
}

/*	------------------------------------------------	*/
/*	     r e s t _ c o n s u m e _ m e s s a g e 		*/
/*	------------------------------------------------	*/
private	struct rest_response *	rest_consume_message(
		struct rest_client  * cptr, 
		struct rest_response * rptr)
{
	if (!( rptr->message = rest_consume_line(cptr)))
		return( rptr );
	else	return( rptr );
}

/*	------------------------------------------------	*/
/*	     r e s t _ c o n s u m e _ c o m m a n d 		*/
/*	------------------------------------------------	*/
private	struct rest_request *	rest_consume_command(
		struct rest_client  * cptr, 
		struct rest_request * rptr)
{
	if (!( rptr->method = rest_consume_token(cptr,' ')))
		return( rptr );
	else if (!( rptr->object = rest_consume_token(cptr,' ')))
		return( rptr );
	else if (!( rptr->version = rest_consume_line(cptr)))
		return( rptr );
	else	return( rptr );
}

/*	------------------------------------------------	*/
/*		r e s t _ a p p e n d _ h e a d e r		*/
/*	------------------------------------------------	*/
private	struct rest_header * rest_append_header( struct rest_header * hptr, char * postfix )
{
	char *	result;
	if (!( result = allocate( 
				strlen( (hptr->value ? hptr->value : "" ) )+
				strlen( (postfix     ? postfix     : "" ) )+8 ) ))
		return( hptr );
	else
	{
		sprintf(result,"%s%s",
			(hptr->value ? hptr->value : "" ),
			(postfix      ? postfix      : "" ));
		liberate( hptr->value );
		hptr->value = result;
		liberate( postfix );
		return( hptr );
	}
}


/*	------------------------------------------------	*/
/*	     r e s t _ c o n s u m e _ h e a d      		*/
/*	------------------------------------------------	*/
private	struct rest_request *	rest_consume_head(
		struct rest_client  * cptr, 
		struct rest_request * rptr)
{
	char	*	nptr;
	struct	rest_header * hptr;
	while (1)
	{
		if ((cptr->bytes - cptr->consumed) > 0)
		{
			switch (cptr->buffer[cptr->consumed])
			{
			case	0	:
				break;
			case	' '	:
			case	'\t'	:
				if (!( hptr = rest_append_header( rptr->last, rest_consume_line( cptr )) ))
				{
					drop_rest_header( hptr );
					liberate( nptr );
					break;
				}
				else	continue;
			default		:
				if (!(nptr = rest_consume_token(cptr,':')))
					break;
				else if (!( hptr = add_rest_header( rptr ) ))
					break;
				else if (!( hptr->value = rest_consume_value( cptr ) ))
				{
					drop_rest_header( hptr );
					liberate( nptr );
					break;
				}
				else	hptr->name = nptr;
				if ( rest_check_debug() )
					printf("%s: %s\n",hptr->name,hptr->value);
				continue;
			}
			break;
		}
		else if ( rest_client_read( cptr ) <= 0 )
			break;
	}
	return( rptr );
}

/*	---------------------------------------------------	*/
/*	r e s t _ c o n s u m e _ r e s p o n s e _ h e a d    	*/
/*	---------------------------------------------------	*/
private	struct rest_response *	rest_consume_response_head(
		struct rest_client  * cptr, 
		struct rest_response * rptr)
{
	char	*	nptr;
	struct	rest_header * hptr;
	while (1)
	{
		if ((cptr->bytes - cptr->consumed) > 0)
		{
			switch (cptr->buffer[cptr->consumed])
			{
			case	0	:
				break;
			case	' '	:
			case	'\t'	:
				if (!( hptr = rest_append_header( rptr->last, rest_consume_line( cptr )) ))
				{
					drop_rest_header( hptr );
					liberate( nptr );
					break;
				}
				else	continue;
			default		:
				if (!(nptr = rest_consume_token(cptr,':')))
					break;
				else if (!( hptr = add_response_header( rptr ) ))
					break;
				else if (!( hptr->value = rest_consume_value( cptr ) ))
				{
					drop_rest_header( hptr );
					liberate( nptr );
					break;
				}
				else	hptr->name = nptr;
				if ( rest_check_debug() )
					printf("%s: %s\n",hptr->name,hptr->value);
				continue;
			}
			break;
		}
		else if ( rest_client_read( cptr ) <= 0 )
			break;
	}
	return( rptr );
}

private	char	*	tempfilepath=_CORDS_REST_PATH;
private	char	*	tempfilestub=_CORDS_REST_STUB;
private	int		tempfilecount=0;

/*	---------------------------------------------	*/  
/*	    r e s t _ a l l o c a t e _ u u i d		*/
/*	---------------------------------------------	*/  
public	char *	rest_allocate_uuid()
{
	uuid_t	result;
	char	buffer[256];
	uuid_generate( result );
	uuid_unparse( result, buffer );
	return( allocate_string( buffer ) );
}

/*	------------------------------------------------	*/
/*	  	r e s t _ k e y _ f i l e			*/
/*	------------------------------------------------	*/
public	char *	rest_key_file( char * nptr, char * vptr )
{
	int	i;
	int	j;
	int	c;
	char	pemname[1024];
	char	buffer[1024];
	FILE *	h;

	sprintf(pemname,"security/%s.pem",nptr);

	if (!( h = fopen( pemname, "w" ) ))
		return( (char *) 0);
	else
	{
		/* step over header */
		/* ---------------- */
		for ( i=0; i< 31; i++ )
			fputc( *(vptr++), h );
		fputc( '\n', h );
		if ( *vptr == 'n' ) vptr++;

		/* 12 body lines */
		/* ------------- */
		for ( j=0; j < 12; j++ )
		{
			for ( i=0; i< 64; i++ )
				fputc( *(vptr++), h );
			if ( *vptr == 'n' ) vptr++;
			fputc( '\n', h );
		}

		/* last line handling */
		/* ------------------ */
		for ( i=0; i< 64; i++ )
		{
			if ((c = *(vptr++)) == 'n' )
				if ( *vptr == '-' )
					break;
			fputc( c, h );
		}
		if ( *vptr == 'n' ) vptr++;
		fputc( '\n', h );

		/* footer handling */
		/* --------------- */
		for ( i=0; i< 29; i++ )
			fputc( *(vptr++), h );

		fclose(h);

		sprintf(buffer,"chmod 0600 %s",pemname);
		system( buffer );

		return( allocate_string( pemname ) );
	}
}

/*	------------------------------------------------	*/
/*	  r e s t _ t e m p o r a r y _ f i l e n a m e 	*/
/*	------------------------------------------------	*/
public	char *	rest_temporary_filename(char * extension)
{
	char	namebuffer[2048];
	char *	tptr;
	if (!( tptr = rest_allocate_uuid()))
		if (!( tptr = allocate_string( tempfilestub ) ))
			return( tptr );
	sprintf(namebuffer,"%s%s.%s",tempfilepath,tptr,( extension ? extension : "tmp" ));
	return( allocate_string( namebuffer ) );
}

/*	------------------------------------------------	*/
/*	     r e s t _ r e s o l v e _ h e a d e r		*/
/*	------------------------------------------------	*/
public	struct rest_header * rest_resolve_header(struct rest_header * hptr,char * name )
{
	if (!( hptr ))
		return( hptr );
	else if (!( name ))
		return((struct rest_header *) 0);
	else
	{
		while ( hptr )
		{
			if ( hptr->name )
				if (!( strcasecmp( hptr->name, name ) ))
					break;
			hptr = hptr->next;
		}
		return( hptr );
	}
}

/*	------------------------------------------------	*/
/*	  r e s t _ c o n t e n t _ e x t e n s i o n		*/
/*	------------------------------------------------	*/
private	char *	rest_content_extension( char * sptr )
{
	if (!( sptr ))
		return( "tmp" );
	if ((!( strcmp( sptr, "text/html" 	)))
	||  (!( strcmp( sptr, "application/html" 	)))
	||  (!( strcmp( sptr, "x-application/html"))))
		return( "html" );
	if ((!( strcmp( sptr, "text/json" 	)))
	||  (!( strcmp( sptr, "application/json" 	)))
	||  (!( strcmp( sptr, "application/json+occi" 	)))
	||  (!( strcmp( sptr, "application/json:occi" 	)))
	||  (!( strcmp( sptr, "x-application/json"))))
		return( "json" );
	if ((!( strcmp( sptr, "text/xml" 	)))
	||  (!( strcmp( sptr, "application/xml" 	)))
	||  (!( strcmp( sptr, "x-application/xml"))))
		return( "xml" );
	if ((!( strcmp( sptr, "text/php" 	)))
	||  (!( strcmp( sptr, "application/php" 	)))
	||  (!( strcmp( sptr, "x-application/php"))))
		return( "php" );
	if ((!( strcmp( sptr, "text/occi" 	)))
	||  (!( strcmp( sptr, "application/occi" 	)))
	||  (!( strcmp( sptr, "x-application/occi"))))
		return( "occi" );
	if ((!( strcmp( sptr, "text/plain" 	)))
	||  (!( strcmp( sptr, "application/text" 	)))
	||  (!( strcmp( sptr, "x-application/text"))))
		return( "txt" );
	else	return( "tmp" );
}


/*	------------------------------------------------	*/
/*	     r e s t _ c o n s u m e _ b o d y      		*/
/*	------------------------------------------------	*/
private	struct rest_request *	rest_consume_body(
		struct rest_client  * cptr, 
		struct rest_request * rptr,
		struct rest_header  * hptr )
{
	char	*	xptr;
	FILE	*	h;
	char	*	nptr;
	int		bytes=0;

	if (!( hptr ))
		return( rptr );
	else if (!( hptr->value ))
		return( rptr );
	else if (!(bytes = atoi( hptr->value )))
		return( rptr );
	else if (!( hptr = rest_resolve_header( rptr->first, _HTTP_CONTENT_TYPE ) ))
		return( rptr );
	else if (!( xptr = rest_content_extension( hptr->value )))
		return( rptr );
	else if (!( nptr = rest_temporary_filename(xptr)))
		return( rptr );
	else	rest_request_body( rptr, nptr, _FILE_BODY );

	if (!( h = fopen( rptr->body,"wb" ) ))
	{
		/* TODO FAILURE */
		return( rptr );
	}
	else
	{
		while ( bytes )
		{
			if ((cptr->bytes - cptr->consumed) > 0)
			{
				if ( fwrite((cptr->buffer+cptr->consumed),(cptr->bytes - cptr->consumed),1,h) <= 0 )
				{
					/* TODO FAILURE */
					break;
				}
				else 	bytes -= (cptr->bytes - cptr->consumed);
			}

			if (!( bytes ))
				break;

			else if ( rest_client_read( cptr ) <= 0 )
			{
				/* OK NO MORE */
				break;
			}
		}
		fclose(h);
		return( rptr );
	}
}

/*	------------------------------------------------	*/
/*	     r e s t _ c o n s u m e _ c h u n k s 		*/
/*	------------------------------------------------	*/
private	struct rest_request *	rest_consume_chunks(
		struct rest_client  * cptr, 
		struct rest_request * rptr,
		struct rest_header  * hptr )
{
	char	*	xptr;
	FILE	*	h;
	char	*	nptr;
	int		bytes=0;

	if (!( hptr = rest_resolve_header( rptr->first, _HTTP_CONTENT_TYPE ) ))
		return( rptr );
	else if (!( xptr = rest_content_extension( hptr->value )))
		return( rptr );
	else if (!( nptr = rest_temporary_filename(xptr)))
		return( rptr );
	else	rest_request_body( rptr, nptr, _FILE_BODY );

	if (!( h = fopen( rptr->body,"wb" ) ))
	{
		/* TODO FAILURE */
		return( rptr );
	}
	else
	{
		while ((bytes = rest_consume_chunk_size( cptr )) != 0)
		{
			while ( bytes )
			{
				if ((cptr->bytes - cptr->consumed) > 0)
				{
					if ( fwrite((cptr->buffer+cptr->consumed),(cptr->bytes - cptr->consumed),1,h) <= 0 )
					{
						/* TODO FAILURE */
						break;
					}
					else 	bytes -= (cptr->bytes - cptr->consumed);
				}
	
				if (!( bytes ))
					break;

				else if ( rest_client_read( cptr ) <= 0 )
				{
					/* OK NO MORE */
					break;
				}
			}
			if ( bytes )
				break;
			else if ( rest_consume_eol( cptr ) != '\n' )
				break;
		}
		fclose(h);
		return( rptr );
	}
}

/*	------------------------------------------------	*/
/*	     r e s t _ d e t e c t _ b o d y      		*/
/*	------------------------------------------------	*/
private	struct rest_request *	rest_detect_body( 
		struct rest_client  * cptr, 
		struct rest_request * rptr)
{
	struct	rest_header * hptr;
	if (( hptr = rest_resolve_header( rptr->first, _HTTP_CONTENT_LENGTH )) != (struct rest_header *) 0)
		return( rest_consume_body( cptr, rptr, hptr ) );
	else if (!( hptr = rest_resolve_header( rptr->first, _HTTP_TRANSFER_ENCODING )))
		return( rptr );
	else if (!( strcasecmp( hptr->value, _HTTP_CHUNKED ) ))
		return( rest_consume_chunks( cptr, rptr, hptr ) );
	else	return( rptr );
}

/*	---------------------------------------------------	*/
/*	r e s t _ c o n s u m e _ r e s p o n s e _ b o d y     */
/*	---------------------------------------------------	*/
private	struct rest_response *	rest_consume_response_body(
		struct rest_client  * cptr, 
		struct rest_response * rptr,
		struct rest_header * hptr )
{
	FILE	*	h;
	char	*	xptr;
	char	*	nptr;
	int		bytes=0;

	if (!( hptr ))
		return( rptr );
	else if (!( hptr->value ))
		return( rptr );
	else if (!(bytes = atoi( hptr->value )))
		return( rptr );
	else if (!( hptr = rest_resolve_header( rptr->first, _HTTP_CONTENT_TYPE ) ))
		return( rptr );
	else if (!( xptr = rest_content_extension( hptr->value )))
		return( rptr );
	else if (!( nptr = rest_temporary_filename(xptr)))
		return( rptr );
	else	rest_response_body( rptr, nptr, _FILE_BODY );

	if (!( h = fopen( rptr->body,"wb" ) ))
	{
		/* TODO FAILURE */
		return( rptr );
	}
	else
	{
		while ( bytes )
		{
			if ((cptr->bytes - cptr->consumed) > 0)
			{
				if ( fwrite((cptr->buffer+cptr->consumed),(cptr->bytes - cptr->consumed),1,h) <= 0 )
				{
					break;
				}
				else 	bytes -= (cptr->bytes - cptr->consumed);
				if (!( bytes ))
					break;
			}

			if (!( rest_client_read( cptr ) ))
			{
				/* OK NO MORE */
				break;
			}
		}
		fclose(h);
		return( rptr );
	}
}

/*	-------------------------------------------------------		*/
/*	r e s t _ c o n s u m e _ r e s p o n s e _ c h u n k s		*/ 
/*	-------------------------------------------------------		*/
private	struct rest_response *	rest_consume_response_chunks(
		struct rest_client  * cptr, 
		struct rest_response * rptr,
		struct rest_header * hptr )
{
	FILE	*	h;
	char	*	xptr;
	char	*	nptr;
	int		bytes=0;

	if (!( hptr = rest_resolve_header( rptr->first, _HTTP_CONTENT_TYPE ) ))
		return( rptr );
	else if (!( xptr = rest_content_extension( hptr->value )))
		return( rptr );
	else if (!( nptr = rest_temporary_filename(xptr)))
		return( rptr );
	else	rest_response_body( rptr, nptr, _FILE_BODY );

	if (!( h = fopen( rptr->body,"wb" ) ))
	{
		/* TODO FAILURE */
		return( rptr );
	}
	else
	{
		while ((bytes = rest_consume_chunk_size( cptr )) != 0)
		{
			if (bytes > (cptr->bytes - cptr->consumed)) // If more bytes in chunk than in buffer
			{
				while (bytes > (cptr->bytes - cptr->consumed))
				{
					// Write whatever remains in buffer to file.
					if (fwrite((cptr->buffer+cptr->consumed), (cptr->bytes - cptr->consumed), 1, h) <= 0)
					{
						/* TODO FAILURE */
					}

					// Mark that many bytes as done.
					bytes -= (cptr->bytes - cptr->consumed);

					// Read some more data.
					if (!rest_client_read(cptr)) // If failed to read more data
					{
						/* TODO FAILURE Report error that couldn't read remaining bytes */
						break;

					} // Ends if failed to read more data

				} // Ends while more bytes in chunk than in buffer

				if (bytes > 0) // If still need to write bytes
				{
					if ((cptr->bytes - cptr->consumed) < bytes) // If don't have enough bytes in buffer
					{
						/* TODO - Report error that couldn't get all the bytes */

					}
					else // Else enough bytes in buffer
					{
						// Write the remaining bytes to the file.
						if (fwrite((cptr->buffer+cptr->consumed), bytes, 1, h) <= 0)
						{
							/* TODO FAILURE */
						}
						else
						{
							// Mark the remaining bytes as consumed.
							cptr->consumed += bytes;
							bytes = 0;
						}

					} // Ends else enough bytes in buffer
				} // Ends if still need to write bytes
			} // Ends if more bytes in chunk than in buffer
			else // Else all bytes in buffer
			{
				// Write the remaining bytes to the file.
				if (fwrite((cptr->buffer+cptr->consumed), bytes, 1, h) <= 0)
				{
					/* TODO FAILURE */
				}
				else
				{
					// Mark the remaining bytes as consumed.
					cptr->consumed += bytes;
					bytes = 0;
				}

			} // Ends else all bytes in buffer

			if ( rest_consume_eol( cptr ) != '\n' )
			{
				/* TODO Report error in missing \r\n */
				break;
			}
		}

		/* Now get the headers encoded at the end of the chunk sequences. */
		rest_consume_response_head(cptr, rptr);

		fclose(h);
		return( rptr );
	}
}

/*	-------------------------------------------------	*/
/*	r e s t _ d e t e c t _ r e s p o n s e _ b o d y      	*/
/*	-------------------------------------------------	*/
private	struct rest_response *	rest_detect_response_body( 
		struct rest_client  * cptr, 
		struct rest_response * rptr)
{
	struct	rest_header * hptr;
	if (( hptr = rest_resolve_header( rptr->first, _HTTP_CONTENT_LENGTH )) != (struct rest_header *) 0)
		return( rest_consume_response_body( cptr, rptr, hptr ) );
	else if (!( hptr = rest_resolve_header( rptr->first, _HTTP_TRANSFER_ENCODING )))
		return( rptr );
	else if (!( strcasecmp( hptr->value, _HTTP_CHUNKED ) ))
		return( rest_consume_response_chunks( cptr, rptr, hptr ) );
	else	return( rptr );
}

/*	------------------------------------------------	*/
/*	     r e s t _ c o n s u m e _ r e q u e s t		*/
/*	------------------------------------------------	*/
private	struct rest_request *	rest_consume_request( struct rest_client * cptr, int port )
{
	struct	rest_request * 	rptr;

	if (!( rptr = allocate_rest_request() ))
		return( rptr );
	else	rptr->port = port;

	if (!( rptr = rest_consume_command( cptr, rptr )))
		return( rptr );
	else if (!( rptr = rest_parse_object( cptr, rptr )))
		return( rptr );

	else if (!( rptr = rest_consume_head( cptr, rptr )))
		return( rptr );

	rest_show_request( rptr );

	if (!( rptr = rest_detect_body( cptr, rptr )))
		return( rptr );
	else	return( rptr ); 
}

/*	------------------------------------------------	*/
/*	   r e s t _ c o n s u m e _ r e s p o n s e 		*/
/*	------------------------------------------------	*/
private	struct	rest_response * rest_consume_response( struct rest_client * cptr )
{
	struct	rest_response *	aptr;
	if (!( aptr = allocate_rest_response() ))
		return( aptr );
	else if (!( aptr = rest_consume_version( cptr, aptr )))
		return( aptr );
	else if (!( aptr = rest_consume_status( cptr, aptr )))
		return( aptr );
	else if (!( aptr = rest_consume_message( cptr, aptr )))
		return( aptr );
	else if (!( aptr = rest_consume_response_head( cptr, aptr )))
		return( aptr );
	else if (!( aptr = rest_detect_response_body( cptr, aptr )))
		return( aptr );
	else	return( aptr ); 
}

/*	------------------------------------------------	*/
/*	     r e s t _ a c c e p t _ r e s p o n s e		*/
/*	------------------------------------------------	*/
public	struct	rest_response * rest_accept_response( struct rest_client * cptr )
{
	struct	rest_request *	rptr;
	struct	rest_response *	aptr;

	// -----------------------------------------
	// Attempt to read from the connected socket
	// -----------------------------------------
	if ( rest_client_read( cptr ) <= 0)
	{
		rest_drop_client( cptr );
		return((struct rest_response *) 0);
	}
	else if (!( aptr = rest_consume_response( cptr ) ))
	{
		rest_drop_client( cptr );
		return((struct rest_response *) 0);
	}
	else
	{
		rest_drop_client( cptr );
		return( aptr );
	}
}

/*	-----------------------------------------------------	*/
/*	r e s t _ l i b e r a t e _ r e s p o n s e _ f i l e	*/
/*	-----------------------------------------------------	*/
private	int	rest_liberate_response_file( struct rest_response * aptr )
{
	if (!( aptr ))
		return( 0 );
	else if (!( aptr->body ))
		return( 0 );
	else if ( aptr->type != _FILE_BODY )
		return( 0 );
	else 	
	{
		unlink( aptr->body );
		return(0);
	}
}

/*	------------------------------------------------	*/
/*	     r e s t _ p r o c e s s _ m e s s a g e		*/
/*	------------------------------------------------	*/
private	int	rest_process_message( 
		struct rest_client * cptr,
		struct rest_request * rptr )
{
	struct	rest_response *	aptr;
	if (!( aptr = rest_process_request( cptr, rptr ) ))
	{
		rest_log_debug("rest: process request failure");
		rest_log_debug("rest: liberate response");
		liberate_rest_request( rptr );
		rest_log_debug("rest: process message ooops");
		return(0);
	}
	else 
	{
		aptr->version = allocate_string( rptr->version );
		rest_log_debug("rest: transmit response");
		rest_transmit_response( cptr, aptr, rptr );
		rest_log_debug("rest: liberate request");
		rest_liberate_response_file( aptr );
		liberate_rest_request( rptr );
		rest_log_debug("rest: liberate response");
		rest_liberate_response( aptr );
		rest_drop_client( cptr );
		rest_log_debug("rest: process message done");
		return(0);
	}
}


#ifdef	_REST_THREAD_WORKER

private	void	rest_detach_client( struct rest_client * cptr );
private	void *	rest_thread_message( void * vptr );
#include "restthread.c"

/*	------------------------------------------------	*/
/*	     r e s t _ t h r e a d _ m e s s a g e		*/
/*	------------------------------------------------	*/
private	void	rest_log_thread( char * mptr )
{
	rest_debug_log_file( mptr );
}

private	void *	rest_thread_message( void * vptr )
{
	struct rest_thread * tptr=vptr;
	int	status;
	struct rest_client * cptr;
	struct rest_request * rptr;

	rest_log_thread("thread: pthread detach");
	pthread_detach( tptr->id );
	while(tptr->status)
	{
		/* await job for thread */
		/* -------------------- */
		rest_log_thread("thread: awaiting job message");
		while(1)
		{
			lock_rest_thread(tptr);
			cptr = tptr->client;
			rptr = tptr->request;
			unlock_rest_thread( tptr );
			if (!( cptr ))
				wait_rest_thread(tptr);
			else	break;
		}
		
		/* process thread job */
		/* ------------------ */
		rest_log_thread("thread: rest process message start");
		status = rest_process_message( cptr, rptr );
		rest_log_thread("thread: rest process message exit");

		/* terminate thread job */
		/* -------------------- */
		lock_rest_thread( tptr );
		tptr->client  = (struct rest_client *) 0;
		tptr->request = (struct rest_request *) 0;
		unlock_rest_thread( tptr );
	}
	tptr->status = 0;
	rest_log_thread("thread: rest pthread exit");
	pthread_exit((void *) 0);
}
#endif

/*	------------------------------------------------	*/
/*	     r e s t _ d e t a c h _ c l i e n t   		*/
/*	------------------------------------------------	*/
private	void	rest_detach_client( struct rest_client * cptr )
{
	struct	rest_server * sptr;
	if (!( sptr = cptr->parent ))
		return;
	lock_rest_server( sptr );
	cptr->parent = (struct rest_server *) 0;
	if (!( cptr->previous ))
	{
		if ((sptr->first = cptr->next))
			sptr->first->previous = (struct rest_client *) 0;
	}
	else if ((cptr->previous->next = cptr->next ))
		cptr->next->previous = cptr->previous;

	if (!( cptr->next ))
	{
		if ((sptr->last = cptr->previous))
			sptr->last->next = (struct rest_client *) 0;
	}
	else if ((cptr->next->previous = cptr->previous ))
		cptr->previous->next = cptr->next;

	cptr->previous = cptr->next = (struct rest_client *) 0;
	unlock_rest_server( sptr );
	return;
}

/*	------------------------------------------------	*/
/*	     r e s t _ a c c e p t _ m e s s a g e		*/
/*	------------------------------------------------	*/
private	int	rest_accept_message( struct rest_client * cptr, int port )
{
	struct	rest_request *	rptr;
#ifdef	_REST_THREAD_WORKER
	struct	rest_thread * tptr;
	struct	rest_thread_context * xptr=(struct rest_thread_context *) 0;
#endif	
	// -----------------------------------------
	// Attempt to read from the connected socket
	// -----------------------------------------
	if (!( rest_client_read( cptr )))
	{
		rest_drop_client( cptr );
		return(0);
	}

	/* ----------------------------------- */
	/* otherwise it may be an HTTP request */ 
	/* ----------------------------------- */
	if (!( rptr = rest_consume_request( cptr, port ) ))
	{
		rest_drop_client( cptr );
		return(0);
	}

#ifdef	_REST_THREAD_WORKER
	if (!( thread_control & 1 ))
	{
		return( rest_process_message( cptr, rptr ) );
	}
	else if (!( tptr = rest_start_thread( cptr, rptr ) ))
	{
		rest_drop_client( cptr );
		return(27);
	}
	else
	{
		rest_log_debug("server: thread started");
		return(0);
	}
#else
	return( rest_process_message( cptr, rptr ) );
#endif
}


/*	------------------------------------------------	*/
/*		r e s t _ s e r v e r _ s e l e c t 		*/
/*	------------------------------------------------	*/
private	int	rest_server_select( struct rest_server * sptr )
{
	char 	buffer[2048];
	struct	rest_extension * xptr;
	struct	rest_client * hptr;
	int	status;
	int	h;
	struct	timeval TimeOut;
	fd_set	r;
	fd_set 	w;
	fd_set	x;
	FD_ZERO(&r);
	FD_ZERO(&w);
	FD_ZERO(&x);
	h = sptr->net.socket;
	sprintf(buffer,"server: select_sockets(%u",h);
	FD_SET(h,&r);
	for ( 	hptr=sptr->first;
		hptr != (struct rest_client *) 0;
		hptr = hptr->next ) 
	{
		if ( hptr->net.socket > h )
			h = hptr->net.socket;
		sprintf(buffer+strlen(buffer),",%u",hptr->net.socket);
		FD_SET(hptr->net.socket,&r);
	}				
	for ( 	xptr=sptr->firstx;
		xptr != (struct rest_extension *) 0;
		xptr = xptr->next ) 
	{
		if (!( xptr->net ))
			continue;
		else
		{
			if ( xptr->net->socket > h )
				h = xptr->net->socket;
			sprintf(buffer+strlen(buffer),";%u",hptr->net.socket);
			FD_SET(xptr->net->socket,&r);
		}
	}				

	strcat(buffer,")");
	rest_log_debug( buffer );

	TimeOut.tv_sec  = (unsigned long) sptr->seconds;
	TimeOut.tv_usec = (unsigned long) sptr->fraction;
	sptr->active    = (struct rest_client *) 0;
	sptr->activex   = (struct rest_extension *) 0;
	if ((status = select( h+1, &r, NULL, NULL ,&TimeOut )) > 0 ) 
	{
		/* test for activity on primary */
		if ( FD_ISSET(sptr->net.socket,&r) )
		{
			sprintf(buffer,"server: primary socket selected(%u)",sptr->net.socket);
			rest_log_debug( buffer );
			return(1);
		}
		for ( 	hptr=sptr->first;
			hptr != (struct rest_client *) 0;
			hptr = hptr->next ) 
		{
			if (!( hptr ))
				continue;
			/* test for activity on this host */
			if ( FD_ISSET(hptr->net.socket,&r) ) 
			{
				sptr->active = hptr;
				sprintf(buffer,"server: secondary socket selected(%u)",hptr->net.socket);
				rest_log_debug( buffer );
				return(2);
			}
		}
		for ( 	xptr=sptr->firstx;
			xptr != (struct rest_extension *) 0;
			xptr = xptr->next ) 
		{
			if (!( xptr ))
				continue;
			else if (!( xptr->net ))
				continue;
			/* test for activity on this host */
			if ( FD_ISSET(xptr->net->socket,&r) ) 
			{
				sptr->activex = xptr;
				sprintf(buffer,"server: extended socket selected(%u)",xptr->net->socket);
				rest_log_debug( buffer );
				return(2);
			}
		}
		rest_log_debug( "server: whoops selected socket not found" );
		return(0);			
	}
	else	
	{
		rest_log_debug("server: socket not selected");
		return(status);
	}
}

/*	------------------------------------------------	*/
/*	  r e s t _ a c c e p t _ c o n n e c t i o n		*/
/*	------------------------------------------------	*/
private	int	handshakes=0;
private	int	rest_accept_connection( struct rest_server * sptr )
{
	int	workerid=0;
	int	length=sizeof( struct sockaddr );
	struct	rest_client * cptr;
	char	chname[1024];
	char 	buffer[1024];

#ifndef	_REST_THREAD_WORKER
	if ( sptr->workers < sptr->threads )
	{
		rest_log_debug("server: create worker");
		if (!( rest_new_worker( sptr ) ))
			return(0);
	}
#endif
	// -----------------------------------------------------
	// add a new host control structure to the server's list
	// -----------------------------------------------------
	rest_log_debug("server: create client");
	if (!( cptr = rest_new_client( sptr ) ))
		return( failure(27,"rest server","accept connection"));
	else if ( sptr->tlsconf )
		tls_server_startup( &cptr->net, sptr->tlsconf->option );

	rest_log_debug("server: accept socket");
	// -----------------------------------------------------
	//   attempt to accept the incoming client connection
	// -----------------------------------------------------
	memset( &cptr->net.remote,0,sizeof(struct sockaddr));
	if ((cptr->net.socket = socket_accept( sptr->net.socket, &cptr->net.remote, &length )) < 0 )
	{
		rest_log_debug("server: accept failure");
		(void) failure(500,"rest server","accept connection");
		rest_drop_client( cptr );
		return(0);
	}
	else if (!( sptr->tlsconf ))
	{
		sprintf(buffer,"server: accept (no tls) (socket=%u)",cptr->net.socket);
		rest_log_debug(buffer);
		return( 0 );
	}
	else 
	{
		rest_log_debug("server: accept handshake");
		handshakes++;
		if (!( tls_server_handshake( &cptr->net, sptr->tlsconf->option ) ))
		{
			sprintf(buffer,"rest server(%u): tls socket(%u) handshake(%u) failure(%u)",
				getpid(),cptr->net.socket,sptr->tlsconf->option, handshakes);
			rest_log_debug( buffer );
			rest_drop_client( cptr );
		}
		else
		{
			sprintf(buffer,"server: accept (tls) (socket=%u)",cptr->net.socket);
			rest_log_debug(buffer);
		}
		return( 0 );
	}
}

/*	------------------------------------------------	*/
/*	      r e s t _ a d d _ e x t e n s i o n		*/
/*	------------------------------------------------	*/
public	struct rest_extension * rest_add_extension( struct rest_server * sptr )
{
	struct	rest_extension * xptr;

	if (!( xptr = allocate_rest_extension() ))
		return( xptr );
	else
	{
		if (!( xptr->previous = sptr->lastx ))
			sptr->firstx = xptr;
		else	xptr->previous->next = xptr;
		sptr->lastx = xptr;
		return( xptr );
	}
}

/*	------------------------------------------------	*/
/*	      r e s t _ d r o p _ e x t e n s i o n		*/
/*	------------------------------------------------	*/
public	struct rest_extension * rest_drop_extension(struct rest_server * sptr,struct rest_extension * xptr)
{
	if ( xptr )
	{
		if ( xptr->previous )
			xptr->previous->next = xptr->next;
		else 	sptr->firstx = xptr->next;
		if ( xptr->next )
			xptr->next->previous = xptr->previous;
		else	sptr->lastx = xptr->previous;
		xptr->payload = (void *) 0;
		liberate_rest_extension( xptr );
	}
	return((struct rest_extension *) 0);
}

/*	------------------------------------------------	*/
/*	   s i g t i m _ s e r v e r _ c a t c h e r		*/
/*	------------------------------------------------	*/
private	int	_server_sigalrm=0;
private	void	sigalrm_server_catcher(int s)
{
	_server_sigalrm=1;
	return;
}

/*	------------------------------------------------	*/
/*	     	   r e q u e s t _ w a k e u p 			*/
/*	------------------------------------------------	*/
private	int	request_wakeup( struct rest_server * sptr )
{
	sptr->wakeup = 5000;
	_server_sigalrm=0;
	return(0);
}

/*	------------------------------------------------	*/
/*	     	     h a n d l e _ w a k e u p 			*/
/*	------------------------------------------------	*/
private	int	handle_wakeup( struct rest_server * sptr )
{
	request_wakeup(sptr);
	return(0);
}

/*	------------------------------------------------	*/
/*	   s i g i n t _ s e r v e r _ c a t c h e r		*/
/*	------------------------------------------------	*/
private	int	_server_sigint=0;
private	void	sigint_server_catcher(int s)
{
	_server_sigint=1;
	return;
}

/*	------------------------------------------------	*/
/*		r e s t _ s e r v e r _ s i g n al 		*/
/*	------------------------------------------------	*/
public	int	rest_server_signal()
{
	return( _server_sigint );
}

/*	------------------------------------------------	*/
/*	    r e s t _ a l l o c a t e _ s e r v e r		*/
/*	------------------------------------------------	*/
private	struct	rest_server * rest_allocate_server()
{
	struct	rest_server * rptr;
	if (!( rptr = allocate_rest_server() ))
		return( rptr );
	else
	{
		memset( &rptr->net, 0, sizeof( struct connection ) );
		memset( &rptr->method, 0, sizeof( struct rest_interface ) );
		memset( &rptr->lock,0,sizeof( rptr->lock));
		rptr->tlsconf = (struct tls_configuration *) 0;
		return( rptr );
	}
}

/*	------------------------------------------------	*/
/*	    r e s t _ l i b e r a t e _ s e r v e r		*/
/*	------------------------------------------------	*/
private	struct	rest_server * rest_liberate_server( struct rest_server * rptr )
{
	struct	rest_extension * xptr;
	if ( rptr )
	{
		connection_close( &rptr->net, 1 );
		if ( rptr->tlsconf )
			rptr->tlsconf = release_tls_configuration( rptr->tlsconf );
		while ((xptr=rptr->firstx) != (struct rest_extension *) 0)
		{
			rptr->firstx = xptr->next;
			liberate_rest_extension( xptr );
		}
	}
	
	return( liberate_rest_server( rptr ) );
}		

/*	------------------------------------------------	*/
/*	    	r e s t _ n e w _ c l i e n t			*/
/*	------------------------------------------------	*/
private	struct	rest_client * rest_new_client(struct rest_server * sptr)
{
	struct	rest_client * rptr;
	lock_rest_server( sptr );
	if (!( rptr = add_rest_client(sptr) ))
	{
		unlock_rest_server( sptr );
		return( rptr );
	}
	else if (!( rptr->buffer = allocate((rptr->buffersize = _MAX_RESTBUFFER)) ))
	{
		unlock_rest_server( sptr );
		return( liberate_rest_client( rptr ) );
	}
	else	
	{
		memset( &rptr->net, 0, sizeof( struct connection ) );
		rptr->server = sptr;
		unlock_rest_server( sptr );
		return( rptr );
	}
}

/*	------------------------------------------------	*/
/*	    r e s t _ d r o p _ c l i e n t			*/
/*	------------------------------------------------	*/
public	struct	rest_client * rest_drop_client( struct rest_client * cptr )
{
	char 			buffer[1024];
	struct	rest_server * 	sptr;

	if (!( cptr ))
		return( cptr );

	if ((sptr = cptr->parent))
		lock_rest_server( sptr );

	rest_log_debug( "rest: drop client" );
	if ( cptr )
	{
		sprintf(buffer,"connection close (socket=%u)",cptr->net.socket);
		rest_log_debug( buffer );
		connection_close( &cptr->net,1);
	}
	cptr = drop_rest_client( cptr );
	if ( sptr )
		unlock_rest_server( sptr );
	return( cptr );
}		

/*	------------------------------------------------	*/
/*			r e s t _ s e r v e r			*/
/*	------------------------------------------------	*/
public	int	rest_server( char * nptr, int port, char * tls, int max, struct rest_interface * iptr )
{
	int	status;
	struct	rest_server * sptr;

	if ( tls )
		if (!( strlen(tls) ))
			tls = (char *) 0;

	if (!( sptr = rest_open_server( port, tls, max, iptr ) ))
		return(40);
	else if (!( sptr->name = allocate_string( nptr ) ))
	{
		rest_liberate_server( sptr );
		return( 27 );
	}
	else
	{
		/* ----------------------------------------------- */
		/* allow application to see server before starting */
		/* ----------------------------------------------- */
		if ( sptr->method.initialise )
		{
			if (!( (*sptr->method.initialise)(sptr->method.instance,sptr) ))
			{
				rest_liberate_server( sptr );
				return( 33 );
			}
		}

		/* ----------------------------------------------- */
		/* enter the  operational loop  receiving messages */
		/* ----------------------------------------------- */
		_server_sigint = 0;
		signal( SIGINT,sigint_server_catcher);
		signal( SIGTERM,sigint_server_catcher);
		request_wakeup(sptr);

		if ( check_verbose() )
			printf("\n   REST Server %s : listening on port : %u\n",sptr->name,port);

		while (!(_server_sigint)) 
		{

			if ( sptr->wakeup )
				sptr->wakeup--;

			if (!( sptr->wakeup ))
				handle_wakeup( sptr );

			else if (!(status = rest_server_select( sptr )))
			{
				if (!( rest_release_zombies( sptr ) ))
					continue;
				else	break;
			}
			else if ( status == -1 ) 
			{
				if ( errno != EINTR )
				{
					rest_log_debug("rest server select interrupt errno != EINTR");
					break;
				}
				else	continue;
			}
			else if ( sptr->active )
			{
				rest_log_debug( "server: accept message" );
				if (!( rest_accept_message( sptr->active, port ) ))
					continue;
				else	break;
			}
			else if ( sptr->activex )
			{
				if (sptr->method.extension)
					if (!( (*sptr->method.extension)(sptr->method.instance,sptr,sptr->activex) ))
						rest_drop_extension( sptr, sptr->activex );
				continue;
			}
			else
			{
				rest_log_debug( "server: accept connection" );
			 	if (!( rest_accept_connection( sptr ) ))
					continue;
				else	break;
			}
		}
		if ( check_verbose() )
			printf("   REST Server %s : shuting down\n",sptr->name);
		rest_liberate_server( sptr );
		return(0);
	}
}

	/* ------------- */
#endif	/* _restserver_c */
	/* ------------- */


