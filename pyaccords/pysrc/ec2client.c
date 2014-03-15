/*-------------------------------------------------------------------------------*/
/* accords platform                                                              */
/* copyright 2013, Hamid MEDJAHED (hmedjahed@prologue.fr) prologue               */
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
#ifndef	_ec2client_c
#define	_ec2client_c
#include "ctools.h"
#include "pytools.h"
#include "amazonEc2.h"
#include "ec2client.h"
#include "stdnode.h"
#include "occi.h"
#include "cordslang.h"
#include "cosacsctrl.h"
#include "ec2contract.h"
#include <Python.h>


/* ---------------------------------------------------------------------- */
/* ec2 resolve region   						  */
/* ---------------------------------------------------------------------- */
/* transforms a standard geographical identifier to the corresponding ec2 */
/* specific value.							  */
/* ---------------------------------------------------------------------- */
char * ec2_resolve_region( struct ec2_subscription * sptr, char * region )
{
	char * result;
	rest_log_message( "ec2_resolve_region" );
	rest_log_message( region );

        /* -------------------------------------- */
        /* invalid region uses subscription value */
        /* -------------------------------------- */
        if (!( rest_valid_string( region ) ))
                return( allocate_string( sptr->zone) );
        else if (!( strcasecmp( region, "any" ) ))
                return( allocate_string( sptr->zone) );
        else if (!( strcasecmp( region, "default" ) ))
                return( allocate_string( sptr->zone) );

	/* ------------------------------------------- */
	/* detect and preserve AWS EC2 standard values */
	/* ------------------------------------------- */
	if ((!( strcasecmp( region, "us-east-1" ) ))
	||  (!( strcasecmp( region, "us-west-1" ) ))
	||  (!( strcasecmp( region, "us-west-2" ) ))
	||  (!( strcasecmp( region, "eu-west-1" ) ))
	||  (!( strcasecmp( region, "eu-west-2" ) ))
	||  (!( strcasecmp( region, "eu-east-1" ) ))
	||  (!( strcasecmp( region, "eu-east-2" ) ))
	||  (!( strcasecmp( region, "ap-southeast-1" ) ))
	||  (!( strcasecmp( region, "ap-southeast-2" ) ))
	||  (!( strcasecmp( region, "ap-northeast-1" ) ))
	||  (!( strcasecmp( region, "sa-east-1" ) )))
		return( allocate_string( region ) );

	/* -------------------------------------------- */
	/* attempt to resolve via geo location category */
	/* -------------------------------------------- */
        else if ((result = occi_resolve_geolocation( "amazonEc2", "accords", region )) != (char *) 0)
                return( result );

        /* --------------------- */
        /* europe, east and west */
        /* --------------------- */
        else if (!( strcasecmp( region, "europe" ) ))
                return( allocate_string( "eu-west-1" ) );
        else if (!( strcasecmp( region, "east-europe" ) ))
                return( allocate_string( "eu-west-1" ) );
        else if (!( strcasecmp( region, "west-europe" ) ))
                return( allocate_string( "eu-west-1" ) );

        /* --------------------- */
        /* states, east and west */
        /* --------------------- */
        else if (!( strcasecmp( region, "america" ) ))
                return( allocate_string( "us-west-1" ) );
        else if (!( strcasecmp( region, "north-america" ) ))
                return( allocate_string( "us-west-2" ) );
        else if (!( strcasecmp( region, "south-america" ) ))
                return( allocate_string( "sa-east-1" ) );
        else if (!( strcasecmp( region, "us-east" ) ))
                return( allocate_string( "us-east-1" ) );
        else if (!( strcasecmp( region, "us-west" ) ))
                return( allocate_string( "us-west-2" ) );

        /* ------------------- */
        /* asia, east and west */
        /* ------------------- */
        else if (!( strcasecmp( region, "asia" ) ))
                return( allocate_string( "ap-southeast-1" ) );
        else if (!( strcasecmp( region, "north-asia" ) ))
                return( allocate_string( "ap-northeast-1" ) );
        else if (!( strcasecmp( region, "east-asia" ) ))
                return( allocate_string( "ap-southeast-2" ) );
        else if (!( strcasecmp( region, "south-asia" ) ))
                return( allocate_string( "ap-southeast-2" ) );
        else if (!( strcasecmp( region, "west-asia" ) ))
                return( allocate_string( "ap-southeast-1" ) );

        /* ---------------------- */
        /* africa north and south */
        /* ---------------------- */
        else if (!( strcasecmp( region, "africa" ) ))
                return( allocate_string( "eu-west-1" ) );
        else if (!( strcasecmp( region, "north-africa" ) ))
                return( allocate_string( "eu-west-1" ) );
        else if (!( strcasecmp( region, "south-africa" ) ))
                return( allocate_string( "ap-southeast-2" ) );

        /* -------------------------------------- */
        /* invalid region uses subscription value */
        /* -------------------------------------- */
        else    return( allocate_string( sptr->zone ) );
}

/*	------------	*/
/*	set ec2 zone	*/
/*	------------	*/
char * set_ec2_zone(struct ec2_subscription* subptr, char * zone)
{
	char 	buffer[2048];
	int	i;
	if ( zone )
	{
		if ( subptr->zone )
			subptr->zone = liberate( subptr->zone );
		subptr->zone = allocate_string( zone );
		if ( subptr->keypair )
		{
			strcpy( buffer, subptr->keypair );
			for ( i=0; buffer[i] != 0; i++ )
				if ( buffer[i] == '.' )
					break;
			buffer[i] = 0;
			strcat( buffer,".");
			strcat( buffer,zone);
			strcat( buffer,".pub" );
			subptr->keypair = liberate( subptr->keypair );
			subptr->keypair = allocate_string( buffer );
			rest_log_message( subptr->keypair );
		}
	}
	return( subptr->zone );
}

/* -------------------------------------------------------------------- */
/* get ec2 location                                                     */
/* ---------------------------------------------------------------------*/
char * get_ec2_zone(struct ec2_subscription* subptr, char * zone)
{
	char srcdir[1024];
  	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;
	PyThreadState* pythr=NULL;
	char * response;

	//python interface	
	sprintf(srcdir,"%s/pyaccords/pysrc",PYPATH);
	PyEval_AcquireLock();
	if((pythr = Py_NewInterpreter())== NULL)
	{
		rest_log_message("interpreter init error \n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
	}
	PyThreadState_Swap(pythr);
	python_path(srcdir);
	if((pName = PyString_FromString("ec2client")) == NULL)
	{
		rest_log_message("erro: in ec2client.py no such file name\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;

	}
	else if((pModule = PyImport_Import(pName)) == NULL)
	{
		rest_log_message("error: failed to load ec2client module\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;

	}
	else if((pDict = PyModule_GetDict(pModule)) == NULL)
	{
		rest_log_message("error: failed to load dict name in ec2client module\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
	}
	else if((pFunc = PyDict_GetItemString(pDict,"ec2_get_zone")) == NULL)
	{
		rest_log_message("error: failed to load ec2_get_zone function in ec2client module\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
	}
	else result=PyObject_CallFunction(pFunc,"sss",subptr->accesskey,subptr->secretkey,zone);
	if (!result || PyErr_Occurred())
        {
       		PyErr_Print();
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
       		return (0);
       	}

	response=allocate_string(PyString_AsString( result ));
	Py_DECREF(pModule);
	Py_DECREF(pName);

        PyEval_ReleaseThread(pythr); 
        PyEval_AcquireThread(pythr);
	Py_EndInterpreter(pythr);
        PyEval_ReleaseLock();

	return response;
}

/* ---------------------------------------------------------------------------*/
/*             r e s o l v e  e c 2  i m a g e                                */
/* ---------------------------------------------------------------------------*/
char * get_ec2_imgname(struct ec2_subscription* subptr, char *imgname,char * zone)
{
	char srcdir[1024];
  	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;
	PyThreadState* pythr=NULL;
	char * response;
	//python interface	
        sprintf(srcdir,"%s/pyaccords/pysrc",PYPATH);
	PyEval_AcquireLock();

	if((pythr = Py_NewInterpreter())== NULL)
	{
		rest_log_message("interpreter init error \n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
	}
	PyThreadState_Swap(pythr);
	python_path(srcdir);
	if((pName = PyString_FromString("ec2client")) == NULL)
	{
		rest_log_message("erro: in ec2client.py no such file name\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;

	}
	else if((pModule = PyImport_Import(pName)) == NULL)
	{
		rest_log_message("error: failed to load ec2client module\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;

	}
	else if((pDict = PyModule_GetDict(pModule)) == NULL)
	{
		rest_log_message("error: failed to load dict name in ec2client module\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
	}
	else if((pFunc = PyDict_GetItemString(pDict,"ec2_get_image")) == NULL)
	{
		rest_log_message("error: failed to load ec2_get_image function in ec2client module\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
	}
	else result=PyObject_CallFunction(pFunc,"ssss",subptr->accesskey,subptr->secretkey,imgname,zone);
	if (!result || PyErr_Occurred())
        {
       		PyErr_Print();
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
       	}

	response=allocate_string(PyString_AsString( result ));
	Py_DECREF(pModule);
	Py_DECREF(pName);
	
        PyEval_ReleaseThread(pythr); 
        PyEval_AcquireThread(pythr);
	Py_EndInterpreter(pythr);
        PyEval_ReleaseLock();

	return response;
}

/*	-----------------------------------------------------------------	*/
/*		r e s o l v e _ c o n t r a c t _ f l a v o r  			*/
/*	-----------------------------------------------------------------	*/
char * get_ec2_flavor(int memory, int cores, int speed, int storage, char * architecture)
{
	char srcdir[1024];
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;
	PyThreadState* pythr=NULL;
	char * response;

	//python interface	
        sprintf(srcdir,"%s/pyaccords/pysrc",PYPATH);
	PyEval_AcquireLock();

	if((pythr = Py_NewInterpreter())== NULL)
	{
		rest_log_message("interpreter init error \n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
	}
	PyThreadState_Swap(pythr);
	python_path(srcdir);
	if((pName = PyString_FromString("ec2client")) == NULL)
	{
		rest_log_message("erro: in ec2client.py no such file name\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;

	}
	else if((pModule = PyImport_Import(pName)) == NULL)
	{
		rest_log_message("error: failed to load ec2client module\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;

	}
	else if((pDict = PyModule_GetDict(pModule)) == NULL)
	{
		rest_log_message("error: failed to load dict name in ec2client module\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
	}
	else if((pFunc = PyDict_GetItemString(pDict,"ec2_get_flavor")) == NULL)
	{
		rest_log_message("error: failed to load ec2_get_flavor function in ec2client module\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
	}
	else result=PyObject_CallFunction(pFunc,"iiiis",memory,cores,speed,storage,architecture);
	if (!result || PyErr_Occurred())
        {
       		PyErr_Print();
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
       	}

	response=allocate_string(PyString_AsString( result ));
	Py_DECREF(pModule);
	Py_DECREF(pName);

	PyEval_ReleaseThread(pythr); 
        PyEval_AcquireThread(pythr);
	Py_EndInterpreter(pythr);
        PyEval_ReleaseLock();

	return response;
}

char * create_ec2_secgroup(struct ec2_subscription * subptr, struct amazonEc2 * pptr)
{
 	char srcdir[1024];
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;
	PyThreadState* pythr=NULL;
	char * response;
	
	//python interface	
        sprintf(srcdir,"%s/pyaccords/pysrc",PYPATH);
	PyEval_AcquireLock();

	if((pythr = Py_NewInterpreter())== NULL)
	{
		rest_log_message("interpreter init error \n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
	}
	PyThreadState_Swap(pythr);
	python_path(srcdir);
	if((pName = PyString_FromString("ec2client")) == NULL)
	{
		rest_log_message("erro: in ec2client.py no such file name\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;

	}
	else if((pModule = PyImport_Import(pName)) == NULL)
	{
		rest_log_message("error: failed to load ec2client module\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;

	}
	else if((pDict = PyModule_GetDict(pModule)) == NULL)
	{
		rest_log_message("error: failed to load dict name in ec2client module\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
	}
	else if((pFunc = PyDict_GetItemString(pDict,"ec2_create_secgroup")) == NULL)
	{
		rest_log_message("error: failed to load ec2_create_secgroup function in ec2client module\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
	}
	else result=PyObject_CallFunction(pFunc,"ssss",subptr->accesskey,subptr->secretkey,subptr->zone,pptr->firewall);
	if (!result || PyErr_Occurred())
        {
       		PyErr_Print();
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
       	}

	response=allocate_string(PyString_AsString( result ));
	Py_DECREF(pModule);
	Py_DECREF(pName);

	PyEval_ReleaseThread(pythr); 
        PyEval_AcquireThread(pythr);
	Py_EndInterpreter(pythr);
        PyEval_ReleaseLock();

	return response;
}

char * add_ec2_rule(struct ec2_subscription * subptr, char * group, char * rulename, int fport, int tport, char * protocol)
{
	char srcdir[1024];
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;
	PyThreadState* pythr=NULL;
	char * response;

	//python interface	
        sprintf(srcdir,"%s/pyaccords/pysrc",PYPATH);
	PyEval_AcquireLock();

	if((pythr = Py_NewInterpreter())== NULL)
	{
		rest_log_message("interpreter init error \n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
	}
	PyThreadState_Swap(pythr);
	python_path(srcdir);
	if((pName = PyString_FromString("ec2client")) == NULL)
	{
		rest_log_message("erro: in ec2client.py no such file name\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;

	}
	else if((pModule = PyImport_Import(pName)) == NULL)
	{
		rest_log_message("error: failed to load ec2client module\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;

	}
	else if((pDict = PyModule_GetDict(pModule)) == NULL)
	{
		rest_log_message("error: failed to load dict name in ec2client module\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
	}
	else if((pFunc = PyDict_GetItemString(pDict,"ec2_add_rule")) == NULL)
	{
		rest_log_message("error: failed to load ec2_add_rule function in ec2client module\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
	}
	else result=PyObject_CallFunction(pFunc,"sssssiis",subptr->accesskey,subptr->secretkey,subptr->zone,group,rulename,fport,tport,protocol);
	if (!result || PyErr_Occurred())
        {
       		PyErr_Print();
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
       	}

	response=allocate_string(PyString_AsString( result ));
	Py_DECREF(pModule);
	Py_DECREF(pName);
	
	PyEval_ReleaseThread(pythr); 
        PyEval_AcquireThread(pythr);
	Py_EndInterpreter(pythr);
        PyEval_ReleaseLock();

	return response;
}

char * delete_ec2_secgroup(struct ec2_subscription * subptr, struct amazonEc2 * pptr)
{
 	char srcdir[1024];
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;
	PyThreadState* pythr=NULL;
	char * response;

	//python interface	
        sprintf(srcdir,"%s/pyaccords/pysrc",PYPATH);
	PyEval_AcquireLock();

	if((pythr = Py_NewInterpreter())== NULL)
	{
		rest_log_message("interpreter init error \n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
	}
	PyThreadState_Swap(pythr);
	python_path(srcdir);
	if((pName = PyString_FromString("ec2client")) == NULL)
	{
		rest_log_message("erro: in ec2client.py no such file name\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;

	}
	else if((pModule = PyImport_Import(pName)) == NULL)
	{
		rest_log_message("error: failed to load ec2client module\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;

	}
	else if((pDict = PyModule_GetDict(pModule)) == NULL)
	{
		rest_log_message("error: failed to load dict name in ec2client module\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
	}
	else if((pFunc = PyDict_GetItemString(pDict,"ec2_delete_secgroup")) == NULL)
	{
		rest_log_message("error: failed to load ec2_delete_secgroup function in ec2client module\n");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
	}
	else result=PyObject_CallFunction(pFunc,"ssss",subptr->accesskey,subptr->secretkey,subptr->zone,pptr->firewall);
	if (!result || PyErr_Occurred())
        {
       		PyErr_Print();
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (char*) 0;
       	}

	response=allocate_string(PyString_AsString( result ));
	Py_DECREF(pModule);
	Py_DECREF(pName);
	
	PyEval_ReleaseThread(pythr); 
        PyEval_AcquireThread(pythr);
	Py_EndInterpreter(pythr);
        PyEval_ReleaseLock();
	
	return response;
}

char * build_ec2_firewall(struct ec2_subscription * subptr, struct amazonEc2 * pptr)
{
	int	status;
	char *	sptr;
	char *	rulename;
	char *	rulefrom;
	char *	ruleproto;
	char *	ruleto;
	struct	occi_element * eptr;
	struct	standard_message firewall;
	struct	standard_message port;
	char * secgroup;
        char * response;

	memset( &firewall, 0, sizeof(struct standard_message));
	memset( &port, 0, sizeof(struct standard_message));

	/* ---------------------------------------------------- */
	/* prepare the file containing the firewall description */
	/* ---------------------------------------------------- */
	if ((status = get_standard_message( &firewall, pptr->firewall, _CORDS_CONTRACT_AGENT, default_tls() )) != 0)
		return( (char*) 0 );
	else if(!(pptr->group = create_ec2_secgroup(subptr,pptr)))
			return((char *)0 );
	{
		secgroup = pptr->group;
		/* -------------------------- */
		/* create the security group  */
		/* -------------------------- */
		for (	eptr = first_standard_message_link( firewall.message );
			eptr != (struct occi_element *) 0;
			eptr = next_standard_message_link( eptr ) )
		{
			/* ------------------------ */
			/* retrieve the port record */
			/* ------------------------ */
			if (!( sptr = standard_message_link_value( eptr->value )))
				continue;
			else if ((status = get_standard_message( &port, sptr, _CORDS_CONTRACT_AGENT, default_tls() )) != 0)
				continue;

			/* ---------------------------------- */
			/* retrieve the port rule information */
			/* ---------------------------------- */
			if ((!( rulename = occi_extract_atribut( port.message, "occi", 
				_CORDS_PORT, _CORDS_NAME ) ))
			||  (!( ruleproto = occi_extract_atribut( port.message, "occi", 
				_CORDS_PORT, _CORDS_PROTOCOL ) ))
			||  (!( rulefrom = occi_extract_atribut( port.message, "occi", 
				_CORDS_PORT, _CORDS_FROM ) ))
			||  (!( ruleto = occi_extract_atribut( port.message, "occi", 
				_CORDS_PORT, _CORDS_TO   ) )) )
			{
				release_standard_message( &port );
				release_standard_message( &firewall );
				break;
			}
			else if (!(response = add_ec2_rule( subptr, secgroup, rulename, atoi(rulefrom), atoi(ruleto), ruleproto )))
				return response;
		}
		
	}
	return( secgroup );
}


int start_ec2_instance(struct ec2_subscription * subptr, struct amazonEc2 * pptr)
{
	char sendstr[1024]=" ";
	char strtmp[1024]=" ";
	char * strcmptr = NULL;
	int  status= 0;
        char srcdir[1024];
	char * response = NULL;
	char * token;

	listcc categoryAtr;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;
	PyThreadState* pythr=NULL;

	if(!(strValid(pptr->id))) strcpy(sendstr," ");
	else strcpy(sendstr,pptr->id);
	
	if(!(strValid(pptr->name))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->name,',');
	
	if(!(strValid(pptr->flavor))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->flavor,',');
	
	if(!(strValid(pptr->image))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->image,',');
	
	if(!(strValid(pptr->original))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->original,',');

	if(!(strValid(pptr->profile))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->profile,',');

	 if(!(strValid(pptr->node))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->node,',');
	
	if(!(strValid(pptr->price))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->price,',');

	if(!(strValid(pptr->account))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->account,',');

	if(!(strValid(pptr->number))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->number,',');

	if(!(strValid(pptr->rootpass))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->rootpass,',');

	if(!(strValid(pptr->reference))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->reference,',');

	if(!(strValid(pptr->network))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->network,',');

	if(!(strValid(pptr->access))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->access,',');

	if(!(strValid(pptr->accessip))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->accessip,',');

	if(!(strValid(pptr->keypair))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->keypair,',');

	if(!(strValid(pptr->placementgroup))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->placementgroup,',');

	if(!(strValid(pptr->publicaddr))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->publicaddr,',');

	 if(!(strValid(pptr->privateaddr))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->privateaddr,',');

	if(!(strValid(pptr->firewall))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->firewall,',');

	if(!(strValid(pptr->group))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->group,',');

	if(!(strValid(pptr->zone))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->zone,',');

	if(!(strValid(pptr->hostname))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->hostname,',');

	if(!(strValid(pptr->workload))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->workload,',');

	if(!(strValid(pptr->agent))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->agent,',');

	sprintf(strtmp,"%d",pptr->stamp);
	strcmptr = allocate_string(strtmp);
	if(strValid(strcmptr)) strConcat(sendstr,strtmp,',');
	else strConcat(sendstr," ",',');
	
	sprintf(strtmp,"%d",pptr->state);
	strcmptr = allocate_string(strtmp);
	if(strValid(strcmptr)) strConcat(sendstr,strtmp,',');
	else strConcat(sendstr," ",',');
	
	if(!(strValid(pptr->keyfile))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->keyfile,',');

	sprintf(strtmp,"%d",pptr->agentstatus);
	strcmptr = allocate_string(strtmp);
	if(strValid(strcmptr)) strConcat(sendstr,strtmp,',');
	else strConcat(sendstr," ",',');

	//           python interface
	sprintf(srcdir,"%s/pyaccords/pygen",PYPATH);
	PyEval_AcquireLock();
	pythr = Py_NewInterpreter();
        if(pythr == NULL) printf("interpreter init error \n");
	PyThreadState_Swap(pythr);
	python_path(srcdir);
	if((pName = PyString_FromString("amazonEc2Act")) == NULL) 
	{
		rest_log_message("error: in amazonEc2Act.py no such file name");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock(); 
		return (1);
	}
	else if((pModule = PyImport_Import(pName)) == NULL) 
	{
		rest_log_message("error: failed to load amazonEc2Act module");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock(); 
		return (1);	
	}
	else if((pDict = PyModule_GetDict(pModule)) == NULL) 
	{
		rest_log_message("error: failed to load dict name in amazonEc2Act module");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock(); 
		return (1);
	}
	else if((pFunc = PyDict_GetItemString(pDict,"start")) == NULL) 
	{
		rest_log_message("error: failed to load start function in amazonEc2Act module");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock(); 
		return (1);
	}
	else result=PyObject_CallFunction(pFunc,"sssss",subptr->accesskey,subptr->secretkey,subptr->zone,subptr->keypair,sendstr);
	
	if (!result || PyErr_Occurred())
        {
       		PyErr_Print();
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock(); 
		return (1);
       	}

	response=allocate_string(PyString_AsString( result ));
	Py_DECREF(pModule);
	Py_DECREF(pName);
	
	PyEval_ReleaseThread(pythr); 
	PyEval_AcquireThread(pythr);
	Py_EndInterpreter(pythr);
	PyEval_ReleaseLock(); 

	resetListe(&categoryAtr);
	token= strtok(response,",");
	for(; token != NULL ;)
	{
		addBacke(&categoryAtr,token);
		token=strtok(NULL, ",");
	}
	elemm *pelem = categoryAtr.first;
	if(pelem){
		pptr->id = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->name = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->flavor = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->image = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->original = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->profile = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->node = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->price = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->account = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->number = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->rootpass = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->reference = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->network = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->access = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->accessip = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->keypair = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->placementgroup = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->publicaddr = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->privateaddr = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->firewall = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->group = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->zone = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->hostname = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->workload = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
        if(pelem){
		pptr->agent = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->stamp = atoi(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->state = atoi(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->keyfile = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->agentstatus = atoi(pelem->value);
	}
        
        liberateListe(&categoryAtr);
	return status;
}

/*	-----------------------------------------------------------------	*/
/*	   s t o p _ E C 2 _ p r o v i s i o n i n g	                	*/
/*	-----------------------------------------------------------------	*/
int stop_ec2_provisioning( struct amazonEc2 * pptr )
{
	struct	ec2_subscription * subptr = (struct ec2_subscription *) 0;
	char 	reference[2024];
	int     status = 0;
	char srcdir[1024];
	char * response;
	char * token;
	char sendstr[1024]=" ";
	char strtmp[1024]=" ";
	char * strcmptr = NULL;
	listcc	categoryAtr;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;
	PyThreadState* pythr=NULL;

	/* ------------------------ */
	/* prepare the subscription */
	/* ------------------------ */
	if (!( pptr ))
		return(118);
	else if ( pptr->state == _OCCI_IDLE )
		return(0);
	else if (!(subptr = use_ec2_configuration( pptr->profile )))
		return(118);
	else if (!( set_ec2_zone( subptr, pptr->zone ) ))
		return( 118 );
	else	sprintf(reference,"%s/%s/%s",get_identity(),_CORDS_EC2,pptr->id);

	/* ------------------------------------------- */
	/* perform pre-release actions for destruction */
	/* ------------------------------------------- */
	if ( pptr->agentstatus == _USE_COSACS )
	{
		if ( cosacs_test_interface( pptr->hostname, _COSACS_STOP_TIMEOUT, _COSACS_STOP_RETRY ) )
		{
			cosacs_metadata_instructions( 
				pptr->hostname, _CORDS_RELEASE,
				reference, default_publisher(), pptr->account );
		}
	}
		
	if(!(strValid(pptr->id))) strcpy(sendstr," ");
	else strcpy(sendstr,pptr->id);

	if(!(strValid(pptr->name))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->name,',');

	if(!(strValid(pptr->flavor))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->flavor,',');
	
	if(!(strValid(pptr->image))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->image,',');
	
	if(!(strValid(pptr->original))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->original,',');
	
	if(!(strValid(pptr->profile))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->profile,',');

	if(!(strValid(pptr->node))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->node,',');
	
	if(!(strValid(pptr->price))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->price,',');

	if(!(strValid(pptr->account))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->account,',');

	if(!(strValid(pptr->number))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->number,',');

	if(!(strValid(pptr->rootpass))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->rootpass,',');

	if(!(strValid(pptr->reference))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->reference,',');

	if(!(strValid(pptr->network))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->network,',');

	if(!(strValid(pptr->access))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->access,',');

	if(!(strValid(pptr->accessip))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->accessip,',');

	if(!(strValid(pptr->keypair))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->keypair,',');

	if(!(strValid(pptr->placementgroup))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->placementgroup,',');

	if(!(strValid(pptr->publicaddr))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->publicaddr,',');

	 if(!(strValid(pptr->privateaddr))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->privateaddr,',');

	 if(!(strValid(pptr->firewall))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->firewall,',');

	if(!(strValid(pptr->group))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->group,',');

	if(!(strValid(pptr->zone))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->zone,',');

	if(!(strValid(pptr->hostname))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->hostname,',');

	if(!(strValid(pptr->workload))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->workload,',');

	if(!(strValid(pptr->agent))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->agent,',');

	sprintf(strtmp,"%d",pptr->stamp);
	if(strValid(strtmp)) strConcat(sendstr,strtmp,',');
	else strConcat(sendstr," ",',');
	
	sprintf(strtmp,"%d",pptr->state);
	if(strValid(strtmp)) strConcat(sendstr,strtmp,',');
	else strConcat(sendstr," ",',');

	if(!(strValid(pptr->keyfile))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->keyfile,',');

	sprintf(strtmp,"%d",pptr->agentstatus);
	strcmptr = allocate_string(strtmp);
	if(strValid(strcmptr)) strConcat(sendstr,strtmp,',');
	else strConcat(sendstr," ",',');


	//           python interface
	sprintf(srcdir,"%s/pyaccords/pygen",PYPATH);
	PyEval_AcquireLock();
	pythr = Py_NewInterpreter();
        if(pythr == NULL) printf("interpreter init error \n");
	PyThreadState_Swap(pythr);
	python_path(srcdir);
	if((pName = PyString_FromString("amazonEc2Act")) == NULL) 
	{
		rest_log_message("error: in amazonEc2Act.py no such file name");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock(); 
		return (1);
	}	
	else if((pModule = PyImport_Import(pName)) == NULL) 
	{
		rest_log_message("error: failed to load amazonEc2Act module");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock(); 
		return (1);
	}
	else if((pDict = PyModule_GetDict(pModule)) == NULL) 
	{
		rest_log_message("error: failed to load dict name in amazonEc2Act module");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (1); 
	}
	else if((pFunc = PyDict_GetItemString(pDict,"stop")) == NULL) 
	{
		rest_log_message("error: failed to load start function in amazonEc2Act module");
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();
		return (1); 
	}
	else result=PyObject_CallFunction(pFunc,"ssss",subptr->accesskey,subptr->secretkey,subptr->zone,sendstr);
	
	if (!result || PyErr_Occurred())
        {
       		PyErr_Print();
		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock(); 
       		return (1);
       	}

	response=allocate_string(PyString_AsString( result ));
	Py_DECREF(pModule);
	Py_DECREF(pName);

	PyEval_ReleaseThread(pythr); 
	PyEval_AcquireThread(pythr);
	Py_EndInterpreter(pythr);
	PyEval_ReleaseLock();  

	resetListe(&categoryAtr);
	token= strtok(response,",");
	for(; token != NULL ;)
	{
		addBacke(&categoryAtr,token);
		token=strtok(NULL, ",");
	}
	elemm *pelem = categoryAtr.first;
	if(pelem){
		pptr->id = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->name = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->flavor = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->image = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->original = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->profile = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->node = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->price = allocate_string(allocate_string(pelem->value));
		pelem = pelem->next;
	}
	if(pelem){
		pptr->account = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->number = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->rootpass = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->reference = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->network = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->access = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->accessip = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->keypair = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->placementgroup = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->publicaddr = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->privateaddr = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->firewall = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->group = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->zone = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->hostname = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->workload = allocate_string(pelem->value);
		pelem = pelem->next;
	}
        if(pelem){
		pptr->agent = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->stamp = atoi(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->state = atoi(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->keyfile = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->agentstatus = atoi(pelem->value);
	}

	//delete_ec2_secgroup(subptr, pptr);
	liberateListe(&categoryAtr);
	return status;

}

/*	--------------------------------------------------------	*/
/* 	       r e s e t _ E C 2 _ s e r v e r		                */
/*	--------------------------------------------------------	*/
int	reset_ec2_server( struct amazonEc2 * pptr )
{
	if ( pptr )
	{
		if ( pptr->number ) pptr->number = liberate( pptr->number );
		if ( pptr->hostname ) pptr->hostname = liberate( pptr->hostname );
		if ( pptr->reference ) pptr->reference = liberate( pptr->reference );
		if ( pptr->rootpass ) pptr->rootpass = liberate( pptr->rootpass );
		if ( pptr->publicaddr ) pptr->publicaddr = liberate( pptr->publicaddr );
		if ( pptr->privateaddr ) pptr->privateaddr = liberate( pptr->privateaddr );
		pptr->number = allocate_string("");
		pptr->hostname = allocate_string("");
		pptr->rootpass  = allocate_string("");
		pptr->publicaddr = allocate_string("");
		pptr->privateaddr = allocate_string("");
		pptr->state = _OCCI_IDLE;
	}
	return(0);
}


/*	-----------------------------------------------------------------	*/
/*	   r e s t a r t _ E C 2 _ i n s t a n c e	                	*/
/*	-----------------------------------------------------------------	*/
int	restart_ec2_instance( struct amazonEc2 * pptr )
{
	struct	ec2_subscription * subptr = (struct ec2_subscription *) 0;
	int     status = 0;
	char srcdir[1024];
	char * response;
	char * token;
	char sendstr[1024]=" ";
	char strtmp[1024]=" ";
	char * strcmptr = NULL;
	listcc	categoryAtr;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;
	PyThreadState* pythr=NULL;

	/* ------------------------ */
	/* prepare the subscription */
	/* ------------------------ */
	if (!( pptr ))
		return(118);
	else if (!(subptr = use_ec2_configuration(pptr->profile )))
		return(118);
	else if (!( set_ec2_zone( subptr, pptr->zone ) ))
		return( 118 );
		
	if(!(strValid(pptr->id))) strcpy(sendstr," ");
	else strcpy(sendstr,pptr->id);

	if(!(strValid(pptr->name))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->name,',');

	if(!(strValid(pptr->flavor))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->flavor,',');
	
	if(!(strValid(pptr->image))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->image,',');
	
	if(!(strValid(pptr->original))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->original,',');
	
	if(!(strValid(pptr->profile))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->profile,',');

	if(!(strValid(pptr->node))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->node,',');
	
	if(!(strValid(pptr->price))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->price,',');

	if(!(strValid(pptr->account))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->account,',');

	if(!(strValid(pptr->number))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->number,',');

	if(!(strValid(pptr->rootpass))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->rootpass,',');

	if(!(strValid(pptr->reference))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->reference,',');

	if(!(strValid(pptr->network))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->network,',');

	if(!(strValid(pptr->access))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->access,',');

	if(!(strValid(pptr->accessip))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->accessip,',');

	if(!(strValid(pptr->keypair))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->keypair,',');

	if(!(strValid(pptr->placementgroup))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->placementgroup,',');

	if(!(strValid(pptr->publicaddr))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->publicaddr,',');

	 if(!(strValid(pptr->privateaddr))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->privateaddr,',');

	 if(!(strValid(pptr->firewall))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->firewall,',');

	if(!(strValid(pptr->group))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->group,',');

	if(!(strValid(pptr->zone))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->zone,',');

	if(!(strValid(pptr->hostname))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->hostname,',');

	if(!(strValid(pptr->workload))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->workload,',');

	if(!(strValid(pptr->agent))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->agent,',');

	sprintf(strtmp,"%d",pptr->stamp);
	if(strValid(strtmp)) strConcat(sendstr,strtmp,',');
	else strConcat(sendstr," ",',');
	
	sprintf(strtmp,"%d",pptr->state);
	if(strValid(strtmp)) strConcat(sendstr,strtmp,',');
	else strConcat(sendstr," ",',');

	if(!(strValid(pptr->keyfile))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->keyfile,',');

	sprintf(strtmp,"%d",pptr->agentstatus);
	strcmptr = allocate_string(strtmp);
	if(strValid(strcmptr)) strConcat(sendstr,strtmp,',');
	else strConcat(sendstr," ",',');
	

	//           python interface
	sprintf(srcdir,"%s/pyaccords/pygen",PYPATH);
	PyEval_AcquireLock();
	pythr = Py_NewInterpreter();
        if(pythr == NULL) printf("interpreter init error \n");
	PyThreadState_Swap(pythr);
	python_path(srcdir);
	pName = PyString_FromString("amazonEc2Act");
	if(pName == NULL) printf("erro: in amazonEc2Act.py no such file name\n");
	else pModule = PyImport_Import(pName);
	if(pModule == NULL) printf("error: failed to load amazonEc2Act module\n");
	else pDict = PyModule_GetDict(pModule);
	if(pDict == NULL) printf("error: failed to load dict name in amazonEc2Act module\n");
	else pFunc = PyDict_GetItemString(pDict,"restart");
	if(pFunc == NULL) printf("error: failed to load start function in amazonEc2Act module\n");
	else result=PyObject_CallFunction(pFunc,"ssss",subptr->accesskey,subptr->secretkey,subptr->zone,sendstr);
		
	if (!result || PyErr_Occurred())
        {
       		PyErr_Print();
       		return (0);
       	}

	response=allocate_string(PyString_AsString( result ));
	Py_DECREF(pModule);
	Py_DECREF(pName);

	PyEval_ReleaseThread(pythr); 
	PyEval_AcquireThread(pythr);
	Py_EndInterpreter(pythr);
	PyEval_ReleaseLock(); 

	resetListe(&categoryAtr);
	token= strtok(response,",");
	for(; token != NULL ;)
	{
		addBacke(&categoryAtr,token);
		token=strtok(NULL, ",");
	}
	elemm *pelem = categoryAtr.first;
	if(pelem){
		pptr->id = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->name = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->flavor = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->image = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->original = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->profile = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->node = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->price = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->account = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->number = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->rootpass = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->reference = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->network = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->access = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->accessip = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->keypair = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->placementgroup = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->publicaddr = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->privateaddr = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->firewall = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->group = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->zone = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->hostname = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->workload = allocate_string(pelem->value);
		pelem = pelem->next;
	}
        if(pelem){
		pptr->agent = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->stamp = atoi(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->state = atoi(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->keyfile = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->agentstatus = atoi(pelem->value);
	}
	liberateListe(&categoryAtr);
	return status;

}

/*	-----------------------------------------------------------------	*/
/*	   s u s p e n d _ E C 2 _ i n s t a n c e	                	*/
/*	-----------------------------------------------------------------	*/
int suspend_ec2_instance( struct amazonEc2 * pptr )
{
	struct	ec2_subscription * subptr = (struct ec2_subscription *) 0;
	int     status = 0;
	char srcdir[1024];
	char * response;
	char * token;
	char sendstr[1024]=" ";
	char strtmp[1024]=" ";
	char * strcmptr = NULL;
	listcc	categoryAtr;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;
	PyThreadState* pythr=NULL;

	/* ------------------------ */
	/* prepare the subscription */
	/* ------------------------ */
	if (!( pptr ))
		return(118);
	else if (!(subptr = use_ec2_configuration(pptr->profile )))
		return(118);
	else if (!( set_ec2_zone( subptr, pptr->zone ) ))
		return( 118 );
		
	if(!(strValid(pptr->id))) strcpy(sendstr," ");
	else strcpy(sendstr,pptr->id);

	if(!(strValid(pptr->name))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->name,',');

	if(!(strValid(pptr->flavor))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->flavor,',');
	
	if(!(strValid(pptr->image))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->image,',');
	
	if(!(strValid(pptr->original))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->original,',');
	
	if(!(strValid(pptr->profile))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->profile,',');

	if(!(strValid(pptr->node))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->node,',');
	
	if(!(strValid(pptr->price))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->price,',');

	if(!(strValid(pptr->account))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->account,',');

	if(!(strValid(pptr->number))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->number,',');

	if(!(strValid(pptr->rootpass))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->rootpass,',');

	if(!(strValid(pptr->reference))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->reference,',');

	if(!(strValid(pptr->network))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->network,',');

	if(!(strValid(pptr->access))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->access,',');

	if(!(strValid(pptr->accessip))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->accessip,',');

	if(!(strValid(pptr->keypair))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->keypair,',');

	if(!(strValid(pptr->placementgroup))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->placementgroup,',');

	if(!(strValid(pptr->publicaddr))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->publicaddr,',');

	 if(!(strValid(pptr->privateaddr))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->privateaddr,',');

	 if(!(strValid(pptr->firewall))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->firewall,',');

	if(!(strValid(pptr->group))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->group,',');

	if(!(strValid(pptr->zone))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->zone,',');

	if(!(strValid(pptr->hostname))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->hostname,',');

	if(!(strValid(pptr->workload))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->workload,',');

	if(!(strValid(pptr->agent))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->agent,',');

	sprintf(strtmp,"%d",pptr->stamp);
	if(strValid(strtmp)) strConcat(sendstr,strtmp,',');
	else strConcat(sendstr," ",',');
	
	sprintf(strtmp,"%d",pptr->state);
	if(strValid(strtmp)) strConcat(sendstr,strtmp,',');
	else strConcat(sendstr," ",',');

	if(!(strValid(pptr->keyfile))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->keyfile,',');

	sprintf(strtmp,"%d",pptr->agentstatus);
	strcmptr = allocate_string(strtmp);
	if(strValid(strcmptr)) strConcat(sendstr,strtmp,',');
	else strConcat(sendstr," ",',');
	

	//           python interface
	sprintf(srcdir,"%s/pyaccords/pygen",PYPATH);
	PyEval_AcquireLock();
	pythr = Py_NewInterpreter();
        if(pythr == NULL) printf("interpreter init error \n");
	PyThreadState_Swap(pythr);
	python_path(srcdir);
	pName = PyString_FromString("amazonEc2Act");
	if(pName == NULL) printf("erro: in amazonEc2Act.py no such file name\n");
	else pModule = PyImport_Import(pName);
	if(pModule == NULL) printf("error: failed to load amazonEc2Act module\n");
	else pDict = PyModule_GetDict(pModule);
	if(pDict == NULL) printf("error: failed to load dict name in amazonEc2Act module\n");
	else pFunc = PyDict_GetItemString(pDict,"suspend");
	if(pFunc == NULL) printf("error: failed to load start function in amazonEc2Act module\n");
	else result=PyObject_CallFunction(pFunc,"ssss",subptr->accesskey,subptr->secretkey,subptr->zone,sendstr);
	
	if (!result || PyErr_Occurred())
        {
       		PyErr_Print();
       		return (0);
       	}

	response=allocate_string(PyString_AsString( result ));
	Py_DECREF(pModule);
	Py_DECREF(pName);
	
	PyEval_ReleaseThread(pythr); 
	PyEval_AcquireThread(pythr);
	Py_EndInterpreter(pythr);
	PyEval_ReleaseLock();  

	resetListe(&categoryAtr);
	token= strtok(response,",");
	for(; token != NULL ;)
	{
		addBacke(&categoryAtr,token);
		token=strtok(NULL, ",");
	}
	elemm *pelem = categoryAtr.first;
	if(pelem){
		pptr->id = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->name = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->flavor = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->image = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->original = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->profile = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->node = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->price = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->account = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->number = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->rootpass = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->reference = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->network = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->access = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->accessip = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->keypair = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->placementgroup = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->publicaddr = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->privateaddr = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->firewall = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->group = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->zone = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->hostname = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->workload = allocate_string(pelem->value);
		pelem = pelem->next;
	}
        if(pelem){
		pptr->agent = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->stamp = atoi(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->state = atoi(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->keyfile = allocate_string(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->agentstatus = atoi(pelem->value);
	}
	liberateListe(&categoryAtr);
	return status;

}


/*	-----------------------------------------------------------------	*/
/*	   s n a p s h o t _ E C 2 _ i n s t a n c e	                	*/
/*	-----------------------------------------------------------------	*/
int	snapshot_ec2_instance( struct amazonEc2 * pptr )
{
	struct	ec2_subscription * subptr = (struct ec2_subscription *) 0;
	int     status = 0;
	char buffer[1024];
	char * uuid;
	char srcdir[1024];
	char * response;
	char * token;
	char sendstr[1024]=" ";
	char strtmp[1024]=" ";
	char * strcmptr = NULL;
	listcc	categoryAtr;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;
	PyThreadState* pythr=NULL;
	/* ------------------------ */
	/* prepare the subscription */
	/* ------------------------ */
	if (!( pptr ))
		return(118);
	else if (!(subptr = use_ec2_configuration(pptr->profile )))
		return(118);
	else if (!( set_ec2_zone( subptr, pptr->zone ) ))
		return( 118 );
	else if(!(uuid = rest_allocate_uuid()))
		return 11;
	else
	{
		sprintf(buffer,"%s_image",uuid);
		
		if(!(strValid(pptr->id))) strcpy(sendstr," ");
		else strcpy(sendstr,pptr->id);

		if(!(strValid(pptr->name))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->name,',');

		if(!(strValid(pptr->flavor))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->flavor,',');
		
		if(!(strValid(pptr->image))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->image,',');
		
		if(!(strValid(pptr->original))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->original,',');
		
		if(!(strValid(pptr->profile))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->profile,',');

		if(!(strValid(pptr->node))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->node,',');
		
		if(!(strValid(pptr->price))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->price,',');

		if(!(strValid(pptr->account))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->account,',');

		if(!(strValid(pptr->number))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->number,',');

		if(!(strValid(pptr->rootpass))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->rootpass,',');

		if(!(strValid(pptr->reference))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->reference,',');

		if(!(strValid(pptr->network))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->network,',');

		if(!(strValid(pptr->access))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->access,',');

		if(!(strValid(pptr->accessip))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->accessip,',');

		if(!(strValid(pptr->keypair))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->keypair,',');

		if(!(strValid(pptr->placementgroup))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->placementgroup,',');

		if(!(strValid(pptr->publicaddr))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->publicaddr,',');

		 if(!(strValid(pptr->privateaddr))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->privateaddr,',');

		 if(!(strValid(pptr->firewall))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->firewall,',');

		if(!(strValid(pptr->group))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->group,',');

		if(!(strValid(pptr->zone))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->zone,',');

		if(!(strValid(pptr->hostname))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->hostname,',');

		if(!(strValid(pptr->workload))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->workload,',');

		if(!(strValid(pptr->agent))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->agent,',');
		
		sprintf(strtmp,"%d",pptr->stamp);
	 	if(strValid(strtmp)) strConcat(sendstr,strtmp,',');
		else strConcat(sendstr," ",',');
	
		sprintf(strtmp,"%d",pptr->state);
		if(strValid(strtmp)) strConcat(sendstr,strtmp,',');
		else strConcat(sendstr," ",',');
	
		if(!(strValid(pptr->keyfile))){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->keyfile,',');

		sprintf(strtmp,"%d",pptr->agentstatus);
		strcmptr = allocate_string(strtmp);
		if(strValid(strcmptr)) strConcat(sendstr,strtmp,',');
		else strConcat(sendstr," ",',');


		//           python interface
		sprintf(srcdir,"%s/pyaccords/pygen",PYPATH);
		PyEval_AcquireLock();
		pythr = Py_NewInterpreter();
       		if(pythr == NULL) printf("interpreter init error \n");
		PyThreadState_Swap(pythr);
		python_path(srcdir);
		pName = PyString_FromString("amazonEc2Act");
		if(pName == NULL) printf("erro: in amazonEc2Act.py no such file name\n");
		else pModule = PyImport_Import(pName);
		if(pModule == NULL) printf("error: failed to load amazonEc2Act module\n");
		else pDict = PyModule_GetDict(pModule);
		if(pDict == NULL) printf("error: failed to load dict name in amazonEc2Act module\n");
		else pFunc = PyDict_GetItemString(pDict,"suspend");
		if(pFunc == NULL) printf("error: failed to load start function in amazonEc2Act module\n");
		else result=PyObject_CallFunction(pFunc,"ssss",subptr->accesskey,subptr->secretkey,subptr->zone,buffer,sendstr);
		
		if (!result || PyErr_Occurred())
        	{
       			PyErr_Print();
       			return (0);
       		}

		response=allocate_string(PyString_AsString( result ));
		Py_DECREF(pModule);
		Py_DECREF(pName);

		PyEval_ReleaseThread(pythr); 
		PyEval_AcquireThread(pythr);
		Py_EndInterpreter(pythr);
		PyEval_ReleaseLock();  

		resetListe(&categoryAtr);
		token= strtok(response,",");
		for(; token != NULL ;)
		{
			addBacke(&categoryAtr,token);
			token=strtok(NULL, ",");
		}
		elemm *pelem = categoryAtr.first;
		if(pelem){
			pptr->id = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->name = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->flavor = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->image = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->original = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->profile = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->node = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->price = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->account = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->number = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->rootpass = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->reference = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->network = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->access = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->accessip = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->keypair = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->placementgroup = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->publicaddr = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->privateaddr = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->firewall = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->group = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->zone = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->hostname = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->workload = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->agent = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->stamp = atoi(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->state = atoi(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->keyfile = allocate_string(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->agentstatus = atoi(pelem->value);
		}

	}
        liberateListe(&categoryAtr);
	return status;

}


#endif
