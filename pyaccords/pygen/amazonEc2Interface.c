/*-------------------------------------------------------------------------------*/
/* ACCORDS Platform                                                              */
/* copyright 2012 ,Hamid MEDJAHE & Elyes ZEKRI  (hmedjahed@prologue.fr) Prologue */
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
#include "../pysrc/pytools.h"
#include "../pysrc/listcateg.h"
#include "amazonEc2.h"
#include "../pysrc/ec2contract.h"
#include <Python.h>

private int amazonEc2_create(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct occi_kind_node * nptr;
	struct amazonEc2 * pptr;
	char sendstr[1024];
	char strtmp[1024];
	char srcdir[1024];
	char * response = NULL;
	char * token;
	listcc categoryAtr;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;
	PyThreadState* pythr=NULL;

	pptr = vptr;
	
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
	
	//python interface
	sprintf(srcdir,"%s/pyaccords/pygen",PYPATH);
	pythr = Py_NewInterpreter();
	python_path(srcdir);
	pName = PyString_FromString("amazonEc2");
	if(pName == NULL) printf("erro: in amazonEc2.py no such file name\n");
	else pModule = PyImport_Import(pName);
	if(pModule == NULL) printf("error: failed to load amazonEc2 module\n");
	else pDict = PyModule_GetDict(pModule);
	if(pDict == NULL) printf("error: failed to load dict name in amazonEc2 module\n");
	else pFunc = PyDict_GetItemString(pDict,"create");
	if(pFunc == NULL) printf("error: failed to load create function in amazonEc2 module\n");
	else result = PyObject_CallFunction(pFunc,"s",sendstr);
	if (!result || PyErr_Occurred())
        {
       		PyErr_Print();
       		return (0);
       	}
	
	response=allocate_string(PyString_AsString( result ));
	Py_DECREF(pModule);
	Py_DECREF(pName);
	Py_EndInterpreter(pythr);
	
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
	
	return (create_ec2_contract(optr,pptr, _CORDS_CONTRACT_AGENT, default_tls()));
}


private int amazonEc2_retrieve(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct occi_kind_node * nptr;
	struct amazonEc2 * pptr;
	char sendstr[1024];
	char strtmp[1024];
	char srcdir[1024];
	char * response = NULL;
	char * token;
	listcc categoryAtr;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;
	PyThreadState* pythr=NULL;

	pptr = vptr;

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
	pythr = Py_NewInterpreter();
	python_path(srcdir);
	pName = PyString_FromString("amazonEc2");
	if(pName == NULL) printf("erro: in amazonEc2.py no such file name\n");
	else pModule = PyImport_Import(pName);
	if(pModule == NULL) printf("error: failed to load amazonEc2 module\n");
	else pDict = PyModule_GetDict(pModule);
	if(pDict == NULL) printf("error: failed to load dict name in amazonEc2 module\n");
	else pFunc = PyDict_GetItemString(pDict,"retrieve");
	if(pFunc == NULL) printf("error: failed to load create function in amazonEc2 module\n");
	else result=PyObject_CallFunction(pFunc,"s",sendstr);
	if (!result || PyErr_Occurred())
        {
       		PyErr_Print();
       		return (0);
       	}
	
	response=allocate_string(PyString_AsString( result ));
	Py_DECREF(pModule);
	Py_DECREF(pName);
	Py_EndInterpreter(pythr);
	
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
	return 1;
}


private int amazonEc2_update(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct occi_kind_node * nptr;
	struct amazonEc2 * pptr;
	char sendstr[1024];
	char strtmp[1024];
        char srcdir[1024];
	char * response = NULL;
	char * token;
	listcc categoryAtr;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;
	PyThreadState* pythr=NULL;

	pptr = vptr;

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
	pythr = Py_NewInterpreter();
	python_path(srcdir);
	pName = PyString_FromString("amazonEc2");
	if(pName == NULL) printf("erro: in amazonEc2.py no such file name\n");
	else pModule = PyImport_Import(pName);
	if(pModule == NULL) printf("error: failed to load amazonEc2 module\n");
	else pDict = PyModule_GetDict(pModule);
	if(pDict == NULL) printf("error: failed to load dict name in amazonEc2 module\n");
	else pFunc = PyDict_GetItemString(pDict,"update");
	if(pFunc == NULL) printf("error: failed to load retrieve function in amazonEc2 module\n");
	else result=PyObject_CallFunction(pFunc,"s",sendstr);
	
	if (!result || PyErr_Occurred())
        {
       		PyErr_Print();
       		return (0);
       	}

	response=allocate_string(PyString_AsString( result ));
	Py_DECREF(pModule);
	Py_DECREF(pName);
	Py_EndInterpreter(pythr);

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
	return 1;
}


private int amazonEc2_delete(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct occi_kind_node * nptr;
	struct amazonEc2 * pptr;
	char sendstr[1024];
	char strtmp[1024];
        char srcdir[1024];
	char * response;
	char * token;
	listcc categoryAtr;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;
	PyThreadState* pythr=NULL;

	pptr = vptr;
	
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
	pythr = Py_NewInterpreter();
	python_path(srcdir);
	pName = PyString_FromString("amazonEc2");
	if(pName == NULL) printf("erro: in amazonEc2.py no such file name\n");
	else pModule = PyImport_Import(pName);
	if(pModule == NULL) printf("error: failed to load amazonEc2 module\n");
	else pDict = PyModule_GetDict(pModule);
	if(pDict == NULL) printf("error: failed to load dict name in amazonEc2 module\n");
	else pFunc = PyDict_GetItemString(pDict,"delete");
	if(pFunc == NULL) printf("error: failed to load delete function in amazonEc2 module\n");
	else result=PyObject_CallFunction(pFunc,"s",sendstr);
	
	if (!result || PyErr_Occurred())
        {
       		PyErr_Print();
       		return (0);
       	}
	
	response=allocate_string(PyString_AsString( result ));
	Py_DECREF(pModule);
	Py_DECREF(pName);
	Py_EndInterpreter(pythr);

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
	return (delete_ec2_contract(optr, pptr, _CORDS_CONTRACT_AGENT, default_tls()));
}


