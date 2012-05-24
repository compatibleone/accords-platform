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

#define	_COSACS_START "cosacs:start"

struct	accords_configuration Cosacs = {
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
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(Cosacs.debug);		}
public	int	check_verbose()		{	return(Cosacs.verbose);		}
public	char *	default_publisher()	{	return(Cosacs.publisher);	}
public	char *	default_operator()	{	return(Cosacs.operator);	}
public	char *	default_tls()		{	return(Cosacs.tls);		}

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

private	int	banner()
{
	printf("\n   CompatibleOne Software Appliance Configuration Services : Version 1.0a.0.04");
	printf("\n   Beta Version : 21/03/2012");
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
/* 	  actions and methods required for the cosacs instance category		*/
/*	------------------------------------------------------------------	*/

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
			else if (!( strcmp( "fork", sptr->nature ) ))
				execmode = 1;
			else if (!( strcmp( "process", sptr->nature ) ))
				execmode = 1;
			else	execmode = 0;
			if ( execmode )
				fprintf(h,"%s&\n",sptr->syntax);
			else	fprintf(h,"%s\n",sptr->syntax);
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


/*	------------------------------------------------------------------	*/
/*			c o s a c s _ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	cosacs_operation( char * nptr )
{
	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_cosacs.xml");

	if (!( optr = occi_cords_metadata_builder( Cosacs.domain, "metadata" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if (!( optr = occi_cords_file_builder( Cosacs.domain, "file" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if (!( optr = occi_cords_script_builder( Cosacs.domain, "script" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = &cords_script_interface;

	if (!( optr = occi_cords_probe_builder( Cosacs.domain, "probe" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	rest_initialise_log(Cosacs.monitor);

	return( occi_server(  nptr, Cosacs.restport, Cosacs.tls, Cosacs.threads, first,(char *) 0 ) );

}

/*	------------------------------------------------------------------	*/
/*				c o s a c s 					*/
/*	------------------------------------------------------------------	*/
private	int	cosacs(int argc, char * argv[] )
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
		return( banner() );
	else	return( cosacs( argc, argv ) );
}


	/* --------- */
#endif	/* _cosacs_c */
	/* --------- */

