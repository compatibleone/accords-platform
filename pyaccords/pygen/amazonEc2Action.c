/*-------------------------------------------------------------------------------*/
/* ACCORDS Platform                                                              */
/* copyright 2013 ,Hamid MEDJAHE   (hmedjahed@prologue.fr) Prologue              */
/* copyright 2013 ,Jamie Marshall  (ijm667@hotmail.com) Prologue                 */
/*-------------------------------------------------------------------------------*/
/* Licensed under the Apache License, Version 2.0 (the "License");               */
/* you may not use this file except in compliance with the License.              */
/* You may obtain a copy of the License at                                       */
/*                                                                               */
/*       http://www.apache.org/licenses/LICENSE-2.0                              */
/*                                                                               */
/* Unless required by applicable law or agreed to in writing, software           */
/* distributed under the License is distributed on an "AS IS" BASIS,             */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.      */
/* See the License for the specific language governing permissions and           */
/* limitations under the License.                                                */
/*-------------------------------------------------------------------------------*/

#include "occi.h"
#include "../pysrc/ctools.h"
#include "../pysrc/ec2client.h"
#include "occipublisher.c"
#include "amazonEc2.h"
#include "cosacsctrl.h"
#include "../pysrc/pytools.h"
#include <Python.h>

/*	-------------------------------------------------	*/
/*	  e c 2 _ l a u n c h _ u s i n g _ k e y p a i r		*/
/*	-------------------------------------------------	*/
public	int	ec2_launch_using_keypair( struct amazonEc2 * pptr, char * username, char * command )
{
	if (!( pptr ))
		return( 118 );
	else if (!( command ))
		return( 118 );
	else if (!( pptr->hostname ))
		return( 118 );
	else if (!( pptr->keyfile ))
		return( 118 );
	else	return( ssh_launch_using_keypair( username, pptr->keyfile, pptr->hostname, command ) );
}
 
/*	-----------------------------------	*/
/*	e c 2 _ i n s t a l l _ c o s a c s	*/
/*	-----------------------------------	*/
private	int	ec2_install_cosacs( struct ec2_subscription * subptr, struct amazonEc2 * pptr )
{
	int	status;
	char *	hostdepot=(char *) 0;
	char *	buffer=(char *) 0;
	char *	syntax=(char *) 0;
	char *	username=(char *) 0;
	char *	version=(char *) 0;
	char *	package=(char *) 0;
	char *	sptr;

	if (!( hostdepot = get_default_depot() ))
		return( _NO_COSACS );

	/* ---------------------------------- */
	/* detect cosacs installation request */
	/* ---------------------------------- */
	if (!( sptr = pptr->agent ))
		return( _NO_COSACS );
	else if (( strncasecmp( sptr, "cosacs:install", strlen("cosacs:install") ) != 0 )
	     &&  ( strncasecmp( sptr, "install:cosacs", strlen("install:cosacs") ) != 0 ))
		return( _NO_COSACS );
	else	sptr += strlen( "cosacs:install" );

	if ( *sptr != ':' )
		return( _NO_COSACS );
	else	sptr++;

	/* -------------------------------------- */
	/* extract user, version and package info */
	/* -------------------------------------- */
	if (!( buffer = allocate_string( sptr ) ))
		return( _NO_COSACS );

	username = sptr = strcpy( buffer, sptr );

	while ( *sptr )
	{
		if ( *sptr == ':' )
		{
			*(sptr++) = 0;
			if (!( version ))
				version = sptr;
			else if (!( package ))
				package = sptr;
			else	break;
		}
		else	sptr++;
	}
	if ((!( package)) || (!( version )) || (!( username )))
	{
		liberate( buffer ) ;
		return( _NO_COSACS );
	}

	/* ---------------------------------------- */
	/* build installation command syntax string */
	/* ---------------------------------------- */
	else if (!( syntax = allocate( strlen( hostdepot ) + strlen( version ) + ( strlen( package ) * 2) + 64 ) ))
	{
		liberate( buffer ) ;
		return( _NO_COSACS );
	}
	sprintf(syntax,"wget %s/%s/%s",hostdepot,version,package);
	status = ec2_launch_using_keypair( pptr, username, syntax );
	sprintf(syntax,"bash ./%s",package);
	status = ec2_launch_using_keypair( pptr, username, syntax );

	buffer = liberate( buffer );
	syntax = liberate( syntax );

	if ( status )
		return( _NO_COSACS  );
	else	return( _USE_COSACS );
}

/*	-----------------------------------	*/
/*	   s t a r t _ a m a z o n E c 2	*/
/*	-----------------------------------	*/
public	struct rest_response * start_amazonEc2(
	struct occi_category * optr,
	struct rest_client * cptr,
	struct rest_request * rptr,
	struct rest_response * aptr,
	void * vptr )
{
	struct amazonEc2 * pptr;
	int 	status = 0;
	char * 	secgroup;
	struct 	ec2_subscription * subptr = (struct ec2_subscription*) 0;
	char	reference[512];	
	
	/* --------------------------------- */
	/* retrieve the instance information */
	/* --------------------------------- */
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 404, "ec2 Invalid Action" ) );
	else if(pptr->state == _OCCI_SUSPENDED)
	{
		if (!(subptr = use_ec2_configuration(pptr->profile)))
			return( rest_html_response (aptr, 404, "ac2 configuration not found"));
		else if((status = start_ec2_instance(subptr,pptr)!=0))
			return ( rest_html_response( aptr, status, "start ec2 vm failure"));
	}
	else if (pptr->state != _OCCI_IDLE )
		return( rest_html_response(aptr, 200, "ok ") );
	else if (!(subptr = use_ec2_configuration(pptr->profile)))
		return( rest_html_response (aptr, 404, "ac2 configuration not found"));
	else if(!(secgroup = build_ec2_firewall( subptr,pptr )))
		return ( rest_html_response( aptr, 888, "ec2 firewall failure"));
	else if((status = start_ec2_instance(subptr,pptr)!=0))
		return ( rest_html_response( aptr, status, "start ec2 vm failure"));

	sprintf(reference,"%s/%s/%s",get_identity(),_CORDS_EC2,pptr->id);

	/* ---------------------------- */
	/* launch the COSACS operations */
	/* ---------------------------- */
	switch ((pptr->agentstatus = use_cosacs_agent( pptr->agent )))
	{
	case	_INSTALL_COSACS	:
		if (!( subptr->keypair ))
		{
			pptr->agentstatus = _NO_COSACS;
			break;
		}
		else if (!( pptr->keyfile = allocate_string( subptr->keypair ) ))
		{
			pptr->agentstatus = _NO_COSACS;
			break;
		}
		else if (!( pptr->agentstatus = ec2_install_cosacs( subptr, pptr ) ))
			break;

	case	_USE_COSACS	:
		if ( cosacs_test_interface( pptr->hostname, _COSACS_START_TIMEOUT, _COSACS_START_RETRY ) )
		{
			cosacs_metadata_instructions( 
				pptr->hostname, _CORDS_CONFIGURATION,
				reference, default_publisher(), pptr->account );
		}
	}
	
	/* --------------------------- */
	/* now handle the transactions */
	/* --------------------------- */
	if (!( rest_valid_string( pptr->price ) ))
		return( rest_html_response( aptr, 200, "OK" ) );
	else if ( occi_send_transaction( _CORDS_EC2, pptr->price, "action=start", pptr->account, reference ) )
		return( rest_html_response( aptr, 200, "OK" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) ); 
}

/*	-----------------------------------	*/
/*	    s t o p  _ a m a z o n E c 2	*/
/*	-----------------------------------	*/
struct rest_response * stop_amazonEc2(
	struct occi_category * optr,
	struct rest_client * cptr,
	struct rest_request * rptr,
	struct rest_response * aptr,
	void * vptr )
{
	struct amazonEc2 * pptr;
	int status = 0;
	char reference[1024];
	 
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 404, "Invalid EC2 Action" ) );
	else if ( pptr->state == _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if ( (status=stop_ec2_provisioning( pptr )) != 0)
		return (rest_html_response(aptr,status, "Not Found"));
	else
	{
		reset_ec2_server( pptr );
		pptr->stamp = time((long *) 0);
		autosave_amazonEc2_nodes();
		sprintf(reference,"%s/%s/%s",get_identity(),_CORDS_EC2,pptr->id);
		if (!( rest_valid_string( pptr->price ) ))
			return( rest_html_response( aptr, 200, "OK" ) );
		else if ( occi_send_transaction( _CORDS_EC2, pptr->price, "action=stop", pptr->account, reference ) )
			return( rest_html_response( aptr, 200, "OK" ) );
		else	return( rest_html_response( aptr, 200, "OK" ) );
	}
	

}

//            category restart action  
struct rest_response * restart_amazonEc2(
	struct occi_category * optr,
	struct rest_client * cptr,
	struct rest_request * rptr,
	struct rest_response * aptr,
	void * vptr )
{
	struct amazonEc2 * pptr;
	int status = 0;
	
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if (( status = restart_ec2_instance(pptr)) != 0)
		return( rest_html_response( aptr, status+800, "Ec2 service failure found"));
	else
	{
		if ( pptr->state == _OCCI_SUSPENDED )
		{
			pptr->stamp = time((long *) 0);
			pptr->state = _OCCI_RUNNING;
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

//            category suspend action  
struct rest_response * suspend_amazonEc2(
	struct occi_category * optr,
	struct rest_client * cptr,
	struct rest_request * rptr,
	struct rest_response * aptr,
	void * vptr )
{
	struct amazonEc2 * pptr;
	int status = 0;
	
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if( pptr->state == _OCCI_RUNNING)
        {

        	if((status = suspend_ec2_instance(pptr))!=0)
			return (rest_html_response(aptr, status, "Ec2 instance failure"));
		else 
		{
		   	pptr->stamp = time((long *) 0);
			pptr->state = _OCCI_SUSPENDED;
			return (rest_html_response(aptr, 200, "OK"));
		}

	}
	else return (rest_html_response(aptr,404,"Invalid Action"));
}

//            category snapshot action  
struct rest_response * snapshot_amazonEc2(
	struct occi_category * optr,
	struct rest_client * cptr,
	struct rest_request * rptr,
	struct rest_response * aptr,
	void * vptr )
{
	struct amazonEc2 * pptr;
	int status = 0;
	char reference[1024];

	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state == _OCCI_IDLE )
		return( rest_html_response( aptr, 400, "Contract Not Active" ) );
	else if ((status = snapshot_ec2_instance(pptr)) != 0)
		return( rest_html_response( aptr, status, "Ec2 service failure"));
	else if (!( status ))
	{
		sprintf(reference,"%s/%s/%s",get_identity(),_CORDS_EC2,pptr->id);
		if (!( rest_valid_string( pptr->price ) ))
			return( rest_html_response( aptr, 200, "OK" ) );
		else if ( occi_send_transaction( _CORDS_EC2, pptr->price, "action=snapshot", pptr->account, reference ) )
			return( rest_html_response( aptr, 200, "OK" ) );
		else	return( rest_html_response( aptr, 200, "OK" ) );
	}
	else  	return( rest_html_response( aptr, 400, "Bad Request" ) );
}


char * amazonEc2_getname(int a)
{
  static char action[256];
  switch (a)
  {
      case 0:
          strcpy(action,"start");
          break;
      case 1:
          strcpy(action,"stop");
          break;
      case 2:
          strcpy(action, "restart");
          break;
      case 3:
          strcpy(action,"snapshot");
          break;
      case 4:
          strcpy(action,"suspend");
          break;
      default:
          strcpy(action,"amazonEc2");
          break;
   }
   return action;
}

int amazonEc2_getnumber()
{
 return 5;
}

