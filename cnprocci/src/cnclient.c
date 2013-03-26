/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2013 by Daniel Sprouse (ComputeNext) <daniel@computenext.com>   */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License");      */
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at                              */
/*                                                                      */
/*  http://www.apache.org/licenses/LICENSE-2.0                          */
/*                                                                      */
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or      */
/* implied.                                                             */
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License.                                       */
/* -------------------------------------------------------------------- */
#ifndef _cn_client_c
#define _cn_client_c

#include "cnclient.h"
#include "encode64.c"

public  struct cn_config * cn_allocate_config()
{
	struct	cn_config * sptr;
	if (!( sptr = (struct cn_config *) allocate( sizeof( struct cn_config ) ) ))
		return( sptr );
	else
	{
	 	memset( sptr, 0, sizeof( struct cn_config ));
		return( sptr );
	}
}

public  struct cn_response * cn_liberate_response( struct cn_response * rptr )
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
	return((struct cn_response *) 0);
}

public  struct cn_response * cn_check( struct rest_response *  aptr )
{
	struct	cn_response * rptr=(struct cn_response *) 0;
	struct	rest_header * hptr=(struct rest_header *) 0;;
	if ( aptr )
	{
		if ( check_verbose() )
		{
			printf("   CN Client Response : %s %u %s\n",aptr->version,aptr->status,aptr->message);

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
	if (!( rptr = allocate( sizeof( struct cn_response ) ) ))
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
		     ||  (!( strcmp( rptr->content, "x-application/json")))
		     ||  (!( strcmp( rptr->content, "application/json; charset=utf-8"))))
		{
			
			if(!( rptr->jsonroot = json_parse_file( aptr->body ) ))
				rptr->nature = _TEXT_NONE;
			else rptr->nature = _TEXT_JSON;
				
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

private struct rest_header * cn_client_build_header( struct cn_config * sptr ) 
{
	struct	rest_header *   hptr=(struct rest_header * ) 0;
	char	buffer[1024];
	char	encoded[1024];
	char    basic_auth[1024];
	
	sprintf(buffer,"%s:%s", sptr->apikey, sptr->apisec);
	
	if (!( sptr->apikey ))
		return( hptr );
	else if (!( sptr->apisec ))
		return( hptr );
	else if (!( sptr->authentication ))
	{
		if (!( EncodeBase64(encoded, buffer, strlen( buffer )) ))
			return( hptr );
			
		sprintf(basic_auth,"Basic %s", encoded);
		
		if (!(sptr->authentication = allocate_string( basic_auth )))
			return( hptr );
	}
	
	if (!( hptr = rest_create_header( _HTTP_AUTHORIZATION, sptr->authentication ) ))
		return((struct rest_header *) 0);
	else if (!( hptr->next = rest_create_header( _HTTP_ACCEPT, "application/json" ) ))
		return( liberate_rest_header( hptr ) );
	else if (!( hptr->next->next = rest_create_header( _HTTP_CONTENT_TYPE, "application/json" ) ))
		return( liberate_rest_header( hptr ) );	
	else if (!(hptr->next->next->previous = hptr->next))
		return( liberate_rest_header( hptr ) );	
	else	return((hptr->next->previous = hptr));
}

private struct cn_response * cn_client_delete_request( struct cn_config * sptr, char * buffer )
{
	struct  cn_response *   rptr=(struct cn_response *) 0;
	struct  url         *   uptr;
	char                *   nptr;
	struct  rest_header *   hptr=(struct rest_header * ) 0;
	
	if (!( hptr = cn_client_build_header( sptr ) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->host )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr=cn_check( rest_client_delete_request( nptr, sptr->tls, sptr->agent, hptr ) ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

private struct cn_response * cn_client_get_request( struct cn_config * sptr, char * buffer )
{
	struct  cn_response *   rptr=(struct cn_response *) 0;
	struct  url         *   uptr;
	char                *   nptr;
	struct  rest_header *   hptr=(struct rest_header * ) 0;
	
	if (!( hptr = cn_client_build_header( sptr ) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->host )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr=cn_check( rest_client_get_request( nptr, sptr->tls, sptr->agent, hptr ) ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

private struct cn_response * cn_client_post_request( struct cn_config * sptr, char * buffer, char * filename )
{
	struct  cn_response *   rptr=(struct cn_response *) 0;
	struct  url         *   uptr;
	char                *   nptr;
	struct  rest_header *   hptr=(struct rest_header * ) 0;
	
	if (!( hptr = cn_client_build_header( sptr ) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->host )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr=cn_check( rest_client_post_request( nptr, sptr->tls, sptr->agent, filename, hptr ) ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

private struct cn_response * cn_client_put_request( struct cn_config * sptr, char * buffer, char * filename )
{
	struct  cn_response *   rptr=(struct cn_response *) 0;
	struct  url         *   uptr;
	char                *   nptr;
	struct  rest_header *   hptr=(struct rest_header * ) 0;
	
	if (!( hptr = cn_client_build_header( sptr ) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->host )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr=cn_check( rest_client_put_request( nptr, sptr->tls, sptr->agent, filename, hptr ) ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

public  struct cn_response * cn_list_images( struct cn_config * sptr, char * query )
{
	char	buffer[1024];
	
	sprintf(buffer,"/resourceQuery/query/image?%s",query);
	return( cn_client_get_request(sptr, buffer ) ); 
}

public  struct cn_response * cn_list_instancetypes( struct cn_config * sptr, char * query )
{
	char	buffer[1024];
	
	sprintf(buffer,"/resourceQuery/query/instanceType?%s",query);
	return( cn_client_get_request(sptr, buffer ) ); 
}

public  struct cn_response * cn_list_virtualmachines( struct cn_config * sptr, char * query )
{
	char	buffer[1024];
	
	sprintf(buffer,"/resourceQuery/query/virtualMachine?%s",query);
	return( cn_client_get_request(sptr, buffer ) ); 
}

public  struct cn_response * cn_list_volumestores( struct cn_config * sptr, char * query )
{
	char	buffer[1024];
	
	sprintf(buffer,"/resourceQuery/query/volumeStorage?%s",query);
	return( cn_client_get_request(sptr, buffer ) ); 
}

public  struct cn_response * cn_list_securitygroups( struct cn_config * sptr )
{
	return( cn_client_get_request(sptr, "/securitygroups" ) );
}

public  struct cn_response * cn_list_keypairs( struct cn_config * sptr )
{
	return( cn_client_get_request(sptr, "/keypairs" ) );
}

public  struct cn_response * cn_list_workloads( struct cn_config * sptr )
{
	return( cn_client_get_request(sptr, "/workloads" ) );
}

public  struct cn_response * cn_list_transactions( struct cn_config * sptr )
{
	return( cn_client_get_request(sptr, "/transactions" ) );
}


public  struct cn_response * cn_get_image( struct cn_config * sptr, char * image )
{
	char	buffer[1024];
	
	sprintf(buffer,"/resource/%s",image);
	return( cn_client_get_request( sptr, buffer ) );
}

public  struct cn_response * cn_get_instancetype( struct cn_config * sptr, char * instancetype )
{
	char	buffer[1024];
	
	sprintf(buffer,"/resource/%s",instancetype);
	return( cn_client_get_request( sptr, buffer ) );
}

public  struct cn_response * cn_get_virtualmachine( struct cn_config * sptr, char * virtualmachine )
{
	char	buffer[1024];
	
	sprintf(buffer,"/resource/%s",virtualmachine);
	return( cn_client_get_request( sptr, buffer ) );
}

public  struct cn_response * cn_get_volumestore( struct cn_config * sptr, char * volumestore )
{
	char	buffer[1024];
	
	sprintf(buffer,"/resource/%s",volumestore);
	return( cn_client_get_request( sptr, buffer ) );
}

public  struct cn_response * cn_get_securitygroup( struct cn_config * sptr, char * securitygroup )
{
	char	buffer[1024];
	
	sprintf(buffer,"/securitygroups/%s",securitygroup);
	return( cn_client_get_request( sptr, buffer ) );
}

public  struct cn_response * cn_get_keypair( struct cn_config * sptr, char * keypair )
{
	char	buffer[1024];
	
	sprintf(buffer,"/keypairs/%s",keypair);
	return( cn_client_get_request( sptr, buffer ) );
}

public  struct cn_response * cn_get_workload( struct cn_config * sptr, char * workload )
{
	char	buffer[1024];
	
	sprintf(buffer,"/workloads/%s",workload);
	return( cn_client_get_request( sptr, buffer ) );
}

public  struct cn_response * cn_get_transaction( struct cn_config * sptr, char * transaction )
{
	char	buffer[1024];
	
	sprintf(buffer,"/transactions/%s",transaction);
	return( cn_client_get_request( sptr, buffer ) );
}


public  struct cn_response * cn_delete_securitygroup( struct cn_config * sptr, char * securitygroup )
{
	char	buffer[1024];
	
	sprintf(buffer,"/securitygroups/%s",securitygroup);
	return( cn_client_delete_request( sptr, buffer ) );
}

public  struct cn_response * cn_delete_keypair( struct cn_config * sptr, char * keypair )
{
	char	buffer[1024];
	
	sprintf(buffer,"/keypairs/%s",keypair);
	return( cn_client_delete_request( sptr, buffer ) );
}

public  struct cn_response * cn_delete_workload( struct cn_config * sptr, char * workload )
{
	char	buffer[1024];
	
	sprintf(buffer,"/workloads/%s",workload);
	return( cn_client_delete_request( sptr, buffer ) );
}

public  struct cn_response * cn_delete_workload_element( struct cn_config * sptr, char * workload, char * workloadelement)
{
	char	buffer[1024];
	
	sprintf(buffer,"/workloads/%s/%s",workload,workloadelement);
	return( cn_client_delete_request( sptr, buffer ) );
}

public  struct cn_response * cn_delete_transaction( struct cn_config * sptr, char * transaction )
{
	char	buffer[1024];
	
	sprintf(buffer,"/transactions/%s",transaction);
	return( cn_client_delete_request( sptr, buffer ) );
}


public  struct cn_response * cn_create_securitygroup( struct cn_config * sptr, char * filename )
{
	return( cn_client_post_request(sptr, "/securitygroups", filename ) );
}

public  struct cn_response * cn_create_keypair( struct cn_config * sptr, char * filename )
{
	return( cn_client_post_request(sptr, "/keypairs", filename ) );
}

public  struct cn_response * cn_create_workload( struct cn_config * sptr, char * filename )
{
	return( cn_client_post_request(sptr, "/workloads", filename ) );
}

public  struct cn_response * cn_create_transaction( struct cn_config * sptr, char * filename )
{
	return( cn_client_post_request(sptr, "/transactions", filename ) );
}


public  struct cn_response * cn_add_port( struct cn_config * sptr, char * securitygroup, char * filename )
{
	char	buffer[1024];
	
	sprintf(buffer,"/securitygroups/%s/addport",securitygroup);
	return( cn_client_put_request( sptr, buffer, filename ) );
}

public  struct cn_response * cn_remove_port( struct cn_config * sptr, char * securitygroup, char * filename )
{
	char	buffer[1024];
	
	sprintf(buffer,"/securitygroups/%s/removeport",securitygroup);
	return( cn_client_put_request( sptr, buffer, filename ) );
}


public  struct cn_response * cn_add_vm_to_workload( struct cn_config * sptr, char * workload, char * filename )
{
	char	buffer[1024];
	
	sprintf(buffer,"/workloads/%s/virtualmachine",workload);
	return( cn_client_post_request( sptr, buffer, filename ) );
}

public  struct cn_response * cn_add_vs_to_workload( struct cn_config * sptr, char * workload, char * filename )
{
	char	buffer[1024];
	
	sprintf(buffer,"/workloads/%s/volumestorage",workload);
	return( cn_client_post_request( sptr, buffer, filename ) );
}


public  struct cn_response * cn_start_transaction( struct cn_config * sptr, char * transaction, char * filename )
{
	char	buffer[1024];
	
	sprintf(buffer,"/transactions/%s/start",transaction);
	return( cn_client_put_request( sptr, buffer, filename ) );
}

public  struct cn_response * cn_stop_transaction( struct cn_config * sptr, char * transaction, char * filename )
{
	char	buffer[1024];
	
	sprintf(buffer,"/transactions/%s/stop",transaction);
	return( cn_client_put_request( sptr, buffer, filename ) );
}


public  char * cn_create_securitygroup_request(  char * name, char * description, char * region, char * provider )
{
	char *	filename;
	FILE *	h;

	if (!( filename = rest_temporary_filename("json")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else 
	{
		fprintf(h,"{\n");
		fprintf(h,"\t%cName%c:%c%s%c,\n",0x0022,0x0022,0x0022,name,0x0022);
		fprintf(h,"\t%cDescription%c:%c%s%c,\n",0x0022,0x0022,0x0022,description,0x0022);
		fprintf(h,"\t%cRegion%c:%c%s%c,\n",0x0022,0x0022,0x0022,region,0x0022);
		fprintf(h,"\t%cProviderId%c:%c%s%c\n",0x0022,0x0022,0x0022,provider,0x0022);
		fprintf(h,"}\n");
		fclose(h);
		return( filename );
	}
}

public  char * cn_create_keypair_request( char * name, char * region, char * provider )
{
	char *	filename;
	FILE *	h;

	if (!( filename = rest_temporary_filename("json")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else 
	{
		fprintf(h,"{\n");
		fprintf(h,"\t%cName%c:%c%s%c,\n",0x0022,0x0022,0x0022,name,0x0022);
		fprintf(h,"\t%cRegion%c:%c%s%c,\n",0x0022,0x0022,0x0022,region,0x0022);
		fprintf(h,"\t%cProvider%c:%c%s%c\n",0x0022,0x0022,0x0022,provider,0x0022);	
		fprintf(h,"}\n");
		fclose(h);
		return( filename );
	}
}

public  char * cn_create_workload_request( char * name )
{
	char *	filename;
	FILE *	h;

	if (!( filename = rest_temporary_filename("json")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else 
	{
		fprintf(h,"{\n");
		fprintf(h,"\t%cWorkloadName%c:%c%s%c\n",0x0022,0x0022,0x0022,name,0x0022);
		fprintf(h,"}\n");
		fclose(h);
		return( filename );
	}
}

public  char * cn_create_transaction_request( char * workload, char * billingtype )
{
	char *	filename;
	FILE *	h;

	if (!( filename = rest_temporary_filename("json")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else 
	{
		fprintf(h,"{\n");
		fprintf(h,"\t%cWorkloadId%c:%c%s%c,\n",0x0022,0x0022,0x0022,workload,0x0022);
		fprintf(h,"\t%cTransactionType%c:%c%s%c\n",0x0022,0x0022,0x0022,billingtype,0x0022);	
		fprintf(h,"}\n");
		fclose(h);
		return( filename );
	}
}


public  char * cn_add_port_request( char * protocol, char * from, char * to, char * cidr )
{
	char *	filename;
	FILE *	h;

	if (!( filename = rest_temporary_filename("json")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else 
	{
		fprintf(h,"{\n");
		fprintf(h,"\t%cProtocol%c:%c%s%c,\n",0x0022,0x0022,0x0022,protocol,0x0022);
		fprintf(h,"\t%cFromPort%c:%c%s%c,\n",0x0022,0x0022,0x0022,from,0x0022);	
		fprintf(h,"\t%cToPort%c:%c%s%c,\n",0x0022,0x0022,0x0022,to,0x0022);	
		fprintf(h,"\t%cIpAddress%c:%c%s%c\n",0x0022,0x0022,0x0022,cidr,0x0022);	
		fprintf(h,"}\n");
		fclose(h);
		return( filename );
	}
}

public  char * cn_remove_port_request( char * protocol, char * from, char * to, char * cidr )
{
	char *	filename;
	FILE *	h;

	if (!( filename = rest_temporary_filename("json")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else 
	{
		fprintf(h,"{\n");
		fprintf(h,"\t%cProtocol%c:%c%s%c,\n",0x0022,0x0022,0x0022,protocol,0x0022);
		fprintf(h,"\t%cFromPort%c:%c%s%c,\n",0x0022,0x0022,0x0022,from,0x0022);	
		fprintf(h,"\t%cToPort%c:%c%s%c,\n",0x0022,0x0022,0x0022,to,0x0022);	
		fprintf(h,"\t%cIpAddress%c:%c%s%c\n",0x0022,0x0022,0x0022,cidr,0x0022);	
		fprintf(h,"}\n");
		fclose(h);
		return( filename );
	}
}

public  char * cn_add_vm_request(
        char * name,                /* the display name for the workload element */
        int count,                  /* the number of identical elements to be added */
        char * image,               /* the identity of the image */
        char * instancetype,        /* the identity of the instance */
        char * securitygroup,       /* OPTIONAL: the identity of the security group */
        char * keypair,             /* OPTIONAL: the identity of the keypair */
        char * resourceparameters ) /* OPTIONAL: extra JSON to be passed to the provider to be used for deployment */
{
	char *	filename;
	FILE *	h;

	if (!( filename = rest_temporary_filename("json")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else 
	{
		fprintf(h,"{\n");
		fprintf(h,"\t%cName%c:%c%s%c,\n",0x0022,0x0022,0x0022,name,0x0022);
		fprintf(h,"\t%cImageId%c:%c%s%c,\n",0x0022,0x0022,0x0022,image,0x0022);	
		fprintf(h,"\t%cInstanceTypeId%c:%c%s%c,\n",0x0022,0x0022,0x0022,instancetype,0x0022);
		
		if(securitygroup != (char *) 0)
			fprintf(h,"\t%cSecurityGroupNames%c:[%c%s%c],\n",0x0022,0x0022,0x0022,securitygroup,0x0022);
			
		if(keypair != (char *) 0)
			fprintf(h,"\t%cKeyPairName%c:%c%s%c,\n",0x0022,0x0022,0x0022,keypair,0x0022);
			
		if(resourceparameters != (char *) 0)	
			fprintf(h,"\t%cResourceParameters%c:%c%s%c,\n",0x0022,0x0022,0x0022,resourceparameters,0x0022);
			
		fprintf(h,"\t%cNumberOfResources%c:%c%d%c\n",0x0022,0x0022,0x0022,count,0x0022);	
		fprintf(h,"}\n");
		fclose(h);
		return( filename );
	}
}

public  char * cn_add_vs_request(
        char * name,        /* the display name for the workload element */
        char * provider,    /* the identity of the provider */
        char * region,      /* the name of the region within the provider */
        char * zone,        /* the name of the zone within the region */
        int size )          /* the size in GB for the volume */
{
	char *	filename;
	FILE *	h;

	if (!( filename = rest_temporary_filename("json")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else 
	{
		fprintf(h,"{\n");
		fprintf(h,"\t%cName%c:%c%s%c,\n",0x0022,0x0022,0x0022,name,0x0022);
		fprintf(h,"\t%cProviderId%c:%c%s%c,\n",0x0022,0x0022,0x0022,provider,0x0022);	
		fprintf(h,"\t%cRegion%c:%c%s%c,\n",0x0022,0x0022,0x0022,region,0x0022);
		fprintf(h,"\t%cZone%c:[%c%s%c],\n",0x0022,0x0022,0x0022,zone,0x0022);
		fprintf(h,"\t%cSizeInGBytes%c:%c%d%c\n",0x0022,0x0022,0x0022,size,0x0022);	
		fprintf(h,"}\n");
		fclose(h);
		return( filename );
	}
}


public  char * cn_start_transaction_request()
{
	char *	filename;
	FILE *	h;

	/* API does not require any content, return an empty file */
	if (!( filename = rest_temporary_filename("json")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else 
	{
		fprintf(h,"");
		fclose(h);
		return( filename );
	}
}

public  char * cn_stop_transaction_request() 
{
	char *	filename;
	FILE *	h;

	/* API does not require any content, return an empty file */
	if (!( filename = rest_temporary_filename("json")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else 
	{
		fprintf(h,"");
		fclose(h);
		return( filename );
	}
}


public	struct cn_config *   cn_initialise_client( 
		char * apikey, char * apisec, char * host, 
		char * agent, char * version, char * tls )
{
	struct cn_config * sptr=(struct cn_config *) 0;
	
	if (!( sptr = cn_allocate_config()))
		return( sptr );
	if (!( sptr->apikey = allocate_string( apikey )))
		return(cn_liberate_config( sptr ) );
	if (!( sptr->apisec = allocate_string( apisec )))
		return(cn_liberate_config( sptr ));
	if (!( sptr->host = allocate_string( host )))
		return(cn_liberate_config( sptr ));
	else if (!( sptr->agent = allocate_string( agent )))
		return(cn_liberate_config( sptr ));
	else if (!( sptr->version = allocate_string( version )))
		return(cn_liberate_config( sptr ));

	sptr->authentication = (char *) 0;
	if (!( tls ))
		sptr->tls = (char *) 0;
	else if ((sptr->tls = allocate_string(tls)) != (char *) 0)
		if ( (!( strlen( sptr->tls ) )) || ( *(sptr->tls) == '0' ) )
			sptr->tls = liberate( sptr->tls );

	return( sptr );
}

public  struct cn_config *  cn_liberate_config( struct cn_config * sptr )
{
	if ( sptr )
	{
		liberate( sptr );
	}
	return((struct cn_config *) 0);
}


        /* -------------- */
#endif  /* _cnclient_h    */
        /* -------------- */

