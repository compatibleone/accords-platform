/*-------------------------------------------------------------------------------*/
/* ACCORDS Platform                                                              */
/* copyright 2012 ,Hamid MEDJAHE & Elyes ZEKRI  (hmedjahed@prologue.fr) Prologue */
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
#include "occi.h"
#include "ctools.h"
#include "pytools.h"
#include "ec2config.h"
#include <Python.h>

//            category current action  
struct rest_response * current_ec2config(
	struct occi_category * optr,
	struct rest_client * cptr,
	struct rest_request * rptr,
	struct rest_response * aptr,
	void * vptr )
{
	struct ec2config * pptr;
	char sendstr[1024]=" ";
	char strtmp[1024]=" ";
	int  status = 0;
	char message[1024];
        char srcdir[1024];
	char * response;
	char * token;
	PyObject    *pName=NULL, *pModule=NULL, *pDict=NULL, *pFunc=NULL,*result=NULL;
	PyThreadState* pythr=NULL;
        listcc restResponse;

	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else{
		
		if(!(strValid(pptr->id))) strcpy(sendstr," ");
		else strcpy(sendstr,pptr->id);

		if(!(strValid(pptr->name))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->name,',');

		if(!(strValid(pptr->description))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->description,',');

		if(!(strValid(pptr->user))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->user,',');

		if(!(strValid(pptr->password))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->password,',');

		if(!(strValid(pptr->accesskey))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->accesskey,',');

		if(!(strValid(pptr->secretkey))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->secretkey,',');

		if(!(strValid(pptr->authenticate))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->authenticate,',');

		if(!(strValid(pptr->agent))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->agent,',');

		if(!(strValid(pptr->host))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->host,',');

		
		if(!(strValid(pptr->version))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->version,',');
		
		if(!(strValid(pptr->location))){
			strcpy(strtmp," ");
                	strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->location,',');

		if(!(strValid(pptr->namespace))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->namespace,',');

		if(!(strValid(pptr->base))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->base,',');

		if(!(strValid(pptr->tls))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->tls,',');

		if(!(strValid(pptr->current))){
			strcpy(strtmp," ");
			strConcat(sendstr,strtmp,',');
		}
		else strConcat(sendstr,pptr->current,',');

		//           python interface
		sprintf(srcdir,"%s/pyaccords/pysrc",PYPATH);
		pythr = Py_NewInterpreter();
		python_path(srcdir);
		pName = PyString_FromString("ec2configAct");
		if(pName == NULL) printf("erro: in ec2configAct.py no such file name\n");
		else pModule = PyImport_Import(pName);
		if(pModule == NULL) printf("error: failed to load ec2configAct module\n");
		else pDict = PyModule_GetDict(pModule);
		if(pDict == NULL) printf("error: failed to load dict name in ec2configAct module\n");
		else pFunc = PyDict_GetItemString(pDict,"current");
		if(pFunc == NULL) printf("error: failed to load start function in ec2configAct module\n");
		else result=PyObject_CallFunction(pFunc,"s",sendstr);
	
		if (!result || PyErr_Occurred());
        	{
       			PyErr_Print();
       			return (rest_html_response( aptr, 404, "Invalid Action" ) );
       		}

		response=allocate_string(PyString_AsString( result ));
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

char * ec2config_getname(int a)
{
  static char action[256];
  
  switch (a)
  {
      case 0:
          strcpy(action,"current");
          break;
      default:
          strcpy(action,"ec2config");
          break;
  }
  return action;
}

int ec2config_getnumber()
{
 return 1;
}

