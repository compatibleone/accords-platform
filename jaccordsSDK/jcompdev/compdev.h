/*-------------------------------------------------------------------------------*/
/* Accords Platform                                                              */
/* copyright 2012 ,Hamid MEDJAHE (hmedjahed@prologue.fr)    Prologue             */
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

#ifndef _COMPDEV_H
#define _COMPDEV_H
#include "../../cocci/src/accords.h"

void fillInAccordsConfiguration(struct accords_configuration *componentModule, char *moduleName);
struct rest_response * callocciCategoryAction(const char *name);
int callocciCategoryActionNumber(const char *name);
char * callocciCategoryActionName(const char *name, int a);
struct occi_category * callocciCategoryBuilder(const char *name, char *a,char *b);
struct occi_interface * callocciCategoryInterface(const char *name);
int module_main(int argc, char * argv[], char moduleName[], char categoryNameList[]);
int callocciCategoryAccess(const char *name);
	
#endif
