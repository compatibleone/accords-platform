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
#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

typedef struct el
{
 char value[1024];
 struct el *prev,*next;
}elem;

typedef struct
{
  elem *first;
  elem *last;
}listc;

void resetList(listc *l);
void addBack(listc *l, char *val);
void addFront(listc *l, char *val);
void liberateList(listc *l);

#endif
