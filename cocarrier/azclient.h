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
#ifndef _az_client_h
#define _az_client_h

#include "standard.h"
#include "rest.h"
#include "url.h"
#include "urlpublic.h"
#include "restclient.h"
#include "co_json.h"
#include "document.h"
#include "azconfig.h"

struct	az_subscription
{
	struct az_config Az;
};

/*	------------------------	*/
/*	VM ROLE Creation Options	*/
/*	------------------------	*/
#define	_AZURE_IS_STAGING	0x0001
#define	_AZURE_IS_WINDOWS	0x0002
#define	_AZURE_HARDDISK		0x0004
#define	_AZURE_READONLY		0x0008
#define	_AZURE_REPROVISION 	0x0010
#define	_AZURE_LINUX_SSH	0x0100
#define	_AZURE_WINDOW_RESET	0x1000
#define	_AZURE_WINDOWS_UPDATE	0x2000

#define	_TEXT_NONE 	0
#define	_TEXT_XML	1
#define	_TEXT_JSON	2

struct	az_response
{
	int	nature;
	char			*	content;
	struct	xml_element 	*	xmlroot;
	struct	data_element	*	jsonroot;
	struct	rest_response	*	response;
};

#define	_AZ_NS_COMPUTE	"http://schemas.microsoft.com/windowsazure"
#define _AZ_BASE "core.windows.net"


struct az_vm_config
{
 char	*	id;
 char *vmname;
 char *vmadmipw;
 char *zonetime;
 char *domainname;
 char *userdomain;
 char *passwduserd;
 char *domainjname;
 char *distgname;
 char *storename;
 char *hostnamevm;
 char *lusername;
 char *lpassword;
 int  ssh;
 int  pubkeys;
 char *certfingerprint;
 char *certfingerprintkp;
 char *pubkeypath;
 char *keypairfingerprint;
 char *keypairepath;
 int networkconfig;
 char *lbendpoint;
 char *localport;
 char *endpointname;
 char *externalport;
 int  lbprob;
 char *pathstatus;
 char *portstatus;
 char *probprotocol;
 char *protocol;
 int subnet;
 char *frontendsubnet;
 char *backendsubnet;
 char *vmavailability;
 int datadisks;
 char *hostcaching;
 char *disklabel;
 char *diskname;
 char *lun;
 char *disksize;
 char *medialink;
 char *oshostcaching;
 char *osdisklabel;
 char *osdiskname;
 char *osmedialink;
 char *srimagename;
 char *rolesize;
 char *vnetworkname;
 char *dnsname;
 char *dnsipaddress;
};

struct vm_network{
 char* dnsname;
 char* ipv4address;
 char *localsitename;
 char *CIDRidentifier;
 char *IPV4addressvpngateway;
 char *virtualnetworkname;
 char *affinitygroupname;
 char *label;
 char *CIDRidentifieras;
 char *subnetname;
 char *CIDRidentifiersn;
 char *primarydnsname;
 char *gwprofile;
 char *localsitenamep;
};
/* ----------------------- */
/* azure request functions */
/* ----------------------- */

public	struct	az_response * 
    az_client_get_request(
        char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	az_response * 
    az_client_delete_request(
        char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	az_response * 
    az_client_head_request(
        char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	az_response * 
    az_client_post_request(
        char * target, char * tls, char * agent, char * filename, struct rest_header * hptr );

public	struct	az_response * 
    az_client_put_request(
        char * target, char * tls, char * agent, char * filename, struct rest_header * hptr );

/* --------------- */
/* hosted services */
/* --------------- */
public	struct	az_response *	az_create_hosted_service( struct az_subscription * sptr, char * filename );
public	struct	az_response *	az_get_hosted_service( struct az_subscription * sptr, char * hostname );
public	struct	az_response *	az_update_hosted_service( struct az_subscription * sptr, char * hostname, char * filename );
public	struct	az_response *	az_delete_hosted_service( struct az_subscription * sptr, char * hostname );
public	struct	az_response *	az_list_hosted_services( struct az_subscription * sptr);

/* --------------- */
/* affinity groups */
/* --------------- */
public	struct	az_response *	az_create_affinity_group( struct az_subscription * sptr, char * filename );
public	struct	az_response *	az_retrieve_affinity_group( struct az_subscription * sptr, char * filename );
public	struct	az_response *	az_update_affinity_group( struct az_subscription * sptr, char * filename );
public	struct	az_response *	az_delete_affinity_group( struct az_subscription * sptr, char * filename );
public	struct	az_response *	az_list_affinity_groups( struct az_subscription * sptr);

public	char * az_create_affinity_group_request( struct az_subscription * sptr,char * name,char * label, char * description,char * location );

/* ------------ */
/* certificates */
/* ------------ */
public	struct	az_response *	az_create_certificate( struct az_subscription * sptr, char * filename );
public	struct	az_response *	az_retrieve_certificate( struct az_subscription * sptr, char * filename );
public	struct	az_response *	az_delete_certificate( struct az_subscription * sptr, char * filename );
public	struct	az_response *	az_list_certificates( struct az_subscription * sptr);

/* --------- */
/* locations */
/* --------- */
public	struct	az_response *	az_list_locations( struct az_subscription * sptr);
public	struct	az_response *	az_retrieve_location( struct az_subscription * sptr,char * nptr);

/* ------- */
/* servers */
/* ------- */
public	struct	az_response *	az_list_servers	( struct az_subscription * sptr);
public	struct	az_response *	az_create_hosted_service( struct az_subscription * sptr, char * filename );
public	struct	az_response *	az_get_server	( struct az_subscription * sptr, char * id );
public	struct	az_response *	az_update_server( struct az_subscription * sptr, char * id, char * filename );
public	struct	az_response *	az_delete_server( struct az_subscription * sptr, char * id );

public	char * az_create_hosted_service_request( struct az_subscription * sptr,
    char * name, char * label, char * description, 
    char * location, char * group );

/* ------- */
/* flavors */
/* ------- */
public	struct	az_response *	az_list_flavors( struct az_subscription * sptr);
public	struct	az_response *	az_get_flavor  ( struct az_subscription * sptr, char * id );

/* ------ */
/* images */
/* ------ */
public	struct	az_response *	az_list_images  ( struct az_subscription * sptr);
public	struct	az_response *	az_create_image( struct az_subscription * sptr, char * filename );
public	struct	az_response *	az_get_image    ( struct az_subscription * sptr, char * id );
public	struct	az_response *	az_delete_image ( struct az_subscription * sptr, char * id );

/*	DISK OPERATIONS		*/
public	struct	az_response * 	az_list_os_disks( struct az_subscription * sptr);
public	struct	az_response * 	az_get_os_disk( struct az_subscription * sptr,char * name);
public	struct	az_response * 	az_delete_os_disk( struct az_subscription * sptr,char * disk);
public	struct	az_response * 	az_add_os_disk( struct az_subscription * sptr, char * filename );
public	char *			az_add_os_disk_request( struct az_subscription * sptr, char * name, char * description, char * media, int option );

/*	OS OPERATIONS		*/
public	struct	az_response * 	az_list_os_images( struct az_subscription * sptr);
public	struct	az_response * 	az_get_os_image( struct az_subscription * sptr,char * name);
public	struct	az_response * 	az_delete_os_image( struct az_subscription * sptr,char * name);
public	struct	az_response * 	az_create_os_image( struct az_subscription * sptr,char * filename );
public	char * 			az_create_os_request( struct az_subscription * sptr,char * name, char * label, char * media, char * os);
	
public	char * az_create_image_request( struct az_subscription * sptr,
    char * identity, char * server );

/* ---------- */
/* operations */
/* ---------- */
public	struct	az_response *	az_list_operations( struct az_subscription * sptr, char * start, char * end);
public	struct	az_response *	az_get_operation_status( struct az_subscription * sptr,char * opid);

/* ----------- */
/* deployments */
/* ----------- */
public	struct	az_response *	az_list_deployments( struct az_subscription * sptr,char * server);
public	struct	az_response *	az_create_deployment( struct az_subscription * sptr, char * filename, char * server, char * slot );
public	struct	az_response *	az_get_deployment( struct az_subscription * sptr, char * service, char * deployment );
public	struct	az_response *	az_delete_deployment( struct az_subscription * sptr, char * server, char * slot );

public	char * az_create_deployment_request( struct az_subscription * sptr,
    char * name, char * label, char * image, 
    char * configuration );

/* ------------- */
/* subscriptions */
/* ------------- */
public	struct	az_response *	az_get_subscription( struct az_subscription * sptr);
public	struct 	az_response * 	liberate_az_response( struct az_response * rptr );	

/* ----------------------- */
/* trafic manager profiles */
/* ----------------------- */
public	struct	az_response *	az_list_WATM_profiles( struct az_subscription * sptr);

/* ---------------------------------- */
/* trafic manager profile definitions */
/* ---------------------------------- */
public	struct	az_response *	az_list_WATM_definitions( struct az_subscription * sptr,char * profile );

/* ---------------- */
/* storage services */
/* ---------------- */
public	struct	az_response *	az_create_storage_service( struct az_subscription * sptr, char * filename );
public	struct	az_response *	az_retrieve_storage_service( struct az_subscription * sptr, char * filename );
public	struct	az_response *	az_update_storage_service( struct az_subscription * sptr, char * filename );
public	struct	az_response *	az_delete_storage_service( struct az_subscription * sptr, char * filename );
public	struct	az_response *	az_list_storage_services( struct az_subscription * sptr);
public	struct	az_response *  	az_list_containers( struct az_subscription * sptr, char * account );

public	char * az_start_vm_request( struct az_subscription * sptr);
public	char * az_shutdown_vm_request( struct az_subscription * sptr);
public	char * az_restart_vm_request( struct az_subscription * sptr);

public	char * az_capture_vm_request( struct az_subscription * sptr,
	/* 	struct os_subscription * subptr,	*/
	char *	hostname,
	char *	ilabel,
	char *	iname,
	int	option );

public	char * az_create_vm_request( struct az_subscription * sptr,
	/* 	struct os_subscription * subptr,	*/
	char * name,		
	char * label,		
	char * image,	
	char * media,	
	char * flavor,		
	char * network,		
	char * zone,		
	int    access,
	char * endpoints );

public	char * az_create_network_config_request( struct az_subscription * sptr, char * name, char * group, char * address, char * label );
public	char * az_delete_network_config_request( struct az_subscription * sptr);

public	struct	az_response * az_create_vm( struct az_subscription * sptr, char * filename );
public	struct	az_response * az_delete_vm( struct az_subscription * sptr, char * depname, char * rolename );
public	struct	az_response * az_operation_vm( struct az_subscription * sptr, char * filename, char * depname, char * rolename );
public	struct	az_response * az_get_vm( struct az_subscription * sptr, char * depname, char * rolename );
public	struct	az_response * az_list_vm( struct az_subscription * sptr, char * depname );;

public	struct	az_response * az_get_network_config( struct az_subscription * sptr);
public	struct	az_response * az_update_network_config( struct az_subscription * sptr,char * filename);
public	struct	az_response * az_delete_network_config( struct az_subscription * sptr,char * filename);
public	struct	az_response * az_list_network( struct az_subscription * sptr);

/* ---------------- -------*/
/* azure storage services  */
/* ----------------------- */
public	char * az_create_storage_service_request( struct az_subscription * sptr,char * name,char * label, char * description,char * group,char * location );
public	struct az_subscription * az_initialise_client( char * user, char * password, char * host, char * agent, char * version, char * tls,char * ns, char * subscription );

char* getCanonicalizedHeaders( struct rest_header *hptr);
char * getCanonicalizedResource( char *uri,char *myAccountStorage);
char *timestamp();
char *timestampo();
int indexOfchar( char str[],char c);
int searchWord( char *mot,char *line);
void str_sub( const char *s, unsigned int start, unsigned int end, char new_s[]);
void tri_iteratif( char *tableau[], int taille);
public char* AuthorizationHeader(char *method, char* now, char *uri,struct rest_header *hptr, char* myStorageAccount, char * mykeyStorage,int lengthb,char* ifMatch , char* md5 );

public	struct	az_response * azure_create_container( struct az_subscription * sptr, char * myStorageAccount,char * mykeyStorage,char *containerName);
public	struct	az_response * azure_delete_container( struct az_subscription * sptr, char * myStorageAccount,char *mykeyStorage,char* containerName);
public	struct	az_response * azure_list_container( struct az_subscription * sptr, char * myStorageAccount,char *mykeyStorage);

public	struct	az_response * azure_list_blob( struct az_subscription * sptr, char * myStorageAccount,char *mykeyStorage,char*containerName);
public	struct	az_response * azure_create_blob( struct az_subscription * sptr, char * myStorageAccount,char *mykeyStorage,char*containerName,char * blobName, char *content);
public	struct	az_response * azure_get_blob_content( struct az_subscription * sptr, char * myStorageAccount,char *mykeyStorage,char*containerName,char * blobName);
public	struct	az_response * azure_delete_blob( struct az_subscription * sptr, char * myStorageAccount,char *mykeyStorage,char*containerName,char * blobName);
public	char * az_create_blob_content( struct az_subscription * sptr,char *contentBody);

public	struct	az_response * azure_list_table( struct az_subscription * sptr, char * myStorageAccount,char *mykeyStorage);
public	struct	az_response * azure_create_table( struct az_subscription * sptr, char * myStorageAccount,char *mykeyStorage,char *tablename);
public	char * az_create_table_request_body( struct az_subscription * sptr, char *tablename);
public	struct	az_response * azure_delete_table( struct az_subscription * sptr, char * myStorageAccount,char *mykeyStorage,char *tablename);

public	struct	az_response * azure_list_queue( struct az_subscription * sptr, char * myStorageAccount,char *mykeyStorage);
public	struct	az_response * azure_create_queue( struct az_subscription * sptr, char * myStorageAccount,char *mykeyStorage,char *name);
public	struct	az_response * azure_delete_queue( struct az_subscription * sptr, char * myStorageAccount,char *mykeyStorage,char *name);

public	struct	az_response * azure_storage_request( struct az_subscription * sptr,char * myStorageAccount,char * mykeyStorage,char *uri,char *method,char *contentBody);

/* ---------------- -------*/
/* azure hosted  services  */
/* ----------------------- */
public	struct	az_response * azure_list_storage_account( struct az_subscription * sptr);
public	struct	az_response * azure_create_storage_account( struct az_subscription * sptr,char *strgname,char *location, char *affigroupe);
public	char * az_create_storage_account_request_body( struct az_subscription * sptr, char * name,char * label,char * description,char * location,char * group );
public	struct	az_response * azure_delete_storage_account( struct az_subscription * sptr,char *strgname);
public	struct	az_response * azure_get_storage_account_keys( struct az_subscription * sptr,char *strgname);
public	struct	az_response * azure_regenerate_storage_account_key( struct az_subscription * sptr,char *strgname,char *keytype);
public	char * az_regenerate_storage_account_key_request_body( struct az_subscription * sptr, char * keytype );

public	struct	az_response * azure_list_hosted_service( struct az_subscription * sptr);
public	struct	az_response * azure_create_hosted_service( struct az_subscription * sptr,char *srvname, char *label, char *description, char *location, char *affigroupe);
public	char * az_create_hosted_service_request_body( struct az_subscription * sptr,char * name,char * label,char * description,char * location,char * group );
public	struct	az_response * azure_get_hosted_service_properties( struct az_subscription * sptr,char *srvname);
public	struct	az_response * azure_delete_hosted_service( struct az_subscription * sptr,char *strgname);

public	struct	az_response * azure_create_deployment( struct az_subscription * sptr,char *serviceName, char *deploymentSlot, char *deployname, char *pkgUrl, char *configfile, char *label, char *startdeploy);
public char* az_create_deployment_request_body( struct az_subscription * sptr,char *deployname,char*pkgUrl,char *label,char *configfile, char *startdeploy);
public	struct	az_response * azure_delete_deployement( struct az_subscription * sptr,char *servicename, char *deploymentslot);

public	struct	az_response * azure_list_affinitygroups( struct az_subscription * sptr);
public	struct	az_response * azure_create_affinitygroups( struct az_subscription * sptr,char * name,char * label,char * description,char * location);
public	char * az_create_affinity_group_request_body( struct az_subscription * sptr,char * name,char * label,char * description,char * location );
public	struct	az_response * azure_delete_affinitygroup( struct az_subscription * sptr,char *name);

public	struct	az_response * azure_list_locations( struct az_subscription * sptr);
public	FILE *  az_start_endpoints( struct az_subscription * sptr, char * filename );
public	void	az_create_endpoint( struct az_subscription * sptr, FILE * h, char * name, int localport, int port, char * protocol );
public	void	az_close_endpoints( struct az_subscription * sptr, FILE * h );

/* --------------------------*/
/* azure vm images services  */
/* ------------------------- */
public	struct	az_response * azure_list_os_image( struct az_subscription * sptr);
public	struct	az_response * azure_add_os_image( struct az_subscription * sptr,char * name,char * label,char *medialink,char * os);
public char *az_create_os_image_request_body( struct az_subscription * sptr,char * name,char * label,char *medialink,char * os);
public	struct	az_response * azure_delete_os_image( struct az_subscription * sptr,char *name, char *label);
public char *az_delete_os_image_request_body( struct az_subscription * sptr,char * label);
public	struct	az_response * azure_update_os_image( struct az_subscription * sptr,char * name,char * label,char *medialink,char * os);

/* --------------------------*/
/* azure vm  services        */
/* ------------------------- */
public	struct	az_response * azure_create_vm_deployment( struct az_subscription * sptr,char * sname, char * dname, char * label,char *vnetworkname,char *vmname,char *provisioningconfig);
public char *az_create_vm_request_body( struct az_subscription * sptr,char *name,char *label,char *vnetworkname,char *vmname, char *provisioningconfig);
public	struct	az_response * azure_set_network_configuration( struct az_subscription * sptr);
public char *az_ceate_network_configuration_request_body( struct az_subscription * sptr);
public	struct	az_response * azure_get_network_configuration( struct az_subscription * sptr);

public	struct	az_response *azure_service_management_request( struct az_subscription * sptr,char *uri,char *method,char *contentBody);


#endif	/*_az_client_h*/
		




