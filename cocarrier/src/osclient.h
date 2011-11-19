/* ------------------------------------------------------------------------------------	*/
/*				 CompatibleOne Cloudware				*/
/* ------------------------------------------------------------------------------------ */
/*											*/
/* Ce fichier fait partie de ce(tte) oeuvre de Iain James Marshall et est mise a 	*/
/* disposition selon les termes de la licence Creative Commons Paternit‚ : 		*/
/*											*/
/*			 	Pas d'Utilisation Commerciale 				*/
/*				Pas de Modification 					*/
/*				3.0 non transcrit.					*/
/*											*/
/* ------------------------------------------------------------------------------------ */
/* 			Copyright (c) 2011 Iain James Marshall for Prologue 		*/
/*				   All rights reserved					*/
/* ------------------------------------------------------------------------------------ */
#ifndef _os_client_h
#define _os_client_h

#include "standard.h"
#include "rest.h"
#include "url.h"
#include "urlpublic.h"
#include "restclient.h"
#include "json.h"
#include "document.h"
#include "osconfig.h"

#define	_TEXT_NONE 	0
#define	_TEXT_XML	1
#define	_TEXT_JSON	2

struct	os_response
{
	int	nature;
	char			*	content;
	struct	xml_element 	*	xmlroot;
	struct	data_element	*	jsonroot;
	struct	rest_response	*	response;
};

#define	_OS_NS_COMPUTE_V11	"http://docs.openstack.org/compute/api/v1.1"
#define	_OS_NS_COMPUTE_V10	"http://docs.rackspacecloud.com/servers/api/v1.0"

public	char * os_create_server_request(
	char * identity, char * flavor, char * image, char * personality, char * resource );
public	char * os_create_metadata_request( char * values );
public	char * os_create_meta_request( char * key, char * value );
public	char * os_create_image_request(char * identity, char * server );

public	struct	os_response * 
	os_client_get_request(
		char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	os_response * 
	os_client_delete_request(
		char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	os_response * 
	os_client_head_request(
		char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	os_response * 
	os_client_post_request(
		char * target, char * tls, char * agent, char * filename, struct rest_header * hptr );

public	struct	os_response * 
	os_client_put_request(
		char * target, char * tls, char * agent, char * filename, struct rest_header * hptr );

public	struct	os_response *	os_list_servers	( );
public	struct	os_response *	os_list_flavors( );
public	struct	os_response *	os_list_images  ( );
public	struct	os_response *	os_list_metadata( char * id );

public	struct	os_response *	os_create_server( char * filename );
public	struct	os_response *	os_create_image( char * filename, char * serverid );

public	struct	os_response *	os_get_server	( char * id );
public	struct	os_response *	os_get_flavor  ( char * id );
public	struct	os_response *	os_get_image    ( char * id );
public	struct	os_response *	os_get_metadata( char * id, char * name );

public	struct	os_response *	os_update_server( char * id, char * filename );
public	struct	os_response *	os_update_metadata( char * id, char * name, char * value );
public	struct	os_response *	os_create_metadata( char * id, char * values );

public	struct	os_response *	os_delete_server( char * id );
public	struct	os_response *	os_delete_image ( char * id );
public	struct	os_response *	os_delete_metadata( char * id, char * name );

public	struct os_response * liberate_os_response( struct os_response * rptr );	


#endif	/* _os_client_h */
	/* ------------ */


