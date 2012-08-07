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
#ifndef _az_client_h
#define _az_client_h

#include "standard.h"
#include "rest.h"
#include "url.h"
#include "urlpublic.h"
#include "restclient.h"
#include "json.h"
#include "document.h"
#include "azconfig.h"

/*	------------------------	*/
/*	VM ROLE Creation Options	*/
/*	------------------------	*/
#define	_AZURE_IS_PRODUCTION	0x0001
#define	_AZURE_IS_WINDOWS	0x0002
#define	_AZURE_HARDDISK		0x0004
#define	_AZURE_READONLY		0x0008
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
public	struct	az_response *	az_create_hosted_service( char * filename );
public	struct	az_response *	az_get_hosted_service( char * hostname );
public	struct	az_response *	az_update_hosted_service( char * hostname, char * filename );
public	struct	az_response *	az_delete_hosted_service( char * hostname );
public	struct	az_response *	az_list_hosted_services();

/* --------------- */
/* affinity groups */
/* --------------- */
public	struct	az_response *	az_create_affinity_group( char * filename );
public	struct	az_response *	az_retrieve_affinity_group( char * filename );
public	struct	az_response *	az_update_affinity_group( char * filename );
public	struct	az_response *	az_delete_affinity_group( char * filename );
public	struct	az_response *	az_list_affinity_groups();

public	char * az_create_affinity_group_request(char * name,char * label, char * description,char * location );

/* ------------ */
/* certificates */
/* ------------ */
public	struct	az_response *	az_create_certificate( char * filename );
public	struct	az_response *	az_retrieve_certificate( char * filename );
public	struct	az_response *	az_delete_certificate( char * filename );
public	struct	az_response *	az_list_certificates();

/* --------- */
/* locations */
/* --------- */
public	struct	az_response *	az_list_locations();

/* ------- */
/* servers */
/* ------- */
public	struct	az_response *	az_list_servers	();
public	struct	az_response *	az_create_hosted_service( char * filename );
public	struct	az_response *	az_get_server	( char * id );
public	struct	az_response *	az_update_server( char * id, char * filename );
public	struct	az_response *	az_delete_server( char * id );

public	char * az_create_hosted_service_request(
    char * name, char * label, char * description, 
    char * location, char * group );

/* ------- */
/* flavors */
/* ------- */
public	struct	az_response *	az_list_flavors();
public	struct	az_response *	az_get_flavor  ( char * id );

/* ------ */
/* images */
/* ------ */
public	struct	az_response *	az_list_images  ();
public	struct	az_response *	az_create_image( char * filename );
public	struct	az_response *	az_get_image    ( char * id );
public	struct	az_response *	az_delete_image ( char * id );
public	struct	az_response * 	az_list_os_images();
public	struct	az_response * 	az_get_os_image(char * name);
public	struct	az_response * 	az_list_os_disks();
public	struct	az_response * az_delete_os_image(char * name);
public	struct	az_response * az_create_os_image(char * filename );
public	char * 	az_create_os_request(char * name, char * label, char * media, char * os);
	
public	char * az_create_image_request(
    char * identity, char * server );

/* ---------- */
/* operations */
/* ---------- */
public	struct	az_response *	az_list_operations();

/* ----------- */
/* deployments */
/* ----------- */
public	struct	az_response *	az_list_deployments(char * server);
public	struct	az_response *	az_create_deployment( char * filename, char * server, char * slot );
public	struct	az_response *	az_get_deployment( char * server, char * slot);
public	struct	az_response *	az_delete_deployment( char * server, char * slot );

public	char * az_create_deployment_request(
    char * name, char * label, char * image, 
    char * configuration );

/* ------------- */
/* subscriptions */
/* ------------- */
public	struct	az_response *	az_get_subscription();
public	struct 	az_response * 	liberate_az_response( struct az_response * rptr );	

/* ----------------------- */
/* trafic manager profiles */
/* ----------------------- */
public	struct	az_response *	az_list_WATM_profiles();

/* ---------------------------------- */
/* trafic manager profile definitions */
/* ---------------------------------- */
public	struct	az_response *	az_list_WATM_definitions(char * profile );

/* ---------------- */
/* storage services */
/* ---------------- */
public	struct	az_response *	az_create_storage_service( char * filename );
public	struct	az_response *	az_retrieve_storage_service( char * filename );
public	struct	az_response *	az_update_storage_service( char * filename );
public	struct	az_response *	az_delete_storage_service( char * filename );
public	struct	az_response *	az_list_storage_services();
public	struct	az_response *  	az_list_containers( char * account );

public	char * az_create_vm_request(
	/* 	struct os_subscription * subptr,	*/
	char * name,		
	char * label,		
	char * image,	
	char * media,	
	char * flavor,		
	char * network,		
	char * zone,		
	int    access );

public	char * az_create_network_config_request( char * name, char * group, char * address, char * label );
public	char * az_delete_network_config_request();

public	struct	az_response * az_create_vm( char * filename );
public	struct	az_response * az_delete_vm( char * depname, char * rolename );
public	struct	az_response * az_get_vm( char * depname, char * rolename );
public	struct	az_response * az_list_vm( char * depname );;

public	struct	az_response * az_get_network_config();
public	struct	az_response * az_update_network_config(char * filename);
public	struct	az_response * az_delete_network_config(char * filename);
public	struct	az_response * az_list_network();

/* ---------------- -------*/
/* azure storage services  */
/* ----------------------- */
public	char * az_create_storage_service_request(char * name,char * label, char * description,char * group,char * location );

public	int	az_initialise_client( char * user, char * password, char * host, char * agent, char * version, char * tls,char * ns, char * subscription );

char* getCanonicalizedHeaders(struct rest_header *hptr);
char * getCanonicalizedResource(char *uri,char *myAccountStorage);
char *timestamp();
char *timestampo();
int indexOfchar(char str[],char c);
int searchWord(char *mot,char *line);
void str_sub(const char *s, unsigned int start, unsigned int end, char new_s[]);
void tri_iteratif(char *tableau[], int taille);
public char* AuthorizationHeader(char *method, char* now, char *uri,struct rest_header *hptr, char* myStorageAccount, char * mykeyStorage,int lengthb,char* ifMatch , char* md5 );

public	struct	az_response * azure_create_container( char * myStorageAccount,char * mykeyStorage,char *containerName);
public	struct	az_response * azure_delete_container( char * myStorageAccount,char *mykeyStorage,char* containerName);
public	struct	az_response * azure_list_container( char * myStorageAccount,char *mykeyStorage);

public	struct	az_response * azure_list_blob( char * myStorageAccount,char *mykeyStorage,char*containerName);
public	struct	az_response * azure_create_blob( char * myStorageAccount,char *mykeyStorage,char*containerName,char * blobName, char *content);
public	struct	az_response * azure_get_blob_content( char * myStorageAccount,char *mykeyStorage,char*containerName,char * blobName);
public	struct	az_response * azure_delete_blob( char * myStorageAccount,char *mykeyStorage,char*containerName,char * blobName);
public	char * az_create_blob_content(char *contentBody);

public	struct	az_response * azure_list_table( char * myStorageAccount,char *mykeyStorage);
public	struct	az_response * azure_create_table( char * myStorageAccount,char *mykeyStorage,char *tablename);
public	char * az_create_table_request_body( char *tablename);
public	struct	az_response * azure_delete_table( char * myStorageAccount,char *mykeyStorage,char *tablename);

public	struct	az_response * azure_list_queue( char * myStorageAccount,char *mykeyStorage);
public	struct	az_response * azure_create_queue( char * myStorageAccount,char *mykeyStorage,char *name);
public	struct	az_response * azure_delete_queue( char * myStorageAccount,char *mykeyStorage,char *name);

public	struct	az_response * azure_storage_request(char * myStorageAccount,char * mykeyStorage,char *uri,char *method,char *contentBody);

/* ---------------- -------*/
/* azure hosted  services  */
/* ----------------------- */
public	struct	az_response * azure_list_storage_account();
public	struct	az_response * azure_create_storage_account(char *strgname,char *location, char *affigroupe);
public	char * az_create_storage_account_request_body( char * name,char * label,char * description,char * location,char * group );
public	struct	az_response * azure_delete_storage_account(char *strgname);
public	struct	az_response * azure_get_storage_account_keys(char *strgname);
public	struct	az_response * azure_regenerate_storage_account_key(char *strgname,char *keytype);
public	char * az_regenerate_storage_account_key_request_body( char * keytype );

public	struct	az_response * azure_list_hosted_service();
public	struct	az_response * azure_create_hosted_service(char *srvname, char *label, char *description, char *location, char *affigroupe);
public	char * az_create_hosted_service_request_body(char * name,char * label,char * description,char * location,char * group );
public	struct	az_response * azure_get_hosted_service_properties(char *srvname);
public	struct	az_response * azure_delete_hosted_service(char *strgname);

public	struct	az_response * azure_create_deployment(char *serviceName, char *deploymentSlot, char *deployname, char *pkgUrl, char *configfile, char *label, char *startdeploy);
public char* az_create_deployment_request_body(char *deployname,char*pkgUrl,char *label,char *configfile, char *startdeploy);
public	struct	az_response * azure_delete_deployement(char *servicename, char *deploymentslot);

public	struct	az_response * azure_list_affinitygroups();
public	struct	az_response * azure_create_affinitygroups(char * name,char * label,char * description,char * location);
public	char * az_create_affinity_group_request_body(char * name,char * label,char * description,char * location );
public	struct	az_response * azure_delete_affinitygroup(char *name);

public	struct	az_response * azure_list_locations();

/* --------------------------*/
/* azure vm images services  */
/* ------------------------- */
public	struct	az_response * azure_list_os_image();
public	struct	az_response * azure_add_os_image(char * name,char * label,char *medialink,char * os);
public char *az_create_os_image_request_body(char * name,char * label,char *medialink,char * os);
public	struct	az_response * azure_delete_os_image(char *name, char *label);
public char *az_delete_os_image_request_body(char * label);
public	struct	az_response * azure_update_os_image(char * name,char * label,char *medialink,char * os);

/* --------------------------*/
/* azure vm  services        */
/* ------------------------- */
public	struct	az_response * azure_create_vm_deployment(char * sname, char * dname, char * label,char *vnetworkname,char *vmname,char *provisioningconfig);
public char *az_create_vm_request_body(char *name,char *label,char *vnetworkname,char *vmname, char *provisioningconfig);
public	struct	az_response * azure_set_network_configuration();
public char *az_ceate_network_configuration_request_body();
public	struct	az_response * azure_get_network_configuration();

public	struct	az_response *azure_service_management_request(char *uri,char *method,char *contentBody);


#endif	/*_az_client_h*/
		




