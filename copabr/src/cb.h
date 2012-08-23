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

struct	cords_placement_criteria
{
	char *		node;		/* node description		*/
	char *		price;		/* by price			*/
	char *		energy;		/* by energy			*/
	char *		opinion;	/* by opinion			*/
	char *		provider;	/* by provider type		*/
	char *		operator;	/* by named operator		*/
	char *		zone;		/* by geographical zone		*/
	char *		security;	/* by security considerations	*/
	char *		solution;	/* placement ID of solution	*/
	char *		algorithm;	/* the placement algorithm	*/
};

struct	cords_guarantee_criteria;
struct	cords_guarantee_element
{
	struct	cords_guarantee_element * previous;
	struct	cords_guarantee_element * next;
	struct	cords_guarantee_criteria* parent;

	char *		reference;	/* guarantee instance identity	*/
	char *		importance;	/* importance of the condition	*/
	char *		obligated;	/* the obligated party account	*/
	char *		scope;		/* the involved service items	*/
	char *		property;	/* the name of the metric 	*/
	char *		condition;	/* the involved condition	*/
	char *		objective;	/* the involved value		*/
};

struct	cords_guarantee_criteria
{
	int		elements;
	struct	cords_guarantee_element * first;
	struct	cords_guarantee_element * last;
};

struct	cords_provisioning
{
	char			* planID;
	char			* namePlan;
	struct	occi_response 	* plan;
	char			* reqID;
	struct	occi_response 	* manifest;
	char			* slaID;
	struct	occi_response 	* sla;
	char			* accID;
	char			* accName;
	struct	occi_response 	* account;
	char			* secID;
	struct	occi_response 	* security;
	char			* confID;
	struct	occi_response 	* configuration;
	char			* releaseID;
	struct	occi_response 	* release;
	char			* interID;
	struct	occi_response 	* interface;
	char			* instID;
	struct	occi_response 	* instance;
	struct	xml_element 	* document;
	struct	cords_placement_criteria * placement;
	struct	cords_guarantee_criteria * warranty;
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
	char 			* accountName;	/* name of account for which service is engaged		*/
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

	struct	cords_placement_criteria selector;
	struct	cords_guarantee_criteria warranty;

};

public	int	cords_initialise_broker_operator( 
		int mode, char * provider, char * account, 
		char * pricelist, char * domain );

public 	struct	occi_element * cords_first_link( struct occi_response * zptr );
public 	struct 	occi_element * cords_next_link( struct occi_element * eptr );

public	struct	xml_element  * cords_build_service( 
	char * 	name, char * plan, char * manifest, char * sla, char * account, char * tarification );

public	struct	xml_element * 	cords_build_contract( 
	char * 	node, 	char * name, char * provider );

public	char *	cords_create_service( char * plan, char * agent, char * tls );

public	char *	cords_manifest_broker(
	char * 	host, char * plan, char * nameplan, char * manifest, char * agent, char * tls, char * sla,
	struct xml_element ** root );

public	char *	cords_service_broker(
	char * 	host, 
	char * 	service,
	char * 	servicename,
	char *	plan,
	char * 	manifest, 
	char *	sla,
	char * 	agent, 
	char * 	tls, 
	struct xml_element ** root );

public	int	get_provisioning_status();

public	struct	xml_element * cords_instance_node( 
		struct cords_placement_criteria * selector,
		struct cords_guarantee_criteria * warrenty,
		char * host,
		char * id,
		char * agent,
		char * tls,
		char * sla,
		char * namePlan,
		char * account,
		char * accountName );


	/* ----- */
#endif	/* _cb_h */
	/* ----- */


