/* ------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                   */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>    */
/* --------------------------------------------------------------------*/
/*  This is free software; you can redistribute it and/or modify it    */
/*  under the terms of the GNU Lesser General Public License as        */
/*  published by the Free Software Foundation; either version 2.1 of   */
/*  the License, or (at your option) any later version.                */
/*                                                                     */
/*  This software is distributed in the hope that it will be useful,   */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of     */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   */
/*  Lesser General Public License for more details.                    */
/*                                                                     */
/*  You should have received a copy of the GNU Lesser General Public   */
/*  License along with this software; if not, write to the Free        */
/*  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA */
/*  02110-1301 USA, or see the FSF site: http://www.fsf.org.           */
/* --------------------------------------------------------------------*/
#ifndef	_coobas_c
#define	_coobas_c

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

struct	accords_configuration CooBas = {
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
	"coobas.xml",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(CooBas.debug);		}
public	int	check_verbose()		{	return(CooBas.verbose);		}
public	char *	default_publisher()	{	return(CooBas.publisher);	}
public	char *	default_operator()	{	return(CooBas.operator);	}
public	char *	default_tls()		{	return(CooBas.tls);		}

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
/*		c o o b a s _ l o a d 			*/
/*	---------------------------------------------	*/
/*	this function loads coobas configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	coobas_load()
{
	load_accords_configuration( &CooBas, "coobas" );
	return;
}

#include "occibuilder.h"

private	int	banner()
{
	printf("\n   CompatibleOne Ordering, Billing and Accounting COOBAS : Version 1.0a.0.04");
	printf("\n   Beta Version : 02/01/2012");
	printf("\n   Copyright (c) 2012 Iain James Marshall, Prologue");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*	------------------------------------------------------------------	*/
/*			c o o b a s _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * coobas_initialise(  void * v,struct rest_server * sptr )
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
/*			c o o b a s _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	coobas_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, CooBas.user ) )
		return(0);
	else if ( strcmp( password, CooBas.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			c o o b a s _ e x t e n s i o n 			*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * coobas_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	------------------------------------------------------------------	*/
/* 	 actions and methods required for the coobas instance categories	*/
/*	------------------------------------------------------------------	*/

/*	------------------------------------------------------------------	*/
/*		s t a r t _ i n v o i c e _ d o c u m e n t			*/
/*	------------------------------------------------------------------	*/
private	FILE * start_invoice_document( struct cords_invoice * pptr )
{
	char 	buffer[256];
	FILE * h=(FILE *) 0;

	if (!( h = fopen( pptr->document, "w" )))
		return( (FILE *) 0);

	if ( pptr->date ) 
		liberate( pptr->date );

	sprintf(buffer,"%u",time((long*)0));

	if (!( pptr->date = allocate_string( buffer ) ))
	{
		fclose(h);
		return((FILE *) 0);
	}
	
	fprintf(h,"<html><head><title>COOBAS:INVOICE:%s</title>\n",pptr->id);
	fprintf(h,"<style type='text/css' media='SCREEN'>\n");
	fprintf(h,".oddrow { background-color: white; }\n");
	fprintf(h,".evenrow { background-color: silver; }\n");
	fprintf(h,".headrow { background-color: grey; color: yellow; }\n");
	fprintf(h,"tr       { border-style: none; }\n");
	fprintf(h,"th       { border-style: none; }\n");
	fprintf(h,"</style></head>\n");
	fprintf(h,"<body><div align=center><p><table width='95%c' border=1>\n",0x0025);
	fprintf(h,"<tr class=headrow><th width='20%c'>Document</th><th>%s</th></tr>\n",0x0025,pptr->document);
	fprintf(h,"<tr class=evenrow><th>Date    </th><th>%s</th></tr>\n",pptr->date);
	fprintf(h,"<tr class=oddrow> <th>Invoice </th><th>%s</th></tr>\n",pptr->id);
	fprintf(h,"<tr class=evenrow><th>Number  </th><th>%s</th></tr>\n",pptr->id);
	fprintf(h,"<tr class=oddrow> <th>Account </th><th>%s</th></tr>\n",pptr->account);
	fprintf(h,"</table><p>\n");
	fprintf(h,"<table width='95%c' border=1>\n",0x0025);
	fprintf(h,"<tr class=headrow><th width='20%c'>Transaction<th>Date<th>Description<th width='20%c'>Price</tr>\n",0x0025,0x0025);
	return( h );
}

/*	------------------------------------------------------------------	*/
/*		c l o s e _ i n v o i c e _ d o c u m e n t			*/
/*	------------------------------------------------------------------	*/
private	void	close_invoice_document( FILE * h, struct cords_invoice * pptr )
{
	if ( h )
	{
		fprintf(h,"</table><p>\n");
		fprintf(h,"<table width='95%c' border=1>\n",0x0025);
		fprintf(h,"<tr class=headrow><th>Transactions</th><th width='20%c'>%u</th></tr>\n",0x0025,pptr->transactions);
		fprintf(h,"<tr class=evenrow><th>Total      </th><th>%s</th></tr>\n",pptr->total);
		fprintf(h,"<tr class=oddrow> <th>Taxe       </th><th>%u</th></tr>\n",0);
		fprintf(h,"<tr class=evenrow><th>Grand Total</th><th>%s</th></tr>\n",pptr->total);
		fprintf(h,"</table><p></div></body></html>\n");
		fclose(h);
	}
	return;
}

/*	--------------------------------------------------------	*/
/*		u p d a t e _ i n v o i c e _ t o t a l			*/
/*	--------------------------------------------------------	*/
private	void	update_invoice_total( struct cords_invoice * pptr, char * price )
{
	int	v=0;
	char 	work[64];
	if (!( price ))
		return;
	else	v = atoi( price );
	if ( pptr->total )
	{
		v += atoi( pptr->total );
		liberate( pptr->total );
	}
	sprintf(work,"%u",v);
	pptr->total = allocate_string( work );
	return;
}
			

/*	------------------------------------------------------------------	*/
/*	     i n v o i c e _ d o c u m e n t _ t r a n s a c t i o n		*/
/*	------------------------------------------------------------------	*/
private	void	invoice_document_transaction(
		FILE * h,
		struct cords_invoice * pptr,	/* the invoice structure 	*/
		char * transaction,		/* the transaction reference 	*/
		struct occi_response * yptr,	/* the transaction record	*/
		char * price,			/* the price reference		*/
		struct occi_response * zptr	/* the price information	*/
		)
{
	char *	vptr;
	char *	klass;
	pptr->transactions++;
	if (!( pptr->transactions & 1 ))
		klass = "oddrow";
	else	klass = "evenrow";
	fprintf(h,"<tr class='%s'>",klass);
	fprintf(h,"<th><a href='%s'>%u</a><th> \n",transaction,pptr->transactions);
	if (( vptr = occi_extract_atribut( yptr, "occi", _CORDS_TRANSACTION,"when")))
		fprintf(h,"%s",vptr);
	fprintf(h,"<th><a href='%s'>",transaction);
	if (( vptr = occi_extract_atribut( yptr, "occi", _CORDS_TRANSACTION,"source")))
		fprintf(h,"%s",vptr);
	if (( vptr = occi_extract_atribut( yptr, "occi", _CORDS_TRANSACTION,"description")))
		fprintf(h," %s",vptr);
	fprintf(h,"</a></th>\n");
	fprintf(h,"<th><a href='%s'>\n",price);
	if (( vptr = occi_extract_atribut( zptr, "occi", _CORDS_PRICE,"rate")))
	{
		fprintf(h,"%s ",vptr);
		update_invoice_total( pptr, vptr );
	}
	if (( vptr = occi_extract_atribut( zptr, "occi", _CORDS_PRICE,"currency")))
		fprintf(h," %s",vptr);
	fprintf(h,"</a></th></tr>\n");
	return;
}

/*	------------------------------------------------------------------	*/
/*	     p r o c e s s _ i n v o i c e _ t r a n s a c t i o n s		*/
/*	------------------------------------------------------------------	*/
private	int	process_invoice_transactions( struct cords_invoice * pptr )
{
	char *	host;
	char *	price;
	FILE *	h;
	struct	occi_response * xptr;
	struct	occi_response * yptr;
	struct	occi_response * zptr;
	struct	occi_element  * eptr;
	struct	occi_element  * fptr;
	char	buffer[1024];

	/* -------------------------------------- */
	/* allocate the invoice document filename */
	/* -------------------------------------- */
	if ( pptr->document )
		pptr->document = liberate( pptr->document );

	if ( pptr->total )
		pptr->total = liberate( pptr->total );

	sprintf(buffer,"rest/%s.htm",pptr->id);

	if (!( pptr->document = allocate_string( buffer ) ))
		return(0);

	/* --------------------------------- */
	/* retrieve the list of transactions */
	/* --------------------------------- */
	if (!( xptr = cords_retrieve_named_instance_list( 
		_CORDS_TRANSACTION, 
		"occi.transaction.account", 
		pptr->account, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( 0 );

	/* -------------------------- */
	/* create the output document */
	/* -------------------------- */
	else if (!( h = start_invoice_document( pptr ) ))
	{
		xptr = occi_remove_response( xptr );
		return( 0 );
	}

	/* ---------------------------- */
	/* for each of the transactions */
	/* ---------------------------- */
	pptr->transactions=0;

	for (	eptr=xptr->first;
		eptr != (struct occi_element *) 0;
		eptr = eptr->next )
	{
		/* ----------------------------------- */
		/* recover the transaction information */
		/* ----------------------------------- */
		if (!( eptr->value ))
			continue;
		else if (!( host = cords_build_host( xptr, eptr->value) ))
			continue;
		else if (!( yptr = occi_simple_get( host, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		{
			liberate(host);
			continue;
		}
		/* ------------------------------ */
		/* retrieve the price information */
		/* ------------------------------ */
		else if (!( price = occi_extract_atribut( yptr, "occi", _CORDS_TRANSACTION,_CORDS_PRICE)))
		{
			yptr = occi_remove_response( yptr );
			liberate(host);
			continue;
		}
		else if (!( zptr = occi_simple_get(price, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		{
			yptr = occi_remove_response( yptr );
			liberate(host);
			continue;
		}
		else	
		{
			invoice_document_transaction( h, pptr, host, yptr, price, zptr );
			yptr = occi_remove_response( yptr );
			zptr = occi_remove_response( zptr );
			continue;
		}
	}

	/* -------------------------------------------- */
	/* collect transactions for the invoice account */
	/* -------------------------------------------- */
	xptr = occi_remove_response( xptr );
	close_invoice_document( h, pptr );
	return(0);
}

/*	-------------------------------------------	*/
/* 	      c r e a t e _ i n v o i c e  		*/
/*	-------------------------------------------	*/
private	int	create_invoice(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct cords_invoice * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( pptr->account ))
		return( 0 ); 
	else 	return( process_invoice_transactions( pptr ) );
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ i n v o i c e  		*/
/*	-------------------------------------------	*/
private	int	retrieve_invoice(struct occi_category * optr, void * vptr)
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
private	int	update_invoice(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct cords_invoice * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( pptr->account ))
		return( 0 ); 
	else 	return( process_invoice_transactions( pptr ) );
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ i n v o i c e		*/
/*	-------------------------------------------	*/
private	int	delete_invoice(struct occi_category * optr, void * vptr)
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

/*	-------------------------------------------	*/
/* 	      c r e a t e _ t r a n s a c t i o n  	*/
/*	-------------------------------------------	*/
private	int	create_transaction(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct cords_transaction * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( pptr->account ))
		return( 0 ); 
	else
	{
		pptr->when = time((long *) 0);
		return( 0 );
	}
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ t r a n s a c t i o n  	*/
/*	-------------------------------------------	*/
private	int	retrieve_transaction(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct cords_transaction * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ t r a n s a c t i o n  	*/
/*	-------------------------------------------	*/
private	int	update_transaction(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct cords_transaction * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ t r a n s a c t i o n	*/
/*	-------------------------------------------	*/
private	int	delete_transaction(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct cords_transaction * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

private	struct	occi_interface	transaction_interface = {
	create_transaction,
	retrieve_transaction,
	update_transaction,
	delete_transaction
	};

/*	------------------------------------------------------------------	*/
/*			c o o b a s _ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	coobas_operation( char * nptr )
{
	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_coobas.xml");

	if (!( optr = occi_cords_account_builder( CooBas.domain, "account" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;
	optr->access |= _OCCI_NO_PRICING;

	if (!( optr = occi_cords_transaction_builder( CooBas.domain, "transaction" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	optr->callback  = &transaction_interface;
	optr->access |= _OCCI_NO_PRICING;

	if (!( optr = occi_cords_price_builder( CooBas.domain, "price" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;
	optr->access |= _OCCI_NO_PRICING;

	if (!( optr = occi_cords_invoice_builder( CooBas.domain, "invoice" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = &invoice_interface;
	optr->access |= _OCCI_NO_PRICING;

	if (!( optr = occi_cords_connection_builder( CooBas.domain, "connection" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback = (void *) 0;
	optr->access |= ( _OCCI_NO_PRICING | _OCCI_PRIVATE | _OCCI_CONSUMER );

	rest_initialise_log(CooBas.monitor );

	if (!( CooBas.identity ))
		return( occi_server(nptr,CooBas.restport,CooBas.tls,CooBas.threads,first, (char *) 0 ) );
	else
	{
		initialise_occi_publisher( CooBas.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			CooBas.user, CooBas.password,
			CooBas.identity,  nptr, 
			CooBas.restport, CooBas.tls, 
			CooBas.threads, first ) );
	}
}

/*	------------------------------------------------------------------	*/
/*				c o o b a s 					*/
/*	------------------------------------------------------------------	*/
private	int	coobas(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	coobas_load();
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
				CooBas.verbose=1;
				continue;
			case	'd'	:
				CooBas.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = coobas_operation(aptr) ))
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
	else	return( coobas( argc, argv ) );
}


	/* --------- */
#endif	/* _coobas_c */
	/* --------- */

