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
#ifndef	_cool_c	
#define	_cool_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "restthread.h"
#include "occi.h"
#include "occibuilder.h"
#include "occiclient.h"
#include "document.h"
#include "cool.h"
#include "cordslang.h"
#include "cb.h"
#include "url.h"

/* 	----------------------------------------	*/	
/*	Prototype Contract Negotiation Functions	*/
/* 	----------------------------------------	*/
struct	accords_configuration Cool = {
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
	"cool.xml",
	"europe",
	"storage",	
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(Cool.debug);		}
public	int	check_verbose()		{	return(Cool.verbose);		}
public	char *	default_publisher()	{	return(Cool.publisher);		}
public	char *	default_operator()	{	return(Cool.operator);		}
public	char *	default_tls()		{	return(Cool.tls);		}
public	char *	default_zone()		{	return(Cool.zone);		}

private	int	cool_create_job( char * contract, char * nptr );
private	int	cool_create_workload( char * contract, int type );
private	struct elastic_contract * scaledown_elastic_contract( struct elastic_contract * contract );
private	struct elastic_contract * scaleup_elastic_contract( char * contract, int allocate );

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

/*	---------------------------------------------------------------	*/  
/*			c o o l _ l o g _ m e s s a g e			*/
/*	---------------------------------------------------------------	*/  
private	void	cool_log_message( char * message, int priority )
{
#ifdef	_FINAL_VERSION
	if ( priority )
		rest_log_message( message );
	else if ( check_debug() )
#endif
		rest_log_message( message );
	return;
}

/*	---------------------------------------------------------------	*/  
/*	c o o l _ c o n f i g u r a t i o n				*/
/*	---------------------------------------------------------------	*/  
/*	this function loads cool configuration				*/
/*	from the xml configuration file.				*/
/*	---------------------------------------------------------------	*/  
private	void	cool_configuration()
{
	load_accords_configuration( &Cool, "cool" );
	return;
}

/*	---------------------------------------------------------------	*/  
/*	c o o l _ b a n n e r						*/
/*	---------------------------------------------------------------	*/  
/*	this function will be called to display module identification 	*/
/*	when launched from the command line without any parameters.	*/
/*	---------------------------------------------------------------	*/  
private	int	cool_banner()
{
	printf("\n   CompatibleOne Elasticity Manager : Version 1.1a.0.01");
	printf("\n   Beta Version : 03/03/2013 ");
	printf("\n   Copyright (c) 2013 Iain James Marshall, Prologue");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);
}

/*	--------------------------------------- 	*/
/*	c o o l _ i n i t i a l i s e			*/
/*	--------------------------------------- 	*/
private	struct rest_server * cool_initialise(  void * v,struct rest_server * sptr )
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

/*	-------------------------------------------	*/
/*	c o o l _ a u t h o r i s e 			*/
/*	------------------------------------------- 	*/
private	int	cool_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, Cool.user ) )
		return(0);
	else if ( strcmp( password, Cool.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------- 	*/
/*	c o o l _ e x t e n s i o n 			*/
/*	-------------------------------------------	*/
private	struct rest_extension * cool_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

#include "comonsconnection.c"
#include "job.c"
#include "occijob.c"
#include "workload.c"
#include "occiworkload.c"


/*	-------------------------------------------	*/
/*	E l a s t i c i t y   M a n a g e m e n t		*/
/*	-------------------------------------------	*/
private	struct	elastic_control Elastic = 
{
	/* ------------------------------------ */
	/* will be provided through environment	*/
	/* ------------------------------------ */
	(char *) 0, 	/* elastic job occi id	*/
	(char *) 0,	/* elastic security	*/
	0,	/* use elastic occi		*/
	80,	/* elastic rest port		*/
	1,	/* elastic floor 		*/
	2, 	/* elastic ceiling		*/
	0,	/* elastic strategy		*/
	10,	/* elastic elastic upper	*/
	2,	/* lower			*/
	10,	/* elastic unit			*/
	60,	/* elastic period		*/
	/* ------------------------------------ */
	/* will be calculated during operation  */
	/* ------------------------------------ */
	6,	/* elastic units		*/
	0,	/* first unit			*/
	5,	/* last unit			*/
	/* ------------------------------------ */
	/* to be retrieved from first contract  */
	/* ------------------------------------ */
	(char*) 0, /* template contract id 	*/
	(char*) 0, /* template contract name	*/
	(char*) 0, /* parent service		*/
	(char*) 0, /* service level agreement	*/
	0,	   /* total count of contracts	*/
	0,	   /* count of active contracts	*/
	0,	   /* total start duration	*/
	0,	   /* total stop duration	*/
	0,	   /* average start duration	*/
	0,	   /* average stop duration	*/
	/* ------------------------------------ */
	/* operational load balancing variables */
	/* ------------------------------------ */
	0,	/* total hit count		*/
	0,	/* max hit count per period	*/
	0,	/* last hit time		*/
	(struct elastic_contract *) 0, /* first	*/
	(struct elastic_contract *) 0, /* last  */
	(struct elastic_contract *) 0  /* next  */
};

/*	-----------------------------------------------------------	*/
/*		   	c o o l _ l o c k 				*/
/*	-----------------------------------------------------------	*/
private	void	cool_lock()
{
	pthread_mutex_lock( &Elastic.lock );
	return;
}

/*	-----------------------------------------------------------	*/
/*			c o o l _ u n l o c k 				*/
/*	-----------------------------------------------------------	*/
private	void	cool_unlock()
{
	pthread_mutex_unlock( &Elastic.lock );
	return;
}

/*	---------------------------------------------------------	*/
/*			l b _ f a i l u r e				*/
/*	---------------------------------------------------------	*/
private	struct rest_response * lb_failure(struct rest_client * cptr,  int status, char * message )
{
	struct	rest_response * aptr;
	if (!( aptr = rest_allocate_response(cptr)))
		return( aptr );
	else	return( rest_html_response( aptr, status, message ) );
}


/*	-------------------------------------------	*/
/* 		s c a l e u p _ j o b 			*/
/*	-------------------------------------------	*/
private	struct rest_response * scaleup_job(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_job * pptr;
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	struct	occi_response * zptr;
	struct	occi_element  * eptr;
	char *	wptr;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Failure" ) );
	else if ( Elastic.active < Elastic.ceiling )
	{
		scaleup_elastic_contract( Elastic.first->contract, 1 );
		return( rest_html_response( aptr, 200, "OK" ) );
	}
	else	return( rest_html_response( aptr, 400, "Elastic Ceiling Reached" ) );
}

/*	-------------------------------------------	*/
/* 		s c a l e d o w n _ j o b 		*/
/*	-------------------------------------------	*/
private	struct rest_response * scaledown_job(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_job * pptr;
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	struct	occi_response * zptr;
	struct	occi_element  * eptr;
	char *	wptr;

	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Failure" ) );

	else if ( Elastic.active > Elastic.floor )
	{
		scaledown_elastic_contract( Elastic.last );
		return( rest_html_response( aptr, 200, "OK" ) );
	}
	else	return( rest_html_response( aptr, 400, "Elastic Floor Reached" ) );
	
}


/*	--------------------------------------------------------------	*/
/*		l i b e r a t e _ e l a s t i c _ c o n t r a c t 		*/
/*	--------------------------------------------------------------	*/
private	struct elastic_contract * liberate_elastic_contract(struct	elastic_contract * eptr)
{
	struct	occi_response * zptr;
	if ( eptr )
	{
		/* ----------------------------- */
		/* clean up the elastic contract */
		/* ----------------------------- */
		if ( eptr->hostname )
			eptr->hostname = liberate( eptr->hostname );

		/* ------------------------------- */
		/* clean up the negotiation fields */
		/* ------------------------------- */
		if ( eptr->parentservice )
			eptr->parentservice = liberate( eptr->parentservice );
		if ( eptr->agreement )
			eptr->agreement = liberate( eptr->agreement );
		if ( eptr->service )
			eptr->service = liberate( eptr->service );
		if ( eptr->contract )
			eptr->contract = liberate( eptr->contract );
		if ( eptr->zptr )
			eptr->zptr = occi_remove_response( eptr->zptr );
		if ( eptr->yptr )
			eptr->yptr = occi_remove_response( eptr->yptr );
		if ( eptr->xptr )
			eptr->xptr = occi_remove_response( eptr->xptr );
		if ( eptr->wptr )
			eptr->wptr = occi_remove_response( eptr->wptr );
		if ( eptr->profile )
			eptr->profile = liberate( eptr->profile );
		if ( eptr->provision )
			eptr->provision = liberate( eptr->provision );
		if ( eptr->account )
			eptr->account = liberate( eptr->account );
		if ( eptr->accountname )
			eptr->accountname = liberate( eptr->accountname );
		if ( eptr->node )
			eptr->node = liberate( eptr->node );
		if ( eptr->name )
			eptr->name = liberate( eptr->name );

		/* --------------------------------------------- */
		/* remove from the linked list if it is attached */
		/* --------------------------------------------- */
		if (( eptr->previous ) || ( eptr->next ))
		{
			cool_lock();
			if (!( eptr->previous ))
				Elastic.first = eptr->next;
			else	eptr->previous->next = eptr->next;
			if (!( eptr->next ))
				Elastic.last = eptr->previous;
			else	eptr->next->previous = eptr->previous;
			cool_unlock();
		}

		eptr = liberate( eptr );
	}
	return((struct elastic_contract *) 0);
}

/*	--------------------------------------------------------------	*/
/*		a l l o c a t e _ e l a s t i c _ c o n t r a c t 		*/
/*	--------------------------------------------------------------	*/
private	struct elastic_contract * allocate_elastic_contract()
{
	struct	elastic_contract * eptr;
	if (!( eptr = (struct elastic_contract *) allocate( sizeof( struct elastic_contract ) ) ))
		return( eptr );
	else	memset( (char *) eptr, 0, sizeof( struct elastic_contract ) );

	if (!( eptr->service = allocate_string( ( rest_valid_string( Elastic.security ) ? "https" : "http" ) )))
		return( liberate_elastic_contract( eptr ) );
	else	eptr->port = Elastic.port;
	return( eptr );
}

/*	---------------------------------------------------------	*/
/*		n e x t _ e l a s t i c _ c o n t r a c t 	*/
/*	---------------------------------------------------------	*/
private	struct elastic_contract * next_elastic_contract()
{
	if (!( Elastic.current ))
		return( (Elastic.current = Elastic.first) );
	else if (!( Elastic.current->next ))
		return( (Elastic.current = Elastic.first) );
	else	return( (Elastic.current = Elastic.current->next ) );
}

/*	---------------------------------------------------------	*/
/*		n e x t _ e l a s t i c _ c o n t r a c t 	*/
/*	---------------------------------------------------------	*/
private	struct elastic_contract * cool_next_elastic_contract()
{
	struct	elastic_contract * eptr;
	cool_lock();
	eptr = next_elastic_contract();
	cool_unlock();
	return(eptr);
}

/*	--------------------------------------------	*/
/*	c o o l _ a v e r a g e _ d u r a t i o n s	*/
/*	--------------------------------------------	*/
/*	calculates the average start/stop durations	*/
/*	for current collection of active contracts.	*/
/*	This gives a precise indication of how long	*/
/*	it would take to start a new contract.		*/
/*	--------------------------------------------	*/
private	void	cool_average_durations()
{
	char	buffer[1024];
	struct elastic_contract * eptr;

	cool_log_message("calculate_average_durations",1);

	Elastic.active			=
	Elastic.total_start_duration	=
	Elastic.total_stop_duration	=
	Elastic.average_start_duration	=
	Elastic.average_stop_duration	= 0;

	for (	eptr=Elastic.first;
		eptr != (struct elastic_contract *) 0;
		eptr = eptr->next )
	{
		if (!( eptr->isactive ))
			continue;
		else
		{
			Elastic.active++;
			Elastic.total_start_duration += eptr->startduration;
			Elastic.total_stop_duration += eptr->stopduration;
		}
	}

	if ( Elastic.active )
	{
		Elastic.average_start_duration = (Elastic.total_start_duration / Elastic.active);
		Elastic.average_stop_duration  = (Elastic.total_stop_duration  / Elastic.active);
		sprintf(buffer,"average_start_duration=%u( tot=%u / nb=%u )",
			Elastic.average_start_duration,Elastic.total_start_duration,Elastic.active);
		cool_log_message( buffer, 1 );
		sprintf(buffer,"average_stop_duration=%u( tot=%u / nb=%u )",
			Elastic.average_stop_duration,Elastic.total_stop_duration,Elastic.active);
		cool_log_message( buffer, 1 );
	}

	return;	
}

/*	---------------------------------------------	*/
/*	c o o l _ r e t r i e v e _ d u r a t i o n s	*/
/*	---------------------------------------------	*/
private	void	cool_retrieve_durations( 
		struct elastic_contract * eptr , 
		struct occi_response * yptr )
{
	char *	result;

	if (!( eptr ))
		return;
	else if (!( yptr ))
		return;

	cool_log_message("retrieve_durations",1);

	/* ---------------------------------------------- */
	/* retrieve and store the start and stop duration */
	/* ---------------------------------------------- */
	if (( result = occi_extract_atribut( 
			yptr, Cool.domain, 
			_CORDS_CONTRACT, _CORDS_STARTDURATION )) != (char * ) 0)
		eptr->startduration = atoi( result );

	/* ---------------------------------------------- */
	/* retrieve and store the start and stop duration */
	/* ---------------------------------------------- */
	if (( result = occi_extract_atribut( 
		yptr, Cool.domain, 
		_CORDS_CONTRACT, _CORDS_STOPDURATION )) != (char * ) 0)
		eptr->stopduration = atoi( result );

	/* --------------------------- */
	/* calculate the statistic now */
	/* --------------------------- */
	cool_average_durations();
	return;
}

/*	--------------------------------------------	*/
/*	  u s e _ e l a s t i c _ c o n t r a c t	*/
/*	--------------------------------------------	*/
private	struct elastic_contract * use_elastic_contract( struct elastic_contract * eptr, char * contract)
{
	char	buffer[2048];
	struct	occi_response * zptr;
	char *	result;

	cool_log_message("use_elastic_contract",1);

	/* --------------------------------- */
	/* retrieve the contract information */
	/* --------------------------------- */
	if (!( contract ))
		return( liberate_elastic_contract( eptr ) );
	else if (!( zptr = occi_simple_get( contract , _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( liberate_elastic_contract( eptr ) );

	/* ----------------------------- */
	/* store the contract identifier */
	/* ----------------------------- */
	else if (!( eptr->contract ))
		if (!( eptr->contract = allocate_string( contract ) ))
			return( liberate_elastic_contract( eptr ) );

	/* ---------------------- */
	/* if not secondary usage */
	/* ---------------------- */
	if (!( eptr->allocated & 2 ))
	{
		/* ---------------------------------------------- */
		/* retrieve and store the contract hostname value */
		/* ---------------------------------------------- */
		if (!( result = occi_extract_atribut( 
			zptr, Cool.domain, 
			_CORDS_CONTRACT, _CORDS_HOSTNAME )))
			return( liberate_elastic_contract( eptr ) );
		else if (!( eptr->hostname = allocate_string( result ) ))
			return( liberate_elastic_contract( eptr ) );
	}

	/* ------------------------------- */
	/* append to the list of contracts */
	/* ------------------------------- */
	cool_lock();
	if (!( eptr->previous = Elastic.last ))
		Elastic.first = eptr;
	else	eptr->previous->next = eptr;
	Elastic.last = eptr;
	cool_unlock();
	Elastic.total++;

	/* -------------------------------- */
	/* if this is the template contract */
	/* -------------------------------- */
	if (!( eptr->allocated ))
	{
		if (!( Elastic.contract = allocate_string( contract )))
			return( liberate_elastic_contract( eptr ) );

		/* ------------------------------ */
		/* the parent service is required */
		/* ------------------------------ */
		else if (!( result = occi_extract_atribut( 
			zptr, Cool.domain, 
			_CORDS_CONTRACT, _CORDS_PARENTSERVICE )))
			return( liberate_elastic_contract( eptr ) );
		else if (!( Elastic.parentservice = allocate_string( result ) ))
			return( liberate_elastic_contract( eptr ) );

		/* ----------------------------- */
		/* the contract name is required */
		/* ----------------------------- */
		else if (!( result = occi_extract_atribut( 
			zptr, Cool.domain, 
			_CORDS_CONTRACT, _CORDS_NAME )))
			return( liberate_elastic_contract( eptr ) );
		else if (!( Elastic.contractname = allocate_string( result ) ))
			return( liberate_elastic_contract( eptr ) );

		/* ------------------------- */
		/* the agreement is optional */
		/* ------------------------- */
		else if (( result = occi_extract_atribut( 
			zptr, Cool.domain, 
			_CORDS_CONTRACT, _CORDS_AGREEMENT )) != (char * ) 0)
			if (!( Elastic.agreement = allocate_string( result ) ))
				return( liberate_elastic_contract( eptr ) );

		/* ------------------------------- */
		/* indicate the template is active */
		/* ------------------------------- */
		eptr->isactive = 1;

		cool_retrieve_durations( eptr, zptr );

	}

	/* ---------------------------------------------------------------- */
	/* add the contract to the list of contracts managed by the service */
	/* this must be performed for newly allocated contracts and not for */
	/* the primary template contract nor the recovered contracts.	    */ 
	/* ---------------------------------------------------------------- */
	if ( eptr->allocated & 1 )
	{
		cool_log_message("link_elastic_contract",1);
		if ((zptr = occi_create_link( Elastic.parentservice, eptr->contract, _CORDS_SERVICE_AGENT, default_tls() )) != (struct occi_response *) 0)
			zptr = occi_remove_response( zptr );
	}

	zptr = occi_remove_response( zptr );
	return(eptr);
	

}

/*	---------------------------------------------------	*/
/*	n e g o t i a t e _ e l a s t i c _ c o n t r a c t	*/
/*	---------------------------------------------------	*/
private	char *	negotiate_elastic_contract(
			char * node,char * name, char * user, 
			struct cords_placement_criteria * selector, 
			struct cords_guarantee_criteria * warranty,
			char * agreement)
{
	char *	contract=(char *) 0;
	struct	xml_element * document=(struct xml_element *) 0;
	struct	xml_atribut * aptr;
	cool_log_message("cool:negotiate_elastic_contract",0);

	if ( selector )
		selector->flags = _INHIBIT_AUTOSTART;

	if (!( document = cords_instance_node(
		selector, warranty, name, node, _CORDS_CONTRACT_AGENT, default_tls(), agreement, user, user, user) ))
		return( (char *) 0 );

	else if (!( aptr = document_atribut( document, _CORDS_ID ) ))
	{
		document = document_drop( document );
		return((char * ) 0);
	}
	else if (!( contract = allocate_string( aptr->value ) ))
	{
		document = document_drop( document );
		return((char * ) 0);
	}
	else
	{
		document = document_drop( document );
		cool_log_message("cool:negotiate_elastic_contract:done",0);
		return(contract);
	}
}

/*	--------------------------------------------------	*/
/*	   c o o l _ t r a n s f o r m _ c o n t r o l 		*/
/*	--------------------------------------------------	*/
private	char *	cool_transform_control( char * control, char * source, char * target )
{
	struct	occi_response	* zptr;
	struct	occi_element	* dptr;
	struct	occi_response	* xptr;
	struct	occi_element	* fptr;
	char *	ihost=(char *) 0;
	char *	result=(char *) 0;
	char	buffer[2048];

	/* --------------------------- */
	/* retrieve the source control */
	/* --------------------------- */
	if  (!(zptr = occi_simple_get( control, _CORDS_CONTRACT_AGENT, default_tls() )))
		return((char *) 0);

	else if ((!( zptr->response )) || ( zptr->response->status > 299 ))
	{
		zptr = occi_remove_response( zptr );
		return((char *) 0);
	}

	/* ------------------------------------ */
	/* initate the control creation message */
	/* ------------------------------------ */
	if ((!( fptr = occi_create_element(       "occi.control.name",     target ) ))
	||  (!( fptr = occi_append_element( fptr, "occi.control.contract", target ) )))
	{
		zptr = occi_remove_response( zptr );
		return((char *) 0);
	}

	/* ------------------------------------------------ */
	/* duplicate the elements of the control to be kept */
	/* ------------------------------------------------ */
	for (	dptr = zptr->first;
		dptr != (struct occi_element *) 0;
		dptr = dptr->next )
	{
		if ((!( dptr->name ))
		||  (!( dptr->value)))
			continue;
		else if ((!( strcmp( dptr->name, "occi.control.agreement" 	) ))
		     ||  (!( strcmp( dptr->name, "occi.control.property" 	) ))
		     ||  (!( strcmp( dptr->name, "occi.control.condition" 	) ))
		     ||  (!( strcmp( dptr->name, "occi.control.objective" 	) ))
		     ||  (!( strcmp( dptr->name, "occi.control.reference" 	) ))
		     ||  (!( strcmp( dptr->name, "occi.control.importance" 	) ))
		     ||  (!( strcmp( dptr->name, "occi.control.obligated" 	) ))
		     ||  (!( strcmp( dptr->name, "occi.control.scope" 		) )))
		{
			if (!( fptr = occi_append_element( fptr, dptr->name, dptr->value ) ))
			{
				zptr = occi_remove_response( zptr );
				return((char *) 0);
			}
		}
	}

	/* ---------------------------- */
	/* clean up from previous phase */
	/* ---------------------------- */
	zptr = occi_remove_response( zptr );

	/* ------------------------------------------- */
	/* recover the identity of the CONTROL manager */
	/* ------------------------------------------- */
	if (!( ihost = occi_resolve_category_provider( _CORDS_CONTROL, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return((char *) 0);
	else
	{
		sprintf(buffer,"%s/%s/",ihost,_CORDS_CONTROL);
		liberate( ihost );
	}

	/* ---------------------------------------- */
	/* creat the new control and extract the ID */
	/* ---------------------------------------- */
	if  ((xptr = occi_simple_post( buffer, fptr, _CORDS_CONTRACT_AGENT, default_tls() )) !=  (struct occi_response *) 0)
	{
		/* ---------------------------------------- */
		/* extract the instruction ID from response */
		/* ---------------------------------------- */
		if (( result = occi_extract_location( xptr )) != (char *) 0)
			result = allocate_string( result );
		xptr = occi_remove_response( xptr );
	}

	return( result );
}

/*	--------------------------------------------------	*/
/*	 c o o l _ t r a n f o r m _ in s t r u c t i o n	*/
/*	--------------------------------------------------	*/
/* 	duplicates the occi element list of an instruction	*/
/*	and filters out the id and status, replacing the 	*/
/*	previous provision with the new provider and then	*/
/*	transforming all others	instructions that reference 	*/
/*	the previous source contract to refer to the new 	*/
/*	target contract.							*/
/*	--------------------------------------------------	*/ 
private	struct	occi_element * cool_transform_instruction( 
		struct occi_element * eptr, 
		char * source, char * result, char * provision )
{
	int	ismonitor=0;
	char *	control=(char *) 0;
	char *	newcontrol=(char *) 0;
	struct	occi_element * root=(struct occi_element *) 0;
	struct	occi_element * foot=(struct occi_element *) 0;
	struct	occi_element * work=(struct occi_element *) 0;

	cool_log_message("transform instruction",0);

	for (	;
		eptr != (struct occi_element *) 0;
		eptr = eptr->next )
	{
		/* -------------------------- */
		/* filter out unwanted fields */
		/* -------------------------- */
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;
		else if (!( strcmp( eptr->name, "occi.core.id" ) ))
			continue;
		else if (!( strcmp( eptr->name, "occi.core.status" ) ))
			continue;

		/* ------------------------------- */
		/* detect a monitoring instruction */
		/* ------------------------------- */
		if ((!( strcmp( eptr->name, "occi.instruction.method" ) )) && (!( strcmp( eptr->value , _CORDS_MONITOR ) )))
			ismonitor=1;

		else if ((!( strcmp( eptr->name, "occi.instruction.type" ) )) && (!( strcmp( eptr->value , _CORDS_MONITOR ) )))
			ismonitor=1;

		if (!( strcmp( eptr->name, "occi.instruction.provision" ) ))
		{
			/* ---------------------------------------------------------------- */
			/* create a new element and replace the reference to the provision  */
			/* ---------------------------------------------------------------- */
			if (!( work = occi_create_element( eptr->name, provision ) ))
				continue;
		}

		/* --------------------------------------------------------------- */
		/* detect a value for a monitor which needs control transformation */
		/* --------------------------------------------------------------- */
		else if (!( strcmp( eptr->name, "occi.instruction.value" ) ))
		{
			if ( ismonitor )
			{
				/* ------------------------------------------ */
				/* duplicate the control with new contract ID */
				/* ------------------------------------------ */
				if (!( newcontrol = cool_transform_control( eptr->value, source, result ) ))
					continue;
				/* ---------------------------------------------------------------- */
				/* create a new element and replace the reference to the provision  */
				/* ---------------------------------------------------------------- */
				else if (!( work = occi_create_element( eptr->name, newcontrol ) ))
					continue;
			}
			/* ---------------------------------------------------------------- */
			/* create a new element and replace a reference to the old contract */
			/* ---------------------------------------------------------------- */
			else if (!( work = occi_create_element( 
					eptr->name, 
					( strcmp( eptr->value, source ) ? eptr->value : result ) ) ))
				continue;
		}		
		/* ---------------------------------------------------------------- */
		/* create a new element and replace a reference to the old contract */
		/* ---------------------------------------------------------------- */
		else if (!( work = occi_create_element( 
				eptr->name, 
				( strcmp( eptr->value, source ) ? eptr->value : result ) ) ))
			continue;

		/* ------------------------------ */
		/* append to the new element list */
		/* ------------------------------ */
		if (!( work->previous = foot ))
			root = work;
		else	work->previous->next = work;
		foot = work;
	}
	return( root );
}

/*	--------------------------------------------------	*/
/*	  c o o l _ d u p l i c a t e _ c o n t r a c t		*/
/*	--------------------------------------------------	*/
private	int	cool_duplicate_contract( char * result, char * source, char * provision )
{

	char	*	ihost;
	char 	*	vptr;
	struct	occi_response * zptr=(struct occi_response *) 0;
	struct	occi_response * zzptr=(struct occi_response *) 0;
	struct	occi_response * zyptr=(struct occi_response *) 0;
	struct	occi_response * yptr=(struct occi_response *) 0;
	struct	occi_element  * fptr=(struct occi_element  *) 0;
	struct	occi_element  * eptr=(struct occi_element  *) 0;
	struct	occi_client   * kptr=(struct occi_client   *) 0;
	struct	occi_request  * qptr=(struct occi_request  *) 0;
	char *	iid;
	char	instruction[4096];
	char	buffer[4096];
	char	tempname[4096];
	int	length=0;

	cool_log_message("duplicate contract",0);

	/* ---------------------------------------------------------------- */
	/* select / retrieve instruction category service provider identity */
	/* ---------------------------------------------------------------- */
	if (!( ihost = occi_resolve_category_provider( _CORDS_INSTRUCTION, _CORDS_CONTRACT_AGENT, default_tls() ) ))
	 	return( 401 );

	/* ---------------------------------------------------------------- */
	/* retrieve the collection of instructions for the current contract */
	/* ---------------------------------------------------------------- */
	sprintf(instruction,"%s/%s/",ihost,_CORDS_INSTRUCTION);
	liberate( ihost );
	length = strlen(instruction);

	if (!( kptr = occi_create_client( instruction, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( 401 );

	else if (!(qptr = occi_create_request( 
			kptr, 
			kptr->target->object, 
			_OCCI_NORMAL )))
		return(401);
	else if (!( fptr = occi_request_element( 
			qptr,
			"occi.instruction.target", source ) ))
	{
		qptr = occi_remove_request( qptr );
		return( 401 );
	}
	else if (!( yptr = occi_client_get( kptr, qptr ) ))
	{
		qptr = occi_remove_request( qptr );
		return( 401 );
	}
	else	qptr = occi_remove_request ( qptr );

	/* ---------------------------------------------------- */
	/* for each of the instructions of the current contract */
	/* ---------------------------------------------------- */
	for (	eptr = yptr->first;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;
		else if (!( vptr = allocate_string( eptr->value )))
			continue;
		else if (!( vptr = occi_category_id( vptr ) ))
			continue;
		else
		{
			strcat( instruction, vptr );
			liberate( vptr );
		}

		/* ----------------------------------------- */
		/* retrieve the current instruction instance */
		/* ----------------------------------------- */
		zptr = occi_simple_get( instruction, _CORDS_CONTRACT_AGENT, default_tls() );

		/* ----------------------- */
		/* quick reset of base url */
		/* ----------------------- */
		instruction[length] = 0;

		/* ----------------------------------- */
		/* transform the retrieved instruction */
		/* ----------------------------------- */
		if ( zptr != (struct occi_response *) 0)
		{
			/* ------------------------------------------------------------ */
			/* duplicate and transform the information of this instruction  */ 
			/* the new contract and its associated provisioning contract	*/
			/* ------------------------------------------------------------ */
			if (( fptr = cool_transform_instruction( zptr->first , source, result, provision )) != (struct occi_element *) 0)
			{
				if  ((zyptr = occi_simple_post( instruction, fptr, _CORDS_CONTRACT_AGENT, default_tls() )) !=  (struct occi_response *) 0)
				{
					/* ---------------------------------------- */
					/* extract the instruction ID from response */
					/* ---------------------------------------- */

					if (!( iid = occi_extract_location( zyptr ) ))
						zyptr = occi_remove_response( zyptr );
					else if (!( iid = allocate_string( iid ) ))
						zyptr = occi_remove_response( zyptr );
					else
					{
						zyptr = occi_remove_response( zyptr );

						/* ---------------------------------------- */
						/* link the instruction to the new contract */
						/* ---------------------------------------- */
						if ((zzptr = occi_create_link( result, iid, _CORDS_CONTRACT_AGENT, default_tls() )) !=  (struct occi_response *) 0)
							zzptr = occi_remove_response ( zzptr );
						iid = liberate( iid );
					}
				}
			}
			zptr = occi_remove_response ( zptr );
		}

	}

	yptr = occi_remove_response ( yptr );

	return(0);
}

/*	-------------------------------------------------	*/
/*	   s t a r t _ e l a s t i c _ c o n t r a c t		*/
/*	-------------------------------------------------	*/
/*	invoke the start action then recover statistics		*/
/*	-------------------------------------------------	*/
private	int	start_elastic_contract( struct elastic_contract * eptr )
{
	struct	occi_response * yptr;

	cool_log_message("start_elastic_contract",1);

	if (!( yptr = cords_invoke_action( eptr->contract, _CORDS_START, _CORDS_CONTRACT_AGENT, default_tls() )))
		return(( eptr->isactive = 0 ));
	else if ( cords_check_invocation( yptr, (struct rest_response *) 0 ) != 0 )
	{
		yptr = occi_remove_response( yptr );
		return(( eptr->isactive = 0 ));
	}
	else 
	{
		yptr = occi_remove_response( yptr );
		if (!( yptr = occi_simple_get( eptr->contract, _CORDS_CONTRACT_AGENT, default_tls() )))
			return(( eptr->isactive = 0 ));
		else 
		{
			cool_retrieve_durations( eptr, yptr );
			yptr = occi_remove_response( yptr );
			return(( eptr->isactive = 1 ));
		}
	}
}

/*	---------------------------------------------	*/
/*	o c c i _ e l a s t i c _ c o n d i t i o n s	*/
/*	---------------------------------------------	*/
private	struct elastic_contract * occi_elastic_conditions(
	struct elastic_contract * eptr,
	char * sla,
	struct cords_placement_criteria * selector,
	struct cords_guarantee_criteria * warranty )
{
	struct	occi_response * zptr;
	if (!( rest_valid_string( sla ) ))
		return( eptr );
	else if (!( zptr = occi_simple_get( sla, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( liberate_elastic_contract( eptr ) );
	else if (!( cords_retrieve_conditions( sla, sla, zptr, selector, warranty, _CORDS_CONTRACT_AGENT, default_tls() ) ))
	{
		zptr = occi_remove_response( zptr );
		return( eptr );
	}
	else
	{
		zptr = occi_remove_response( zptr );
		return( liberate_elastic_contract( eptr ) );
	}
}

/*	---------------------------------------------	*/
/*	  n e w _ e l a s t i c _ c o n t r a c t	*/
/*	---------------------------------------------	*/
/*	creates a new elastic contract by duplicating	*/
/*	an existing contract and provision instance.	*/
/*	---------------------------------------------	*/
private	struct elastic_contract * new_elastic_contract( struct elastic_contract * eptr, char * contract )
{
	struct	occi_response * zptr=(struct occi_response *) 0;
	char *	result=(char *) 0;
	char *	econtract=(char *) 0;
	char *	eprovision=(char *) 0;
	int	status;
	struct	cords_placement_criteria selector;
	struct	cords_guarantee_criteria warranty;

	memset( &warranty, 0, sizeof( struct cords_guarantee_criteria ));
	memset( &selector, 0, sizeof( struct cords_placement_criteria ));

	cool_log_message("new_elastic_contract",1);

	/* ------------------------------ */
	/* retrieve the CONTRACT instance */
	/* ------------------------------ */
	if (!( contract ))
		return( liberate_elastic_contract( eptr ) );
	else if (!( eptr->zptr = occi_simple_get( contract , _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( liberate_elastic_contract( eptr ) );

	/* ------------------------- */
	/* retrieve the PROFILE name */
	/* ------------------------- */
	else if (!( result = occi_extract_atribut( 
					eptr->zptr, Cool.domain, 
					_CORDS_CONTRACT, _CORDS_PROFILE )))
		return( liberate_elastic_contract( eptr ) );
	else if (!( eptr->profile = allocate_string( result ) ))
		return( liberate_elastic_contract( eptr ) );

	/* ----------------------- */
	/* retrieve the SERVICE ID */
	/* ----------------------- */
	if (( result = occi_extract_atribut( 
					eptr->zptr, Cool.domain, 
					_CORDS_CONTRACT, _CORDS_PARENTSERVICE )) != (char *) 0)
	{
		if (!( eptr->parentservice = allocate_string( result )))
			return( liberate_elastic_contract( eptr ) );
		else	warranty.service = eptr->parentservice;
	}

	/* ----------------------- */
	/* retrieve the AGREEMENT  */
	/* ----------------------- */
	if (( result = occi_extract_atribut( 
					eptr->zptr, Cool.domain, 
					_CORDS_CONTRACT, _CORDS_AGREEMENT )) != (char *) 0)
		if (!( eptr->agreement = allocate_string( result )))
			return( liberate_elastic_contract( eptr ) );

	/* ---------------------------- */
	/* retrieve the NODE identifier */
	/* ---------------------------- */
	if (!( result = occi_extract_atribut( 
					eptr->zptr, Cool.domain, 
					_CORDS_CONTRACT, _CORDS_NODE )))
		return( liberate_elastic_contract( eptr ) );
	else if (!( eptr->node = allocate_string( result ) ))
		return( liberate_elastic_contract( eptr ) );

	/* ------------------------------- */
	/* extract the PROVIDER identifier */
	/* ------------------------------- */
	else if (!( result = occi_extract_atribut( 
					eptr->zptr, Cool.domain, 
					_CORDS_CONTRACT, _CORDS_PROVIDER )))
		return( liberate_elastic_contract( eptr ) );
	else if (!( eptr->provision = allocate_string( result ) ))
		return( liberate_elastic_contract( eptr ) );

	/* ------------------------------- */
	/* retrieve the PROVISION instance */
	/* ------------------------------- */
	else if (!( eptr->yptr = occi_simple_get( eptr->provision, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( liberate_elastic_contract( eptr ) );

	/* ------------------------------ */
	/* extract the ACCOUNT identifier */
	/* ------------------------------ */
	else if (!( result = occi_extract_atribut( 
					eptr->yptr, Cool.domain, 
					eptr->profile, _CORDS_ACCOUNT )))
		return( liberate_elastic_contract( eptr ) );
	else if (!( eptr->account = allocate_string( result ) ))
		return( liberate_elastic_contract( eptr ) );

	else if (!( eptr->wptr = occi_simple_get( eptr->account, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( liberate_elastic_contract( eptr ) );
	else if (!( eptr->accountname = occi_extract_atribut( eptr->wptr, Cool.domain, _CORDS_ACCOUNT, _CORDS_NAME ))) 
		return( liberate_elastic_contract( eptr ) );

	/* ------------------------- */
	/* extract the CONTRACT name */
	/* ------------------------- */
	else if (!( result = occi_extract_atribut( 
					eptr->yptr, Cool.domain, 
					eptr->profile, _CORDS_NAME )))
		return( liberate_elastic_contract( eptr ) );

	else if (!( eptr->name = allocate_string( result ) ))
		return( liberate_elastic_contract( eptr ) );

	else if (!( eptr = occi_elastic_conditions( eptr, eptr->agreement, &selector, &warranty ) ))
		return( liberate_elastic_contract( eptr ) );

	else if (!( econtract = negotiate_elastic_contract( 
			eptr->node, Elastic.contractname, eptr->accountname, &selector, &warranty, eptr->agreement ) ))
		return( liberate_elastic_contract( eptr ) );

	else if (!( eptr->xptr = occi_simple_get( econtract , _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( liberate_elastic_contract( eptr ) );

	else if (( status = cool_duplicate_contract( econtract, contract, eprovision )) != 0)
		return( liberate_elastic_contract( eptr ) );
	else
	{
		/* --------------------------------- */
		/* clean up the temporary points now */
		/* --------------------------------- */
		if ( eptr->zptr ) eptr->zptr = occi_remove_response( eptr->zptr );
		if ( eptr->yptr ) eptr->yptr = occi_remove_response( eptr->yptr );
		if ( eptr->xptr ) eptr->xptr = occi_remove_response( eptr->xptr );
		if ( eptr->wptr ) eptr->wptr = occi_remove_response( eptr->wptr );

		/* ----------------------------- */
		/* store contract name and start */
		/* ----------------------------- */
		if (!( eptr->contract = allocate_string( econtract ) ))
			return( liberate_elastic_contract( eptr ) );
		else if (!( start_elastic_contract( eptr ) ))
			return( liberate_elastic_contract( eptr ) );

		/* -------------------------------- */
		/* add to list of elastic contracts */
		/* -------------------------------- */
		else	return( use_elastic_contract( eptr, econtract ) );	
		
	}
}

/*	--------------------------------------------	*/
/*	  a d d _ e l a s t i c _ c o n t r a c t	*/
/*	--------------------------------------------	*/
private	struct elastic_contract * add_elastic_contract( char * contract, int allocate )
{
	struct	elastic_contract * eptr;
	char	buffer[245];
	int	status;

	sprintf(buffer,"add_elastic_contract(%u)",allocate);
	cool_log_message(buffer,1);
	cool_log_message(contract,1);

	if ( Elastic.occi )
	{
		if ((status = cool_create_workload( contract, allocate )) != 0)
			return((struct elastic_contract *) 0);
	}

	if (!( eptr = allocate_elastic_contract() ))
		return( eptr );
	else if ((eptr->allocated = allocate) & 1)
		return( new_elastic_contract( eptr, contract ) );
	else	return( use_elastic_contract( eptr, contract ) );
}


/*	----------------------------------------------------	*/
/*	 s c a l e d o w n _ e l a s t i c _ c o n t r a c t	*/
/*	----------------------------------------------------	*/
private	struct elastic_contract * scaledown_elastic_contract( struct elastic_contract * contract )
{
	struct	occi_response * yptr;
	char	buffer[245];

	sprintf(buffer,"scaledown_elastic_contract()");
	cool_log_message(buffer,1);

	if ( Elastic.active > Elastic.floor )
	{
		while ( contract )
			if (!( contract->isactive ))
				contract = contract->previous;
		if (!( contract ))
			return( contract );
		else 	
		{
			contract->isactive = 0;
			if (( yptr = cords_invoke_action( contract->contract, _CORDS_STOP, _CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
				yptr = occi_remove_response( yptr );
			return( contract );
		}
	}
	else	return((struct elastic_contract*) 0);
}

/*	-------------------------------------------------	*/
/*	 s c a l e u p _ e l a s t i c _ c o n t r a c t	*/
/*	-------------------------------------------------	*/
private	struct elastic_contract * scaleup_elastic_contract( char * contract, int allocate )
{
	struct 	elastic_contract * eptr;
	struct	occi_response * yptr;
	char	buffer[245];

	sprintf(buffer,"scaleup_elastic_contract(%u)",allocate);
	cool_log_message(buffer,1);

	for (	eptr = Elastic.first;
		eptr != (struct elastic_contract *) 0;
		eptr = eptr->next )
	{
		if ( eptr->isactive )
			continue;
		else if (!( start_elastic_contract( eptr ) ))
			continue;
		else	return( eptr );
	}
	return( add_elastic_contract( contract, allocate ) );
}

/*	---------------------------------------------------	*/
/*	r e t r i e v e _ e l a s t i c _ c o n t r a c t s	*/
/*	---------------------------------------------------	*/
/*	this is used to retrieve the collection of contract	*/
/*	category instances that were appended to the parent	*/
/*	service during an eventual preceding operation.		*/
/*	---------------------------------------------------	*/
private	int	retrieve_elastic_contracts()
{
	char	*	id;
	struct elastic_contract * cptr;
	struct	occi_response * zptr;
	struct	occi_response * yptr;
	struct	occi_element  * dptr;
	struct	occi_element  * eptr;

	cool_log_message( "retrieve elastic contracts", 0 );

	if (!( Elastic.parentservice ))
		return( 0 );
	else if (!( zptr = occi_simple_get( Elastic.parentservice, _CORDS_CONTRACT_AGENT, default_tls() )))
		return( 0 );
	else
	{
		for (	dptr = cords_first_link( zptr );
			dptr != (struct occi_element *) 0;
			dptr = cords_next_link( dptr ) )
		{
			if ((!( dptr->name ))
			||  (!( dptr->value)))
				continue;
			else if (!( id =  occi_unquoted_link( dptr->value ) ))
				continue;
			/* ---------------------------------- */
			/* filter out the one we already have */
			/* ---------------------------------- */
			else if (!( strcmp( id, Elastic.contract ) ))
			{
				id = liberate( id );
				continue;
			}
			else if (!( yptr = occi_simple_get( id, _CORDS_CONTRACT_AGENT, default_tls() )))
			{
				id = liberate( id );
				continue;
			}
			/* -------------------------------- */
			/* locate identical named contracts */
			/* -------------------------------- */
			else if ((!( eptr = occi_locate_element( yptr->first, "occi.contract.name" ) ))
			     ||  (!( rest_valid_string( eptr->value ) ))
			     ||  ( strcmp( eptr->value, Elastic.contractname ) != 0 ))
			{
				id = liberate( id );
				yptr = occi_remove_response( yptr );
				continue;
			}
			else
			{
				if ((cptr = add_elastic_contract( id, 2 )) != (struct elastic_contract *) 0)
				{
					if ((!( eptr = occi_locate_element( yptr->first, "occi.contract.state" ) ))
					||  (!( rest_valid_string( eptr->value ) ))
					||  ( atoi( eptr->value ) != 0 ))
					{
						id = liberate( id );
						yptr = occi_remove_response( yptr );
						cptr->isactive = 1;
						continue;
					}
					else if ( Elastic.active >= Elastic.floor )
					{
						id = liberate( id );
						yptr = occi_remove_response( yptr );
						cptr->isactive = 0;
						continue;
					}
					else
					{
						start_elastic_contract( cptr );
						yptr = occi_remove_response( yptr );
						id = liberate( id );
						continue;
					}
				}
			}
		}
	}
	zptr = occi_remove_response( zptr );
	return( 1 );
}

/*	---------------------------------------------------------	*/
/*		l b _ u p d a t e _ s t a t i s t i c s 			*/
/*	---------------------------------------------------------	*/
private	void	lb_update_statistics()
{	
	int		now=time((long *) 0);
	if ( Elastic.lasthit == now )
		Elastic.hitcount++;
	else
	{
		Elastic.lasthit=now;
		Elastic.hitcount=1;
	}
	if ( Elastic.hitcount > Elastic.maxhit )
		Elastic.maxhit = Elastic.hitcount;

	if ( Elastic.hitcount > Elastic.upper )
		if ( Elastic.active < Elastic.ceiling )
			scaleup_elastic_contract( Elastic.first->contract, 1 );
	return;
}

/*	---------------------------------------------------------	*/
/*		c o o l _ c o n t r a c t _ h o s t n a m e		*/
/*	---------------------------------------------------------	*/
/*	this function corrects a missing hostname value if it is	*/
/*	possible to do so.						*/
/*	---------------------------------------------------------	*/
private	struct elastic_contract * cool_contract_hostname( struct elastic_contract * eptr )
{
	struct	occi_response * zptr;
	struct	occi_element * dptr;
	char *	vptr;
	if (!( eptr ))
		return( eptr );
	else if ( rest_valid_string( eptr->hostname ) )
		return( eptr );
	else if (!( eptr->contract ))
		return((struct elastic_contract *) 0);
	else if (!( zptr = occi_simple_get( eptr->contract, _CORDS_CONTRACT_AGENT, default_tls() )))
		return((struct elastic_contract *) 0);
	else if (!( dptr = occi_locate_element( zptr->first, "occi.contract.hostname" ) ))
		return((struct elastic_contract *) 0);
	else if (!( vptr = allocate_string( dptr->value ) ))
		return((struct elastic_contract *) 0);
	else if (!( vptr = occi_unquoted_value( vptr ) ))
		return((struct elastic_contract *) 0);
	else if (!( eptr->hostname = allocate_string( vptr ) ))
		return((struct elastic_contract *) 0);
	else	return( eptr );
}

/*	---------------------------------------------------------	*/
/*			l b _ r e d i r e c t					*/
/*	---------------------------------------------------------	*/
private	struct rest_response * lb_redirect( struct rest_client * cptr, struct rest_request * rptr )
{
	struct	rest_header	* hptr;
	struct	rest_response * aptr;
	char		buffer[2048];
	struct	elastic_contract * eptr;

	lb_update_statistics();

	if (!( eptr = cool_next_elastic_contract() )) 
		return( lb_failure(cptr,  500, "Server Failure : No Host" ) );

	else if (!( rest_valid_string( eptr->hostname ) ))
		if (!( eptr = cool_contract_hostname( eptr ) ))
			return( lb_failure(cptr,  500, "Server Failure : Bad Host" ) );

	if (!( aptr = rest_allocate_response(cptr)))
		return( lb_failure(cptr,  500, "Server Failure : Out of Memory" ) );
	else
	{
		sprintf(buffer,"%s://%s:%u",eptr->service,eptr->hostname,eptr->port);

		cool_log_message( "redirection",1);
		cool_log_message( buffer, 1 );

		if (!( hptr = rest_response_header( aptr, _HTTP_LOCATION, buffer )))
		{
			aptr = rest_liberate_response( aptr );
			return( lb_failure(cptr,  500, "Server Failure : Out of Memory" ) );
		}
		else	return( rest_html_response( aptr, _HTTP_MOVED, "Moved" ) );
	}
}

/*	---------------------------------------------------------	*/
/*			l b _ g e t					*/
/*	---------------------------------------------------------	*/
private	struct rest_response * lb_get( 
		void * vptr,
		struct rest_client * cptr, 
		struct rest_request * rptr )
{
	return( lb_redirect(cptr, rptr ) );
}

/*	---------------------------------------------------------	*/
/*			l b _ p u t 					*/
/*	---------------------------------------------------------	*/
private	struct rest_response * lb_put( 
		void * vptr,
		struct rest_client * cptr, 
		struct rest_request * rptr )
{
	return( lb_redirect(cptr, rptr ) );
}

/*	---------------------------------------------------------	*/
/*			l b _ p o s t 					*/
/*	---------------------------------------------------------	*/
private	struct rest_response * lb_post( 
		void * vptr,
		struct rest_client * cptr, 
		struct rest_request * rptr )
{
	return( lb_redirect(cptr, rptr ) );
}

/*	---------------------------------------------------------	*/
/*			l b _ d e l e t e 				*/
/*	---------------------------------------------------------	*/
private	struct rest_response * lb_delete( 
		void * vptr,
		struct rest_client * cptr, 
		struct rest_request * rptr )
{
	return( lb_redirect(cptr, rptr ) );
}

/*	---------------------------------------------------------	*/
/*			l b _ h e a d 					*/
/*	---------------------------------------------------------	*/
private	struct rest_response * lb_head( 
		void * vptr,
		struct rest_client * cptr, 
		struct rest_request * rptr )
{
	return( lb_redirect(cptr, rptr ) );
}

/*	---------------------------------------------------------	*/
/* 				c o o l _ o c c i _ o p e r a t i o n			*/
/*	---------------------------------------------------------	*/
private	int	cool_occi_operation( char * nptr )
{

	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name(_COOL_LINKS);


	/* -------------------------------------- */
	/* add the job and workload control stuff */
	/* -------------------------------------- */
	if (!( optr = occi_cords_job_builder( Cool.domain, "job" )))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_add_action( optr,_COOL_SCALEUP,"",scaleup_job)))
		return( 27 );
	else if (!( optr = occi_add_action( optr,_COOL_SCALEDOWN,"",scaledown_job)))
		return( 27 );

	if (!( optr = occi_cords_workload_builder( Cool.domain, "workload" )))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	/* -------------------------------------- */
	/* add the monitoring connection category */
	/* -------------------------------------- */
	if (!( optr = comons_connection_builder( Cool.domain ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	/* ---------------------------------- */
	/* add the monitoring packet category */
	/* ---------------------------------- */
	if (!( optr = comons_packet_builder( Cool.domain, _COOL_PACKETS ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	rest_initialise_log(Cool.monitor);

	if (!( Cool.identity ))
		return( occi_server(  nptr, Cool.restport, Cool.tls, Cool.threads, first,(char *) 0 ) );
	else
	{
		initialise_occi_publisher( Cool.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			Cool.user, Cool.password,
			Cool.identity,  nptr, 
			Cool.restport, Cool.tls, 
			Cool.threads, first ) );
	}
}

/*	---------------------------------------------------------	*/
/* 				c o o l _ o c c i _ m a n a g e r 				*/
/*	---------------------------------------------------------	*/
private	void * 	cool_occi_manager( void * vptr )
{
	struct rest_thread * tptr=vptr;
	rest_log_message("thread: cool pthread detach");
	pthread_detach( tptr->id );

	(void) cool_occi_operation( "coolocci/v1.0" );

	tptr->status = 0;
	rest_log_message("thread: cool pthread exit");
	pthread_exit((void *) 0);
}

/*	---------------------------------------------------------	*/
/*			c o o l _ e x i t 				*/
/*	---------------------------------------------------------	*/
private	int	cool_exit( int error, struct rest_thread * tptr )
{
	if ( tptr )
	{
		while( tptr->status )
			sleep(5);
	}
	return( error );
}

/*	---------------------------------------------------------	*/
/*			l o a d _ b a l a n c e r			*/
/*	---------------------------------------------------------	*/
private	int	load_balancer( char * nptr )
{
	int	status=0;
	struct	rest_interface  Osi = 
	{
		(void *) 0,
		(void *) 0,
		(void *) 0,
		lb_get,
		lb_post,
		lb_put,
		lb_delete,
		lb_head,
		(void *) 0,
		(void *) 0,
		(void *) 0,
		(void *) 0,
		(void *) 0,
		(void *) 0
	};

	/* -------------------------------------------- */
	/* ensure TLS is correct either NULL or Valid   */
	/* -------------------------------------------- */
	if (!( rest_valid_string( Cool.tls ) ))
		Cool.tls = (char *) 0;

	/* -------------------------------------------- */
	/* ensure TLS is correct either NULL or Valid   */
	/* -------------------------------------------- */
	if (!( rest_valid_string( Elastic.security ) ))
		Elastic.security = (char *) 0;

	/* -------------------------------------------- */
	/* the ceiling must be higher or equal to floor */
	/* -------------------------------------------- */
	if ( Elastic.floor > Elastic.ceiling )
		Elastic.floor = Elastic.ceiling;

	/* ------------------------------------------- */
	/* ensure valid contract identifier available  */
	/* ------------------------------------------- */
	if ((!( Elastic.first ))
	||  (!( Elastic.first->contract )))
		return( 118 );

	/* ------------------------------------------- */
	/* raise the contract count to reach the floor */
	/* ------------------------------------------- */
	while ( Elastic.active < Elastic.floor )
		if (!( scaleup_elastic_contract( Elastic.first->contract, 1 ) ))
			return( 127 );


	Osi.authorise = (void *) 0;

	/* ------------------------------------------ */
	/* this parameter now controls thread workers */
	/* ------------------------------------------ */
	rest_thread_control(0);

	/* --------------------------------- */
	/* launch the REST HTTP Server layer */
	/* --------------------------------- */
	cool_log_message( "cool rest server starting", 0 );
	status = rest_server(  "cool" , Elastic.port, Elastic.security, 0, &Osi );
	cool_log_message( "cool rest server shutdown", 0 );
	return( status );
}

/*	-------------------------------------------	*/
/*		c o o l _ t e s t _ o c c i		*/
/*	-------------------------------------------	*/
private	int	cool_test_occi( char * url )
{
	struct	rest_response * rptr;

	cool_log_message("cool test occi server",0);

	if (!( rptr = rest_client_try_get_request( url, default_tls(), _CORDS_CONTRACT_AGENT, (struct rest_header *) 0, 5, 12 ) ))
	{
		cool_log_message("occi server failure to respond",0);
		return(0);
	}
	else	
	{
		cool_log_message("cool occi server ok",0);
		rptr = liberate_rest_response( rptr );
		return(1);
	}
}

/*	---------------------------------------------	*/
/*	c o o l _ t r a n s p o s e _ i d e n t i t y
/*	---------------------------------------------	*/
private	int	cool_transpose_identity()
{
	char *	ihost=(char *) 0;
	struct	url *	uptr;
	/* ----------------------------------------- */
	/* transpose the port of the occi server url */
	/* ----------------------------------------- */
	if (!( uptr = analyse_url( Cool.identity )))
		return( 30 );
	else
	{
		uptr->port = Cool.restport;
		if (!( ihost = serialise_url( uptr,"" )))
		{
			uptr = liberate_url( uptr );
			return( 31 );
		}
		else
		{
			if ( Cool.identity )
				Cool.identity = liberate( Cool.identity );
			Cool.identity = ihost;
			uptr = liberate_url( uptr );
			return(0);
		}
	}
}


/*	-------------------------------------------	*/
/*		c o o l _ c r e a t e _ j o b 		*/
/*	-------------------------------------------	*/
private	int	cool_create_job( char * contract, char * nptr )
{
	char	buffer[2048];
	struct	occi_element * eptr = (struct	occi_element *) 0;
	struct	occi_element * root = (struct	occi_element *) 0;
	struct	occi_element * foot = (struct	occi_element *) 0;
	struct	occi_response * zptr;
	char	value[64];
	char *	ihost;
	cool_log_message("cool_create_job",0);
	cool_log_message( contract,0);

	sprintf(buffer,"%s/job/",Cool.identity);

	/* ---------------------------------------- */
	/* wait for the occi server thread to start */
	/* ---------------------------------------- */
	if (!( cool_test_occi( buffer ) ))
		return( 132 );

	cool_log_message( "identity job category",0);
	cool_log_message( buffer,0);

	/* ----------------------------------------- */
	/* create the occi job category instance now */
	/* ----------------------------------------- */
	if (!( eptr = occi_create_element( "occi.job.name", nptr ) ))
		return( 27 );
	else if (!( eptr->previous = foot))
		root = eptr;
	else	foot->next = eptr;
	foot = eptr;

	if (!( eptr = occi_create_element( "occi.job.description", "load balanced job" ) ))
		return( 27 );
	else if (!( eptr->previous = foot))
		root = eptr;
	else	foot->next = eptr;
	foot = eptr;

	if (!( eptr = occi_create_element( "occi.job.contract", contract )))
		return( 27 );
	else if (!( eptr->previous = foot))
		root = eptr;
	else	foot->next = eptr;
	foot = eptr;

	sprintf(value,"%u",Elastic.floor);
	
	if (!( eptr = occi_create_element( "occi.job.floor", value ) ))
		return( 27 );
	else if (!( eptr->previous = foot))
		root = eptr;
	else	foot->next = eptr;
	foot = eptr;

	sprintf(value,"%u",Elastic.ceiling);
	
	if (!( eptr = occi_create_element( "occi.job.ceiling", value ) ))
		return( 27 );
	else if (!( eptr->previous = foot))
		root = eptr;
	else	foot->next = eptr;
	foot = eptr;

	sprintf(value,"%u",Elastic.strategy);
	
	if (!( eptr = occi_create_element( "occi.job.strategy", value ) ))
		return( 27 );
	else if (!( eptr->previous = foot))
		root = eptr;
	else	foot->next = eptr;
	foot = eptr;

	if (!( zptr = occi_simple_post( buffer, root, _CORDS_CONTRACT_AGENT, default_tls() )))
	{
		cool_log_message( "job post failure",0);
		return( 118 );
	}
	else if (!( ihost = occi_extract_location( zptr ) ))
	{
		zptr = occi_remove_response( zptr );
		return( 30 );
	}
	else if (!( Elastic.job = allocate_string( ihost ) ))
	{
		zptr = occi_remove_response( zptr );
		return( 27 );
	}
	else
	{
		cool_log_message( "elastic_job",0);
		cool_log_message( Elastic.job,0);
		zptr = occi_remove_response( zptr );
		return( 0 );
	}
}
/*	-------------------------------------------	*/
/*	  c o o l _ c r e a t e _ w o r k l o a d  	*/
/*	-------------------------------------------	*/
private	int	cool_create_workload( char * contract, int type )
{
	char	buffer[2048];
	struct	occi_element * eptr = (struct	occi_element *) 0;
	struct	occi_element * root = (struct	occi_element *) 0;
	struct	occi_element * foot = (struct	occi_element *) 0;
	struct	occi_response * zptr;
	struct	occi_response * yptr;
	char *	ihost=(char *) 0;
	char	value[64];
	int	now;
	struct	url *	uptr;

	cool_log_message( "cool_create_workload",0);
	cool_log_message( contract,0);

	sprintf(buffer,"%s/workload/",Cool.identity);

	cool_log_message( "identity workload category",0);
	cool_log_message( buffer,0);

	if (!( eptr = occi_create_element( "occi.workload.name", "workload" ) ))
		return( 27 );
	else if (!( eptr->previous = foot))
		root = eptr;
	else	foot->next = eptr;
	foot = eptr;

	if (!( eptr = occi_create_element( "occi.workload.description", "load balanced workload" ) ))
		return( 27 );
	else if (!( eptr->previous = foot))
		root = eptr;
	else	foot->next = eptr;
	foot = eptr;

	if (!( eptr = occi_create_element( "occi.workload.contract", contract )))
		return( 27 );
	else if (!( eptr->previous = foot))
		root = eptr;
	else	foot->next = eptr;
	foot = eptr;

	now = time((long *) 0);
	sprintf(value,"%u",now);

	if (!( eptr = occi_create_element( "occi.workload.timestamp", value )))
		return( 27 );
	else if (!( eptr->previous = foot))
		root = eptr;
	else	foot->next = eptr;
	foot = eptr;

	sprintf(value,"%u",type);

	if (!( eptr = occi_create_element( "occi.workload.nature", value )))
		return( 27 );
	else if (!( eptr->previous = foot))
		root = eptr;
	else	foot->next = eptr;
	foot = eptr;

	if (!( zptr = occi_simple_post( buffer, root, _CORDS_CONTRACT_AGENT, default_tls() )))
	{
		cool_log_message( "workload post failure",0);
		return( 118 );
	}
	else if (!( ihost = occi_extract_location( zptr ) ))
	{
		zptr = occi_remove_response( zptr );
		return( 30 );
	}
	else
	{
		cool_log_message( "elastic workload",0);
		cool_log_message( ihost,0);

		/* ---------------------------------- */
		/* link the workload to thze job list */
		/* ---------------------------------- */
		if ((yptr = occi_create_link( Elastic.job, ihost, _CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
			yptr = occi_remove_response( yptr );
		zptr = occi_remove_response( zptr );
		return( 0 );
	}
}

/*	--------------------------------------------	*/
/*		c o o l _ o p e r a t i o n 		*/
/*	--------------------------------------------	*/
/*	This function performs the main operation of	*/
/*	the compatible one http load balancer module	*/
/*	- platform access protol AAA			*/
/*	- OCCI server thread startup			*/
/*	- elastic configuration collection		*/
/*	- load balancer startup				*/
/*	--------------------------------------------	*/
private	int	cool_operation( char * nptr )
{
	struct	elastic_contract * ecptr;
	char *	eptr;
	int	status;
	char *	tls;
	struct	tls_configuration * tlsconf=(struct tls_configuration *) 0;
	struct	rest_thread * tptr=(struct rest_thread *) 0;

	memset( &Elastic.lock,0,sizeof( Elastic.lock));
	set_default_agent( nptr );
	rest_initialise_log(Cool.monitor);

	/* --------------------------------------------- */
	/* initialise the resolver and publisher default */
	/* --------------------------------------------- */
	initialise_occi_resolver( _DEFAULT_PUBLISHER, (char *) 0, (char *) 0, (char *) 0 );

	if ((status = occi_publisher_default()) != 0 )
		return( status );

	/* ------------------------------ */
	/* Detect need for OCCI Interface */
	/* ------------------------------ */
	if (( eptr = getenv( "elastic_occi" )) != (char *) 0)
		Elastic.occi = atoi( eptr );

	/* ----------------------------------------------- */
	/* set up the two ports and transpose the identity */
	/* ----------------------------------------------- */
	Elastic.port = Cool.restport;
	Cool.restport = _COOL_PORT;

	if ((status = cool_transpose_identity()) != 0)
		return(status);

	/* -------------------------------------------- */
	/* handle transport layer security, if required */
	/* -------------------------------------------- */
	if (!( rest_valid_string(Cool.tls) ))
		Cool.tls = (char *) 0;
	else if (!( tlsconf = tls_configuration_load( Cool.tls ) ))
		return( 40 );
	else if ( tlsconf->authenticate )
	{
		if ((Cool.user) && (Cool.password))
		{
			if ((status = occi_secure_AAA( Cool.user, Cool.password, nptr, Cool.tls )) != 0)
				return( status );
			else	cool_log_message( "authentication", 0 );
		}
	}
	activate_event_manager();

	/* ----------------------------------------------------	*/	
	/* Authentication and Authorization has been performed 	*/
	/* so now the two servers can be brought up to handle  	*/
	/* the elastic management and the OCCI interfaces	*/
	/* ----------------------------------------------------	*/	
	cool_log_message( "operation", 0 );

	if ( Elastic.occi )
	{
		/* ---------------------- */
		/* Launch the OCCI Thread */
		/* ---------------------- */
		if (!( tptr = allocate_rest_thread() ))
			return( 37 );
		else
		{
			tptr->status = 1;

			pthread_attr_init( &tptr->attributes);
		}
		/* --------------------- */
		/* launch the new thread */
		/* --------------------- */
		if ((status = pthread_create(
				&tptr->id,
				&tptr->attributes,
				cool_occi_manager,	
				(void *) tptr)) > 0 )
			return( 38 );
		else	sleep(2);
	}

	/* -------------------------------- */
	/* launch the elastic load balencer */
	/* -------------------------------- */

	/* ------------------------------ */
	/* analyse the elasticity options */
	/* ------------------------------ */
	if (!( eptr = getenv( "elastic_security" ) ))
		Elastic.security = (char *) 0;
	else if (!( Elastic.security = allocate_string( eptr ) ))
		return( 27 );

	if (!( eptr = getenv( "elastic_floor" ) ))
		Elastic.floor = 1;
	else	Elastic.floor = atoi(eptr);

	if (!( eptr = getenv( "elastic_ceiling" ) ))
		Elastic.ceiling = 1;
	else	Elastic.ceiling = atoi(eptr);

	if (!( eptr = getenv( "elastic_upper" ) ))
		Elastic.upper = 10;
	else	Elastic.upper = atoi(eptr);

	if (!( eptr = getenv( "elastic_lower" ) ))
		Elastic.lower = 2;
	else	Elastic.lower = atoi(eptr);

	if (!( eptr = getenv( "elastic_strategy" ) ))
		Elastic.strategy = 0;
	else	Elastic.strategy = atoi(eptr);

	if (!( eptr = getenv( "elastic_unit" ) ))
		Elastic.unit = 10;
	else	Elastic.unit = atoi(eptr);

	if (!( eptr = getenv( "elastic_period" ) ))
		Elastic.period = 60;
	else	Elastic.period = atoi(eptr);

	if (!( eptr = getenv( "elastic_contract" ) ))
		return( cool_exit( 118, tptr ) );

	if ( Elastic.occi )
	{
		if ((status = cool_create_job( eptr, "environment" )) != 0)
			return( cool_exit( 118, tptr ) );
	}

	if (!( add_elastic_contract( eptr, 0 ) ))
		return( cool_exit( 27, tptr ) );

	else if (!( retrieve_elastic_contracts() ))
		return( cool_exit( 27, tptr ) );

	/* ----------------------------- */
	/* put the load balancer online  */
	/* ----------------------------- */
	rest_initialise_log( Cool.monitor );

	cool_log_message( "load balancer starting",1 );
	status = load_balancer( nptr );
	cool_log_message( "load balancer shutdown",1 );

	cool_exit( status, tptr );

	/* ----------------------------- */
	/* release the elastic contracts */
	/* ----------------------------- */
	while ((ecptr = Elastic.first) != (struct elastic_contract *) 0)
	{
		ecptr = liberate_elastic_contract( ecptr );
	}

	/* -------------------------------------------- */
	/* terminate security session, if one is active */
	/* -------------------------------------------- */
	if (( Cool.tls ) && ( tlsconf ))
	{
		occi_release_AAA( Cool.user, Cool.password, nptr, Cool.tls );
		if ( tlsconf )
			tlsconf = release_tls_configuration(tlsconf );
	}

	return( status );
}

/*	------------------------------------------- 	*/
/*	c o o l _ o p t i o n s			*/
/*	------------------------------------------- 	*/
/*	Command line option analysis			*/
/*	------------------------------------------- 	*/
private	int	cool_options(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	cool_configuration();
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
				Cool.verbose=1;
				continue;
			case	'd'	:
				Cool.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = cool_operation(aptr) ))
			continue;
		else	break;
	}
	return(status);
}

/*	--------------------------------------------	*/
/*	m a i n 					*/
/*	--------------------------------------------	*/
/*	Entry point from the command line		*/
/*	--------------------------------------------	*/
public	int	main(int argc, char * argv[] )
{
	if ( argc == 1 )
		return( cool_banner() );
	else	return( cool_options( argc, argv ) );
}

	/* ---------- */
#endif 	/* _cool_c */
	/* ---------- */


