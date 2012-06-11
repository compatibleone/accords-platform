/* ---------------------------------------------------------------------------- */
/* Advanced Capabilities for Compatible One Resources Delivery System - ACCORDS	*/
/* module to launch a new Accords component                                     */
/* Hamid MEDJAHED & Elyes ZEKRI for (C) 2011 Prologue              		*/
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
#include "component.c"


/***************************************************************************************************************/
/* Function to launch a new accords module (component)                                                         */
/***************************************************************************************************************/
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
 
 if (! PyArg_ParseTuple( args,"iO!sss", &argc,&PyList_Type, &listObj,&moduleName, &categoryNameList,&categoryActionNumberList)) return NULL;
 numLines = PyList_Size(listObj);

 if (numLines < 0)   return NULL;
 argv = (char**) malloc(sizeof (char*) * numLines);
  for(i = 0; i < numLines; i++)
      argv= (char*) malloc(sizeof (char) * 256);

 for (i=0; i<numLines; i++)
 {

  strObj = PyList_GetItem(listObj, i); 
  argv[i] = PyString_AsString( strObj ); 

 }

 a=module(argc,argv,moduleName,categoryNameList,categoryActionNumberList);


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
