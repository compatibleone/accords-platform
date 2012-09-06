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
#include "listcateg.h"
#include <Python.h>
#include "pytools.h"

private int intercloudGW_create(struct occi_category * optr, void * vptr)
{
	struct occi_kind_node * nptr;
	struct intercloudGW * pptr;
	char sendstr[1024];
	char strtmp[1024];
	char srcdir[1024];
	char * response = NULL;
	char * token;
	FILE * exp_file;
	listcc categoryAtr;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;

	PyThreadState* pythr=NULL;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);

	if(!(pptr->name)) strcpy(sendstr," ");
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
		pName = PyString_FromString("intercloudGW");
		if(pName == NULL) printf("erro: in intercloudGW no such file name\n");
		else pModule = PyImport_Import(pName);
		if(pModule == NULL) printf("error: failed to load intercloudGW module\n");
		else pDict = PyModule_GetDict(pModule);
		if(pDict == NULL) printf("error: failed to load dict name in intercloudGW module\n");
		else pFunc = PyDict_GetItemString(pDict,"create");
		if(pFunc == NULL) printf("error: failed to load create function in intercloudGW module\n");
		else result=PyObject_CallFunction(pFunc,"s",sendstr);
		if(result) response=allocate_string(PyString_AsString( result ));
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
		pptr->name = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->node = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->account = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->price = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->state = pelem->value;
	}
	return 1;
}


private int intercloudGW_retrieve(struct occi_category * optr, void * vptr)
{
	struct occi_kind_node * nptr;
	struct intercloudGW * pptr;
	char sendstr[1024];
	char strtmp[1024];
	char srcdir[1024];
	char * response = NULL;
	char * token;
	FILE * exp_file;
	listcc categoryAtr;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;

	PyThreadState* pythr=NULL;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);

	if(!(pptr->name)) strcpy(sendstr," ");
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
		pName = PyString_FromString("intercloudGW");
		if(pName == NULL) printf("erro: in intercloudGW no such file name\n");
		else pModule = PyImport_Import(pName);
		if(pModule == NULL) printf("error: failed to load intercloudGW module\n");
		else pDict = PyModule_GetDict(pModule);
		if(pDict == NULL) printf("error: failed to load dict name in intercloudGW module\n");
		else pFunc = PyDict_GetItemString(pDict,"retrieve");
		if(pFunc == NULL) printf("error: failed to load retrieve function in intercloudGW module\n");
		else result=PyObject_CallFunction(pFunc,"s",sendstr);
		if(result) response=allocate_string(PyString_AsString( result ));
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
		pptr->name = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->node = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->account = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->price = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->state = pelem->value;
	}
	return 1;
}


private int intercloudGW_update(struct occi_category * optr, void * vptr)
{
	struct occi_kind_node * nptr;
	struct intercloudGW * pptr;
	char sendstr[1024];
	char strtmp[1024];
	char srcdir[1024];
	char * response = NULL;
	char * token;
	FILE * exp_file;
	listcc categoryAtr;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;

	PyThreadState* pythr=NULL;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);

	if(!(pptr->name)) strcpy(sendstr," ");
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
		pName = PyString_FromString("intercloudGW");
		if(pName == NULL) printf("erro: in intercloudGW no such file name\n");
		else pModule = PyImport_Import(pName);
		if(pModule == NULL) printf("error: failed to load intercloudGW module\n");
		else pDict = PyModule_GetDict(pModule);
		if(pDict == NULL) printf("error: failed to load dict name in intercloudGW module\n");
		else pFunc = PyDict_GetItemString(pDict,"update");
		if(pFunc == NULL) printf("error: failed to load update function in intercloudGW module\n");
		else result=PyObject_CallFunction(pFunc,"s",sendstr);
		if(result) response=allocate_string(PyString_AsString( result ));
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
		pptr->name = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->node = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->account = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->price = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->state = pelem->value;
	}
	return 1;
}


private int intercloudGW_delete(struct occi_category * optr, void * vptr)
{
	struct occi_kind_node * nptr;
	struct intercloudGW * pptr;
	char sendstr[1024];
	char strtmp[1024];
	char srcdir[1024];
	char * response = NULL;
	char * token;
	FILE * exp_file;
	listcc categoryAtr;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;

	PyThreadState* pythr=NULL;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);

	if(!(pptr->name)) strcpy(sendstr," ");
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
		pName = PyString_FromString("intercloudGW");
		if(pName == NULL) printf("erro: in intercloudGW no such file name\n");
		else pModule = PyImport_Import(pName);
		if(pModule == NULL) printf("error: failed to load intercloudGW module\n");
		else pDict = PyModule_GetDict(pModule);
		if(pDict == NULL) printf("error: failed to load dict name in intercloudGW module\n");
		else pFunc = PyDict_GetItemString(pDict,"delete");
		if(pFunc == NULL) printf("error: failed to load delete function in intercloudGW module\n");
		else result=PyObject_CallFunction(pFunc,"s",sendstr);
		if(result) response=allocate_string(PyString_AsString( result ));
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
		pptr->name = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->node = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->account = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->price = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->state = pelem->value;
	}
	return 1;
}


