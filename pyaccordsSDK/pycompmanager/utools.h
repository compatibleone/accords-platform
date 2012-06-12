#ifndef _UTOOLS_H_
#define _UTOOLS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include "list.h"


#define TAILLE 256
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
#define PY_ACT_LIST "pyaccords/pysrc/listaction.h"

int insertCategory(char pathf[], char categoryName[],int indice,int flag);
void str_sub(const char *s, unsigned int start, unsigned int end, char new_s[]);
int insertInFile(char pathf[],char categoryName[],char categoryNames[],int indice);
int enTete(char pathf[]);
char *getCategoryName(char strCt[],char *tok,int p);
int getLineNumber(char pathf[]);
int searchWord(char *mot,char *line);
int deleteInFile(char pathf[],char categoryName[]);
void strConcat(char str1[],char str2[], char d);


 
#endif
