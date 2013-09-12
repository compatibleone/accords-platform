
#ifndef _on_client_h
#define _on_client_h

#include "standard.h"
#include "rest.h"
#include "url.h"
#include "urlpublic.h"
#include "restclient.h"
#include "co_json.h"
#include "document.h"
#include "onconfig.h"

#define	_TEXT_NONE 	0
#define	_TEXT_XML	1
#define	_TEXT_JSON	2

struct	on_response
{
	int	nature;
	char			*	content;
	struct	xml_element 	*	xmlroot;
	struct	data_element	*	jsonroot;
	struct	rest_response	*	response;
};

#define	_ON_NS_COMPUTE	"http://docs.opennebula.org/compute/api/"

public	struct	on_response *	on_create_image( char * id, char * filename );

public	char * on_create_image_request(
		char * number,
		char * oldnumber,
		char * newname,
		char * driver
		);

public	char * on_create_server_request(
	char * identity, char * flavour, char * image, char* pubnetwork,char* prinetwork, char * architecture, char * driver );

public	char * on_create_compute_request(
	char * identity, char * flavour, char * image, char* pubnetwork,char* prinetwork, char * architecture, char * driver );

public	char * on_create_storage_request(
	char * name, char * description, char * type, char * size );

public	char * on_create_network_request(
	char * name, char * address, char * size, char * access );


public	struct	on_response * 
	on_client_get_request(
		char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	on_response * 
	on_client_delete_request(
		char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	on_response * 
	on_client_head_request(
		char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	on_response * 
	on_client_post_request(
		char * target, char * tls, char * agent, char * filename, struct rest_header * hptr );

public	struct	on_response * 
	on_client_put_request(
		char * target, char * tls, char * agent, char * filename, struct rest_header * hptr );

public	struct on_response * liberate_on_response( struct on_response * rptr )	;

public	struct	on_response *	on_list_compute_pool();
public	struct	on_response *	on_list_storage_pool();
public	struct	on_response *	on_list_network_pool();
public	struct	on_response *	on_list_servers	();

public	struct	on_response *	on_list_flavors();
public	struct	on_response *	on_list_images  ();

public	struct	on_response *	on_get_request( char * buffer );
public	struct	on_response *	on_list_request( char * buffer );
public	struct	on_response *	on_delete_request( char * buffer );
public	struct	on_response *	on_put_request( char * buffer, char * filename );
public	struct	on_response *	on_post_request( char * buffer, char * filename );

public	struct	on_response *	on_create_compute( char * filename );
public	struct	on_response *	on_create_storage( char * filename );
public	struct	on_response *	on_create_network( char * filename );
public	struct	on_response *	on_create_server( char * filename );

public	struct	on_response *	on_get_compute( char * id );
public	struct	on_response *	on_get_storage( char * id );
public	struct	on_response *	on_get_network( char * id );
public	struct	on_response *	on_get_server	( char * id );

public	struct	on_response *	on_get_flavour  ( char * id );
public	struct	on_response *	on_get_image    ( char * id );

public	struct	on_response *	on_update_compute( char * id, char * filename );
public	struct	on_response *	on_update_storage( char * id, char * filename );
public	struct	on_response *	on_update_network( char * id, char * filename );
public	struct	on_response *	on_update_server( char * id, char * filename );

public	struct	on_response *	on_delete_compute( char * id );
public	struct	on_response *	on_delete_storage( char * id );
public	struct	on_response *	on_delete_network( char * id );
public	struct	on_response *	on_delete_server( char * id );

public	struct	on_response *	on_delete_image ( char * id );
public	char * 	on_stop_compute_request( char * identity );
public	struct	on_response *	on_stop_compute( char * id, char * filename );

public	char * 	on_start_compute_request( char * identity );
public	struct	on_response *	on_start_compute( char * id, char * filename );
public	char * 	on_shutdown_compute_request( char * identity );
public	struct	on_response *	on_shutdown_compute( char * id, char * filename );
public	char * on_public_image_request( char * image );
public	struct	on_response *	on_public_image ( char * id, char * filename );
public	char * on_private_image_request( char * image );
public	struct	on_response *	on_private_image ( char * id, char * filename );
public	char * on_rename_image_request( char * image, char * newname );
public	char * on_persistent_image_request( char * image );
public	char * on_volatile_image_request( char * image );
public	struct	on_response *	on_rename_image ( char * id, char * filename );
public	struct	on_response *	on_persistent_image ( char * id, char * filename );
public	struct	on_response *	on_volatile_image ( char * id, char * filename );

#endif	/* _on_client_h */
	/* ------------ */



