/*-------------------------------------------------------------------------------*/
/* ACCORDS Platform                                                              */
/* copyright 2012, Hamid MEDJAHE (hmedjahed@prologue.fr)    Prologue             */
/*-------------------------------------------------------------------------------*/
/* Licensed under the Apache License, Version 2.0 (the "License");             */
/* you may not use this file except in compliance with the License.              */
/* You may obtain a copy of the License at                                       */
/*                                                                               */
/*       http://www.apache.org/licenses/LICENSE-2.0                              */
/*                                                                               */
/* Unless required by applicable law or agreed to in writing, software           */
/* distributed under the License is distributed on an "AS IS" BASIS,           */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.      */
/* See the License for the specific language governing permissions and           */
/* limitations under the License.                                                */
/*-------------------------------------------------------------------------------*/
#include "../../occi/src/occi.h"
#include "ctools.h"
#include <Python.h>
#include "pytools.h"

/*-------------------------------------------------------------------------------*/
/*           linkgw start action    */
/*-------------------------------------------------------------------------------*/
struct rest_response * start_linkgw(
	struct occi_category * optr,
	struct rest_client * cptr,
	struct rest_request * rptr,
	struct rest_response * aptr,
	void * vptr )
{
	struct linkgw * pptr;
	char sendstr[1024]=" ";
	char strtmp[1024]=" ";
	int status;
	char message[1024];
	char srcdir[1024];
	char * response = NULL;
	char * token;
	FILE * exp_file;
	listcc restResponse;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;

	PyThreadState* pythr=NULL;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else{
		if(!(pptr->name))strcpy(sendstr," ");
		else if(pptr->name[0]=='\0') strcpy(sendstr," ");
		else strcpy(sendstr,pptr->name);
		if(!(pptr->intercloudGW)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->intercloudGW[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->intercloudGW,',');
		if(!(pptr->account)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->account[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->account,',');
		if(!(pptr->gwsrc)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->gwsrc[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->gwsrc,',');
		if(!(pptr->gwdst)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->gwdst[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->gwdst,',');
		if(!(pptr->tunnelproto)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->tunnelproto[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->tunnelproto,',');
		if(!(pptr->addressgresrc)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->addressgresrc[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->addressgresrc,',');
		if(!(pptr->addressgredst)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->addressgredst[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->addressgredst,',');
		if(!(pptr->prefix)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->prefix[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->prefix,',');
		if(!(pptr->authenticationkey)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->authenticationkey[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->authenticationkey,',');
		if(!(pptr->endpointsrc)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->endpointsrc[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->endpointsrc,',');
		if(!(pptr->endpointdst)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->endpointdst[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->endpointdst,',');
		if(!(pptr->state)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->state[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->state,',');
		//           python interface
		sprintf(srcdir,"%s/pyaccords/pysrc",PYPATH);
		pythr = Py_NewInterpreter();
		python_path(srcdir);
		pName = PyString_FromString("linkgwAct");
		if(pName == NULL) printf("erro: in linkgwAct no such file name\n");
		else pModule = PyImport_Import(pName);
		if(pModule == NULL) printf("error: failed to load linkgwAct module\n");
		else pDict = PyModule_GetDict(pModule);
		if(pDict == NULL) printf("error: failed to load dict name in linkgw module\n");
		else pFunc = PyDict_GetItemString(pDict,"start");
		if(pFunc == NULL) printf("error: failed to load start function in linkgw module\n");
		else result=PyObject_CallFunction(pFunc,"s",sendstr);
		if(result) response=allocate_string(PyString_AsString( result ));
		Py_DECREF(pModule);
		Py_DECREF(pName);
		Py_EndInterpreter(pythr);

		resetListe(&restResponse);
		token= strtok(response,",");
		for(; token != NULL ;)
		{
			addBacke(&restResponse,token);
			token=strtok(NULL, ",");
		}
		elemm *pelem = restResponse.first;
		if(pelem){
			status = atoi(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			strcpy(message, pelem->value);
			pelem = pelem->next;
		}
		return( rest_html_response( aptr, status, message ) );
	}
}

/*-------------------------------------------------------------------------------*/
/*           linkgw stop action    */
/*-------------------------------------------------------------------------------*/
struct rest_response * stop_linkgw(
	struct occi_category * optr,
	struct rest_client * cptr,
	struct rest_request * rptr,
	struct rest_response * aptr,
	void * vptr )
{
	struct linkgw * pptr;
	char sendstr[1024]=" ";
	char strtmp[1024]=" ";
	int status;
	char message[1024];
	char srcdir[1024];
	char * response = NULL;
	char * token;
	FILE * exp_file;
	listcc restResponse;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;

	PyThreadState* pythr=NULL;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else{
		if(!(pptr->name))strcpy(sendstr," ");
		else if(pptr->name[0]=='\0') strcpy(sendstr," ");
		else strcpy(sendstr,pptr->name);
		if(!(pptr->intercloudGW)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->intercloudGW[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->intercloudGW,',');
		if(!(pptr->account)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->account[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->account,',');
		if(!(pptr->gwsrc)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->gwsrc[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->gwsrc,',');
		if(!(pptr->gwdst)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->gwdst[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->gwdst,',');
		if(!(pptr->tunnelproto)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->tunnelproto[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->tunnelproto,',');
		if(!(pptr->addressgresrc)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->addressgresrc[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->addressgresrc,',');
		if(!(pptr->addressgredst)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->addressgredst[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->addressgredst,',');
		if(!(pptr->prefix)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->prefix[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->prefix,',');
		if(!(pptr->authenticationkey)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->authenticationkey[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->authenticationkey,',');
		if(!(pptr->endpointsrc)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->endpointsrc[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->endpointsrc,',');
		if(!(pptr->endpointdst)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->endpointdst[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->endpointdst,',');
		if(!(pptr->state)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->state[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->state,',');
		//           python interface
		sprintf(srcdir,"%s/pyaccords/pysrc",PYPATH);
		pythr = Py_NewInterpreter();
		python_path(srcdir);
		pName = PyString_FromString("linkgwAct");
		if(pName == NULL) printf("erro: in linkgwAct no such file name\n");
		else pModule = PyImport_Import(pName);
		if(pModule == NULL) printf("error: failed to load linkgwAct module\n");
		else pDict = PyModule_GetDict(pModule);
		if(pDict == NULL) printf("error: failed to load dict name in linkgw module\n");
		else pFunc = PyDict_GetItemString(pDict,"stop");
		if(pFunc == NULL) printf("error: failed to load stop function in linkgw module\n");
		else result=PyObject_CallFunction(pFunc,"s",sendstr);
		if(result) response=allocate_string(PyString_AsString( result ));
		Py_DECREF(pModule);
		Py_DECREF(pName);
		Py_EndInterpreter(pythr);

		resetListe(&restResponse);
		token= strtok(response,",");
		for(; token != NULL ;)
		{
			addBacke(&restResponse,token);
			token=strtok(NULL, ",");
		}
		elemm *pelem = restResponse.first;
		if(pelem){
			status = atoi(pelem->value);
			pelem = pelem->next;
		}
		if(pelem){
			strcpy(message, pelem->value);
			pelem = pelem->next;
		}
		return( rest_html_response( aptr, status, message ) );
	}
}

char *linkgw_getname(int a)
{
	static char action[256];
	switch(a)
	{
		case 0:
			strcpy(action,"start");
			break;
		case 1:
			strcpy(action,"stop");
			break;
		default:
			strcpy(action,"linkgw");
			break;
	}
	return action;
}

int linkgw_getnumber()
{
	return 2;
}
