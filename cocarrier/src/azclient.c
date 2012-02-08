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
#ifndef _az_client_c
#define _az_client_c

#include "azclient.h"
#include "tlsconfig.h"
#include "tlsload.h"

private	struct az_config Waz = {
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
/*		l i b e r a t e _ a z _ r e s p o n s e			*/
/*	------------------------------------------------------------	*/
public	struct az_response * liberate_az_response( struct az_response * rptr )	
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
	return((struct az_response *) 0);
}

/*	------------------------------------------------------------	*/
/*		 		a z _ c h e c k 			*/
/*	------------------------------------------------------------	*/
private	struct	az_response * az_check( struct rest_response * aptr )
{
	struct	az_response * rptr=(struct az_response *) 0;
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
	if (!( rptr = allocate( sizeof( struct az_response ) ) ))
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
/*		 a z _ c l i e n t _ g e t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	az_response * 
	az_client_get_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	return( az_check( rest_client_get_request( target, tls, nptr, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*	      a z _ c l i e n t _ d e l e t e _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	az_response * 
	az_client_delete_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	return( az_check( rest_client_delete_request( target, tls, nptr, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*		 a z _ c l i e n t _ h e a d _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	az_response * 
	az_client_head_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	return( az_check( rest_client_head_request( target, tls, nptr, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*		 a z _ c l i e n t _ p o s t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	az_response * 
	az_client_post_request(
		char * target, char * tls, char * nptr, char * filename, struct rest_header * hptr )
{
	return( az_check( rest_client_post_request( target, tls, nptr, filename, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*		 a z _ c l i e n t _ p u t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	az_response * 
	az_client_put_request(
		char * target, char * tls, char * nptr, char * filename, struct rest_header * hptr )
{
	return( az_check( rest_client_put_request( target, tls, nptr, filename, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*			a z _ a u t h e n t i c a t e ()		*/
/*	------------------------------------------------------------	*/
public	struct	rest_header   *	az_authenticate	( )
{
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	if (!( Waz.user ))
		return( hptr );
	else if (!( Waz.password ))
		return( hptr );
	else if (!( Waz.version ))
		return( hptr );
	else if (!( hptr = rest_create_header( "x-ms-version", Waz.version ) ))
		return( hptr );
	else if (!( hptr->next = rest_create_header( _HTTP_CONTENT_TYPE, "text/xml" ) ))
		return( hptr );
	else	return((hptr->next->previous = hptr));
}

/*	------------------------------------------------------------	*/
/*		a z u r e  _ c r e a t e _ o p e r a t i o n		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	azure_create_operation( char * action, char * filename )
{
	struct	az_response	*	rptr=(struct az_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	if (!( hptr = az_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Waz.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, action ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = az_client_post_request( nptr, Waz.tls, Waz.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		a z u r e  _ l i s t _ o p e r a t i o n		*/
/*	------------------------------------------------------------	*/
private	struct	az_response * azure_list_operation( char * buffer )
{
	struct	az_response	*	rptr=(struct az_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	if (!( hptr = az_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Waz.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = az_client_get_request( nptr, Waz.tls, Waz.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		a z u r e  _ re t r i e v e _ o p e r a t i o n		*/
/*	------------------------------------------------------------	*/
private	struct	az_response * azure_retrieve_operation( char * buffer )
{
	struct	az_response	*	rptr=(struct az_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	if (!( hptr = az_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Waz.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = az_client_get_request( nptr, Waz.tls, Waz.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		a z u r e  _ d e l e t e _ o p e r a t i o n		*/
/*	------------------------------------------------------------	*/
private	struct	az_response * azure_delete_operation( char * buffer )
{
	struct	az_response	*	rptr=(struct az_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	if (!( hptr = az_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Waz.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = az_client_delete_request( nptr, Waz.tls, Waz.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*	     a z _ c r e a t e _ a f f i n i t y _ g r o u p 		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_create_affinity_group( char * filename )
{
	return( azure_create_operation("/affinitygroups", filename) );
}

/*	------------------------------------------------------------	*/
/*	   a z _ r e t r i e v e _ a f f i n i t y _ g r o u p 		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_retrieve_affinity_group( char * id )
{
	char 	buffer[1024];
	sprintf(buffer,"/affinitygroups/%s",id);
	return( azure_retrieve_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*	     a z _ u p d a t e _ a f f i n i t y _ g r o u p 		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_update_affinity_group( char * filename )
{
	return((struct az_response *) 0);
}

/*	------------------------------------------------------------	*/
/*	     a z _ d e l e t e _ a f f i n i t y _ g r o u p s		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_delete_affinity_group( char * id )
{
	char 	buffer[1024];
	sprintf(buffer,"/affinitygroups/%s",id);
	return( azure_delete_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*		a z _ l i s t _ a f f i n i t y _ g r o u p s		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_list_affinity_groups()
{
	return( azure_list_operation( "/affinitygroups" ) );
}

/*	------------------------------------------------------------	*/
/*	     a z _ c r e a t e _ s t o r a g e _ s e r v i c e 		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_create_storage_service( char * filename )
{
	return( azure_create_operation("/services/storageservices", filename) );
}

/*	------------------------------------------------------------	*/
/*	   a z _ r e t r i e v e _ s t o r a g e _ s e r v i c e 	*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_retrieve_storage_service( char * id )
{
	char 	buffer[1024];
	sprintf(buffer,"/services/storageservices/%s",id);
	return( azure_retrieve_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*	     a z _ u p d a t e _ s t o r a g e _ s e r v i c e 		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_update_storage_service( char * filename )
{
	return((struct az_response *) 0);
}

/*	------------------------------------------------------------	*/
/*	     a z _ d e l e t e _ s t o r a g e _ s e r v i c e		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_delete_storage_service( char * id )
{
	char 	buffer[1024];
	sprintf(buffer,"/services/storageservices/%s",id);
	return( azure_delete_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*		a z _ l i s t _ s t o r a g e _ s e r v i c e s		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_list_storage_services()
{
	return( azure_list_operation( "/services/storageservices" ) );
}

/*	------------------------------------------------------------	*/
/*		a z _ l i s t _ o p e r a t i o n s			*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_list_operations(char * start,char * end)
{
	char buffer[2048];
	sprintf(buffer,"/operations?StartTime=%s&EndTime=%s",start,end);
	return( azure_list_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*		a z _ l i s t _ W A T M _ p r o f i l e s		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_list_WATM_profiles()
{
	return( azure_list_operation( "/services/WATM/profiles" ) );
}

/*	------------------------------------------------------------	*/
/*		a z _ l i s t _ W A T M _ definitions			*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_list_WATM_definitions(char * profile )
{
	char 	buffer[2048];
	sprintf(buffer,"/services/WATM/profiles/%s/definitions",profile );
	return( azure_list_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*	     a z _ c r e a t e _ h o s t e d _ s e r v i c e 		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_create_hosted_service( char * filename )
{
	return((struct az_response *) 0);
}

/*	------------------------------------------------------------	*/
/*	   a z _ r e t r i e v e _ h o s t e d _ s e r v i c e 		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_retrieve_hosted_service( char * filename )
{
	return((struct az_response *) 0);
}

/*	------------------------------------------------------------	*/
/*	     a z _ u p d a t e _ h o s t e d _ s e r v i c e 		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_update_hosted_service( char * filename )
{
	return((struct az_response *) 0);
}

/*	------------------------------------------------------------	*/
/*	     a z _ d e l e t e _ h o s t e d _ s e r v i c e 		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_delete_hosted_service( char * filename )
{
	return((struct az_response *) 0);
}

/*	------------------------------------------------------------	*/
/*		a z _ l i s t _ h o s t e d _ s e r v i c e s		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_list_hosted_services()
{
	return( azure_list_operation( "/services/hostedservices" ) );
}

/*	------------------------------------------------------------	*/
/*	     a z _ c r e a t e _ c e r t i f i c a t e 			*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_create_certificate( char * filename )
{
	return((struct az_response *) 0);
}

/*	------------------------------------------------------------	*/
/*	   a z _ r e t r i e v e _ c e r t i f i c a t e 		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_retrieve_certificate( char * filename )
{
	return((struct az_response *) 0);
}

/*	------------------------------------------------------------	*/
/*	     a z _ d e l e t e _ c e r t i f i c a t e 			*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_delete_certificate( char * filename )
{
	return((struct az_response *) 0);
}

/*	------------------------------------------------------------	*/
/*		a z _ l i s t _ c e r t i f i c a t e s			*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_list_certificates(char * server)
{
	char	buffer[1024];
	sprintf(buffer,"/services/hostedservices/%s/certificates" , server ); 
	return( azure_list_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*			a z _ l i s t _ l o c a t i o n s 		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_list_locations()
{
	return( azure_list_operation( "/locations" ) );
}


/*	------------------------------------------------------------	*/
/*			a z _ l i s t _ s e r v e r s			*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_list_servers	( )
{
	return( azure_list_operation( "/services/hostedservices" ) );
}


/*	------------------------------------------------------------	*/
/*			a z _ l i s t _ f l a v o u r s			*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_list_flavors( )
{
	return( azure_list_operation( "/flavors" )) ;
}


/*	------------------------------------------------------------	*/
/*			a z _ l i s t _ i m a g e s 			*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_list_images( )
{
	return( azure_list_operation( "/images" ) );
}



/*	------------------------------------------------------------	*/
/*		a z _ c r e a te _  s e r v e r _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	char * az_create_server_request(
	char * identity, 
	char * image, 
	char * flavor, 
	char * personality, 
	char * resource )
{
	char *	filename;
	FILE *	h;

	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<CreateDeployment xmlns=%c%s%c>\n",0x0022,Waz.namespace,0x0022);
		fprintf(h,"\t<Name>%s</Name>\n",identity);
		fprintf(h,"\t<PackageUrl>%s</PackageUrl>\n",image);
		fprintf(h,"\t<Label>%s</Label>\n","BASE64:LABEL");
		fprintf(h,"\t<Configuration>%s</Configuration>\n","BASE64:CONFIGURATION");
		fprintf(h,"\t<StartDeployment>true</StartDeployment>\n");
		fprintf(h,"\t<TreatWarningsAsError>true</TreatWarningsAsError>\n");
		fprintf(h,"</CreateDeployment>\n");
		fclose(h);
		return( filename );
	}
}

/*	------------------------------------------------------------------	*/
/*	a z _ c r e a t e _  a f f i n i t y _ g r o u p _ r e q u e s t	*/
/*	------------------------------------------------------------------	*/
public	char * az_create_affinity_group_request(
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
		fprintf(h,"<CreateAffinityGroup xmlns=%c%s%c>\n",0x0022,Waz.namespace,0x0022);
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
/*	a z _ c r e a t e _  s t o r a g e _ s e r v i c e _ r e q u e s t	*/
/*	------------------------------------------------------------------	*/
public	char * az_create_storage_service_request(
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
		fprintf(h,"<CreateStorageServiceInput xmlns=%c%s%c>\n",0x0022,Waz.namespace,0x0022);
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
/*		a z _ c r e a te _  i m a g e _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	char * az_create_image_request(char * identity, char * server )
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
		fprintf(h,"<CreateImage xmlns=%c%s%c\n",0x0022,Waz.namespace,0x0022);
		fprintf(h,"\tName=%c%s%c\n",0x0022,identity,0x0022);
		fprintf(h,"\tDeployment=%c%s%c />\n",0x0022,server,0x0022);
		fclose(h);
		return( filename );
	}
}

/*	------------------------------------------------------------	*/
/*			a z _ c r e a t e _  i m a g e   		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_create_image( char * filename )
{
	struct	az_response	*	rptr=(struct az_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/images");
	if (!( hptr = az_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Waz.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = az_client_post_request( nptr, Waz.tls, Waz.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			a z _ c r e a t e _  s e r v e r 		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_create_server( char * filename )
{
	return( azure_create_operation( "/services/hostedservices", filename ) );
}

	
/*	------------------------------------------------------------	*/
/*			a z _ g e t _ s u b s c r i p t i o n		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_get_subscription()
{

	return( azure_list_operation( "" ));
}


/*	------------------------------------------------------------	*/
/*			a z _ g e t _ s e r v e r 			*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_get_server	(  char * id )
{
	struct	az_response	*	rptr=(struct az_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/services/hostedservices/%s",id);
	if (!( hptr = az_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Waz.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = az_client_get_request( nptr, Waz.tls, Waz.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			a z _ g e t _ f l av o u r			*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_get_flavor(  char * id )
{
	struct	az_response	*	rptr=(struct az_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/flavors/%s",id);
	if (!( hptr = az_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Waz.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = az_client_get_request( nptr, Waz.tls, Waz.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			a z _ g e t _ i m a g e 			*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_get_image 	(  char * id )
{
	struct	az_response	*	rptr=(struct az_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/images/%s",id);
	if (!( hptr = az_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Waz.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = az_client_get_request( nptr, Waz.tls, Waz.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			a z _ u p d a t e _ s e r v e r 		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_update_server(  char * id, char * filename )
{
	struct	az_response	*	rptr=(struct az_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/services/hostedservices/%s",id);
	if (!( hptr = az_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Waz.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = az_client_put_request( nptr, Waz.tls, Waz.agent,filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			a z _ d e l e t e _ s e r v e r 		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_delete_server(  char * id )
{
	struct	az_response	*	rptr=(struct az_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/services/hostedservices/%s",id);
	if (!( hptr = az_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Waz.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = az_client_delete_request( nptr, Waz.tls, Waz.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			a z _ d e l e t e _ i m a g e 			*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_delete_image	(  char * id )
{
	struct	az_response	*	rptr=(struct az_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/images/%s",id);
	if (!( hptr = az_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Waz.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = az_client_get_request( nptr, Waz.tls, Waz.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		a z _ i n i t i a l i s e _ c l i e n t 		*/
/*	------------------------------------------------------------	*/
public	int	az_initialise_client( 
		char * user, char * password, 
		char * host, char * agent, 
		char * version, char * tls,
		char * ns, char * subscription )
{
	if (!( Waz.user = allocate_string( user )))
		return( 27 );
	if (!( Waz.password = allocate_string( password )))
		return( 27 );
	if (!( Waz.host = allocate_string( host )))
		return( 27 );
	else if (!( Waz.agent = allocate_string( agent )))
		return( 27 );
	else if (!( Waz.version = allocate_string( version )))
		return( 27 );
	else if (!( Waz.namespace = allocate_string( ns ) ))
		return( 27 );
	else if (!( Waz.subscription = allocate_string( subscription ) ))
		return( 27 );

	else if (!( Waz.base = allocate( strlen( Waz.host ) + strlen( Waz.subscription ) + 16 ) ))
		return( 27 );
	else	sprintf(Waz.base,"%s%s",Waz.host,Waz.subscription);

	Waz.authenticate= (char *) 0;
	Waz.tls = tls;
	return( 0 );
}



#endif	/* _az_client_c */
	/* ------------ */




