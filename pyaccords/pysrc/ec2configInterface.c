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
#include "ctools.h"
#include "ec2config.h"
#include "pytools.h"
#include "listcateg.h"
#include <Python.h>

private int ec2config_create(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct occi_kind_node * nptr;
	struct ec2config * pptr;
	char sendstr[1024];
	char strtmp[1024];
        char srcdir[1024];
	char * response;
	char * token;
	listcc categoryAtr;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;
	PyThreadState* pythr=NULL;

	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	
	if(!(strValid(pptr->id))) strcpy(sendstr," ");
	else strcpy(sendstr,pptr->id);

	if(!(strValid(pptr->name))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->name,',');

	if(!(strValid(pptr->description))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->description,',');

	if(!(strValid(pptr->user))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->user,',');

	if(!(strValid(pptr->password))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->password,',');

	if(!(strValid(pptr->accesskey))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->accesskey,',');

	if(!(strValid(pptr->secretkey))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->secretkey,',');

	if(!(strValid(pptr->authenticate))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->authenticate,',');

	if(!(strValid(pptr->agent))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->agent,',');

	if(!(strValid(pptr->host))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->host,',');

	
	if(!(strValid(pptr->version))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->version,',');

	if(!(strValid(pptr->location))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->location,',');

	if(!(strValid(pptr->namespace))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->namespace,',');

	if(!(strValid(pptr->base))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->base,',');

	if(!(strValid(pptr->tls))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->tls,',');

	if(!(strValid(pptr->current))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->current,',');

	//           python interface
	sprintf(srcdir,"%s/pyaccords/pysrc",PYPATH);
	pythr = Py_NewInterpreter();
	python_path(srcdir);
	pName = PyString_FromString("ec2config");
	if(pName == NULL) printf("erro: in ec2config.py no such file name\n");
	else pModule = PyImport_Import(pName);
	if(pModule == NULL) printf("error: failed to load amazonEc2 module\n");
	else pDict = PyModule_GetDict(pModule);
	if(pDict == NULL) printf("error: failed to load dict name in amazonEc2 module\n");
	else pFunc = PyDict_GetItemString(pDict,"create");
	if(pFunc == NULL) printf("error: failed to load create function in ec2config module\n");
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
		pptr->description = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->user = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->password = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->accesskey = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->secretkey = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->authenticate = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->agent = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->host = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->version = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->location = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->namespace = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->base = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->tls = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->current = pelem->value;
	}
	return 1;
}


private int ec2config_retrieve(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct occi_kind_node * nptr;
	struct ec2config * pptr;
	char sendstr[1024];
	char strtmp[1024];
        char srcdir[1024];
	char * response;
	char * token;
	listcc categoryAtr;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;
	PyThreadState* pythr=NULL;

	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);

	if(!(strValid(pptr->id))) strcpy(sendstr," ");
	else strcpy(sendstr,pptr->id);

	if(!(strValid(pptr->name))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->name,',');

	if(!(strValid(pptr->description))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->description,',');

	if(!(strValid(pptr->user))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->user,',');

	if(!(strValid(pptr->password))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->password,',');

	if(!(strValid(pptr->accesskey))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->accesskey,',');

	if(!(strValid(pptr->secretkey))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->secretkey,',');

	if(!(strValid(pptr->authenticate))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->authenticate,',');

	if(!(strValid(pptr->agent))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->agent,',');

	if(!(strValid(pptr->host))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->host,',');

	
	if(!(strValid(pptr->version))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->version,',');
	
	if(!(strValid(pptr->location))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->location,',');

	if(!(strValid(pptr->namespace))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->namespace,',');

	if(!(strValid(pptr->base))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->base,',');

	if(!(strValid(pptr->tls))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->tls,',');

	if(!(strValid(pptr->current))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->current,',');
 	
	//           python interface
	sprintf(srcdir,"%s/pyaccords/pysrc",PYPATH);
	pythr = Py_NewInterpreter();
	python_path(srcdir);
	pName = PyString_FromString("ec2config");
	if(pName == NULL) printf("erro: in ec2config.py no such file name\n");
	else pModule = PyImport_Import(pName);
	if(pModule == NULL) printf("error: failed to load ec2config module\n");
	else pDict = PyModule_GetDict(pModule);
	if(pDict == NULL) printf("error: failed to load dict name in ec2config module\n");
	else pFunc = PyDict_GetItemString(pDict,"retrieve");
	if(pFunc == NULL) printf("error: failed to load retrieve function in ec2config module\n");
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
		pptr->description = pelem->value;
		pelem = pelem->next;
	}
        if(pelem){
	        pptr->user = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
  		pptr->password = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->accesskey = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->secretkey = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->authenticate = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->agent = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->host = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->version = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->location = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->namespace = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->base = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->tls = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->current = pelem->value;
	}
	return 1;
}


private int ec2config_update(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct occi_kind_node * nptr;
	struct ec2config * pptr;
	char sendstr[1024];
	char strtmp[1024];
        char srcdir[1024];
	char * response;
	char * token;
	listcc categoryAtr;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;
	PyThreadState* pythr=NULL;

	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	
	if(!(strValid(pptr->id))) strcpy(sendstr," ");
	else strcpy(sendstr,pptr->id);

	if(!(strValid(pptr->name))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->name,',');

	if(!(strValid(pptr->description))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->description,',');

	if(!(strValid(pptr->user))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->user,',');

	if(!(strValid(pptr->password))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->password,',');

	if(!(strValid(pptr->accesskey))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->accesskey,',');

	if(!(strValid(pptr->secretkey))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->secretkey,',');

	if(!(strValid(pptr->authenticate))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->authenticate,',');

	if(!(strValid(pptr->agent))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->agent,',');

	if(!(strValid(pptr->host))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->host,',');

	
	if(!(strValid(pptr->version))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->version,',');
	
	if(!(strValid(pptr->location))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->location,',');

	if(!(strValid(pptr->namespace))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->namespace,',');

	if(!(strValid(pptr->base))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->base,',');

	if(!(strValid(pptr->tls))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->tls,',');

	if(!(strValid(pptr->current))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->current,',');

	//           python interface
	sprintf(srcdir,"%s/pyaccords/pysrc",PYPATH);
	pythr = Py_NewInterpreter();
	python_path(srcdir);
	pName = PyString_FromString("ec2config");
	if(pName == NULL) printf("erro: in ec2config.py no such file name\n");
	else pModule = PyImport_Import(pName);
	if(pModule == NULL) printf("error: failed to load amazonEc2 module\n");
	else pDict = PyModule_GetDict(pModule);
	if(pDict == NULL) printf("error: failed to load dict name in ec2config module\n");
	else pFunc = PyDict_GetItemString(pDict,"delete");
	if(pFunc == NULL) printf("error: failed to load update function in ec2config module\n");
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
		pptr->description = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->user = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->password = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->accesskey = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->secretkey = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->authenticate = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->agent = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->host = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->version = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->location = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->namespace = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->base = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->tls = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->current = pelem->value;
	}
	return 1;
}


private int ec2config_delete(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct occi_kind_node * nptr;
	struct ec2config * pptr;
	char sendstr[1024];
	char strtmp[1024];
        char srcdir[1024];
	char * response;
	char * token;
	listcc categoryAtr;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;
	PyThreadState* pythr=NULL;

	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	
	if(!(strValid(pptr->id))) strcpy(sendstr," ");
	else strcpy(sendstr,pptr->id);

	if(!(strValid(pptr->name))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->name,',');

	if(!(strValid(pptr->description))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->description,',');

	if(!(strValid(pptr->user))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->user,',');

	if(!(strValid(pptr->password))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->password,',');

	if(!(strValid(pptr->accesskey))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->accesskey,',');

	if(!(strValid(pptr->secretkey))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->secretkey,',');

	if(!(strValid(pptr->authenticate))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->authenticate,',');

	if(!(strValid(pptr->agent))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->agent,',');

	if(!(strValid(pptr->host))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->host,',');

	
	if(!(strValid(pptr->version))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->version,',');
	
	if(!(strValid(pptr->location))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->location,',');

	if(!(strValid(pptr->namespace))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->namespace,',');

	if(!(strValid(pptr->base))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->base,',');

	if(!(strValid(pptr->tls))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->tls,',');

	if(!(strValid(pptr->current))){
		strcpy(strtmp," ");
                strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->current,',');

	//           python interface
	sprintf(srcdir,"%s/pyaccords/pysrc",PYPATH);
	pythr = Py_NewInterpreter();
	python_path(srcdir);
	pName = PyString_FromString("ec2config");
	if(pName == NULL) printf("erro: in ec2config.py no such file name\n");
	else pModule = PyImport_Import(pName);
	if(pModule == NULL) printf("error: failed to load ec2config module\n");
	else pDict = PyModule_GetDict(pModule);
	if(pDict == NULL) printf("error: failed to load dict name in ec2config module\n");
	else pFunc = PyDict_GetItemString(pDict,"delete");
	if(pFunc == NULL) printf("error: failed to load delete function in ec2config module\n");
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
		pptr->description = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->user = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->password = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->accesskey = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->secretkey = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->authenticate = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->agent = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->host = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->version = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->location = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->namespace = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->base = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->tls = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->current = pelem->value;
	}
	return 1;
}


