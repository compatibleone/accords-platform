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

#ifndef _on_client_c
#define _on_client_c

#include "onclient.h"

private	struct	on_config On = {
	(char *) 0,
	(char *) 0,

	(char *) 0,
	(char *) 0,

	(char *) 0,
	(char *) 0,

	(char *) 0,
	(char *) 0,

	0 

	};

/*	------------------------------------------------------------	*/
/*		l i b e r a t e _ o n _ r e s p o n s e			*/
/*	------------------------------------------------------------	*/
public	struct on_response * liberate_on_response( struct on_response * rptr )	
{
	if ( rptr )
	{
		if ( rptr->response )
			rptr->response = liberate_rest_response( rptr->response );
		if ( rptr->jsonroot )
			rptr->jsonroot = drop_data_element( rptr->jsonroot );
		if ( rptr->xmlroot )
			rptr->xmlroot = document_drop( rptr->xmlroot );
		if ( rptr->content )
			liberate( rptr->content );
		liberate( rptr );
	}
	return((struct on_response *) 0);
}


public	char * on_create_server_request(
		char * identity, 
		char * flavour, 
		char * image,
		char * personality,
		char * filename ) { return(0); }

public	struct	on_response * 
	on_client_get_request(
		char * target, int options, char * agent, struct rest_header * hptr ) { return(0); }

public	struct	on_response * 
	on_client_delete_request(
		char * target, int options, char * agent, struct rest_header * hptr ) { return(0); }

public	struct	on_response * 
	on_client_head_request(
		char * target, int options, char * agent, struct rest_header * hptr ) { return(0); }

public	struct	on_response * 
	on_client_post_request(
		char * target, int options, char * agent, char * filename, struct rest_header * hptr ) { return(0); }

public	struct	on_response * 
	on_client_put_request(
		char * target, int options, char * agent, char * filename, struct rest_header * hptr ) { return(0); }

public	struct	on_response *	on_list_servers	( ) { return(0); }
public	struct	on_response *	on_list_flavours( ) { return(0); }
public	struct	on_response *	on_list_images  ( ) { return(0); }

public	struct	on_response *	on_create_server( char * filename ) { return(0); }

public	struct	on_response *	on_get_server	( char * id ) { return(0); }
public	struct	on_response *	on_get_flavour  ( char * id ) { return(0); }
public	struct	on_response *	on_get_image    ( char * id ) { return(0); }

public	struct	on_response *	on_update_server( char * id, char * filename ) { return(0); }

public	struct	on_response *	on_delete_server( char * id ) { return(0); }
public	struct	on_response *	on_delete_image ( char * id ) { return(0); }

/*	------------------------------------------------------------	*/
/*		o n _ i n i t i a l i s e _ c l i e n t 		*/
/*	------------------------------------------------------------	*/
public	int	on_initialise_client( 
		char * user, char * password, 
		char * host, char * agent, char * version, char * tls )
{
	if (!( On.user = allocate_string( user )))
		return( 27 );
	if (!( On.password = allocate_string( password )))
		return( 27 );
	if (!( On.host = allocate_string( host )))
		return( 27 );
	else if (!( On.agent = allocate_string( agent )))
		return( 27 );
	else if (!( On.version = allocate_string( version )))
		return( 27 );
	else if (!( On.namespace = allocate( strlen( _ON_NS_COMPUTE ) + strlen( On.version ) + 16 ) ))
		return( 27 );
	else	
	{
		sprintf(On.namespace,"%s%s",_ON_NS_COMPUTE,On.version);
		On.authenticate= (char *) 0;
		On.tls = tls;
		return( 0 );
	}
}



	/* ----------- */
#endif	/* _onclient_c */
	/* ----------- */

