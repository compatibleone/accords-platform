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
	printf("\n   CompatibleOne Open Stack OsOcciProcci : Version 1.0a.0.02");
	printf("\n   Beta Version : 19/04/2012");
	printf("\n   Copyright (c) 2012 Iain James Marshall, Prologue ");
	printf("\n");
	accords_configuration_options();
	printf("\n");
	printf("\nCommand Line Operation:");
	printf("\n   -i --host {host} --tenant {tenant} ... ");
	printf("\n      --user {user} --pass   {pass}   {command line syntax} \n");
	printf("\nCommand Line Syntax:");
	printf("\n   LIST   [ COMPUTE | NETWORK | STORAGE ] ");
	printf("\n   GET    [ COMPUTE | NETWORK | STORAGE ] {id} ");
	printf("\n   DELETE [ COMPUTE | NETWORK | STORAGE ] {id} ");
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
#include "occios.c"

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

		if (!( optr = occi_add_action( optr,_CORDS_START,"",start_occi_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_SAVE,"",save_occi_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_SNAPSHOT,"",snapshot_occi_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_STOP,"",stop_occi_openstack)))
			return( optr );
#ifdef	_OS_OCCI_PROCCI_ACTIONS
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

/*	--------------------------------------------------	*/
/*		o c c i _ o s _ d r a w l i n e			*/
/*	--------------------------------------------------	*/
private	void	occi_drawline()
{
	printf("--------------------------------------------------------------\n");
	return;
}

/*	------------------------------------------------------------------	*/
/*			o s o c c i c o m a n d r e s p o n s e 		*/
/*	------------------------------------------------------------------	*/
private	int	occi_os_command_response( 
			char * command ,char * subject, char * extra,
			struct rest_response * qptr )
{
	int	c;
	struct	rest_header * hptr;
	FILE *	h;
	occi_drawline();
	printf("\nOCCI OS COMMAND: %s %s %s \n", command, subject, (extra ? extra : "\0") );
	if (!( qptr ))
	{
		printf("NO REST RESPONSE !\n");
		occi_drawline();
		return(32);
	}
	else if (!( qptr = process_occi_os_attributes( qptr )))
	{
		printf("BODY PROCESSING FAILURE !\n");
		occi_drawline();
		return(32);
	}
	else
	{
		occi_drawline();
		printf("REST RESPONSE: %u %s\n",qptr->status, qptr->message);
		occi_drawline();
		printf("REST HEADER: \n");
		for (	hptr=qptr->first;
			hptr != (struct rest_header *) 0;
			hptr = hptr->next )
		{
			if ( hptr->name )
			{
				printf("%s:",hptr->name);
				if ( hptr->value )
					printf("%s",hptr->value);
				printf("\n");
			}
		}
		occi_drawline();
		if ( qptr->body )
		{
			printf("REST BODY: %s\n",qptr->body);
			occi_drawline();
			if (( h = fopen( qptr->body, "r")) !=(FILE *) 0)
			{
				while ((c = fgetc(h)) != -1)
				{
					if (!( c ))
						break;
					else 	printf("%c",c);
				}
				fclose(h);
			}
			printf("\n");
			occi_drawline();
		}
		qptr = liberate_rest_response( qptr );
		return( 0 );
	}
}

/*	------------------------------------------------------------------	*/
/*			o s o c c i p r o c c i l a u n c h			*/
/*	------------------------------------------------------------------	*/
private	int	OsOcciProcciLaunch( 
		char * host, char * user, char * pass, char * tenant, 
		char * command, char * subject, char * extra )
{
	struct	rest_response * rptr;
	if (!( host    ))	return(30);
	if (!( user    ))	return(30);
	if (!( pass    ))	return(30);
	if (!( tenant  ))	return(30);
	if (!( command ))	return(30);
	if (!( subject ))	return(30);

	os_occi_initialise_client( user, pass, host, tenant, "osocciprocci/1.0a", (char *) 0);

	if (!( strcasecmp( command, "list" ) ))
	{
		if (!( strcasecmp( subject, "compute" ) ))
		{
			return( occi_os_command_response( command, subject, extra, 
				get_occi_os_compute( 
				occi_os_category_url( "/compute/" ) ) ));
		}

		else if (!( strcasecmp( subject, "network" ) ))
		{
			return( occi_os_command_response( command, subject, extra, 
				get_occi_os_network( 
				occi_os_category_url( "/network/" ) ) ));
		}
		else if (!( strcasecmp( subject, "storage" ) ))
		{
			return( occi_os_command_response( command, subject, extra, 
				get_occi_os_storage( 
				occi_os_category_url( "/storage/" ) ) ));
		}
		else
		{
			printf("\nincorrect subject : %s %s \n",command, subject );
			return( 30 );
		}

	}
	else if (!( strcasecmp( command, "get" ) ))
	{
		if (!( strcasecmp( subject, "compute" ) ))
		{
			return( occi_os_command_response( command, subject, extra, get_occi_os_compute( extra ) ));
		}

		else if (!( strcasecmp( subject, "storage" ) ))
		{
			return( occi_os_command_response( command, subject, extra, get_occi_os_storage( extra ) ));
		}

		else if (!( strcasecmp( subject, "network" ) ))
		{
			return( occi_os_command_response( command, subject, extra, get_occi_os_network( extra ) ));
		}

		else
		{
			printf("\nincorrect subject : %s %s \n",command, subject );
			return( 30 );
		}
	}
	else if (!( strcasecmp( command, "delete" ) ))
	{
		if (!( strcasecmp( subject, "compute" ) ))
		{
			return( occi_os_command_response( command, subject, extra, delete_occi_os_compute( extra ) ));
		}

		else if (!( strcasecmp( subject, "storage" ) ))
		{
			return( occi_os_command_response( command, subject, extra, delete_occi_os_storage( extra ) ));
		}

		else if (!( strcasecmp( subject, "network" ) ))
		{
			return( occi_os_command_response( command, subject, extra, delete_occi_os_network( extra ) ));
		}
		else
		{
			printf("\nincorrect subject : %s %s \n",command, subject );
			return( 30 );
		}
	}
	else
	{
		printf("\nincorrect command : %s \n",command );
		return( 30 );
	}
}

/*	------------------------------------------------------------------	*/
/*			o s o c c i p r o c c i c o m m a n d			*/
/*	------------------------------------------------------------------	*/
private	int	OsOcciProcciCommand( int argi, int argc, char * argv[] )
{
	int	c;
	int	status=0;
	char *	aptr;
	char *	host;
	char *	pass;
	char *	tenant;
	char * 	user;
	char *	command=(char *) 0;
	char *	subject=(char *) 0;
	while ( argi < argc )
	{
		if (!( aptr = argv[++argi] ))
			break;
		else if ( *aptr == '-' )
		{
			aptr++;
			switch((c = *(aptr++)))
			{
			case	'-'	:
				if (!( strcmp( aptr, "host" ) ))
				{
					host = argv[++argi];
					continue;
				}
				else if (!( strcmp( aptr, "user" ) ))
				{
					user = argv[++argi];
					continue;
				}
				else if (!( strcmp( aptr, "pass" ) ))
				{
					pass = argv[++argi];
					continue;
				}
				else if (!( strcmp( aptr, "tenant" ) ))
				{
					tenant = argv[++argi];
					continue;
				}
				else	
				{
					printf("\nIncorrect command line option: --%s \n",aptr );
					return(30);
				}
			default		:
				printf("\nIncorrect command line option: -%c%s \n",c,aptr );
				return(30);
			}
		}
		else if (!( command ))
			command = aptr;
		else 	return( OsOcciProcciLaunch( host, user, pass, tenant, command, aptr, argv[++argi] ) );
	}
	return(0);
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
			case	'i'	:
				return( OsOcciProcciCommand( argi, argc, argv ) );
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
/*					m a i n 				*/
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

