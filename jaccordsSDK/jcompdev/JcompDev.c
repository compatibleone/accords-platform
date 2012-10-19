/*-------------------------------------------------------------------------------*/
/* ACCORDS Platform                                                              */
/* copyright 2012 ,Hamid MEDJAHE  (hmedjahed@prologue.fr) Prologue               */
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
#include "JcompDev.h"
#include "compdev.h"
#include <string.h>
#include <stdlib.h>


/*--------------------------------------------------------------------------------*/
/* Function to launch a new accords module (component)                            */
/*--------------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_JcompDev_launchModule(
	JNIEnv *env,
	jobject thisobject,
	jint a, 
	jobjectArray jargs, 
	jstring moduleN, 
	jstring categoryL)
{       
 	int b; 
	int i;
	jsize len;
	jobject jobj;           
	const char *str;
        char **argv; 
	int argc = a;
 	char * categoryNameList = (char*)(*env)->GetStringUTFChars(env,categoryL,0);
 	char *moduleName = (char*)(*env)->GetStringUTFChars(env,moduleN,0);
	len = (*env)->GetArrayLength(env, jargs);
	argv = (char**) malloc(sizeof(char*) * len);
        for(i=0; i<len; i++)
	{
		jobj = (*env)->GetObjectArrayElement(env, jargs, i);
		str = (*env)->GetStringUTFChars(env, jobj, 0);
		argv[i] = (char*) malloc(sizeof(char) * strlen(str) +1 );
		strcpy(argv[i], str);
	}

	b=module_main(argc,argv,moduleName,categoryNameList);

 	return b;
}

