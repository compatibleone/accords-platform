/********************************************************************************************************/
/* Hamid MEDAJHED & Elyes ZEKRI (c) Prologue                                                            */
/********************************************************************************************************/
#include "../../occi/src/occi.h"
#include "ctools.h"
#include "listcateg.h"
#include <Python.h>

private int ec2config_create(struct occi_category * optr, void * vptr)
{
	struct occi_kind_node * nptr;
	struct cords_ec2config * pptr;
	char sendstr[1024];
	char strtmp[1024];
        char srcdir[1024];
	char * response;
	char * token;
	FILE * exp_file;
	listcc categoryAtr;
	PyObject*    main_module, * global_dict, * cbFunc, *result;

	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);

	if(!(pptr->name)) strcpy(sendstr," ");
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
        sprintf(srcdir,"%s/pyaccords/pysrc/ec2config.py",PYPATH);
	exp_file = fopen(PYPATH, "r");
	if(!exp_file) printf("error in ec2configInterface.c ec2config.py :No such file or directory\n");
	Py_Initialize();
	PyRun_SimpleFile(exp_file,PYPATH);
	main_module = PyImport_AddModule("__main__");
	global_dict = PyModule_GetDict(main_module);
	cbFunc = PyDict_GetItemString(global_dict,"create");
	if(!cbFunc) printf("error in ec2configInterface.c :no python function\n");
	result=PyObject_CallFunction(cbFunc,"s",sendstr);
	response=PyString_AsString( result );
	Py_Finalize();

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
		pptr->description = pelem->value;
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


private int ec2config_retrieve(struct occi_category * optr, void * vptr)
{
	struct occi_kind_node * nptr;
	struct cords_ec2config * pptr;
	char sendstr[1024];
	char strtmp[1024];
        char srcdir[1024];
	char * response;
	char * token;
	FILE * exp_file;
	listcc categoryAtr;
	PyObject*    main_module, * global_dict, * cbFunc, *result;

	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);

	if(!(pptr->name)) strcpy(sendstr," ");
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
        sprintf(srcdir,"%s/pyaccords/pysrc/ec2config.py",PYPATH);
	exp_file = fopen(srcdir, "r");
	if(!exp_file) printf("error in ec2configInterface.c ec2config.py :No such file or directory\n");
	Py_Initialize();
	PyRun_SimpleFile(exp_file,srcdir);
	main_module = PyImport_AddModule("__main__");
	global_dict = PyModule_GetDict(main_module);
	cbFunc = PyDict_GetItemString(global_dict,"retrieve");
	if(!cbFunc) printf("error in ec2configInterface.c :no python function\n");
	result=PyObject_CallFunction(cbFunc,"s",sendstr);
	response=PyString_AsString( result );
	Py_Finalize();

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
		pptr->description = pelem->value;
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


private int ec2config_update(struct occi_category * optr, void * vptr)
{
	struct occi_kind_node * nptr;
	struct cords_ec2config * pptr;
	char sendstr[1024];
	char strtmp[1024];
        char srcdir[1024];
	char * response;
	char * token;
	FILE * exp_file;
	listcc categoryAtr;
	PyObject*    main_module, * global_dict, * cbFunc, *result;

	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);

	if(!(pptr->name)) strcpy(sendstr," ");
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
        sprintf(srcdir,"%s/pyaccords/pysrc/ec2config.py",PYPATH);
	exp_file = fopen(srcdir, "r");
	if(!exp_file) printf("error in ec2configInterface.c ec2config.py :No such file or directory\n");
	Py_Initialize();
	PyRun_SimpleFile(exp_file,srcdir);
	main_module = PyImport_AddModule("__main__");
	global_dict = PyModule_GetDict(main_module);
	cbFunc = PyDict_GetItemString(global_dict,"update");
	if(!cbFunc) printf("error in ec2configInterface.c :no python function\n");
	result=PyObject_CallFunction(cbFunc,"s",sendstr);
	response=PyString_AsString( result );
	Py_Finalize();

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
		pptr->description = pelem->value;
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


private int ec2config_delete(struct occi_category * optr, void * vptr)
{
	struct occi_kind_node * nptr;
	struct cords_ec2config * pptr;
	char sendstr[1024];
	char strtmp[1024];
        char srcdir[1024];
	char * response;
	char * token;
	FILE * exp_file;
	listcc categoryAtr;
	PyObject*    main_module, * global_dict, * cbFunc, *result;

	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);

	if(!(pptr->name)) strcpy(sendstr," ");
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
        sprintf(srcdir,"%s/pyaccords/pysrc/ec2config.py",PYPATH);
	exp_file = fopen(srcdir, "r");
	if(!exp_file) printf("error in ec2configInterface.c ec2config.py :No such file or directory\n");
	Py_Initialize();
	PyRun_SimpleFile(exp_file,srcdir);
	main_module = PyImport_AddModule("__main__");
	global_dict = PyModule_GetDict(main_module);
	cbFunc = PyDict_GetItemString(global_dict,"delete");
	if(!cbFunc) printf("error in ec2configInterface.c :no python function\n");
	result=PyObject_CallFunction(cbFunc,"s",sendstr);
	response=PyString_AsString( result );
	Py_Finalize();

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
		pptr->description = pelem->value;
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


