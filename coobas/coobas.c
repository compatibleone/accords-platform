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
#include "occibody.h"
#include "cspi.h"
#include <time.h>

#include "link_backend.h"

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
	"europe",
	"storage",
	"coobas",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

/*	----------------------------------------	*/
/*	invoice messages and information strings	*/
/*	----------------------------------------	*/
private	struct	invoice_information 
{
	int	scripting;
	float	taxrate;
	char *	transaction;
	char *	description;
	char *	price;
	char * 	invoice;
	char *	date;
	char *	number;
	char *	account;
	char *	comment;
	char *	legal;
	char *	total;
	char *	taxes;
	char *	grandtotal;
	char *	transactions;
} 
Invoice = 
{
	1,		/* activates use of scripting */
	18.60,
	"Transaction",
	"Description",
	"Price",
	"Invoice",
	"Date",
	"Number",
	"Account",
	"Thanking you for your custom",
	"Copyright (c) 2012 CompatibleOne under Creative Commons License",
	"Total",
	"Taxes",
	"Grand Total",
	"Transactions"

};

public	int	check_debug()		{	return(CooBas.debug);		}
public	int	check_verbose()		{	return(CooBas.verbose);		}
public	char *	default_publisher()	{	return(CooBas.publisher);	}
public	char *	default_operator()	{	return(CooBas.operator);	}
public	char *	default_tls()		{	return(CooBas.tls);		}
public	char *	default_zone()		{	return(CooBas.zone);		}

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
	printf("\n   CompatibleOne Ordering, Billing and Accounting COOBAS : Version 1.0a.0.06");
	printf("\n   Beta Version : 27/01/2013");
	printf("\n   Copyright (c) 2012, 2013 Iain James Marshall, Prologue");
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
#include "comonsconnection.c"

/*	------------------------------------------------------------------	*/
/*			t r a n s a c t i o n _ d a t e 			*/
/*	------------------------------------------------------------------	*/
private	char * transaction_date( int tt )
{
	char 	buffer[256];
	struct	tm * tptr;
	time_t t;
	t = (time_t) tt;

	if (!( tptr = localtime( &t )))
		sprintf(buffer,"%u",(unsigned)t);
	else
	{
		sprintf(buffer,"%u/%u/%u",
			tptr->tm_mday, tptr->tm_mon+1,tptr->tm_year+1900);
	}
	return( allocate_string( buffer ) );
}

/*	------------------------------------------------------------------	*/
/*		t r a n s a c t i o n _ d a t e _ a n d _ t i m e 		*/
/*	------------------------------------------------------------------	*/
private	char * transaction_date_and_time( int tt )
{
	char 	buffer[256];
	struct	tm * tptr;
	time_t t;
	t = (time_t) tt;

	if (!( tptr = localtime( &t )))
		sprintf(buffer,"%u",(unsigned)t);
	else
	{
		sprintf(buffer,"%u/%u/%u %u:%u:%u",
			tptr->tm_mday, tptr->tm_mon+1,tptr->tm_year+1900,
			tptr->tm_hour, tptr->tm_min, tptr->tm_sec);
	}
	return( allocate_string( buffer ) );
}

/*	------------------------------------------------------------------	*/
/*	    o p e r a t o r _ i n v o i c e _ i n f o r m a t i o n		*/
/*	------------------------------------------------------------------	*/
	char *	description;
	char *	price;
	char * 	invoice;
	char *	date;
	char *	number;
	char *	account;
private char *	operator_invoice_information( char * label )
{
	if (!( label ))
		return( "&nbsp;" );
	else if (!( strcmp( label,"invoice:comment" ) ))
		return( Invoice.comment );
	else if (!( strcmp( label,"invoice:account" ) ))
		return( Invoice.account );
	else if (!( strcmp( label,"invoice:number" ) ))
		return( Invoice.number );
	else if (!( strcmp( label,"invoice:date" ) ))
		return( Invoice.date );
	else if (!( strcmp( label,"invoice:invoice" ) ))
		return( Invoice.invoice );
	else if (!( strcmp( label,"invoice:price" ) ))
		return( Invoice.price );
	else if (!( strcmp( label,"invoice:description" ) ))
		return( Invoice.description );
	else if (!( strcmp( label,"invoice:legal" ) ))
		return( Invoice.legal );
	else if (!( strcmp( label,"invoice:total" ) ))
		return( Invoice.total );
	else if (!( strcmp( label,"invoice:taxes" ) ))
		return( Invoice.taxes );
	else if (!( strcmp( label,"invoice:grandtotal" ) ))
		return( Invoice.grandtotal );
	else if (!( strcmp( label,"invoice:transaction" ) ))
		return( Invoice.transaction );
	else if (!( strcmp( label,"invoice:transactions" ) ))
		return( Invoice.transactions );
	else 	return("&nbsp;");
}


/*	------------------------------------------------------------------	*/
/*		s t a r t _ i n v o i c e _ d o c u m e n t			*/
/*	------------------------------------------------------------------	*/
private	FILE * start_invoice_document( struct cords_invoice * pptr )
{
	FILE * h=(FILE *) 0;
	int	t;

	if (!( h = fopen( pptr->document, "w" )))
		return( (FILE *) 0);

	if ( pptr->date ) 
		liberate( pptr->date );
	t = time((long *) 0);

	if (!( pptr->date = transaction_date( t )))
	{
		fclose(h);
		return((FILE *) 0);
	}
	
	/* -------------------------- */
	/* invoice style instructions */
	/* -------------------------- */
	fprintf(h,"<html><head><title>COOBAS:INVOICE:%s</title>\n",pptr->id);
	fprintf(h,"<style type='text/css' media='SCREEN'>\n");
	fprintf(h,".oddrow { background-color: white; }\n");
	fprintf(h,".evenrow { background-color: silver; }\n");
	fprintf(h,".headrow { background-color: grey; color: yellow; }\n");
	fprintf(h,"tr       { border-style: none; }\n");
	fprintf(h,"th       { border-style: none; }\n");
	fprintf(h,"</style></head>\n");
	fprintf(h,"<body><div align=center><p>\n");

	/* ------------------------------------------- */
	/* Invoice description information to the left */
	/* ------------------------------------------- */
	fprintf(h,"<table width='95%c' border=1>\n",0x0025);
	fprintf(h,"<tr><th colspan=4>%s</th></tr>\n","&nbsp;");
	fprintf(h,"<tr class=headrow><th><div align=right>%s</div></th><th style='padding-left:2mm;'><div align=left>%s</div></th>\n",
		operator_invoice_information("invoice:invoice"),pptr->id);

	/* ---------------------------------------------------------- */
	/* payment account information will be presented in this cell */
	/* ---------------------------------------------------------- */
	fprintf(h,"<th colspan=2 rowspan=4>&nbsp;</th></tr>\n");

	/* ------------------------------------------------ */
	/* More Invoice description information to the left */ 
	/* ------------------------------------------------ */
	fprintf(h,"<tr class=oddrow> <th><div align=right>%s</div></th><th style='padding-left: 2mm;'><div align=left>%s</div></th></tr>\n", operator_invoice_information("invoice:date"),pptr->date);
	fprintf(h,"<tr class=evenrow><th><div align=right>%s</div></th><th style='padding-left: 2mm;'><div align=left>%s</div></th></tr>\n",operator_invoice_information("invoice:number"),pptr->id);
	fprintf(h,"<tr class=oddrow> <th><div align=right>%s</div></th><th style='padding-left: 2mm;'><div align=left><a href='%s'>%s</a></div></th></tr>\n",operator_invoice_information("invoice:account"),pptr->account,pptr->account);

	/* ---------------------------------------------------- */
	/* fprintf(h,"</table><p>\n");				*/
	/* fprintf(h,"<table width='95%c' border=1>\n",0x0025);	*/
	/* ---------------------------------------------------- */
	fprintf(h,"<tr><th colspan=4>%s</th></tr>\n","&nbsp;");
	fprintf(h,"<tr><td colspan=4>&nbsp;</td></tr>\n");

	/* --------------------------------- */
	/* transaction details header fields */
	/* --------------------------------- */
	fprintf(h,"<tr class=headrow><th width='20%c'>%s",0x0025,operator_invoice_information("invoice:transaction"));
	fprintf(h,"<th>%s</th>",operator_invoice_information("invoice:date"));
	fprintf(h,"<th>%s</th>",operator_invoice_information("invoice:description"));
	fprintf(h,"<th width='20%c'>%s</th></tr>\n",0x0025,operator_invoice_information("invoice:price"));

	return( h );
}

/*	------------------------------------------------------------------	*/
/*		c l o s e _ i n v o i c e _ d o c u m e n t			*/
/*	------------------------------------------------------------------	*/
private	void	close_invoice_document( FILE * h, struct cords_invoice * pptr )
{
	if ( h )
	{
		
		/* ---------------------------------------------------- */
		/* fprintf(h,"</table><p>\n");				*/
		/* fprintf(h,"<table width='95%c' border=1>\n",0x0025); */
		/* ---------------------------------------------------- */
		fprintf(h,"<tr><td colspan=4>&nbsp;</td></tr>\n");

		/* ---------------------------- */
		/* totals and information frame */
		/* ---------------------------- */
		fprintf(h,"<tr class=headrow><th colspan=2 rowspan=3>%s</th>\n",operator_invoice_information("invoice:comment"));

		fprintf(h,"<th><div align=right>%s</div></th><th><div align=right>%s</div></th></tr>\n",
			operator_invoice_information("invoice:total"),pptr->total);
		fprintf(h,"<tr class=oddrow> <th><div align=right>%s</div></th><th><div align=right>%s</div></th></tr>\n",
			operator_invoice_information("invoice:taxes"),pptr->taxe);
		fprintf(h,"<tr class=evenrow><th><div align=right>%s</div></th><th><div align=right>%s</div></th></tr>\n",
			operator_invoice_information("invoice:grandtotal"),pptr->grandtotal);

		/* ------------------------------- */
		/* legal information and copyright */
		/* ------------------------------- */
		fprintf(h,"<tr><th colspan=4>%s</th></tr>\n","&nbsp;");
		fprintf(h,"<tr><th colspan=4>%s</th></tr>\n",operator_invoice_information("invoice:legal"));
		fprintf(h,"<tr><th colspan=4>%s</th></tr>\n","&nbsp;");


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
	char 	work[64];
	float	t=0.0;
	float	v=0.0;
	float	f=0.0;

	/* ------------------------------------------- */
	/* add this price or cost to the running total */
	/* ------------------------------------------- */
	if (!( price ))
		return;
	else	sscanf(price,"%f",&v);

	if ( pptr->total )
	{
		sscanf(pptr->total,"%f",&f);
		v += f;
		liberate( pptr->total );
	}

	sprintf(work,"%.2f",v);

	if (!( pptr->total = allocate_string( work ) ))
		return;

	/* ------------------------------------------- */
	/* calculate the current taxe value from total */
	/* ------------------------------------------- */
	t = ((v*Invoice.taxrate)/100);	

	if ( pptr->taxe )
		liberate( pptr->taxe );

	sprintf(work,"%.2f",t);
	if (!( pptr->taxe  = allocate_string( work ) ))
		return;

	/* ------------------------------------------- */
	/* calculate the grand total inclusive of taxe */
	/* ------------------------------------------- */
	v += t;
	if ( pptr->grandtotal )
		liberate( pptr->grandtotal );

	sprintf(work,"%.2f",v);

	if (!( pptr->grandtotal  = allocate_string( work ) ))
		return;
	else	return;
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
	float   f=0.0;
	char *	dptr;
	char *	vptr;
	char *	klass;

	pptr->transactions++;
	if (!( pptr->transactions & 1 ))
		klass = "oddrow";
	else	klass = "evenrow";

	fprintf(h,"<tr class='%s'>",klass);

	/* ------------------------ */
	/* transaction number field */
	/* ------------------------ */
	fprintf(h,"<th><a href='%s'>%u</a><th> \n",transaction,pptr->transactions);

	/* ---------------------- */
	/* transaction date field */
	/* ---------------------- */
	if (( vptr = occi_extract_atribut( yptr, "occi", _CORDS_TRANSACTION,"when")))
	{
		if (!( dptr = transaction_date_and_time( atoi( vptr ) ) ))
			fprintf(h,"%s",vptr);
		else
		{
			fprintf(h,"%s",dptr);
			liberate( dptr );
		}
	}
	fprintf(h,"<th>");

	/* ----------------------------- */
	/* transaction description field */
	/* ----------------------------- */
	if (( vptr = occi_extract_atribut( yptr, "occi", _CORDS_TRANSACTION,"source")))
		fprintf(h,"%s",vptr);
	if (( vptr = occi_extract_atribut( yptr, "occi", _CORDS_TRANSACTION,"description")))
		fprintf(h," %s",vptr);
	fprintf(h,"</th>\n");

	/* ----------------------- */
	/* transaction price field */
	/* ----------------------- */

	fprintf(h,"<th><div align=right><a href='%s'>\n",price);

	/* -------------------------------------------------------------------- */
	/* TODO : need to handle rate type pricing schemes per hour, per day .. */
	/* -------------------------------------------------------------------- */
	/* TODO : need to rationalise the prices to the target account currency */
	/* -------------------------------------------------------------------- */
	if (( vptr = occi_extract_atribut( zptr, "occi", _CORDS_PRICE,"rate")))
	{
		sscanf(vptr,"%f",&f);
		fprintf(h,"%.2f",f);
		update_invoice_total( pptr, vptr );
	}

	/* ---------------------------------- */
	/* dont display the currency any more */
	/* ---------------------------------- */
	/* -------------------------------------------------------------------------------------------------- */
	/* if (( vptr = occi_extract_atribut( zptr, "occi", _CORDS_PRICE,"currency"))) fprintf(h," %s",vptr); */
	/* -------------------------------------------------------------------------------------------------- */

	fprintf(h,"</a></div></th></tr>\n");
	return;
}

/*	------------------------------------------------------------------	*/
/*	  s c r i p t e d _ t r a n s a c t i o n _ p r o c e s s i n g		*/
/*	------------------------------------------------------------------	*/
private	int	scripted_transaction_processing( char * reference, struct cords_invoice * pptr )
{
	char *	argv[10];
	int	argc=0;
	char *	result;
	if (!( rest_valid_string( pptr->processing ) ))
		return( 0 );
	else
	{
		argv[argc++] = "coobas.invoice";
		argv[argc++] = reference;
		argv[argc++] = pptr->account;
		argv[argc++] = default_publisher();
		argv[argc++] = default_operator();
		argv[argc++] = default_tls();
		argv[argc] = (char *) 0;
		if (!( result = evaluate_cordscript( pptr->processing, argc, argv )))
			return( 0 );
		else if (!( pptr->grandtotal = allocate_string( result ) ))
			return( 0 );
		else	return( 0 );
	}
}

/*	------------------------------------------------------------------	*/
/*	     p r o c e s s _ i n v o i c e _ t r a n s a c t i o n s		*/
/*	------------------------------------------------------------------	*/
private	int	process_invoice_transactions( struct cords_invoice * pptr )
{
	char *	host;
	char *	price;
	char *  state = (char *)0;
	FILE *	h;
	struct	occi_response * xxptr;
	struct	occi_response * xptr;
	struct	occi_response * yptr;
	struct	occi_response * zptr;
	struct	occi_element  * eptr;
	struct	occi_element  * fptr;
	char	buffer[1024];
	char	reference[2048];

	/* -------------------------------------- */
	/* allocate the invoice document filename */
	/* -------------------------------------- */
	if ( pptr->document )
		pptr->document = liberate( pptr->document );

	if ( pptr->total )
		pptr->total = liberate( pptr->total );

	sprintf(buffer,"rest/%s.htm",pptr->id);

	sprintf(reference,"%s/%s/%s",CooBas.identity,_CORDS_INVOICE,pptr->id);

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

	if (( xxptr = occi_delete_links( reference, _CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
		xxptr = occi_remove_response( xxptr );

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

		/* ------------------------------------------------------------- */
		/* retrieve the transaction state information                    */
                /* if state is equal to _CORDS_CLOSED_TRANSACTION_STATE  the     */
                /* transaction was already closed (included in invoice)          */
                /* ------------------------------------------------------------- */

                state = occi_extract_atribut(yptr, "occi", _CORDS_TRANSACTION, "state");
                if ((state != (char *)0) && (strncmp(state, _CORDS_CLOSED_TRANSACTION_STATE, strlen(_CORDS_CLOSED_TRANSACTION_STATE)) == 0))
                {
		    yptr = occi_remove_response( yptr );
		    liberate(host);
		    continue;
                }

		/* ------------------------------ */
		/* retrieve the price information */
		/* ------------------------------ */
		if (!( price = occi_extract_atribut( yptr, "occi", _CORDS_TRANSACTION,_CORDS_PRICE)))
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
			if ((xxptr = occi_create_link( reference, host, _CORDS_CONTRACT_AGENT, default_tls())) != (struct occi_response *) 0)
				xxptr = occi_remove_response( xxptr );

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

	/* -------------------------------------------------- */
	/* detect and perform scripted transaction processing */
	/* -------------------------------------------------- */
	if (!( rest_valid_string( pptr->processing ) ))
		return( 0 );
	else if (!( Invoice.scripting ))
		return( 0 );
	else	return( scripted_transaction_processing( reference, pptr ) );
}

/*	-------------------------------------------	*/
/* 	      c r e a t e _ i n v o i c e  		*/
/*	-------------------------------------------	*/
private	int	create_invoice(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct cords_invoice * pptr;
	pptr = vptr;
	if (!( rest_valid_string( pptr->account ) ))
		return( 0 ); 
	return( process_invoice_transactions( pptr ) );
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ i n v o i c e 	 	*/
/*	-------------------------------------------	*/
private	int	update_invoice(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct cords_invoice * pptr;
	pptr = vptr;
	if ( pptr->state > 0 )
		return(0);
	else if (!( rest_valid_string( pptr->account ) ))
		return( 0 ); 
	return( process_invoice_transactions( pptr ) );
}

private	struct	occi_interface	invoice_interface = {
	create_invoice,
	NULL,
	update_invoice,
	NULL
	};

/*	-------------------------------------------	*/
/* 	      c r e a t e _ t r a n s a c t i o n  	*/
/*	-------------------------------------------	*/
private	int	create_transaction(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct cords_transaction * pptr;
	pptr = vptr;
	if (!( pptr->account )) {
		return( 0 ); 
	}
    pptr->when = time((long *) 0);
    return( 0 );
}




private	struct	occi_interface	transaction_interface = {
	create_transaction,
	NULL,
	NULL,
	NULL
	};

/*	------------------------------------------------------------------	*/
/*		h t m l _ i n v o i c e _ r e n d e r i n g			*/
/*	------------------------------------------------------------------	*/
private	char *	html_invoice_rendering(
		struct occi_category * cptr,
		struct rest_header  * hptr )
{
	char *	vptr;
	char *	filename=(char *) 0;
	struct	rest_header * root=hptr;
	struct	rest_header * contentlength=(struct rest_header *) 0;
	struct	rest_header * contentype=(struct rest_header *) 0;
	while ( hptr )
	{
		if (!( hptr->name ))
			hptr = hptr->next;
		else if (!( strcasecmp( hptr->name, _HTTP_CONTENT_TYPE ) ))
		{
			contentype = hptr;
			hptr = hptr->next;
		}
		else if (!( strcasecmp( hptr->name, _HTTP_CONTENT_LENGTH ) ))
		{
			contentlength = hptr;
			hptr = hptr->next;
		}
		else if (!( strcasecmp( hptr->name, _OCCI_LOCATION ) ))
			return( occi_html_body( cptr, root ) );
		else if (!( strcasecmp( hptr->name, _OCCI_ATTRIBUTE ) ))
		{
			if (!( rest_valid_string( hptr->value ) ))
			{
				hptr = hptr->next;
				continue;
			}
			else if (!( strncasecmp( (vptr = hptr->value), "occi.invoice.document", strlen("occi.invoice.document") ) ))
			{
				vptr += strlen("occi.invoice.document");
				if ( *vptr == '=' )
					vptr++;
				if (!( filename = allocate_string( vptr ) ))
					return( occi_html_body( cptr, root ) );

			}
			hptr = hptr->next;
			continue;
		}
		else	hptr = hptr->next;
	}
	if (!( filename ))
		return( occi_html_body( cptr, root ) );
	else if (!( contentype ))
		return( occi_html_body( cptr, root ) );
	else if (!( contentlength ))
		return( occi_html_body( cptr, root ) );
	else
	{
		rest_replace_header( contentype, _OCCI_TEXT_HTML );
		return( occi_content_length( contentlength, filename) );
	}
}


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
	else 
	{
		process_invoice_transactions( pptr );
		autosave_cords_invoice_nodes();
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	------------------------------------------------------------------	*/
/*		c l o s e _ i n v o i c e _ t r a n s a c t i o n s		*/
/*	------------------------------------------------------------------	*/
private	int	close_invoice_transactions( struct cords_invoice * pptr )
{
    const struct cords_xlink * lptr;
    struct	occi_element 	* gptr;
    struct	occi_response 	* zptr;
    struct	occi_response 	* yptr;
    
    /* ----------------------------------------------------- */
    /* for all defined contract nodes of the current service */
    /* ----------------------------------------------------- */
    for (lptr = initialise_and_get_first_link(pptr->id); NULL != lptr; lptr = next_link(pptr->id)) {        
        /* --------------------------------------------------- */
        /* launch / invoke the required action on the contract */
        /* --------------------------------------------------- */
        if (!( zptr = occi_simple_get( lptr->target, _CORDS_SERVICE_AGENT, default_tls() ) ))
            continue;
        /* --------------------------------------- */
        /* retrieve the instruction value property */
        /* --------------------------------------- */
        else if (!(gptr = occi_locate_element(zptr->first,"occi.transaction.state" )))
            zptr = occi_remove_response ( zptr );
        else
        {            
            if (gptr->value)
            gptr->value = liberate(gptr->value);
            
            gptr->value = allocate_string(_CORDS_CLOSED_TRANSACTION_STATE);
            
            if (!( yptr = occi_simple_put( lptr->target, zptr->first, _CORDS_SERVICE_AGENT, default_tls() ) ))
            continue;
            else	
            yptr = occi_remove_response( yptr );
            
            zptr = occi_remove_response( zptr );
        }
    }
    
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
	else 
	{
		pptr->state = 1;
		autosave_cords_invoice_nodes();
		close_invoice_transactions( pptr );
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

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
	optr->html_rendering = html_invoice_rendering;
	optr->access |= _OCCI_NO_PRICING;

	if (!( optr = occi_add_action( optr,"process","",process_invoice)))
		return( 28 );
	else if (!( optr = occi_add_action( optr,"close","",close_invoice)))
		return( 28 );

	if (!( optr = comons_connection_builder( CooBas.domain ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = comons_packet_builder( CooBas.domain, "packet_coobas.xml" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

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
