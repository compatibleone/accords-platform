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
#ifndef	_orgabas_c
#define	_orgabas_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"
#include "cordslang.h"
#include "cp.h"
#include "cb.h"
#include "occibody.h"
#include "cspi.h"
#include <time.h>

struct	accords_configuration OrgaBas = {
	0,0,
	0,0,0,0,
	(char *) 0,
	(char *) 0, 
	_CORDS_DEFAULT_PUBLISHER,
	_CORDS_DEFAULT_OPERATOR,
	_CORDS_DEFAULT_USER,
	_CORDS_DEFAULT_PASSWORD,
	"http",  80,
	"xmpp",  8000,
	"domain",
	"orgabas.xml",
	"europe",
	"storage",
	"orgabas",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(OrgaBas.debug);		}
public	int	check_verbose()		{	return(OrgaBas.verbose);	}
public	char *	default_publisher()	{	return(OrgaBas.publisher);	}
public	char *	default_operator()	{	return(OrgaBas.operator);	}
public	char *	default_tls()		{	return(OrgaBas.tls);		}
public	char *	default_zone()		{	return(OrgaBas.zone);		}

public	int	failure( int e, char * m1, char * m2 )
{
	if ( e )
	{
		printf("\n*** failure %u",e);
		if ( m1 )
			printf(" : %s",m1);
		if ( m2 )
			printf(" : %s",m2);
		printf(" **`\n");
	}
	return( e );
}

/*	---------------------------------------------	*/  
/*		o r g a b a s _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads orgabas configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	orgabas_load()
{
	load_accords_configuration( &OrgaBas, "orgabas" );
	return;
}

#include "occibuilder.h"

private	int	banner()
{
	printf("\n   CompatibleOne Ordering, Billing and Accounting ORGABAS : Version 1.1a.0.02");
	printf("\n   Beta Version : 31/10/2013");
	printf("\n   Copyright (c) 2012, 2013 Iain James Marshall");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}
/*	-----------------------------------------------		*/
/*	r e s o l v e _ o p e r a t o r _ a c c o u n t 	*/
/*	-----------------------------------------------		*/
private	char * provideraccount=(char *) 0;
private	int	working=0;
public 	char * ll_resolve_provider_account()
{
	char *	provider;
	struct	occi_response * zptr;
	struct	occi_response * uptr;
	struct	occi_element * dptr;
	if (!( provideraccount ))
	{
		if (!( provider = default_operator()))
			return( (char *) 0 );
		else if (!( zptr = cords_retrieve_named_instance_list( _CORDS_ACCOUNT, "occi.account.name", provider, get_default_agent(), default_tls() ) ))
			return( (char *) 0 );
		else if (!( uptr = cords_retrieve_named_instance( zptr, get_default_agent(), default_tls() ) ))
			return( (char *) 0 );
		else if (!( dptr = occi_locate_element ( uptr->first, "occi.core.id" )))
			return( (char *) 0 );
		else if (!( provideraccount = allocate_string( dptr->value ) ))
			return( (char *) 0 );
	 	else if (!( provideraccount = occi_unquoted_value( provideraccount ) ))
			return( (char *) 0 );
	}
	return( allocate_string( provideraccount ) );
}

public 	char * resolve_provider_account()
{
	char * result=(char *) 0;
	if ( working )
		return( allocate_string( default_operator() ) );
	else
	{
		working++;
		result = ll_resolve_provider_account();
		working--;
		return( result );
	}
}

/*	------------------------------------------------------------------	*/
/*			o r g a b a s _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * orgabas_initialise(  void * v,struct rest_server * sptr )
{
	struct	rest_extension * xptr;
	if (!( xptr = rest_add_extension( sptr ) ))
		return((struct rest_server *) 0);
	else
	{
		xptr->net = (struct connection *) 0;
		return( sptr );
	}
}

/*	------------------------------------------------------------------	*/
/*			o r g a b a s _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	orgabas_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, OrgaBas.user ) )
		return(0);
	else if ( strcmp( password, OrgaBas.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			o r g a b a s _ e x t e n s i o n 			*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * orgabas_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	------------------------------------------------------------------	*/
/* 	 actions and methods required for the orgabas instance categories	*/
/*	------------------------------------------------------------------	*/
#include "comonsconnection.c"

/*	-------------------------------------------	*/
/* 	      c r e a t e _ i n v o i c e  		*/
/*	-------------------------------------------	*/
private	int	create_invoice(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct cords_invoice * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( rest_valid_string( pptr->account ) ))
		return( 0 ); 
	else 	return( 0 );
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ i n v o i c e  		*/
/*	-------------------------------------------	*/
private	int	retrieve_invoice(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct cords_invoice * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ i n v o i c e 	 	*/
/*	-------------------------------------------	*/
private	int	update_invoice(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct cords_invoice * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if ( pptr->state > 0 )
		return(0);
	else if (!( rest_valid_string( pptr->account ) ))
		return( 0 ); 
	else 	return( 0 );
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ i n v o i c e		*/
/*	-------------------------------------------	*/
private	int	delete_invoice(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct cords_invoice * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

private	struct	occi_interface	invoice_interface = {
	create_invoice,
	retrieve_invoice,
	update_invoice,
	delete_invoice
	};


/*	------------------------------------------------------------------	*/
/*			p r o c e s s _ i n v o i c e				*/
/*	------------------------------------------------------------------	*/
private	struct rest_response * process_invoice(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_invoice * pptr;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Incorrect Message Category" ) );
	else if ( pptr->state )
		return( rest_html_response( aptr, 200, "OK" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	------------------------------------------------------------------	*/
/*			    c l o s e _ i n v o i c e				*/
/*	------------------------------------------------------------------	*/
private	struct rest_response * close_invoice(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_invoice * pptr;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Incorrect Message Category" ) );
	else if ( pptr->state )
		return( rest_html_response( aptr, 200, "OK" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/*	  U s e r   L d a p   M a n a g e m e n t 	*/
/*	-------------------------------------------	*/
#include "ldapuser.c"
#include "orgaclient.c"
#include "orgaccount.c"
#include "orgatrans.c"

/*	------------------------------------------------------------------	*/
/*			o r g a b a s _ o p e r a t i o n			*/
/*	------------------------------------------------------------------	*/
private	int	orgabas_operation( char * nptr )
{
	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	load_ldap_configuration("ldapuser.xml", "ldapuser" );

	set_autosave_cords_xlink_name("links_orgabas.xml");

	if (!( optr = occi_cords_account_builder( OrgaBas.domain, "account" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = &account_interface;
	optr->access |= _OCCI_NO_PRICING;

	if (!( optr = occi_cords_transaction_builder( OrgaBas.domain, "transaction" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	optr->callback  = &transaction_interface;
	optr->access |= _OCCI_NO_PRICING;

	if (!( optr = occi_ldap_user_builder( OrgaBas.domain, "user" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;
	optr->access |= _OCCI_NO_PRICING;

	if (!( optr = occi_cords_price_builder( OrgaBas.domain, "price" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;
	optr->access |= _OCCI_NO_PRICING;

	if (!( optr = occi_cords_invoice_builder( OrgaBas.domain, "invoice" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = &invoice_interface;
	optr->access |= _OCCI_NO_PRICING;

	if (!( optr = occi_add_action( optr,"process","",process_invoice)))
		return( 28 );
	else if (!( optr = occi_add_action( optr,"close","",close_invoice)))
		return( 28 );

	if (!( optr = comons_connection_builder( OrgaBas.domain ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = comons_packet_builder( OrgaBas.domain, "packet_orgabas.xml" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	rest_initialise_log(OrgaBas.monitor );

	if (!( OrgaBas.identity ))
		return( occi_server(nptr,OrgaBas.restport,OrgaBas.tls,OrgaBas.threads,first, (char *) 0 ) );
	else
	{
		initialise_occi_publisher( OrgaBas.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			OrgaBas.user, OrgaBas.password,
			OrgaBas.identity,  nptr, 
			OrgaBas.restport, OrgaBas.tls, 
			OrgaBas.threads, first ) );
	}
}

/*	------------------------------------------------------------------	*/
/*				o r g a b a s 					*/
/*	------------------------------------------------------------------	*/
private	int	orgabas(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	orgabas_load();
	while ( argi < argc )
	{
		if (!( aptr = argv[++argi] ))
			break;
		else if ( *aptr == '-' )
		{
			aptr++;
			switch( *(aptr++) )
			{
			case	'v'	:
				OrgaBas.verbose=1;
				continue;
			case	'd'	:
				OrgaBas.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = orgabas_operation(aptr) ))
			continue;
		else	break;
	}
	return(status);
}

/*	------------------------------------------------------------------	*/
/*					m a i n 				*/
/*	------------------------------------------------------------------	*/
public	int	main(int argc, char * argv[] )
{
	if ( argc == 1 )
		return( banner() );
	else	return( orgabas( argc, argv ) );
}


	/* ---------- */
#endif	/* _orgabas_c */
	/* ---------- */
