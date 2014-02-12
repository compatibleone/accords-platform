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
#ifndef	_teston_c
#define	_teston_c

#include "onclient.h"
#include "xxlink.c"

private	int	debug=0;
private	int	verbose=1;
private	char *	tls=(char *) 0;
private	char *	zone="europe";
private	char *	operator="europe";
private	char *	publisher=(char *) 0;

public	int	check_debug()		{	return(debug);		}
public	int	check_verbose()		{	return(verbose);	}
public	char *	default_publisher()	{	return(publisher);	}
public	char *	default_tls()		{	return(tls);		}
public	char *	default_zone()		{	return(zone);		}
public	char *	default_operator()	{	return(operator);	}

/* ------------------------------------------------------------------------------------ */
/*					f a i l u r e					*/
/* ------------------------------------------------------------------------------------ */
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

/* ------------------------------------------------------------------------------------ */
/*					o n _ r e s u l t				*/
/* ------------------------------------------------------------------------------------ */
private	int	on_result( struct on_response * rptr )
{
	char	buffer[2048];
	if ( rptr )
	{
		switch ( rptr->nature )
		{
		case	_TEXT_JSON	:
			if ( rptr->jsonroot )
			{
				json_show( rptr->jsonroot );
				printf("\n");		
			}
			break;
		case	_TEXT_XML	:
			if ( rptr->xmlroot )
			{
				document_show_element( rptr->xmlroot, 0 );
				printf("\n");
			}
			break;
		default			:
			if (!( rptr->response ))
				break;
			else if (!( rptr->response->body ))
				break;
			else if ( rptr->response->status != 204 )
			{
				FILE *fp = fopen(rptr->response->body, "r");
				int sz = 0;

				do {
					sz = fread(buffer, 1, sizeof(buffer), fp);
				} while(sz > 0 && fwrite(buffer, 1, sz, stdout) > 0);
				fclose(fp);
				printf("\n");
			}
		}
		liberate_on_response( rptr );
		return(0);
	}
	else	return( failure(99,"no","result") );
}

/* ------------------------------------------------------------------------------------ */
/*			o n _ c l i e n t _ l i s t _ o b j e c t			*/
/* ------------------------------------------------------------------------------------ */
private	struct	on_response * on_list_object( 
	struct on_subscription * subptr,
		char * keyword, 
		char * tls, 
		char * agent, 
		struct rest_header * hptr )
{
	if (!( keyword ))
		return((struct on_response *) 0);

	else if (!( strcasecmp( keyword, "flavors" ) ))
	{
		printf("\nAccords OpenNebula Flavors:\n");
		printf("\nsmall  : Storage <= 40G,       Memory <= 2G      ");
		printf("\nmedium : Storage >  40G < 80G, Memory >  2G < 4G ");
		printf("\nlarge  : Storage >= 80G,       Memory >= 4G      \n");
		return((struct on_response *) 0);
	}
	else if ((!( strcasecmp( keyword, "compute" ) ))
	     ||  (!( strcasecmp( keyword, "servers" ) )))
	{
		return( on_get_request( subptr, "/compute" ) );
	}

	else if ((!( strcasecmp( keyword, "storage" ) ))
	     ||  (!( strcasecmp( keyword, "images"  ) )))
	{
		return( on_get_request( subptr, "/storage" ) );
	}

	else if (!( strcasecmp( keyword, "network" ) ))
	{
		return( on_get_request( subptr, "/network" ) );
	}
	else	return((struct on_response *) 0);
}

/* ------------------------------------------------------------------------------------ */
/*			o n _ c l i e n t _ p o s t _ o b j e c t			*/
/* ------------------------------------------------------------------------------------ */
private	struct	on_response * on_post_object( 
	struct on_subscription * subptr,
		char * keyword, 
		char * tls, 
		char * agent,
		struct rest_header * hptr,
		char * p3,
		char * p4,
		char * p5,
		char * p6,
		char * p7 )
{
	char *	filename;
	if (!( keyword ))
		return((struct on_response *) 0);
	else if (!( strcasecmp( keyword, "server" ) ))
	{
		if ( p7 )
		{
			if ((!( strcmp( p7, "none" ) ))
			||  (!( strcmp( p7, "any"  ) )))
				p7 = (char *) 0;
		}
		if (!( filename = on_create_server_request(subptr,  p3, p4, p5, p6,(char *) 0, "x86_64", p7 )))
			return((struct on_response *) 0);
		else 	return( on_post_request( subptr, "/compute", filename ) );
	}
	else if (!( strcasecmp( keyword, "compute" ) ))
	{
		if ( p7 )
		{
			if ((!( strcmp( p7, "none" ) ))
			||  (!( strcmp( p7, "any"  ) )))
				p7 = (char *) 0;
		}

		if (!( filename = on_create_compute_request(subptr,  p3, p4, p5, p6,(char *) 0, "x86_64", p7 )))
			return((struct on_response *) 0);
		else	return( on_post_request( subptr, "/compute", filename ) );
	}
	else if (!( strcasecmp( keyword, "storage" ) ))
	{
		if (!( filename = on_create_storage_request(subptr,  p3, p4, "OS", p5 ) ))
			return((struct on_response *) 0);
		else	return( on_post_request( subptr, "/storage", filename ) );
	}
	else if (!( strcasecmp( keyword, "network" ) ))
	{
		if (!( filename = on_create_network_request(subptr,  p3, p4, p5, p6 )))
			return((struct on_response *) 0);
		else	return( on_post_request( subptr, "/network", filename ) );
	}
	else	return((struct on_response *) 0);
}

/* ------------------------------------------------------------------------------------ */
/*			o n _ c l i e n t _ s a v e _ o b j e c t			*/
/* ------------------------------------------------------------------------------------ */
private	struct	on_response * on_save_object( 
	struct on_subscription * subptr,
		char * keyword, 
		char * tls, 
		char * agent,
		struct rest_header * hptr,
		char * p3,
		char * p4,
		char * p5,
		char * p6 )
{
	char *	filename;
	if (!( keyword ))
		return((struct on_response *) 0);
	else if (!( strcasecmp( keyword, "server" ) ))
	{
		if (!( filename = on_create_image_request(subptr,  p3, p4, p5, p6 )))
			return((struct on_response *) 0);
		else 	return( on_create_image(subptr,  p3, filename ) );
	}
	else if (!( strcasecmp( keyword, "compute" ) ))
	{
		if (!( filename = on_create_image_request(subptr,  p3, p4, p5, p6 )))
			return((struct on_response *) 0);
		else 	return( on_create_image(subptr,  p3, filename ) );
	}
	else	return((struct on_response *) 0);
}

/* ------------------------------------------------------------------------------------ */
/*			o n _ c l i e n t _ s t o p _ o b j e c t			*/
/* ------------------------------------------------------------------------------------ */
private	struct	on_response * on_stop_object( 
	struct on_subscription * subptr,
		char * keyword, 
		char * tls, 
		char * agent,
		char * p3,
		struct rest_header * hptr )
{
	char *	filename;
	if (!( keyword ))
		return((struct on_response *) 0);
	else if (!( strcasecmp( keyword, "server" ) ))
	{
		if (!( filename = on_stop_compute_request(subptr,  p3 )))
			return((struct on_response *) 0);
		else 	return( on_stop_compute(subptr,  p3, filename ) );
	}
	else if (!( strcasecmp( keyword, "compute" ) ))
	{
		if (!( filename = on_stop_compute_request(subptr,  p3 )))
			return((struct on_response *) 0);
		else 	return( on_stop_compute(subptr,  p3, filename ) );
	}
	else	return((struct on_response *) 0);
}

/* ------------------------------------------------------------------------------------ */
/*			o n _ c l i e n t _ s h u t d o w n _ o b j e c t		*/
/* ------------------------------------------------------------------------------------ */
private	struct	on_response * on_shutdown_object( 
	struct on_subscription * subptr,
		char * keyword, 
		char * tls, 
		char * agent,
		char * p3,
		struct rest_header * hptr )
{
	char *	filename;
	if (!( keyword ))
		return((struct on_response *) 0);
	else if (!( strcasecmp( keyword, "server" ) ))
	{
		if (!( filename = on_shutdown_compute_request(subptr,  p3 )))
			return((struct on_response *) 0);
		else 	return( on_shutdown_compute(subptr,  p3, filename ) );
	}
	else if (!( strcasecmp( keyword, "compute" ) ))
	{
		if (!( filename = on_shutdown_compute_request(subptr,  p3 )))
			return((struct on_response *) 0);
		else 	return( on_shutdown_compute(subptr,  p3, filename ) );
	}
	else	return((struct on_response *) 0);
}

/* ------------------------------------------------------------------------------------ */
/*			o n _ c l i e n t _ p u b l i c _ o b j e c t			*/
/* ------------------------------------------------------------------------------------ */
private	struct	on_response * on_public_object( 
	struct on_subscription * subptr,
		char * keyword, 
		char * tls, 
		char * agent,
		char * p3,
		struct rest_header * hptr )
{
	char *	filename;
	if (!( keyword ))
		return((struct on_response *) 0);
	else if (!( strcasecmp( keyword, "storage" ) ))
	{
		if (!( filename = on_public_image_request(subptr, p3 )))
			return((struct on_response *) 0);
		else 	return( on_public_image( subptr, p3, filename ) );
	}
	else if (!( strcasecmp( keyword, "image" ) ))
	{
		if (!( filename = on_public_image_request( subptr, p3 )))
			return((struct on_response *) 0);
		else 	return( on_public_image(subptr,  p3, filename ) );
	}
	else	return((struct on_response *) 0);
}

/* ------------------------------------------------------------------------------------ */
/*		o n _ c l i e n t _ p e r s i s t e n t _ o b j e c t			*/
/* ------------------------------------------------------------------------------------ */
private	struct	on_response * on_persistent_object( 
	struct on_subscription * subptr,
		char * keyword, 
		char * tls, 
		char * agent,
		char * p3,
		struct rest_header * hptr )
{
	char *	filename;
	if (!( keyword ))
		return((struct on_response *) 0);
	else if (!( strcasecmp( keyword, "storage" ) ))
	{
		if (!( filename = on_persistent_image_request(subptr,  p3 )))
			return((struct on_response *) 0);
		else 	return( on_persistent_image(subptr,  p3, filename ) );
	}
	else if (!( strcasecmp( keyword, "image" ) ))
	{
		if (!( filename = on_persistent_image_request(subptr,  p3 )))
			return((struct on_response *) 0);
		else 	return( on_persistent_image(subptr,  p3, filename ) );
	}
	else	return((struct on_response *) 0);
}

/* ------------------------------------------------------------------------------------ */
/*		o n _ c l i e n t _ v o l a t i l e _ o b j e c t			*/
/* ------------------------------------------------------------------------------------ */
private	struct	on_response * on_volatile_object( 
	struct on_subscription * subptr,
		char * keyword, 
		char * tls, 
		char * agent,
		char * p3,
		struct rest_header * hptr )
{
	char *	filename;
	if (!( keyword ))
		return((struct on_response *) 0);
	else if (!( strcasecmp( keyword, "storage" ) ))
	{
		if (!( filename = on_volatile_image_request(subptr,  p3 )))
			return((struct on_response *) 0);
		else 	return( on_volatile_image(subptr,  p3, filename ) );
	}
	else if (!( strcasecmp( keyword, "image" ) ))
	{
		if (!( filename = on_volatile_image_request(subptr,  p3 )))
			return((struct on_response *) 0);
		else 	return( on_volatile_image(subptr,  p3, filename ) );
	}
	else	return((struct on_response *) 0);
}

/* ------------------------------------------------------------------------------------ */
/*			o n _ c l i e n t _ p u b l i c _ o b j e c t			*/
/* ------------------------------------------------------------------------------------ */
private	struct	on_response * on_rename_object( 
	struct on_subscription * subptr,
		char * keyword, 
		char * tls, 
		char * agent,
		char * p3,
		char * p4 )
{
	char *	filename;
	if (!( keyword ))
		return((struct on_response *) 0);
	else if (!( strcasecmp( keyword, "storage" ) ))
	{
		if (!( filename = on_rename_image_request( subptr, p3, p4 )))
			return((struct on_response *) 0);
		else 	return( on_rename_image(subptr,  p3, filename ) );
	}
	else if (!( strcasecmp( keyword, "image" ) ))
	{
		if (!( filename = on_rename_image_request(subptr,  p3, p4 )))
			return((struct on_response *) 0);
		else 	return( on_rename_image(subptr,  p3, filename ) );
	}
	else	return((struct on_response *) 0);
}

/* ------------------------------------------------------------------------------------ */
/*			o n _ c l i e n t _ p r i v a t e _ o b j e c t			*/
/* ------------------------------------------------------------------------------------ */
private	struct	on_response * on_private_object( 
	struct on_subscription * subptr,
		char * keyword, 
		char * tls, 
		char * agent,
		char * p3,
		struct rest_header * hptr )
{
	char *	filename;
	if (!( keyword ))
		return((struct on_response *) 0);
	else if (!( strcasecmp( keyword, "storage" ) ))
	{
		if (!( filename = on_private_image_request(subptr,  p3 )))
			return((struct on_response *) 0);
		else 	return( on_private_image( subptr, p3, filename ) );
	}
	else if (!( strcasecmp( keyword, "image" ) ))
	{
		if (!( filename = on_private_image_request( subptr, p3 )))
			return((struct on_response *) 0);
		else 	return( on_private_image(subptr,  p3, filename ) );
	}
	else	return((struct on_response *) 0);
}

/* ------------------------------------------------------------------------------------ */
/*			o n _ c l i e n t _ s t a r t _ o b j e c t			*/
/* ------------------------------------------------------------------------------------ */
private	struct	on_response * on_start_object( 
	struct on_subscription * subptr,
		char * keyword, 
		char * tls, 
		char * agent,
		char * p3,
		struct rest_header * hptr )
{
	char *	filename;
	if (!( keyword ))
		return((struct on_response *) 0);
	else if (!( strcasecmp( keyword, "server" ) ))
	{
		if (!( filename = on_start_compute_request(subptr, p3 )))
			return((struct on_response *) 0);
		else 	return( on_start_compute( subptr,p3, filename ) );
	}
	else if (!( strcasecmp( keyword, "compute" ) ))
	{
		if (!( filename = on_start_compute_request( subptr,p3 )))
			return((struct on_response *) 0);
		else 	return( on_start_compute(subptr, p3, filename ) );
	}
	else	return((struct on_response *) 0);
}

/* ------------------------------------------------------------------------------------ */
/*			o n _ c l i e n t _ g e t _ o b j e c t				*/
/* ------------------------------------------------------------------------------------ */
private	struct	on_response * on_get_object( 
	struct on_subscription * subptr,
		char * keyword, 
		char * tls, 
		char * agent, 
		char * parameter, 
		struct rest_header * hptr )
{
	char buffer[2048];
	if (!( keyword ))
		return((struct on_response *) 0);
	else if (!( strcasecmp( keyword, "server" ) ))
	{
		sprintf(buffer,"%s/%s","/compute",parameter );
		return( on_get_request( subptr, buffer ) );
	}
	else if (!( strcasecmp( keyword, "compute" ) ))
	{
		sprintf(buffer,"%s/%s","/compute",parameter );
		return( on_get_request( subptr, buffer ) );
	}
	else if (!( strcasecmp( keyword, "storage" ) ))
	{
		sprintf(buffer,"%s/%s","/storage",parameter );
		return( on_get_request( subptr, buffer ) );
	}
	else if (!( strcasecmp( keyword, "network" ) ))
	{
		sprintf(buffer,"%s/%s","/network",parameter );
		return( on_get_request( subptr, buffer ) );
	}
	else	return((struct on_response *) 0);
}

/* ------------------------------------------------------------------------------------ */
/*			o n _ c l i e n t _ p u t _ o b j e c t				*/
/* ------------------------------------------------------------------------------------ */
private	struct	on_response * on_put_object( 
	struct on_subscription * subptr,
		char * keyword, 
		char * tls, 
		char * agent, 
		char * parameter, 
		char * filename,
		struct rest_header * hptr )
{
	char buffer[2048];
	if (!( keyword ))
		return((struct on_response *) 0);
	else if (!( strcasecmp( keyword, "server" ) ))
	{
		sprintf(buffer,"%s/%s","/compute",parameter );
		return( on_put_request( subptr, buffer, filename ) );
	}
	else if (!( strcasecmp( keyword, "compute" ) ))
	{
		sprintf(buffer,"%s/%s","/compute",parameter );
		return( on_put_request( subptr, buffer, filename ) );
	}
	else if (!( strcasecmp( keyword, "storage" ) ))
	{
		sprintf(buffer,"%s/%s","/storage",parameter );
		return( on_put_request( subptr, buffer, filename ) );
	}
	else if (!( strcasecmp( keyword, "network" ) ))
	{
		sprintf(buffer,"%s/%s","/network",parameter );
		return( on_put_request( subptr, buffer, filename ) );
	}
	else	return((struct on_response *) 0);
}

/* ------------------------------------------------------------------------------------ */
/*			o n _ c l i e n t _ d e l e t e _ o b j e c t			*/
/* ------------------------------------------------------------------------------------ */
private	struct	on_response * on_delete_object( 
	struct on_subscription * subptr,
		char * keyword, 
		char * tls, 
		char * agent, 
		char * parameter, 
		struct rest_header * hptr )
{
	char buffer[2048];
	if (!( keyword ))
		return((struct on_response *) 0);
	else if (!( strcasecmp( keyword, "server" ) ))
	{
		sprintf(buffer,"%s/%s","/compute",parameter );
		return( on_delete_request( subptr, buffer ) );
	}
	else if (!( strcasecmp( keyword, "compute" ) ))
	{
		sprintf(buffer,"%s/%s","/compute",parameter );
		return( on_delete_request( subptr, buffer ) );
	}
	else if (!( strcasecmp( keyword, "storage" ) ))
	{
		sprintf(buffer,"%s/%s","/storage",parameter );
		return( on_delete_request( subptr, buffer ) );
	}
	else if (!( strcasecmp( keyword, "image" ) ))
	{
		sprintf(buffer,"%s/%s","/storage",parameter );
		return( on_delete_request( subptr, buffer ) );
	}
	else if (!( strcasecmp( keyword, "network" ) ))
	{
		sprintf(buffer,"%s/%s","/network",parameter );
		return( on_delete_request( subptr, buffer ) );
	}
	else	return((struct on_response *) 0);
}


/* ------------------------------------------------------------------------------------ */
/*				o n _ o p e r a t i o n					*/
/* ------------------------------------------------------------------------------------ */
private	int	on_operation( 
	struct on_subscription * subptr,
	char * p1, char * p2, char * p3, char * p4, char * p5, char * p6, char * p7 )
{
	struct	rest_header * hptr = (struct rest_header *) 0;
	char	*	agent = "ON-CLIENT/1.0";
	char	*	nomfic;
	if (!( p1))
		return( failure( 30,"p1", "required") );
	else if (!( p2 ))
		return( failure( 30,"p2", "required") );
	else if (!( strcasecmp(p1,"LIST" ) ))
		return( on_result( on_list_object( subptr, p2, default_tls(), agent, hptr ) ) );
	else if (!( p3 ))
		return( failure( 30,"p3", "required") );
	else if (!( strcasecmp(p1,"GET" ) ))
		return( on_result( on_get_object( subptr,p2, default_tls(), agent, p3, hptr ) ) );
	else if (!( strcasecmp(p1,"DELETE" ) ))
		return( on_result( on_delete_object( subptr,p2, default_tls(), agent, p3, hptr ) ) );
	else if (!( strcasecmp(p1,"STOP" ) ))
		return( on_result( on_stop_object( subptr,p2, default_tls(), agent, p3, hptr ) ) );
	else if (!( strcasecmp(p1,"SHUTDOWN" ) ))
		return( on_result( on_shutdown_object( subptr,p2, default_tls(), agent, p3, hptr ) ) );
	else if (!( strcasecmp(p1,"PUBLIC" ) ))
		return( on_result( on_public_object( subptr,p2, default_tls(), agent, p3, hptr ) ) );
	else if (!( strcasecmp(p1,"PRIVATE" ) ))
		return( on_result( on_private_object( subptr,p2, default_tls(), agent, p3, hptr ) ) );
	else if (!( strcasecmp(p1,"PERSISTENT" ) ))
		return( on_result( on_persistent_object( subptr,p2, default_tls(), agent, p3, hptr ) ) );
	else if (!( strcasecmp(p1,"VOLATILE" ) ))
		return( on_result( on_volatile_object( subptr,p2, default_tls(), agent, p3, hptr ) ) );
	else if (!( strcasecmp(p1,"RENAME" ) ))
		return( on_result( on_rename_object( subptr,p2, default_tls(), agent, p3, p4 ) ) );
	else if (!( strcasecmp(p1,"START" ) ))
		return( on_result( on_start_object( subptr,p2, default_tls(), agent, p3, hptr ) ) );
	else if (!( p4 ))
		return( failure( 30,"p4", "required") );
	else if (!( strcasecmp(p1,"PUT" ) ))
		return( on_result( on_put_object(subptr, p2, default_tls(), agent, p3, p4, hptr ) ) );
	else if (!( p5 ))
		return( failure( 30,"p5", "required") );
	else if (!( p6 ))
		return( failure( 30,"p6", "required") );
	else if (!( strcasecmp(p1,"SAVE" ) ))
		return( on_result( on_save_object( subptr,p2, default_tls(), agent, hptr, p3, p4, p5, p6 ) ) );
	else if (!( strcasecmp(p1,"POST" ) ))
		return( on_result( on_post_object( subptr,p2, default_tls(), agent, hptr, p3, p4, p5, p6, p7 ) ) );
	else	return( failure(32, "incorrect value for p1: ", p1 ) );
}

/* ------------------------------------------------------------------------------------ */
/*				o n _ c o m m a n d 					*/
/* ------------------------------------------------------------------------------------ */
private	int	on_command(int argc, char * argv[] )
{
	struct	on_subscription * subptr;
	int	status;
	int	argi=1;
	char *	aptr;
	char *	user=(char *) 0;
	char *	password=(char *) 0;
	char *	host=(char *) 0;
	char *	agent="CO-ONCLIENT/1.0";
	char *	version="v1.0";
	while ( argi < argc )
	{
		if (!( aptr = argv[argi++] ))
			break;
		else if ( *aptr != '-' )
		{
			if (!( host ))
				return( failure( status, "requires value for", "--host" ) );
			else if (!( user ))
				return( failure( status, "requires value for", "--user" ) );
			else if (!( password ))
				return( failure( status, "requires value for", "--password" ) );
			else if (!(subptr = on_initialise_client( user, password, host, agent, version, tls )))
				return( failure( status, "initialising", "client" ) );
			else	return( on_operation( subptr, aptr, argv[argi], argv[argi+1], argv[argi+2], argv[argi+ 3], argv[argi+ 4], argv[argi+ 5] ) );
		}
		else if (  *(++aptr) == '-' )
		{
			aptr++;

			if (!( strcasecmp( aptr,"user" ) ))
				user = argv[argi++];
			else if (!( strcasecmp( aptr,"password" ) ))
				password = argv[argi++];
			else if (!( strcasecmp( aptr,"version" ) ))
				version = argv[argi++];
			else if (!( strcasecmp( aptr,"host" ) ))
				host = argv[argi++];
			else if (!( strcasecmp( aptr,"agent" ) ))
				agent = argv[argi++];
			else if (!( strcasecmp( aptr,"tls" ) ))
				tls = argv[argi++];
			else if (!( strcasecmp( aptr,"verbose" ) ))
				verbose = 1;
			else if (!( strcasecmp( aptr,"debug" ) ))
				debug=1;
			else	return(failure(30,"incorrect","option"));
			continue;
		}
		else
		{
			switch( *(aptr++) )
			{
			case	'v'	:
				verbose=1; continue;
			case	'd'	:
				debug=1; continue;
			default		:
				return(failure(30,"incorrect","option"));
			}
		}
	}
	return(0);
}

/* ------------------------------------------------------------------------------------ */
/*				o n _ b a n n e r 					*/
/* ------------------------------------------------------------------------------------ */
private	int	on_banner()
{
	printf("\n   CO-OS : CompatibleOne OpenNebula Client Test : Version 1.0a.0.06");
	printf("\n   Beta Version 29/10/2012");
	printf("\n   Copyright (c) 2011,2012 Iain James Marshall, Prologue" );
	printf("\n");
	printf("\n   CRUD Operations ");
	printf("\n");
	printf("\n   LIST [ compute | servers | storage | images | network ] ");
	printf("\n");
	printf("\n   REST Methods");
	printf("\n");
	printf("\n   [ GET      [ compute | storage | network | server ] {id} ");
	printf("\n   [ DELETE   [ compute | storage | network | server ] {id} ");
	printf("\n   [ POST       compute {name} {small|medium|large} {image} {network} {driver} ");
	printf("\n   [ SAVE       compute {id} {number} {name} {driver} ");
	printf("\n   [ STOP       compute {id} ");
	printf("\n   [ START      compute {id} ");
	printf("\n   [ SHUTDOWN   compute {id} ");
	printf("\n   [ POST       storage {name} {description} {type} {size} ");
	printf("\n   [ PUBLIC     storage {id} ");
	printf("\n   [ PRIVATE    storage {id} ");
	printf("\n   [ PERSISTENT storage {id} ");
	printf("\n   [ VOLATILE   storage {id} ");
	printf("\n   [ RENAME     storage {id} {newname} ");
	printf("\n   [ POST       network {name} {[address | NONE]} {size} [PUBLIC | PRIVATE] ");
	printf("\n   [ PUT      [ compute | storage | network | server ] {id} {filename} ");
	printf("\n ");
	printf("\n   Options");
	printf("\n     --user <username>     set account user name ");
	printf("\n     --password <value>    set account password  ");
	printf("\n     --host <hostname>     set host name         ");
	printf("\n     --version <value>     set host version      ");
	printf("\n     --agent   <name>      set test agent name   ");
	printf("\n     --tls <config>        set tls config        ");
	printf("\n     --verbose             activate verbose messages ");
	printf("\n     --debug               activate debug messages ");

	printf("\n\n");
	return( 0 );
}

/* ------------------------------------------------------------------------------------ */
/*					m a i n 					*/
/* ------------------------------------------------------------------------------------ */
public	int	main(int argc, char * argv[] )
{
	if ( argc == 1 )
		return( on_banner() );
	else	return( on_command(argc,argv) );
}

	/* --------- */
#endif	/* _teston_c */
	/* --------- */


