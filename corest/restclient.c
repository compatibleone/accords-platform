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
#ifndef	_restclient_c
#define	_restclient_c

#include "standard.h"
#include "rest.h"
#include "restpublic.h"
#include "url.h"
#include "url.c"
#include "restclient.h"
#include "restlog.h"

/*	-------------------------------------------------	*/
/*		s e r v i c e _ p r e f i x _ u r l		*/
/*	-------------------------------------------------	*/
/*	called to check the presence of a service prefix	*/
/*	for the provided url or to prepend the default		*/
/*	prefix as provided.					*/
/*	-------------------------------------------------	*/
public	char *	service_prefix_url( char * url, char * prefix )
{
	char *	sptr;
	char *	result=(char *) 0;

	if (!( url )) 
		return( url );
	else if (!( prefix ))
		return( url );
	else	sptr = url;

	/* ------------------------------------ */
	/* step over an eventual service prefix */
	/* ------------------------------------ */
	while (( *sptr ) && ( *sptr != ':' ) && ( *sptr != '.') && ( *sptr != '/')) sptr++;


	/* ---------------------------------------- */
	/* check for and detect host name or number */
	/* ---------------------------------------- */
	if (( *sptr == ':' ) && ( *(sptr+1) == '/' ))
		return( url );

	/* ---------------------------------------- */
	/* reallocate memory for prefix url storage */
	/* ---------------------------------------- */
	else if (!( result = allocate( strlen( url ) + strlen( prefix ) + 4 ) ))
		return( liberate( url ) );
	else
	{
		sprintf( result, "%s://%s",prefix,url);
		liberate( url );
		return( result );
	}
}

/*	-------------------------------------------------	*/
/*			a n a l y s e _ u r l			*/
/*	-------------------------------------------------	*/
public	struct	url *	analyse_url( char * target )
{
	int	c;
	char	*	sptr;
	char	*	wptr;
	struct	url * uptr;

	if (!( uptr = allocate_url()))
		return( uptr );

	else if (!( wptr = allocate_string(target) ))
		return( uptr );

	sptr = wptr;

	/* consume an eventual service prefix */
	/* ---------------------------------- */
	while (( *sptr ) && ( *sptr != ':' ) && ( *sptr != '.') && ( *sptr != '/')) sptr++;

	if (!( *sptr ))
	{
		if (!( uptr->object = allocate_string( wptr ) ))
			return( uptr );
		else	return( uptr );
	}
	else if ( *sptr == ':' )
	{
		*(sptr++)=0;
		if (!( uptr->service = allocate_string( wptr ) ))
			return( uptr );
		wptr = sptr;
	}	

	/* consume an eventual host name */
	/* ----------------------------- */
	if ((*sptr == '/') && (*(sptr+1) == '/'))
	{
		sptr+= 2;
		wptr = sptr;
		while (( *sptr ) && ( *sptr != ':' ) && ( *sptr != '/')) sptr++;
		c = *sptr;
		*sptr = 0;

		if (!( uptr->host = allocate_string( wptr ) ))
			return( uptr );
		else if (!( c ))
			return( uptr );
		else if ( c == '/' )
			*sptr = c;
		else if ( c == ':' )
		{
			sptr++;
			uptr->port = atoi(sptr);
			while (( *sptr ) && ( *sptr != '/')) sptr++;
			if (!( *sptr ))
				return( uptr );
		}	
		wptr = sptr;
	}
	if (!( uptr->object = allocate_string( wptr ) ))
		return( uptr );
	else	return( uptr );
}

/*	-------------------------------------------------	*/
/*			s h o w _ u r l	 			*/
/*	-------------------------------------------------	*/
private	void	show_url( struct url * uptr )
{
	if ( uptr )
	{
		printf("url {\n");
		if ( uptr->service )
			printf("service: %s\n",uptr->service);
		if ( uptr->host )
			printf("host: %s\n",uptr->host);
		if ( uptr->port )
			printf("port: %u\n",uptr->port);
		if ( uptr->object )
			printf("object: %s\n",uptr->object);
		printf("}\n");
	}
	return;
}	

/*	-------------------------------------------------	*/
/*		r e p l a c e _ u r l _ o b j e c t		*/
/*	-------------------------------------------------	*/
public	char 	  * replace_url_object( struct url * uptr, char * xptr )
{
	if (!( uptr ))
		return( (char *) 0);
	else
	{
		if ( uptr->object )
			uptr->object = liberate( uptr->object );
		return((uptr->object = allocate_string( xptr )));
	}
}

/*	-------------------------------------------------	*/
/*		     s e r i a l i s e  _ u r l			*/
/*	-------------------------------------------------	*/
public char 	  * serialise_url( struct url * uptr, char * xptr )
{
	char	buffer[8192];
	sprintf(buffer,"%s://%s:%u%s%s",
		uptr->service,uptr->host,uptr->port,
		(uptr->object ? uptr->object : "" ),
		( xptr ? xptr : "" ));
	return( allocate_string( buffer ) );
}

/*	-------------------------------------------------	*/
/*		s e r i a l i s e  _ u r l _ h o s t		*/
/*	-------------------------------------------------	*/
public char 	  * serialise_url_host( struct url * uptr )
{
	char	buffer[8192];
	sprintf(buffer,"%s://%s:%u",uptr->service,uptr->host,uptr->port);
	return( allocate_string( buffer ) );
}

/*	----------------------------------------------------	*/
/*	s e r i a l i s e  _ u r l _ h o s t _ n o _ p o r t	*/
/*	----------------------------------------------------	*/
public char 	  * serialise_url_host_no_port( struct url * uptr )
{
	char	buffer[8192];
	sprintf(buffer,"%s://%s",uptr->service,uptr->host);
	return( allocate_string( buffer ) );
}

/*	-------------------------------------------------	*/
/*	  s e r i a l i s e  _ u r l _ h o s t _ p o r t	*/
/*	-------------------------------------------------	*/
public char 	  * serialise_url_host_port( struct url * uptr )
{
	char	buffer[8192];
	sprintf(buffer,"%s:%u",uptr->host,uptr->port);
	return( allocate_string( buffer ) );
}

/*	-------------------------------------------------	*/
/*		     a p p e n d  _ u r l			*/
/*	-------------------------------------------------	*/
/*	appends the second parameter to the first one and	*/
/*	liberates the first one then returns allocation		*/
/*	of new string						*/
/*	-------------------------------------------------	*/
public char 	  * append_url( char * sptr, char * xptr )
{
	char	*	rptr;
	if (!( sptr ))
		return( sptr );
	else if (!( xptr ))
		return( sptr );
	else if (!( rptr = allocate( strlen( sptr ) + strlen( xptr ) + 1 )))
		return( sptr );
	else 
	{
		strcat(strcpy(rptr,sptr),xptr);
		liberate( sptr );
		return( rptr );
	}
}

/*	-------------------------------------------------	*/
/*		     v a l i d a t e _ u r l			*/
/*	-------------------------------------------------	*/
public	struct	url * validate_url( struct url * uptr )
{

	/* show_url( uptr ); */

	/* check host */
	if (!( uptr->host ))
		return( liberate_url( uptr ) );

	/* check port */
	if (!( uptr->port ))
	{
		if (!( uptr->service ))
			return( liberate_url( uptr ) );
		else if (!( strcmp(uptr->service,"http") ))
			uptr->port = 80;
		else if (!( strcmp(uptr->service,"https") ))
			uptr->port = 443;
		else	return( liberate_url( uptr ) );
	}

	/* check tls */
	if (( uptr->service )
	&&  (!( strcmp(uptr->service,"https") )))
		uptr->tls = allocate_string("security/tls.xml");
	else	uptr->tls = (char *) 0;

	return( uptr );
}


/*	------------------------------------------------	*/
/*	    r e s t _ a l l o c a t e _ c l i e n t		*/
/*	------------------------------------------------	*/
private	struct	rest_client * rest_allocate_client()
{
	struct	rest_client * rptr;
	if (!( rptr = allocate_rest_client() ))
		return( rptr );
	else if (!( rptr->buffer = allocate((rptr->buffersize = _MAX_RESTBUFFER)) ))
		return( liberate_rest_client( rptr ) );
	else	
	{
		memset( &rptr->net, 0, sizeof( struct connection ) );
		return( rptr );
	}
}

/*	------------------------------------------------	*/
/*	    r e s t _ l i b e r a t e _ c l i e n t 		*/
/*	------------------------------------------------	*/
private	struct	rest_client * rest_liberate_client( struct rest_client * cptr )
{
	if ( cptr )
	{
		connection_close( &cptr->net, 1 );
	}
	return( liberate_rest_client( cptr ) );
}		

/*	------------------------------------------------	*/
/*		r e s t _ o p e n _ c l i e n t			*/
/*	------------------------------------------------	*/
private	struct rest_client * 	rest_open_client( char * host, int port, char * tls )
{
	struct rest_client *  cptr;
	char	buffer[1024]; 
	if (!( port ))
		return((struct rest_client*) 0); 
	else if (!( cptr = rest_allocate_client() ))
	{
		if ( check_debug() )
			failure(27,"rest","allocate client");
		return( cptr );
	}
	else if ((cptr->net.socket = socket_create(get_socket_type(), SOCK_STREAM, 0  )) < 0)
	{
		if ( check_debug() )
			failure(errno,"socket_create","errno");
		return( rest_liberate_client( cptr ) );
	}
	else if (!( socket_connect( cptr->net.socket, host, port  ) ))
	{
		if ( check_debug() )
		{
			sprintf(buffer,"socket_connect(%s:%u)",host,port);
		        failure(errno,buffer,strerror(errno));
		}
		return( rest_liberate_client( cptr ) );
	}
	else if (!( tls ))
		return( cptr );

	else if (!( cptr->tlsconf = tls_configuration_load( tls ) ))
	{
		if ( check_debug() )
			failure(27,"rest","tls configuration");
		return( rest_liberate_client( cptr ) );
	}
	else
	{
		tls_configuration_use( cptr->tlsconf );
		if (!( tls_client_handshake( &cptr->net, cptr->tlsconf->option ) ))
			return( rest_liberate_client( cptr ) );
		else	return( cptr );
	}
}

/*	------------------------------------------------	*/
/*	    r e s t _ t r y _ o p e n _ c l i e n t		*/
/*	------------------------------------------------	*/
/*	attempts to open a client connection using coscs	*/
/*	and needs to do very careful connect polling to		*/
/*	avoid crashing the:					*/
/*	A) callers TCP stack					*/
/*	B) the targets router					*/
/*	C) the targets network address translation		*/
/*	D) the target machines TCP stack			*/
/*	E) anyone else who might be in the way			*/
/*	------------------------------------------------	*/
private	struct rest_client * 	rest_try_open_client( char * host, int port, char * tls, int timeout, int retry )
{
	struct rest_client *  cptr;
	char	buffer[1024]; 
	int	status;

	if (!( port ))
		return((struct rest_client*) 0); 

	else if (!( cptr = rest_allocate_client() ))
	{
		if ( check_debug() )
			failure(27,"rest","allocate client");
		return( cptr );
	}
	else if ((cptr->net.socket = socket_create(get_socket_type(), SOCK_STREAM, 0  )) < 0)
	{
		if ( check_debug() )
			failure(errno,"socket_create","errno");
		return( rest_liberate_client( cptr ) );
	}
	while ( retry-- )
	{
		if (( status = socket_try_connect( cptr->net.socket, host, port, ( timeout > 1 ? timeout / 2 : 1 ) )) == 1)
		{
			/* ------- */
			/* success */
			/* ------- */
			break;
		}
		else if ( status == 0 )
		{
			/* ------- */
			/* failure */
			/* ------- */
			sleep(( timeout > 1 ? timeout / 2 : 1 ));
			continue;
		}
		else
		{
			/* --------------------- */
			/* unrecoverable failure */
			/* --------------------- */
			return( rest_liberate_client( cptr ) );
		}
	}

	/* -------------------------------------- */
	/* if the number of retries has timed out */
	/* -------------------------------------- */
	if (!( retry ))
		return( rest_liberate_client( cptr ) );
	else
	{
		/* ------------------------------------ */
		/* drop this connection it may be dirty */
		/* ------------------------------------ */
		cptr = rest_liberate_client( cptr );
		/* ------------------------ */
		/* allow the dust to settle */
		/* ------------------------ */
		sleep(2);
		/* ----------------------- */
		/* and try a standard open */
		/* ----------------------- */
		return( rest_open_client( host, port, tls ) );
	}
}

/*	------------------------------------------------	*/
/*	   r e s t _ c l i e n t _ r e s p o n s e		*/
/*	------------------------------------------------	*/
private	struct	rest_response *	rest_client_response( int status, char * message, char * nptr )
{
	struct 	rest_response	* aptr;
	struct	rest_header 	* hptr;

	if (!( aptr = allocate_rest_response() ))
		return( aptr );
	else if (!( hptr = rest_response_header( aptr, _HTTP_USER_AGENT, nptr ) ))
		return( aptr );
	else	return( rest_html_response( aptr, status, message ) );
}

/*	------------------------------------------------	*/
/*	   r e s t _ c l i e n t _ r e q u e s t 		*/
/*	------------------------------------------------	*/
public	struct	rest_request * rest_client_request(
			struct rest_client * cptr,
			char * method,
			struct url * uptr,
			char * agent )
{
	struct	rest_header * hptr;
	struct	rest_request * rptr;
	rest_log_send_request( agent, method, uptr );
	if (!( rptr = allocate_rest_request() ))
		return( rptr );
	else if (!( rptr->method = allocate_string( method )))
		return( liberate_rest_request( rptr ) );
	else if (!( rptr->object = allocate_string( ( uptr->object ? uptr->object : "/" ) )))
		return( liberate_rest_request( rptr ) );
	else if (!( rptr->version = allocate_string( "HTTP/1.1" ) ))
		return( liberate_rest_request( rptr ) );
	else if (!( hptr = add_rest_header( rptr ) ))
		return( liberate_rest_request( rptr ) );
	else if (!( hptr->name = allocate_string( _HTTP_HOST ) ))
		return( liberate_rest_request( rptr ) );
	else if (!( hptr->value = allocate_string( uptr->host ) ))
		return( liberate_rest_request( rptr ) );
	else if (!( hptr = add_rest_header( rptr ) ))
		return( liberate_rest_request( rptr ) );
	else if (!( hptr->name = allocate_string( _HTTP_USER_AGENT ) ))
		return( liberate_rest_request( rptr ) );
	else if (!( hptr->value = allocate_string( agent ) ))
		return( liberate_rest_request( rptr ) );
	else
	{
		rest_log_header( agent, rptr->first, "-->" );
		return( rptr );
	}
}


/*	------------------------------------------------	*/
/*	   r e s t _ r e q u e s t _ s t a r t			*/
/*	------------------------------------------------	*/
private	void rest_request_start( struct rest_client * cptr )
{
	cptr->bytes = cptr->consumed = 0;
	return;
}

/*	------------------------------------------------	*/
/*	   r e s t _ r e q u e s t _ f l u s h 			*/
/*	------------------------------------------------	*/
private	int	show_rest_request=0;

public	int	rest_show_request(int v)
{
	int	r=show_rest_request;
	show_rest_request=v;
	return( r );
}

private	void rest_request_flush( struct rest_client * cptr )
{
	if ( cptr->bytes )
	{
		if ( show_rest_request )
		{
			printf("\n===Start Rest Request===\n%s===End Rest Request===\n",cptr->buffer );
		}
		rest_client_write( cptr );
		rest_request_start( cptr );
	}
	return;
}

/*	------------------------------------------------	*/
/*	   r e s t _ r e q u e s t _ s t r i n g		*/
/*	------------------------------------------------	*/
private	void rest_request_string( struct rest_client * cptr, char * message, char * term )
{
	int	bytes;
	if (!( cptr ))
		return;
	else if ((!( message )) || (!( term )))
		return;
	else	bytes = strlen(message) + strlen(term);

	if ( ( cptr->bytes + bytes ) >= cptr->buffersize )
		rest_request_flush( cptr );
	sprintf(cptr->buffer+cptr->bytes,"%s%s",message,term);
	cptr->bytes += bytes;
	return;
}

/*	------------------------------------------------	*/
/*	   r e s t _ r e q u e s t _ v a l u e			*/
/*	------------------------------------------------	*/
private	void rest_request_value(  struct rest_client * cptr, int value, char * term )
{
	char	buffer[64];
	sprintf(buffer,"%u",value);
	rest_request_string(cptr, buffer, term);
	return;
}

/*	------------------------------------------------	*/
/*	   r e s t _ r e q u e s t _ f i l e 	 		*/
/*	------------------------------------------------	*/
/*	the client will prepare a file for sending over		*/
/*	to the server in the body of the message.		*/
/*	------------------------------------------------	*/
private	void rest_request_file(  struct rest_client * cptr, char * filename )
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
			else	rest_request_flush( cptr );
		}
		fclose(h);
	}
	return;
}


/*	------------------------------------------------	*/
/*		r e s t _ s e n d _ r e q u e s t 		*/
/*	------------------------------------------------	*/
public	struct	rest_request * rest_send_request(
			struct rest_client * cptr,
			struct rest_request * rptr )
{
	struct	rest_header * hptr;


	if (!( cptr ))	return( rptr );
	if (!( rptr ))	return( rptr );

	if ( check_verbose() )
		printf("Rest Client Request : %s %s %s \n",rptr->method, rptr->object,rptr->version );

	rest_request_start( cptr );

	rest_request_string( cptr, rptr->method, " ");
	rest_request_string( cptr, rptr->object, " ");
	rest_request_string( cptr, rptr->version, "\r\n");

	for ( 	hptr = rptr->first;
		hptr != (struct rest_header *) 0;
		hptr = hptr->next )
	{
		rest_request_string( cptr, hptr->name   , ": ");
		rest_request_string( cptr, hptr->value  , "\r\n");
	}

	rest_request_string( cptr, "\0" , "\r\n");

	if ( rptr->body )
	{
		switch ( rptr->type )
		{
		case	_FILE_BODY	:
			rest_request_file( cptr, rptr->body );		
			break;
		case	_TEXT_BODY	:
			rest_request_string( cptr, rptr->body, "\0" );		
			break;
		}
	}

	rest_request_flush( cptr );
	return( rptr );
}

/*	---------------------------------------------------	*/
/*	r e s t _ c l i e n t _ c o n t e n t _ l e n g t h	*/
/*	---------------------------------------------------	*/
public	struct	rest_request * rest_client_content_length(
		struct rest_request * rptr,
		char * filename )
{
	struct	stat info;
	char	length[64];
	struct	rest_header * hptr;
	if (( hptr = rest_resolve_header( rptr->first, _HTTP_CONTENT_LENGTH )) != (struct rest_header *) 0)
		return( rptr );
	else if ( stat( filename,&info ) < 0 )
		return( rptr );
	else
	{
		sprintf(length,"%u",info.st_size);
		if (!( rest_request_header(rptr,_HTTP_CONTENT_LENGTH,length) ))
			return( rptr );
		else 	return( rptr );
	}
}

/*	------------------------------------------------	*/
/*	r e s t _ c l i e n t _ c o n t e n t _ t y p e		*/
/*	------------------------------------------------	*/
public	struct	rest_request * rest_client_content_type( 
		struct rest_request * rptr,
		char * filename )
{
	struct	rest_header * hptr;
	char *	sptr;
	char *	wptr;
	char * tptr;

	if (( hptr = rest_resolve_header( rptr->first, _HTTP_CONTENT_TYPE )) != (struct rest_header *) 0)
		return( rptr );
	for (	wptr = (char *) 0, sptr=filename;
		*sptr != 0;
		sptr++)
		if ( *sptr == '.' )
			wptr = sptr;
	if (!( wptr ))
		return( rptr );
	else if (!( strcmp( wptr,".xml"  )))
		tptr = "application/xml";
	else if (!( strcmp( wptr,".js"  )))
		tptr = "application/javascript";
	else if (!( strcmp( wptr,".css"  )))
		tptr = "text/css";
	else if (!( strcmp( wptr,".htm"  )))
		tptr = "text/html";
	else if (!( strcmp( wptr,".html" )))
		tptr = "text/html";
	else if (!( strcmp( wptr,".json" )))
		tptr = "application/json";
	else if (!( strcmp( wptr,".occi" )))
		tptr = "text/occi";
	else if (!( strcmp( wptr,".txt"  )))
		tptr = "text/plain";
	else if (!( strcmp( wptr,".gif"  )))
		tptr = "image/gif";
	else if (!( strcmp( wptr,".jpeg"  )))
		tptr = "image/jpeg";
	else if (!( strcmp( wptr,".jpg"  )))
		tptr = "image/jpeg";
	else if (!( strcmp( wptr,".png"  )))
		tptr = "image/png";
	else	return( rptr );

	if (!( rest_request_header(rptr,_HTTP_CONTENT_TYPE,tptr) ))
		return( rptr );
	else 	return( rptr );
}

/*	------------------------------------------------	*/
/*		r e s t _ c l i e n t _ b o d y			*/
/*	------------------------------------------------	*/
private	struct	rest_request *	rest_client_body( struct rest_request * rptr, char * filename )
{
	struct	rest_header * hptr;
	if (!( rptr ))
		return( rptr );
	else if (!( filename ))
		return( rptr );
	else if (!( strlen( filename ) ))
	{
		if (!( hptr = rest_resolve_header( rptr->first, _HTTP_CONTENT_LENGTH ) ))
		{
			rest_request_header( rptr, _HTTP_CONTENT_LENGTH, "0" );
		}
		return( rptr );
	}
	else if (!( rest_client_content_length( rptr, filename ) ))
		return( rptr );
	else if (!( rest_client_content_type( rptr, filename ) ))
		return( rptr );
	else
	{
		rest_request_body( rptr, filename, _FILE_BODY );
		return( rptr );
	}
}

/*	------------------------------------------------	*/
/*	      r e s t _ c l i e n t _ h e a d e r s		*/
/*	------------------------------------------------	*/
private	struct	rest_request *	rest_client_headers( struct rest_request * rptr, struct rest_header * hptr )
{
	if (!( rptr ))
		return( rptr );
	else if (!( hptr ))
		return( rptr );
	else
	{
		if (!( hptr->previous = rptr->last ))
			rptr->first = hptr;
		else 	hptr->previous->next = hptr;
		while ( hptr->next )
			hptr = hptr->next;
		rptr->last = hptr;
		return( rptr );
	}			
}

/*	------------------------------------------------	*/
/*	       r e s t _ c l i e n t _ h e a d e r 		*/
/*	------------------------------------------------	*/
public	struct	rest_header *	rest_client_header( struct rest_header * root, char * name, char * value )
{
	struct	rest_header *	hptr;

	return( root );
}


/*	--------------------------------------------------------	*/
/*	 r e s t _ c l i e n t _ a c c e p t _ r e s p o n s e		*/
/*	--------------------------------------------------------	*/
private	struct	rest_response *	rest_client_accept_response( struct rest_client * cptr, char * agent )
{
	struct	rest_response * aptr;
	struct	rest_header * hptr;
	if (!( aptr = rest_accept_response( cptr ) ))
		rest_log_recv_response( agent, aptr );
	else	rest_log_recv_response( agent, aptr );
	if ( show_rest_request )
	{
		if (!( aptr ))
			printf("\nNULL RESPONSE");
		else
		{
			printf("\n%s %u %s",
				( aptr->version ? aptr->version : "NULL/VERSION"),
				aptr->status,
				( aptr->message ? aptr->message : "NULL/MESSAGE"));
			for ( 	hptr=aptr->first;
				hptr != (struct rest_header *) 0;
				hptr = hptr->next )
			{
				printf("\n%s: %s",
					(hptr->name ? hptr->name : "NULL/NAME"),
					(hptr->value ? hptr->value : "NULL/VALUE"));
			}
		}
		printf("\n");
	}
	return( aptr );
}


/*	--------------------------------------------------------	*/
/*		r e s t _ c h e c k _ r e d i r e c t i o n		*/
/*	--------------------------------------------------------	*/
private char * rest_check_redirection( struct rest_response * rptr, char * target, struct rest_request * qptr )
{
	struct	rest_header * hptr;
	if (!( rptr ))
		return((char *) 0);
	else if ( rptr->status < 301 )
		return((char *) 0);
	else if ( rptr->status >= 302 )
		return((char *) 0);
	if (!( hptr = rest_resolve_header( rptr->first, _HTTP_LOCATION ) ))
		return((char *) 0);
	else if (!( target = allocate_string( hptr->value ) ))
		return( target );
	else	return( target );
}


/*	------------------------------------------------	*/
/*	 r e s t _ c l i e n t _ g e t _ r e q u e s t 		*/
/*	------------------------------------------------	*/
public	struct	rest_response * rest_client_get_request( 
		char * target, char * tls, char * agent, struct rest_header * hptr )
{
	struct	rest_response 	* aptr;
	struct	url		* uptr;
	struct	rest_client 	* cptr;
	struct	rest_request 	* rptr;
	char 	buffer[2048];
	
	while (1)
	{
	if ( check_debug() )
		printf("REST Client Request : GET %s \n",target );

	if (!( rest_valid_string( target ) ))
		return( rest_client_response( 599, "NULL URL", agent ) );

	if (!( uptr = analyse_url( target ) ))
		return( rest_client_response( 600, "Url Anaysis", agent ) );

	else if (!( uptr = validate_url( uptr )))
	{
		return( rest_client_response( 601, "Url Validation", agent ) );
	}

	else if (!( cptr = rest_open_client(uptr->host,uptr->port,tls)))
	{
		sprintf(buffer,"Rest Client Failure to open : %s:%u \n",uptr->host,uptr->port);
		rest_log_message( buffer );
		liberate_url( uptr );
		return( rest_client_response( 602, "Client Failure", agent ) );
	}
	else if (!( rptr = rest_client_request( cptr, "GET", uptr, agent )))
		return( rest_client_response( 603, "Request Creation", agent ) );
	else if (!( rptr = rest_client_headers( rptr, hptr ) ))
		return( rest_client_response( 603, "Request Headers", agent ) );
	else if (!( rptr = rest_send_request( cptr, rptr ) ))
		return( rest_client_response( 603, "Request Send", agent ) );
	else if (!( aptr = rest_client_accept_response( cptr, agent ) ))
		return( rest_client_response( 603, "Response Failure", agent ) );
	else if (!( target = rest_check_redirection( aptr, target, rptr ) ))
	{
		rptr = liberate_rest_request( rptr );
		return( aptr );
	}
	else
	{
		rptr = liberate_rest_request( rptr );
		aptr = rest_liberate_response( aptr );
	}
	}

}

/*	--------------------------------------------------------	*/
/*	 r e s t _ c l i e n t _ t r y _ g e t _ r e q u e s t 		*/
/*	--------------------------------------------------------	*/
public	struct	rest_response * rest_client_try_get_request( 
		char * target, char * tls, char * agent, struct rest_header * hptr, int timeout, int retry )
{
	struct	rest_response 	* aptr;
	struct	url		* uptr;
	struct	rest_client 	* cptr;
	struct	rest_request 	* rptr;
	char 	buffer[2048];
	
	while (1)
	{
	if ( check_debug() )
		printf("REST Client Request : GET %s \n",target );


	if (!( uptr = analyse_url( target ) ))
		return( rest_client_response( 600, "Url Anaysis", agent ) );

	else if (!( uptr = validate_url( uptr )))
	{
		return( rest_client_response( 601, "Url Validation", agent ) );
	}

	else if (!( cptr = rest_try_open_client(uptr->host,uptr->port,tls, timeout, retry)))
	{
		sprintf(buffer,"Rest Client Failure to open : %s:%u \n",uptr->host,uptr->port);
		rest_log_message( buffer );
		liberate_url( uptr );
		return( rest_client_response( 602, "Client Failure", agent ) );
	}
	else if (!( rptr = rest_client_request( cptr, "GET", uptr, agent )))
		return( rest_client_response( 603, "Request Creation", agent ) );
	else if (!( rptr = rest_client_headers( rptr, hptr ) ))
		return( rest_client_response( 603, "Request Headers", agent ) );
	else if (!( rptr = rest_send_request( cptr, rptr ) ))
		return( rest_client_response( 603, "Request Send", agent ) );
	else if (!( aptr = rest_client_accept_response( cptr, agent ) ))
		return( rest_client_response( 603, "Response Failure", agent ) );
	else if (!( target = rest_check_redirection( aptr, target, rptr ) ))
	{
		rptr = liberate_rest_request( rptr );
		return( aptr );
	}
	else
	{
		rptr = liberate_rest_request( rptr );
		aptr = rest_liberate_response( aptr );
	}
	}

}

/*	---------------------------------------------------	*/
/*	r e s t _ c l i e n t _ d e l e t e _ r e q u e s t 	*/
/*	---------------------------------------------------	*/
public	struct	rest_response * rest_client_delete_request( 
		char * target, char * tls, char * agent, struct rest_header * hptr )
{
	struct	rest_response 	* aptr;
	struct	url		* uptr;
	struct	rest_client 	* cptr;
	struct	rest_request 	* rptr;
	char 	buffer[2048];
	
	while(1)
	{
	if ( check_debug() )
		printf("REST Client Request : DELETE %s \n",target );

	if (!( uptr = analyse_url( target ) ))
		return( rest_client_response( 600, "Incorrect Url", agent ) );

	else if (!( cptr = rest_open_client(uptr->host,uptr->port,tls)))
	{
		sprintf(buffer,"Rest Client Failure to open : %s:%u \n",uptr->host,uptr->port);
		rest_log_message( buffer );
		liberate_url( uptr );
		return( rest_client_response( 602, "Client Failure", agent ) );
	}
	else if (!( rptr = rest_client_request( cptr, "DELETE", uptr, agent )))
		return( rest_client_response( 603, "Request Creation", agent ) );
	else if (!( rptr = rest_client_headers( rptr, hptr ) ))
		return( rest_client_response( 603, "Request Headers", agent ) );
	else if (!( rptr = rest_send_request( cptr, rptr ) ))
		return( rest_client_response( 603, "Request Send", agent ) );
	else if (!( aptr = rest_client_accept_response( cptr, agent ) ))
		return( rest_client_response( 603, "Response Failure", agent ) );
	else if (!( target = rest_check_redirection( aptr, target, rptr ) ))
	{
		rptr = liberate_rest_request( rptr );
		return( aptr );
	}
	else
	{
		rptr = liberate_rest_request( rptr );
		aptr = rest_liberate_response( aptr );
	}
	}
}

/*	---------------------------------------------------	*/
/*	  r e s t _ c l i e n t _ h e a d _ r e q u e s t 	*/
/*	---------------------------------------------------	*/
public	struct	rest_response * rest_client_head_request( 
		char * target, char * tls, char * agent, struct rest_header * hptr )
{
	struct	rest_response 	* aptr;
	struct	url		* uptr;
	struct	rest_client 	* cptr;
	struct	rest_request 	* rptr;
	char 	buffer[2048];
	
	while(1)
	{
	if ( check_debug() )
		printf("REST Client Request : HEAD %s \n",target );

	if (!( uptr = analyse_url( target ) ))
		return( rest_client_response( 600, "Incorrect Url", agent ) );

	else if (!( cptr = rest_open_client(uptr->host,uptr->port,tls)))
	{
		sprintf(buffer,"Rest Client Failure to open : %s:%u \n",uptr->host,uptr->port);
		rest_log_message( buffer );
		liberate_url( uptr );
		return( rest_client_response( 602, "Client Failure", agent ) );
	}
	else if (!( rptr = rest_client_request( cptr, "HEAD", uptr, agent )))
		return( rest_client_response( 603, "Request Creation", agent ) );
	else if (!( rptr = rest_client_headers( rptr, hptr ) ))
		return( rest_client_response( 603, "Request Headers", agent ) );
	else if (!( rptr = rest_send_request( cptr, rptr ) ))
		return( rest_client_response( 603, "Request Send", agent ) );
	else if (!( aptr = rest_client_accept_response( cptr, agent ) ))
		return( rest_client_response( 603, "Response Failure", agent ) );
	else if (!( target = rest_check_redirection( aptr, target, rptr ) ))
	{
		rptr = liberate_rest_request( rptr );
		return( aptr );
	}
	else
	{
		rptr = liberate_rest_request( rptr );
		aptr = rest_liberate_response( aptr );
	}
	}
}

/*	---------------------------------------------------	*/
/*	  r e s t _ c l i e n t _ p o s t _ r e q u e s t 	*/
/*	---------------------------------------------------	*/
public	struct	rest_response * rest_client_post_request( 
		char * target, char * tls, char * agent, char * filename, struct rest_header * hptr  )
{
	struct	rest_response 	* aptr;
	struct	url		* uptr;
	struct	rest_client 	* cptr;
	struct	rest_request 	* rptr;
	char 	buffer[2048];
	
	while(1)
	{
	if ( check_debug() )
		printf("REST Client Request : POST %s \n",target );

	if (!( uptr = analyse_url( target ) ))
		return( rest_client_response( 600, "Incorrect Url", agent ) );

	else if (!( cptr = rest_open_client(uptr->host,uptr->port,tls)))
	{
		sprintf(buffer,"Rest Client Failure to open : %s:%u \n",uptr->host,uptr->port);
		rest_log_message( buffer );
		liberate_url( uptr );
		return( rest_client_response( 602, "Client Failure", agent ) );
	}
	else if (!( rptr = rest_client_request( cptr, "POST", uptr, agent )))
		return( rest_client_response( 603, "Request Creation", agent ) );
	else if (!( rptr = rest_client_headers( rptr, hptr ) ))
		return( rest_client_response( 603, "Request Headers", agent ) );
	else if (!( rptr = rest_client_body( rptr, filename ) ))
		return( rest_client_response( 603, "Request Body", agent ) );
	else if (!( rptr = rest_send_request( cptr, rptr ) ))
		return( rest_client_response( 603, "Request Send", agent ) );
	else if (!( aptr = rest_client_accept_response( cptr, agent ) ))
		return( rest_client_response( 603, "Response Failure", agent ) );
	else if (!( target = rest_check_redirection( aptr, target, rptr ) ))
	{
		rptr = liberate_rest_request( rptr );
		return( aptr );
	}
	else
	{
		rptr = liberate_rest_request( rptr );
		aptr = rest_liberate_response( aptr );
	}
	}

}

/*	---------------------------------------------------	*/
/*	   r e s t _ c l i e n t _ p u t _ r e q u e s t 	*/
/*	---------------------------------------------------	*/
public	struct	rest_response * rest_client_put_request( 
		char * target, char * tls, char * agent, char * filename, struct rest_header * hptr  )
{
	struct	rest_response 	* aptr;
	struct	url		* uptr;
	struct	rest_client 	* cptr;
	struct	rest_request 	* rptr;
	char 	buffer[2048];
	
	while(1)
	{
	if ( check_debug() )
		printf("REST Client Request : PUT %s \n",target );

	if (!( uptr = analyse_url( target ) ))
		return( rest_client_response( 600, "Incorrect Url", agent ) );

	else if (!( cptr = rest_open_client(uptr->host,uptr->port,tls)))
	{
		sprintf(buffer,"Rest Client Failure to open : %s:%u \n",uptr->host,uptr->port);
		rest_log_message( buffer );
		liberate_url( uptr );
		return( rest_client_response( 602, "Client Failure", agent ) );
	}
	else if (!( rptr = rest_client_request( cptr, "PUT", uptr, agent )))
		return( rest_client_response( 603, "Request Creation", agent ) );
	else if (!( rptr = rest_client_headers( rptr, hptr ) ))
		return( rest_client_response( 603, "Request Headers", agent ) );
	else if (!( rptr = rest_client_body( rptr, filename ) ))
		return( rest_client_response( 603, "Request Body", agent ) );
	else if (!( rptr = rest_send_request( cptr, rptr ) ))
		return( rest_client_response( 603, "Request Send", agent ) );
	else if (!( aptr = rest_client_accept_response( cptr, agent ) ))
		return( rest_client_response( 603, "Response Failure", agent ) );
	else if (!( target = rest_check_redirection( aptr, target, rptr ) ))
	{
		rptr = liberate_rest_request( rptr );
		return( aptr );
	}
	else
	{
		rptr = liberate_rest_request( rptr );
		aptr = rest_liberate_response( aptr );
	}
	}

}

/*	---------------------------------------------------	*/
/*	  r e s t _ c l i e n t _ c o p y _ r e q u e s t 	*/
/*	---------------------------------------------------	*/
public	struct	rest_response * 
	rest_client_copy_request(
		char * target1, char * tls1, char * nptr1, struct rest_header * hptr1,
		char * target2, char * tls2, char * nptr2, struct rest_header * hptr2 )
{
	return((struct rest_response *) 0);
}

void dump_rest_client(FILE *h, struct rest_client *cptr)
{
	/* rest_client *cptr */
	fprintf(h, "client: %d %s\n", cptr->server->port, cptr->server->name);
}

#endif	/* _restclient_c */
	/* ------------- */


