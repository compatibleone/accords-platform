#ifndef	_abalssl_c
#define	_abalssl_c

#include <openssl/engine.h>
#include "allocate.h"
/*	-------------------------------------------------	*/
/*			A B A L    S S L			*/
/*	-------------------------------------------------	*/
/*								*/
/*	This file implements the various additional code	*/
/*	required for the operation of bdanet and  secure	*/
/*	socket layer connections.				*/
/*								*/
/*	-------------------------------------------------	*/
/*	SSL Options : 						*/
/*		2	force ssl debug				*/
/*		4	use ssl					*/
/*		8	verify peer				*/
/*		16	fail if no peer certificate		*/
/*		32	private key is DER not PEM		*/
/*		64	certificate is DER not PEM		*/
/*		128	SSL v23 compatible mode			*/
/*		256	Inhibit Internal Certificate Check	*/
/*	-------------------------------------------------	*/

#define	_SSL_DEBUG		2
#define	_USE_SSL		4
#define	_REQUEST_PEER		8
#define	_REQUIRE_PEER		16
#define	_DER_KEY		32
#define	_DER_CERTIFICATE	64
#define	_SSL_COMPATIBLE		128
#define	_SSL_INTERNAL		256
#define _OPENSSL_ENGINE         512

private	pthread_mutex_t security_control = PTHREAD_MUTEX_INITIALIZER;

private	int	SSL_READY=0;
private	int	ssl_contexts=0;
private	int	total_contexts=0;
private	int	ssl_debug;

#define	Portable_srandom	srandom
#define	SSL_debug 		check_debug()
#define	SIGNAL_verbose 		check_verbose()

#define	_DEFAULT_KEYFILE  	"server.pem"
#define	_DEFAULT_PASSWORD 	"password"
#define	_DEFAULT_CERTIFICATE	"root.pem"
#define	_DEFAULT_CA_LIST	"calist.pem"

#define ENGINE_ID               "pkcs11"

char *	SslKeyFile=(char *) 0;
char *	SslPassWord=(char *) 0;
char *	SslCertFile=(char *) 0;
char *	SslCaList=(char *) 0;

public	void	security_lock(int h, char * f)
{
	char	buffer[1024];
	sprintf(buffer,"%s_security_lock(%u)",f,h);
	rest_debug_log_file( buffer );
	pthread_mutex_lock( &security_control );
	return;
}

public	void	security_unlock(int h, char * f)
{
	char	buffer[1024];
	sprintf(buffer,"%s_security_unlock(%u)",f,h);
	rest_debug_log_file( buffer );
	pthread_mutex_unlock( &security_control );
	return;
}

private	void	tls_show_errors( char * message)
{
	char SSL_ErrorBuf[1024];
	int	sslerr;
	if (( check_debug() ) || ( ssl_debug ))
	{
		while ((sslerr = ERR_get_error())) 
			if ( ERR_error_string( sslerr, SSL_ErrorBuf ) ) 
				printf("SSL ERR(%s) : %lu : %s \n",message,sslerr,SSL_ErrorBuf );
	}
	return;
}

private CONNECTIONPTR close_connection( CONNECTIONPTR cptr )
{
	if ( cptr->closure )
	{
		(*cptr->closure)(cptr->parent);
	}
	return( cptr );
}

#ifdef UNIX
private	__sighandler_t	socketstate=SIG_DFL;
#endif

private	int	sockethit=0;


#ifdef UNIX

public	int	is_socket_hit()
{
	return( sockethit );
}

public	void	socketcatcher( int signal )
{
	sockethit++;
	return;
}

public	void	start_socket_catcher(int socket, char * message )
{
	sockethit=0;
	socketstate = signal(SIGPIPE,socketcatcher);
	return;
}

public	void	close_socket_catcher(int socket, char * message, int ecode )
{
	(void) signal(SIGPIPE,socketstate);
	socketstate = SIG_DFL;
	if ( sockethit ) 
	{
		if ( SIGNAL_verbose )
			printf("socket catcher failure at %lu : %s : %u \r\n",time((long) 0),message,ecode);
	}
	return;
}

#else

#define	is_socket_hit() 0
#define start_socket_catcher(a,b)
#define close_socket_catcher(a,b,c)

#endif


public	int	socketreader( int handle, char * buffer, int length )
{
	int	status;
	start_socket_catcher(handle,"socket read");
	*buffer = 0;
	status = read( handle, buffer, length );
	close_socket_catcher(handle,"socket read",errno);
	return( status );
}

public	int	socketwriter( int handle, char * buffer, int length )
{
	int	status;
	start_socket_catcher(handle,"socket write");
	status = write( handle, buffer, length );
	close_socket_catcher(handle,"socket write",errno);
	return( status );
}

/*	------------------------------------------------	*/
/*	h t t p s _ u s e _ e n c r y p t i o n ( aptr )	*/
/*	------------------------------------------------	*/
/*	Called from the configuration and initialisation	*/
/*	options analyser to submit keyfile name.		*/
/*	Returns zero to indicate success or other error.	*/
/*	------------------------------------------------	*/

public	int	https_use_encryption(	char *	aptr	)
{
	if ((!( aptr )) || (!( *aptr )))
		return(0);

	if (( SslKeyFile ) && (!( strcmp( SslKeyFile, aptr ) )))
		return( 0 );

	if ( SslKeyFile ) 
	{
		liberate( SslKeyFile );
		SslKeyFile = (char *) 0;
	}
	SslKeyFile = allocate_string( aptr );

	if (!( SslKeyFile ))
		return( 27 );
	else	return( 0  );
}

/*	--------------------------------------------------	*/
/*	h t t p s _ u s e _ c e r t i f i c a t e ( aptr )	*/
/*	--------------------------------------------------	*/
/*	Called from the configuration and initialisation	*/
/*	options analyser to submit certification filename.	*/
/*	Returns zero to indicate success or other error.	*/
/*	--------------------------------------------------	*/

public	int	https_use_certificate(	char *	aptr	)
{
	if ((!( aptr )) || (!( *aptr )))
		return(0);
	if (( SslCertFile ) && (!( strcmp( SslCertFile, aptr ) )))
		return( 0 );
	if ( SslCertFile ) 
	{
		liberate( SslCertFile );
		SslCertFile = (char *) 0;
	}
	SslCertFile = allocate_string( aptr );

	if (!( SslCertFile ))
		return( 27 );
	else	return( 0  );
}

/*	--------------------------------------------------	*/
/*	   h t t p s _ u s e _ C A _ l i s t ( aptr )   	*/
/*	--------------------------------------------------	*/
/*	Called from the configuration and initialisation	*/
/*	options analyser to submit the certification root	*/
/*	Authority Chain File.					*/
/*	--------------------------------------------------	*/

public	int	https_use_CA_list(	char *	aptr	)
{
	if ((!( aptr )) || (!( *aptr )))
		return(0);
	if (( SslCaList ) && (!( strcmp( SslCaList, aptr ) )))
		return( 0 );
	if ( SslCaList ) 
	{
		liberate( SslCaList );
		SslCaList = (char *) 0;
	}
	SslCaList = allocate_string( aptr );

	if (!( SslCaList ))
		return( 27 );
	else	return( 0  );
}

/*	------------------------------------------------	*/
/*	  h t t p s _ u s e _ p a s s w o r d ( aptr )		*/
/*	------------------------------------------------	*/
/*	Called from the configuration and initialisation	*/
/*	options analyser to submit password value.		*/
/*	Returns zero to indicate success or other error.	*/
/*	------------------------------------------------	*/

public	int	https_use_password(	char *	aptr	)
{
	if ((!( aptr )) || (!( *aptr )))
		return(0);

	if (( SslPassWord ) && (!( strcmp( SslPassWord, aptr ) )))
		return( 0 );

	if ( SslPassWord ) 
	{
		liberate( SslPassWord );
		SslPassWord = (char *) 0;
	}
	SslPassWord = allocate_string_secure( aptr );

	if (!( SslPassWord  ))
		return( 27 );
	else	return( 0  );
}


/*	------------------------------------------------	*/
/*	   s s l s o c k e t r e a d e r ( h, b ,l )       	*/
/*	------------------------------------------------	*/
/*	This function performs all socket reading for		*/
/*	secure socket layer connections.			*/
/*	------------------------------------------------	*/

public	int	sslsocketreader( 
	SSL * handle,
	char * buffer,
	int length )
{
	int	status;
	start_socket_catcher(0,"ssl read");
	if ((status = SSL_read( handle, buffer, length )) >= 0)
		*(buffer+status) = 0;
	else if ( status == -1 )
	{
		if ( check_debug() )
		{
			printf("socket reader failure at %lu : %s : %u \r\n",
				time((long) 0),"sslread",
				SSL_get_error( handle,status ) );
		}
	}
	close_socket_catcher(0,"ssl read",SSL_get_error(handle,status));
	return( status );
}

/*	------------------------------------------------	*/
/*	   s s l s o c k e t w r i t e r ( h, b ,l )       	*/
/*	------------------------------------------------	*/
/*	This function performs all socket writing for		*/
/*	secure socket layer connections.			*/
/*	------------------------------------------------	*/

public	int	sslsocketwriter( 
	SSL * handle,
	char * buffer,
	int length )
{
	int	status;
	start_socket_catcher(0,"ssl write");
	status = SSL_write( handle, buffer, length );
	close_socket_catcher(0,"ssl write",SSL_get_error(handle,status));
	return( status );
}

/*	------------------------------------------------	*/
/*	     s s l _ t c p _ r e a d b ( h, b ,l ) 	      	*/
/*	------------------------------------------------	*/
/*	This function reads and returns one byte from a		*/
/*	secure socket layer connection.				*/
/*	------------------------------------------------	*/

public	int	ssl_tcp_readb( 	SSL * 	handle )
{
	char	c;
	if ( sslsocketreader( handle, ( char *) & c, 1 ) == 1 ) {
		return( (c & 0x00FF) );
		}
	else	return( 0xFF00 );
}


/*	------------------------------------------------	*/
/*	     s s l _ t c p _ r e a d w ( h, b ,l ) 	      	*/
/*	------------------------------------------------	*/
/*	This function reads and returns one word from a		*/
/*	secure socket layer connection.				*/
/*	------------------------------------------------	*/

public	int	ssl_tcp_readw( 	SSL * 	handle )
{
	 int	v;
	v = ssl_tcp_readb( handle );
	v <<= 8; v &= 0xFF00;
	v |= ssl_tcp_readb( handle );
	return( (v & 0xFFFF) );
}

/*	------------------------------------------------	*/
/*	     s s l _ t c p _ w r i t e ( h, b ,l ) 	      	*/
/*	------------------------------------------------	*/
/*	This function writes the buffer length block to		*/
/*	a secure socket layer connection.				*/
/*	------------------------------------------------	*/

public 	int 	ssl_tcp_write( 
	SSL *	handle,
	char *	bptr,
	int	blen )
{
	blen = sslsocketwriter( handle, bptr, blen );
	return( blen );
}

/*	------------------------------------------------	*/
/*	     s s l _ t c p _ r e a d   ( h, b ,l ) 	      	*/
/*	------------------------------------------------	*/
/*	This function reads the buffer length block from	*/
/*	a secure socket layer connection.				*/
/*	------------------------------------------------	*/

public	int	ssl_tcp_read( 
	SSL *	handle,
	char *	bptr,
	int	blen )
{
#define	_OPTIMISED_HTTP
#ifdef	_OPTIMISED_HTTP
	return( sslsocketreader( handle, bptr, blen ) );
#else
	while ( blen > 0 ) 
	{
		*(bptr++) = ssl_tcp_readb( handle );
		blen--;
	}
	return( blen );
#endif
}

public	int	ssl_tcp_accept( SSL *	handle )
{
	int	status;
	start_socket_catcher(0,"ssl accept");
	status = SSL_accept( handle );
	close_socket_catcher(0,"ssl accept",errno);
	if ( SSL_debug ) 
	{
		printf("SSL_accept(pid=%u,h=%x) => %d \r\n",getpid(),handle,status);
	}
	return( status );
}

public	int	ssl_tcp_connect( SSL *	handle )
{
	int	status;
	if ( SSL_debug ) 
	{
		printf("SSL_connect(pid=%u,h=%x)\r\n",getpid(),handle);
	}
	start_socket_catcher(0,"ssl connect");
	status = SSL_connect( handle );
	close_socket_catcher(0,"ssl connect",errno);
	return( status );
}

private	int	do_ssl_shutdown=0;
public	int	ssl_tcp_shutdown( SSL *	handle )
{
	int	status=0;
	if ( SSL_debug ) 
	{
		printf("SSL_shutdown(pid=%u,h=%x)\r\n",getpid(),handle);
	}
	start_socket_catcher(-1,"ssl shutdown");
	if ( do_ssl_shutdown )
	{
		SSL_shutdown(handle);
		SSL_clear(handle);
	}
	close_socket_catcher(-1,"ssl shutdown",errno);
	SSL_free( handle );
	return( status );
}

public	int	connection_shutdown( CONNECTIONPTR cptr )
{
	int	status;
	if ( cptr->object ) 
	{
		status = ssl_tcp_shutdown( cptr->object );
		cptr->object = (void*) 0;
		return( status );
	}
	else	return( shutdown( cptr->socket, 2 ) );
}

/*	------------------------------------------------	*/
/*	c l o s e _ s s l _ c o n n e c t i o n ( cptr )     	*/
/*	------------------------------------------------	*/
/*	Performs all necessary operations required for		*/
/*	the correct closure of a secure socket connection.	*/
/*	------------------------------------------------	*/

void	close_ssl_connection( 
	CONNECTIONPTR	cptr,
	int		mode)
{
	/* Check for an SSL object */
	/* ----------------------- */
	security_lock( cptr->socket, "close" );

	/* printf("close_ssl_connection( %lx, %u )\n",cptr,mode); */

	if ( cptr->newobject ) 
	{
		ssl_tcp_shutdown(cptr->newobject);
		cptr->newobject = (void *) 0;
	}
	security_unlock( cptr->socket, "close" );

	if ( mode )
	{
		security_lock( cptr->socket, "close" );
		connection_shutdown( cptr );

		if ( cptr->context ) 
		{
			/* if ( ssl_contexts ) { printf("old SSL CTX : %lx (%u/%u)\n",cptr,--ssl_contexts,total_contexts); } */
			SSL_CTX_free( cptr->context );
			cptr->context = (void *) 0;
		}

		security_unlock( cptr->socket, "close" );
	}
	return;
}

public	int	connection_read( CONNECTIONPTR cptr, char * buffer, int length )
{
	int	result=0;
	if ( cptr->newobject )
	{
		result = sslsocketreader( cptr->newobject, buffer, length );
	}
	else if ( cptr->object )
	{
		result = sslsocketreader( cptr->object, buffer, length );
	}
	else	result = socketreader( cptr->socket, buffer, length );
	if ( result != -1 )
	{
		cptr->inbound += result;
	}
	return( result );
}

public	int	connection_name( CONNECTIONPTR cptr, char * buffer, int buflen )
{
	unsigned char *	vptr;
	vptr = (unsigned char *) &cptr->remote;
	vptr += sizeof( int );
	sprintf(buffer,"%u.%u.%u.%u",*vptr,*(vptr+1),*(vptr+2),*(vptr+3));	
	return( strlen( buffer ) );
}

public	int	connection_write( CONNECTIONPTR cptr, char * buffer, int length )
{
	if ( check_debug() ) printf( "connection_write(%s,%u)\n",buffer,length);

	cptr->outbound += length;

	if ( cptr->newobject )
		return( sslsocketwriter( cptr->newobject, buffer, length ));
	else if ( cptr->object )
		return( sslsocketwriter( cptr->object, buffer, length ));
	else	return(    socketwriter( cptr->socket, buffer, length ));
}

public	int	connection_close( CONNECTIONPTR cptr, int mode )
{
	if ( cptr->object ) 
		close_ssl_connection( cptr, mode );
	return( socket_close( cptr->socket ) );
}


/*	--------------------------------	*/
/*	c h e c k _ s s l _ r e a d y ()	*/
/*	--------------------------------	*/
/* 	Check for SSL library ready for use 	*/
/*	--------------------------------	*/

void	check_ssl_ready()
{
	if (!( SSL_READY )) 
	{
	        SSL_load_error_strings();                /* readable error messages */
	        SSL_library_init();                      /* initialize library */
	        Portable_srandom(177);
		SSL_READY=1;
	}
	return;
}


/*	-----------------------------------------------------	*/
/*	s s l _ p a s s w o r d _ c a l l b a c k (b,n,rw,ud)	*/
/*	-----------------------------------------------------	*/
/*	This function implements, as its name suggests, the	*/
/*	password callback for the lowlevel SSL routines.	*/
/*	-----------------------------------------------------	*/

private	int	ssl_password_callback(char *buf,int num, int rwflag,void *userdata)
{
	if (!( SslPassWord ))
		return(0);
 	else if(num<strlen(SslPassWord)+1)
      		return(0);
	else	
	{
		strcpy(buf,SslPassWord);
		return(strlen(SslPassWord));
	}
}

/*	-----------------------------------------------------------	*/
/*	   s e t _ s s l _ c e r t i f i c a t e _ a n d _ k e y	*/
/*	-----------------------------------------------------------	*/
CONNECTIONPTR	set_ssl_certificate_and_key( CONNECTIONPTR cptr, int mode )
{
	int	oof=0;
	return( cptr );
}

/*	-----------------------------------------------------------	*/
/*		t l s _ v e r i f y _ c a l l b a c k			*/
/*	-----------------------------------------------------------	*/
/*	this function will be called back at each certificate to be	*/
/*	informed as to how to proceed incase of failure and to have	*/
/*	application certificate verification performed as well.		*/
/*	-----------------------------------------------------------	*/
private	int	tls_client_verify_callback( int preverify, X509_STORE_CTX * certificate )
{
	if ( check_debug() )
		printf("tls_client_verify_callback(%u)\n",preverify);
	return(1);	/* never fail : for now */
}

private	int	tls_server_verify_callback( int preverify, X509_STORE_CTX * certificate )
{
	if ( check_debug() )
		printf("tls_server_verify_callback(%u)\n",preverify);
	return(1);	/* never fail : for now */
}

/*	-----------------------------------------------------------	*/
/*		t l s _ c h e c k _ c e r t i f i c a t e		*/
/*	-----------------------------------------------------------	*/
private	int	tls_check_certificate( X509_STORE_CTX * certificate, void * arg )
{
	if ( check_debug() )
		printf("tls_check_certificate()\n");

	return(1);	/* never fail : for now */
}

/*	------------------------------------------------	*/
/*	s e t u p _ e n g i n e (  )	*/
/*	------------------------------------------------	*/
/*	Initialise an engine		*/
/*	------------------------------------------------	*/

ENGINE *setup_engine(const char *engine)
        {
        ENGINE *e = NULL;

        if (engine)
	  {
	    ENGINE_load_builtin_engines();
	    if ((e = ENGINE_by_id(engine)) == NULL)
	      {
		if ( check_debug() )
			printf("engine: %s\n", engine);
		tls_show_errors( "invalid engine");
		return NULL;
	      }
	    if(!ENGINE_set_default(e, ENGINE_METHOD_ALL))
	      {
		if ( check_debug() )
			printf("engine: %s\n", ENGINE_get_id(e));
		tls_show_errors( "can't use that engine");
		ENGINE_free(e);
		return NULL;
	      }
	    /* Free our "structural" reference. */
	    ENGINE_free(e);
	  }
	return e;
	}

/*	------------------------------------------------	*/
/*	b u i l d _ s s l _ c o n t e x t ( cptr, mode )	*/
/*	------------------------------------------------	*/
/*	Builds and initialises a secure socket layer		*/
/*	control structure for both client and server 		*/
/*	operational modes.					*/
/*	------------------------------------------------	*/

private	int	ll_build_ssl_context(CONNECTIONPTR	cptr, int mode, int service )
{
	char 	buffer[1024];
	int	oof=0;
	void *	fptr=(void *) 0;
	ENGINE *e=NULL;
	EVP_PKEY *pkey=NULL;

	if ( SSL_debug )
	{
		sprintf(buffer,"build ssl context( x=%x, h=%u, m=%u, s=%u )",cptr,cptr->socket,mode,service);
		rest_log_message( buffer );
	}

	check_ssl_ready();

	/* ----------- */
	/* SSL OBJECT  */
	/* ----------- */
	if (!( SslKeyFile )) 
	{
		if ( https_use_encryption( _DEFAULT_KEYFILE ) )
		{
			close_connection( cptr );
			return(0);
		}
	}
			
	if (!( SslCertFile ))
	{
		if ( https_use_certificate( _DEFAULT_CERTIFICATE ) )
		{
			close_connection( cptr );
			return(0);
		}
	}

	if (!( SslCaList ))
	{
		if ( https_use_CA_list( _DEFAULT_CA_LIST ) )
		{
			close_connection( cptr );
			return(0);
		}
	}

	if (!( SslPassWord ))
	{
		if ( https_use_password( _DEFAULT_PASSWORD ) )
		{
			close_connection( cptr );
			return(0);
		}
	}

	if ( mode & _SSL_COMPATIBLE )
	{
		if (!( fptr = (void *) SSLv23_method() )) 
		{
			tls_show_errors( "SSLv23_method" );
			close_connection( cptr );
			return( 0 );
		}
	}
	else if (!( fptr = (void *) TLSv1_method() )) 
	{
		tls_show_errors( "TLSv1_method" );
		close_connection( cptr );
		return( 0 );
	}
	
	if (!(cptr->context = SSL_CTX_new(fptr) )) 
	{
		tls_show_errors( "SSL_CTX_new" );
		close_connection( cptr );
		return( 0 );
	}
	/* printf("new SSL CTX : %lx (%u/%u)\n",cptr,++ssl_contexts,++total_contexts); */

	SSL_CTX_set_mode (cptr->context, SSL_MODE_ENABLE_PARTIAL_WRITE);
	SSL_CTX_set_mode (cptr->context, SSL_MODE_AUTO_RETRY);

	if (!( mode & _SSL_INTERNAL ))
	{
		/* ----------------------------------------------- */
		/* set the custom certificat check callback method */
		/* ----------------------------------------------- */
		SSL_CTX_set_cert_verify_callback( cptr->context, tls_check_certificate, cptr );
	}
	else
	{
		/* ------------------------------------------- */
		/* set the standard certificate check callback */
		/* ------------------------------------------- */
		SSL_CTX_set_cert_verify_callback( cptr->context, NULL, cptr );
	}

	if ( mode )
	{
		if ( mode & _DER_CERTIFICATE )
		{
			if (!(oof=SSL_CTX_use_certificate_file(
				cptr->context,SslCertFile,SSL_FILETYPE_ASN1))) 
			{
				tls_show_errors( "SSL_use_certificate_chain_file" );
				close_connection( cptr );
				return( 0 );
			}
		}
	
		else if (!(oof = SSL_CTX_use_certificate_chain_file(cptr->context,SslCertFile))) 
		{
			tls_show_errors( "SSL_use_certificate_chain_file" );
			close_connection( cptr );
			return( 0 );
		}
	
		SSL_CTX_set_default_passwd_cb(cptr->context,ssl_password_callback);

		if ( mode & _OPENSSL_ENGINE )
		  {
		    /* setup engine pkcs11 */
		    e = setup_engine(ENGINE_ID);
		    if (e == NULL)
		      {
			close_connection( cptr );
			return( 0 );
		      }    

		    pkey = ENGINE_load_private_key(e, SslKeyFile, NULL, NULL);
		    if (!pkey) 
		      {
			tls_show_errors( "ENGINE_load_private_key" );
			close_connection( cptr );
			ENGINE_free(e);
			return( 0 );
		      }
		    if (!(oof = SSL_CTX_use_PrivateKey(cptr->context,pkey)))
		      {
			tls_show_errors( "SSL_CTX_use_PrivateKey" );
			close_connection( cptr );
			ENGINE_free(e);
			return( 0 );
		      }
		    EVP_PKEY_free(pkey);
		    ENGINE_free(e);
		  }
		else
		  {
		    if (!(oof = SSL_CTX_use_PrivateKey_file(
			cptr->context,SslKeyFile,
			(mode & _DER_KEY ? SSL_FILETYPE_ASN1 : SSL_FILETYPE_PEM) ) ))
		      {
			tls_show_errors( "SSL_CTX_use_PrivateKey_file" );
			close_connection( cptr );
			return( 0 );
		      }
		  }
		
		if ( SslCaList )
		{
			if (!(SSL_CTX_load_verify_locations(cptr->context,SslCaList, 0))) 
			{
				tls_show_errors( "SSL_CTX_load_verify_locations" );
				close_connection( cptr );
				return( 0 );
			}
		}

		else if (!( mode &  _DER_CERTIFICATE ))
		{
			if (!(SSL_CTX_load_verify_locations(cptr->context,SslCertFile, 0))) 
			{
				tls_show_errors( "SSL_CTX_load_verify_locations" );
				close_connection( cptr );
				return( 0 );
			}
		}

#if (OPENSSL_VERSION_NUMBER < 0x00905100L)
		SSL_CTX_set_verify_depth(cptr->context,1);
#endif
	}

	if (!( cptr->object = SSL_new( cptr->context ) )) 
	{
		tls_show_errors( "SSL_new" );
		close_connection( cptr );
		return( 0 );
	}
	if ( mode & _REQUIRE_PEER )
		SSL_set_verify( cptr->object,SSL_VERIFY_FAIL_IF_NO_PEER_CERT|SSL_VERIFY_PEER,tls_client_verify_callback);
	else if ( mode & _REQUEST_PEER )
		SSL_set_verify( cptr->object,SSL_VERIFY_PEER|SSL_VERIFY_CLIENT_ONCE,tls_client_verify_callback);
	else	SSL_set_verify( cptr->object,SSL_VERIFY_NONE,tls_client_verify_callback);
	return( 1 );
}

private	int	build_ssl_context(CONNECTIONPTR	cptr, int mode, int service )
{
	int	status;
	security_lock( 0, "build_context" );
	ssl_debug = (mode & _SSL_DEBUG);
	status = ll_build_ssl_context( cptr, mode, service );
	security_unlock( 0, "build_context" );
	return( status );
}

/*	----------------------------------------------------------	*/
/*		t l s _ v a l i d a t e _ s e r v e r			*/
/*	----------------------------------------------------------	*/
public	int	tls_validate_server( CONNECTIONPTR cptr, int mode )
{
	if (!( mode & _REQUEST_PEER ) )
		return(0);
	else if (!( mode & _REQUIRE_PEER ) )
		return(0);
	else	return(0);
}

/*	----------------------------------------------------------	*/
/*		t l s _ c l i e n t _ h a n d s h a k e			*/
/*	----------------------------------------------------------	*/
public	int	tls_client_handshake( CONNECTIONPTR cptr, int mode )
{
	int	status;
	rest_debug_log_file( "tls_client_handshake start" );
	if (!( build_ssl_context( cptr, mode, 0 ) ))
	{
		rest_debug_log_file( "tls_client_handshake failed" );
		return(0);
	}
	else	
	{
		security_lock( 0, "client_set_fd" );
		status = SSL_set_fd( cptr->object, cptr->socket );
		security_unlock( 0, "client_set_fd" );
		if (!( status ))
		{
			tls_show_errors("SSL_set_fd");
			rest_debug_log_file( "tls_client_handshake failed" );
			return(0);
		}
		else if ((status = ssl_tcp_connect( cptr->object )) < 0 )
		{
			tls_show_errors("SSL_connect<0");
			rest_debug_log_file( "tls_client_handshake failed" );
			return(0);
		}
		else if (!( status ))
		{
			tls_show_errors("SSL_connect==0");
			rest_debug_log_file( "tls_client_handshake failed" );
			return(1);
		}
		else if (!( tls_validate_server( cptr->object, mode ) ))
		{
			if ( check_debug() )
				printf("SSL_connect(%u) handshake OK\n",mode);
			rest_debug_log_file( "tls_client_handshake complete" );
			return(1);
		}
		else
		{
			tls_show_errors("incorrect client certificate");
			return(0);
		}
	}
}

/*	----------------------------------------------------------	*/
/*		t l s _ v a l i d a t e _ c l i e n t			*/
/*	----------------------------------------------------------	*/
public	int	tls_validate_client( CONNECTIONPTR cptr, int mode )
{
	if (!( mode & _REQUEST_PEER ) )
		return(0);
	else if (!( mode & _REQUIRE_PEER ) )
		return(0);
	else	return(0);
}

/*	----------------------------------------------------------	*/
/*		t l s _ s e r v e r _ h a n d s h a k e		*/
/*	----------------------------------------------------------	*/
public	int	tls_server_handshake( CONNECTIONPTR cptr, int mode )
{
	rest_debug_log_file( "tls_server_handshake start" );
	security_lock( 0, "server_SSL_new" );
	cptr->newobject = SSL_new( cptr->context );
	security_unlock( 0, "server_SSL_new" );
	if (!( cptr->newobject ))
	{
		tls_show_errors( "SSL_new" );
		return(0);
	}	
	else
	{
		security_lock( 0, "server_set_verify" );
		if ( mode & _REQUIRE_PEER )
			SSL_set_verify( cptr->newobject,SSL_VERIFY_FAIL_IF_NO_PEER_CERT|SSL_VERIFY_PEER,tls_server_verify_callback);
		else if ( mode & _REQUEST_PEER )
			SSL_set_verify( cptr->newobject,SSL_VERIFY_PEER|SSL_VERIFY_CLIENT_ONCE,tls_server_verify_callback);
		else	SSL_set_verify( cptr->newobject,SSL_VERIFY_NONE,tls_server_verify_callback);
		security_unlock( 0, "server_set_verify" );

		security_lock( 0, "server_set_fd" );
		SSL_set_fd( cptr->newobject, cptr->socket );
		security_unlock( 0, "server_set_fd" );

		if ( ssl_tcp_accept( cptr->newobject ) < 0 )
		{
			tls_show_errors("SSL_accept");
			return(0);
		}
		else if (!( tls_validate_client( cptr->newobject, mode ) ))
		{
			rest_debug_log_file( "tls_server_handshake complete" );
			return(1);
		}
		else
		{
			tls_show_errors("incorrect client certificate");
			return(0);
		}

	}
}

/*	----------------------------------------------------------	*/
/*		t l s _ s e r v e r _ s t a r t u p    			*/
/*	----------------------------------------------------------	*/
public	int	tls_server_startup( CONNECTIONPTR cptr, int mode )
{
	if (!( build_ssl_context( cptr, mode, 1 ) ))
		return(0);
	else	return(1);
}

#endif	/* _abalssl_c */
	/* ---------- */



