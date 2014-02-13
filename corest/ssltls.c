#ifndef	_abalssl_c
#define	_abalssl_c

#include <openssl/x509v3.h>
#include <openssl/ssl.h>
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
#define _OPENSSL_ENGINE        	512
#define _SSL_ACCEPT_INVALID    	1024
#define _SSL_SELF_SIGNED      	2048

#define _OPTIMISE_OCCI_LOCAL   	4096
#define _OPTIMISE_OCCI_CLIENT  	8192

#define _SSL_VALID_CERT        16384
#define _SSL_SAME_CA           32768

#define _SSL_MODES         (_SSL_ACCEPT_INVALID|_SSL_SELF_SIGNED|_SSL_VALID_CERT|_SSL_SAME_CA)

#define _SSL_IS_ISSUER         32768

private	pthread_mutex_t security_control = PTHREAD_MUTEX_INITIALIZER;

private	int	SSL_READY=0;
private	int	ssl_contexts=0;
private	int	total_contexts=0;
private	int	ssl_debug;
private	int	bugfix=1;

#define	Portable_srandom	srandom
#define	SSL_debug 		(check_debug() || ssl_debug)
#define debug(...) { if(check_debug()) fprintf(stderr, ##__VA_ARGS__); }
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
	if ((status = SSL_read( handle, buffer, length )) == 0) {
		/* DG: FIXED (?) DOS when underlying socket was not cleanly closed */
		if(SSL_get_error(handle, status) == SSL_ERROR_SYSCALL) 
		{
			if ( SSL_debug )
			{
				printf("Socket read 0 (EOF?): %u\n", SSL_get_error(handle, status));
			}
			status = -1;
		}
	} 
	else if ( status < 0 )
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
	X509 *err_cert;
	char buf[256];
	int err, depth;
	SSL_CTX *sslctx;
	SSL *ssl;
	long mode;
	err_cert = X509_STORE_CTX_get_current_cert(certificate);
	err = X509_STORE_CTX_get_error(certificate);
	depth = X509_STORE_CTX_get_error_depth(certificate);
	ssl = X509_STORE_CTX_get_ex_data(certificate,
								SSL_get_ex_data_X509_STORE_CTX_idx());
	sslctx = SSL_get_SSL_CTX(ssl);
	mode = (long)SSL_CTX_get_app_data(sslctx);
	debug("mode: %ld preverify:%d\n", mode, preverify);
	X509_NAME_oneline(X509_get_subject_name(err_cert), buf, 256);

	X509 *peer_cert = X509_STORE_CTX_get_current_cert(certificate);
	X509 *self_cert = SSL_get_certificate(ssl);;

	int is_issuer_err = X509_check_issued(peer_cert, self_cert);
	debug("is issuer: %d\n", is_issuer_err);
	if(!is_issuer_err && !(mode & _SSL_IS_ISSUER)) {
		SSL_CTX_set_app_data(sslctx, (void*)(mode|_SSL_IS_ISSUER));
	}


	if ( SSL_debug ) {
		if (!preverify) {
			printf("verify error:num=%d:%s\ndepth=%d:%s\n", err,
					X509_verify_cert_error_string(err), depth, buf);
		} else {
			printf("depth=%d:%s\n", depth, buf);
		}
		if (!preverify && (err == X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT))
		{
			X509_NAME_oneline(X509_get_issuer_name(certificate->current_cert), buf, 256);
			printf("issuer= %s\n", buf);
		}
	}
	if (!preverify) {
		switch(err) {
			/* List of possible codes at:
			 * http://www.openssl.org/docs/apps/verify.html#
			 */
			case X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN:
				if(mode & _SSL_INTERNAL || mode & (_SSL_SELF_SIGNED|_SSL_ACCEPT_INVALID)) {
					return 1;
				} else {
					printf("ERROR: Self-signed cert not allowed (%ld)\n", mode);
				}
				break;
		}
		debug("verify error:num=%d:%s\ndepth=%d:%s\n", err,
			 X509_verify_cert_error_string(err), depth, buf);

		return 0;
	}
	return(1);
}

private	int	tls_server_verify_callback( int preverify, X509_STORE_CTX * certificate )
{
	if ( SSL_debug )
		printf("tls_server_verify_callback(%u)\n",preverify);
	X509 *err_cert;
	char buf[256];
	int err, depth;
	SSL_CTX *sslctx;
	SSL *ssl;
	long mode;
	err_cert = X509_STORE_CTX_get_current_cert(certificate);
	err = X509_STORE_CTX_get_error(certificate);
	depth = X509_STORE_CTX_get_error_depth(certificate);
	ssl = X509_STORE_CTX_get_ex_data(certificate,
								SSL_get_ex_data_X509_STORE_CTX_idx());
	sslctx = SSL_get_SSL_CTX(ssl);
	mode = (long)SSL_CTX_get_app_data(sslctx);
	debug("mode: %ld\n", mode);
	X509_NAME_oneline(X509_get_subject_name(err_cert), buf, 256);

	X509 *peer_cert = X509_STORE_CTX_get_current_cert(certificate);
	X509 *self_cert = SSL_get_certificate(ssl);;

	int is_issuer_err = X509_check_issued(peer_cert, self_cert);
	debug("is issuer: %d\n", is_issuer_err);
	if(!is_issuer_err && !(mode & _SSL_IS_ISSUER)) {
		SSL_CTX_set_app_data(sslctx, (void*)(mode|_SSL_IS_ISSUER));
	}

	if ( check_debug() ) {
		if (!preverify) {
			printf("verify error:num=%d:%s\ndepth=%d:%s\n", err,
					X509_verify_cert_error_string(err), depth, buf);
		} else {
			printf("depth=%d:%s\n", depth, buf);
		}
		if (!preverify && (err == X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT))
		{
			X509_NAME_oneline(X509_get_issuer_name(certificate->current_cert), buf, 256);
			printf("issuer= %s\n", buf);
		}
	}
	if (!preverify) {
		switch(err) {
			/* List of possible codes at:
			 * http://www.openssl.org/docs/apps/verify.html#
			 */
			case X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN:
				if(mode & _SSL_INTERNAL || mode & (_SSL_SELF_SIGNED|_SSL_ACCEPT_INVALID)) {
					return 1;
				} else {
					printf("ERROR: Self-signed cert not allowed (%ld)\n", mode);
				}
				break;
		}
		debug("verify error:num=%d:%s\ndepth=%d:%s\n", err,
			 X509_verify_cert_error_string(err), depth, buf);

		return 0;
	}
	return(1);
}

/*	-----------------------------------------------------------	*/
/*		t l s _ c h e c k _ c e r t i f i c a t e		*/
/*	-----------------------------------------------------------	*/
typedef enum {
	MatchFound,
	MatchNotFound,
	NoSANPresent,
	MalformedCertificate,
	Error
} HostnameValidationResult;

private HostnameValidationResult matches_common_name(const char *hostname, const X509 *server_cert) {
	int common_name_loc = -1;
	X509_NAME_ENTRY *common_name_entry = NULL;
	ASN1_STRING *common_name_asn1 = NULL;
	char *common_name_str = NULL;

	// Find the position of the CN field in the Subject field of the certificate
	common_name_loc = X509_NAME_get_index_by_NID(X509_get_subject_name((X509 *) server_cert), NID_commonName, -1);
	if (common_name_loc < 0) {
		return Error;
	}

	// Extract the CN field
	common_name_entry = X509_NAME_get_entry(X509_get_subject_name((X509 *) server_cert), common_name_loc);
	if (common_name_entry == NULL) {
		return Error;
	}

	// Convert the CN field to a C string
	common_name_asn1 = X509_NAME_ENTRY_get_data(common_name_entry);
	if (common_name_asn1 == NULL) {
		return Error;
	}
	common_name_str = (char *) ASN1_STRING_data(common_name_asn1);

	// Make sure there isn't an embedded NUL character in the CN
	if ((size_t)ASN1_STRING_length(common_name_asn1) != strlen(common_name_str)) {
		return MalformedCertificate;
	}

	// Compare expected hostname with the CN
	//if (Curl_cert_hostcheck(common_name_str, hostname) == CURL_HOST_MATCH)
	if (!strcmp(common_name_str, hostname)) {
		return MatchFound;
	}
	else {
		return MatchNotFound;
	}
}

private HostnameValidationResult matches_subject_alternative_name(const char *hostname, const X509 *server_cert) {
	HostnameValidationResult result = MatchNotFound;
	int i;
	int san_names_nb = -1;
	STACK_OF(GENERAL_NAME) *san_names = NULL;

	// Try to extract the names within the SAN extension from the certificate
	san_names = X509_get_ext_d2i((X509 *) server_cert, NID_subject_alt_name, NULL, NULL);
	if (san_names == NULL) {
		return NoSANPresent;
	}
	san_names_nb = sk_GENERAL_NAME_num(san_names);

	// Check each name within the extension
	for (i=0; i<san_names_nb; i++) {
		const GENERAL_NAME *current_name = sk_GENERAL_NAME_value(san_names, i);

		if (current_name->type == GEN_DNS) {
			// Current name is a DNS name, let's check it
			char *dns_name = (char *) ASN1_STRING_data(current_name->d.dNSName);

			// Make sure there isn't an embedded NUL character in the DNS name
			if ((size_t)ASN1_STRING_length(current_name->d.dNSName) != strlen(dns_name)) {
				result = MalformedCertificate;
				break;
			}
			else { // Compare expected hostname with the DNS name
				//if (Curl_cert_hostcheck(dns_name, hostname)
				//    == CURL_HOST_MATCH)
				if (!strcmp(dns_name, hostname)) {
					result = MatchFound;
					break;
				}
			}
		}
	}
	sk_GENERAL_NAME_pop_free(san_names, GENERAL_NAME_free);

	return result;
}

private HostnameValidationResult validate_hostname(const char *hostname, X509 *cert) {
	HostnameValidationResult result;

	if((hostname == NULL) || (cert == NULL))
		return Error;

	// First try the Subject Alternative Names extension
	result = matches_subject_alternative_name(hostname, cert);
	if (result == NoSANPresent) {
		// Extension was not found: try the Common Name
		result = matches_common_name(hostname, cert);
	}

	return result;
}

private	int	tls_check_certificate( X509_STORE_CTX * x509_ctx, void * arg )
{

	char cert_str[256];
	CONNECTIONPTR	cptr = (CONNECTIONPTR)arg;
	const char *host = cptr->hostname ? cptr->hostname : "undefined";
	const char *res_str = "tls_check_certificate() failed";
	HostnameValidationResult res = Error;
	SSL *ssl;
	STACK_OF(X509_NAME) *sk;
	int is_server = 0;
	if ( SSL_debug )
		printf("tls_check_certificate(%s)\n", cptr->hostname);
	if ( bugfix )
		return( 1 );

	if(cptr->newobject != NULL) {
		ssl = cptr->newobject;
		is_server = 1;
		res = MatchFound; // FIXME: check for certificate hash ?
	} else {
		ssl = cptr->object;
	}

	/* This is the function that OpenSSL would call if we hadn't called
	 * SSL_CTX_set_cert_verify_callback().  Therefore, we are "wrapping"
	 * the default functionality, rather than replacing it.
	 * This is where tls_client_verify_callback() gets called.
	 * This is where tls_server_verify_callback() gets called. */
	int ok_so_far = X509_verify_cert(x509_ctx);


	X509 *peer_cert = X509_STORE_CTX_get_current_cert(x509_ctx);
	//X509 *self_cert = SSL_get_certificate(ssl);;

	X509_check_ca(peer_cert);

	if(SSL_debug) {
		if (((sk=SSL_get_client_CA_list(ssl)) != NULL) && (sk_X509_NAME_num(sk) > 0)) {
			char *buf = NULL;
			X509_NAME *xn;
			int i;
			printf("---\nAcceptable client certificate CA names\n");
			for (i=0; i<sk_X509_NAME_num(sk); i++) {
				xn = sk_X509_NAME_value(sk,i);
				buf = X509_NAME_oneline(xn, buf, 0);
				if(buf) {
					printf("%s\n", buf);
					free(buf);
				}
			}
		}
		else {
			printf("---\nNo client certificate CA names sent\n");
		}
	}

	/* In Server mode: We don't have the hostname, what should we check BTW ?
	 * This will be called for Azure self-signed certs AFAIK
	 * Maybe we could check an attribute of the certificate
	 */
	if (ok_so_far) {
		SSL_CTX *sslctx = SSL_get_SSL_CTX(ssl);
		int mode = (long)SSL_CTX_get_app_data(sslctx);
		if(mode & _SSL_SAME_CA) {
			if(!(mode & _SSL_IS_ISSUER)) {
				debug("\n\nConnection refused: not same CA\n\n");
				return 0;
			}
		}
		if(is_server == 0) {
			res = validate_hostname(host, peer_cert);

			switch (res) {
				case MatchFound:
					res_str = "MatchFound";
					break;
				case MatchNotFound:
					res_str = "MatchNotFound";
					break;
				case NoSANPresent:
					res_str = "NoSANPresent";
					break;
				case MalformedCertificate:
					res_str = "MalformedCertificate";
					break;
				case Error:
					res_str = "Error";
					break;
				default:
					res_str = "WTF!";
					break;
			}
		}
	}

	X509_NAME_oneline(X509_get_subject_name (peer_cert),
			cert_str, sizeof (cert_str));
	if (res == MatchFound) {
		debug("https server '%s' has this certificate, "
				"which looks good to me:\n%s\n",
				host, cert_str);
		return 1;
	} else {
		debug("ERROR: Got '%s' for hostname '%s' and certificate:\n%s\n",
				res_str, host, cert_str);
		return 0;
	}
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

	SSL_CTX_set_app_data(cptr->context, (void*)(long)mode);

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
			/* if this is not a file, try a directory */
			if (!(SSL_CTX_load_verify_locations(cptr->context, 0, SslCaList)))
			{
				tls_show_errors( "SSL_CTX_load_verify_locations" );
				close_connection( cptr );
				return( 0 );
			}
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
	if(!SSL_CTX_check_private_key(cptr->context)) {
		printf("Private key does not match the public certificate\n");
		return 0;
	}

	if (!( cptr->object = SSL_new( cptr->context ) )) 
	{
		tls_show_errors( "SSL_new" );
		close_connection( cptr );
		return( 0 );
	}
	if ( (mode & _REQUIRE_PEER) || (mode & _SSL_MODES >= _SSL_SAME_CA) )
		SSL_set_verify( cptr->object,SSL_VERIFY_FAIL_IF_NO_PEER_CERT|SSL_VERIFY_PEER,tls_client_verify_callback);
	else if ( (mode & _REQUEST_PEER) || (mode & _SSL_MODES >= _SSL_VALID_CERT) )
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
public	int	tls_validate_server( SSL * handle, int mode )
{
	X509 *cert;
	int res;
	cert = SSL_get_peer_certificate(handle);
	if(mode & _REQUIRE_PEER) {
		if(!cert) {
			debug("SSL_accept(pid=%u,h=%p) => No peer certificate %d \r\n",getpid(),handle,-1);
			return -1;
		}
		X509_free(cert);
	}
	if ((res=SSL_get_verify_result(handle)) != X509_V_OK) {
		switch(res) {
			/* List of possible codes at:
			 * http://www.openssl.org/docs/apps/verify.html#
			 */
			case X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN:
				if(mode & _SSL_INTERNAL || mode & (_SSL_SELF_SIGNED|_SSL_ACCEPT_INVALID)) {
					debug("SSL accepting self-signed cert\n");
					return 0;
				} else {
					debug("SSL refusing self-signed cert\n");
				}
				break;
		}
		debug("SSL_accept(pid=%u,h=%p) => Bad peer certificate %d \r\n",getpid(),handle,res);
		return -1;
	}
	return 0;
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
public	int	tls_validate_client( SSL *	handle, int mode )
{
	X509 *cert;
	int res;
	cert = SSL_get_peer_certificate(handle);
	if(mode & _REQUIRE_PEER) {
		if(!cert) {
			debug("SSL_accept(pid=%u,h=%p) => No peer certificate %d \r\n",getpid(),handle,-1);
			return -1;
		}
		X509_free(cert);
	}
	if ((res=SSL_get_verify_result(handle)) != X509_V_OK) {
		switch(res) {
			/* List of possible codes at:
			 * http://www.openssl.org/docs/apps/verify.html#
			 */
			case X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN:
				if(mode & _SSL_INTERNAL || mode & (_SSL_SELF_SIGNED|_SSL_ACCEPT_INVALID)) {
					debug("SSL accepting self-signed cert\n");
					return 0;
				} else {
					debug("SSL refusing self-signed cert\n");
				}
				break;
		}
		debug("SSL_accept(pid=%u,h=%p) => Bad peer certificate %d \r\n",getpid(),handle,res);
		return -1;
	}
	return 0;
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
        //DG FIXME: Server should build and send the CA list
        //SSL_CTX_set_client_CA_list(tlsctx, SSL_load_client_CA_file(SslCertFile));

		if ( (mode & _REQUIRE_PEER) || ((mode & _SSL_MODES) >= _SSL_SAME_CA) ) {
			debug("MODE SAME_CA\n");
			SSL_set_verify( cptr->newobject,SSL_VERIFY_FAIL_IF_NO_PEER_CERT|SSL_VERIFY_PEER,tls_server_verify_callback);
		} else if ( (mode & _REQUEST_PEER) || ((mode & _SSL_MODES) >= _SSL_SELF_SIGNED) ) {
			debug("MODE VALID_CERT\n");
			SSL_set_verify( cptr->newobject,SSL_VERIFY_PEER|SSL_VERIFY_CLIENT_ONCE,tls_server_verify_callback);
		} else {
			debug("MODE INVALID_CERT %d\n", mode);
			SSL_set_verify( cptr->newobject,SSL_VERIFY_NONE,tls_server_verify_callback);
		}
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



