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
#ifndef _az_client_h
#define _az_client_h

#include "standard.h"
#include "rest.h"
#include "url.h"
#include "urlpublic.h"
#include "restclient.h"
#include "json.h"
#include "document.h"
#include "azconfig.h"

#define	_TEXT_NONE 	0
#define	_TEXT_XML	1
#define	_TEXT_JSON	2

struct	az_response
{
	int	nature;
	char			*	content;
	struct	xml_element 	*	xmlroot;
	struct	data_element	*	jsonroot;
	struct	rest_response	*	response;
};

#define	_AZ_NS_COMPUTE	"http://schemas.microsoft.com/windowsazure"

public	char * az_create_server_request(
	char * identity, char * flavor, char * image, char * personality, char * resource );
public	char * az_create_image_request(char * identity, char * server );

public	struct	az_response * 
	az_client_get_request(
		char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	az_response * 
	az_client_delete_request(
		char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	az_response * 
	az_client_head_request(
		char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	az_response * 
	az_client_post_request(
		char * target, char * tls, char * agent, char * filename, struct rest_header * hptr );

public	struct	az_response * 
	az_client_put_request(
		char * target, char * tls, char * agent, char * filename, struct rest_header * hptr );

public	struct	az_response *	az_create_hosted_service( char * filename );
public	struct	az_response *	az_retrieve_hosted_service( char * filename );
public	struct	az_response *	az_update_hosted_service( char * filename );
public	struct	az_response *	az_delete_hosted_service( char * filename );
public	struct	az_response *	az_list_hosted_services();
public	struct	az_response *	az_create_affinity_group( char * filename );
public	struct	az_response *	az_retrieve_affinity_group( char * filename );
public	struct	az_response *	az_update_affinity_group( char * filename );
public	struct	az_response *	az_delete_affinity_group( char * filename );
public	struct	az_response *	az_list_affinity_groups();
public	struct	az_response *	az_create_certificate( char * filename );
public	struct	az_response *	az_retrieve_certificate( char * filename );
public	struct	az_response *	az_delete_certificate( char * filename );
public	struct	az_response *	az_list_certificates();
public	struct	az_response *	az_list_locations();
public	struct	az_response *	az_list_servers	();
public	struct	az_response *	az_list_flavors();
public	struct	az_response *	az_list_images  ();

public	struct	az_response *	az_create_server( char * filename );
public	struct	az_response *	az_create_image( char * filename );

public	struct	az_response *	az_get_server	( char * id );
public	struct	az_response *	az_get_flavor  ( char * id );
public	struct	az_response *	az_get_image    ( char * id );

public	struct	az_response *	az_update_server( char * id, char * filename );

public	struct	az_response *	az_delete_server( char * id );
public	struct	az_response *	az_delete_image ( char * id );

public	struct az_response * liberate_az_response( struct az_response * rptr );	


#endif	/* _az_client_h */
	/* ------------ */



