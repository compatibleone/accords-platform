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
#ifndef	_occi_h
#define _occi_h

#include "standard.h"
#include "allocate.h"
#include "rest.h"
#include "occipar.h"
#include "occiact.h"
#include "occiatb.h"
#include "occicat.h"
#include "occilink.h"
#include "occimixin.h"
#include "linknode.h"
#include "kindnode.h"

public	struct	occi_kind_node * allocate_occi_kind_node();
public	struct	occi_kind_node * liberate_occi_kind_node(struct	occi_kind_node * n);
public	struct	occi_link_node * allocate_occi_link_node();
public	struct	occi_link_node * liberate_occi_link_node(struct	occi_link_node * n);
public	char *	occi_http_link( struct occi_category * optr, char * target, char * id );

#define	_CORDS_SERVICE_AGENT 	"CO-RDS-SERVICE/1.0"
#define	_CORDS_CONTRACT_AGENT 	"CO-RDS-CONTRACT/1.0"
#define	_CORDS_BROKER_AGENT 	"CO-RDS-BROKER/1.0"
#define	_CORDS_PARSER_AGENT 	"CO-RDS-PARSER/1.0"
#define	_CORDS_OS_AGENT 	"CO-RDS-OS/1.0"
#define	_CORDS_ON_AGENT 	"CO-RDS-ON/1.0"
#define	_CORDS_AZ_AGENT 	"CO-RDS-AZ/1.0"
#define	_CORDS_PA_AGENT 	"CO-RDS-PA/1.0"
#define	_CORDS_SG_AGENT 	"CO-RDS-SG/1.0"

public	char *	default_publisher();
public	char *	default_tls();

#define	_CORDS_DEFAULT_PUBLISHER "http://publisher.compatibleone.fr:28001"
#define	_DEFAULT_PUBLISHER default_publisher()

#define	_OCCI_PRIORITY		1	/* the service is to be offered first	*/
#define	_OCCI_PRIVATE		2	/* the service is not to be published	*/
#define	_OCCI_SECRET 		4	/* the will not appear in categories 	*/
#define	_OCCI_AUTO_PUBLISH 	8	/* the service will be auto un/publish	*/
#define	_OCCI_SECURE 		16	/* each access must be secure		*/

#define	_OCCI_IDLE		0	/* the resource is idle			*/
#define	_OCCI_ACTIVE		1	/* the resource is active		*/
#define	_OCCI_RUNNING		1	/* the resource is active and running	*/
#define	_OCCI_SUSPENDED		2	/* the resource is active but paused	*/

#define	_OCCI_AUTHORIZE		"X-OCCI-AUTHORIZE"
#define	_OCCI_LINKHEAD		"LINK"
#define	_OCCI_LOCATION		"X-OCCI-LOCATION"
#define	_OCCI_ATTRIBUTE		"X-OCCI-ATTRIBUTE"
#define	_OCCI_CATEGORY		"CATEGORY"

#define	_OCCI_TEXT_PLAIN	"text/plain"
#define	_OCCI_MIME_OCCI		"text/occi"
#define	_OCCI_MIME_JSON		"application/json+occi"
#define	_OCCI_MIME_XML		"application/xml+occi"

/*	-------------------------------------------------------------------	*/
/*	      o c c i   a c t i o n   t a r g e t   i n t e r f a c e 		*/
/*	-------------------------------------------------------------------	*/
/*	this structure provides a function provider for the functions to be	*/
/*	invoked through the OCCI POST?action=name interface calls.		*/
/*	-------------------------------------------------------------------	*/
struct	occi_action_target
{
	struct	rest_response * (*action)
			(
			struct occi_category * category, 
			struct rest_client * client, 
			struct rest_request * request,
			struct rest_request * response,
			void * data
			);
};

/*	-------------------------------------------------------------------	*/
/*		   o c c i _ c a l l b a c k _ i n t e r f a c e		*/
/*	-------------------------------------------------------------------	*/

struct	occi_interface
{
	int	(*create)  (struct occi_category * optr, void * vptr);
	int	(*retrieve)(struct occi_category * optr, void * vptr);
	int	(*update)  (struct occi_category * optr, void * vptr);
	int	(*delete)  (struct occi_category * optr, void * vptr);
};

public 	char *			occi_unquoted_value( char * sptr );
public 	char *			occi_unquoted_link( char * sptr );
public	struct	occi_link *	liberate_occi_link( struct occi_link * lptr);
public	struct	occi_mixin *	liberate_occi_mixin( struct occi_mixin * mptr);
public	struct	occi_link *	allocate_occi_link();
public	struct	occi_mixin *	allocate_occi_mixin();
public	struct	occi_category *	occi_parse_category( char * sptr );
public	struct	occi_category * occi_remove_category( struct occi_category * cptr );
public	struct	occi_category * occi_create_category( char * domain, char * name, char * scheme, char * klass, char * rel, char * title );
public	struct	occi_category * occi_add_action(struct occi_category * cptr,char * name,char * parameters, void * method);
public	struct	occi_category * occi_add_attribute(struct occi_category * cptr,char * name,int m,int i );
public	char *	occi_http_capacity( struct occi_category * optr );
public	char *	occi_http_category( struct occi_category * optr );
public	char *	occi_http_attribute( struct occi_category * optr, struct occi_attribute * aptr );
public	char *	occi_http_attribute_value( struct occi_category * optr, struct occi_attribute * aptr, char * vptr );
public	struct	occi_attribute * occi_resolve_attribute( struct occi_category * optr, char * nptr );
public	char *	occi_allocate_uuid();
public	char *	occi_category_id( char * sptr );
public	char *	occi_json_capacity( struct occi_category * optr );
public	char *	occi_json_capacities( struct occi_category * optr );
public	char *	occi_json_category( struct occi_category * optr );

public	int	
	occi_process_atributs(
		struct occi_category * optr,
		struct rest_request * rptr,
		struct rest_response * aptr,
		void * pptr,
		void (*method)(struct occi_category *,void *,char *, char *));

public	struct	rest_response * occi_invoke_action(
		struct occi_action * fptr,	
		struct occi_category * optr,	
		struct rest_client * cptr,	
		struct rest_request * rptr,	
		struct rest_response * aptr,	
		void * pptr );

public	struct	occi_link_node*	occi_first_link_node();

#endif	/* _occi_h */
	/* ------- */




