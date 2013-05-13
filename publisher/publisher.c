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
#ifndef	_publisher_c
#define	_publisher_c

#include "standard.h"
#include "publication.h"
#include "publisher.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "occibuilder.h"
#include "accords.h"
#include "cordslang.h"
#include "occiclient.h"

/*	-------------------------------------	*/
/* 	f u n c t i o n   p r o t o t y p e s	*/
/*	-------------------------------------	*/

private	int	launch_publication( struct cords_publication * pptr, struct rest_client * cptr );

private	struct rest_response * publisher_delete_all( 
		struct rest_client * cptr, 
		struct rest_request * rptr,
		struct rest_response * aptr );

private	struct rest_response * publisher_delete_item( 
		struct rest_client * cptr, 
		struct rest_request * rptr,
		struct rest_response * aptr,
		char * id );

/*	---------------------------------------------	*/  
/*	p u b l i s h e r   c o n f i g u r a t i o n	*/
/*	---------------------------------------------	*/  

struct	accords_configuration Publisher = {
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
	"publisher.xml",
	"europe",
	"storage",
	"publisher",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

/*	---------------------------------------------	*/  
/*		    c h e c k _ d e b u g 		*/
/*	---------------------------------------------	*/  
public	int	check_debug()		{	return(Publisher.debug);		}

/*	---------------------------------------------	*/  
/*		    c h e c k _ d e b u g 		*/
/*	---------------------------------------------	*/  
public	int	check_verbose()		{	return(Publisher.verbose);		}
public	char *	default_publisher()	{	return((char *) 0);			}
public	char *	default_identity()	{	return((char *) 0);			}
public	char *	default_operator()	{	return(Publisher.operator);		}
public	char *	default_tls()		{	return(Publisher.tls);			}
public	char *	default_zone()		{	return(Publisher.zone);			}

/*	-------------------------------------------------	*/  
/*	r e t r i e v e _ m a s t e r _ p u b l i s h e r	*/
/*	-------------------------------------------------	*/  
public	char *	retrieve_master_publisher()		
{
	if (!( rest_valid_string( Publisher.publisher ) ))
		return((char *) 0 );
	else	return( Publisher.publisher );
}

/*	-----------------------------------------------------	*/  
/*	r e t r i e v e _ r e m o t e _ p u b l i c a t i o n	*/
/*	-----------------------------------------------------	*/  
public	char *	retrieve_remote_publication(struct cords_publication * pptr)		
{
	if (!( rest_valid_string( pptr->remote ) ))
		return((char *) 0 );
	else	return( pptr->remote );
}

/*	---------------------------------------------	*/  
/*		    	f a i l u r e			*/
/*	---------------------------------------------	*/  
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
/*		    	b a n n e r			*/
/*	---------------------------------------------	*/ 
/*	displays the module identification banner	*/
/*	---------------------------------------------	*/ 
private	int	banner()
{
	printf("\n   CO-PUB : CompatibleOne Service Publisher : Version 1.0b.0.03");
	printf("\n   Beta Version : 25/05/2012");
	printf("\n   Copyright (c) 2011, 2012 Iain James Marshall, Prologue ");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);
}

private	int	processes=0;
private	int	publisher_fork() 		{ return( ++processes ); }
private	void	publisher_kill(int a, int b)	{ return;		 }

/*	---------------------------------------------	*/  
/*		p u b l i s h e r _ l o a d		*/
/*	---------------------------------------------	*/
/*	this function loads publisher configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	publisher_load()
{
	load_accords_configuration( &Publisher, "publisher" );
	return;
}


/*	---------------------------------------------	*/
/*	  p u b l i s h e r _ i n i t i a l i s e 	*/
/*	---------------------------------------------	*/
/*	this function is called from the rest server	*/
/*	just prior to going on line to allow the host	*/
/*	to perform any initialisation before entering	*/
/*	the operational server cycle.			*/  
/*	---------------------------------------------	*/  
private	struct rest_server * publisher_initialise(  void * v,struct rest_server * sptr )
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

/*	---------------------------------------------	*/
/*	   p u b l i s h e r _ a u t h o r i s e  	*/
/*	---------------------------------------------	*/
/*	thsi function is called to consult the host 	*/
/*	to ensure that the provided users credentials	*/
/*	are authorised for access to the server.	*/
/*	---------------------------------------------	*/  
private	int	publisher_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	---------------------------------------------	*/ 
/*	    p u b l i s h e r _ e x t e n s i o n 	*/
/*	---------------------------------------------	*/
/*	this function would be called back from the	*/
/*	server operational loop upon detection of any	*/
/*	socket axtivity on an eventual extension that	*/
/*	would have been declared during initialisation.	*/  
/*	---------------------------------------------	*/  
private	struct rest_extension * publisher_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}


/*	---------------------------------------------------------	*/
/*		 s u s p e n d _ p u b l i c a t i o n 			*/
/*	---------------------------------------------------------	*/
private	struct	rest_response * suspend_publication(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_publication * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		pptr->when = 0;
		pptr->state = _OCCI_SUSPENDED;
		autosave_cords_publication_nodes("publication.xml");
	  	return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	---------------------------------------------------------	*/
/*		 r e s t a r t _ p u b l i c a t i o n 			*/
/*	---------------------------------------------------------	*/
private	struct	rest_response * restart_publication(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_publication * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		pptr->uptime = 0;
		pptr->when = time((long*) 0);
		pptr->state = _OCCI_RUNNING;
		autosave_cords_publication_nodes("publication.xml");
	  	return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	--------------------------------------------------------	*/
/*		b u i l d _ m a s t e r _ i n s t a n c e		*/
/*	--------------------------------------------------------	*/
private	struct occi_element * build_master_instance( struct cords_publication * pptr )
{
	struct	occi_element * root=(struct occi_element *) 0;
	struct	occi_element * eptr=(struct occi_element *) 0;
	struct	occi_element * foot=(struct occi_element *) 0;

	if ( retrieve_remote_publication( pptr ) )
	{
		if (!( eptr = occi_create_element( "occi.publisher.id", pptr->remote ) ))
			return( occi_remove_elements( root ) );
		else if (!( eptr->previous = foot ))
			root = eptr;
		else	foot->next = eptr;
		foot = eptr;
	}

	if (!( eptr = occi_create_element( "occi.publisher.what", pptr->what ) ))
		return( occi_remove_elements( root ) );
	else if (!( eptr->previous = foot ))
		root = eptr;
	else	foot->next = eptr;
	foot = eptr;

	if (!( eptr = occi_create_element( "occi.publisher.where", pptr->where ) ))
		return( occi_remove_elements( root ) );
	else if (!( eptr->previous = foot ))
		root = eptr;
	else	foot->next = eptr;
	foot = eptr;

	if (!( eptr = occi_create_element( "occi.publisher.why", pptr->why ) ))
		return( occi_remove_elements( root ) );
	else if (!( eptr->previous = foot ))
		root = eptr;
	else	foot->next = eptr;
	foot = eptr;

	if (!( eptr = occi_create_element( "occi.publisher.who", pptr->who ) ))
		return( occi_remove_elements( root ) );
	else if (!( eptr->previous = foot ))
		root = eptr;
	else	foot->next = eptr;
	foot = eptr;

	if (!( eptr = occi_create_element( "occi.publisher.pass", pptr->pass ) ))
		return( occi_remove_elements( root ) );
	else if (!( eptr->previous = foot ))
		root = eptr;
	else	foot->next = eptr;
	foot = eptr;

	if (!( eptr = occi_create_element( "occi.publisher.identity", (pptr->identity ? pptr->identity : "") ) ))
		return( occi_remove_elements( root ) );
	else if (!( eptr->previous = foot ))
		root = eptr;
	else	foot->next = eptr;
	foot = eptr;

	if (!( eptr = occi_create_element( "occi.publisher.zone", (pptr->zone ? pptr->zone : "") ) ))
		return( occi_remove_elements( root ) );
	else if (!( eptr->previous = foot ))
		root = eptr;
	else	foot->next = eptr;
	foot = eptr;

	if (!( eptr = occi_create_element( "occi.publisher.price", ( pptr->price ? pptr->price : "") ) ))
		return( occi_remove_elements( root ) );
	else if (!( eptr->previous = foot ))
		root = eptr;
	else	foot->next = eptr;
	foot = eptr;

	if (!( eptr = occi_create_element( "occi.publisher.rating", (pptr->rating ? pptr->rating : "") ) ))
		return( occi_remove_elements( root ) );
	else if (!( eptr->previous = foot ))
		root = eptr;
	else	foot->next = eptr;
	foot = eptr;

	if (!( eptr = occi_create_element( "occi.publisher.operator", (pptr->operator ? pptr->operator : "") ) ))
		return( occi_remove_elements( root ) );
	else if (!( eptr->previous = foot ))
		root = eptr;
	else	foot->next = eptr;
	foot = eptr;

	return( root );
}

/*	--------------------------------------------------------	*/
/*		s t o r e _ m a s t e r _ i n s t a n c e		*/
/*	--------------------------------------------------------	*/
private	int	store_master_instance( struct cords_publication * pptr, struct occi_response * zptr )
{
	struct	occi_element * eptr;
	char *	vptr;

	if (( vptr = occi_extract_atribut( zptr, "occi", _CORDS_PUBLICATION, "id" ))!=(char *) 0)
	{
		if ( pptr->remote ) 
			pptr->remote = liberate( pptr->remote );
		if (!( pptr->remote = allocate_string( vptr ) ))
			return(27);
	}
	if (( vptr = occi_extract_atribut( zptr, "occi", _CORDS_PUBLICATION, "what" ))!=(char *) 0)
	{
		if ( pptr->what ) 
			pptr->what = liberate( pptr->what );
		if (!( pptr->what = allocate_string( vptr ) ))
			return(27);
	}
	if (( vptr = occi_extract_atribut( zptr, "occi", _CORDS_PUBLICATION, "where" ))!=(char *) 0)
	{
		if ( pptr->where ) 
			pptr->where = liberate( pptr->where );
		if (!( pptr->where = allocate_string( vptr ) ))
			return(27);
	}
	if (( vptr = occi_extract_atribut( zptr, "occi", _CORDS_PUBLICATION, "why" ))!=(char *) 0)
	{
		if ( pptr->why ) 
			pptr->why = liberate( pptr->why );
		if (!( pptr->why = allocate_string( vptr ) ))
			return(27);
	}
	if (( vptr = occi_extract_atribut( zptr, "occi", _CORDS_PUBLICATION, "who" ))!=(char *) 0)
	{
		if ( pptr->who ) 
			pptr->who = liberate( pptr->who );
		if (!( pptr->who = allocate_string( vptr ) ))
			return(27);
	}
	if (( vptr = occi_extract_atribut( zptr, "occi", _CORDS_PUBLICATION, "pass" ))!=(char *) 0)
	{
		if ( pptr->pass ) 
			pptr->pass = liberate( pptr->pass );
		if (!( pptr->pass = allocate_string( vptr ) ))
			return(27);
	}
	if (( vptr = occi_extract_atribut( zptr, "occi", _CORDS_PUBLICATION, "identity" ))!=(char *) 0)
	{
		if ( pptr->identity ) 
			pptr->identity = liberate( pptr->identity );
		if (!( pptr->identity = allocate_string( vptr ) ))
			return(27);
	}
	if (( vptr = occi_extract_atribut( zptr, "occi", _CORDS_PUBLICATION, "zone" ))!=(char *) 0)
	{
		if ( pptr->zone ) 
			pptr->zone = liberate( pptr->zone );
		if (!( pptr->zone = allocate_string( vptr ) ))
			return(27);
	}
	if (( vptr = occi_extract_atribut( zptr, "occi", _CORDS_PUBLICATION, "price" ))!=(char *) 0)
	{
		if ( pptr->price ) 
			pptr->price = liberate( pptr->price );
		if (!( pptr->price = allocate_string( vptr ) ))
			return(27);
	}
	if (( vptr = occi_extract_atribut( zptr, "occi", _CORDS_PUBLICATION, "rating" ))!=(char *) 0)
	{
		if ( pptr->rating ) 
			pptr->rating = liberate( pptr->rating );
		if (!( pptr->rating = allocate_string( vptr ) ))
			return(27);
	}
	if (( vptr = occi_extract_atribut( zptr, "occi", _CORDS_PUBLICATION, "operator" ))!=(char *) 0)
	{
		if ( pptr->operator ) 
			pptr->operator = liberate( pptr->operator );
		if (!( pptr->operator = allocate_string( vptr ) ))
			return(27);
	}
	return(0);
}

/*	-------------------------------------------------	*/
/* 	m a s t e r _ c r e a t e _ p u b l i c a t i o n 	*/
/*	-------------------------------------------------	*/
private	int	master_create_publication( struct cords_publication * pptr )
{
	struct occi_response * zptr=(struct occi_response *) 0;
	struct occi_element * eptr=(struct occi_element *) 0;
	char * master;
	char * host;
	char   buffer[1024];
	if (!( master = retrieve_master_publisher() ))
		return(0);
	else	sprintf(buffer,"%s/publication/",master);

	if (!( eptr = build_master_instance( pptr )))
		return(0);

	else if (!( zptr = occi_simple_post( buffer, eptr, _CORDS_CONTRACT_AGENT, Publisher.tls ) ))
	{
		eptr = occi_remove_elements( eptr );
		return(0);
	}
	else if (!( host = occi_extract_location( zptr ) ))
	{
		zptr = occi_remove_response( zptr );	
		eptr = occi_remove_elements( eptr );
		return(0);
	}
	else	rest_add_http_prefix(buffer,1024,host);
	if (!( pptr->remote = allocate_string( buffer ) ))
	{
		zptr = occi_remove_response( zptr );	
		eptr = occi_remove_elements( eptr );
		return(0);
	}
	else
	{
		zptr = occi_remove_response( zptr );	
		eptr = occi_remove_elements( eptr );
		return(0);
	}
}

/*	-------------------------------------------	*/
/* 	    c r e a t e _ p u b l i c a t i o n 	*/
/*	-------------------------------------------	*/
private	int	create_publication(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct occi_response * zptr=(struct occi_response *) 0;
	struct	occi_kind_node * nptr;
	struct	cords_publication * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else
	{
		pptr->when  = time((long *) 0);
		pptr->uptime = 0;
		pptr->state  = _OCCI_RUNNING;
		return( master_create_publication( pptr ) );
	}
}

/*	-----------------------------------------------------	*/
/* 	m a s t e r _ r e t r i e v e _ p u b l i c a t i o n 	*/
/*	-----------------------------------------------------	*/
private	int	master_retrieve_publication( struct cords_publication * pptr )
{
	struct occi_response * zptr=(struct occi_response *) 0;
	char * master;
	char * remote;
	int    status;
	if (!( master = retrieve_master_publisher() ))
		return( 0 );
	else if (!( remote = retrieve_remote_publication(pptr) ))
		return(0);

	else if (!( zptr = occi_simple_get( remote, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return(0);
	else
	{
		status = store_master_instance( pptr, zptr );
		zptr = occi_remove_response( zptr );	
		return(status);
	}
}

/*	-------------------------------------------	*/
/* 	  r e t r i e v e _ p u b l i c a t i o n 	*/
/*	-------------------------------------------	*/
private	int	retrieve_publication(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_publication * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	{
		pptr->uptime = (time((long*) 0) - pptr->when);
		return( master_retrieve_publication( pptr ) );
	}
}

/*	-------------------------------------------------	*/
/* 	m a s t e r _ u p d a t e _ p u b l i c a t i o n 	*/
/*	-------------------------------------------------	*/
private	int	master_update_publication( struct cords_publication * pptr )
{
	struct occi_response * zptr=(struct occi_response *) 0;
	struct occi_element  * eptr=(struct occi_element *) 0;
	char * master;
	char * remote;
	if (!( master = retrieve_master_publisher() ))
		return( 0 );
	else if (!( remote = retrieve_remote_publication(pptr) ))
		return(0);
	else if (!( eptr = build_master_instance( pptr )))
		return(0);
	else if (!( zptr = occi_simple_put( remote, eptr, _CORDS_CONTRACT_AGENT, default_tls() ) ))
	{
		eptr = occi_remove_elements( eptr );
		return(0);
	}
	else
	{
		zptr = occi_remove_response( zptr );	
		eptr = occi_remove_elements( eptr );
		return(0);
	}
}

/*	-------------------------------------------	*/
/* 	    u p d a t e _ p u b l i c a t i o n 	*/
/*	-------------------------------------------	*/
private	int	update_publication(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_publication * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else
	{
		pptr->uptime = (time((long*) 0) - pptr->when);
		return( master_update_publication( pptr ) );
	}
}

/*	-------------------------------------------------	*/
/* 	m a s t e r _ d e l e t e _ p u b l i c a t i o n 	*/
/*	-------------------------------------------------	*/
private	int	master_delete_publication( struct cords_publication * pptr )
{
	struct occi_response * zptr=(struct occi_response *) 0;
	char * master;
	char * remote;
	if (!( master = retrieve_master_publisher() ))
		return( 0 );
	else if (!( remote = retrieve_remote_publication(pptr) ))
		return(0);
	else if (!( zptr = occi_simple_delete( remote, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return(0);
	else
	{
		zptr = occi_remove_response( zptr );	
		return(0);
	}
}

/*	-------------------------------------------	*/
/* 	    d e l e t e _ p u b l i c a t i o n 	*/
/*	-------------------------------------------	*/
private	int	delete_publication(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_publication * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return( master_delete_publication( pptr ) );
}

private	struct	occi_interface	publication_interface = {
	create_publication,
	retrieve_publication,
	update_publication,
	delete_publication
	};

/*	-------------------------------------------	*/
/* 	    c r e a t e _ e n q u i r y 	*/
/*	-------------------------------------------	*/
private	int	create_enquiry(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	enquiry * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	  r e t r i e v e _ e n q u i r y 	*/
/*	-------------------------------------------	*/
private	int	retrieve_enquiry(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	enquiry * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	    u p d a t e _ e n q u i r y 	*/
/*	-------------------------------------------	*/
private	int	update_enquiry(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	enquiry * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	    d e l e t e _ e n q u i r y 	*/
/*	-------------------------------------------	*/
private	int	delete_enquiry(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	enquiry * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

private	struct	occi_interface	enquiry_interface = {
	create_enquiry,
	retrieve_enquiry,
	update_enquiry,
	delete_enquiry
	};

/*	-------------------------------------------	*/
/* 	    c r e a t e _ a g e n c y 	*/
/*	-------------------------------------------	*/
private	int	create_agency(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_agency * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else
	{
		pptr->start  = time((long *) 0);
		pptr->uptime = 0;
		pptr->state  = 1;
		return(0);
	}
}

/*	-------------------------------------------	*/
/* 	  r e t r i e v e _ a g e n c y 	*/
/*	-------------------------------------------	*/
private	int	retrieve_agency(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_agency * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else
	{
		pptr->uptime = (time((long*) 0) - pptr->start);
		return(0);
	}
}

/*	-------------------------------------------	*/
/* 	    u p d a t e _ a g e n c y 	*/
/*	-------------------------------------------	*/
private	int	update_agency(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_agency * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	    d e l e t e _ a g e n c y 	*/
/*	-------------------------------------------	*/
private	int	delete_agency(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_agency * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

private	struct	occi_interface	agency_interface = {
	create_agency,
	retrieve_agency,
	update_agency,
	delete_agency
	};

/*	---------------------------------------------------------	*/
/*		 	s t o p _ a g e n c y				*/
/*	---------------------------------------------------------	*/
private	struct	rest_response * stop_agency(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_agency * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		pptr->start = 0;
		pptr->state = 0;
		autosave_cords_agency_nodes("agency.xml");
	  	return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	---------------------------------------------------------	*/
/*		 	s t a r t _ a g e n c y			*/
/*	---------------------------------------------------------	*/
private	struct	rest_response * start_agency(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_agency * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		pptr->uptime = 0;
		pptr->start = time((long*) 0);
		pptr->state = 1;
		autosave_cords_agency_nodes("agency.xml");
	  	return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	---------------------------------------------	*/  
/*	    p u b l i s h e r _ o p e r a t i o n	*/
/*	---------------------------------------------	*/  
private	int	publisher_operation( char * nptr )
{
	struct	occi_category * optr;
	struct	occi_category * first = (struct occi_category *) 0;
	struct	occi_category * last  = (struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_publisher.xml");

	/* ----------------------------- */
	/* general publications category */
	/* ----------------------------- */
	if (!( optr = occi_cords_publication_builder( Publisher.domain,"publication" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = &publication_interface;
	optr->access |= (_OCCI_NO_PRICING | _OCCI_NO_AUTHORIZE);

	if (!( optr = occi_add_action( optr,"suspend","",suspend_publication)))
		return( 27 );
	else if (!( optr = occi_add_action( optr,"restart","",restart_publication)))
		return( 27 );

	if (!( optr = occi_cords_enquiry_builder( Publisher.domain, "enquiry" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = &enquiry_interface;
	optr->access |= _OCCI_NO_PRICING;

	if (!( optr = occi_cords_operator_builder( Publisher.domain, "operator" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->access |= _OCCI_NO_PRICING;

	if (!( optr = occi_cords_agency_builder( Publisher.domain, "agency" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = &agency_interface;
	optr->access |= _OCCI_NO_PRICING;
	
	if (!( optr = occi_add_action( optr,"start","",start_agency)))
		return( 27 );
	else if (!( optr = occi_add_action( optr,"stop","",stop_agency)))
		return( 27 );

	Publisher.monitor &= ~2;

	rest_initialise_log(Publisher.monitor);

	/* ------------------------------------- */
	/* launch the rest/occi server operation */
	/* ------------------------------------- */
	if ( Publisher.tls )
		if (!( strlen( Publisher.tls ) ))
			Publisher.tls = (char *) 0;
	return( occi_server(  	nptr, 
				Publisher.restport, 
				Publisher.tls, 
				Publisher.threads, first,(char *) 0 ));
}


/*	--------------------------------------------------	*/
/*			p u b l i s h e r			*/
/*	--------------------------------------------------	*/
private	int	publisher(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;

	publisher_load();
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
				Publisher.verbose=1;
				continue;
			case	'd'	:
				Publisher.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;

			}
			status = 30;
			break;
		}
		else if (!( status = publisher_operation(aptr) ))
			continue;
		else	break;
	}
	return(status);
}

/*	---------------------------------------------	*/ 
/*			m a i n  			*/
/*	---------------------------------------------	*/
/*	the entry point to the publisher operation.	*/  
/*	---------------------------------------------	*/  
public	int	main(int argc, char * argv[] )
{
	set_xml_echo(1);
	if ( argc == 1 )
		return( banner() );
	else	return( publisher( argc, argv ) );
}

	/* ------------ */
#endif	/* _publisher_c */
	/* ------------ */



