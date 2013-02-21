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

private int gw_create(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct occi_kind_node * nptr;
	struct gw * pptr;
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
	if(!(pptr->publicaddr)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->publicaddr[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->publicaddr,',');
	if(!(pptr->privateaddr)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->privateaddr[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->privateaddr,',');
	if(!(pptr->ethername)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->ethername[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->ethername,',');
	if(!(pptr->intercloudGW)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->intercloudGW[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->intercloudGW,',');
	if(!(pptr->contract)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->contract[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->contract,',');
	if(!(pptr->provider_type)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->provider_type[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->provider_type,',');
	if(!(pptr->provider_platform)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->provider_platform[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->provider_platform,',');
	if(!(pptr->connection)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->connection[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->connection,',');
	if(!(pptr->account)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->account[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->account,',');
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
		pName = PyString_FromString("gw");
		if(pName == NULL) printf("erro: in gw no such file name\n");
		else pModule = PyImport_Import(pName);
		if(pModule == NULL) printf("error: failed to load gw module\n");
		else pDict = PyModule_GetDict(pModule);
		if(pDict == NULL) printf("error: failed to load dict name in gw module\n");
		else pFunc = PyDict_GetItemString(pDict,"create");
		if(pFunc == NULL) printf("error: failed to load create function in gw module\n");
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
		pptr->publicaddr = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->privateaddr = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->ethername = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->intercloudGW = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->contract = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->provider_type = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->provider_platform = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->connection = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->account = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->state = pelem->value;
	}
	return 1;
}


private int gw_retrieve(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct occi_kind_node * nptr;
	struct gw * pptr;
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
	if(!(pptr->publicaddr)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->publicaddr[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->publicaddr,',');
	if(!(pptr->privateaddr)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->privateaddr[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->privateaddr,',');
	if(!(pptr->ethername)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->ethername[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->ethername,',');
	if(!(pptr->intercloudGW)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->intercloudGW[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->intercloudGW,',');
	if(!(pptr->contract)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->contract[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->contract,',');
	if(!(pptr->provider_type)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->provider_type[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->provider_type,',');
	if(!(pptr->provider_platform)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->provider_platform[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->provider_platform,',');
	if(!(pptr->connection)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->connection[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->connection,',');
	if(!(pptr->account)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->account[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->account,',');
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
		pName = PyString_FromString("gw");
		if(pName == NULL) printf("erro: in gw no such file name\n");
		else pModule = PyImport_Import(pName);
		if(pModule == NULL) printf("error: failed to load gw module\n");
		else pDict = PyModule_GetDict(pModule);
		if(pDict == NULL) printf("error: failed to load dict name in gw module\n");
		else pFunc = PyDict_GetItemString(pDict,"retrieve");
		if(pFunc == NULL) printf("error: failed to load retrieve function in gw module\n");
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
		pptr->publicaddr = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->privateaddr = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->ethername = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->intercloudGW = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->contract = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->provider_type = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->provider_platform = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->connection = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->account = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->state = pelem->value;
	}
	return 1;
}


private int gw_update(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct occi_kind_node * nptr;
	struct gw * pptr;
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
	if(!(pptr->publicaddr)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->publicaddr[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->publicaddr,',');
	if(!(pptr->privateaddr)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->privateaddr[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->privateaddr,',');
	if(!(pptr->ethername)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->ethername[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->ethername,',');
	if(!(pptr->intercloudGW)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->intercloudGW[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->intercloudGW,',');
	if(!(pptr->contract)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->contract[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->contract,',');
	if(!(pptr->provider_type)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->provider_type[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->provider_type,',');
	if(!(pptr->provider_platform)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->provider_platform[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->provider_platform,',');
	if(!(pptr->connection)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->connection[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->connection,',');
	if(!(pptr->account)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->account[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->account,',');
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
		pName = PyString_FromString("gw");
		if(pName == NULL) printf("erro: in gw no such file name\n");
		else pModule = PyImport_Import(pName);
		if(pModule == NULL) printf("error: failed to load gw module\n");
		else pDict = PyModule_GetDict(pModule);
		if(pDict == NULL) printf("error: failed to load dict name in gw module\n");
		else pFunc = PyDict_GetItemString(pDict,"update");
		if(pFunc == NULL) printf("error: failed to load update function in gw module\n");
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
		pptr->publicaddr = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->privateaddr = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->ethername = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->intercloudGW = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->contract = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->provider_type = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->provider_platform = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->connection = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->account = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->state = pelem->value;
	}
	return 1;
}


private int gw_delete(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct occi_kind_node * nptr;
	struct gw * pptr;
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
	if(!(pptr->publicaddr)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->publicaddr[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->publicaddr,',');
	if(!(pptr->privateaddr)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->privateaddr[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->privateaddr,',');
	if(!(pptr->ethername)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->ethername[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->ethername,',');
	if(!(pptr->intercloudGW)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->intercloudGW[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->intercloudGW,',');
	if(!(pptr->contract)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->contract[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->contract,',');
	if(!(pptr->provider_type)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->provider_type[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->provider_type,',');
	if(!(pptr->provider_platform)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->provider_platform[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->provider_platform,',');
	if(!(pptr->connection)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->connection[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->connection,',');
	if(!(pptr->account)){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else if(pptr->account[0]=='\0'){
		strcpy(strtmp," ");
		strConcat(sendstr,strtmp,',');
	}
	else strConcat(sendstr,pptr->account,',');
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
		pName = PyString_FromString("gw");
		if(pName == NULL) printf("erro: in gw no such file name\n");
		else pModule = PyImport_Import(pName);
		if(pModule == NULL) printf("error: failed to load gw module\n");
		else pDict = PyModule_GetDict(pModule);
		if(pDict == NULL) printf("error: failed to load dict name in gw module\n");
		else pFunc = PyDict_GetItemString(pDict,"delete");
		if(pFunc == NULL) printf("error: failed to load delete function in gw module\n");
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
		pptr->publicaddr = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->privateaddr = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->ethername = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->intercloudGW = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->contract = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->provider_type = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->provider_platform = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->connection = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->account = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->state = pelem->value;
	}
	return 1;
}


