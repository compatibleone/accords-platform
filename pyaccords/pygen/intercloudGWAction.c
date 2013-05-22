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
#include "occi.h"
#include "../pysrc/ctools.h"
#include <Python.h>
#include "../pysrc/pytools.h"

/*-------------------------------------------------------------------------------*/
/*           intercloudGW start action    */
/*-------------------------------------------------------------------------------*/
struct rest_response * start_intercloudGW(
	struct occi_category * optr,
	struct rest_client * cptr,
	struct rest_request * rptr,
	struct rest_response * aptr,
	void * vptr )
{
	struct intercloudGW * pptr;
	char sendstr[1024]=" ";
	char strtmp[1024]=" ";
	int status;
	char message[1024];
	char srcdir[1024];
	char * response = NULL;
	char * token;
	listcc restResponse;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;

	PyThreadState* pythr=NULL;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else{
		if(!(pptr->name))strcpy(sendstr," ");
		else if(pptr->name[0]=='\0') strcpy(sendstr," ");
		else strcpy(sendstr,pptr->name);
		if(!(pptr->node)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->node[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->node,',');
		if(!(pptr->account)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->account[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->account,',');
		if(!(pptr->price)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->price[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->price,',');
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
		pName = PyString_FromString("intercloudGWAct");
		if(pName == NULL) printf("erro: in intercloudGWAct no such file name\n");
		else pModule = PyImport_Import(pName);
		if(pModule == NULL) printf("error: failed to load intercloudGWAct module\n");
		else pDict = PyModule_GetDict(pModule);
		if(pDict == NULL) printf("error: failed to load dict name in intercloudGW module\n");
		else pFunc = PyDict_GetItemString(pDict,"start");
		if(pFunc == NULL) printf("error: failed to load start function in intercloudGW module\n");
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
/*           intercloudGW stop action    */
/*-------------------------------------------------------------------------------*/
struct rest_response * stop_intercloudGW(
	struct occi_category * optr,
	struct rest_client * cptr,
	struct rest_request * rptr,
	struct rest_response * aptr,
	void * vptr )
{
	struct intercloudGW * pptr;
	char sendstr[1024]=" ";
	char strtmp[1024]=" ";
	int status;
	char message[1024];
	char srcdir[1024];
	char * response = NULL;
	char * token;
	listcc restResponse;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;

	PyThreadState* pythr=NULL;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else{
		if(!(pptr->name))strcpy(sendstr," ");
		else if(pptr->name[0]=='\0') strcpy(sendstr," ");
		else strcpy(sendstr,pptr->name);
		if(!(pptr->node)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->node[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->node,',');
		if(!(pptr->account)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->account[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->account,',');
		if(!(pptr->price)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->price[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->price,',');
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
		pName = PyString_FromString("intercloudGWAct");
		if(pName == NULL) printf("erro: in intercloudGWAct no such file name\n");
		else pModule = PyImport_Import(pName);
		if(pModule == NULL) printf("error: failed to load intercloudGWAct module\n");
		else pDict = PyModule_GetDict(pModule);
		if(pDict == NULL) printf("error: failed to load dict name in intercloudGW module\n");
		else pFunc = PyDict_GetItemString(pDict,"stop");
		if(pFunc == NULL) printf("error: failed to load stop function in intercloudGW module\n");
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

char *intercloudGW_getname(int a)
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
			strcpy(action,"intercloudGW");
			break;
	}
	return action;
}

int intercloudGW_getnumber()
{
	return 2;
}
