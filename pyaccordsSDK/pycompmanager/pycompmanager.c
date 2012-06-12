/* ---------------------------------------------------------------------------- */
/* Advanced Capabilities for Compatible One Resources Delivery System - ACCORDS	*/
/* module to interface Accords platform  for Python                             */
/* Hamid MEDJAHED & Elyes ZEKRI for (C) 2011 Prologue                           */
/* ---------------------------------------------------------------------------- */
/*										*/
/* This is free software; you can redistribute it and/or modify it		*/
/* under the terms of the GNU Lesser General Public License as			*/
/* published by the Free Software Foundation; either version 2.1 of		*/
/* the License, or (at your option) any later version.				*/
/*										*/
/* This software is distributed in the hope that it will be useful,		*/
/* but WITHOUT ANY WARRANTY; without even the implied warranty of		*/
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU		*/
/* Lesser General Public License for more details.				*/
/*										*/
/* You should have received a copy of the GNU Lesser General Public		*/
/* License along with this software; if not, write to the Free			*/
/* Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA		*/
/* 02110-1301 USA, or see the FSF site: http://www.fsf.org.			*/
/*										*/
/* ---------------------------------------------------------------------------- */

#include <Python.h>
#include "compmanager.h"

static PyObject *pycompmanager_generateCategory(PyObject *self, PyObject *args)
{
 int dim;
 char * categoryAttributes;         
 char * pathf;          
 char * categoryName;
 char * categoryActions;
 int flag;

 if (! PyArg_ParseTuple( args,"sssis",&categoryName, &categoryAttributes,&categoryActions,&flag,&pathf)) return NULL;
 dim=generateAccordsCategory(categoryName,categoryAttributes,categoryActions,flag,pathf);
 if(dim <= 0) return NULL;
 printf(" %s OCCI category is created\n",categoryName); 
 return Py_BuildValue("i", dim);

}


static PyObject *pycompmanager_removeCategory(PyObject *self, PyObject *args)
{
  char *categoryName;
  char * pathf;
  int a;
  int indice;
  int flag;
  if (! PyArg_ParseTuple( args,"siis",&categoryName,&indice,&flag,&pathf)) return NULL;
  a=deleteCategory(pathf,categoryName,indice,flag);
  printf("%s OCCI category is removed\n",categoryName);
  return Py_BuildValue("i",a);

}

static PyObject *pycompmanager_generateComponent(PyObject *self, PyObject *args)
{
 int a;          
 char * pathf;          
 char * moduleName;
 char * categoryNameList;
 char * categoryActionNumberList; 
 if (! PyArg_ParseTuple( args,"ssss",&moduleName,&categoryNameList,&categoryActionNumberList,&pathf)) return NULL;
 a=generateModuleFile(moduleName,categoryNameList,categoryActionNumberList,pathf);
 printf(" %s component is created\n",moduleName);
 return Py_BuildValue("i", a);

}


static PyObject *pycompmanager_removeComponent(PyObject *self, PyObject *args)
{
 int a;          
 char * pathf;          
 char * moduleName;
 
 if (! PyArg_ParseTuple( args,"ss",&moduleName,&pathf)) return NULL;
 
 a=deleteModule(moduleName,pathf);
 printf(" %s component is removed\n",moduleName);
 return Py_BuildValue("i", a);

}

static PyObject *pycompmanager_commit(PyObject *self, PyObject *args)
{
 int a;          
 char * pathf;          
 
 if (! PyArg_ParseTuple( args,"s", &pathf)) return NULL;
 
 a=commitPlatform(pathf);
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
