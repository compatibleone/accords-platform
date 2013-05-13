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
#ifndef _cn_client_h
#define _cn_client_h

#include "standard.h"
#include "rest.h"
#include "url.h"
#include "urlpublic.h"
#include "restclient.h"
#include "json.h"
#include "document.h"
#include "cnconfig.h"

#define _TEXT_NONE      0
#define _TEXT_XML       1
#define _TEXT_JSON      2

struct  cn_response
{
        int     nature;
        char *  content;
        struct  rest_response * response;
        struct  xml_element * xmlroot;
        struct  data_element * jsonroot;
};

public  struct cn_response * cn_liberate_response( struct cn_response * rptr );
public  struct cn_config   * cn_allocate_config();
public  struct cn_response * cn_check( struct rest_response *  aptr );

public  struct cn_response * cn_list_images( struct cn_config * sptr, char * query );
public  struct cn_response * cn_list_instancetypes( struct cn_config * sptr, char * query );
public  struct cn_response * cn_list_virtualmachines( struct cn_config * sptr, char * query );
public  struct cn_response * cn_list_volumestores( struct cn_config * sptr, char * query );
public  struct cn_response * cn_list_securitygroups( struct cn_config * sptr );
public  struct cn_response * cn_list_keypairs( struct cn_config * sptr );
public  struct cn_response * cn_list_workloads( struct cn_config * sptr );
public  struct cn_response * cn_list_transactions( struct cn_config * sptr );

public  struct cn_response * cn_get_image( struct cn_config * sptr, char * image );
public  struct cn_response * cn_get_instancetype( struct cn_config * sptr, char * instancetype );
public  struct cn_response * cn_get_virtualmachine( struct cn_config * sptr, char * virtualmachine );
public  struct cn_response * cn_get_volumestore( struct cn_config * sptr, char * volumestore );
public  struct cn_response * cn_get_securitygroup( struct cn_config * sptr, char * securitygroup );
public  struct cn_response * cn_get_keypair( struct cn_config * sptr, char * keypair );
public  struct cn_response * cn_get_workload( struct cn_config * sptr, char * workload );
public  struct cn_response * cn_get_transaction( struct cn_config * sptr, char * transaction );

public  struct cn_response * cn_delete_securitygroup( struct cn_config * sptr, char * securitygroup );
public  struct cn_response * cn_delete_keypair( struct cn_config * sptr, char * keypair );
public  struct cn_response * cn_delete_workload( struct cn_config * sptr, char * workload );
public  struct cn_response * cn_delete_workload_element( struct cn_config * sptr, char * workload, char * workloadelement);
public  struct cn_response * cn_delete_transaction( struct cn_config * sptr, char * transaction );

public  struct cn_response * cn_create_securitygroup( struct cn_config * sptr, char * filename );
public  struct cn_response * cn_create_keypair( struct cn_config * sptr, char * filename );
public  struct cn_response * cn_create_workload( struct cn_config * sptr, char * filename );
public  struct cn_response * cn_create_transaction( struct cn_config * sptr, char * filename );

public  struct cn_response * cn_add_port( struct cn_config * sptr, char * securitygroup, char * filename );
public  struct cn_response * cn_remove_port( struct cn_config * sptr, char * securitygroup, char * filename );

public  struct cn_response * cn_add_vm_to_workload( struct cn_config * sptr, char * workload, char * filename );
public  struct cn_response * cn_add_vs_to_workload( struct cn_config * sptr, char * workload, char * filename );

public  struct cn_response * cn_start_transaction( struct cn_config * sptr, char * transaction, char * filename );
public  struct cn_response * cn_stop_transaction( struct cn_config * sptr, char * transaction, char * filename );

public  char *
    cn_create_securitygroup_request( char * name, char * description, char * region, char * provider );
public  char *
    cn_create_keypair_request( char * name, char * region, char * provider );
public  char *
    cn_create_workload_request( char * name );
public  char *
    cn_create_transaction_request( char * workload, char * billingtype );
    
public  char *
    cn_add_port_request( char * protocol, char * from, char * to, char * cidr );
public  char *
    cn_remove_port_request( char * protocol, char * from, char * to, char * cidr  );

public  char *
    cn_add_vm_request(
        char * name, int count, char * image, char * instancetype,
        char * securitygroup, char * keypair, char * resourceparameters );
        
public  char *
    cn_add_vs_request(
        char * name, char * provider, char * region, char * zone, int size );
        
public  char * cn_start_transaction_request();
public  char * cn_stop_transaction_request();

public	struct cn_config *   cn_initialise_client( 
		char * apikey, char * apisec, char * host, 
		char * agent, char * version, char * tls );
		
public  struct cn_config *   cn_liberate_config( struct cn_config * sptr );

        /* -------------- */
#endif  /* _cnclient_h */
        /* -------------- */
