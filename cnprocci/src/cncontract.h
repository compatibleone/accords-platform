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
#ifndef	_cncontract_h	
#define	_cncontract_h

#define	_CN_STATUS_DELETED  "Deleted"
#define	_CN_STATUS_CREATED  "Created"
#define	_CN_STATUS_FAILED  "Failed"
#define	_CN_BUILD_SLEEP     20
#define	_CN_DESTROY_SLEEP   10
#define	_CORDS_CN_AGENT _CORDS_CONTRACT_AGENT

struct	cords_vector
{
	char *	id;
	struct occi_response * message;
};

struct	cords_cn_contract
{
	struct	cn_config *     config;
	struct	cords_vector	node;
	struct	cords_vector	infrastructure;
	struct	cords_vector	compute;
	struct	cords_vector	network;
	struct	cords_vector	firewall;
	struct	cords_vector	port;
	struct	cords_vector	storage;
	struct	cords_vector	image;
	struct	cords_vector	system;
	struct	cords_vector	package;
	struct	cn_response *	instancetypes;
	struct	cn_response *	images;
	struct  cn_response *   securitygroups;
	char *  provider;
	char *  region;
	char *  providerId;
};

public 	struct	rest_response * start_computenext(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr );

public 	struct	rest_response * stop_computenext(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr );

public 	struct	rest_response * save_computenext(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr );
	
public	int	create_computenext_contract(
		struct occi_category * optr,
		struct computenext * pptr,
		struct rest_request  * rptr);

public	int	delete_computenext_contract(
		struct occi_category * optr,
		struct computenext * pptr,
		struct rest_request  * rptr);

private	struct cn_config * 	use_computenext_configuration( char * nptr );
private	struct	cn_config * 	resolve_cn_configuration( char * sptr );
private	struct cn_response * 	stop_computenext_provisioning( struct computenext * pptr );
private	char *			build_computenext_securitygroup(struct cn_config * config, struct cords_cn_contract * cptr, struct computenext * pptr);
private	char *			build_computenext_workload(struct cn_config * config, struct computenext * pptr, struct cords_cn_contract * cptr );
private char * 			resolve_contract_securitygroup( struct cn_config * config, struct cords_cn_contract * cptr,struct computenext * pptr  );
private char * 			resolve_contract_image( struct cn_config * config, struct cords_cn_contract * cptr );
private char * 			resolve_contract_instancetype( struct cn_config * config, struct cords_cn_contract * cptr );
private	struct cn_response * 	stop_computenext_provisioning( struct computenext * pptr );
private	char *			resolve_vm_image_url( char * iid, char * pid );
private	int			computenext_build_failure( struct computenext * pptr, int status, char * message );
private	int			terminate_computenext_contract( int status, struct cords_cn_contract * cptr );

	/* ----------- */
#endif	/* _cncontract_h */
	/* ----------- */
