#ifndef	_restclient_h
#define	_restclient_h

/*	------	*/
/*	HEADER 	*/
/*	------	*/
public struct rest_header * 
	rest_client_header( 
		struct rest_header * hptr, char * n, char * v );

/*	---	*/
/*	GET	*/
/*	---	*/
public	struct	rest_response * 
	rest_client_get_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr );

/*	------	*/
/*	DELETE	*/
/*	------	*/
public	struct	rest_response * 
	rest_client_delete_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr );

/*	----	*/
/*	HEAD	*/
/*	----	*/
public	struct	rest_response * 
	rest_client_head_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr );

/*	----	*/
/*	POST	*/
/*	----	*/
public	struct	rest_response * 
	rest_client_post_request(
		char * target, char * tls, char * nptr, char * filename, struct rest_header * hptr );

/*	---	*/
/*	PUT	*/
/*	---	*/
public	struct	rest_response * 
	rest_client_put_request(
		char * target, char * tls, char * nptr, char * filename, struct rest_header * hptr );

#endif	/* _restclient_h */
	/* ------------- */

