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
#ifndef	_cosacs_c
#define	_cosacs_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"
#include "cordslang.h"

#define	_COSACS_START 		"cosacs:start"
#define	_COSACS_SHITDOWN	"cosacs:shutdown"
#define	_COSACS_LOCAL		"127.0.0.1"

private	struct	accords_configuration Cosacs = {
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
	"cosacs.xml",
	"europe",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

private	char * CosacsContract=(char *) 0;

public	int	check_debug()		{	return(Cosacs.debug);		}
public	int	check_verbose()		{	return(Cosacs.verbose);		}
public	char *	default_publisher()	{	return(Cosacs.publisher);	}
public	char *	default_operator()	{	return(Cosacs.operator);	}
public	char *	default_tls()		{	return(Cosacs.tls);		}
public	char *	default_zone()		{	return(Cosacs.zone);		}

/*	---------------------------------------------	*/  
/*			f a i l u r e			*/
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
/*	 	   c o s a c s _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads cosacs  configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	cosacs_load()
{
	load_accords_configuration( &Cosacs, "cosacs" );
	return;
}

/*	---------------------------------------------	*/  
/*		c o s a c s _ b a n n e r		*/
/*	---------------------------------------------	*/  
private	int	cosacs_banner()
{
	printf("\n   CompatibleOne Software Appliance Configuration Services : Version 1.0a.0.08");
	printf("\n   Beta Version : 01/08/2012");
	printf("\n   Copyright (c) 2012 Iain James Marshall, Prologue");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);
}

/*	------------------------------------------------------------------	*/
/*			c o s a c s _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * cosacs_initialise(  void * v,struct rest_server * sptr )
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
/*			c o s a c s _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	cosacs_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, Cosacs.user ) )
		return(0);
	else if ( strcmp( password, Cosacs.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			c o s a c s _ e x t e n s i o n 			*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * cosacs_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	------------------------------------------------------------------	*/
/*			c o s a c s _ f i l e _ e n c o d e			*/
/*	------------------------------------------------------------------	*/
private	char *	cosacs_file_encode( char * filename )
{
	FILE * h;
	int	bytes;
	char	buffer[8193];
	if (!( h = fopen( filename, "r" )))
		return((char *) 0);
	else if (!( bytes = fread(buffer,1,8192,h) ))
	{
		fclose(h);
		return((char *) 0);
	}
	else	
	{
		fclose(h);
		buffer[bytes] = 0;
		return( rest_encode_html( buffer ) );
	}
}

/*	---------------------------------------------------	*/
/*		c o s a c s _ p o s t _ s a m p l e s		*/
/*	---------------------------------------------------	*/
private	void	cosacs_post_samples( struct cords_probe * pptr, int samples, char * filename )
{
	struct	url *	uptr;
	struct	occi_element * dptr;
	struct	occi_client * kptr;
	char *	host;
	char *	ihost;
	char *	vptr;
	char	now[64];
	char	buffer[1024];
	struct	occi_response * yptr;
	struct	occi_response * zptr;
	struct	occi_request  * qptr;

	/* ------------------------------------- */
	/* prepare the date stamp and packet URL */
	/* ------------------------------------- */
	sprintf(now,"%u",time((long*) 0));
	sprintf(buffer,"/%s/",_CORDS_PACKET);
	if (!( pptr->connection ))
		return;
	else if (!( uptr = analyse_url( pptr->connection ) ))
		return;
	else if (!( replace_url_object( uptr, buffer ) ))
		return;
	else if (!( host = serialise_url( uptr, (char *) 0 ) ))
		return;
	else	uptr = liberate_url( uptr );

	/* ------------------------------------- */
	/* prepare the local cosacs identity URL */
	/* ------------------------------------- */
	if (!( rest_valid_string( Cosacs.identity ) ))
	{
		host = liberate( host );
		return;
	}
	else	sprintf(buffer,"%s/%s/%s",Cosacs.identity,_CORDS_PROBE,pptr->id);

	/* ------------------------------------- */
	/* send the data packet  to the consumer */
	/* ------------------------------------- */
	if (!( kptr = occi_create_client( host, _CORDS_CONTRACT_AGENT, default_tls() ) ))
	{
		host = liberate( host );
		return;
	}
	else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
	{
		host = liberate( host );
		kptr = occi_remove_client( kptr );
		return;
	}
	else if ((!(dptr=occi_request_element(qptr,"occi.packet.probe"  	, buffer 	  ) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.packet.connection"   	, pptr->connection) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.packet.metric"  	, pptr->metric    ) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.packet.start"		, now 		  ) ))
	     ||  (!(dptr=occi_request_element(qptr,"occi.packet.finish"		, now 		  ) )))
	{
		host = liberate( host );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return;
	}
	host = liberate( host );
	sprintf(now,"%u",samples);
	if (!(dptr=occi_request_element(qptr,"occi.packet.samples"  	, now )))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return;
	}
	sprintf(now,"%u",++pptr->packets);
	if (!(dptr=occi_request_element(qptr,"occi.packet.sequence"  	, now )))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return;
	}
	else if (!( vptr = cosacs_file_encode( filename )))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return;
	}
	else if (!(dptr=occi_request_element(qptr,"occi.packet.data"  	, vptr )))
	{
		vptr = liberate( vptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return;
	}
	else	vptr = liberate( vptr );

	if (!( yptr = occi_client_post( kptr, qptr ) ))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return;
	}

	/* -------------------------------------- */
	/* recover response packet identifier URL */
	/* -------------------------------------- */
	else if (!( ihost = occi_extract_location( yptr ) ))
	{
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return;
	}

	/* ------------------------------------ */
	/* send a new link to the COSACS master */
	/* ------------------------------------ */
	else if (!( zptr =  occi_create_link( buffer,  ihost, _CORDS_CONTRACT_AGENT, default_tls() ) ))
	{
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return;
	}
	else
	{
		zptr = occi_remove_response( zptr );
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return;
	}
}

/*	------------------------------------------------------------------	*/
/*			c o s a c s _ p r o b e _ w o r k e r			*/
/*	------------------------------------------------------------------	*/
private	void	cosacs_probe_worker( struct cords_probe * pptr )
{
	int	sample=0;
	char 	filename[1024];
	char 	buffer[2048];
	sprintf(filename,"rest/%s.probe",pptr->id);
	unlink( filename );
	while (1)
	{
		if ( rest_valid_string( pptr->expression) )
			sprintf(buffer,"%s >> %s",pptr->expression, filename);
		else	sprintf(buffer,"date >> %s",filename);
		system( buffer );
		if ( ++sample >= pptr->samples )
		{
			cosacs_post_samples( pptr, sample,filename);
			sample=0;
			unlink( filename );
		}
		if ( pptr->period )
		{
			sleep(pptr->period);
		}
	}
}

/*	----------------------------------------------------------	*/
/*		c o s a c s _ v a l i d a t e _ p r o b e		*/
/*	----------------------------------------------------------	*/
private	int cosacs_validate_probe( struct cords_probe * pptr )
{
	struct	occi_response * zptr;
	struct	occi_element  * eptr;

	if (!( pptr ))
		return( 0 );
	else if (!( pptr->metric ))
		return( 0 );
	else if (!( zptr = occi_simple_get( pptr->metric, _CORDS_CONTRACT_AGENT, default_tls() )))
		return( 0 );
	else if (!(eptr = occi_locate_element( zptr->first, "occi.metric.expression" )))
	{
		zptr = occi_remove_response( zptr );
		return( 0 );
	}
	else if (!( pptr->expression = allocate_string( eptr->value ) ))
	{
		zptr = occi_remove_response( zptr );
		return( 0 );
	}
	else if (!(eptr = occi_locate_element( zptr->first, "occi.metric.period" )))
	{
		zptr = occi_remove_response( zptr );
		return( 0 );
	}
	else if (!( pptr->period = atoi( eptr->value ) ))
	{
		zptr = occi_remove_response( zptr );
		return( 0 );
	}
	else if (!(eptr = occi_locate_element( zptr->first, "occi.metric.samples" )))
	{
		zptr = occi_remove_response( zptr );
		return( 0 );
	}
	else if (!( pptr->samples = atoi( eptr->value ) ))
	{
		zptr = occi_remove_response( zptr );
		return( 0 );
	}
	else
	{
		zptr = occi_remove_response( zptr );
		return( 1 );
	}
}

/*	-------------------------------------------	*/
/* 		s t a r t _ p r o b e			*/
/*	-------------------------------------------	*/
private	struct rest_response * start_probe(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	int	pid;
	struct	cords_probe * pptr;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Failure" ) );
	else if ( pptr->state )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if (!( cosacs_validate_probe( pptr ) ))
		return( rest_html_response( aptr, 802, "Probe Validation Failure" ) );
	else
	{
		switch ((pid = fork()))
		{
		case	-1	:
			return( rest_html_response( aptr, 500, "Probe Process Start Failure" ) );
		case	0	:
			cosacs_probe_worker( pptr );
			exit(0);
		default		:
			pptr->pid = pid;
			pptr->state = 1;
			autosave_cords_probe_nodes();
			return( rest_html_response( aptr, 200, "OK" ) );
		}
	}		
}

/*	-------------------------------------------	*/
/* 		s t o p _ p r o b e			*/
/*	-------------------------------------------	*/
private	struct rest_response * stop_probe(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_probe * pptr;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Failure" ) );
	else if (!( pptr->state ))
		return( rest_html_response( aptr, 200, "OK" ) );
	else
	{
		if ( pptr->pid )
		{
			/* remove the worker */
			kill( pptr->pid, 9 );
			pptr->pid = 0;
		}
		pptr->state = 0;
		autosave_cords_probe_nodes();
		return( rest_html_response( aptr, 200, "OK" ) );
	}		
}

/*	-------------------------------------------	*/
/* 	      c o s a c s _ l a u n c h			*/
/*	-------------------------------------------	*/
public	struct	occi_kind_node * occi_first_cords_metadata_node();
public	struct	occi_kind_node * occi_first_cords_script_node();

private	int	cosacs_launch(struct occi_category * optr, struct cords_script * pptr )
{
	struct	cords_metadata * mptr;
	struct	cords_script * sptr;
	struct	occi_kind_node * kptr;
	char *	tempname=(char *) 0;
	FILE *	h=(FILE *) 0;
	int	execmode=0;
	int	metadatas=0;
	int	scripts=0;

	/* ----------------------------------- */
	/* first mark this script item as done */
	/* ----------------------------------- */
	pptr->state = 1;

	/* ------------------------------------ */
	/* generate the list of meta data items */
	/* ------------------------------------ */
	for ( 	kptr=occi_first_cords_metadata_node();
		kptr != (struct occi_kind_node *) 0;
		kptr = kptr->next )
	{
		if (!( mptr = (struct cords_metadata *) kptr->contents ))
			continue;
		else if ( mptr->state )
			continue;
		else if (!( mptr->name ))
			continue;
		else
		{
			/* -------------------------- */
			/* build the script file name */
			/* -------------------------- */
			if (!( h ))
			{
				if (!( tempname = rest_temporary_filename("sh")))
					break;
				else if (!( h = fopen( tempname, "w" ) ))
				{
					printf("error creating file: %s\n",tempname);
					break;			
				}
				else	fprintf(h,"#!/bin/sh\n#\n#cosacs:start\n#%s\n",tempname);
			}		
			/* --------------------------- */
			/* generate the meta data item */
			/* --------------------------- */
			if ( mptr->value )
				fprintf(h,"export %s=%c%s%c\n",mptr->name,0x0022,mptr->value,0x0022);
			else	fprintf(h,"export %s=\n",mptr->name);
			mptr->state = 1;
			metadatas++;
		}
	}
	/* -------------------------------------------- */
	/* save the meta data now that it has been used */
	/* -------------------------------------------- */
	if ( metadatas )
	{
		autosave_cords_metadata_nodes();
		sync();
	}

	/* --------------------------------- */
	/* generate the list of script items */
	/* --------------------------------- */
	for ( 	kptr=occi_first_cords_script_node();
		kptr != (struct occi_kind_node *) 0;
		kptr = kptr->next )
	{
		if (!( sptr = (struct cords_script *) kptr->contents ))
			continue;
		else if ( sptr->state )
			continue;
		else if (!( sptr->syntax ))
			continue;
		else
		{
			/* -------------------------- */
			/* build the script file name */
			/* -------------------------- */
			if (!( h ))
			{
				if (!( tempname = rest_temporary_filename("sh")))
					break;
				else if (!( h = fopen( tempname, "w" ) ))
				{
					printf("error creating file: %s\n",tempname);
					break;			
				}
				else	fprintf(h,"#!/bin/sh\n#\n#cosacs:start\n#%s\n",tempname);
			}		
			/* ------------------------ */
			/* generate the script item */
			/* ------------------------ */
			if (!( sptr->nature ))
				execmode = 0;
			else if (!( strcmp( "run", sptr->nature ) ))
				execmode = 0;
			else if (!( strcmp( "system", sptr->nature ) ))
				execmode = 0;
			else if (!( strcmp( "kill", sptr->nature ) ))
				execmode = 2;
			else if (!( strcmp( "fork", sptr->nature ) ))
				execmode = 1;
			else if (!( strcmp( "process", sptr->nature ) ))
				execmode = 1;
			else	execmode = 0;
			switch( execmode )
			{
			case	1	:
				fprintf(h,"%s&\n",sptr->syntax);
				break;
			case	0	:	
				fprintf(h,"%s\n",sptr->syntax);
				break;
			case	2	:
				fprintf(h,"killall -15 %s\n",sptr->syntax);
				break;
			}
			sptr->state = 1;
			scripts++;
		}
	}
	/* --------------------------------------------- */
	/* save the scripts now that they have been used */
	/* --------------------------------------------- */
	if ( scripts )
	{
		autosave_cords_script_nodes();
		sync();
	}

	/* -------------------------- */
	/* did a script get generated */
	/* -------------------------- */
	if ( h )
	{
		/* -------------------- */
		/* terminate the script */
		/* -------------------- */
		fprintf(h,"#end of file\n");
		fclose(h); h = (FILE *) 0;
		chmod( tempname, 0777 );
		execmode = 0;

		/* -------------------- */
		/* determine the nature */
		/* -------------------- */
		if (!( pptr->nature ))
			execmode = 0;
		else if (!( strcmp( "run", sptr->nature ) ))
			execmode = 0;
		else if (!( strcmp( "system", sptr->nature ) ))
			execmode = 0;
		else if (!( strcmp( "fork", sptr->nature ) ))
			execmode = 1;
		else if (!( strcmp( "process", sptr->nature ) ))
			execmode = 1;
		else	execmode = 0;

		/* ----------------------------- */
		/* launch the script as required */
		/* ----------------------------- */
		switch ( execmode )
		{
		case	0 :
			system( tempname );
			pptr->result = errno;
			break;
		case 	1 :
			if (!( pptr->result = fork() ))
			{
				system( tempname );
				exit(0);
			}
			break;
		}
	}

	/* ---------------------------------- */
	/* liberate the filename if allocated */
	/* ---------------------------------- */
	if ( tempname ) tempname = liberate( tempname );

	return(0);
}

/*	-------------------------------------------	*/
/* 	      c r e a t e _ s c r i p t  		*/
/*	-------------------------------------------	*/
private	int	create_cords_script(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_script * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( pptr->name ))
		return( 0 ); 
	else if (!( strcmp( _COSACS_START, pptr->name ) ))
		return( cosacs_launch( optr, pptr ) );
	else	return( 0 );
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ s c r i p t  		*/
/*	-------------------------------------------	*/
private	int	retrieve_cords_script(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_script * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ s c r i p t 	 	*/
/*	-------------------------------------------	*/
private	int	update_cords_script(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_script * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ s c r i p t  		*/
/*	-------------------------------------------	*/
private	int	delete_cords_script(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_script * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

private	struct	occi_interface	cords_script_interface = {
	create_cords_script,
	retrieve_cords_script,
	update_cords_script,
	delete_cords_script
	};

/*	-------------------------------------------------	*/
/*	     c o s a c s _ u s e _ p u b l i s h e r		*/
/*	-------------------------------------------------	*/
/*	this is necessary when security enforcement is to	*/
/*	be performed by all cooperating modules requiring	*/
/*	the standard authentication, authorization action	*/
/*	to be performed.					*/
/*	-------------------------------------------------	*/
private	int	cosacs_use_publisher()
{
	char *	tls;
	if (!( rest_valid_string( Cosacs.publisher ) ))
		return( 0 );
	else
	{
		initialise_occi_resolver ( Cosacs.publisher, (char *) 0, (char *) 0, (char *) 0 );
		initialise_occi_publisher( Cosacs.publisher, (char *) 0, (char *) 0, (char *) 0 );
	}

	if (!( rest_valid_string((tls = default_tls()) )))
		return( 0 );
	else if (!( rest_valid_string( Cosacs.user ) ))
		return( 0 );
	else if (!( rest_valid_string( Cosacs.password ) ))
		return( 0 );
	else	return( occi_secure_AAA( Cosacs.user, Cosacs.password, _CORDS_CONTRACT_AGENT, tls ) );
}

/*	-------------------------------------------------	*/
/*		i n t e r c e p t _ i d e n t i t y		*/
/*	-------------------------------------------------	*/
private	int	intercept_identity( char * vptr )
{
	char	buffer[1024];

	if (!( strcmp( vptr, _COSACS_LOCAL ) ))
		return( 0 );
	else if ( rest_valid_string( Cosacs.identity ) )
		return(0);
	else
	{
		sprintf(buffer,"http://%s:%u",vptr,Cosacs.restport);
		if (!( Cosacs.identity = allocate_string( buffer ) ))
			return(0); 
		else	return(0);
	}
}

/*	-------------------------------------------------	*/
/*		i n t e r c e p t _ p u b l i s h e r		*/
/*	-------------------------------------------------	*/
private	int	intercept_publisher( char * vptr )
{
	if ( rest_valid_string( Cosacs.publisher ) )
		return(0);
	else if (!( Cosacs.publisher = allocate_string( vptr ) ))
		return(0); 
	else	return( cosacs_use_publisher() );
}

/*	-------------------------------------------------	*/
/*		i n t e r c e p t _ c o n t r a c t		*/
/*	-------------------------------------------------	*/
private	int	intercept_contract( char * vptr )
{
	if ( rest_valid_string( CosacsContract ) )
		return(0);
	else if (!( CosacsContract = allocate_string( vptr ) ))
		return(0); 
	else	return(0);
}

/*	-------------------------------------------------	*/
/*	  i n t e r c e p t _ a u t h o r i z a t i o n		*/
/*	-------------------------------------------------	*/
private	int	intercept_authorization( char * vptr )
{
	return( occi_resolve_authorization( vptr ) );
}

/*	-------------------------------------------------	*/
/*		i n t e r c e p t _ m e t a d a t a		*/
/*	-------------------------------------------------	*/
/*	interception of cosacs specific metadata is to be	*/
/*	performed for the recovery of operational values	*/
/*	that are know only to the contract negotiator		*/
/*	that spawned the hosting virtual machine.		*/
/*	-------------------------------------------------	*/
private	int	intercept_metadata( char * nptr, char * vptr )
{
	/* --------------------------------- */
	/* no name nor value then do nothing */
	/* --------------------------------- */
	if (!( nptr )) 		return(0);
	else if (!( vptr ))	return(0);

	/* ----------------------------------- */
	/* detect the cosacs identity variable */
	/* ----------------------------------- */
	if (!( strcmp( nptr,"cosacs" ) ))
		return( intercept_identity( vptr ) );

	/* ------------------------------------ */
	/* detect the cosacs publisher identity */
	/* ------------------------------------ */
	else if (!( strcmp( nptr,"publisher" ) ))
		return( intercept_publisher( vptr ) );

	/* ------------------------------------------ */
	/* detect the cosacs host contract identifier */
	/* ------------------------------------------ */
	else if (!( strcmp( nptr,"contract" ) ))
		return( intercept_contract( vptr ) );

	/* -------------------------------------------- */
	/* detect the callers authorization declaration */
	/* -------------------------------------------- */
	else if (!( strcmp( nptr,"authorization" ) ))
		return( intercept_authorization( vptr ) );

	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      c r e a t e _ c o n t r a c t  		*/
/*	-------------------------------------------	*/
private	int	create_metadata(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_metadata * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( rest_valid_string( pptr->name ) ))
		return( 0 );
	else if (!( rest_valid_string( pptr->value ) ))
		return( 0 );
	else	return( intercept_metadata( pptr->name, pptr->value ) );
}


/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ c o n t r a c t  		*/
/*	-------------------------------------------	*/
private	int	retrieve_metadata(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_metadata * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ c o n t r a c t  		*/
/*	-------------------------------------------	*/
private	int	update_metadata(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_metadata * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ c o n t r a c t	  	*/
/*	-------------------------------------------	*/
private	int	delete_metadata(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_metadata * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

private	struct	occi_interface	cords_metadata_interface = 
{
	create_metadata,
	retrieve_metadata,
	update_metadata,
	delete_metadata
};

/*	------------------------------------------------------------------	*/
/*			c o s a c s _ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	cosacs_operation( char * nptr )
{
	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_cosacs.xml");

	/* -------------------------------------------------------------------- */
	/* add the metadata category manager for envirnment variable definition */
	/* -------------------------------------------------------------------- */
	if (!( optr = occi_cords_metadata_builder( Cosacs.domain, "metadata" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->access |= ( _OCCI_CONTRACT | _OCCI_NO_PRICING );
	optr->callback  = &cords_metadata_interface;

	if (!( optr = occi_cords_file_builder( Cosacs.domain, "file" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->access |= ( _OCCI_CONTRACT | _OCCI_NO_PRICING );
	optr->callback  = (void *) 0;

	/* ---------------------------------------------------------------- */
	/* add the script category manager for script definition and launch */
	/* ---------------------------------------------------------------- */
	if (!( optr = occi_cords_script_builder( Cosacs.domain, "script" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->access |= ( _OCCI_CONTRACT | _OCCI_NO_PRICING );
	optr->callback  = &cords_script_interface;

	/* ----------------------------------------------------------- */
	/* add the monitoring probe category for monitoring operations */
	/* ----------------------------------------------------------- */
	if (!( optr = occi_cords_probe_builder( Cosacs.domain, "probe" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->access |= ( _OCCI_CONTRACT | _OCCI_NO_PRICING );
	optr->callback  = (void *) 0;

	/* ---------------------------------------- */
	/* addition of the probe management methods */
	/* for data collection and delivery control */
	/* ---------------------------------------- */
	if (!( optr = occi_add_action( optr,"start","",start_probe)))
		return( 28 );

	else if (!( optr = occi_add_action( optr,"stop","",stop_probe)))
		return( 28 );

	/* ------------------------------------------ */
	/* start the COSACS server message processing */
	/* ------------------------------------------ */
	rest_initialise_log(Cosacs.monitor);

	if (!( rest_valid_string( Cosacs.identity ) ))
		return( occi_server(  nptr, Cosacs.restport, Cosacs.tls, Cosacs.threads, first, (char *) 0 ) );
	else
	{
		initialise_occi_publisher( Cosacs.publisher, (char*) 0, (char *) 0, (char *) 0);

		return( publishing_occi_server(
			Cosacs.user, Cosacs.password,
			Cosacs.identity,  nptr, 
			Cosacs.restport, Cosacs.tls, 
			Cosacs.threads, first ) );
	}

}

/*	------------------------------------------------------------------	*/
/*			c o s a c s _ c o m m a n d				*/
/*	------------------------------------------------------------------	*/
private	int	cosacs_command(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	cosacs_load();
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
				Cosacs.verbose=1;
				continue;
			case	'd'	:
				Cosacs.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = cosacs_operation(aptr) ))
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
		return( cosacs_banner() );
	else	return( cosacs_command( argc, argv ) );
}


	/* --------- */
#endif	/* _cosacs_c */
	/* --------- */

