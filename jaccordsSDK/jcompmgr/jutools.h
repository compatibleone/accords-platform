/* ----------------------------------------------------------------------------- */
/* ACCORDS Platform                                                        	 */
/* module to interface Accords platform  for java                                */
/* Copyright 2012  Hamid MEDJAHED (hmedjahed@prologue.fr) Prologue               */
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

#ifndef _JUTOOLS_H_
#define _JUTOOLS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include "jlist.h"
#include "../../jaccords/jsrc/jpath.h"

#define DIM 1024
#define CORDS_SRC "cords"
#define INCLUDE_CORDS_H "cords/cords.h"
#define INCLUDE_OCCI_BUILDER "occi/occibuilder.h"
#define INCLUDE_OCCI_PROVIDER_BUILDER "jaccords/jsrc/jlistoccibuilder.h"
#define INCLUDE_CORDS_BASE "cords/cordsbase.c"
#define OCCI_PATH "occi"
#define OCCI_CORDS "occi/cordsocci.c"
#define JACCORDS "jaccords"
#define JACCORDSS "jsrc"
#define J_CRUD_INCLUDE "jaccords/jsrc/jcategcrud.h"
#define J_CRUD_STRUCT "jaccords/jsrc/jcrudinterf.h"
#define JCODEV_DIR "jaccordsSDK/jcompdev/"
#define JLISTCATEG_FILE "jaccords/jsrc/jlistcateg.h"
#define LIB_JCOMPDEV "jcompdev"
#define J_ACT_STRUCT "jaccords/jsrc/jcategaction.h"
#define J_ACT_NAME_STRUCT "jaccords/jsrc/jcategactionname.h"
#define J_ACT_NUMBER_STRUCT "jaccords/jsrc/jcategactionnumber.h"
#define J_ACT_LIST "jaccords/jsrc/jlistaction.h"
#define J_CLASS_SRC "jaccords/jsrc/jclass/"
#define J_CATEGACCESS_STRUCT "jaccords/jsrc/jcategaccess.h"

int insertCategory(char pathf[], char categoryName[],int indice,int flag);
void str_sub(const char *s, unsigned int start, unsigned int end, char new_s[]);
char *sub_str(const char *s, unsigned int start, unsigned int end);
int insertInFile(char pathf[],char categoryName[],char categoryNames[]);
int enTete(char pathf[]);
char *getCategoryName(char strCt[],char *tok,int p);
int getLineNumber(char pathf[]);
int searchWord(char mot[], char line[]);
int deleteInFile(char pathf[],char categoryName[]);
void strConcat(char str1[],char str2[], char d);
int insertInFileJStruct(char pathf[],char categoryName[],char categoryNames[]);
int insertInFileOcciStruct(char pathf[],char categoryName[],char categoryNames[]);

#endif
