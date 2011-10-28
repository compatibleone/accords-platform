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
#ifndef	_publisher_c
#define	_publisher_c

#include "standard.h"
#include "publication.h"
#include "publisher.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "occibuilder.h"

/*	-------------------------------------	*/
/* 	f u n c t i o n   p r o t o t y p e s	*/
/*	-------------------------------------	*/

private	int	launch_publication( struct publication * pptr, struct rest_client * cptr );

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

struct	publisher_configuration Publisher = {
	0,		/* monitor */
	1,		/* threads */
	0,0,0,0,0,0,
	"admin", "admin",
	"http://127.0.0.1",  80,
	"xmpp.yourinside.com",  8000,
	"/home/xmpp/compatibleone",
	"occi",
	"publisher.xml", "publications.xml",
	(struct publication *) 0,
	(struct publication *) 0,
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
public	char *	default_publisher()	{	return(_CORDS_DEFAULT_PUBLISHER);	}
public	char *	default_tls()		{	return(Publisher.tls);			}

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
	printf("\n   CO-PUB : CompatibleOne Service Publisher : Version 1.0a.0.03");
	printf("\n   Beta Version : 26/10/2011");
	printf("\n   Copyright (c) 2011 Iain James Marshall, Prologue ");
	printf("\n\n");
}

/*	---------------------------------------------	*/  
/*		    	h e l p				*/
/*	---------------------------------------------	*/
/*	displays the collection of command line flags	*/
/*	in response to the --help command		*/   
/*	---------------------------------------------	*/  
private	int	help()
{
	banner();

	printf("\nOptions: ");

	printf("\n   --debug                  toggle debug messages   ");
	printf("\n   --verbose                toggle info  messages   ");
	printf("\n   --authorise              toggle authorisation    ");
	printf("\n   --autosave               toggle autosave flag    ");
	printf("\n   --help                   this help screen        ");

	printf("\n   --threads  <1|0>         toggle use of threads   ");
	printf("\n   --tls      <name>        specify tls configuration ");
	printf("\n   --resthost <name>        specify rest server host name or address");
	printf("\n   --restport <number>      specify rest server port");

	printf("\n   --chathost <name>        specify chat server host name or address");
	printf("\n   --chatagent <name>       specify chat agent program name  ");
	printf("\n   --chatport <number>      specify chat server port");

	printf("\n   --user     <name>        specify user log name   ");
	printf("\n   --password <value>       specify user password   ");
	printf("\n   --domain   <value>       specify publisher domain");
	printf("\n   --memory   <value>       specify memory filename ");
	printf("\n   --config   <value>       specify configuration   ");
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
	struct	xml_element * document;
	struct	xml_element * eptr;
	struct	xml_element * vptr;
	struct	xml_atribut * aptr;
	set_xml_echo(Publisher.debug);
	if (( document = document_parse_file( Publisher.config )) != (struct xml_element *) 0)
	{
		if ( Publisher.verbose )
			printf("   Publisher loading configuration from : %s \n",Publisher.config);

		if (( eptr = document_element( document, "publisher" )) != (struct xml_element *) 0)
		{
			if (( vptr = document_element( eptr, "rest" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "host" )) != (struct xml_atribut *) 0)
					Publisher.resthost = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "port" )) != (struct xml_atribut *) 0)
					Publisher.restport = document_atribut_value( aptr );
				if ((aptr = document_atribut( vptr, "threads" )) != (struct xml_atribut *) 0)
					Publisher.threads = document_atribut_value( aptr );
			}
			if (( vptr = document_element( eptr, "xmpp" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "host" )) != (struct xml_atribut *) 0)
					Publisher.chathost = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "port" )) != (struct xml_atribut *) 0)
					Publisher.chatport = document_atribut_value( aptr );
				if ((aptr = document_atribut( vptr, "agent" )) != (struct xml_atribut *) 0)
					Publisher.chatagent = document_atribut_string( aptr );
			}
			if (( vptr = document_element( eptr, "security" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "user" )) != (struct xml_atribut *) 0)
					Publisher.user = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "pass" )) != (struct xml_atribut *) 0)
					Publisher.password = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "tls" )) != (struct xml_atribut *) 0)
					Publisher.tls = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "authorise" )) != (struct xml_atribut *) 0)
					Publisher.authorise = document_atribut_value( aptr );
				if ((aptr = document_atribut( vptr, "monitor")) != (struct xml_atribut *) 0)
					Publisher.monitor = document_atribut_value( aptr );
			}
			if (( vptr = document_element( eptr, "memory" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "file" )) != (struct xml_atribut *) 0)
					Publisher.memory = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "autosave" )) != (struct xml_atribut *) 0)
					Publisher.autosave = document_atribut_value( aptr );
			}
			if (( vptr = document_element( eptr, "domain" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "name" )) != (struct xml_atribut *) 0)
					Publisher.domain = document_atribut_string( aptr );
			}
		}
		document = document_drop( document );
	}
	else if ( Publisher.verbose )
		printf("   Publisher failed to load configuration from : %s \n",Publisher.config);
	set_xml_echo(0);
	return;
}


/*	---------------------------------------------	*/  
/*		p u b l i s h e r _ s a v e		*/
/*	---------------------------------------------	*/
/*	save publisher configuration information to 	*/
/*	the XML configuration file.			*/  
/*	---------------------------------------------	*/  
private	void	publisher_save()
{
	FILE	*	h;
	if (( h = fopen( Publisher.config, "w" )) != (FILE *) 0)
	{
		if ( Publisher.verbose )
			printf("   Publisher saving configuration to : %s \n",Publisher.config);

		fprintf(h,"<?xml version='1.0' encoding='utf-8'?>\n");
		fprintf(h,"<publisher>\n");
		fprintf(h,"\t<rest host='%s' port='%u' threads='%u'/>\n",
			Publisher.resthost,Publisher.restport,Publisher.threads);
		fprintf(h,"\t<xmpp host='%s' port='%u' agent='%s' />\n",
			Publisher.chathost,Publisher.chatport,Publisher.chatagent);
		fprintf(h,"\t<security admin='%s' pass='%s' tls='%s' authorise='%u'/>\n",
			Publisher.user,Publisher.password,(Publisher.tls?Publisher.tls:""),Publisher.authorise);
		fprintf(h,"\t<memory file='%s' autosave='%u'/>\n",Publisher.memory,Publisher.autosave);
		fprintf(h,"\t<domain name='%s'/>\n",Publisher.domain);
		fprintf(h,"</publisher>\n");

		fclose(h);
	}
	else if ( Publisher.verbose )
		printf("   Publisher failed to save configuration to : %s \n",Publisher.config);
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
	struct	publication * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		pptr->when = 0;
		pptr->state = _OCCI_SUSPENDED;
		autosave_publication_nodes("publication.xml");
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
	struct	publication * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		pptr->uptime = 0;
		pptr->when = time((long*) 0);
		pptr->state = _OCCI_RUNNING;
		autosave_publication_nodes("publication.xml");
	  	return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	    c r e a t e _ p u b l i c a t i o n 	*/
/*	-------------------------------------------	*/
private	int	create_publication(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	publication * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else
	{
		pptr->when  = time((long *) 0);
		pptr->uptime = 0;
		pptr->state  = _OCCI_RUNNING;
		return(0);
	}
}

/*	-------------------------------------------	*/
/* 	  r e t r i e v e _ p u b l i c a t i o n 	*/
/*	-------------------------------------------	*/
private	int	retrieve_publication(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	publication * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	{
		pptr->uptime = (time((long*) 0) - pptr->when);
		return(0);
	}
}

/*	-------------------------------------------	*/
/* 	    u p d a t e _ p u b l i c a t i o n 	*/
/*	-------------------------------------------	*/
private	int	update_publication(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	publication * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else
	{
		pptr->uptime = (time((long*) 0) - pptr->when);
		return(0);
	}
}

/*	-------------------------------------------	*/
/* 	    d e l e t e _ p u b l i c a t i o n 	*/
/*	-------------------------------------------	*/
private	int	delete_publication(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	publication * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
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
private	int	create_enquiry(struct occi_category * optr, void * vptr)
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
private	int	retrieve_enquiry(struct occi_category * optr, void * vptr)
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
private	int	update_enquiry(struct occi_category * optr, void * vptr)
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
private	int	delete_enquiry(struct occi_category * optr, void * vptr)
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
private	int	create_agency(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	agency * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else
	{
		pptr->start  = time((long *) 0);
		pptr->uptime = 0;
		pptr->state  = 1;
	}
}

/*	-------------------------------------------	*/
/* 	  r e t r i e v e _ a g e n c y 	*/
/*	-------------------------------------------	*/
private	int	retrieve_agency(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	agency * pptr;
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
private	int	update_agency(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	agency * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	    d e l e t e _ a g e n c y 	*/
/*	-------------------------------------------	*/
private	int	delete_agency(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	agency * pptr;
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
	struct	agency * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		pptr->start = 0;
		pptr->state = 0;
		autosave_agency_nodes("agency.xml");
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
	struct	agency * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		pptr->uptime = 0;
		pptr->start = time((long*) 0);
		pptr->state = 1;
		autosave_agency_nodes("agency.xml");
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

	if (!( optr = occi_publication_builder( Publisher.domain,"publication" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = &publication_interface;

	if (!( optr = occi_add_action( optr,"suspend","",suspend_publication)))
		return( 27 );
	else if (!( optr = occi_add_action( optr,"restart","",restart_publication)))
		return( 27 );

	if (!( optr = occi_enquiry_builder( Publisher.domain, "enquiry" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = &enquiry_interface;


	if (!( optr = occi_agency_builder( Publisher.domain, "agency" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = &agency_interface;

	if (!( optr = occi_add_action( optr,"start","",start_agency)))
		return( 27 );
	else if (!( optr = occi_add_action( optr,"stop","",stop_agency)))
		return( 27 );

	rest_initialise_log(0);

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
				if (!( strcmp( aptr, "verbose" ) ))
				{
					Publisher.verbose += 1;
					Publisher.verbose &= 1;
				}
				else if (!( strcmp( aptr, "debug" ) ))
				{
					Publisher.debug += 1;
					Publisher.debug &= 1;
				}
				else if (!( strcmp( aptr, "tls" ) ))
				{
					Publisher.tls = allocate_string( argv[++argi] );
				}
				else if (!( strcmp( aptr, "autosave" ) ))
				{
					Publisher.autosave += 1;
					Publisher.autosave &= 1;
				}
				else if (!( strcmp( aptr, "authorise" ) ))
				{
					Publisher.authorise += 1;
					Publisher.authorise &= 1;
				}
				else if (!( strcmp( aptr, "help" ) ))
					return( help() );
				else if (!( strcmp( aptr, "chatagent" ) ))
					Publisher.chatagent = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "domain" ) ))
					Publisher.domain = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "memory" ) ))
					Publisher.memory = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "config" ) ))
				{
					Publisher.config = allocate_string( argv[++argi] );
					publisher_load();
				}
				else if (!( strcmp( aptr, "user" ) ))
					Publisher.user = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "password" ) ))
					Publisher.password = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "resthost" ) ))
					Publisher.resthost = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "restport" ) ))
					Publisher.restport = atoi( argv[++argi] );
				else if (!( strcmp( aptr, "chathost" ) ))
					Publisher.chathost = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "chatport" ) ))
					Publisher.chatport = atoi( argv[++argi] );
				else if (!( strcmp( aptr, "threads" ) ))
					Publisher.threads = atoi( argv[++argi] );
				else	break;
				continue;
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



