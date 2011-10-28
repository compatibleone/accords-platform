
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

#define	_ON_NS_COMPUTE	"http://docs.opennebula.org/compute/api/"

public	char * on_create_server_request(
	char * identity, char * flavour, char * image, char * personality, char * resource );

public	struct	on_response * 
	on_client_get_request(
		char * target, int options, char * agent, struct rest_header * hptr );

public	struct	on_response * 
	on_client_delete_request(
		char * target, int options, char * agent, struct rest_header * hptr );

public	struct	on_response * 
	on_client_head_request(
		char * target, int options, char * agent, struct rest_header * hptr );

public	struct	on_response * 
	on_client_post_request(
		char * target, int options, char * agent, char * filename, struct rest_header * hptr );

public	struct	on_response * 
	on_client_put_request(
		char * target, int options, char * agent, char * filename, struct rest_header * hptr );

public	struct	on_response *	on_list_servers	( );
public	struct	on_response *	on_list_flavours( );
public	struct	on_response *	on_list_images  ( );

public	struct	on_response *	on_create_server( char * filename );

public	struct	on_response *	on_get_server	( char * id );
public	struct	on_response *	on_get_flavour  ( char * id );
public	struct	on_response *	on_get_image    ( char * id );

public	struct	on_response *	on_update_server( char * id, char * filename );

public	struct	on_response *	on_delete_server( char * id );
public	struct	on_response *	on_delete_image ( char * id );



#endif	/* _on_client_h */
	/* ------------ */



