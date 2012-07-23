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
#include "list.h"

/******************************************************************************/
void resetList(listc *l)
{
   l->first = NULL;
   l->last = NULL;
}
/******************************************************************************/
void addBack(listc *l, char *val)
{
   elem *nouv = malloc(sizeof(elem));
   if(!nouv) exit(EXIT_FAILURE);
   strncpy(nouv->value,val,255);
   nouv->value[255]='\0';
   nouv->prev = l->last;
   nouv->next = NULL;
   if(l->last) l->last->next = nouv;
   else l->first = nouv;
   l->last = nouv;        
}
/******************************************************************************/
void addFront(listc *l, char *val)
{
   elem *nouv = malloc(sizeof(elem));
   if(!nouv) exit(EXIT_FAILURE);
   strncpy(nouv->value,val,1024);
   nouv->value[1024]='\0';
   nouv->next = l->first;
   nouv->prev = NULL;      
   if(l->first) l->first->prev = nouv;
   else l->last = nouv;
   l->first = nouv;
}

/*******************************************************************************/
void liberateList(listc *l)
{
   elem *tmp;
   elem *pelem = l->first;
   while(pelem)
   {
     tmp = pelem;
     pelem = pelem->next;
     free(tmp);
   }
   l->first = NULL;
   l->last = NULL;
}
