/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>     */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License"); 	*/
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at 				*/
/*  									*/
/*  http://www.apache.org/licenses/LICENSE-2.0 				*/
/*  									*/
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 	*/
/* implied. 								*/
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License. 					*/
/* -------------------------------------------------------------------- */
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

public	char *	get_identity();
public	char *	get_component();
public	char * 	get_default_agent();
public	int	set_default_agent( char * nptr);
public	char *	get_operator_profile();


#define	_CORDS_SERVICE_AGENT 	get_default_agent()
#define	_CORDS_CONTRACT_AGENT 	get_default_agent()
#define	_CORDS_BROKER_AGENT 	"CO-RDS-BROKER/1.0"
#define	_CORDS_PARSER_AGENT 	"CO-RDS-PARSER/1.0"
#define	_CORDS_OS_AGENT 	"CO-RDS-OS/1.0"
#define	_CORDS_ON_AGENT 	"CO-RDS-ON/1.0"
#define	_CORDS_EC2_AGENT 	"CO-RDS-EC2/1.0"
#define	_CORDS_AZ_AGENT 	"CO-RDS-AZ/1.0"
#define	_CORDS_PA_AGENT 	"CO-RDS-PA/1.0"
#define	_CORDS_SG_AGENT 	"CO-RDS-SG/1.0"

public	char *	default_publisher();
public	char *	default_tls();
public	char *	default_zone();
public	char *	default_operator();
public	char *	get_default_account();
public	char *	get_default_account_id();

#define	_CORDS_DEFAULT_PUBLISHER "http://publisher.compatibleone.fr:28001"
#define	_DEFAULT_PUBLISHER default_publisher()
#define	_CORDS_DEFAULT_OPERATOR	"accords"
#define	_CORDS_DEFAULT_USER	"admin"
#define	_CORDS_DEFAULT_PASSWORD	"admin"

#define	_OCCI_OS_TEMPLATE	"http://schemas.ogf.org/occi/infrastructure#os_tpl"
#define	_OCCI_RESOURCE_TEMPLATE	"http://schemas.ogf.org/occi/infrastructure#resource_tpl"


/*	COSACS Global Parameters	*/
/*	------------------------	*/
#define	_COSACS_PORT 		8286
#define	_COSACS_START 		"cosacs:start"
#define	_COSACS_RUN   		"cosacs:run"

#define	_OCCI_PRIORITY		1	/* the service is to be offered first	*/
#define	_OCCI_PRIVATE		2	/* the service is not to be published	*/
#define	_OCCI_SECRET 		4	/* the will not appear in categories 	*/
#define	_OCCI_AUTO_PUBLISH 	8	/* the service will be auto un/publish	*/
#define	_OCCI_SECURE 		16	/* each access must be secure		*/
#define	_OCCI_NO_PRICING	32	/* inhibit category pricing 		*/
#define	_OCCI_NO_AUTHORIZE	64	/* inhibit category authorize failure	*/
#define	_OCCI_CONSUMER	 	128	/* publish monitoring consumer		*/
#define	_OCCI_PROVIDER	 	256	/* publish resources provider 		*/
#define	_OCCI_CONTRACT	 	512	/* publish resources under contract	*/

#define	_OCCI_IDLE		0	/* the resource is idle			*/
#define	_OCCI_ACTIVE		1	/* the resource is active		*/
#define	_OCCI_RUNNING		1	/* the resource is active and running	*/
#define	_OCCI_SUSPENDED		2	/* the resource is active but paused	*/

#define	_OCCI_AUTHORIZE		"X-OCCI-AUTHORIZE"
#define	_OCCI_ACCOUNT		"X-OCCI-ACCOUNT"
#define	_OCCI_LINKHEAD		"LINK"
#define	_OCCI_LOCATION		"X-OCCI-LOCATION"
#define	_OCCI_ATTRIBUTE		"X-OCCI-ATTRIBUTE"
#define	_OCCI_CATEGORY		"CATEGORY"

#define	_OCCI_TEXT_PLAIN	"text/plain"
#define	_OCCI_TEXT_OCCI		"text/occi"
#define	_OCCI_MIME_OCCI		"text/occi"
#define	_OCCI_TEXT_HTML		"text/html"

#define	_OCCI_OCCI_PHP		"application/php+occi"
#define	_OCCI_APP_PHP		"application/php"
#define	_OCCI_TEXT_PHP		"text/php"

#define	_OCCI_MIME_PHP		"text/php"

#define	_OCCI_OLD_JSON		"application/json:occi"	/* houssems messy JSON */

#define	_OCCI_OCCI_JSON		"application/json+occi"
#define	_OCCI_APP_JSON		"application/json"
#define	_OCCI_APPLICATION_JSON	"application/json"
#define	_OCCI_X_APPLICATION_JSON	"x-application/json"
#define	_OCCI_TEXT_JSON		"text/json"
#define	_OCCI_MIME_JSON		"text/json"
#define	_OCCI_TEXT_FORM		"application/x-www-form-urlencoded"

#define	_OCCI_OCCI_XML		"application/xml+occi"
#define	_OCCI_APP_XML		"application/xml"
#define	_OCCI_APPLICATION_XML	"application/xml"
#define	_OCCI_X_APPLICATION_XML	"x-application/xml"
#define	_OCCI_TEXT_XML		"text/xml"
#define	_OCCI_MIME_XML		"text/xml"

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
	int	(*create)  (struct occi_category * optr, void * vptr,struct rest_request * rptr);
	int	(*retrieve)(struct occi_category * optr, void * vptr,struct rest_request * rptr);
	int	(*update)  (struct occi_category * optr, void * vptr,struct rest_request * rptr);
	int	(*delete)  (struct occi_category * optr, void * vptr,struct rest_request * rptr);
};

public	struct	occi_category * occi_create_os_template( char * domain, char * name, char * schema, char * title );
public	struct	occi_category * occi_create_resource_template( char * domain, char * name, char * schema, char * title );
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
public	struct	occi_action *	occi_resolve_action( struct occi_category * cptr, char * name );
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

public	struct	rest_response * occi_invoke_action_with_parameters(
		struct occi_action * fptr,	
		struct rest_header * hptr,
		struct occi_category * optr,	
		struct rest_client * cptr,	
		struct rest_request * rptr,	
		struct rest_response * aptr,	
		void * pptr );

public	struct	occi_link_node*	occi_first_link_node();
public	struct	occi_link_node*	occi_last_link_node();
public	struct  occi_link_node * occi_first_category_link( 
		char * category, 
		char * id );
public	struct  occi_link_node * occi_next_category_link( 
		struct occi_link_node * nptr, 
		char * category, 
		char * id );
public 	struct rest_response *	occi_content_type( 
	struct occi_category * cptr,
	struct rest_request * qptr, 
	struct rest_response * rptr );

#endif	/* _occi_h */
	/* ------- */




