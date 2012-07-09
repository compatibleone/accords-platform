/********************************************************************************************************/
/* Hamid MEDAJHED (c) Prologue                                                            */
/********************************************************************************************************/
#include "../../occi/src/occi.h"
#include "ctools.h"
#include <Python.h>

//            category current action  
struct rest_response * ec2config_action0(
	struct occi_category * optr,
	struct rest_client * cptr,
	struct rest_request * rptr,
	struct rest_response * aptr,
	void * vptr )
{
	struct cords_ec2config * pptr;
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
		if(!(pptr->description)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->description[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->description,',');
		if(!(pptr->accesskey)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->accesskey[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->accesskey,',');
		if(!(pptr->secretkey)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->secretkey[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->secretkey,',');
		if(!(pptr->authenticate)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->authenticate[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->authenticate,',');
		if(!(pptr->agent)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->agent[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->agent,',');
		if(!(pptr->host)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->host[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->host,',');
		if(!(pptr->version)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->version[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->version,',');
		if(!(pptr->namespace)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->namespace[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->namespace,',');
		if(!(pptr->base)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->base[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->base,',');
		if(!(pptr->tls)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->tls[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->tls,',');
		if(!(pptr->current)){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else if(pptr->current[0]=='\0'){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->current,',');
		//           python interface
                sprintf(srcdir,"%s/pyaccords/pysrc/ec2configAct.py",PYPATH);
		exp_file = fopen(srcdir, "r");
		if(!exp_file) printf("error in ec2configAction.c ec2config.py :No such file or directory\n");
		Py_Initialize();
		PyRun_SimpleFile(exp_file,srcdir);
		main_module = PyImport_AddModule("__main__");
		global_dict = PyModule_GetDict(main_module);
		cbFunc = PyDict_GetItemString(global_dict,"current");
		if(!cbFunc) printf("error in ec2configAction.c :no python function\n");
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
