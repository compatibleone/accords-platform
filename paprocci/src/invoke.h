/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2012 by Oasis (INRIA Sophia Antipolis) and ActiveEon teams.     */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License"); 	*/
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at 				*/
/*  									*/
/*  http://www.apache.org/licenses/LICENSE-2.0 				*/
/*  									*/
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 	*/
/* implied. 								*/
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License. 					*/
/* -------------------------------------------------------------------- */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <dirent.h>

void destroy(JNIEnv * env, JavaVM * jvm);

struct list_jar {
    char* value;
    struct list_jar * next;
};

struct jvm_struct {
	JNIEnv * env;
	JavaVM * jvm;
	jobject * procci;
};

typedef struct list_jar item_jar;

char* getjars(const char* targetdir);

struct jvm_struct * startjvm();
void connect_proactive(struct jvm_struct * jvmp);

