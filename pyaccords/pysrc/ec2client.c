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
	pythr = Py_NewInterpreter();
        if(pythr == NULL) printf("interpreter init error \n");
	PyThreadState_Swap(pythr);
	python_path(srcdir);
	pName = PyString_FromString("ec2client");
	if(pName == NULL) printf("erro: in ec2client.py no such file name\n");
	else pModule = PyImport_Import(pName);
	if(pModule == NULL) printf("error: failed to load ec2client module\n");
	else pDict = PyModule_GetDict(pModule);
	if(pDict == NULL) printf("error: failed to load dict name in ec2client module\n");
	else pFunc = PyDict_GetItemString(pDict,"ec2_get_zone");
	if(pFunc == NULL) printf("error: failed to load ec2_get_zone function in ec2client module\n");
	else result=PyObject_CallFunction(pFunc,"sss",subptr->accesskey,subptr->secretkey,zone);
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
	pythr = Py_NewInterpreter();
        if(pythr == NULL) printf("interpreter init error \n");
	PyThreadState_Swap(pythr);
	python_path(srcdir);
	pName = PyString_FromString("ec2client");
	if(pName == NULL) printf("erro: in ec2client.py no such file name\n");
	else pModule = PyImport_Import(pName);
	if(pModule == NULL) printf("error: failed to load ec2client module\n");
	else pDict = PyModule_GetDict(pModule);
	if(pDict == NULL) printf("error: failed to load dict name in ec2client module\n");
	else pFunc = PyDict_GetItemString(pDict,"ec2_get_image");
	if(pFunc == NULL) printf("error: failed to load ec2_get_image function in ec2client module\n");
	else result=PyObject_CallFunction(pFunc,"ssss",subptr->accesskey,subptr->secretkey,imgname,zone);
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
	pythr = Py_NewInterpreter();
        if(pythr == NULL) printf("interpreter init error \n");
	PyThreadState_Swap(pythr);
	python_path(srcdir);
	pName = PyString_FromString("ec2client");
	if(pName == NULL) printf("erro: in ec2client.py no such file name\n");
	else pModule = PyImport_Import(pName);
	if(pModule == NULL) printf("error: failed to load ec2client module\n");
	else pDict = PyModule_GetDict(pModule);
	if(pDict == NULL) printf("error: failed to load dict name in ec2client module\n");
	else pFunc = PyDict_GetItemString(pDict,"ec2_get_flavor");
	if(pFunc == NULL) printf("error: failed to load ec2_get_flavor function in ec2client module\n");
	else result=PyObject_CallFunction(pFunc,"iiiis",memory,cores,speed,storage,architecture);
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
	pythr = Py_NewInterpreter();
        if(pythr == NULL) printf("interpreter init error \n");
	PyThreadState_Swap(pythr);
	python_path(srcdir);
	pName = PyString_FromString("ec2client");
	if(pName == NULL) printf("erro: in ec2client.py no such file name\n");
	else pModule = PyImport_Import(pName);
	if(pModule == NULL) printf("error: failed to load ec2client module\n");
	else pDict = PyModule_GetDict(pModule);
	if(pDict == NULL) printf("error: failed to load dict name in ec2client module\n");
	else pFunc = PyDict_GetItemString(pDict,"ec2_create_secgroup");
	if(pFunc == NULL) printf("error: failed to load ec2_create_secgroup function in ec2client module\n");
	else result=PyObject_CallFunction(pFunc,"ssss",subptr->accesskey,subptr->secretkey,subptr->zone,pptr->firewall);
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
	pythr = Py_NewInterpreter();
        if(pythr == NULL) printf("interpreter init error \n");
	PyThreadState_Swap(pythr);
	python_path(srcdir);
	pName = PyString_FromString("ec2client");
	if(pName == NULL) printf("erro: in ec2client.py no such file name\n");
	else pModule = PyImport_Import(pName);
	if(pModule == NULL) printf("error: failed to load ec2client module\n");
	else pDict = PyModule_GetDict(pModule);
	if(pDict == NULL) printf("error: failed to load dict name in ec2client module\n");
	else pFunc = PyDict_GetItemString(pDict,"ec2_add_rule");
	if(pFunc == NULL) printf("error: failed to load ec2_add_rule function in ec2client module\n");
	else result=PyObject_CallFunction(pFunc,"sssssiis",subptr->accesskey,subptr->secretkey,subptr->zone,group,rulename,fport,tport,protocol);
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
	pythr = Py_NewInterpreter();
        if(pythr == NULL) printf("interpreter init error \n");
	PyThreadState_Swap(pythr);
	python_path(srcdir);
	pName = PyString_FromString("ec2client");
	if(pName == NULL) printf("erro: in ec2client.py no such file name\n");
	else pModule = PyImport_Import(pName);
	if(pModule == NULL) printf("error: failed to load ec2client module\n");
	else pDict = PyModule_GetDict(pModule);
	if(pDict == NULL) printf("error: failed to load dict name in ec2client module\n");
	else pFunc = PyDict_GetItemString(pDict,"ec2_delete_secgroup");
	if(pFunc == NULL) printf("error: failed to load ec2_delete_secgroup function in ec2client module\n");
	else result=PyObject_CallFunction(pFunc,"ssss",subptr->accesskey,subptr->secretkey,subptr->zone,pptr->firewall);
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
	char * response;
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

	sprintf(strtmp,"%d",pptr->when);
	strcmptr = allocate_string(strtmp);
	if(strValid(strcmptr)) strConcat(sendstr,strtmp,',');
	else strConcat(sendstr," ",',');
	
	sprintf(strtmp,"%d",pptr->state);
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
	else pFunc = PyDict_GetItemString(pDict,"start");
	if(pFunc == NULL) printf("error: failed to load start function in amazonEc2Act module\n");
	else result=PyObject_CallFunction(pFunc,"sssss",subptr->accesskey,subptr->secretkey,subptr->zone,subptr->keypair,sendstr);
	
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
		pptr->id = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->name = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->flavor = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->image = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->original = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->profile = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->node = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->price = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->account = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->number = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->rootpass = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->reference = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->network = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->access = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->accessip = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->keypair = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->placementgroup = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->publicaddr = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->privateaddr = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->firewall = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->group = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->zone = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->hostname = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->workload = pelem->value;
		pelem = pelem->next;
	}
        if(pelem){
		pptr->agent = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->when = atoi(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->state = atoi(pelem->value);
	}

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
	else	sprintf(reference,"%s/%s/%s",get_identity(),_CORDS_EC2,pptr->id);

	/* ------------------------------------------- */
	/* perform pre-release actions for destruction */
	/* ------------------------------------------- */
	if ( use_cosacs_agent( pptr->agent ) )
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

	sprintf(strtmp,"%d",pptr->when);
	if(strValid(strtmp)) strConcat(sendstr,strtmp,',');
	else strConcat(sendstr," ",',');
	
	sprintf(strtmp,"%d",pptr->state);
	if(strValid(strtmp)) strConcat(sendstr,strtmp,',');
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
	else pFunc = PyDict_GetItemString(pDict,"stop");
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
		pptr->id = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->name = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->flavor = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->image = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->original = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->profile = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->node = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->price = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->account = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->number = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->rootpass = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->reference = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->network = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->access = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->accessip = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->keypair = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->placementgroup = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->publicaddr = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->privateaddr = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->firewall = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->group = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->zone = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->hostname = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->workload = pelem->value;
		pelem = pelem->next;
	}
        if(pelem){
		pptr->agent = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->when = atoi(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->state = atoi(pelem->value);
	}

	//delete_ec2_secgroup(subptr, pptr);

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

	sprintf(strtmp,"%d",pptr->when);
	if(strValid(strtmp)) strConcat(sendstr,strtmp,',');
	else strConcat(sendstr," ",',');
	
	sprintf(strtmp,"%d",pptr->state);
	if(strValid(strtmp)) strConcat(sendstr,strtmp,',');
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
		pptr->id = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->name = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->flavor = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->image = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->original = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->profile = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->node = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->price = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->account = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->number = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->rootpass = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->reference = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->network = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->access = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->accessip = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->keypair = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->placementgroup = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->publicaddr = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->privateaddr = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->firewall = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->group = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->zone = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->hostname = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->workload = pelem->value;
		pelem = pelem->next;
	}
        if(pelem){
		pptr->agent = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->when = atoi(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->state = atoi(pelem->value);
	}

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

	sprintf(strtmp,"%d",pptr->when);
	if(strValid(strtmp)) strConcat(sendstr,strtmp,',');
	else strConcat(sendstr," ",',');
	
	sprintf(strtmp,"%d",pptr->state);
	if(strValid(strtmp)) strConcat(sendstr,strtmp,',');
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
		pptr->id = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->name = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->flavor = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->image = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->original = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->profile = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->node = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->price = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->account = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->number = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->rootpass = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->reference = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->network = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->access = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->accessip = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->keypair = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->placementgroup = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->publicaddr = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->privateaddr = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->firewall = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->group = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->zone = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->hostname = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->workload = pelem->value;
		pelem = pelem->next;
	}
        if(pelem){
		pptr->agent = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->when = atoi(pelem->value);
		pelem = pelem->next;
	}
	if(pelem){
		pptr->state = atoi(pelem->value);
	}

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
		
		sprintf(strtmp,"%d",pptr->when);
	 	if(strValid(strtmp)) strConcat(sendstr,strtmp,',');
		else strConcat(sendstr," ",',');
	
		sprintf(strtmp,"%d",pptr->state);
		if(strValid(strtmp)) strConcat(sendstr,strtmp,',');
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
			pptr->id = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->name = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->flavor = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->image = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->original = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->profile = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->node = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->price = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->account = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->number = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->rootpass = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->reference = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->network = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->access = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->accessip = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->keypair = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->placementgroup = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->publicaddr = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->privateaddr = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->firewall = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->group = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->zone = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->hostname = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->workload = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->agent = pelem->value;
			pelem = pelem->next;
		}
		if(pelem){
			pptr->when = atoi(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			pptr->state = atoi(pelem->value);
		}
	}

	return status;

}


#endif
