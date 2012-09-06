/*-------------------------------------------------------------------------------*/
/* accords platform                                                              */
/* copyright 2012, Hamid MEDJAHED & Elyes ZEKRI (hmedjahed@prologue.fr) prologue */
/*-------------------------------------------------------------------------------*/
/* licensed under the apache license, version 2.0 (the "license");               */
/* you may not use this file except in compliance with the license.              */
/* you may obtain a copy of the license at                                       */
/*                                                                               */
/*       http://www.apache.org/licenses/license-2.0                              */
/*                                                                               */
/* unless required by applicable law or agreed to in writing, software           */
/* distributed under the license is distributed on an "as is" basis,             */
/* without warranties or conditions of any kind, either express or implied.      */
/* see the license for the specific language governing permissions and           */
/* limitations under the license.                                                */
/*-------------------------------------------------------------------------------*/
#ifndef	_ec2procci_boto_c
#define	_ec2procci_boto_c
#define DEFAULT_IMAGE "ami-03c2f677"
#include "ctools.h"
#include <Python.h>


private int ec2_initialise_client(char* accesskey, char* privatekey, char* agent, char* tls)
{
        char srcdir[1024];
	FILE * exp_file;
	PyObject*    main_module, * global_dict, * cbFunc, *result;
	char * response;
	//           python interface
        sprintf(srcdir,"%s/cocarrier/src/ec2client.py",PYPATH);
	exp_file = fopen(srcdir, "r");
	if(!exp_file) printf("error in ec2_procci_boto.c ec2client.py :No such file or directory\n");
	Py_Initialize();
	PyRun_SimpleFile(exp_file, srcdir);
	main_module = PyImport_AddModule("__main__");
	global_dict = PyModule_GetDict(main_module);
	cbFunc = PyDict_GetItemString(global_dict,"ec2_initialise_client");
	if(!cbFunc) printf("error in ec2_procci_boto.c :no python function ec2_initialise_client\n");
	result=PyObject_CallFunction(cbFunc,"ssss",accesskey, privatekey, agent, tls);
	response=PyString_AsString( result );
	Py_Finalize();

	
	return 1;
}





/*	-----------------------------------------------------------------	*/
/*		r e s o l v e _ c o n t r a c t _ i m a g e   			*/
/*	-----------------------------------------------------------------	*/
private	char *	resolve_contract_image( char* accesskey, char* privatekey, char* zone, char* imageID )
{
	char srcdir[1024];
	FILE * exp_file;
	PyObject*    main_module, * global_dict, * cbFunc, *result;
	char * response;
	//           python interface
        sprintf(srcdir,"%s/cocarrier/src/ec2client.py",PYPATH);
	exp_file = fopen(srcdir, "r");
	if(!exp_file) printf("error in ec2_procci_boto.c ec2client.py :No such file or directory\n");
	Py_Initialize();
	PyRun_SimpleFile(exp_file,srcdir);
	main_module = PyImport_AddModule("__main__");
	global_dict = PyModule_GetDict(main_module);
	cbFunc = PyDict_GetItemString(global_dict,"ec2_resolve_image_g");
	if(!cbFunc) printf("error in ec2_procci_boto.c :no python function ec2_resolve_image_g \n");
	result=PyObject_CallFunction(cbFunc,"ssss",accesskey, privatekey,zone, imageID);
	response=PyString_AsString( result );
	Py_Finalize();
	if (!strcmp(response,"OK"))
		return imageID;
	else return DEFAULT_IMAGE;
}



/*	-----------------------------------------------------------------	*/
/*		r e s o l v e _ c o n t r a c t _ f l a v o r  			*/
/*	-----------------------------------------------------------------	*/
private	char *	resolve_contract_flavor()
{
	char srcdir[1024];
	FILE * exp_file;
	PyObject*    main_module, * global_dict, * cbFunc, *result;
	char * response;
	//           python interface
        sprintf(srcdir,"%s/cocarrier/src/ec2client.py",PYPATH);
	exp_file = fopen(srcdir, "r");
	if(!exp_file) printf("error in ec2_procci_boto.c ec2client.py :No such file or directory\n");
	Py_Initialize();
	PyRun_SimpleFile(exp_file,srcdir);
	main_module = PyImport_AddModule("__main__");
	global_dict = PyModule_GetDict(main_module);
	cbFunc = PyDict_GetItemString(global_dict,"ec2_resolve_flavor");
	if(!cbFunc) printf("error in ec2_procci_boto.c :no python function ec2_resolve_flavor\n");
	result=PyObject_CallFunction(cbFunc,NULL);
	response=PyString_AsString( result );
	Py_Finalize();
	return response;
	
}




#endif
