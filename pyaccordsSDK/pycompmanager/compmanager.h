#ifndef _COMPMANAGER_H_
#define _COMPMANAGER_H_


#include "utools.h"

int generateAccordsCategory(char *name,char *categoryAttributes,char *categoryActions,int flag);
int createCategoryCordsCfile(char *categoryName,listc categoryAttributes,int dim, char pathf[]);
int createCategoryOcciFile(char *categoryName,listc categoryAttributes,int dim,char pathf[]);
int enTete(char pathf[]);
int generateModuleFile(char * moduleName, char * categoryNameList);
int generateCategoryInterfceStructFile(char pathf[]);
int generateCategoryInterfaceCfile(char *categoryName,listc categoryAtr,char pathf[]);
int generateCategoryPySourcefile(char *categoryName,listc categoryAtr,char pathf[]);
int deleteCategory(char categoryName[],int indice,int flag);
int deleteModule(char moduleName[]);
int commitPlatform();
int generateCategoryActionStruct(char *categoryName,listc categoryAct,int n,char pathf[]);
int generateCategoryActionPyfile(char *categoryName,listc categoryAtr,listc categoryAct,char pathf[]);
int generateCategoryActionCfile(char *categoryName,listc categoryAtr,listc categoryAct,char pathf[]);

#endif
