#ifndef _COMPMANAGER_H_
#define _COMPMANAGER_H_


#include "utools.h"

int generateAccordsCategory(char *name,char *categoryAttributes, char *categoryActions, char *categoryAccess);
int createCategoryCordsCfile(char *categoryName, listc categoryAttributes, int dim, int flag, char pathf[]);
int createCategoryOcciFile(char *categoryName, listc categoryAttributes, int dim, int flag, char pathf[]);
int enTete(char pathf[]);
int generateModuleFile(char * moduleName, char * categoryNameList);
int generateCategoryInterfceStructFile(char pathf[]);
int generateCategoryInterfaceCfile(char *categoryName, listc categoryAtr, int flag, char pathf[]);
int generateCategoryPySourcefile(char *categoryName, listc categoryAtr, char pathf[]);
int deleteCategory(char categoryName[],int indice);
int deleteModule(char moduleName[]);
int commitPlatform();
int generateCategoryActionStruct(char *categoryName, listc categoryAct, int n, char pathf[]);
int generateCategoryActionPyfile(char *categoryName, listc categoryAtr, listc categoryAct, char pathf[]);
int generateCategoryActionCfile(char *categoryName, listc categoryAtr, listc categoryAct, int flag, char pathf[]);
int generatePyCategoryTypeStruct(char *categoryName, char * categoryAccess, char pathf[]);
int callocciCategoryAct(const char *name);

#endif
