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

#ifndef _COMPMANAGER_H_
#define _COMPMANAGER_H_


#include "jutools.h"

int generateJAccordsCategory(char *name,char *categoryAttributes, char *categoryActions, char * categoryAccess);
int createJCategoryCordsCfile(char *categoryName, listc categoryAttributes, int dim, int flag, char pathf[]);
int createJCategoryOcciFile(char *categoryName, listc categoryAttributes, int dim, int flag, char pathf[]);
int enJTete(char pathf[]);
int generateJModuleFile(char * moduleName, char * categoryNameList);
int generateJCategoryInterfceStructFile(char pathf[]);
int generateJCategoryInterfaceCfile(char *categoryName, listc categoryAtr, int flag, char pathf[]);
int generateCategoryJSourcefile(char *categoryName, listc categoryAtr, char pathf[]);
int deleteJCategory(char categoryName[], int indice);
int deleteJModule(char moduleName[]);
int generateJCategoryActionStruct(char *categoryName, listc categoryAct, int n, char pathf[]);
int generateCategoryActionJfile(char *categoryName, listc categoryAtr, listc categoryAct, char pathf[]);
int generateJCategoryActionCfile(char *categoryName, listc categoryAtr, listc categoryAct, int flag, char pathf[]);
int generateJCategoryTypeStruct(char *categoryName, char * categoryAccess, char pathf[]);
int callocciCategoryActionNumber(const char *name);

#endif
