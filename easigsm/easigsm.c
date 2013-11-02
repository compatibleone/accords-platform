#ifndef	_easigsm_c	
#define	_easigsm_c

#define	_DUMMY_ECSLAM 1

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "cspi.h"

/* 	----------------------------------------	*/	
/*	Prototype Contract Negotiation Functions	*/
/* 	----------------------------------------	*/

struct	accords_configuration EasiGsm = {
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
	"europe",
	"easigsm.xml",
	"storage",
	"easigsm",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(EasiGsm.debug);		}
public	int	check_verbose()		{	return(EasiGsm.verbose);		}
public	char *	default_publisher()	{	return(EasiGsm.publisher);	}
public	char *	default_operator()	{	return(EasiGsm.operator);		}
public	char *	default_tls()		{	return(EasiGsm.tls);		}
public	char *	default_zone()		{	return(EasiGsm.zone);		}

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

#include "ecslam_serviceTemplate.c"
#include "ecslam_agreement.c"
#include "ecslam_negotiation.c"

#include "gsm_agreement.c"
#include "gsm_instance.c"

#include "occiecslam_serviceTemplate.c"
#include "occiecslam_agreement.c"
#include "occiecslam_negotiation.c"

#include "occigsm_agreement.c"
#include "occigsm_instance.c"

/*	---------------------------------------------------------------	*/  
/*	e a s i g s m _ c o n f i g u r a t i o n				*/
/*	---------------------------------------------------------------	*/  
/*	this function loads easigsm configuration			*/
/*	from the xml configuration file.				*/
/*	---------------------------------------------------------------	*/  
private	void	easigsm_configuration()
{
	load_accords_configuration( &EasiGsm, "easigsm" );
	return;
}

/*	-----------------------------------------------------	----------	*/  
/*	e a s i g s m _ b a n n e r				*/
/*	---------------------------------------------------------------	*/  
/*	this function will be called to display module identification 	*/
/*	when launched from the command line without any parameters.	*/
/*	---------------------------------------------------------------	*/  
private	int	easigsm_banner()
{
	printf("\n   CompatibleOne Easiclouds Generic Service Manager: Version 1.0a.0.02");
	printf("\n   Beta Version : 02/11/2013 ");
	printf("\n   Copyright (c) 2013 Iain James Marshall ");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*	------------------------------------------- */
/*	e a s i g s m _ i n i t i a l i s e	    */
/*	------------------------------------------- */

private	struct rest_server * easigsm_initialise(  void * v,struct rest_server * sptr )
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
/*	e a s i g s m _ a u t h o r i s e 		*/
/*	------------------------------------------- 	*/
private	int	easigsm_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, EasiGsm.user ) )
		return(0);
	else if ( strcmp( password, EasiGsm.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	-------------------------------------------	*/
/*	e a s i g s m _ e x t e n s i o n 		*/
/*	-------------------------------------------	*/
private	struct rest_extension * easigsm_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	-------------------------------------------	*/
/* 	      g s m _ s t a r t _ s e r v i c e		*/
/*	-------------------------------------------	*/
private	struct	rest_response * gsm_start_instance(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	gsm_instance * pptr;
	int	status;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state != _OCCI_IDLE )
	 	return( rest_html_response( aptr, 200, "OK Started" ) );
	else
	{
		if ( rest_valid_string( pptr->start ) )
		{
			evaluate_category_cordscript( "easigsm", "instance", pptr->id, pptr->start, "" );
			pptr->state = _OCCI_RUNNING;
			autosave_gsm_instance_nodes();
		 	return( rest_html_response( aptr, 200, "OK Started" ) );
		}
		else
		{
			pptr->state = _OCCI_RUNNING;
			autosave_gsm_instance_nodes();
		 	return( rest_html_response( aptr, 200, "OK Started" ) );
		}
	}
}

/*	-------------------------------------------	*/
/* 	   g s m _ s c a l e u p _ s e r v i c e	*/
/*	-------------------------------------------	*/
private	struct	rest_response * gsm_scaleup_instance(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	gsm_instance * pptr;
	int	status;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state != _OCCI_RUNNING )
	 	return( rest_html_response( aptr, 200, "OK Not Started" ) );
	else
	{
		if ( rest_valid_string( pptr->scaleup ) )
		{
			evaluate_category_cordscript( "easigsm", "instance", pptr->id, pptr->scaleup, "" );
		 	return( rest_html_response( aptr, 200, "OK Started" ) );
		}
		else
		{
		 	return( rest_html_response( aptr, 200, "OK Started" ) );
		}
	}
}

/*	-------------------------------------------	*/
/* 	 g s m _ s c a l e d o w n _ s e r v i c e	*/
/*	-------------------------------------------	*/
private	struct	rest_response * gsm_scaledown_instance(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	gsm_instance * pptr;
	int	status;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state != _OCCI_RUNNING )
	 	return( rest_html_response( aptr, 200, "OK Not Started" ) );
	else
	{
		if ( rest_valid_string( pptr->scaledown ) )
		{
			evaluate_category_cordscript( "easigsm", "instance", pptr->id, pptr->scaledown, "" );
		 	return( rest_html_response( aptr, 200, "OK Started" ) );
		}
		else
		{
		 	return( rest_html_response( aptr, 200, "OK Started" ) );
		}
	}
}

/*	-------------------------------------------	*/
/* 	      g s m _ s t o p _ s e r v i c e		*/
/*	-------------------------------------------	*/
private	struct	rest_response * gsm_stop_instance(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	gsm_instance * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state == _OCCI_IDLE )
	 	return( rest_html_response( aptr, 200, "OK Stopped" ) );
	else
	{
		if ( rest_valid_string( pptr->stop ) )
		{
			evaluate_category_cordscript( "easigsm", "instance", pptr->id, pptr->stop, "" );
			pptr->state = _OCCI_RUNNING;
			autosave_gsm_instance_nodes();
		 	return( rest_html_response( aptr, 200, "OK Started" ) );
		}
		else
		{
			pptr->state = _OCCI_IDLE;
			autosave_gsm_instance_nodes();
		 	return( rest_html_response( aptr, 200, "OK Stopped" ) );
		}
	}
}

/*	-------------------------------------------	*/
/* 	  g s m _ v e r i f y _ a g r e e m e n t 	*/
/*	-------------------------------------------	*/
private	struct	rest_response * gsm_verify_agreement(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	gsm_agreement * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state != _OCCI_IDLE )
	 	return( rest_html_response( aptr, 200, "OK Accepted" ) );
	else
	{
		if ( rest_valid_string( pptr->verify ) )
		{
			evaluate_category_cordscript( "easigsm", "agreement", pptr->id, pptr->verify, "" );
			pptr->state = _OCCI_RUNNING;
			autosave_gsm_agreement_nodes();
		 	return( rest_html_response( aptr, 200, "OK Started" ) );
		}
		else
		{
			pptr->state = _OCCI_IDLE;
			autosave_gsm_agreement_nodes();
		 	return( rest_html_response( aptr, 200, "OK Verified" ) );
		}
	}
}

/*	-------------------------------------------	*/
/*		ecslam_negotiate_negotiation		*/
/*	-------------------------------------------	*/
private	struct	rest_response * ecslam_negotiate_negotiation(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	char	buffer[2048];
	struct	occi_element * dptr;
	struct	occi_response * zptr;
	char *	ihost;
	struct	ecslam_negotiation * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 400, "Invalid Action" ) );
	else if ( pptr->state )
	 	return( rest_html_response( aptr, 400, "Cannot Negotiate" ) );
	else
	{
		if (!( ihost = occi_resolve_category_provider( "ecslam_agreement", _CORDS_CONTRACT_AGENT, default_tls() ) ))
			return( rest_html_response( aptr, 500, "ECSLAM Agremeent Missing" ) );
		else	sprintf(buffer,  "%s/%s/",ihost,"ecslam_agreement");

		if (!( dptr = occi_create_element("occi.ecslam_agreement.name",pptr->name) ))
		 	return( rest_html_response( aptr, 500, "Server Failure" ) );
		else if (!( zptr = occi_simple_post( buffer, dptr, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		 	return( rest_html_response( aptr, 500, "Server Failure" ) );
		else if (!( ihost = occi_extract_location( zptr ) ))
		{
			zptr = occi_remove_response( zptr );
		 	return( rest_html_response( aptr, 500, "Server Failure" ) );
		}
		else if (!( pptr->offer = allocate_string( ihost ) ))
		{
			zptr = occi_remove_response( zptr );
		 	return( rest_html_response( aptr, 500, "Server Failure" ) );
		}
		else
		{
			zptr = occi_remove_response( zptr );
			pptr->state = 1;
			autosave_ecslam_negotiation_nodes();
		 	return( rest_html_response( aptr, 200, "OK") );
		}
	}
}

/*	-------------------------------------------	*/
/*		ecslam_agree_negotiation		*/
/*	-------------------------------------------	*/
private	struct	rest_response * ecslam_agree_negotiation(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	ecslam_negotiation * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 400, "Invalid Action" ) );
	else if ( pptr->state != 1 )
	 	return( rest_html_response( aptr, 400, "Not Negotiated" ) );
	else
	{
		pptr->state = 2;
		autosave_ecslam_negotiation_nodes();
	 	return( rest_html_response( aptr, 200, "OK") );
	}
}

/*	-------------------------------------------	*/
/*		ecslam_decline_negotiation		*/
/*	-------------------------------------------	*/
private	struct	rest_response * ecslam_decline_negotiation(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	ecslam_negotiation * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 400, "Invalid Action" ) );
	else if ( pptr->state != 1 )
	 	return( rest_html_response( aptr, 400, "Not Negotiated" ) );
	else
	{
		pptr->state = 4;
		autosave_ecslam_negotiation_nodes();
	 	return( rest_html_response( aptr, 200, "OK") );
	}
}

/*	-------------------------------------------	*/
/* 	  g s m _ a c c e p t _ a g r e e m e n t 	*/
/*	-------------------------------------------	*/
private	struct	rest_response * gsm_accept_agreement(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	gsm_agreement * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state != _OCCI_IDLE )
	 	return( rest_html_response( aptr, 200, "OK Accepted" ) );
	else
	{
		if ( rest_valid_string( pptr->verify ) )
		{
			evaluate_category_cordscript( "easigsm", "agreement", pptr->id, pptr->accept, "" );
			pptr->state = _OCCI_RUNNING;
			autosave_gsm_agreement_nodes();
		 	return( rest_html_response( aptr, 200, "OK Started" ) );
		}
		else
		{
			pptr->state = _OCCI_RUNNING;
			autosave_gsm_agreement_nodes();
		 	return( rest_html_response( aptr, 200, "OK Accepted" ) );
		}
	}
}

/*	-------------------------------------------	*/
/* 	  g s m _ r e f u s e _ a g r e e m e n t 	*/
/*	-------------------------------------------	*/
private	struct	rest_response * gsm_refuse_agreement(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	gsm_agreement * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state != _OCCI_IDLE )
	 	return( rest_html_response( aptr, 200, "OK Accepted" ) );
	else
	{
		if ( rest_valid_string( pptr->refuse ) )
		{
			evaluate_category_cordscript( "easigsm", "agreement", pptr->id, pptr->refuse, "" );
			pptr->state = _OCCI_RUNNING;
			autosave_gsm_agreement_nodes();
		 	return( rest_html_response( aptr, 200, "OK Started" ) );
		}
		else
		{
			pptr->state = _OCCI_RUNNING;
			autosave_gsm_agreement_nodes();
		 	return( rest_html_response( aptr, 200, "OK Accepted" ) );
		}
	}
}

/*	--------------------------------------------	*/
/*	e a s i g s m _ o p e r a t i o n 		*/
/*	--------------------------------------------	*/
/*	environment and category preparation		*/
/*	--------------------------------------------	*/
private	int	easigsm_operation( char * nptr )
{

	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_easigsm.xml");

	rest_initialise_log( EasiGsm.monitor );

	/* ---------------------------------------- */
	/* preparation of application category list */
	/* ---------------------------------------- */

#ifdef	_DUMMY_ECSLAM

	if (!( optr = occi_ecslam_serviceTemplate_builder( EasiGsm.domain, "ecslam_serviceTemplate" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_ecslam_negotiation_builder( EasiGsm.domain, "ecslam_negotiation" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_add_action( optr,"Negotiate","",ecslam_negotiate_negotiation)))
		return( 27 );
	if (!( optr = occi_add_action( optr,"Agree","",ecslam_agree_negotiation)))
		return( 27 );
	if (!( optr = occi_add_action( optr,"Decline","",ecslam_decline_negotiation)))
		return( 27 );

	if (!( optr = occi_ecslam_agreement_builder( EasiGsm.domain, "ecslam_agreement" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

#endif

	if (!( optr = occi_gsm_agreement_builder( EasiGsm.domain, "gsm_agreement" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_add_action( optr,"verify","",gsm_verify_agreement)))
		return( 27 );
	else if (!( optr = occi_add_action( optr,"accept","",gsm_accept_agreement)))
		return( 27 );
	else if (!( optr = occi_add_action( optr,"refuse","",gsm_refuse_agreement)))
		return( 27 );

	if (!( optr = occi_gsm_instance_builder( EasiGsm.domain, "gsm_instance" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_add_action( optr,"start","",gsm_start_instance)))
		return( 27 );
	if (!( optr = occi_add_action( optr,"scaleup","",gsm_scaleup_instance)))
		return( 27 );
	if (!( optr = occi_add_action( optr,"scaledown","",gsm_scaledown_instance)))
		return( 27 );
	else if (!( optr = occi_add_action( optr,"stop","",gsm_stop_instance)))
		return( 27 );

	if (!( EasiGsm.identity ))
	{
		/* --------------------------	*/
		/* launch simple occi server 	*/
		/* --------------------------	*/
		return( occi_server(  
			nptr, 
			EasiGsm.restport, EasiGsm.tls, 
			EasiGsm.threads, first, (char *) 0 ) );
	}
	else
	{
		/* --------------------------------------------------------------- */
		/* publication and pricing of categories before occi server launch */
		/* --------------------------------------------------------------- */
		initialise_occi_publisher( 
			EasiGsm.publisher, (char*) 0, (char *) 0, (char *) 0);

		return( publishing_occi_server(
				EasiGsm.user, EasiGsm.password,
				EasiGsm.identity,  nptr, 
				EasiGsm.restport, EasiGsm.tls, 
				EasiGsm.threads, first ) );
	}
}

/*	------------------------------------------- */
/*	e a s i g s m _ o p t i o n s		    */
/*	------------------------------------------- */
/*	Command line option analysis		    */
/*	------------------------------------------- */
private	int	easigsm_options(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	easigsm_configuration();
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
				EasiGsm.verbose=1;
				continue;
			case	'd'	:
				EasiGsm.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = easigsm_operation(aptr) ))
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
		return( easigsm_banner() );
	else	return( easigsm_options( argc, argv ) );
}

	/* ---------- */
#endif 	/* _easigsm_c */
	/* ---------- */


