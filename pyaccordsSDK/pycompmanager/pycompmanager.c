/* ----------------------------------------------------------------------------- */
/* ACCORDS Platform                                                              */
/* module to interface Accords platform  for Python                              */
/* Hamid MEDJAHED & Elyes ZEKRI for (C) 2011 Prologue                            */
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

#include <Python.h>
#include "compmanager.h"

static PyObject *pycompmanager_generateCategory(PyObject *self, PyObject *args)
{
 int dim;
 char * categoryAttributes;         
 char * categoryName;
 char * categoryActions;
 int flag;

 if (! PyArg_ParseTuple( args,"sssi",&categoryName, &categoryAttributes,&categoryActions,&flag)) return NULL;
 dim=generateAccordsCategory(categoryName,categoryAttributes,categoryActions,flag);
 if(dim <= 0) 
 {
   printf("Failure to create %s OCCI category\n",categoryName);
   return Py_BuildValue("i", dim);
 }
 else
 {
  printf(" %s OCCI category is created\n",categoryName); 
  return Py_BuildValue("i", dim);
 }

}


static PyObject *pycompmanager_removeCategory(PyObject *self, PyObject *args)
{
  char *categoryName;
  int a;
  int indice;
  int flag;
  if (! PyArg_ParseTuple( args,"sii",&categoryName,&indice,&flag)) return NULL;
  a=deleteCategory(categoryName,indice,flag);
  if(a)
  {
   printf("%s OCCI category is removed\n",categoryName);
   return Py_BuildValue("i",a);
  }
  else
  {
    printf("Failure to remove %s category\n",categoryName);
    return Py_BuildValue("i",a);
  }

}

static PyObject *pycompmanager_generateComponent(PyObject *self, PyObject *args)
{
 int a;          
 char * moduleName;
 char * categoryNameList;
 int paccess;
 int caccess;
 if (! PyArg_ParseTuple( args,"ssii",&moduleName,&categoryNameList,&paccess,&caccess)) return NULL;
 a=generateModuleFile(moduleName,categoryNameList,paccess,caccess);
 if(a)
 {
   printf(" %s component is created\n",moduleName);
   return Py_BuildValue("i", a);
 }
 else
 {
  printf(" Failure to create %s component\n",moduleName);
  return Py_BuildValue("i", a);
 }

}


static PyObject *pycompmanager_removeComponent(PyObject *self, PyObject *args)
{
 int a;          
 char * moduleName;
 
 if (! PyArg_ParseTuple( args,"s",&moduleName)) return NULL;
 
 a=deleteModule(moduleName);
 if(a)
 {
  printf(" %s component is removed\n",moduleName);
  return Py_BuildValue("i", a);
 }
 else
 {
  printf(" Failure to remove %s component\n",moduleName);
  return Py_BuildValue("i", a);
 }
 
}

static PyObject *pycompmanager_commit(PyObject *self, PyObject *args)
{
 int a;                 
 
 if (! PyArg_ParseTuple( args,"")) return NULL;
 
 a=commitPlatform();
 printf("changes are committed\n");
 return Py_BuildValue("i", a);

}

static PyMethodDef ModMethods[] = {
    {"generateCategory",pycompmanager_generateCategory, METH_VARARGS, "Function to generate a category"},
    {"generateComponent",pycompmanager_generateComponent, METH_VARARGS, "Function to generate  a component"},
    {"removeCategory",pycompmanager_removeCategory, METH_VARARGS, "Function to delete a category"},
    {"removeComponent",pycompmanager_removeComponent, METH_VARARGS, "Function to delete a component"},  
    {"commit",pycompmanager_commit, METH_VARARGS, "Function to delete a component"},  

    {NULL,NULL,0,NULL}
};


PyMODINIT_FUNC initpycompmanager(void)
{
    PyObject *m;
    m = Py_InitModule("pycompmanager",ModMethods);
    if (m == NULL)
    return;
}
