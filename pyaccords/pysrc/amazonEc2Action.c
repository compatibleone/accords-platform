/*-------------------------------------------------------------------------------*/
/* ACCORDS Platform                                                              */
/* copyright 2012 ,Hamid MEDJAHE   (hmedjahed@prologue.fr) Prologue              */
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

#include "../../occi/src/occi.h"
#include "ctools.h"
#include "ec2procci_tools.c"
#include <Python.h>


//            category start action  


extern char accesskey[256];
extern char secretkey[256];
extern char zone[256];

struct rest_response * start_amazonEc2(
	struct occi_category * optr,
	struct rest_client * cptr,
	struct rest_request * rptr,
	struct rest_response * aptr,
	void * vptr )
{
	struct cords_amazonEc2 * pptr;
	char sendstr[1024]=" ";
	char strtmp[1024]=" ";
	char status[1024];
	char message[1024];
        char srcdir[1024];
	char * response;
	char * token;
	FILE * exp_file;
	listcc restResponse;
	PyObject *main_module, *global_dict, *cbFunc, *result;

	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else{
		if(!(pptr->name))strcpy(sendstr," ");
		else if(pptr->name[0]=='\0') strcpy(sendstr," ");
		else strcpy(sendstr,pptr->name);
		if(!(pptr->flavor)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->flavor[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->flavor,',');
		if(!(pptr->image)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->image[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->image,',');
		if(!(pptr->original)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->original[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->original,',');
		if(!(pptr->profile)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->profile[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->profile,',');
		if(!(pptr->node)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->node[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->node,',');
		if(!(pptr->price)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->price[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->price,',');
		if(!(pptr->account)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->account[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->account,',');
		if(!(pptr->number)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->number[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->number,',');
		if(!(pptr->rootpass)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->rootpass[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->rootpass,',');
		if(!(pptr->reference)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->reference[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->reference,',');
		if(!(pptr->network)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->network[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->network,',');
		if(!(pptr->access)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->access[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->access,',');
		if(!(pptr->accessip)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->accessip[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->accessip,',');
		if(!(pptr->floating)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->floating[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->floating,',');
		if(!(pptr->floatingid)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->floatingid[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->floatingid,',');
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
		if(!(pptr->firewall)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->firewall[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->firewall,',');
		if(!(pptr->group)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->group[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->group,',');
		if(!(pptr->zone)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->zone[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->zone,',');
		if(!(pptr->hostname)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->hostname[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->hostname,',');
		if(!(pptr->workload)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->workload[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->workload,',');
		if(!(pptr->when)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->when[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,accesskey,',');
		if(!(pptr->state)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->state[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,secretkey,',');
		//           python interface
                sprintf(srcdir,"%s/pyaccords/pysrc/amazonEc2Act.py",PYPATH);
		exp_file = fopen(srcdir, "r");
		if(!exp_file) printf("error in amazonEc2Action.c amazonEc2.py :No such file or directory\n");
		Py_Initialize();
		PyRun_SimpleFile(exp_file,srcdir);
		main_module = PyImport_AddModule("__main__");
		global_dict = PyModule_GetDict(main_module);
		cbFunc = PyDict_GetItemString(global_dict,"start");
		if(!cbFunc) printf("error in amazonEc2Action.c :no python function\n");
		result=PyObject_CallFunction(cbFunc,"s",sendstr);
		response=PyString_AsString( result );
		Py_Finalize();

		resetListe(&restResponse);
		token= strtok(response,",");
		for(; token != NULL ;)
		{
			addBacke(&restResponse,token);
			token=strtok(NULL, ",");
		}
		elemm *pelem = restResponse.first;
		if(pelem){
			strcpy(status , pelem->value);
		pelem = pelem->next;
		}
		if(pelem){
			strcpy(message, pelem->value);
		pelem = pelem->next;
		}
		return( rest_html_response( aptr, status, message ) );
	}
}
//            category save action  
struct rest_response * save_amazonEc2(
	struct occi_category * optr,
	struct rest_client * cptr,
	struct rest_request * rptr,
	struct rest_response * aptr,
	void * vptr )
{
	struct cords_amazonEc2 * pptr;
	char sendstr[1024]=" ";
	char strtmp[1024]=" ";
	char status[1024];
	char message[1024];
        char srcdir[1024];
	char * response;
	char * token;
	FILE * exp_file;
	listcc restResponse;
	PyObject *main_module, *global_dict, *cbFunc, *result;

	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else{
		if(!(pptr->name))strcpy(sendstr," ");
		else if(pptr->name[0]=='\0') strcpy(sendstr," ");
		else strcpy(sendstr,pptr->name);
		if(!(pptr->flavor)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->flavor[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->flavor,',');
		if(!(pptr->image)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->image[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->image,',');
		if(!(pptr->original)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->original[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->original,',');
		if(!(pptr->profile)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->profile[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->profile,',');
		if(!(pptr->node)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->node[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->node,',');
		if(!(pptr->price)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->price[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->price,',');
		if(!(pptr->account)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->account[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->account,',');
		if(!(pptr->number)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->number[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->number,',');
		if(!(pptr->rootpass)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->rootpass[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->rootpass,',');
		if(!(pptr->reference)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->reference[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->reference,',');
		if(!(pptr->network)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->network[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->network,',');
		if(!(pptr->access)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->access[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->access,',');
		if(!(pptr->accessip)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->accessip[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->accessip,',');
		if(!(pptr->floating)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->floating[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->floating,',');
		if(!(pptr->floatingid)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->floatingid[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->floatingid,',');
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
		if(!(pptr->firewall)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->firewall[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->firewall,',');
		if(!(pptr->group)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->group[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->group,',');
		if(!(pptr->zone)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->zone[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->zone,',');
		if(!(pptr->hostname)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->hostname[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->hostname,',');
		if(!(pptr->workload)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->workload[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->workload,',');
		if(!(pptr->when)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->when[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->when,',');
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
                sprintf(srcdir,"%s/pyaccords/pysrc/amazonEc2Act.py",PYPATH); 
		exp_file = fopen(srcdir, "r");
		if(!exp_file) printf("error in amazonEc2Action.c amazonEc2.py :No such file or directory\n");
		Py_Initialize();
		PyRun_SimpleFile(exp_file,srcdir);
		main_module = PyImport_AddModule("__main__");
		global_dict = PyModule_GetDict(main_module);
		cbFunc = PyDict_GetItemString(global_dict,"save");
		if(!cbFunc) printf("error in amazonEc2Action.c :no python function\n");
		result=PyObject_CallFunction(cbFunc,"s",sendstr);
		response=PyString_AsString( result );
		Py_Finalize();

		resetListe(&restResponse);
		token= strtok(response,",");
		for(; token != NULL ;)
		{
			addBacke(&restResponse,token);
			token=strtok(NULL, ",");
		}
		elemm *pelem = restResponse.first;
		if(pelem){
			strcpy(status , pelem->value);
		pelem = pelem->next;
		}
		if(pelem){
			strcpy(message, pelem->value);
		pelem = pelem->next;
		}
		return( rest_html_response( aptr, status, message ) );
	}
}
//            category snapshot action  
struct rest_response * snapshot_amazonEc2(
	struct occi_category * optr,
	struct rest_client * cptr,
	struct rest_request * rptr,
	struct rest_response * aptr,
	void * vptr )
{
	struct cords_amazonEc2 * pptr;
	char sendstr[1024]=" ";
	char strtmp[1024]=" ";
	char status[1024];
	char message[1024];
        char srcdir[1024];
	char * response;
	char * token;
	FILE * exp_file;
	listcc restResponse;
	PyObject *main_module, *global_dict, *cbFunc, *result;

	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else{
		if(!(pptr->name))strcpy(sendstr," ");
		else if(pptr->name[0]=='\0') strcpy(sendstr," ");
		else strcpy(sendstr,pptr->name);
		if(!(pptr->flavor)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->flavor[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->flavor,',');
		if(!(pptr->image)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->image[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->image,',');
		if(!(pptr->original)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->original[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->original,',');
		if(!(pptr->profile)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->profile[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->profile,',');
		if(!(pptr->node)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->node[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->node,',');
		if(!(pptr->price)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->price[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->price,',');
		if(!(pptr->account)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->account[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->account,',');
		if(!(pptr->number)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->number[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->number,',');
		if(!(pptr->rootpass)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->rootpass[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->rootpass,',');
		if(!(pptr->reference)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->reference[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->reference,',');
		if(!(pptr->network)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->network[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->network,',');
		if(!(pptr->access)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->access[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->access,',');
		if(!(pptr->accessip)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->accessip[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->accessip,',');
		if(!(pptr->floating)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->floating[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->floating,',');
		if(!(pptr->floatingid)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->floatingid[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->floatingid,',');
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
		if(!(pptr->firewall)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->firewall[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->firewall,',');
		if(!(pptr->group)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->group[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->group,',');
		if(!(pptr->zone)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->zone[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->zone,',');
		if(!(pptr->hostname)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->hostname[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->hostname,',');
		if(!(pptr->workload)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->workload[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->workload,',');
		if(!(pptr->when)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->when[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->when,',');
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
                sprintf(srcdir,"%s/pyaccords/pysrc/amazonEc2Act.py");
		exp_file = fopen(srcdir, "r");
		if(!exp_file) printf("error in amazonEc2Action.c amazonEc2.py :No such file or directory\n");
		Py_Initialize();
		PyRun_SimpleFile(exp_file,srcdir);
		main_module = PyImport_AddModule("__main__");
		global_dict = PyModule_GetDict(main_module);
		cbFunc = PyDict_GetItemString(global_dict,"snapshot");
		if(!cbFunc) printf("error in amazonEc2Action.c :no python function\n");
		result=PyObject_CallFunction(cbFunc,"s",sendstr);
		response=PyString_AsString( result );
		Py_Finalize();

		resetListe(&restResponse);
		token= strtok(response,",");
		for(; token != NULL ;)
		{
			addBacke(&restResponse,token);
			token=strtok(NULL, ",");
		}
		elemm *pelem = restResponse.first;
		if(pelem){
			strcpy(status , pelem->value);
		pelem = pelem->next;
		}
		if(pelem){
			strcpy(message, pelem->value);
		pelem = pelem->next;
		}
		return( rest_html_response( aptr, status, message ) );
	}
}
//            category stop action  
struct rest_response * stop_amazonEc2(
	struct occi_category * optr,
	struct rest_client * cptr,
	struct rest_request * rptr,
	struct rest_response * aptr,
	void * vptr )
{
	struct cords_amazonEc2 * pptr;
	char sendstr[1024]=" ";
	char strtmp[1024]=" ";
	char status[1024];
	char message[1024];
        char srcdir[1024];
	char * response;
	char * token;
	FILE * exp_file;
	listcc restResponse;
	PyObject *main_module, *global_dict, *cbFunc, *result;

	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else{
		if(!(pptr->name))strcpy(sendstr," ");
		else if(pptr->name[0]=='\0') strcpy(sendstr," ");
		else strcpy(sendstr,pptr->name);
		if(!(pptr->flavor)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->flavor[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->flavor,',');
		if(!(pptr->image)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->image[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->image,',');
		if(!(pptr->original)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->original[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->original,',');
		if(!(pptr->profile)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->profile[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->profile,',');
		if(!(pptr->node)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->node[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->node,',');
		if(!(pptr->price)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->price[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->price,',');
		if(!(pptr->account)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->account[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->account,',');
		if(!(pptr->number)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->number[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->number,',');
		if(!(pptr->rootpass)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->rootpass[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->rootpass,',');
		if(!(pptr->reference)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->reference[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->reference,',');
		if(!(pptr->network)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->network[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->network,',');
		if(!(pptr->access)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->access[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->access,',');
		if(!(pptr->accessip)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->accessip[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->accessip,',');
		if(!(pptr->floating)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->floating[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->floating,',');
		if(!(pptr->floatingid)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->floatingid[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->floatingid,',');
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
		if(!(pptr->firewall)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->firewall[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->firewall,',');
		if(!(pptr->group)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->group[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->group,',');
		if(!(pptr->zone)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->zone[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->zone,',');
		if(!(pptr->hostname)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->hostname[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->hostname,',');
		if(!(pptr->workload)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->workload[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->workload,',');
		if(!(pptr->when)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->when[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->when,',');
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
                sprintf(srcdir,"%s/pyaccords/pysrc/amazonEc2Act.py",PYPATH);
		exp_file = fopen(srcdir, "r");
		if(!exp_file) printf("error in amazonEc2Action.c amazonEc2.py :No such file or directory\n");
		Py_Initialize();
		PyRun_SimpleFile(exp_file,srcdir);
		main_module = PyImport_AddModule("__main__");
		global_dict = PyModule_GetDict(main_module);
		cbFunc = PyDict_GetItemString(global_dict,"stop");
		if(!cbFunc) printf("error in amazonEc2Action.c :no python function\n");
		result=PyObject_CallFunction(cbFunc,"s",sendstr);
		response=PyString_AsString( result );
		Py_Finalize();

		resetListe(&restResponse);
		token= strtok(response,",");
		for(; token != NULL ;)
		{
			addBacke(&restResponse,token);
			token=strtok(NULL, ",");
		}
		elemm *pelem = restResponse.first;
		if(pelem){
			strcpy(status , pelem->value);
		pelem = pelem->next;
		}
		if(pelem){
			strcpy(message, pelem->value);
		pelem = pelem->next;
		}
		return( rest_html_response( aptr, status, message ) );
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
	struct cords_amazonEc2 * pptr;
	char sendstr[1024]=" ";
	char strtmp[1024]=" ";
	char status[1024];
	char message[1024];
        char srcdir[1024];
	char * response;
	char * token;
	FILE * exp_file;
	listcc restResponse;
	PyObject *main_module, *global_dict, *cbFunc, *result;

	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else{
		if(!(pptr->name))strcpy(sendstr," ");
		else if(pptr->name[0]=='\0') strcpy(sendstr," ");
		else strcpy(sendstr,pptr->name);
		if(!(pptr->flavor)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->flavor[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->flavor,',');
		if(!(pptr->image)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->image[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->image,',');
		if(!(pptr->original)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->original[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->original,',');
		if(!(pptr->profile)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->profile[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->profile,',');
		if(!(pptr->node)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->node[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->node,',');
		if(!(pptr->price)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->price[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->price,',');
		if(!(pptr->account)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->account[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->account,',');
		if(!(pptr->number)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->number[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->number,',');
		if(!(pptr->rootpass)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->rootpass[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->rootpass,',');
		if(!(pptr->reference)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->reference[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->reference,',');
		if(!(pptr->network)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->network[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->network,',');
		if(!(pptr->access)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->access[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->access,',');
		if(!(pptr->accessip)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->accessip[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->accessip,',');
		if(!(pptr->floating)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->floating[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->floating,',');
		if(!(pptr->floatingid)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->floatingid[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->floatingid,',');
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
		if(!(pptr->firewall)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->firewall[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->firewall,',');
		if(!(pptr->group)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->group[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->group,',');
		if(!(pptr->zone)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->zone[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->zone,',');
		if(!(pptr->hostname)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->hostname[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->hostname,',');
		if(!(pptr->workload)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->workload[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->workload,',');
		if(!(pptr->when)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->when[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->when,',');
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
                sprintf(srcdir,"%s/pyaccords/pysrc/amazonEc2Act.py",PYPATH);
		exp_file = fopen(srcdir, "r");
		if(!exp_file) printf("error in amazonEc2Action.c amazonEc2.py :No such file or directory\n");
		Py_Initialize();
		PyRun_SimpleFile(exp_file,srcdir);
		main_module = PyImport_AddModule("__main__");
		global_dict = PyModule_GetDict(main_module);
		cbFunc = PyDict_GetItemString(global_dict,"suspend");
		if(!cbFunc) printf("error in amazonEc2Action.c :no python function\n");
		result=PyObject_CallFunction(cbFunc,"s",sendstr);
		response=PyString_AsString( result );
		Py_Finalize();

		resetListe(&restResponse);
		token= strtok(response,",");
		for(; token != NULL ;)
		{
			addBacke(&restResponse,token);
			token=strtok(NULL, ",");
		}
		elemm *pelem = restResponse.first;
		if(pelem){
			strcpy(status , pelem->value);
		pelem = pelem->next;
		}
		if(pelem){
			strcpy(message, pelem->value);
		pelem = pelem->next;
		}
		return( rest_html_response( aptr, status, message ) );
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
	struct cords_amazonEc2 * pptr;
	char sendstr[1024]=" ";
	char strtmp[1024]=" ";
	char status[1024];
	char message[1024];
        char srcdir[1024];
	char * response;
	char * token;
	FILE * exp_file;
	listcc restResponse;
	PyObject *main_module, *global_dict, *cbFunc, *result;

	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else{
		if(!(pptr->name))strcpy(sendstr," ");
		else if(pptr->name[0]=='\0') strcpy(sendstr," ");
		else strcpy(sendstr,pptr->name);
		if(!(pptr->flavor)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->flavor[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->flavor,',');
		if(!(pptr->image)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->image[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->image,',');
		if(!(pptr->original)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->original[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->original,',');
		if(!(pptr->profile)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->profile[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->profile,',');
		if(!(pptr->node)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->node[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->node,',');
		if(!(pptr->price)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->price[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->price,',');
		if(!(pptr->account)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->account[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->account,',');
		if(!(pptr->number)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->number[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->number,',');
		if(!(pptr->rootpass)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->rootpass[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->rootpass,',');
		if(!(pptr->reference)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->reference[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->reference,',');
		if(!(pptr->network)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->network[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->network,',');
		if(!(pptr->access)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->access[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->access,',');
		if(!(pptr->accessip)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->accessip[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->accessip,',');
		if(!(pptr->floating)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->floating[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->floating,',');
		if(!(pptr->floatingid)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->floatingid[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->floatingid,',');
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
		if(!(pptr->firewall)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->firewall[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->firewall,',');
		if(!(pptr->group)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->group[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->group,',');
		if(!(pptr->zone)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->zone[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->zone,',');
		if(!(pptr->hostname)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->hostname[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->hostname,',');
		if(!(pptr->workload)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->workload[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->workload,',');
		if(!(pptr->when)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->when[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->when,',');
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
                sprintf(srcdir,"%s/pyaccords/pysrc/amazonEc2Act.py",PYPATH);
		exp_file = fopen(srcdir, "r");
		if(!exp_file) printf("error in amazonEc2Action.c amazonEc2.py :No such file or directory\n");
		Py_Initialize();
		PyRun_SimpleFile(exp_file,srcdir);
		main_module = PyImport_AddModule("__main__");
		global_dict = PyModule_GetDict(main_module);
		cbFunc = PyDict_GetItemString(global_dict,"restart");
		if(!cbFunc) printf("error in amazonEc2Action.c :no python function\n");
		result=PyObject_CallFunction(cbFunc,"s",sendstr);
		response=PyString_AsString( result );
		Py_Finalize();

		resetListe(&restResponse);
		token= strtok(response,",");
		for(; token != NULL ;)
		{
			addBacke(&restResponse,token);
			token=strtok(NULL, ",");
		}
		elemm *pelem = restResponse.first;
		if(pelem){
			strcpy(status , pelem->value);
		pelem = pelem->next;
		}
		if(pelem){
			strcpy(message, pelem->value);
		pelem = pelem->next;
		}
		return( rest_html_response( aptr, status, message ) );
	}
}

char * amazonEc2_getname(int a)
{
  char action[256];
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
          strcpy(action,"save");
          break;
      case 5:
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
 return 6;
}


