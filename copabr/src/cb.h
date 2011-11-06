
#ifndef	_cb_h
#define	_cb_h

#include "cp.h"

struct	cords_provisioning
{
	char			* planID;
	char			* namePlan;
	struct	occi_response 	* plan;
	char			* reqID;
	struct	occi_response 	* manifest;
	char			* accID;
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

struct	cords_node_descriptor
{
	char			* nameApp;
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

public 	struct	occi_element * cords_first_link( struct occi_response * zptr );
public 	struct occi_element * cords_next_link( struct occi_element * eptr );

#ifndef	_STAND_ALONE_BROKER
public	int cords_manifest_broker(char * host,char * plan, char * nameplan, char * request,char * agent, char * tls );
#else
public	int cords_manifest_broker(char * host, char * plan, char * agent, char * tls, char * result );
#endif

	/* ----- */
#endif	/* _cb_h */
	/* ----- */

