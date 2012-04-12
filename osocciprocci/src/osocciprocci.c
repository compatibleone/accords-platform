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
#ifndef	_osocciprocci_c
#define	_osocciprocci_c

#include "standard.h"
#include "procci.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"
#include "cordslang.h"

struct	accords_configuration OsOcciProcci = {
	0,0,
	0,0,0,
	(char *) 0,
	(char *) 0,
	_CORDS_DEFAULT_PUBLISHER,
	_CORDS_DEFAULT_OPERATOR,
	_CORDS_DEFAULT_USER,
	_CORDS_DEFAULT_PASSWORD,
	"http",  80,
	"xmpp",  8000,
	"domain",
	"OsOcciProcci.xml",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(OsOcciProcci.debug);		}
public	int	check_verbose()		{	return(OsOcciProcci.verbose);	}
public	char *	default_publisher()	{	return(OsOcciProcci.publisher);	}
public	char *	default_operator()	{	return(OsOcciProcci.operator);	}
public	char *	default_tls()		{	return(OsOcciProcci.tls);		}

public	int	failure( int e, char * m1, char * m2 )
{
	if ( e )
	{
		printf("\n***(%u) failure %u",getpid(),e);
		if ( m1 )
			printf(" : %s",m1);
		if ( m2 )
			printf(" : %s",m2);
		printf(" **`\n");
	}
	return( e );
}

/*	---------------------------------------------	*/  
/*		 o s o c c i p r o c c i _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads OsOcciProcci    configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	OsOcciProcci_load()
{
	load_accords_configuration( &OsOcciProcci, "osocciprocci" );
	return;
}

private	int	banner()
{
	printf("\n   CompatibleOne Open Stack OsOcciProcci : Version 1.0a.0.01");
	printf("\n   Beta Version : 02/04/2012");
	printf("\n   Copyright (c) 2012 Iain James Marshall, Prologue ");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*	------------------------------------------------------------------	*/
/*			o s o c c i p r o c c i _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * OsOcciProcci_initialise(  void * v,struct rest_server * sptr )
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
/*			o s o c c i p r o c c i _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	OsOcciProcci_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, OsOcciProcci.user ) )
		return(0);
	else if ( strcmp( password, OsOcciProcci.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			o s o c c i p r o c c i _ e x t e n s i o n 			*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * OsOcciProcci_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

#include "osocciclient.c"
#include "osoccicontract.c"

/*	-------------------------------------------	*/
/* 	      c r e a t e _ o p e n s t a c k  		*/
/*	-------------------------------------------	*/
private	int	create_openstack(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	openstack * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( pptr->node ))
		return( 0 ); 
	else	return(create_openstack_contract( optr, pptr, _CORDS_CONTRACT_AGENT, default_tls()));
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ o p e n s t a c k  	*/
/*	-------------------------------------------	*/
private	int	retrieve_openstack(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	openstack * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ o p e n s t a c k 	 	*/
/*	-------------------------------------------	*/
private	int	update_openstack(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	openstack * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ o p e n s t a c k  		*/
/*	-------------------------------------------	*/
private	int	delete_openstack(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	openstack * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(delete_openstack_contract(optr, pptr, _CORDS_CONTRACT_AGENT, default_tls()));
}

private	struct	occi_interface	openstack_interface = {
	create_openstack,
	retrieve_openstack,
	update_openstack,
	delete_openstack
	};


/*	-------------------------------------------	*/
/* 	       b u i l d _ o p e n s t a c k  		*/
/*	-------------------------------------------	*/
/*	this function is to be called to build the	*/
/*	complete openstack occi category to offer	*/
/*	an openstack compute provisioning service	*/
/*	through a compatible one published occi		*/
/*	service interface.				*/
/*	-------------------------------------------	*/
private	struct	occi_category * build_occi_openstack( char * domain )
{
	struct	occi_category * optr;
	if (!( optr = occi_openstack_builder( domain, "occiopenstack" ) ))
		return( optr );
	else
	{
		optr->access |= _OCCI_PROVIDER;
		optr->callback  = &openstack_interface;

#ifdef	_OS_OCCI_PROCCI_ACTIONS
		if (!( optr = occi_add_action( optr,_CORDS_START,"",start_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_SAVE,"",save_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_SNAPSHOT,"",snapshot_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_STOP,"",stop_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_SUSPEND,"",suspend_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_RESTART,"",restart_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"softboot","",softboot_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"hardboot","",hardboot_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"rebuild","",rebuild_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"resize","",resize_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"confirm","",confirm_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"revert","",revert_openstack)))
			return( optr );
		else	return( optr );
#else
		return( optr );
#endif
	}
}

/*	---------------------------------------------------------	*/
/* 	b u i l d _ o p e n s t a c k _ c o n f i g u r a t i o n	*/
/*	---------------------------------------------------------	*/
/*	this category handles the configuration of the interface	*/
/*	to the oepn stack server for the preceeding category of		*/
/*	provisioning instance requests.					*/
/*	---------------------------------------------------------	*/
private	struct	occi_category * build_occi_openstack_configuration( char * domain )
{
	struct	occi_category * optr;

	if (!( optr = occi_cords_osconfig_builder( domain, "occi_openstack_configuration" ) ))
		return( optr );
#ifdef	_OS_OCCI_PROCCI_ACTIONS
	else if (!( optr = occi_add_action( optr,"current","",set_default_openstack)))
		return( optr );
#endif
	else	return( optr );

}

/*	------------------------------------------------------------------	*/
/*			o s o c c i p r o c c i _ o p e r a t i o n			*/
/*	------------------------------------------------------------------	*/
private	int	OsOcciProcci_operation( char * nptr )
{

	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_openstack.xml");
	set_autosave_os_config_name("osocci_config.xml");
	set_autosave_openstack_name("osocci_openstack.xml");

	if (!( optr = build_occi_openstack( OsOcciProcci.domain ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;


	if (!( optr = build_occi_openstack_configuration( OsOcciProcci.domain ) ))
		return( 27 );

	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	rest_initialise_log( OsOcciProcci.monitor );

	if (!( OsOcciProcci.identity ))
		return( occi_server(  nptr, OsOcciProcci.restport, OsOcciProcci.tls, OsOcciProcci.threads, first, (char *) 0 ) );
	else
	{
		initialise_occi_publisher( OsOcciProcci.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			OsOcciProcci.user, OsOcciProcci.password,
			OsOcciProcci.identity,  nptr, 
			OsOcciProcci.restport, OsOcciProcci.tls, 
			OsOcciProcci.threads, first ) );
	}
}


/*	------------------------------------------------------------------	*/
/*				o s o c c i p r o c c i m a i n 		*/
/*	------------------------------------------------------------------	*/
private	int	OsOcciProcciMain(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	OsOcciProcci_load();
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
				OsOcciProcci.verbose=1;
				continue;
			case	'd'	:
				OsOcciProcci.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = OsOcciProcci_operation(aptr) ))
			continue;
		else	break;
	}
	return(status);
}

/*	------------------------------------------------------------------	*/
/*					m a i n 							*/
/*	------------------------------------------------------------------	*/
public	int	main(int argc, char * argv[] )
{
	if ( argc == 1 )
		return( banner() );
	else	return( OsOcciProcciMain( argc, argv ) );
}


		/* ----------- */
#endif	/* _OsOcciProcci_c */
		/* ----------- */

