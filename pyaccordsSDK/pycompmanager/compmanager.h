#ifndef _COMPMANAGER_H_
#define _COMPMANAGER_H_


#include "utools.h"

int generateAccordsCategory(char *name,char *attrib,char pathf[]);
int createCategoryCordsCfile(char *categoryName,listc categoryAttributes,int dim, char pathf[]);
int createCategoryOcciFile(char *categoryName,listc categoryAttributes,int dim,char pathf[]);
int enTete(char pathf[]);
int generateModuleFile(char * moduleName, char * categoryNameList, char * pathf);
int generateCategoryInterfceStructFile(char pathf[]);
int generateCategoryInterfaceCfile(char *categoryName,listc categoryAtr,char pathf[]);
int generateCategoryPySourcefile(char *categoryName,listc categoryAtr,char pathf[]);
int deleteCategory(char pathf[], char categoryName[]);
int deleteModule(char moduleName[],char pathf[]);
int commitPlatform(char pathf[]);

#endif
