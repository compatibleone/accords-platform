/* ----------------------------------------------------------------------------- */
/* ACCORDS Platform                                                        	 */
/* module to interface Accords platform  for Python                              */
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

#ifndef _UTOOLS_H_
#define _UTOOLS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include "list.h"
#include "../../pyaccords/pysrc/pypath.h"

#define DIM 1024
#define CORDS_SRC "cords/src"
#define INCLUDE_CORDS_H "cords/src/cords.h"
#define INCLUDE_OCCI_BUILDER "occi/src/occibuilder.h"
#define INCLUDE_OCCI_PROVIDER_BUILDER "pyaccords/pysrc/listoccibuilder.h"
#define INCLUDE_CORDS_BASE "cords/src/cordsbase.c"
#define OCCI_PATH "occi/src"
#define OCCI_CORDS "occi/src/cordsocci.c"
#define PYACCORDS "pyaccords"
#define PYACCORDSS "pysrc"
#define PY_CRUD_INCLUDE "pyaccords/pysrc/categcrud.h"
#define PY_CRUD_STRUCT "pyaccords/pysrc/crudinterf.h"
#define PYCODEV_DIR "pyaccordsSDK/pycompdev/"
#define LISTCATEG_FILE "pyaccords/pysrc/listcateg.h"
#define LIB_PYCOMPDEV "pycompdev"
#define PY_ACT_STRUCT "pyaccords/pysrc/categaction.h"
#define PY_ACT_NAME_STRUCT "pyaccords/pysrc/categactionname.h"
#define PY_ACT_NUMBER_STRUCT "pyaccords/pysrc/categactionnumber.h"
#define PY_ACT_LIST "pyaccords/pysrc/listaction.h"
#define PY_CATEGACCESS_STRUCT "pyaccords/pysrc/categaccess.h"

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
int insertInFilePyStruct(char pathf[],char categoryName[],char categoryNames[]);
int insertInFileOcciStruct(char pathf[],char categoryName[],char categoryNames[]);
#endif
