/*-------------------------------------------------------------------------------*/
/* ACCORDS Platform                                                              */
/* copyright 2012 ,Hamid MEDJAHE & Elyes ZEKRI  (hmedjahed@prologue.fr) Prologue */
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

#include <Python.h>
#include "component.c"

/*--------------------------------------------------------------------------------*/
/* Function to launch a new accords module (component)                            */
/*--------------------------------------------------------------------------------*/
static PyObject *pycompdev_launchModule(PyObject *self, PyObject *args)
{                    
 char * categoryNameList;
 char * categoryActionNumberList;
 char **argv;
 int argc;
 char *moduleName;
 int a;
 int i;
 PyObject * listObj; /* the list of strings */
 PyObject * strObj;  /* one string in the list */
 int numLines;
 
 if (! PyArg_ParseTuple( args,"iO!ss", &argc,&PyList_Type, &listObj,&moduleName, &categoryNameList)) return NULL;
 numLines = PyList_Size(listObj);

 if (numLines < 0)   return NULL;
 argv = (char**) malloc(sizeof (char*) * numLines);
  for(i = 0; i < numLines; i++)
      argv= (char**) malloc(sizeof (char *) * 1024);

 for (i=0; i<numLines; i++)
 {

  strObj = PyList_GetItem(listObj, i); 
  argv[i] = PyString_AsString( strObj ); 

 }

 a=module_main(argc,argv,moduleName,categoryNameList);

 return Py_BuildValue("i",a);
}

static PyMethodDef ModMethods[] = {
    {"launchModule",pycompdev_launchModule, METH_VARARGS, "Function to launch a module"},    
    {NULL,NULL,0,NULL}
};

PyMODINIT_FUNC initpycompdev(void)
{
    PyObject *m;
    m = Py_InitModule("pycompdev",ModMethods);
    if (m == NULL)
    return;
}
