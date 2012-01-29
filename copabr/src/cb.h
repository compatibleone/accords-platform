/* ---------------------------------------------------------------------------- */
/* Advanced Capabilities for Compatible One Resources Delivery System - ACCORDS	*/
/* (C) 2011 by Iain James Marshall <ijm667@hotmail.com>				*/
/* ---------------------------------------------------------------------------- */
/*										*/
/* This is free software; you can redistribute it and/or modify it		*/
/* under the terms of the GNU Lesser General Public License as			*/
/* published by the Free Software Foundation; either version 2.1 of		*/
/* the License, or (at your option) any later version.				*/
/*										*/
/* This software is distributed in the hope that it will be useful,		*/
/* but WITHOUT ANY WARRANTY; without even the implied warranty of		*/
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU		*/
/* Lesser General Public License for more details.				*/
/*										*/
/* You should have received a copy of the GNU Lesser General Public		*/
/* License along with this software; if not, write to the Free			*/
/* Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA		*/
/* 02110-1301 USA, or see the FSF site: http://www.fsf.org.			*/
/*										*/
/* ---------------------------------------------------------------------------- */
#ifndef	_cb_h
#define	_cb_h

#include "cp.h"

#define	_BROKER_PROVIDER_CONTRACT	1

struct	cords_provisioning
{
	char			* planID;
	char			* namePlan;
	struct	occi_response 	* plan;
	char			* reqID;
	struct	occi_response 	* manifest;
	char			* accID;
	char			* accName;
	struct	occi_response 	* account;
	char			* secID;
	struct	occi_response 	* security;
	char			* confID;
	struct	occi_response 	* configuration;
	char			* instID;
	struct	occi_response 	* instance;
	struct	xml_element 	* document;
	int			nodes;
};

#define	_SCOPE_COMMON	2
#define	_SCOPE_NORMAL	0
#define	_ACCESS_PUBLIC	1
#define	_ACCESS_PRIVATE	0

struct	cords_node_descriptor
{
	int			scope;
	char 			* account;	/* account for which service is engaged			*/
	char			* nameApp;
	char 			* typeApp;
	char			* scopeApp;	/* the node scope : normal/common 			*/
	char			* accessApp;	/* the node access: public/private 			*/
	char			* hid;
	char			* sid;
	char			* provider;	/* the provisioner, supplier or "provider"		*/
	char			* providerid;	/* the provisioner, supplier or "provider" identity	*/
	char			* profile;	/* configuration profile for the use of provider	*/
	struct	occi_response 	* node;		/* template node from the request			*/
	struct	occi_response 	* hardware;	/* the template infrastructure from the node		*/
	struct	occi_response 	* compute;	/* the compute from the infrastructure			*/
	struct	occi_response 	* storage;	/* the storage from the infrastructure			*/
	struct	occi_response 	* network;	/* the network from the infrastructure			*/
	struct	occi_response 	* software;	/* template image from the node				*/
	struct	occi_response 	* system;	/* the system from the image				*/
	struct	occi_response 	* package;	/* the package from the image				*/
	struct	occi_response 	* contract;	/* the resulting contract/machine instance controller 	*/
};

public	int	cords_initialise_broker_operator( 
		int mode, char * provider, char * account, 
		char * pricelist, char * domain );

public 	struct	occi_element * cords_first_link( struct occi_response * zptr );
public 	struct 	occi_element * cords_next_link( struct occi_element * eptr );

public	struct	xml_element  * cords_build_service( 
	char * 	name, char * plan, char * manifest, char * account, char * tarification );

public	struct	xml_element * 	cords_build_contract( 
	char * 	node, 	char * name, char * provider );

public	char *	cords_create_service( char * plan, char * agent, char * tls );

public	char *	cords_manifest_broker(
	char * 	host, char * plan, char * nameplan, char * manifest, char * agent, char * tls,
	struct xml_element ** root );

public	int	get_provisioning_status();

public	char *	cords_extract_atribut( 
	struct occi_response * zptr, char * domain,
	char * category, char * nptr );

	/* ----- */
#endif	/* _cb_h */
	/* ----- */


