/*-------------------------------------------------------------------------------*/
/* accords platform                                                              */
/* copyright 2012 ,Hamid MEDJAHED & Elyes ZEKRI (hmedjahed@prologue.fr) prologue */
/*-------------------------------------------------------------------------------*/
/* licensed under the apache license, version 2.0 (the "license");               */
/* you may not use this file except in compliance with the license.              */
/* you may obtain a copy of the license at                                       */
/*                                                                               */
/*       http://www.apache.org/licenses/license-2.0                              */
/*                                                                               */
/* unless required by applicable law or agreed to in writing, software           */
/* distributed under the license is distributed on an "as is" basis,             */
/* without warranties or conditions of any kind, either express or implied.      */
/* see the license for the specific language governing permissions and           */
/* limitations under the license.                                                */
/*-------------------------------------------------------------------------------*/
#ifndef	_ec2contract_h
#define	_ec2contract_h

#include "occiclient.h"
#include "cordslang.h"
#include "occiresolver.c"
#include "ec2client.h"


struct	cords_vector
{
	char *	id;
	struct occi_response * message;
};

struct	cords_ec2_contract
{
	struct  ec2_subscription * subscription;
	struct	cords_vector	node;
	struct	cords_vector	infrastructure;
	struct	cords_vector	compute;
	char * architecture;
	char * driver;
	char * location;
	struct	cords_vector	network;
	struct	cords_vector	firewall;
	struct	cords_vector	storage;
	struct	cords_vector	image;
	struct	cords_vector	system;
	struct	cords_vector	package;
};

struct	ec2_compute_infos
{
	int	cores;
	int	speed;
	int	memory;
	int	storage;
	char *	id;
};

struct	ec2_image_infos
{
	char *	id;
	char *	other;
	char *	name;
	char *	updated;
};

struct	ec2config * resolve_ec2_configuration( char * sptr )
struct ec2config * resolve_ec2_configurationb(char * agent, char * tls, char * sptr);
struct ec2_subscription * initialise_ec2_subscription(char * accesskey, char * secretkey, char * zone, char * keypair);
struct ec2_subscription * use_ec2_configuration( char * agent, char * tls, char * sptr );
char * resolve_ec2_location(struct cords_ec2_contract * cptr,struct ec2config * cfptr);
char *	resolve_ec2_image( struct ec2_subscription * subptr, struct cords_ec2_contract * cptr );
char *	resolve_ec2_flavor(struct cords_ec2_contract * cptr);
int create_ec2_contract(struct occi_category * optr, struct amazonEc2 * pptr, char * agent, char * tls);
int terminate_ec2_contract( int status, struct cords_ec2_contract * cptr );
int delete_ec2_contract(struct occi_category * optr,struct amazonEc2 * pptr,char * agent,char * tls );

#endif









