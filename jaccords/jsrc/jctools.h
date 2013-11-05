/*-------------------------------------------------------------------------------*/
/* ACCORDS Platform                                                              */
/* copyright 2012 ,Hamid MEDJAHE  (hmedjahed@prologue.fr) Prologue               */
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
#ifndef _JCTOOLS_H_
#define _JCTOOLS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jpath.h"

typedef struct ele
{
 char value[256];
 struct ele *prev,*next;
}elemm;

typedef struct
{
  elemm *first;
  elemm *last;
}listcc;

void resetListe(listcc *l);
void addBacke(listcc *l, char *val);
void addFronte(listcc *l, char *val);
void liberateListe(listcc *l);

void jstrConcat(char str1[], char str2[],char c);
int strValid( char * vptr );


#endif

