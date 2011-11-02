#ifndef	_restpublic_h
#define	_restpublic_h

public	struct	url *	
	analyse_url( char * target );

public	struct	url *	
	validate_url( struct url * uptr );

public	char	*	
	rest_temporary_filename(char * extension);

public	char *	
	rest_encode_credentials( char * username, char * password );

public	char 	*	
	rest_allocate_uuid();

public	int		
	rest_thread_control(int new_value);

public	int		
	rest_thread_enter(int section);

public	int		
	rest_thread_leave(int section);

public	void 
	rest_response_body(  struct rest_response * aptr, char * body, int type );

public	void 
	rest_request_body(  struct rest_request * aptr, char * body, int type );

public	struct rest_response * 
	rest_bad_request(void * vptr, struct rest_client * cptr,struct rest_request * rptr);

public	struct rest_response * 
	rest_incorect_method(void * vptr, struct rest_client * cptr,struct rest_request * rptr);

public	struct rest_response * 
	rest_response_status( struct rest_response* aptr,int	status, char * message );

public	struct	rest_header * 
	rest_response_header( struct rest_response * aptr, char * nptr, char * vptr );

public	struct	rest_client *
	rest_drop_client( struct rest_client * cptr);

public	struct	rest_response * 
	rest_allocate_response( struct rest_client * cptr);

public	struct rest_response * 
	rest_html_response( struct rest_response  * aptr, int status, char * message );

public	struct rest_response * 
	rest_response_failure( struct rest_client * cptr, int status, char * message );

public	struct rest_header * 
	rest_resolve_header( struct rest_header * hptr,char * name );

public	struct rest_extension * 
	rest_add_extension( struct rest_server * sptr );

public	struct rest_extension * 
	rest_drop_extension( struct rest_server * sptr, struct rest_extension * xptr );

public	int	
	rest_server( char * nptr, int port, char * tls, int max,struct rest_interface * iptr );

public	char * 	
	rest_request_host( struct rest_request * rptr );

public struct rest_response * 	
	liberate_rest_response(struct rest_response * aptr);

public struct rest_response * 	
	allocate_rest_response();

public struct rest_header * 	
	allocate_rest_header();

public struct rest_header * 	
	rest_create_header( char * nptr, char * vptr );

public struct rest_header * 	
	liberate_rest_header(struct rest_header * hptr);

public struct rest_header * 	
	liberate_rest_headers( struct rest_header * hptr );

public struct rest_header * 	
	rest_request_header(struct rest_request * rptr, char * nptr, char * vptr );


#endif	/* _restpublic_h */
	/* ------------- */

