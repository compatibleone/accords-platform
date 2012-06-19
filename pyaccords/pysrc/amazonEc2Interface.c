/********************************************************************************************************/
/* Hamid MEDAJHED & Elyes ZEKRI (c) Prologue                                                            */
/********************************************************************************************************/
#include "../../occi/src/occi.h"
#include "ctools.h"
#include "listcateg.h"
#include <Python.h>
#include "ec2procci_tools.c"

private int amazonEc2_create(struct occi_category * optr, void * vptr)
{
	struct occi_kind_node * nptr;
	struct cords_amazonEc2 * pptr;
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
        sprintf(srcdir,"%s/pyaccords/pysrc/amazonEc2.py",PYPATH);
	exp_file = fopen(srcdir, "r");
	if(!exp_file) printf("error in amazonEc2Interface.c amazonEc2.py :No such file or directory\n");
	Py_Initialize();
	PyRun_SimpleFile(exp_file,srcdir);
	main_module = PyImport_AddModule("__main__");
	global_dict = PyModule_GetDict(main_module);
	cbFunc = PyDict_GetItemString(global_dict,"create");
	if(!cbFunc) printf("error in amazonEc2Interface.c :no python function\n");
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
		pptr->floating = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->floatingid = pelem->value;
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
		pptr->when = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->state = pelem->value;
	}
	return (create_ec2_contract(optr,pptr, _CORDS_CONTRACT_AGENT, default_tls()));
}


private int amazonEc2_retrieve(struct occi_category * optr, void * vptr)
{
	struct occi_kind_node * nptr;
	struct cords_amazonEc2 * pptr;
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
        sprintf(srcdir,"%s/pyaccords/pysrc/amazonEc2.py",PYPATH);
	exp_file = fopen(srcdir, "r");
	if(!exp_file) printf("error in amazonEc2Interface.c amazonEc2.py :No such file or directory\n");
	Py_Initialize();
	PyRun_SimpleFile(exp_file,srcdir);
	main_module = PyImport_AddModule("__main__");
	global_dict = PyModule_GetDict(main_module);
	cbFunc = PyDict_GetItemString(global_dict,"retrieve");
	if(!cbFunc) printf("error in amazonEc2Interface.c :no python function\n");
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
		pptr->floating = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->floatingid = pelem->value;
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
		pptr->when = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->state = pelem->value;
	}
	return 1;
}


private int amazonEc2_update(struct occi_category * optr, void * vptr)
{
	struct occi_kind_node * nptr;
	struct cords_amazonEc2 * pptr;
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
        sprintf(srcdir,"%s/pyaccords/pysrc/amazonEc2.py",PYPATH);
	exp_file = fopen(srcdir, "r");
	if(!exp_file) printf("error in amazonEc2Interface.c amazonEc2.py :No such file or directory\n");
	Py_Initialize();
	PyRun_SimpleFile(exp_file,srcdir);
	main_module = PyImport_AddModule("__main__");
	global_dict = PyModule_GetDict(main_module);
	cbFunc = PyDict_GetItemString(global_dict,"update");
	if(!cbFunc) printf("error in amazonEc2Interface.c :no python function\n");
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
		pptr->floating = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->floatingid = pelem->value;
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
		pptr->when = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->state = pelem->value;
	}
	return 1;
}


private int amazonEc2_delete(struct occi_category * optr, void * vptr)
{
	struct occi_kind_node * nptr;
	struct cords_amazonEc2 * pptr;
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
        sprintf(srcdir,"%s/pyaccords/pysrc/amazonEc2.py",PYPATH);
	exp_file = fopen(srcdir, "r");
	if(!exp_file) printf("error in amazonEc2Interface.c amazonEc2.py :No such file or directory\n");
	Py_Initialize();
	PyRun_SimpleFile(exp_file,srcdir);
	main_module = PyImport_AddModule("__main__");
	global_dict = PyModule_GetDict(main_module);
	cbFunc = PyDict_GetItemString(global_dict,"delete");
	if(!cbFunc) printf("error in amazonEc2Interface.c :no python function\n");
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
		pptr->floating = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->floatingid = pelem->value;
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
		pptr->when = pelem->value;
		pelem = pelem->next;
	}
	if(pelem){
		pptr->state = pelem->value;
	}
	return 1;
}


