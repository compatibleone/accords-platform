#ifndef	_occi_client_h
#define	_occi_client_h

#include "standard.h"
#include "rest.h"
#include "occi.h"
#include "restclient.h"
#include "url.h"
#include "urlpublic.h"

#define	_occi_client

struct	occi_client;
struct	occi_manager;

#include "occic.h"
#include "occihm.h"
#include "occim.h"

struct	occi_client	
{
	struct	occi_client 	* previous;
	struct	occi_client 	* next;
	struct	occi_manager	* parent;

	char 			* host;
	char 			* agent;
	char 			* tls;
	char 			* mimetype;

	int			  categories;
	int			  started;
	int			  requests;
	int			  failures;
	/* extended fields */
	struct	occi_category 	* firstcat;
	struct	occi_category 	* lastcat;

	struct	url		* target;
	char			  buffer[8192];

};

#define	_OCCI_XLINK	"/link/"

#define	_OCCI_NORMAL	0
#define	_OCCI_ACTION	1
#define	_OCCI_LINK  	2
#define	_OCCI_MIXIN 	3

#define	_OCCI_OPTIMISE_CLIENT 1
#define	_OCCI_OPTIMISE_LOCAL  2


struct	occi_element
{
	struct	occi_element * previous;
	struct	occi_element * next;
	char *	name;
	char *	value;
};

struct	occi_request
{
	struct	occi_element * first;
	struct	occi_element * last;
	int	type;	
	char 	* name;
	char 	* host;
	char 	* account;
	struct	occi_category * category;
};

struct	occi_response
{
	struct	occi_element * first;
	struct	occi_element * last;
	int	type;	
	char *	name;
	char *	host;
	struct	occi_category * category;
	struct	rest_response * response;
};

public	struct	occi_client *	occi_flush_client( char * address, int port );
public	struct	occi_client   *	occi_remove_client( struct occi_client * cptr );
public	struct	occi_client   *	occi_create_client( char * host, char * agent, char * tls );
public	struct	occi_client   *	occi_redirect_client( struct occi_client * cptr, char * uri );
public	struct	occi_request  *	occi_create_request( struct occi_client * cptr, char * category, int type );
public	struct	occi_request  *	occi_remove_request( struct occi_request * rptr );
public	struct	occi_request  *	occi_set_request_account( struct occi_request * rptr, char * account );
public	struct	occi_response *	occi_simple_get( char * reference, char * agent, char * tls );
public	struct	occi_response *	occi_simple_put(char * reference, struct occi_element * eptr, char * agent, char * tls );
public	struct	occi_response *	occi_simple_post(char * reference, struct occi_element * eptr, char * agent, char * tls );
public	struct	occi_response *	occi_simple_delete( char * reference, char * agent, char * tls );
public	struct	occi_response *	occi_simple_list( char * reference, struct occi_element * eptr, char * agent, char * tls );
public	struct	occi_response *	occi_client_get( struct occi_client * cptr, struct occi_request * rptr );
public	struct	occi_response *	occi_client_put( struct occi_client * cptr, struct occi_request * rptr );
public	struct	occi_response *	occi_client_post( struct occi_client * cptr, struct occi_request * rptr );
public	struct	occi_response *	occi_client_head( struct occi_client * cptr, struct occi_request * rptr );
public	struct	occi_response *	occi_client_delete( struct occi_client * cptr, struct occi_request * rptr );
public	struct	occi_response *	occi_action_post( struct occi_client * cptr, struct occi_request * rptr, struct rest_header * hptr );
public	struct	occi_response *	occi_remove_response( struct occi_response * aptr );
public	struct	occi_response * allocate_occi_response();
public	struct	occi_element  *	occi_request_element( struct occi_request * rptr, char * nptr, char * vptr );
public	struct	occi_element  *	occi_response_element(struct occi_response * rptr, char * nptr, char * vptr);
public	struct	occi_element  * occi_locate_element( struct occi_element * eptr, char * sptr );
public	struct	occi_element  * occi_append_element( struct occi_element * eptr, char * nptr, char * vptr );
public	struct	occi_element  *	occi_create_element( char * nptr, char * vptr );
public	struct	occi_element  * occi_append_headers(struct occi_request * rptr,struct occi_element * hptr);
public	struct	occi_element  * occi_remove_elements( struct occi_element * eptr );
public	struct	occi_element  *	occi_request_from_response(struct occi_request * qptr,struct occi_response * rptr);
public	struct	rest_header   * occi_client_authentication( char * aptr );
public	struct	rest_header   * occi_server_authentication( char * aptr );
public	struct	rest_header   * occi_client_accept( char * aptr );
public	struct	rest_header   * occi_client_content_type( char * aptr );
public 	struct	rest_header   * occi_add_default_header(struct rest_header  * hptr);
public 	struct	rest_header   * occi_save_default_headers();
public 	void			occi_restore_default_headers(struct rest_header  * hptr);
public 	void			occi_drop_default_headers();
public	struct	occi_response * occi_delete_links( char * from, char * agent, char * tls );
public	struct	occi_response * occi_delete_link( char * from, char * to, char * agent, char * tls );
public	struct	occi_response * occi_create_link( char * from, char * to, char * agent, char * tls );
public	struct	occi_category * occi_resolve_category( struct occi_category * cptr, char * nptr );
public  char *	occi_extract_location( struct occi_response * rptr );
public	char *	occi_extract_atribut( 
	struct occi_response * zptr, char * domain,
	char * category, char * nptr );


#endif 	/* _occi_client_h */
	/* -------------- */


