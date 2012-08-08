/* ------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                   */
/*  (C) 2011 by Iain James MARSHALL (Prologue) <ijm667@hotmail.com>    */  
/*      2012 Hamid MEDJAHED (Prologue) <hmedjahed@prologue.fr> 	       */
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
#include "azconfig.h"
#include "tlsconfig.h"
#include "tlsload.h"
#include "list.h"
#include "hmac_sha2.h"

char subscriptionid[]=" ";

private	struct az_config Waz = {
	(char *) 0,
	(char *) 0,

	(char *) 0,
	(char *) 0,

	(char *) 0,
	(char *) 0,

	(char *) 0,
	(char *) 0,

	"2012-03-01",
	_AZ_NS_COMPUTE,
	_AZ_BASE,
	subscriptionid,
	(char *) 0,
	(char *) 0,
	(char *) 0,

	0

	};

private struct az_vm_config Waz_vm=
{
 (char*) 0,      //id
 "myvmwid",      //vmname
 "AdminPass00",  //vmadmipw
 (char*) 0,      //zonetime
 (char*) 0,      //domainname
 (char*) 0,      //userdomain
 (char*) 0,      //passwduserd
 (char*) 0,      //domainjname
 (char *)0,      //distgname
 (char *)0,      //"Mystr",    //storename
 (char *)0,      //hostnamevm
 (char *)0,      //lusername
 (char *)0,      //lpassword
 0,              //ssh
 0,              //pubkeys
 (char *)0,      //certfingerprint
 (char *)0,      //certfingerprintkp
 (char *)0,      //pubkeypath
 (char *)0,      //keypairfingerprint
 (char *)0,      //keypairepath
 1, //0          //networkconfig
 (char *)0,      //lbendpoint
 "3389",         //localport
 "RemoteDesktop",//endpointname
 (char *)0,      //externalport
 0,              //lbprob
 (char *)0,      //pathstatus
 (char *)0,      //portstatus
 (char *)0,      //probprotocol
 "tcp",          //protocol
 0,              //subnet
 (char *)0,      //frontendsubnet
 (char *)0,      //backendsubnet
 (char *)0,      //vmavailability
 0,              //datadisks
 (char *)0,      //hostcaching
 (char *)0,      //disklabel
 (char *)0,      //diskname
 (char *)0,      //lun
 (char *)0,      //disksize
 (char *)0,      //medialink
 (char *)0,      //oshostcaching
 (char *)0,      //osdisklabel
 (char *)0,      //osdiskname
 (char *)0,      //"https://mystoragevm.blob.core.windows.net/vhds/mystorage-myvm.vhd",   //osmedialink
 "MSFT__Windows-Server-2012-RC-June2012-en-us-30GB.vhd",   //srimagename
 "Small",        //rolesize
 (char *)0,      //vnetworkname
 (char *)0,      //dnsname
 (char *)0       //dnsipaddress
};

struct vm_network Waz_net={
 (char*) 0,        //dnsname
 (char*) 0,        // ipv4address
 (char*) 0,        //localsitename
 (char*) 0,        //CIDRidentifier
 (char*) 0,        //IPV4addressvpngateway
 "VmTestNetnet",   //virtualnetworkname,
 "VmTest",         //affinitygroupname,
 "v2.0",           //label,
 "10.2.0.0/8",     //"10.4.0.0/16", //CIDRidentifieras,
 (char*) 0,        //subnetname,
 (char*) 0,        //CIDRidentifiersn,
 (char*) 0,        //primarydnsname,
 (char*) 0,        //"Small",       //gwprofile,
 (char*) 0,        //"VmLoc",       //localsitenamep
};

int isBlob=0;
int isBlobc=0;
int isTable=0;
int isTablec=0;
int isQue=0;
int isCstrg=0;

/*	------------------------------------------------------------	*/
/*		a z _ r a n d o m _ p a s s w o r d 			*/
/*	------------------------------------------------------------	*/
public	char *	az_random_password( char * pattern, int length )
{
	char 	buffer[72];
	int	i;
	int	seed;
	int	xseed;
	int	x;
	int	v;
	if ( length > 72 ) 
		length=70;
	else if ( length < 7 )
		length=10;
	seed = time((long) 0);
	xseed = seed ^ 0xFFFFFFFF;
	i = 0;
	while ( *pattern )
	{
		x += ( buffer[i] ^ *(pattern++));
	}		
	for (	i=0;
		i < length;
		i++)
	{
		if (!( i ))
			v = (((seed % 26) & 0x00FF) + 'A');
		else
		{
			v = ((seed % 36) & 0x00FF);
			if ( v > 9 )
				v = ((v - 10) + 'a');
			else	v += '0';
		}
		buffer[i] = v;
		seed /= v;
		seed *= x;
		seed ^= xseed;
	}
	buffer[length] = 0;
	printf("random password : %s \n",buffer);
	return(allocate_string(buffer));
}

/*	------------------------------------------------------------	*/
/*		l i b e r a t e _ a z _ r e s p o n s e		          	    */
/*	------------------------------------------------------------	*/
public	struct az_response * liberate_az_response( struct az_response * rptr )	
{
	if ( rptr )
	{
		if (rptr->response)
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
		else if (!( rptr->xmlroot = document_parse_file( aptr->body ) ))
			return( rptr );
		else
		{
			rptr->nature = _TEXT_XML;
			return( rptr );
		}
	}
}

/*	------------------------------------------------------------	*/
/*		 a z _ c l i e n t _ r e q u e s t		        */
/*	------------------------------------------------------------	*/
public	struct	az_response * az_client_request(char * target, char * tls, char * nptr,  char * filename,struct rest_header * hptr, char *method)
{
	if(strcmp(method,"GET")==0)
	{
	 return( az_check( rest_client_get_request( target, tls, nptr, hptr ) ) );
	}
	else if(strcmp(method,"HEAD")==0)
	{
	 return( az_check( rest_client_head_request( target, tls, nptr, hptr ) ) );
	}
	else if(strcmp(method,"DELETE")==0)
	{
	 return( az_check( rest_client_delete_request( target, tls, nptr, hptr ) ) );
	}
	else if(strcmp(method,"PUT")==0)
	{
	 return( az_check( rest_client_put_request( target, tls, nptr, filename, hptr ) ) );
	}
	else if(strcmp(method,"POST")==0)
	{
	 return( az_check( rest_client_post_request( target, tls, nptr, filename, hptr ) ) );
	}
	else
	{
	  return (struct az_response *)0;
	}
}

/*	------------------------------------------------------------	*/
/*		 a z _ c l i e n t _ g e t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	az_response * az_client_get_request(char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	return( az_check( rest_client_get_request( target, tls, nptr, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*	      a z _ c l i e n t _ d e l e t e _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	az_response * az_client_delete_request(char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	return( az_check( rest_client_delete_request( target, tls, nptr, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*		 a z _ c l i e n t _ h e a d _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	az_response * az_client_head_request(char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	return( az_check( rest_client_head_request( target, tls, nptr, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*		 a z _ c l i e n t _ p o s t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	az_response * az_client_post_request(char * target, char * tls, char * nptr, char * filename, struct rest_header * hptr )
{
	return( az_check( rest_client_post_request( target, tls, nptr, filename, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*		 a z _ c l i e n t _ p u t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	az_response * az_client_put_request(char * target, char * tls, char * nptr, char * filename, struct rest_header * hptr )
{
	return( az_check( rest_client_put_request( target, tls, nptr, filename, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*			a z _ a u t h e n t i c a t e ()	        */
/*	------------------------------------------------------------	*/
public	struct	rest_header   *	az_authenticate	( char * content_type, char * accepts )
{
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	struct	rest_header 	*	root=(struct rest_header * ) 0;
	struct	rest_header 	*	foot=(struct rest_header * ) 0;
	if (!( Waz.user ))
		return( hptr );
	else if (!( Waz.password ))
		return( hptr );
	else if (!( Waz.version ))
		return( hptr );
	else if (!( foot = root = rest_create_header( "x-ms-version", Waz.version ) ))
		return( root );
	if ( content_type )
	{
		if (!( hptr = rest_create_header( _HTTP_CONTENT_TYPE, content_type ) ))
			return( root );
		else 
		{
			hptr->previous = root;
			foot->next     = hptr;
			foot = hptr;
		}
	}
	if ( accepts )
	{
		if (!( hptr = rest_create_header( _HTTP_ACCEPT, accepts ) ))
			return( root );
		else 
		{
			hptr->previous = root;
			foot->next     = hptr;
			foot = hptr;
		}
	}
	return( root );
}


/*	------------------------------------------------------------	*/
/*		a z u r e  _ u p d a t e _ o p e r a t i o n		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	azure_update_operation( char * action, char * filename )
{
	struct	az_response	*	rptr=(struct az_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	if (!( hptr = az_authenticate("text/xml","application/xml") ))
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
	else if (!( rptr = az_client_put_request( nptr, Waz.tls, Waz.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		n e t w o r k _ u p d a t e _ o p e r a t i o n		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	network_update_operation( char * action, char * filename )
{
	struct	az_response	*	rptr=(struct az_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	if (!( hptr = az_authenticate("text/plain","application/xml") ))
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
	else if (!( rptr = az_client_put_request( nptr, Waz.tls, Waz.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
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
	if (!( hptr = az_authenticate("text/xml","application/xml") ))
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

	if (!( hptr = az_authenticate("text/xml","application/xml") ))
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

	if (!( hptr = az_authenticate("text/xml","application/xml") ))
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

	if (!( hptr = az_authenticate("text/xml","application/xml") ))
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
/* 	a z u r e  _ l i s t _ c o n t a i n e r		        */
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_list_container( char * myStorageAccount,char *mykeyStorage)
{
	char uri[1024]="?comp=list";
	char *contentBody=NULL;
	return (azure_storage_request(myStorageAccount,mykeyStorage,uri,"GET",contentBody));
}

/*	------------------------------------------------------------	*/
/*		a z u r e  _ d e l e t e _ c o n t a i n e r		*/
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_delete_container( char * myStorageAccount,char *mykeyStorage,char* containerName)
{
	char uri[1024];
	char *contentBody=NULL;
	sprintf(uri,"%s?restype=container",containerName);
	return (azure_storage_request(myStorageAccount,mykeyStorage,uri,"DELETE",contentBody));
}
/*	------------------------------------------------------------	*/
/*		a z u r e  _ c r e a t e _ c o n t a i n e r		*/
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_create_container( char * myStorageAccount,char *mykeyStorage,char* containerName)
{
	char uri[1024];
	char *contentBody=NULL;
	sprintf(uri,"%s?restype=container",containerName);
	return (azure_storage_request(myStorageAccount,mykeyStorage,uri,"PUT",contentBody));
}

/*	------------------------------------------------------------	*/
/*		 a z u r e  _ l i s t _ b l o b s	                */
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_list_blob( char * myStorageAccount,char *mykeyStorage,char*containerName)
{
	char uri[1024];
	char *contentBody=NULL;
	isBlob=1;
	sprintf(uri,"%s?restype=container&comp=list",containerName);
	return (azure_storage_request(myStorageAccount,mykeyStorage,uri,"GET",contentBody));
}


/*	------------------------------------------------------------	*/
/*		a z u r e  _c r e a t e _ b l o b 		        */
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_create_blob( char * myStorageAccount,char *mykeyStorage,char*containerName,char * blobName, char *content)
{
	char uri[1024];
	char *contentBody=NULL;
	isBlobc=1;
	isBlob=1;
	if (!( contentBody=allocate_string(content) ))
		return((struct az_response *) 0);
	else
	{
		sprintf(uri,"%s/%s",containerName,blobName);
		return (azure_storage_request(myStorageAccount,mykeyStorage,uri,"PUT",contentBody));
	}
}

/*	------------------------------------------------------------	*/
/*		a z u r e _d e l e t e _ b l o b_           		*/
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_delete_blob( char * myStorageAccount,char *mykeyStorage,char*containerName,char * blobName)
{
	char uri[1024];
	char *contentBody=NULL;
	isBlob=1;
	sprintf(uri,"%s/%s",containerName,blobName);
	return (azure_storage_request(myStorageAccount,mykeyStorage,uri,"DELETE",contentBody));
}
/*	------------------------------------------------------------	*/
/*		a z u r e _g e t _ b l o b_c o n t e n t 	        */
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_get_blob_content( char * myStorageAccount,char *mykeyStorage,char*containerName,char * blobName)
{
	char uri[1024];
	char *contentBody=NULL;
	isBlob=1;
	sprintf(uri,"%s/%s",containerName,blobName);
	return (azure_storage_request(myStorageAccount,mykeyStorage,uri,"GET",contentBody));
}

/*	------------------------------------------------------------	*/
/*		a z _ c r e a te _  b l o b_ c o n t e n t   		*/
/*	------------------------------------------------------------	*/
public	char * az_create_blob_content(char *contentBody)
{
	char *filename;
	FILE *	h;
	if (!( filename = rest_temporary_filename("txt")))
		return filename;
	else if (!( h = fopen( filename,"w" ) ))
		return (char *)( liberate( filename ) );
	else
	{
		fprintf(h,"%s",contentBody);
		fclose(h);
		return filename;
	}
}

/*	------------------------------------------------------------	*/
/*		a z u r e  _ l i s t _ t a b l e		        */
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_list_table( char * myStorageAccount,char *mykeyStorage)
{
	char uri[1024];
	char *contentBody=NULL;
	isTable=1;
	sprintf(uri,"Tables");
	return (azure_storage_request(myStorageAccount,mykeyStorage,uri,"GET",contentBody));
}

/*	------------------------------------------------------------	*/
/*		a z u r e  _ c r e a t e _ t a b l e		        */
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_create_table( char * myStorageAccount,char *mykeyStorage,char *tablename)
{
	char uri[1024];
	char *contentBody=NULL;
	isTable=1;
	isTablec=1;
	sprintf(uri,"Tables");
	if (!( contentBody=allocate_string(tablename) ))
		return((struct az_response *) 0);
	else	return (azure_storage_request(myStorageAccount,mykeyStorage,uri,"POST",contentBody));
}

/*	------------------------------------------------------------	*/
/*		a z _ c r e a te _  t a b l e _ b o d y	                */
/*	------------------------------------------------------------	*/
public	char * az_create_table_request_body( char *tablename)
{
	char *	filename;
	FILE *	h;
	char * now;
	now=allocate_string(timestampo());
	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"w" ) ))
		return (char *)( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
		fprintf(h,"<entry xmlns:d=\"http://schemas.microsoft.com/ado/2007/08/dataservices\" xmlns:m=\"http://schemas.microsoft.com/ado/2007/08/dataservices/metadata\" xmlns=\"http://www.w3.org/2005/Atom\">\n");
		fprintf(h,"\t<title />\n");
		fprintf(h,"\t<updated>%s</updated>\n",now);
		fprintf(h,"\t<author>\n");
        	fprintf(h,"\t\t<name/>\n"); 
        	fprintf(h,"\t</author>\n"); 
        	fprintf(h,"\t<id/>\n"); 
        	fprintf(h,"\t<content type=\"application/xml\">\n");
        	fprintf(h,"\t\t<m:properties>\n");
        	fprintf(h,"\t\t\t<d:TableName>%s</d:TableName>\n",tablename);
        	fprintf(h,"\t\t</m:properties>\n");
        	fprintf(h,"\t</content>\n"); 
        	fprintf(h,"</entry>\n");
		fclose(h);
		return( filename );
	}
}

/*	------------------------------------------------------------	*/
/*		a z u r e  _ d e l e t e _ t a b l e		        */
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_delete_table( char * myStorageAccount,char *mykeyStorage,char *tablename)
{
	char uri[1024];
	char *contentBody=NULL;
	isTable=1;
	sprintf(uri,"Tables('%s')",tablename);
	return (azure_storage_request(myStorageAccount,mykeyStorage,uri,"DELETE",contentBody));
}


/*	------------------------------------------------------------	*/
/*		a z u r e  _ q u e u e_ t a b l e		        */
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_list_queue( char * myStorageAccount,char *mykeyStorage)
{
	char uri[1024];
	char *contentBody=NULL;
	isQue=1;
	sprintf(uri,"?comp=list");
	return (azure_storage_request(myStorageAccount,mykeyStorage,uri,"GET",contentBody));
}

/*	------------------------------------------------------------	*/
/*		a z u r e  _ c r e a t e_ t a b l e		        */
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_create_queue( char * myStorageAccount,char *mykeyStorage,char *name)
{
	char uri[1024];
	char *contentBody=NULL;
	isQue=1;
	sprintf(uri,"%s",name);
	return (azure_storage_request(myStorageAccount,mykeyStorage,uri,"PUT",contentBody));
}

/*	------------------------------------------------------------	*/
/*		a z u r e  _ d e l e t e_ t a b l e		        */
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_delete_queue( char * myStorageAccount,char *mykeyStorage,char *name)
{
	char uri[1024];
	char *contentBody=NULL;
	isQue=1;
	sprintf(uri,"%s",name);
	return (azure_storage_request(myStorageAccount,mykeyStorage,uri,"DELETE",contentBody));
}

/*	------------------------------------------------------------	*/
/*		a z u r e  _ l i s t_ s t o r a g e_ a c c o u t        */
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_list_storage_account()
{
	char uri[1024];
	char *contentBody=NULL;
	sprintf(uri,"/services/storageservices");
	return (azure_service_management_request(uri,"GET",contentBody));
}

/*	------------------------------------------------------------	*/
/*		a z u r e  _ c r e a t e_ s t o r a g e_ a c c o u n t  */
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_create_storage_account(char *strgname,char *location, char *affigroupe)
{
	char uri[1024];
	char *contentBody=NULL;
	char label[]="v1.0.0";
	char description[]="storageaccountdescription";
	char *filename;
	isCstrg=1;
	if (!( filename=az_create_storage_account_request_body(strgname,label,description,location,affigroupe) ))
		return((struct az_response *) 0);
	else
	{
		sprintf(uri,"/services/storageservices");
		return (azure_service_management_request(uri,"POST",filename));
	}
}

/*	-------------------------------------------------------------------------	*/
/*		a z _ c r e a te _  s t o r a g e _ a c c o u n t _ b o d y	        */
/*	-------------------------------------------------------------------------	*/
public	char * az_create_storage_account_request_body( char * name,char * label,char * description,char * location,char * group )
{
	char *	filename;
	FILE *	h;
	char 	buffer[1024];
	int	n;
	n = EncodeBase64( buffer, label,strlen(label));
	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"w" ) ))
		return (char *)( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<CreateStorageServiceInput xmlns=%c%s%c>\n",0x0022,Waz.namespace,0x0022);
		fprintf(h,"\t<ServiceName>%s</ServiceName>\n",name);
		fprintf(h,"\t<Label>%s</Label>\n",buffer);
		fprintf(h,"\t<Description>%s</Description>\n",description);
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
/*	 a z u r e  _ d e l e t e_ s t o r a g e_ a c c o u n t		*/
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_delete_storage_account(char *strgname)
{
	char uri[1024];
	char *contentBody=NULL;
	
	sprintf(uri,"/services/storageservices/%s",strgname);
	return (azure_service_management_request(uri,"DELETE",contentBody));
}

/*	----------------------------------------------------------------	*/
/*		a z u r e  _ g e t_ s t o r a g e_ a c c o u n t _ k e y s      */
/*	----------------------------------------------------------------	*/
public	struct	az_response * azure_get_storage_account_keys(char *strgname)
{
	char uri[1024];
	char *contentBody=NULL;
	
	sprintf(uri,"/services/storageservices/%s/keys",strgname);
	return (azure_service_management_request(uri,"GET",contentBody));
}

/*	-----------------------------------------------------------------------------	*/
/*		a z u r e  _ r e g e r a t e_ s t o r a g e_ a c c o u n t _ k e y s    */
/*      keytype = Primary|Secondary                                                     */
/*	-----------------------------------------------------------------------------	*/
public	struct	az_response * azure_regenerate_storage_account_key(char *strgname,char *keytype)
{
	char uri[1024];
	char *contentBody=NULL;
	char *filename;
	isCstrg=1;
	if (!( filename=az_regenerate_storage_account_key_request_body(keytype) ))
		return((struct az_response *) 0);
	else
	{
		sprintf(uri,"/services/storageservices/%s/keys?action=regenerate",strgname);
		return (azure_service_management_request(uri,"POST",filename));
	}
}

/*	-----------------------------------------------------------------------------------  */
/*		a z _ r e g e n e r a t e _  s t o r a g e _ a c c o u n t _ k e y_b o d y   */
/*	-----------------------------------------------------------------------------------  */
public	char * az_regenerate_storage_account_key_request_body( char * keytype )
{
	char *	filename;
	FILE *	h;
	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"w" ) ))
		return (char *)( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<RegenerateKeys xmlns=%c%s%c>\n",0x0022,Waz.namespace,0x0022);
		fprintf(h,"\tKeyType>%s</KeyType>\n",keytype);
		fprintf(h,"</RegenerateKeys>\n");
		fclose(h);
		return( filename );
	}
}

/*	------------------------------------------------------------*/
/*		a z u r e  _ c r e a t e_ d e p l o y e m e n t     */
/*	------------------------------------------------------------*/
/* sevicename: the name of your service                             */
/* deploymentslot:staging or production, depending on where you     */
/*			wish to deploy your service package         */
/* deployname: The name for the deployment                          */
/* pkgUrl: A URL that refers to the location of the service package */ 
/*        in the Blob service                                       */
/* configfile: The service configuration file for the deployment.   */
/*             It must be Base64-encoded.                           */
/* label: A label for this deployment that is Base64-encoded        */
/* startdeploy: true or false.Indicates whether to start the        */
/*              deployment immediately after it is created          */
/*	------------------------------------------------------------*/
public	struct	az_response * azure_create_deployment(
	char *servicename, 
	char *deploymentslot, 
	char *deployname, 
	char *pkgUrl, 
	char *configfile, 
	char *label, 
	char *startdeploy)
{
	char uri[1024];
	char *contentBody=NULL;
	char *filename;
	isCstrg=1;
	if (!( filename=az_create_deployment_request_body(deployname,pkgUrl,label,configfile,startdeploy) ))
		return((struct az_response *) 0);
	else
	{
		sprintf(uri,"/services/hostedservices/%s/deploymentslots/%s",servicename,deploymentslot);
		return (azure_service_management_request(uri,"POST",filename));
	}
}

/*	------------------------------------------------------------------	*/
/*	a z _ c r e a t e _  d e p l o y m e n t _ r e q u e s t _ b o d y	*/
/*	------------------------------------------------------------------	*/
public char* az_create_deployment_request_body(char *deployname,char*pkgUrl,char *label,char *configfile, char *startdeploy)
{
	char *	filename;
	FILE *	h;
	char 	buffer[1024];
	char 	bufferc[1024];
	int	n;
	n = EncodeBase64( buffer, label,strlen(label));
	n = EncodeBase64( bufferc, configfile,strlen(configfile));
	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"w" ) ))
		return (char *)( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
		fprintf(h,"<CreateDeployment xmlns=\"http://schemas.microsoft.com/windowsazure\">\n");
		fprintf(h,"\t<Name>%s</Name>\n",deployname);
		fprintf(h,"\t<PackageUrl>%s</PackageUrl>\n",pkgUrl);
		fprintf(h,"\t<Label>%s</Label>\n",buffer);
		fprintf(h,"\t<Configuration>%s</Configuration>\n",bufferc);
		fprintf(h,"\t<StartDeployment>%s</StartDeployment>\n",startdeploy);
		fprintf(h,"</CreateDeployment>\n");
		fclose(h);
	  	return( filename );
	}
}

/*	------------------------------------------------------------	*/
/*		a z u r e  _ d e l e t e_ d e p y m e n t               */
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_delete_deployement_slots(char *servicename, char *deploymentslot)
{
	char uri[1024];
	char *contentBody=NULL;
	sprintf(uri,"/services/hostedservices/%s/deploymentslots/%s",servicename,deploymentslot);
	return (azure_service_management_request(uri,"DELETE",contentBody));
}

/*	------------------------------------------------------------	*/
/*		a z u r e  _ d e l e t e_ d e p y m e n t               */
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_delete_deployement(char *servicename, char *deploymentslot)
{
	char uri[1024];
	char *contentBody=NULL;
	sprintf(uri,"/services/hostedservices/%s/deployments/%s",servicename,deploymentslot);
	return (azure_service_management_request(uri,"DELETE",contentBody));
}

/*	------------------------------------------------------------	*/
/*		a z u r e  _ l i s t_ a f f i n i t y g r o u p s       */
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_list_affinitygroups()
{
	char uri[1024];
	char *contentBody=NULL;
	sprintf(uri,"/affinitygroups");
	return (azure_service_management_request(uri,"GET",contentBody));
}

/*	------------------------------------------------------------	*/
/*		a z u r e  _ c r e a t e_ a f f i n i t y g r o u p s	*/
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_create_affinitygroups(char * name,char * label,char * description,char * location)
{
	char uri[1024];
	char *contentBody=NULL;
	char *filename;
	isCstrg=1;
	if (!( filename=az_create_affinity_group_request_body(name,label,description,location ) ))
		return((struct az_response *) 0);
	else
	{
		sprintf(uri,"/affinitygroups");
		return (azure_service_management_request(uri,"POST",filename));
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
	else if (!( h = fopen( filename,"wa" ) ))
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

/*	--------------------------------------------------------------------------- */
/*	a z _ c r e a t e _  a f f i n i t y _ g r o u p _ r e q u e s t_ b o d y   */
/*	--------------------------------------------------------------------------- */
public	char * az_create_affinity_group_request_body(
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
	else if (!( h = fopen( filename,"w" ) ))
		return (char *)( liberate( filename ) );
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
/*	------------------------------------------------------------	*/
/*		a z u r e  _ d e l e t e_ a f f i n i t y g r o u p     */
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_delete_affinitygroup(char *name)
{
	char uri[1024];
	char *contentBody=NULL;
	sprintf(uri,"/affinitygroups/%s",name);
	return (azure_service_management_request(uri,"DELETE",contentBody));
}

/*	------------------------------------------------------------	*/
/*		a z u r e  _ l i s t_ l o c a t i o n s                 */
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_list_locations()
{
	char uri[1024];
	char *contentBody=NULL;
	sprintf(uri,"/locations");
	return (azure_service_management_request(uri,"GET",contentBody));
}

/*	------------------------------------------------------------	*/
/*		a z u r e  _ l i s t_ o s_ i m a g e s                  */
/*	------------------------------------------------------------	*/
public	struct	az_response * azure_list_os_image()
{
	char uri[1024];
	char *contentBody=NULL;
	sprintf(uri,"/services/images");
	return (azure_service_management_request(uri,"GET",contentBody));
}

/*	----------------------------------------------------------------------------*/
/*		a z u r e  _ a d d _ o s _ i m a g e		                    */
/*	----------------------------------------------------------------------------*/
/* name: Specifies a name for the OS image that Windows Azure uses to identify the  */
/*       image when creating one or more virtual machines.                          */
/* label:Specifies the friendly name of the image.                                  */
/* medialink: Specifies the location of the blob in Windows Azure                   */
/*               blob store where the media for the image is located.               */
/*               Example: http://example.blob.core.windows.net/disks/mydisk.vhd     */
/* os: The operating system type of the OS image.                                   */
/*       Possible values are:Linux / Windows                                        */
/*	----------------------------------------------------------------------------*/
public	struct	az_response * azure_add_os_image(
	char * name,
	char * label,
	char *medialink,
	char * os)
{
	char uri[1024];
	char *contentBody=NULL;
	char *filename;
	isCstrg=1;
	if (!( filename=az_create_os_image_request_body(name,label,medialink,os ) ))
		return((struct az_response *) 0);
	else
	{
		sprintf(uri,"/services/images");
		return (azure_service_management_request(uri,"POST",filename));
	}
}

/*	------------------------------------------------------------------	*/
/*	a z _ c r e a t e _  o s _ i m a g e _ r e q u e s t_ b o d y   	*/
/*	------------------------------------------------------------------	*/
public char *az_create_os_image_request_body(char * name,char * label,char *medialink,char * os)
{
  char *	filename;
	FILE *	h;

	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"w" ) ))
		return (char *)( liberate( filename ) );
	else
	{
		fprintf(h,"<OSImage xmlns=\"http://schemas.microsoft.com/windowsazure\" xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\">\n");
		fprintf(h,"\t<Label>%s</Label>\n",label);
        	fprintf(h,"\t<MediaLink>%s</MediaLink>\n",medialink);
        	fprintf(h,"\t<Name>%s</Name>\n",name);
        	fprintf(h,"\t<OS>%s</OS>\n",os);
        	fprintf(h,"</OSImage>\n");
		fclose(h);
		return (filename);
	}
}

/*	----------------------------------------------------------------------	*/
/*		a z u r e  _ d e l e t e_ o s _ i m a g e                       */
/*	----------------------------------------------------------------------	*/
/*  name:the image to delete                                                    */
/*  label: Specifies the friendly name of the image to delete                   */
/*	----------------------------------------------------------------------	*/
public	struct	az_response * azure_delete_os_image(char *name, char *label)
{
	char uri[1024];
	char *contentBody=NULL;
	char *filename;
	if (!( filename=az_delete_os_image_request_body(label) ))
		return((struct az_response *) 0);
	else
	{
		isCstrg=1;
		sprintf(uri,"/services/images/%s",name);
		return (azure_service_management_request(uri,"DELETE",filename));
	}
}

/*	------------------------------------------------------------------	*/
/*	a z _ d e le t e _  o s _ i m a g e _ r e q u e s t_ b o d y     	*/
/*	------------------------------------------------------------------	*/
public char *az_delete_os_image_request_body(char * label)
{
  char *	filename;
	FILE *	h;
	char 	buffer[1024];
	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"w" ) ))
		return (char *)( liberate( filename ) );
	else
	{
		fprintf(h,"<OSImage xmlns=\"http://schemas.microsoft.com/windowsazure\" xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\">\n");
		fprintf(h,"\t<Label>%s</Label>\n",label);
		fprintf(h,"</OSImage>\n");
		fclose(h);
		return (filename);
	}
}

/*	----------------------------------------------------------------------------	*/
/*		a z u r e  _ u p d a t e_ o s _ i m a g e		                */
/*	----------------------------------------------------------------------------	*/
/* name: Specifies a name for the OS image that Windows Azure uses to identify the      */
/*       image when creating one or more virtual machines.                              */
/* label:Specifies the friendly name of the image.                                      */
/* medialink: Specifies the location of the blob in Windows Azure                       */
/*               blob store where the media for the image is located.                   */
/*               Example: http://example.blob.core.windows.net/disks/mydisk.vhd         */
/* os: The operating system type of the OS image.                                       */
/*       Possible values are:Linux / Windows                                            */
/*	-------------------------------------------------------------------------------	*/
public	struct	az_response * azure_update_os_image(
	char * name,
	char * label,
	char *medialink,
	char * os)
{
	char uri[1024];
	char *contentBody=NULL;
	char *filename;
	isCstrg=1;
	if (!( filename=az_create_os_image_request_body(name,label,medialink,os )))
		return((struct az_response *) 0);
	else
	{
		sprintf(uri,"/services/images/%s",name);
		return (azure_service_management_request(uri,"PUT",filename));
	}
}

/* -------------------------------------------------------------------------------*/
/*   a z u r e  _ c r e a t e_ V i r t u a l M a c h i n e _ D e p l o y m e n t  */
/* -------------------------------------------------------------------------------*/
public	struct	az_response * azure_create_vm_deployment(
	char * sname,
	char * dname,
	char * label,
	char * vnetworkname,
	char * vmname,
	char * provisioningconfig)
{
	char uri[1024];
	char *contentBody=NULL;
	char *filename;
	isCstrg=1;
	isTable=1;
	if (!( filename=az_create_vm_request_body(dname,label,vnetworkname,vmname,provisioningconfig) ))
		return((struct az_response *) 0);
	else
	{
		sprintf(uri,"/services/hostedservices/%s/deployments/",sname);
		return (azure_service_management_request(uri,"POST",filename));
	}
}

/* ------------------------------------------------------------------	*/
/*  a z _ c r e a t e _  v m _ r e q u e s t_ b o d y   	        */
/* ------------------------------------------------------------------	*/
public char *az_create_vm_request_body(char *name,char *label,char *vnetworkname,char *vmname, char *provisioningconfig)
{
    char *	filename;
	FILE *	h;
	char 	buffer[1024];
	int	n;
	n = EncodeBase64( buffer, label,strlen(label));
	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	if (!( h = fopen( filename,"w" ) ))
		return (char *)( liberate( filename ) );
	else
	{ 
          fprintf(h,"<Deployment xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://schemas.microsoft.com/windowsazure\">\n");
	  fprintf(h,"\t<DeploymentSlot>Production</DeploymentSlot>\n");
	  //Required. A name for the deployment. The deployment name must be unique among other deployments for the hosted service
          fprintf(h,"\t<Name>%s</Name>\n",name);

	  //Required. A name for the hosted service that is base-64 encoded. The name can be up to 100 characters in length. 
          fprintf(h,"\t<Label>%s</Label>\n",buffer); 

	  //Required. Contains the provisioning details for the new virtual machine deployment.
          fprintf(h,"\t<RoleList>\n");

	  //Required. Contains the configuration sets that are used to provision the deployment
          fprintf(h,"\t\t<Role>\n");
          fprintf(h,"\t\t\t<RoleName>%s</RoleName>\n",vmname);
          fprintf(h,"\t\t\t<RoleType>PersistentVMRole</RoleType>\n"); 

	  //Required. Contains a collection of configuration sets that contain system and application configuration settings
          fprintf(h,"\t\t\t<ConfigurationSets>\n");

	  //Required. You must specify Either a WindowsProvisioningConfigurationSet or LinuxProvisioningConfigurationSet configuration set
          fprintf(h,"\t\t\t\t<ConfigurationSet>\n");
	  if(provisioningconfig)
	  {
	     if(strcmp(provisioningconfig,"windows")==0)
	     {
		//Include a WindowsProvisioningConfigurationSet (Windows vm metadata)
                fprintf(h,"\t\t\t\t\t<ConfigurationSetType>WindowsProvisioningConfiguration</ConfigurationSetType>\n");

		if(Waz_vm.vmname)
		{
		   //Optional.Specifies the computer name for the virtual machine
                   if(Waz_vm.vmname)fprintf(h,"\t\t\t\t\t<ComputerName>%s</ComputerName>\n",Waz_vm.vmname);

		   //Required. Specifies the base-64 encoded string representing the administrator password 
		   //to use for the virtual machine
                   if(Waz_vm.vmadmipw)
		   {
			 n = EncodeBase64( buffer, Waz_vm.vmadmipw,strlen(Waz_vm.vmadmipw));
			 fprintf(h,"\t\t\t\t\t<AdminPassword>%s</AdminPassword>\n",buffer);
		   }

		   //Optional. Specifies whether the user must change the administrator password on first logon. 
		   //The default value is false
                   fprintf(h,"\t\t\t\t\t<ResetPasswordOnFirstLogon>false</ResetPasswordOnFirstLogon >\n");

		   //Optional. Specifies whether automatic updates are enabled for the virtual machine. 
		   //The default value is true
                   fprintf(h,"\t\t\t\t\t<EnableAutomaticUpdate>true</EnableAutomationUpdate>\n");  

		   //Optional. Specifies the time zone for the virtual machine
                   if(Waz_vm.zonetime)fprintf(h,"\t\t\t\t\t<TimeZone>%s</TimeZone>\n",Waz_vm.zonetime);
		}

		if(Waz_vm.domainname)
		{
                   fprintf(h,"\t\t\t\t\t<DomainJoin>\n");
                   fprintf(h,"\t\t\t\t\t\t<Credentials>\n");
		   //Optional. Specifies the name of the domain used to authenticate an account. The 
		   //value is a fully qualified DNS domain
                   fprintf(h,"\t\t\t\t\t\t\t<Domain>%s</Domain>\n",Waz_vm.domainname);

		   //Required. Specifies a user name in the domain that can be used to join the domain.
                   fprintf(h,"\t\t\t\t\t\t\t<Username>%s</Username>\n",Waz_vm.userdomain);

		   //Required. Specifies the password to use to join the domain.
                   fprintf(h,"\t\t\t\t\t\t\t<Password>%s</Password>\n",Waz_vm.passwduserd);

                   fprintf(h,"\t\t\t\t\t\t</Credentials>\n");

		   //Optional. Contains properties that specify a domain to which the virtual machine will be joined
                   if(Waz_vm.domainjname)fprintf(h,"\t\t\t\t\t\t<JoinDomain>%s</JoinDomain>\n",Waz_vm.domainjname);

		   //Optional. Specifies the Lightweight Directory Access Protocol 
                   //(LDAP) X 500-distinguished name of the organizational unit (OU) 
	           //in which the computer account is created
		   //Example:OU=MyOu,OU=MyParentOu,DC=example.com,DC=MyCompany,DC=com
                   if(Waz_vm.distgname)fprintf(h,"\t\t\t\t\t\t<MachineObjectOU>%s<MachineObjectOU>\n",Waz_vm.distgname);
                   fprintf(h,"\t\t\t\t\t</DomainJoin>\n");
		}
        
		if(Waz_vm.certfingerprint)
		{
		    //Required. Contains a list of service certificates with which to provision to the new virtual machine.
		    fprintf(h,"\t\t\t\t\t<StoredCertificateSettings>\n");
		    fprintf(h,"\t\t\t\t\t\t<CertificateSetting>\n");

		    //Required Specifies the target certificate store location on the virtual machine.
		    //The only supported value is LocalMachine.
		    fprintf(h,"\t\t\t\t\t\t\t<StoreLocation>LocalMachine</StoreLocation>\n");

		    //Required Specifies the name of the certificate store from which retrieve certificate. 
		    fprintf(h,"\t\t\t\t\t\t\t<StoreName>%s</StoreName>\n",Waz_vm.storename);

		    //Required Specifies the thumbprint of the certificate to be provisioned. The thumbprint must 
		    //specify an existing service certificate.
		    fprintf(h,"\t\t\t\t\t\t\t<Thumbprint>%s</Thumbprint>\n",Waz_vm.certfingerprint);
		    fprintf(h,"\t\t\t\t\t\t</CertificateSetting>\n");
		    fprintf(h,"\t\t\t\t\t</StoredCertificateSettings>\n");
		}
	     }
	     else if(strcmp(provisioningconfig,"linux")==0)
	     {
                //Include a LinuxProvisioningConfigurationSet (Linux vm metadata)
                fprintf(h,"\t\t\t\t\t<ConfigurationSetType>LinuxProvisioningConfiguration</ConfigurationSetType>\n");

	        //Required Specifies the host name for the VM
                fprintf(h,"\t\t\t\t\t<HostName>%s</HostName>\n",Waz_vm.hostnamevm);

                //Required Specifies the name of a user to be created in the sudoer group of the virtual machine
                fprintf(h,"\t\t\t\t\t<UserName>%s</UserName>\n",Waz_vm.lusername); 

                //Required Specifies the associated password for the user name
                fprintf(h,"\t\t\t\t\t<UserPassword>%s</UserPassword>\n",Waz_vm.lpassword);

	        //Optional. Specifies whether or not SSH password authentication is disabled
                fprintf(h,"\t\t\t\t\t<DisableSshPasswordAuthentication>false</DisableSshPasswordAuthentication>\n");   

	        //Optional. Specifies the SSH public keys and key pairs to populate in the image during provisioning
	        if(Waz_vm.ssh)
	        {
                   fprintf(h,"\t\t\t\t\t<SSH>\n");

		   //Optional. Specifies the collection of SSH public keys
		   if(Waz_vm.pubkeys)
		   {
                      fprintf(h,"\t\t\t\t\t\t<publicKeys>\n");

		      //Required.Specifies the public key
                      fprintf(h,"\t\t\t\t\t\t\t<publicKey>\n");
		 
		      //Required.. Specifies the SHA1 fingerprint of an X509 certificate associated with the hosted service 
                      //that includes the SSH public key
                      fprintf(h,"\t\t\t\t\t\t\t\t<FingerPrint>%s</FingerPrint>\n",Waz_vm.certfingerprint);

		      //Required. Specifies the full path of a file, on the virtual machine, which stores the SSH public key
		      //example: /home/user/.ssh/authorized_keys
                      fprintf(h,"\t\t\t\t\t\t\t\t<Path>%s</Path>\n",Waz_vm.pubkeypath);    
                      fprintf(h,"\t\t\t\t\t\t\t</publicKey>\n");
                      fprintf(h,"\t\t\t\t\t\t</publicKeys>\n");
		   }
		  
                   //Required. Contains a collection of SSH key pairs.
                   fprintf(h,"\t\t\t\t\t\t<KeyPairs>\n");

		   //Required. Contains an SSH key pair to be installed on the virtual machine
                   fprintf(h,"\t\t\t\t\t\t\t<KeyPair>\n");

		   //Required. Specifies the SHA1 fingerprint of an X509 certificate associated with the hosted service that 
                   //includes the SSH keypair
                   fprintf(h,"\t\t\t\t\t\t\t\t<FingerPrint>certfingerprint</FinguerPrint>\n",Waz_vm.certfingerprintkp);

		   //Required. Specifies the full path of a file, on the virtual machine, which stores the SSH private key. 
		   //The file is overwritten when multiple keys are written to it
                   fprintf(h,"\t\t\t\t\t\t\t\t<Path>%s</Path>\n",Waz_vm.keypairepath);
                   fprintf(h,"\t\t\t\t\t\t\t</KeyPair>\n");
                   fprintf(h,"\t\t\t\t\t\t</KeyPairs>\n");
                   fprintf(h,"\t\t\t\t\t</SSH>\n");
	        }
	     }
	  }
	  fprintf(h,"\t\t\t\t</ConfigurationSet>\n");

	  //Optional. Encapsulates the metadata required to create the virtual network configuration for a virtual machine.  
          if(Waz_vm.networkconfig) 
	  {
		   
	     fprintf(h,"\t\t\t\t<ConfigurationSet>\n");
	     fprintf(h,"\t\t\t\t\t<ConfigurationSetType>NetworkConfiguration</ConfigurationSetType>\n");
             if(Waz_vm.endpointname)
	     {
	        fprintf(h,"\t\t\t\t\t<InputEndpoints>\n");

                //Optional. Contains a collection of external endpoints for the virtual machine
                fprintf(h,"\t\t\t\t\t\t<InputEndpoint>\n");
	        fprintf(h,"\t\t\t\t\t\t\t<EnableDirectServerReturn>false</EnableDirectServerReturn>\n");

                //Required. Specifies a name for a set of load-balanced endpoints. Specifying this element for a given 
                //endpoint adds it to the set
	        if(Waz_vm.lbendpoint)fprintf(h,"\t\t\t\t\t\t\t<LoadBalancedEndpointSetName>%s</LoadBalancedEndpointSetName>\n",
                        Waz_vm.lbendpoint);

	        //Required. Specifies the internal port on which the virtual machine is listening to serve the endpoint.
	        fprintf(h,"\t\t\t\t\t\t\t<LocalPort>%s</LocalPort>\n",Waz_vm.localport);

	        //Required. Specifies the name for the external endpoint
	        fprintf(h,"\t\t\t\t\t\t\t<Name>%s</Name>\n",Waz_vm.endpointname);

	        //Required. Specifies the external port to use for the endpoint
	        if(Waz_vm.externalport)fprintf(h,"\t\t\t\t\t\t\t<Port>%s</Port>\n",Waz_vm.externalport);

	        //Optional. Contains properties that specify the endpoint settings which the Windows Azure load balancer 
	        //uses to monitor the availability of this virtual machine before forwarding traffic to the endpoint.
	        if(Waz_vm.lbprob)
	        {
		    fprintf(h,"\t\t\t\t\t\t\t<LoadBalancerProbe>\n");

		    //Optional. Specifies the relative path name to inspect to determine the virtual machine availability status. 
		    //If Protocol is set to TCP, this value must be NULL.
		    fprintf(h,"\t\t\t\t\t\t\t\t<Path>%s</Path>\n",Waz_vm.pathstatus);

		    //Optional. Specifies the port to use to inspect the virtual machine availability status.
		    fprintf(h,"\t\t\t\t\t\t\t\t<Port>%s</Port>\n",Waz_vm.portstatus);

		    //Optional. Specifies the transport protocol for the endpoint. Possible values are:
		    //HTTP/TCP
		    fprintf(h,"\t\t\t\t\t\t\t\t<Protocol>%s</Protocol>\n",Waz_vm.probprotocol);
		    fprintf(h,"\t\t\t\t\t\t\t</LoadBalancerProbe>\n");
	        }

	        //Required. Specifies the protocol to use to inspect the virtual machine availability status.
		//Possible values are:TCP|UDP 
		fprintf(h,"\t\t\t\t\t\t\t<Protocol>%s</Protocol>\n",Waz_vm.protocol);                   
		fprintf(h,"\t\t\t\t\t\t</InputEndpoint>\n");
		fprintf(h,"\t\t\t\t\t</InputEndpoints>\n");
	    }
	  
	    //Optional. Specifies the name of a subnet to which the virtual machine belongs
            if(Waz_vm.subnet)
	    {
	        fprintf(h,"\t\t\t\t\t<SubnetNames>\n");
		fprintf(h,"\t\t\t\t\t\t<string>%s</string>\n",Waz_vm.frontendsubnet);
		fprintf(h,"\t\t\t\t\t\t<string>%s</string>\n",Waz_vm.backendsubnet);
		fprintf(h,"\t\t\t\t\t</SubnetNames>\n");
            }
            fprintf(h,"\t\t\t\t</ConfigurationSet>\n");
	  }

          fprintf(h,"\t\t\t</ConfigurationSets>\n");
          if(Waz_vm.vmavailability)fprintf(h,"\t\t\t<AvailabilitySetName>%s</AvailabilitySetName>\n",Waz_vm.vmavailability);
	  if(Waz_vm.datadisks)
	  {
	     //Optional. Contains the parameters Windows Azure uses to create a data disk for a virtual machine
             fprintf(h,"\t\t\t<DataVirtualHardDisks>\n");

	     //Required. Contains the properties used to create a data disk.
             fprintf(h,"\t\t\t\t<DataVirtualHardDisk>\n");

	     //Optional. Specifies the caching behavior of the data disk for read/write efficiency. The default value is None
	     //possible value: //None/ReadOnly|ReadWrite
             if(Waz_vm.hostcaching)fprintf(h,"\t\t\t\t\t<HostCaching>%s</HostCaching>\n",Waz_vm.hostcaching);

	     //Optional. Specifies the description for the data disk in the image repository
             if(Waz_vm.disklabel)fprintf(h,"\t\t\t\t\t<DiskLabel>%s</DiskLabel>\n",Waz_vm.disklabel); 

	     //Optional. Specifies an image in the image repository to use to create the data.
	     //When you attach a data disk to a virtual machine, you must specify at least one 
	     //of the following elements: DiskName or SourceMediaLink or LogicalDiskSize
             if(Waz_vm.diskname)fprintf(h,"\t\t\t\t\t<DiskName>%s</DiskName>\n",Waz_vm.diskname);

	     //Required. Specifies the Logical Unit Number (LUN) for the data disk
	     //Valid LUN values are 0 through 15.
             fprintf(h,"\t\t\t\t\t<Lun>%s</Lun>\n",Waz_vm.lun); 

	     //Optional. Specifies the size, in GB, of an empty disk to be attached to the virtual machine
             if(Waz_vm.disksize)fprintf(h,"\t\t\t\t\t<LogicalDiskSizeInGB>%s</LogicalDiskSizeInGB>\n",Waz_vm.disksize); 

	     //Optional. Specifies the location of the physical blob that contains the data disk. 
	     //This link refers to a blob in your Windows Azure storage account
	     //example :http://example.blob.core.windows.net/disks/mydatadisk.vhd
             if(Waz_vm.medialink)fprintf(h,"\t\t\t\t\t<MediaLink>%s</MediaLink>\n",Waz_vm.medialink);

             fprintf(h,"\t\t\t\t</DataVirtualHardDisk>\n");
             fprintf(h,"\t\t\t</DataVirtualHardDisks>\n");
	  }

	  //Required. Contains the parameters Windows Azure uses to create the operating system disk for the virtual machine.
          fprintf(h,"\t\t\t<OSVirtualHardDisk>\n");
	  
	  //Optional. Specifies whether the OS disk can be cached for greater efficiency during writes.
	  // posssible value: ReadOnly|ReadWrite   
          if(Waz_vm.oshostcaching)fprintf(h,"\t\t\t\t<HostCaching>%s</HostCaching>\n",Waz_vm.oshostcaching); 

	  //Optional. Specifies the friendly name of the disk containing the guest OS image in the image repository
          if(Waz_vm.osdisklabel)fprintf(h,"\t\t\t\t<DiskLabel>%s</DiskLabel>\n",Waz_vm.osdisklabel);

	  //Optional.Specifies the name of an operating system image in the image repository.
          if(Waz_vm.osdiskname)fprintf(h,"\t\t\t\t<DiskName>%s</DiskName>\n",Waz_vm.osdiskname); 

	  //Optional. Specifies the URI for a blob in a Windows Azure storage account that contains the 
	  //OS image to use to create the OS disk.
          //Example: http://contoso.blob.core.windows.net/disks/mydisk.vhd
          if(Waz_vm.osmedialink)fprintf(h,"\t\t\t\t<MediaLink>%s</MediaLink>\n",Waz_vm.osmedialink);

	  //Optional. Specifies the name of the disk image to use to create the virtual machine.
          if(Waz_vm.srimagename)fprintf(h,"\t\t\t\t<SourceImageName>%s</SourceImageName>\n",Waz_vm.srimagename);
          fprintf(h,"\t\t\t</OSVirtualHardDisk>\n");    

	  //required. Specifies the role size 
	  //possible value: ExtraSmall|Small|Medium|Large|ExtraLarge 
          fprintf(h,"\t\t\t<RoleSize>%s</RoleSize>\n",Waz_vm.rolesize);     
          fprintf(h,"\t\t</Role>\n");
          fprintf(h,"\t</RoleList>\n");

	  //Required. Specifies the name of an existing virtual network to which the deployment will belong
          if(vnetworkname) fprintf(h,"\t<VirtualNetworkName>%s</VirtualNetworkName>\n",vnetworkname);

	  //Optional. Contains a list of DNS servers to associate with the machine
	  if(Waz_vm.dnsname)
	  {
            fprintf(h,"\t<Dns>\n");
            fprintf(h,"\t\t<DnsServers>\n");
            fprintf(h,"\t\t\t<DnsServer>\n");
            fprintf(h,"\t\t\t\t<Name>%s</Name>\n",Waz_vm.dnsname);
            fprintf(h,"\t\t\t\t<Address>%s</Address>\n",Waz_vm.dnsipaddress);
            fprintf(h,"\t\t\t</DnsServer>\n");
            fprintf(h,"\t\t</DnsServers>\n");
            fprintf(h,"\t</Dns>\n");
	  }
          fprintf(h,"</Deployment>\n");

	  fclose(h);
	  return (filename);
	}
}

/*	----------------------------------------------------------------------------	*/
/*		a z u r e  _ s e t_ n e t w o r k _ c o n f i g u r a t i o n           */
/*	----------------------------------------------------------------------------	*/
public	struct	az_response * azure_set_network_configuration()
{
	char uri[1024];
	char *contentBody=NULL;
	char *filename;
	isCstrg=1;
	filename=az_ceate_network_configuration_request_body();
	sprintf(uri,"/services/networking/media");
	return (azure_service_management_request(uri,"POST",filename));
}

/*	------------------------------------------------------------------	*/
/*	a z _ c r e a t e _  v m _ r e q u e s t_ b o d y   	                */
/*	------------------------------------------------------------------	*/
public char *az_ceate_network_configuration_request_body()
{
    char *	filename;
	FILE *	h;
	char buffer[1024];
	int	n;
	
	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	if (!( h = fopen( filename,"w" ) ))
		return (char *)( liberate( filename ) );
	else
	{ 
	   fprintf(h,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
           fprintf(h,"<NetworkConfiguration xmlns=\"http://schemas.microsoft.com/ServiceHosting/2011/07/NetworkConfiguration\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\n");
	   fprintf(h,"\t<VirtualNetworkConfiguration>\n"); 

	   /*---------------------------------------------DNS-------------------------------------------*/
	   //Optional. Contains the collection of DNS servers.
	   if(Waz_net.dnsname)
	   {
              fprintf(h,"\t\t<Dns>\n");
              fprintf(h,"\t\t\t<DnsServers>\n");

	      //Required. Specifies the name of the DNS server. DNS server names must be unique the configuration.
	      //Required. Specifies the IPv4 address of the DNS server
              fprintf(h,"\t\t\t\t<DnsServer name=\"%s\" IPAddress=\"%s\"/>\n",Waz_net.dnsname,Waz_net.ipv4address);
              fprintf(h,"\t\t\t</DnsServers>\n");
              fprintf(h,"\t\t</Dns>\n");
	   /*-------------------------------------end-DNS-------------------------------------------------*/
	   }
	   else
	   {
	     fprintf(h,"\t\t<Dns />\n");
	   }
	   /*--------------------------------------LocalNetworkSites--------------------------------------*/
	   //Optional. Contains the collection parameters used to configure local network sites 
	   //used in cross-premise network configurations
	   if(Waz_net.localsitename)
	   {
               fprintf(h,"\t\t<LocalNetworkSites>\n");

	       //Required. Specifies the identifier for the local network.
               fprintf(h,"\t\t\t<LocalNetworkSite name=\"%s\">\n",Waz_net.localsitename);

	       /*-------------------------------------AddressSpace---------------------------------------------*/
	       //Required. Contains a collection of Classless Inter-Domain Routing (CIDR) identifiers 
	       //that specify the address space that you will use for your local network site
       	       fprintf(h,"\t\t\t\t<AddressSpace>\n");
	       //Required. Specifies a CIDR identifier that identifies the address space.
               fprintf(h,"\t\t\t\t\t<AddressPrefix>%s</AddressPrefix>\n",Waz_net.CIDRidentifier);
               fprintf(h,"\t\t\t\t</AddressSpace>\n");
	       /*----------------------------------end-AddressSpace---------------------------------------------*/
	       /*------------------------------------VPNGatewayAddress------------------------------------------*/
	       //Required. Specifies the IPv4 Address of the VPN Gateway. You can specify only 
	       //one IP address per local network site.
               fprintf(h,"\t\t\t\t<VPNGatewayAddress>%s</VPNGatewayAddress>\n",Waz_net.IPV4addressvpngateway);
	       /*-------------------------------end-VPNGatewayAddress--------------------------------------------*/
               fprintf(h,"\t\t\t</LocalNetworkSite>\n");    
       	       fprintf(h,"\t\t</LocalNetworkSites>\n");
	   }
	   /*-------------------------------end-LocalNetworkSite-------------------------------------------*/

	   /*----------------------------------------VirtualNetworkSites-----------------------------------*/
	   //Optional. Contains the collection of your virtual networks. Each virtual network is referred 
	   //to as a site
	   if(Waz_net.virtualnetworkname)
	   {
               fprintf(h,"\t\t<VirtualNetworkSites>\n");

	       //Required. Specifies a name for the virtual network. The name must be unique within the subscription
	       //Required. The name of the affinity group that you want this virtual network site to be associated with.
               fprintf(h,"\t\t\t<VirtualNetworkSite name=\"%s\" AffinityGroup=\"%s\">\n",Waz_net.virtualnetworkname,Waz_net.affinitygroupname);

	       //Required. Specifies a friendly identifier for the virtual network
	       //n = EncodeBase64( buffer, Waz_net.label,strlen(Waz_net.label));
               //fprintf(h,"\t\t\t\t<Label>%s</Label>\n",Waz_net.label);
	   
	       /*--------------------------------------AddressSpace---------------------------------------------*/
	       if(Waz_net.CIDRidentifieras)
	       {
                    fprintf(h,"\t\t\t\t<AddressSpace>\n");

	            //Required. Specifies the address space for the virtual network
                    fprintf(h,"\t\t\t\t\t<AddressPrefix>%s</AddressPrefix>\n",Waz_net.CIDRidentifieras);
                    fprintf(h,"\t\t\t\t</AddressSpace>\n");
	       }
	       /*---------------------------------end---AddressSpace---------------------------------------------*/

	       /*--------------------------------------Subnets--------------------------------------------------*/
	       //Optional. Contains the specification for the subnets that you want to create within the address 
	       //space of your virtual network sites
	       if(Waz_net.subnetname)
	       {
                   fprintf(h,"\t\t\t\t<Subnets>\n");
                   fprintf(h,"\t\t\t\t\t<Subnet name=\"%s\">\n",Waz_net.subnetname);
                   fprintf(h,"\t\t\t\t\t\t<AddressPrefix>%s</AddressPrefix>\n",Waz_net.CIDRidentifiersn);
                   fprintf(h,"\t\t\t\t\t</Subnet>\n");
                   fprintf(h,"\t\t\t\t</Subnets>\n");
	       }
	       else
	       {
		   fprintf(h,"\t\t\t\t<Subnets />\n");
               }
	       /*----------------------------------end-Subnets--------------------------------------------------*/

	       /*--------------------------------------DnsServersRef----------------------------------------------*/
	       //Optional. Contains the specification of DNS servers that are used for name 
	       if(Waz_net.primarydnsname)
	       {
                   fprintf(h,"\t\t\t\t<DnsServersRef>\n");
                   fprintf(h,"\t\t\t\t\t<DnsServerRef name=\"%s\" />\n",Waz_net.primarydnsname);
                   fprintf(h,"\t\t\t\t</DnsServersRef>\n");
	       }
	       /*---------------------------------end-DnsServersRef-----------------------------------------------*/

	       /*--------------------------------------Gateway----------------------------------------------------*/
	       if(Waz_net.gwprofile)
	       {
	           //Required for cross-premises connections only. Contains gateway references 
	           //to the local network sites that the virtual network can connect to.
	           //Required. Specifies the gateway connection size. The only supported value is Small.
                   fprintf(h,"\t\t\t\t<Gateway profile=\"%s\">\n",Waz_net.gwprofile);

	           //Required for cross-premises connections only. Contains the list of local networks 
	           //that the virtual network can connect to.
                   fprintf(h,"\t\t\t\t\t<ConnectionsToLocalNetwork>\n");

	           //Required. Specifies the name of the local network. 
	           //Required. Specifies the name of the local network
                   fprintf(h,"\t\t\t\t\t\t<LocalNetworkSiteRef name=\"%s\" />\n",Waz_net.localsitenamep);
                   fprintf(h,"\t\t\t\t\t</ConnectionsToLocalNetwork>\n");
                   fprintf(h,"\t\t\t\t</Gateway>\n");
	       }
	       /*------------------------------------end-Gateway---------------------------------------------------*/
               fprintf(h,"\t\t\t</VirtualNetworkSite>\n");
               fprintf(h,"\t\t</VirtualNetworkSites>\n");
	   }
	   /*---------------------------------end-VirtualNetworkSites-------------------------------------------*/
           fprintf(h,"\t</VirtualNetworkConfiguration>\n");
           fprintf(h,"</NetworkConfiguration>\n");
	   fclose(h);
	   return (filename);
	}
}

/*	----------------------------------------------------------------------------	*/
/*		a z u r e  _ g e t_ n e t w o r k _ c o n f i g u r a t i o n           */
/*	----------------------------------------------------------------------------	*/
public	struct	az_response * azure_get_network_configuration()
{
	char uri[1024];
	char *contentBody=NULL;
	sprintf(uri,"/services/networking/media");
	return (azure_service_management_request(uri,"GET",contentBody));
}

/*	-------------------------------------------------------------------------	*/
/*		a z u r e  _ s e r v i c e _ m a n a g e m e n t _ r e q u e s t        */
/*	--------------------------------------------------------------------------	*/
public	struct	az_response *azure_service_management_request(char *uri,char *method,char *contentBody)
{
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	struct	az_response	*	rptr=(struct az_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	char *filename;
	char *timedate;
	char *tls;
	char base[1024];

	if(isCstrg) 
		filename=contentBody;
	else 	filename=allocate_string("\n");

	sprintf(base,"https://management.core.windows.net/%s",Waz.subscription);
	Waz.base=allocate_string(base);

	tls=allocate_string("azureTls.xml");

	if (!( hptr = rest_create_header( "x-ms-version",(Waz.version ? Waz.version : "")) ))
		return( rptr );
	else if (!( hptr->next = rest_create_header( "Content-Type","text/xml") ))
		return( rptr );
	else	hptr->next->previous = hptr;

	if (!( uptr = analyse_url( Waz.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, uri) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = az_client_request( nptr, tls, Waz.agent, filename,hptr,method) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}
/*------------------------------------------------------------	*/
/*		a z u r e  _ s t o r a g e _ r e q u e s t      */
/*------------------------------------------------------------	*/
public	struct	az_response * azure_storage_request(
		char * myStorageAccount,
		char * mykeyStorage,
		char *uri,
		char *method,
		char *contentBody)
{
	struct	rest_header 	*	foot=(struct rest_header * ) 0;
	struct	rest_header 	*	root=(struct rest_header * ) 0;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	struct	az_response	*	rptr=(struct az_response *) 0;
	struct	az_response	*	rptrb=(struct az_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	char *filename;
	int lengthb=0;
	char contentL[20]="0";
	listc headerParam;
	char   base[1024];
	char   urii[1024];
	char Wazbase[1024];
	char *timedate;

	if (!( filename=allocate_string("\n") ))
		return( rptr );

	else if (isBlob)
	{
		sprintf(Wazbase,"blob.%s",_AZ_BASE);
	}
	else if (isTable)
	{
		sprintf(Wazbase,"table.%s",_AZ_BASE);
	}
	else if (isQue)
	{
		sprintf(Wazbase,"queue.%s",_AZ_BASE);
	}
	else	Wazbase[0] = 0;

	if (!( Waz.base=allocate_string(Wazbase) ))
		return( rptr );		
	else	sprintf(base,"http://%s.%s", myStorageAccount,Waz.base);

	sprintf(urii,"/%s",uri);


	if(isBlobc)
	{
		if (!(root = rest_create_header("x-ms-blob-type", "BlockBlob")))
			return( rptr );
		else	foot = root;
		filename=az_create_blob_content(contentBody);
		lengthb=strlen(contentBody);
		sprintf(contentL,"%d",lengthb);
	}

	if(timedate=allocate_string(timestamp()))
	{
		if (!( hptr = rest_prefix_header(foot, "x-ms-date",timedate) ))
			return( rptr );
		else	root = hptr;
	}

	if (!( hptr = rest_postfix_header( root, "x-ms-version",( Waz.version ? Waz.version : " "))))
		return( rptr );
	else	foot = hptr;
	
	if (!( hptr = rest_postfix_header(foot, "Authorization", 
			AuthorizationHeader(method, timedate, uri,root, myStorageAccount, mykeyStorage,lengthb,"" , "" ))))
		return( rptr );
	else	foot = hptr;
	
	if(lengthb)
	{
		if (!( hptr = rest_postfix_header( foot, "Accept-Charset", "UTF-8") ))
			return( rptr );
		else	foot = hptr;
	}

	if (isTable)
	{
		if (!( hptr = rest_postfix_header( foot,"Content-Type","application/atom+xml")))
			return( rptr );
		else	foot = hptr;
		if (!( hptr = rest_postfix_header( foot,"DataServiceVersion", "1.0;NetFx")))
			return( rptr );
		else	foot = hptr;
		if (!( hptr = rest_postfix_header( foot,"MaxDataServiceVersion", "1.0;NetFx")))
			return( rptr );
		else	foot = hptr;
	  	if(isTablec)
			filename= az_create_table_request_body(contentBody);
    	}

	if ((!(strcmp(method,"PUT") )) || (!( strcmp(method,"DELETE") )))
	{
		if (!( hptr = rest_postfix_header( foot,"Content-Length",contentL)))
			return( rptr );
		else	foot = hptr;
	}
	
	if(!(hptr = root ))
		return (rptr);
	else if (!( uptr = analyse_url( base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, urii) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = az_client_request( nptr, Waz.tls, Waz.agent, filename,hptr,method) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/************************************************************************************************/
/* Generate Autorization header                                                                 */
/************************************************************************************************/
public char* AuthorizationHeader(char *method, 
                                 char* now, 
                                 char *uri,
                                 struct rest_header *hptr, 
                                 char* myStorageAccount, 
                                 char * mykeyStorage,
                                 int lengthb,
                                 char* ifMatch , 
                                 char* md5 )
{
	char MessageSignature[10240];
	char authoHead[10240];
	char buffer[1024];
	int bytes=0;
	char *ahptr;
	char        *p;
	char *CanonicalizedHeaders;
	char *CanonicalizedResource;
	char contentLength[]="";
	char contentL[20]="0";
	char digest[BUFSIZ];
	
	char *response;
	FILE * exp_file;
	FILE *f;

	if((strcmp(method,"GET")==0 || strcmp(method,"HEAD")==0))
	{
	 sprintf(contentL,"%s",contentLength);
	}
	else
	{
	 sprintf(contentL,"%d",lengthb);
	}

	
	CanonicalizedHeaders=allocate_string(getCanonicalizedHeaders(hptr));
	CanonicalizedResource=getCanonicalizedResource(uri,myStorageAccount);

	if (isTable)
        {
           sprintf(MessageSignature,"%s\n\n%s\n%s\n%s",method,"application/atom+xml",now,CanonicalizedResource);
        }
	else
	{
	  sprintf(MessageSignature,"%s\n\n\n%s\n%s\n\n\n\n%s\n\n\n\n%s%s",method,contentL,md5,
                            ifMatch,CanonicalizedHeaders,CanonicalizedResource);
	}

	f=fopen("C:\\Mes_dev\\Azheaderc.txt","w");
	fprintf(f,"%s",MessageSignature);
	fclose(f);

	f=fopen("C:\\Mes_dev\\Azsignaturec.txt","w");
	fprintf(f,"%s",MessageSignature);
	fclose(f);

	 DecodeBase64(mykeyStorage, buffer, &bytes );
	 buffer[bytes] = 0;
	 response=signature_hmac(MessageSignature,buffer);
	 sprintf(authoHead,"SharedKey %s:%s",myStorageAccount,response);
	 ahptr=allocate_string(authoHead);
	
	 return ahptr;
}

/************************************************************************************************/
/* Generate Canonicalized header                                                                */
/************************************************************************************************/
char* getCanonicalizedHeaders(struct rest_header *hptr)
{
	char canonicalheaderp[256];
	char canonicalheadern[256]=" ";
	char canonicalheaderf[256];
	char canonicalheader[256];
	char start[]="";
	char *rptr;
	char **tabstr;
	listc elparam;
	elem *pelem;
	int i,l;
	int count=0;
	resetList(&elparam);
	while(hptr)
	{
		if(searchWord("x-ms-",hptr->name)==1)
		{
			for(i=0;hptr->value[i]!=0;i++)
			{
			  	if (( hptr->value[i] =='\r' )
				&&  ( hptr->value[(i+1)] == '\n')) 
					hptr->value[i]=start[0];
			}
			sprintf(canonicalheaderp,"%s:%s",hptr->name,hptr->value);
			addFront(&elparam,canonicalheaderp);
			count++;
	 	}
	 	 hptr=hptr->next;
	}
	tabstr=(char**)malloc(count*sizeof(char*));
	for(l=0;l<count;l++) 
		tabstr[l]=(char*)malloc(256*sizeof(char));
	pelem=elparam.first;
	l=0;
 
	while(pelem)
	{
		strcpy(tabstr[l],pelem->value);
		pelem=pelem->next;
		l++;
	}
	tri_iteratif(tabstr,count);
	
	for(l=0;l<count;l++)
	{
		sprintf(canonicalheaderp,"%s\n",tabstr[l]);
		sprintf(canonicalheadern,"%s%s",canonicalheadern,canonicalheaderp);
	}

	if ( canonicalheadern )
		str_sub(canonicalheadern,1,strlen(canonicalheadern),canonicalheaderf);

	sprintf(canonicalheader,"%s",canonicalheaderf);
	rptr=allocate_string(canonicalheader);
	return rptr;
}

/************************************************************************************************/
/* Generate Canonicalized Resource                                                              */
/************************************************************************************************/
char *	getCanonicalizedResource(char *uri,char *myAccountStorage)
{
  char CanonicalizedResourcep[256];
  char CanonicalizedResourcepp[256];
  char *CanonicalizedResource;
  char start[]="";
  char uriff[256];
  char urif[256];
  char urip[256];
  char uripp[256];
  char valpp[256];
  char **tabstr;
  listc elparam;
  listc elvalue;
  listc elglobal;
  listc elglobaln;
  int i,l;
  int j=0;
  int fst=0,fs=0;
  int count=0;
  elem *pelem;
  elem *velem;
  elem *gelem;
 
  char * token;

  resetList(&elparam);
  resetList(&elvalue);
  resetList(&elglobal);
  resetList(&elglobaln);

  i=indexOfchar(uri,'?');
  if(i>0) 
  {
	  str_sub(uri,0,i-1,uriff);
	  sprintf(urif,"/%s/%s",myAccountStorage,uriff);
  }
  else if(i==0)
  {
    sprintf(urif,"/%s/",myAccountStorage);
  }
  else
  {
    sprintf(urif,"/%s/%s",myAccountStorage,uri);
    return(allocate_string(urif));
  }

  if(i>=0)
  {
   str_sub(uri,i+1,strlen(uri),urip);
   token= strtok(urip,"&");
   for(;token != NULL;)
   {
     addBack(&elglobal,token);
     token=strtok(NULL,"&");
	 count++;
   }
  }

  tabstr=(char**)malloc(count*sizeof(char*));
  for(l=0;l<count;l++) tabstr[l]=(char*)malloc(256*sizeof(char));
  gelem=elglobal.first;
  l=0;
 
   while(gelem)
   {
	   strcpy(tabstr[l],gelem->value);
	   gelem=gelem->next;
	   l++;
   }
  tri_iteratif(tabstr,count);
  for(l=0;l<count;l++)
  {
   addBack(&elglobaln,tabstr[l]);
  }
  
  gelem=elglobaln.first;
  while(gelem)
  {
	 
     token=strtok(gelem->value,"=");
	 for(;token != NULL;)
     {
         if(j==0)
		 {
		   addBack(&elparam,token);
		   j=1;
		
		 }
		 else
		 {
		   addBack(&elvalue,token);
		   j=0;
		 }
         
		 token=strtok(NULL,"=");
     }
    gelem=gelem->next;
  }
 
  pelem=elparam.first;
  velem=elvalue.first;
  if(pelem) strcpy(valpp,pelem->value);
  while(pelem)
  {
	
     if(strcmp(pelem->value,valpp)==0 && fs==1)
	 {
	    sprintf(uripp,"%s,%s",uripp,velem->value);
	 }
	 else
	 {
	  if(fst==0)
	  {
	   sprintf(uripp,"%s:%s",pelem->value,velem->value);
	   fst=1;
	   fs=1;
	  }
	  else
	  {
	    sprintf(uripp,"%s\n%s:%s",uripp,pelem->value,velem->value);
	  }
	 }
	strcpy(valpp,pelem->value);
	pelem=pelem->next;
	velem=velem->next;

  }
  sprintf(CanonicalizedResourcep,"%s%s\n%s",start,urif,uripp);
  str_sub(CanonicalizedResourcep,0,strlen(CanonicalizedResourcep),CanonicalizedResourcepp);
  CanonicalizedResource=allocate_string(CanonicalizedResourcepp);
  return CanonicalizedResource;
}

/**************************************************************************************************/
/* function to sort a table of strings                                                            */
/**************************************************************************************************/
void tri_iteratif(char *tableau[], int taille)
{
   char *temp; 
   int i, j;

   for (i = 0; i < taille; i++) 
    for (j = 0; j < taille; j++)
      if (strcmp(tableau[i], tableau[j]) < 0)
        {
          temp = tableau[i];
          tableau[i] = tableau[j];
          tableau[j] = temp;
        }
}

/**************************************************************************************************/
/* Function to locate the index of a caratere in a string                                         */
/**************************************************************************************************/
int indexOfchar(char str[],char c)
{
 char *pos = strchr (str, c); 
 int posi = pos ? pos - str : -1; 
 return posi;
}

/**************************************************************************************************/
/* Function to search a word in a string                                                          */
/* mot: char * word name                                                                          */
/* line: char * the string                                                                        */
/* return 1 if the word exist otherwise 0                                                         */
/**************************************************************************************************/
int searchWord(char *mot,char *line)
{
    char car = 0;
    long i = 0;
    int j=0;

    while(car != '\n')
    {
	car = line[j];
	if(mot[i] == car)	
	{
              i++;
	      if(mot[i] == 0 || line[j]=='\n') 
	      {
		     return 1; // on a trouvé, le mot on retourne 1
	      }		
        }
        else
	{
	   i = 0;
	}
      j++;
     }

    return 0;	// si on a rien trouvé, on retourne 0
}
/******************************************************************************************************************************/
/* Function to extract a substring from a string                                                                              */
/* s: (char*) string sourcs                                                                                                   */
/* start: (int) the extracted string start                                                                                    */
/* end: (int) the extracted string end                                                                                        */
/* new_s: (char*) the extracted string                                                                                        */
/******************************************************************************************************************************/
void str_sub(const char *s, unsigned int start, unsigned int end, char new_s[])
{
 if (s && (start < end))
 {
 /* (1)*/
 int i;
 /* (2)*/
  for (i = start; i <= end; i++)
  {
   /* (3)*/   new_s[i-start] = s[i];
  }
 }
 new_s[end-start+1]=0;
}

/***************************************************************/
/*Generate UTC time                                            */
/***************************************************************/
char *timestamp()
{    
	char ms[10];
	char jr[10];
	char jj[10];
	char hr[10];
	char yr[10];
	char tmp[256];
	char *tmpstp;
	time_t ltime; /* calendar time */  
	struct tm *local;

	ltime=time(NULL);   
	local = gmtime(&ltime);
	sprintf(tmp,"%s",asctime(local));
	sscanf(tmp,"%s %s %s %s %s",jr,ms,jj,hr,yr);
	sprintf(tmp,"%s, %s %s %s %s GMT",jr,jj,ms,yr,hr);
	tmpstp=allocate_string(tmp);
    	return tmpstp;
}

/********************************************************************/
/* Function for calculating ISO 8601 Date formats                   */
/********************************************************************/
char *timestampo()
{
#if WIN32 || WIN64
   char buf[500];
   char *tmp;
   SYSTEMTIME tm;
   GetSystemTime(&tm);
   sprintf(buf, "%i-%02i-%02iT%02i:%02i:%02i.%03iZ", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
   tmp=allocate_string(buf);
   return tmp;
#else // defined(UNIX)
   struct tm tm;
   char tmp[120];
   char *tmps;
   time_t t;
   time(&t);
   localtime_r(&t,&tm); 
   sprintf(tmp, "%i-%02i-%02iT%02i:%02i:%02i", tm.tm_year + 1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
   tmps=allocate_string(tmp);
   return tmps;
#endif
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
/*	     a z _ d e l e t e _ a f f i n i t y _ g r o u p s	        */
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
/*	     a z _ c r e a t e _ s t o r a g e _ s e r v i c e 	        */
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
/*		a z _ g e t _ o p e r a t i o n _ s t a t u s		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_get_operation_status(char * opid)
{
	char buffer[2048];
	sprintf(buffer,"/operations/%s",opid);
	return( azure_retrieve_operation( buffer ) );
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
	char 	url[2048];
	sprintf(url,"/services/WATM/profiles/%s/definitions",profile );
	return( azure_list_operation( url ) );
}

/*	------------------------------------------------------------	*/
/*	     a z _ c r e a t e _ h o s t e d _ s e r v i c e 		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_create_hosted_service( char * filename )
{
	char 	uri[2048];
	sprintf(uri,"/services/hostedservices");
	return (azure_create_operation(uri,filename));
}

/*	------------------------------------------------------------	*/
/*	   	a z _ g e t _ h o s t e d _ s e r v i c e 		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_get_hosted_service( char * hostname )
{
	char 	uri[2048];
	sprintf(uri,"/services/hostedservices/%s",hostname);
	return (azure_retrieve_operation(uri));
}

/*	------------------------------------------------------------	*/
/*	     a z _ u p d a t e _ h o s t e d _ s e r v i c e 		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_update_hosted_service( char * hostname, char * filename )
{
	char 	uri[2048];
	sprintf(uri,"/services/hostedservices/%s",hostname);
	return (azure_update_operation(uri,filename));
}

/*	------------------------------------------------------------	*/
/*	     a z _ d e l e t e _ h o s t e d _ s e r v i c e 	        */
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_delete_hosted_service( char * hostname )
{
	char 	uri[2048];
	sprintf(uri,"/services/hostedservices/%s",hostname);
	return (azure_delete_operation(uri));
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
	char	buffer[1024];
	sprintf(buffer,"/services/hostedservices/%s/certificates/%s" , Waz.hostedservice, filename ); 
	return( azure_retrieve_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*	     a z _ d e l e t e _ c e r t i f i c a t e 			*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_delete_certificate( char * filename )
{
	char	buffer[1024];
	sprintf(buffer,"/services/hostedservices/%s/certificates/%s" , Waz.hostedservice,filename ); 
	return( azure_delete_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*		a z _ l i s t _ c e r t i f i c a t e s		        */
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_list_certificates()
{
	char	buffer[1024];
	sprintf(buffer,"/services/hostedservices/%s/certificates" , Waz.hostedservice ); 
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
/*		a z _ r e t r i e v e _ l o c a t i o n s 		*/
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_retrieve_location(char * nptr)
{
	char	buffer[2048];
	sprintf(buffer,"/locations/%s",nptr);
	return( azure_list_operation( nptr ) );
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
/*		a z _ c r e a te _  i m a g e _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	char * az_create_image_request(char * identity, char * server )
{
	char *	filename;
	FILE *	h;

	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return ( liberate( filename ) );
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
	if (!( hptr = az_authenticate("text/xml","application/xml") ))
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
/*		a z _ c r e a te _  s e r v e r _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
/*									*/
/*	The creation of a VM Role is Quite a Complex Message		*/
/*									*/
/*	There are quite a lot of optional parameters that will need	*/
/*	some kind of value but for the moment the parameters passed	*/
/*	have been minimalised as follows:				*/
/*									*/
/*		deployment : the deployment group			*/
/*		role   : the machine name or role			*/
/*		image  : the azure storage name of the image		*/
/*		flavor : small etc					*/
/*		network: the virtual network identity			*/
/*		zone   : the availablility set 				*/
/*		option : options to control the deployment message	*/
/*									*/
/*	------------------------------------------------------------	*/
public	char * az_create_vm_request(
	/* 	struct os_subscription * subptr,	*/
	char * deployment,	/* the identity of the server 	*/
	char * role,		/* the contract ID of the server*/
	char * image,		/* the server image identifier  */
	char * media,		/* the media link to the image  */
	char * flavor,		/* the server machine flavour	*/
	char * network,		/* the network groul		*/
	char * zone,		/* an eventual locality zone	*/
	int    option )		/* option flags for staging 	*/
{
	char *	hostname=(char *) 0;
	FILE *	h=(FILE *) 0;
	char *	newdeployment=(char *) 0;
	char 	buffer[1024];
	char *	filename=(char *) 0;
	char *	password=(char *) 0;
	/* ------------------------------------- */
	/* validate or allocate the unique label */
	/* ------------------------------------- */
	if (!( deployment ))
	{
		if (!( deployment = rest_allocate_uuid() ))
			return( deployment );
		else	newdeployment = deployment;
	}

	/* --------------------------------- */
	/* and then base 64 encode the deployment */
	/* --------------------------------- */
	(void) EncodeBase64( buffer, deployment,strlen(deployment));

	/* ---------------------------------- */
	/* build the VM creation request file */
	/* ---------------------------------- */
	if (!( filename = rest_temporary_filename("xml")))
	{
		if ( newdeployment ) liberate( newdeployment );
		return( filename );
	}
	else if (!( h = fopen( filename,"wa" ) ))
	{
		if ( newdeployment ) liberate( newdeployment );
		return( liberate( filename ) );
	}
	else
	{
	/* ----------------------------------- */
	/* generate the content of the request */
	/* ----------------------------------- */
	/* first the deployment control header */
	/* ----------------------------------- */
	fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
	fprintf(h,"<Deployment xmlns=%chttp://schemas.microsoft.com/windowsazure%c\n",0x0022,0x0022);

		fprintf(h,"\txmlns:i=%chttp://www.w3.org/2001/XMLSchema-instance%c>\n",0x0022,0x0022);
		/* --------------------------- */
		/* the identity must be unique */
		/* --------------------------- */
		fprintf(h,"\t<Name>%s</Name>\n",deployment);
		if ( option & _AZURE_IS_PRODUCTION )
			fprintf(h,"\t<DeploymentSlot>Production</DeploymentSlot>\n");
		else	fprintf(h,"\t<DeploymentSlot>Staging</DeploymentSlot>\n");
		fprintf(h,"\t<Label>%s</Label>\n",buffer);      

		/* --------------------------------- */
		/* and now the description of the VM */
		/* --------------------------------- */
		fprintf(h,"\t<RoleList>\n");
		fprintf(h,"\t<Role>\n");

			/* --------------------------------------- */
			/* VM configuration is described by a Role */
			/* --------------------------------------- */
			fprintf(h,"\t\t<RoleName>%s</RoleName>\n",role);
			fprintf(h,"\t\t<RoleType>PersistentVMRole</RoleType>\n");
			fprintf(h,"\t\t<ConfigurationSets>\n");
			fprintf(h,"\t\t<ConfigurationSet>\n");
			/* ------------------------------------- */
			/* determine the VM type windows | linux */
			/* ------------------------------------- */
			if (!( option & _AZURE_IS_WINDOWS ))
			{
				/* ------------------------------------ */
				/* LINUX SPECIFIC Configuration Section */
				/* ------------------------------------ */
				fprintf(h,"\t\t<ConfigurationSetType>LinuxProvisioningConfiguration</ConfigurationSetType>\n");
				fprintf(h,"\t\t<HostName>%s</HostName>\n",role);
				fprintf(h,"\t\t<UserName>%s</UserName>\n",Waz.user);
				fprintf(h,"\t\t<UserPassword>%s</UserPassword>\n",Waz.password);
				fprintf(h,"\t\t<DisableSshPasswordAuthentication>%s</DisableSshPasswordAuthentication>\n",
					(option & _AZURE_LINUX_SSH ? "true" : "false") );

				/* ----------------------------------- */
				/* TODO: handle eventual SSH Key Pairs */
				/* ----------------------------------- */
				if ( option & _AZURE_LINUX_SSH )
				{	fprintf(h,"\t\t<SSH/>");	}
			}
			else
			{
				/* -------------------------------------- */
				/* WINDOWS SPECIFIC Configuration Section */
				/* -------------------------------------- */
				fprintf(h,"\t\t<ConfigurationSetType>WindowsProvisioningConfiguration</ConfigurationSetType>\n");
				fprintf(h,"\t\t<ComputerName>%s</ComputerName>\n",role);
				fprintf(h,"\t\t<AdminPassword>%s</AdminPassword>\n",Waz.password);
				fprintf(h,"\t\t<ResetPasswordOnFirstLogon>%s</ResetPasswordOnFirstLogon>\n",
					(option & _AZURE_WINDOW_RESET ? "true" : "false") );
				fprintf(h,"\t\t<EnableAutomaticUpdates>%s</EnableAutomaticUpdates>\n",
					(option & _AZURE_WINDOWS_UPDATE ? "true" : "false") );
				/* ------------------------------------------------ */
				/* TODO: the stored certificates need to be handled */
				/* ------------------------------------------------ */
				fprintf(h,"\t\t<StoredCertificateSettings/>");
			}

			fprintf(h,"\t\t</ConfigurationSet>\n");
			fprintf(h,"\t\t</ConfigurationSets>\n");
			if ( rest_valid_string( zone ) )
			{
				fprintf(h,"\t\t<AvailabilitySetName>%s</AvailabilitySetName>\n",zone);
			}
			/* --------------------------- */
        		/* TODO Handle Extension disks */
			/* --------------------------- */
			if ( option & _AZURE_HARDDISK )
			{
				fprintf(h,"\t\t<DataVirtualHardDisk/>\n");
			}
			fprintf(h,"\t\t<OSVirtualHardDisk>\n");
			fprintf(h,"\t\t<HostCaching>%s</HostCaching>\n",
				( option & _AZURE_READONLY ? "ReadOnly" : "ReadWrite" ));

			/* <DiskLabel>os-disk-label</DiskLabel> */
			/* <DiskName>new-or-existing-disk-name</DiskName> */

			if ( rest_valid_string( media ) )
				fprintf(h,"\t\t<MediaLink>%s</MediaLink>\n",media);
			if ( rest_valid_string( image ) )
				fprintf(h,"\t\t<SourceImageName>%s</SourceImageName>\n",image);

			fprintf(h,"\t\t</OSVirtualHardDisk>\n");

			if ( rest_valid_string( flavor ) )
				fprintf(h,"\t\t<RoleSize>%s</RoleSize>\n",flavor);

		fprintf(h,"\t</Role>\n");
		fprintf(h,"\t</RoleList>\n");
			
		/* ------------------------------- */
		/* the virtual network information */
		/* ------------------------------- */
		fprintf(h,"\t<VirtualNetworkName>%s</VirtualNetworkName>\n",network);

		/* --------------------------------------------- */
		/* TODO : Option DNS Description will go in here */
		/* --------------------------------------------- */

	fprintf(h,"</Deployment>\n");
	fclose(h);
	if ( newdeployment ) liberate( newdeployment );
	return( filename );
	}
}

/*	----------------------------------------	*/
/*		a z _ c r e a t e _ vm 			*/
/*	----------------------------------------	*/
public	struct	az_response *	az_create_vm( char * filename )
{
	char	url[2048];
	sprintf(url,"/services/hostedservices/%s/deployments",Waz.hostedservice);
	return( azure_create_operation( url, filename ) );
}

/*	----------------------------------------	*/
/*		a z _ d e l e t e _ vm 			*/
/*	----------------------------------------	*/
public	struct	az_response *	az_delete_vm( char * depname, char * rolename )
{
	char	url[2048];
	sprintf(url,"/services/hostedservices/%s/deployments/%s/roles/%s",Waz.hostedservice,depname,rolename);
	return( azure_delete_operation( url ) );
}

/*	----------------------------------------	*/
/*		a z _ g e t _ vm 			*/
/*	----------------------------------------	*/
public	struct	az_response *	az_get_vm( char * depname, char * rolename )
{
	char	url[2048];
	sprintf(url,"/services/hostedservices/%s/deployments/%s/roles/%s",Waz.hostedservice,depname,rolename);
	return( azure_retrieve_operation( url ) );
}

/*	----------------------------------------	*/
/*		a z _ l i s t _ vm 			*/
/*	----------------------------------------	*/
public	struct	az_response *	az_list_vm( char * depname )
{
	char	url[2048];
	sprintf(url,"/services/hostedservices/%s/deployments/%s/roles",Waz.hostedservice,depname);
	return( azure_list_operation( url ) );
}

/*	-------------------------------------------------	*/
/*	a z _ n e t w o r k _ c o n f i g _ r e q u e s t	*/
/*	-------------------------------------------------	*/
public	char *	az_delete_network_config_request()
{
	FILE * h;
	char *	filename;

	/* ---------------------------------- */
	/* build the VM creation request file */
	/* ---------------------------------- */
	if (!( filename = rest_temporary_filename("xml")))
	{
		return( filename );
	}
	else if (!( h = fopen( filename,"wa" ) ))
	{
		return( liberate( filename ) );
	}
	else
	{
		/* ----------------------------------- */
		/* generate the content of the request */
		/* ----------------------------------- */
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<NetworkConfiguration \n");
			fprintf(h,"\txmlns=%chttp://schemas.microsoft.com/ServiceHosting/2011/07/NetworkConfiguration%c\n",0x0022,0x0022);
			fprintf(h,"\txmlns:i=%chttp://www.w3.org/2001/XMLSchema-instance%c>\n",0x0022,0x0022);
	    	fprintf(h,"<VirtualNetworkConfiguration>\n");
		fprintf(h,"<VirtualNetworkSites/>\n");
	    	fprintf(h,"</VirtualNetworkConfiguration>\n");
		fprintf(h,"</NetworkConfiguration>\n");
		fclose(h);
		return( filename );
	}
}

/*	-------------------------------------------------	*/
/*	a z _ n e t w o r k _ c o n f i g _ r e q u e s t 	*/
/*	-------------------------------------------------	*/
public	char *	az_create_network_config_request(char * name, char * group, char * address, char * label )
{
	FILE * h;
	char *	filename;
	char 	buffer[2048];

	/* ---------------------------------- */
	/* build the VM creation request file */
	/* ---------------------------------- */
	if (!( filename = rest_temporary_filename("xml")))
	{
		return( filename );
	}
	else if (!( h = fopen( filename,"wa" ) ))
	{
		return( liberate( filename ) );
	}
	else
	{
		/* ----------------------------------- */
		/* generate the content of the request */
		/* ----------------------------------- */
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<NetworkConfiguration \n");
			fprintf(h,"\txmlns=%chttp://schemas.microsoft.com/ServiceHosting/2011/07/NetworkConfiguration%c\n",0x0022,0x0022);
			fprintf(h,"\txmlns:i=%chttp://www.w3.org/2001/XMLSchema-instance%c>\n",0x0022,0x0022);
	    	fprintf(h,"<VirtualNetworkConfiguration>\n");
		fprintf(h,"<VirtualNetworkSites>\n");

			fprintf(h,"\t<VirtualNetworkSite name=%c%s%c AffinityGroup=%c%s%c>\n",0x0022,name,0x0022,0x0022,group,0x0022);
			fprintf(h,"\t<AddressSpace><AddressPrefix>%s</AddressPrefix></AddressSpace>\n",address);
			fprintf(h,"\t</VirtualNetworkSite>\n");

		fprintf(h,"</VirtualNetworkSites>\n");
	    	fprintf(h,"</VirtualNetworkConfiguration>\n");
		fprintf(h,"</NetworkConfiguration>\n");
		fclose(h);
		return( filename );
	}
}

/*	-----------------------------------------	*/
/*	a z _ g e t _ n e t w o r k _ c o n f i g	*/
/*	-----------------------------------------	*/
public	struct	az_response * az_get_network_config()
{
	char	url[2048];
	sprintf(url,"/services/networking/media");
	return( azure_retrieve_operation( url ) );
}

/*	-----------------------------------------------	*/
/*	a z _ u p d a t e _ n e t w o r k _ c o n f i g	*/
/*	-----------------------------------------------	*/
public	struct	az_response * az_update_network_config(char * filename)
{
	char	url[2048];
	sprintf(url,"/services/networking/media");
	return( network_update_operation( url, filename ) );
}

/*	-----------------------------------------------	*/
/*	a z _ d e l e t e _ n e t w o r k _ c o n f i g	*/
/*	-----------------------------------------------	*/
public	struct	az_response * az_delete_network_config(char * filename)
{
	char	url[2048];
	sprintf(url,"/services/networking/media");
	return( network_update_operation( url, filename ) );
}

/*	----------------------------------------	*/
/*	     a z _ l i s t _ n e t w o r k 		*/
/*	----------------------------------------	*/
public	struct	az_response * az_list_network()
{
	char	url[2048];
	sprintf(url,"/services/networking/virtualnetwork");
	return( azure_list_operation( url ) );
}

/*	-----------------------------------------------------------------	*/
/*	 a z _ c r e a t e _ h o s t e d _ s e r v i c e _ r e q u e s t 	*/ 
/*	-----------------------------------------------------------------	*/
public	char * az_create_hosted_service_request(
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
	if (!( label ))
		return( label );
	else	n = EncodeBase64( buffer, label,strlen(label));
	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<CreateHostedService xmlns=%c%s%c>\n",0x0022,Waz.namespace,0x0022);
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

/*	------------------------------------------------------------	*/
/*			a z _ l i s t _ o s _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	char * az_create_os_request(char * name, char * label, char * media, char * os)
{
	char *	filename;
	FILE *	h;
	char 	buffer[1024];
	int	n;
	if (!( label ))
		return( label );
	else	n = EncodeBase64( buffer, label,strlen(label));
	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<OSImage xmlns=%c%s%c>\n",0x0022,Waz.namespace,0x0022);
		fprintf(h,"\t<Label>%s</Label>\n",buffer);
		fprintf(h,"\t<MediaLink>%s</MediaLink>\n",media);
		fprintf(h,"\t<Name>%s</Name>\n",name);
		fprintf(h,"\t<OS>%s</OS>\n",os);
		fprintf(h,"</OSImage>\n");
		fclose(h);
		return( filename );
	}
}

/*	------------------------------------------------------------	*/
/*			a z _ c r e a t e _ o s _ i m a g e 		*/
/*	------------------------------------------------------------	*/
public	struct	az_response * az_create_os_image(char * filename )
{
	return( azure_create_operation("/services/images",filename) ); 
}

/*	------------------------------------------------------------	*/
/*			a z _ l i s t _ o s _ i m a g e s               */
/*	------------------------------------------------------------	*/
public	struct	az_response * az_list_os_images()
{
	return( azure_list_operation("/services/images") ); 
}

/*	------------------------------------------------------------	*/
/*			a z _ l i s t _ o s _ i m a g e s               */
/*	------------------------------------------------------------	*/
public	struct	az_response * az_get_os_image(char * name)
{
	char	url[2048];
	sprintf(url,"/services/images/%s",name);
	return( azure_retrieve_operation(url) ); 
}

/*	------------------------------------------------------------	*/
/*		    a z _ d e l e t e _ o s _ i m a g e s		*/
/*	------------------------------------------------------------	*/
public	struct	az_response * az_delete_os_image(char * name)
{
	char	url[2048];
	sprintf(url,"/services/images/%s",name);
	return( azure_delete_operation(url) ); 
}

/*	------------------------------------------------------------	*/
/*			a z _ l i s t _ c o n t a i n e r s             */
/*	------------------------------------------------------------	*/
public	struct	az_response * az_list_containers(char * storage)
{
	char	url[2048];
	sprintf(url,"http://%s.blob.core.windows.net/?comp=list",storage);
	return( azure_retrieve_operation(url) ); 
}

/*	------------------------------------------------------------	*/
/*			a z _ l i s t _ o s _ d i s k s			*/
/*	------------------------------------------------------------	*/
public	struct	az_response * az_list_os_disks()
{
	return( azure_retrieve_operation("/services/disks") ); 
}

/*	------------------------------------------------------------	*/
/*			a z _ g e t _ s u b s c r i p t i o n	        */
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_get_subscription()
{

	return( azure_list_operation( "" ));
}

/*	------------------------------------------------------------	*/
/*			a z _ l i s t _ d e p l o y m e n t             */
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_list_deployment_slots(char * server)
{
	char	buffer[1024];
	sprintf(buffer,"/services/hostedservices/%s/deploymentslots" , server ); 
	return( azure_list_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*			a z _ l i s t _ d e p l o y m e n t             */
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_list_deployments(char * server)
{
	char	buffer[1024];
	sprintf(buffer,"/services/hostedservices/%s/deployments" , server ); 
	return( azure_list_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*		a z _ c r e a t e _ d e p l o y m e n t		        */
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_create_deployment( char * filename, char * server, char * slot )
{
	char	buffer[1024];
	sprintf(buffer,"/services/hostedservices/%s/deploymentslots/%s" , server, slot ); 
	return( azure_create_operation( buffer, filename ) );
}

/*	------------------------------------------------------------	*/
/*				a z _ g e t _ d e p l o y m e n t       */
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_get_deployment( char * server, char * slot )
{
	char	buffer[1024];
	sprintf(buffer,"/services/hostedservices/%s/deploymentslots/%s" , server, slot ); 
	return( azure_retrieve_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*			a z _ d e l e t e _ d e p l o y m e n t         */
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_delete_deployment_slots( char * server, char * slot )
{
	char	buffer[1024];
	sprintf(buffer,"/services/hostedservices/%s/deploymentslots/%s" , server, slot ); 
	return( azure_delete_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*			a z _ d e l e t e _ d e p l o y m e n t         */
/*	------------------------------------------------------------	*/
public	struct	az_response *	az_delete_deployment( char * server, char *deployment )
{
	char	buffer[1024];
	sprintf(buffer,"/services/hostedservices/%s/deployments/%s" , server, deployment );
	return( azure_delete_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*	  a z _ c r e a t e _ d e p l o y m e n t _ r e q u e s t       */
/*	------------------------------------------------------------	*/
public	char * az_create_deployment_request(
	char * name, char * label, char * image, char * configuration )
{
	char *	filename;
	FILE *	h;
	char 	buffer[1024];
	int	n;
	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	if (!( h = fopen( filename,"wa" ) ))
		return (char *)( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<CreateDeployment xmlns=%c%s%c>\n",0x0022,Waz.namespace,0x0022);
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
	if (!( hptr = az_authenticate("text/xml","application/xml") ))
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
	if (!( hptr = az_authenticate("text/xml","application/xml") ))
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
	if (!( hptr = az_authenticate("text/xml","application/xml") ))
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
/*		a z _ i n i t i a l i s e _ s e r v i c e		*/
/*	------------------------------------------------------------	*/
public	int	az_initialise_service( char * servicename )
{
	struct	az_response * azptr;
	int	status;
	if ( Waz.hostedservice ) Waz.hostedservice = liberate( Waz.hostedservice );
	if (!( Waz.hostedservice = allocate_string( servicename ) ))
		return( 27 );
	else	return( 0  );
}


/*	------------------------------------------------------------	*/
/*		a z _ i n i t i a l i s e _ c l i e n t 	        */
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


