#ifndef _on_client_h
#define _on_client_h

#include "standard.h"
#include "rest.h"
#include "url.h"
#include "urlpublic.h"
#include "restclient.h"
#include "json.h"
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

struct	on_subscription 
{
	char *	host;
	char *	base;
	char *	user;
	char *	password;
	char *	version;
	char *	namespace;
	char *	agent;
	char *	tls;
	char *	authenticate;
};

#define	_ON_NS_COMPUTE	"http://docs.opennebula.org/compute/api/"

public	struct	on_response *	on_create_image( struct on_subscription * sptr, char * id, char * filename );

public	char * on_create_image_request(
		struct on_subscription * sptr,
		char * number,
		char * oldnumber,
		char * newname,
		char * driver
		);

public	char * on_create_server_request(
	struct on_subscription * sptr,
	char * identity, char * flavour, char * image, char* pubnetwork,char* prinetwork, char * architecture, char * driver );

public	char * on_create_compute_request(
	struct on_subscription * sptr,
	char * identity, char * flavour, char * image, char* pubnetwork,char* prinetwork, char * architecture, char * driver );

public	char * on_create_storage_request(
	struct on_subscription * sptr,
	char * name, char * description, char * type, char * size );

public	char * on_create_network_request(
	struct on_subscription * sptr,
	char * name, char * address, char * size, char * access );


public	struct	on_response * on_client_get_request(
	char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	on_response * on_client_delete_request(
	char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	on_response * on_client_head_request(
		char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	on_response * on_client_post_request(
		char * target, char * tls, char * agent, char * filename, struct rest_header * hptr );

public	struct	on_response * on_client_put_request(
		char * target, char * tls, char * agent, char * filename, struct rest_header * hptr );

public	struct on_response * liberate_on_response( struct on_response * rptr )	;

public	struct	on_response *	on_list_compute_pool(struct on_subscription * sptr );
public	struct	on_response *	on_list_storage_pool(struct on_subscription * sptr);
public	struct	on_response *	on_list_network_pool(struct on_subscription * sptr);
public	struct	on_response *	on_list_servers	(struct on_subscription * sptr);

public	struct	on_response *	on_list_flavors(struct on_subscription * sptr);
public	struct	on_response *	on_list_images  (struct on_subscription * sptr);

public	struct	on_response *	on_get_request(struct on_subscription * sptr, char * buffer );
public	struct	on_response *	on_list_request(struct on_subscription * sptr, char * buffer );
public	struct	on_response *	on_delete_request(struct on_subscription * sptr, char * buffer );
public	struct	on_response *	on_put_request(struct on_subscription * sptr, char * buffer, char * filename );
public	struct	on_response *	on_post_request(struct on_subscription * sptr, char * buffer, char * filename );

public	struct	on_response *	on_create_compute(struct on_subscription * sptr, char * filename );
public	struct	on_response *	on_create_storage(struct on_subscription * sptr, char * filename );
public	struct	on_response *	on_create_network(struct on_subscription * sptr, char * filename );
public	struct	on_response *	on_create_server(struct on_subscription * sptr, char * filename );

public	struct	on_response *	on_get_compute(struct on_subscription * sptr, char * id );
public	struct	on_response *	on_get_storage(struct on_subscription * sptr, char * id );
public	struct	on_response *	on_get_network(struct on_subscription * sptr, char * id );
public	struct	on_response *	on_get_server	(struct on_subscription * sptr, char * id );

public	struct	on_response *	on_get_flavour  (struct on_subscription * sptr, char * id );
public	struct	on_response *	on_get_image    (struct on_subscription * sptr, char * id );

public	struct	on_response *	on_update_compute(struct on_subscription * sptr, char * id, char * filename );
public	struct	on_response *	on_update_storage(struct on_subscription * sptr, char * id, char * filename );
public	struct	on_response *	on_update_network(struct on_subscription * sptr, char * id, char * filename );
public	struct	on_response *	on_update_server(struct on_subscription * sptr, char * id, char * filename );

public	struct	on_response *	on_delete_compute(struct on_subscription * sptr, char * id );
public	struct	on_response *	on_delete_storage(struct on_subscription * sptr, char * id );
public	struct	on_response *	on_delete_network(struct on_subscription * sptr, char * id );
public	struct	on_response *	on_delete_server(struct on_subscription * sptr, char * id );

public	struct	on_response *			on_delete_image (struct on_subscription * sptr, char * id );
public	char * 					on_stop_compute_request(struct on_subscription * sptr, char * identity );
public	struct	on_response *			on_stop_compute(struct on_subscription * sptr, char * id, char * filename );

public	char * 					on_start_compute_request(struct on_subscription * sptr, char * identity );
public	struct	on_response *			on_start_compute(struct on_subscription * sptr, char * id, char * filename );
public	char * 					on_shutdown_compute_request(struct on_subscription * sptr, char * identity );
public	struct	on_response *			on_shutdown_compute(struct on_subscription * sptr, char * id, char * filename );
public	char * 					on_public_image_request(struct on_subscription * sptr, char * image );
public	struct	on_response *			on_public_image (struct on_subscription * sptr, char * id, char * filename );
public	char * 					on_private_image_request(struct on_subscription * sptr, char * image );
public	struct	on_response *			on_private_image (struct on_subscription * sptr, char * id, char * filename );
public	char * 					on_rename_image_request(struct on_subscription * sptr, char * image, char * newname );
public	char * 					on_persistent_image_request(struct on_subscription * sptr, char * image );
public	char * 					on_volatile_image_request(struct on_subscription * sptr, char * image );
public	struct	on_response *			on_rename_image (struct on_subscription * sptr, char * id, char * filename );
public	struct	on_response *			on_persistent_image (struct on_subscription * sptr, char * id, char * filename );
public	struct	on_response *			on_volatile_image (struct on_subscription * sptr, char * id, char * filename );

public	struct	rest_header *			on_authenticate(struct on_subscription * sptr);
public	struct	on_subscription * 		liberate_on_subscription( struct on_subscription * sptr );
public	struct 	on_subscription *		on_initialise_client( 
		char * user, char * password, 
		char * host, char * agent, char * version, char * tls );

	/* ------------ */
#endif	/* _on_client_h */
	/* ------------ */



