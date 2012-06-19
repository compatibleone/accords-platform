/* ------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                   */
/*  (C) 2011 by Hamid MEDJAHED Elyes ZEKRi Prologue                    */
/* --------------------------------------------------------------------*/
/*  This is free software; you can redistribute it and/or modify it    */
/*  under the terms of the GNU Lesser General Public License as        */
/*  published by the Free Software Foundation; either version 2.1 of   */
/*  the License, or (at your option) any later version.                */
/*                                                                     */
/*  This software is distributed in the hope that it will be useful,   */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of     */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   */
/*  Lesser General Public License for more details.                    */
/*                                                                     */
/*  You should have received a copy of the GNU Lesser General Public   */
/*  License along with this software; if not, write to the Free        */
/*  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA */
/*  02110-1301 USA, or see the FSF site: http://www.fsf.org.           */
/* --------------------------------------------------------------------*/
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
