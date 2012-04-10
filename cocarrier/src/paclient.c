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
#ifndef _pa_client_c
#define _pa_client_c

#include "paclient.h"
#include "tlsconfig.h"
#include "tlsload.h"

private	struct pa_config Wpa = {
	(char *) 0,
	(char *) 0,

	(char *) 0,
	(char *) 0,

	(char *) 0,
	(char *) 0,

	(char *) 0,
	(char *) 0,

	(char *) 0,
	(char *) 0,

	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,

	0

	};



/*	------------------------------------------------------------	*/
/*		l i b e r a t e _ p a _ r e s p o n s e			*/
/*	------------------------------------------------------------	*/
public	struct pa_response * liberate_pa_response( struct pa_response * rptr )	
{
	if ( rptr )
	{
		if ( rptr->response )
			rptr->response = liberate_rest_response( rptr->response );
		if ( rptr->jsonroot )
			rptr->jsonroot = drop_data_element( rptr->jsonroot );
		if ( rptr->xmlroot )
			rptr->xmlroot = document_drop( rptr->xmlroot );
		if ( rptr->content )
			liberate( rptr->content );
		liberate( rptr );
	}
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*		 		p a _ c h e c k 			*/
/*	------------------------------------------------------------	*/
private	struct	pa_response * pa_check( struct rest_response * aptr )
{
	struct	pa_response * rptr=(struct pa_response *) 0;
	struct	rest_header * hptr=(struct rest_header *) 0;;
	if ( aptr )
	{
		if ( check_verbose() )
		{
		printf("   OS Client Response : %s %u %s\n",aptr->version,aptr->status,aptr->message);

			if ( check_debug() )
			{
				for (	hptr=aptr->first;
					hptr != (struct rest_header *) 0;
					hptr = hptr->next )
				{
					if (!( hptr->name )) 
						continue;
					else	printf("   %s: %s\n",hptr->name,hptr->value);
				}
				if ( aptr->body )
				{
					printf("   [ %s ] \n",aptr->body );
				}
			}
		}
	}
	if (!( rptr = allocate( sizeof( struct pa_response ) ) ))
		return( rptr );
	else
	{
		rptr->nature = _TEXT_NONE;
		rptr->content= (char *) 0;
		rptr->xmlroot = (struct xml_element *) 0;
		rptr->jsonroot = (struct data_element *) 0;
		rptr->response = aptr;
		if (!( aptr->body ))
			return(rptr);
		if (!( hptr = rest_resolve_header( aptr->first, "Content-Type" ) ))
			return(rptr);
		else if (!( rptr->content = allocate_string( hptr->value ) ))
			return( rptr );
		else if ((!( strcmp( rptr->content, "text/json" 	)))
		     ||  (!( strcmp( rptr->content, "application/json" 	)))
		     ||  (!( strcmp( rptr->content, "x-application/json"))))
		{
			rptr->nature = _TEXT_JSON;
			rptr->jsonroot = json_parse_file( aptr->body );
			return( rptr );
		}
		else if ((!( strcmp( rptr->content, "text/xml" 		)))
		     ||  (!( strcmp( rptr->content, "application/xml" 	)))
		     ||  (!( strcmp( rptr->content, "x-application/xml"))))
		{
			rptr->nature = _TEXT_XML;
			rptr->xmlroot = document_parse_file( aptr->body );
			return( rptr );
		}
		else	return( rptr );
		
	}
}

/*	------------------------------------------------------------	*/
/*		 p a _ c l i e n t _ g e t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response * 
	pa_client_get_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	return( pa_check( rest_client_get_request( target, tls, nptr, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*	      p a _ c l i e n t _ d e l e t e _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response * 
	pa_client_delete_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	return( pa_check( rest_client_delete_request( target, tls, nptr, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*		 p a _ c l i e n t _ h e a d _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response * 
	pa_client_head_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	return( pa_check( rest_client_head_request( target, tls, nptr, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*		 p a _ c l i e n t _ p o s t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response * 
	pa_client_post_request(
		char * target, char * tls, char * nptr, char * filename, struct rest_header * hptr )
{
	return( pa_check( rest_client_post_request( target, tls, nptr, filename, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*		 p a _ c l i e n t _ p u t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response * 
	pa_client_put_request(
		char * target, char * tls, char * nptr, char * filename, struct rest_header * hptr )
{
	return( pa_check( rest_client_put_request( target, tls, nptr, filename, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*			p a _ a u t h e n t i c a t e ()		*/
/*	------------------------------------------------------------	*/
public	struct	rest_header   *	pa_authenticate	( )
{
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	if (!( Wpa.user ))
		return( hptr );
	else if (!( Wpa.password ))
		return( hptr );
	else if (!( Wpa.version ))
		return( hptr );
	else if (!( hptr = rest_create_header( "x-ms-version", Wpa.version ) ))
		return( hptr );
	else if (!( hptr->next = rest_create_header( _HTTP_CONTENT_TYPE, "text/xml" ) ))
		return( hptr );
	else	return((hptr->next->previous = hptr));
}

/*	------------------------------------------------------------	*/
/*		p r o a c t i v e  _ c r e a t e _ o p e r a t i o n		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	proactive_create_operation( char * action, char * filename )
{
	struct	pa_response	*	rptr=(struct pa_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	if (!( hptr = pa_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Wpa.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, action ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = pa_client_post_request( nptr, Wpa.tls, Wpa.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		p r o a c t i v e  _ l i s t _ o p e r a t i o n		*/
/*	------------------------------------------------------------	*/
private	struct	pa_response * proactive_list_operation( char * buffer )
{
	struct	pa_response	*	rptr=(struct pa_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	if (!( hptr = pa_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Wpa.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = pa_client_get_request( nptr, Wpa.tls, Wpa.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		p r o a c t i v e  _ re t r i e v e _ o p e r a t i o n		*/
/*	------------------------------------------------------------	*/
private	struct	pa_response * proactive_retrieve_operation( char * buffer )
{
	struct	pa_response	*	rptr=(struct pa_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	if (!( hptr = pa_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Wpa.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = pa_client_get_request( nptr, Wpa.tls, Wpa.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		p r o a c t i v e  _ d e l e t e _ o p e r a t i o n		*/
/*	------------------------------------------------------------	*/
private	struct	pa_response * proactive_delete_operation( char * buffer )
{
	struct	pa_response	*	rptr=(struct pa_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	if (!( hptr = pa_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Wpa.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = pa_client_delete_request( nptr, Wpa.tls, Wpa.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*	     p a _ c r e a t e _ a f f i n i t y _ g r o u p 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_create_affinity_group( char * filename )
{
	return( proactive_create_operation("/affinitygroups", filename) );
}

/*	------------------------------------------------------------	*/
/*	   p a _ r e t r i e v e _ a f f i n i t y _ g r o u p 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_retrieve_affinity_group( char * id )
{
	char 	buffer[1024];
	sprintf(buffer,"/affinitygroups/%s",id);
	return( proactive_retrieve_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*	     p a _ u p d a t e _ a f f i n i t y _ g r o u p 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_update_affinity_group( char * filename )
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*	     p a _ d e l e t e _ a f f i n i t y _ g r o u p s		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_delete_affinity_group( char * id )
{
	char 	buffer[1024];
	sprintf(buffer,"/affinitygroups/%s",id);
	return( proactive_delete_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*		p a _ l i s t _ a f f i n i t y _ g r o u p s		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_list_affinity_groups()
{
	return( proactive_list_operation( "/affinitygroups" ) );
}

/*	------------------------------------------------------------	*/
/*	     p a _ c r e a t e _ s t o r a g e _ s e r v i c e 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_create_storage_service( char * filename )
{
	return( proactive_create_operation("/services/storageservices", filename) );
}

/*	------------------------------------------------------------	*/
/*	   p a _ r e t r i e v e _ s t o r a g e _ s e r v i c e 	*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_retrieve_storage_service( char * id )
{
	char 	buffer[1024];
	sprintf(buffer,"/services/storageservices/%s",id);
	return( proactive_retrieve_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*	     p a _ u p d a t e _ s t o r a g e _ s e r v i c e 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_update_storage_service( char * filename )
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*	     p a _ d e l e t e _ s t o r a g e _ s e r v i c e		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_delete_storage_service( char * id )
{
	char 	buffer[1024];
	sprintf(buffer,"/services/storageservices/%s",id);
	return( proactive_delete_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*		p a _ l i s t _ s t o r a g e _ s e r v i c e s		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_list_storage_services()
{
	return( proactive_list_operation( "/services/storageservices" ) );
}

/*	------------------------------------------------------------	*/
/*		p a _ l i s t _ o p e r a t i o n s			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_list_operations(char * start,char * end)
{
	char buffer[2048];
	sprintf(buffer,"/operations?StartTime=%s&EndTime=%s",start,end);
	return( proactive_list_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*		p a _ l i s t _ W A T M _ p r o f i l e s		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_list_WATM_profiles()
{
	return( proactive_list_operation( "/services/WATM/profiles" ) );
}

/*	------------------------------------------------------------	*/
/*		p a _ l i s t _ W A T M _ definitions			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_list_WATM_definitions(char * profile )
{
	char 	buffer[2048];
	sprintf(buffer,"/services/WATM/profiles/%s/definitions",profile );
	return( proactive_list_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*	     p a _ c r e a t e _ h o s t e d _ s e r v i c e 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_create_hosted_service( char * filename )
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*	   p a _ r e t r i e v e _ h o s t e d _ s e r v i c e 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_retrieve_hosted_service( char * filename )
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*	     p a _ u p d a t e _ h o s t e d _ s e r v i c e 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_update_hosted_service( char * filename )
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*	     p a _ d e l e t e _ h o s t e d _ s e r v i c e 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_delete_hosted_service( char * filename )
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*		p a _ l i s t _ h o s t e d _ s e r v i c e s		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_list_hosted_services()
{
	return( proactive_list_operation( "/services/hostedservices" ) );
}

/*	------------------------------------------------------------	*/
/*	     p a _ c r e a t e _ c e r t i f i c a t e 			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_create_certificate( char * filename )
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*	   p a _ r e t r i e v e _ c e r t i f i c a t e 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_retrieve_certificate( char * filename )
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*	     p a _ d e l e t e _ c e r t i f i c a t e 			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_delete_certificate( char * filename )
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*		p a _ l i s t _ c e r t i f i c a t e s			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_list_certificates(char * server)
{
	char	buffer[1024];
	sprintf(buffer,"/services/hostedservices/%s/certificates" , server ); 
	return( proactive_list_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*			p a _ l i s t _ l o c a t i o n s 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_list_locations()
{
	return( proactive_list_operation( "/locations" ) );
}


/*	------------------------------------------------------------	*/
/*			p a _ l i s t _ s e r v e r s			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_list_servers	( )
{
	return( proactive_list_operation( "/services/hostedservices" ) );
}


/*	------------------------------------------------------------	*/
/*			p a _ l i s t _ f l a v o u r s			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_list_flavors( )
{
	return( proactive_list_operation( "/flavors" )) ;
}


/*	------------------------------------------------------------	*/
/*			p a _ l i s t _ i m a g e s 			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_list_images( )
{
	return( proactive_list_operation( "/images" ) );
}



/*	------------------------------------------------------------	*/
/*		p a _ c r e a te _  s e r v e r _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	char * pa_create_server_request(
	char * name,
	char * label,
	char * description,
	char * location,
	char * group )
{
	char *	filename;
	FILE *	h;
	char 	buffer[1024];
	int	n;
	n = EncodeBase64( buffer, label,strlen(label));
	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<CreateHostedService xmlns=%c%s%c>\n",0x0022,Wpa.namespace,0x0022);
		fprintf(h,"\t<ServiceName>%s</ServiceName>\n",name);
		fprintf(h,"\t<Label>%s</Label>\n",buffer);
		fprintf(h,"\t<Description>%s</Description>\n",description);
		if ( group )
			fprintf(h,"\t<AffinityGroup>%s</AffinityGroup>\n",group);
		else if ( location )
			fprintf(h,"\t<Location>%s</Location>\n",location);
		fprintf(h,"</CreateHostedService>\n");
		fclose(h);
		return( filename );
	}
}

/*	------------------------------------------------------------------	*/
/*	p a _ c r e a t e _  a f f i n i t y _ g r o u p _ r e q u e s t	*/
/*	------------------------------------------------------------------	*/
public	char * pa_create_affinity_group_request(
	char * name,
	char * label,
	char * description,
	char * location )
{
	char *	filename;
	FILE *	h;
	char 	buffer[1024];
	int	n;
	n = EncodeBase64( buffer, label,strlen(label));
	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<CreateAffinityGroup xmlns=%c%s%c>\n",0x0022,Wpa.namespace,0x0022);
		fprintf(h,"\t<Name>%s</Name>\n",name);
		fprintf(h,"\t<Label>%s</Label>\n",buffer);
		if ( description )
			fprintf(h,"\t<Description>%s</Description>\n",description);
		fprintf(h,"\t<Location>%s</Location>\n",location);
		fprintf(h,"</CreateAffinityGroup>\n");
		fclose(h);
		return( filename );
	}
}

/*	------------------------------------------------------------------	*/
/*	p a _ c r e a t e _  s t o r a g e _ s e r v i c e _ r e q u e s t	*/
/*	------------------------------------------------------------------	*/
public	char * pa_create_storage_service_request(
	char * name,
	char * label,
	char * description,
	char * location,
	char * group )
{
	char *	filename;
	FILE *	h;
	char 	buffer[1024];
	int	n;
	n = EncodeBase64( buffer, label,strlen(label));

	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<CreateStorageServiceInput xmlns=%c%s%c>\n",0x0022,Wpa.namespace,0x0022);
		fprintf(h,"\t<ServiceName>%s</ServiceName>\n",name);
		fprintf(h,"\t<Description>%s</Description>\n",description);
		fprintf(h,"\t<Label>%s</Label>\n",buffer);
		if ( group )
			fprintf(h,"\t<AffinityGroup>%s</AffinityGroup>\n",group);
		else if ( location )
			fprintf(h,"\t<Location>%s</Location>\n",location);
		fprintf(h,"</CreateStorageServiceInput>\n");
		fclose(h);
		return( filename );
	}
}

/*	------------------------------------------------------------	*/
/*		p a _ c r e a te _  i m a g e _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	char * pa_create_image_request(char * identity, char * server )
{
	char *	filename;
	FILE *	h;

	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<CreateImage xmlns=%c%s%c\n",0x0022,Wpa.namespace,0x0022);
		fprintf(h,"\tName=%c%s%c\n",0x0022,identity,0x0022);
		fprintf(h,"\tDeployment=%c%s%c />\n",0x0022,server,0x0022);
		fclose(h);
		return( filename );
	}
}

/*	------------------------------------------------------------	*/
/*			p a _ c r e a t e _  i m a g e   		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_create_image( char * filename )
{
	struct	pa_response	*	rptr=(struct pa_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/images");
	if (!( hptr = pa_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Wpa.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = pa_client_post_request( nptr, Wpa.tls, Wpa.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			p a _ c r e a t e _  s e r v e r 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_create_server( char * filename )
{
	return( proactive_create_operation( "/services/hostedservices", filename ) );
}
	
/*	------------------------------------------------------------	*/
/*			p a _ g e t _ s u b s c r i p t i o n		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_get_subscription()
{

	return( proactive_list_operation( "" ));
}

/*	------------------------------------------------------------	*/
/*			p a _ l i s t _ d e p l o y m e n t s			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_list_deployments(char * server)
{
	char	buffer[1024];
	sprintf(buffer,"/services/hostedservices/%s/deploymentslots" , server ); 
	return( proactive_list_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*		p a _ c r e a t e _ d e p l o y m e n t				*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_create_deployment( char * filename, char * server, char * slot )
{
	char	buffer[1024];
	sprintf(buffer,"/services/hostedservices/%s/deploymentslots/%s" , server, slot ); 
	return( proactive_create_operation( buffer, filename ) );
}

/*	------------------------------------------------------------	*/
/*				p a _ g e t _ d e p l o y m e n t 			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_get_deployment( char * server, char * slot )
{
	char	buffer[1024];
	sprintf(buffer,"/services/hostedservices/%s/deploymentslots/%s" , server, slot ); 
	return( proactive_retrieve_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*			p a _ d e l e t e _ d e p l o y m e n t 			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_delete_deployment( char * server, char * slot )
{
	char	buffer[1024];
	sprintf(buffer,"/services/hostedservices/%s/deploymentslots/%s" , server, slot ); 
	return( proactive_delete_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*	  p a _ c r e a t e _ d e p l o y m e n t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	char * pa_create_deployment_request(
	char * name, char * label, char * image, char * configuration )
{
	char *	filename;
	FILE *	h;
	char 	buffer[1024];
	int	n;
	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<CreateDeployment xmlns=%c%s%c>\n",0x0022,Wpa.namespace,0x0022);
		fprintf(h,"\t<Name>%s</Name>\n",name);
		fprintf(h,"\t<PackageUrl>%s</PackageUrl>\n",image);
		n = EncodeBase64( buffer, label,strlen(label));
		fprintf(h,"\t<Label>%s</Label>\n",buffer);
		n = EncodeBase64( buffer, configuration,strlen(configuration));
		fprintf(h,"\t<Configuration>%s</Configuration>\n",buffer);
		fprintf(h,"\t<StartDeployment>true</StartDeployment>\n");
		fprintf(h,"\t<TreatWarningsAsError>true</TreatWarningsAsError>\n");
		fprintf(h,"</CreateDeployment>\n");
		fclose(h);
		return( filename );
	}
}

/*	------------------------------------------------------------	*/
/*			p a _ g e t _ s e r v e r 			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_get_server	(  char * id )
{
	struct	pa_response	*	rptr=(struct pa_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/services/hostedservices/%s",id);
	if (!( hptr = pa_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Wpa.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = pa_client_get_request( nptr, Wpa.tls, Wpa.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			p a _ g e t _ f l av o u r			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_get_flavor(  char * id )
{
	struct	pa_response	*	rptr=(struct pa_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/flavors/%s",id);
	if (!( hptr = pa_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Wpa.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = pa_client_get_request( nptr, Wpa.tls, Wpa.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			p a _ g e t _ i m a g e 			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_get_image 	(  char * id )
{
	struct	pa_response	*	rptr=(struct pa_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/images/%s",id);
	if (!( hptr = pa_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Wpa.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = pa_client_get_request( nptr, Wpa.tls, Wpa.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			p a _ u p d a t e _ s e r v e r 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_update_server(  char * id, char * filename )
{
	struct	pa_response	*	rptr=(struct pa_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/services/hostedservices/%s",id);
	if (!( hptr = pa_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Wpa.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = pa_client_put_request( nptr, Wpa.tls, Wpa.agent,filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			p a _ d e l e t e _ s e r v e r 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_delete_server(  char * id )
{
	char	buffer[1024];
	sprintf(buffer,"/services/hostedservices/%s",id);
	return( proactive_delete_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*			p a _ d e l e t e _ i m a g e 			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_delete_image	(  char * id )
{
	struct	pa_response	*	rptr=(struct pa_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/images/%s",id);
	if (!( hptr = pa_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Wpa.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = pa_client_get_request( nptr, Wpa.tls, Wpa.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		p a _ i n i t i a l i s e _ c l i e n t 		*/
/*	------------------------------------------------------------	*/
public	int	pa_initialise_client( 
		char * user, char * password, 
		char * host, char * agent, 
		char * version, char * tls,
		char * ns, char * subscription )
{
	if (!( Wpa.user = allocate_string( user )))
		return( 27 );
	if (!( Wpa.password = allocate_string( password )))
		return( 27 );
	if (!( Wpa.host = allocate_string( host )))
		return( 27 );
	else if (!( Wpa.agent = allocate_string( agent )))
		return( 27 );
	else if (!( Wpa.version = allocate_string( version )))
		return( 27 );
	else if (!( Wpa.namespace = allocate_string( ns ) ))
		return( 27 );
	else if (!( Wpa.subscription = allocate_string( subscription ) ))
		return( 27 );

	else if (!( Wpa.base = allocate( strlen( Wpa.host ) + strlen( Wpa.subscription ) + 16 ) ))
		return( 27 );
	else	sprintf(Wpa.base,"%s%s",Wpa.host,Wpa.subscription);

	Wpa.authenticate= (char *) 0;
	Wpa.tls = tls;
	return( 0 );
}



#endif	/* _pa_client_c */
		/* ------------ */


