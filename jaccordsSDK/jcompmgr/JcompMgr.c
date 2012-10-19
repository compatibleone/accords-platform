/* ----------------------------------------------------------------------------- */
/* ACCORDS Platform                                                              */
/* module to interface Accords platform  for Java                                */
/* Hamid MEDJAHED (c) 2012 for Prologue                                          */
/* ----------------------------------------------------------------------------- */
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

#include "compmgr.h"
#include "JcompMgr.h"

JNIEXPORT jint JNICALL Java_JcompMgr_generateCategory(
	JNIEnv *env, 
	jobject thisobject, 
	jstring categoryN, 
	jstring categoryAtr, 
	jstring categoryAct, 
	jstring categoryA, 
	jint flag)
{
	int dim;
	char * categoryName = (char*) (*env)->GetStringUTFChars(env, categoryN, 0);
	char * categoryAttributes = (char*) (*env)->GetStringUTFChars(env, categoryAtr, 0);         
	char * categoryActions = (char*) (*env)->GetStringUTFChars(env, categoryAct, 0) ;
	char * categoryAccess = (char*) (*env)->GetStringUTFChars(env, categoryA, 0);
	int flags = flag;
 
	dim=generateJAccordsCategory(categoryName, categoryAttributes, categoryActions, categoryAccess, flags);
	if(dim <= 0) 
	{
		printf("Failure to create %s OCCI category\n",categoryName);
		(*env)->ReleaseStringUTFChars(env, categoryN, categoryName);
		(*env)->ReleaseStringUTFChars(env, categoryAtr, categoryAttributes);
		(*env)->ReleaseStringUTFChars(env, categoryAct, categoryActions);
		(*env)->ReleaseStringUTFChars(env, categoryA, categoryAccess);
		return dim;
	}
	else
	{
		printf(" %s OCCI category is created\n",categoryName); 
		(*env)->ReleaseStringUTFChars(env, categoryN, categoryName);
		(*env)->ReleaseStringUTFChars(env, categoryAtr, categoryAttributes);
		(*env)->ReleaseStringUTFChars(env, categoryAct, categoryActions);
		(*env)->ReleaseStringUTFChars(env, categoryA, categoryAccess);
		return dim;
	}
 
}


JNIEXPORT jint JNICALL  Java_JcompMgr_removeCategory(
	JNIEnv *env,
	jobject thisobject,
	jstring categoryN,
	jint index,
	jint flag)
{
	int a;
	char *categoryName = (char*)(*env)->GetStringUTFChars(env, categoryN, 0);
	int indice = index;
	int flags = flag;
	
	a=deleteJCategory(categoryName,indice,flag);
	if(a)
	{
		printf("%s OCCI category is removed\n",categoryName);
		(*env)->ReleaseStringUTFChars(env, categoryN, categoryName);
                return a;
	}
	else
	{
		printf("Failure to remove %s category\n",categoryName);
    		(*env)->ReleaseStringUTFChars(env, categoryN, categoryName);
		return a;
	}

}

JNIEXPORT jint JNICALL  Java_JcompMgr_generateComponent(
	JNIEnv *env, 
	jobject thisobject,
	jstring moduleN,
        jstring categoryL)
        
{
	int a;          
	char * moduleName = (char*)(*env)->GetStringUTFChars(env, moduleN, 0);
	char * categoryNameList = (char*) (*env)->GetStringUTFChars(env, categoryL, 0);
	
	a=generateJModuleFile(moduleName,categoryNameList);
	if(a)
	{
		printf(" %s component is created\n",moduleName);
                (*env)->ReleaseStringUTFChars(env, moduleN, moduleName);
		(*env)->ReleaseStringUTFChars(env, categoryL, categoryNameList);
		return a;
	}
	else
	{
		printf(" Failure to create %s component\n",moduleName);
                (*env)->ReleaseStringUTFChars(env, moduleN, moduleName);
		(*env)->ReleaseStringUTFChars(env, categoryL, categoryNameList);
		return a;
	}
}


JNIEXPORT jint JNICALL Java_JcompMgr_removeComponent(
	JNIEnv *env,
	jobject thisobject,
	jstring moduleN)
{
	int a;          
	char * moduleName = (char*)(*env)->GetStringUTFChars(env, moduleN, 0);
 
	a=deleteJModule(moduleName);
	if(a)
	{
		printf(" %s component is removed\n",moduleName);
		(*env)->ReleaseStringUTFChars(env, moduleN, moduleName);
		return a;
	}
	else
	{
  		printf(" Failure to remove %s component\n",moduleName);
		(*env)->ReleaseStringUTFChars(env, moduleN, moduleName);
		return a;
	}
 
}
