#ifndef _COMPMANAGER_H_
#define _COMPMANAGER_H_


#include "utools.h"

int generateAccordsCategory(char *name,char *categoryAttributes,char *categoryActions,char pathf[]);
int createCategoryCordsCfile(char *categoryName,listc categoryAttributes,int dim, char pathf[]);
int createCategoryOcciFile(char *categoryName,listc categoryAttributes,int dim,char pathf[]);
int enTete(char pathf[]);
int generateModuleFile(char * moduleName, char * categoryNameList, char * categoryActionNumberList,char * pathf);
int generateCategoryInterfceStructFile(char pathf[]);
int generateCategoryInterfaceCfile(char *categoryName,listc categoryAtr,char pathf[]);
int generateCategoryPySourcefile(char *categoryName,listc categoryAtr,char pathf[]);
int deleteCategory(char pathf[], char categoryName[],int indice);
int deleteModule(char moduleName[],char pathf[]);
int commitPlatform(char pathf[]);
int generateCategoryActionStruct(char *categoryName,listc categoryAct,char pathf[]);
int generateCategoryActionPyfile(char *categoryName,listc categoryAtr,listc categoryAct,char pathf[]);
int generateCategoryActionCfile(char *categoryName,listc categoryAtr,listc categoryAct,char pathf[]);



#endif
