/*-------------------------------------------------------------------------------*/
/* ACCORDS Platform                                                              */
/* copyright 2012 ,Hamid MEDJAHED  (hmedjahed@prologue.fr)    Prologue           */
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

#include "jtools.h"

JNIEnv *createJvm(JavaVM ** jvm, char path[])
{
	JavaVMOption options;
	JNIEnv *env;
	long status;
	JavaVMInitArgs vm_args;
	char jcpath[1024];
	sprintf(jcpath,"-Djava.class.path=%s",path);
	options.optionString = jcpath;
	vm_args.version = JNI_VERSION_1_6;
	vm_args.nOptions = 1;
	vm_args.options = &options;
	vm_args.ignoreUnrecognized = 0;
	status =  JNI_CreateJavaVM(jvm, (void**)&env, &vm_args);
	if(status != JNI_ERR)
	{
		return env;
	}
	else
	{
	 	printf("Error: Unable to Launch JVM\n");
                exit(0);
                return 0;
	}
}

JNIEnv * Getenv()
{
    	JavaVM* jvm = (JavaVM*) malloc (sizeof(JavaVM*) * 3);
    	jsize amount;
        jint resultAttach;
	JNIEnv * env = NULL;

    	jint result = JNI_GetCreatedJavaVMs(&jvm, 3, &amount);
    	if(amount != 1){
        	fprintf(stderr, "Expected one JVM, but %d were found...\n", amount);
        	return NULL;
    	}	

    	resultAttach = (*jvm)->AttachCurrentThread(jvm, (void**)&env, NULL); 
    	return env;
}

int Detachenv()
{
    	JavaVM* jvm = (JavaVM*) malloc (sizeof(JavaVM*) * 3);
    	jsize amount;
        jint resultAttach;
	JNIEnv * env = NULL;

    	jint result = JNI_GetCreatedJavaVMs(&jvm, 3, &amount);
    	if(amount != 1){
        	fprintf(stderr, "Expected one JVM, but %d were found...\n", amount);
        	return;
    	}	
    	resultAttach = (*jvm)->DetachCurrentThread(jvm); 
}


