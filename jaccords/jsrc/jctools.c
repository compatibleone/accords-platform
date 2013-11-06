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

#include "jctools.h"

void jstrConcat(char str1[],char str2[],char c)
{
    	int i=0,j=0;
   
   
    	while(str1[i]!='\0'){
        	 i++;
    	}
    	str1[i]=c;
    	i++;
    	while(str2[j]!='\0'){
         	str1[i] = str2[j];   
         	i++;
         	j++;
    	}
    
    	str1[i] = '\0';
}

/*------------------------------------------------------------------------------*/
void resetListe(listcc *l)
{
   	l->first = NULL;
   	l->last = NULL;
}
/*------------------------------------------------------------------------------*/
void addBacke(listcc *l, char *val)
{
   	elemm *nouv = malloc(sizeof(elemm));
   	if(!nouv) exit(0);
   	strcpy(nouv->value,val);
   	nouv->prev = l->last;
   	nouv->next = NULL;
   	if(l->last) l->last->next = nouv;
   	else l->first = nouv;
   	l->last = nouv;        
}
/*-----------------------------------------------------------------------------*/
void addFronte(listcc *l, char *val)
{
  	elemm *nouv = malloc(sizeof(elemm));
  	if(!nouv) exit(EXIT_FAILURE);
   	strcpy(nouv->value,val);
   	nouv->next = l->first;
   	nouv->prev = NULL;      
   	if(l->first) l->first->prev = nouv;
   	else l->last = nouv;
   	l->first = nouv;
}
/*-----------------------------------------------------------------------------*/
void liberateListe(listcc *l)
{
   	elemm *tmp;
   	elemm *pelem = l->first;
   	while(pelem)
  	{
     		tmp = pelem;
     		pelem = pelem->next;
     		free(tmp);
   	}
   	l->first = NULL;
   	l->last = NULL;
}

/*-----------------------------------------------------------------------------*/
int strValid( char * vptr )
{
        if (!( vptr ))
                return( 0 );
        else if (!( strlen( vptr ) ))
                return( 0 );
        else if (!( strcmp( vptr, "(null)" ) ))
                return( 0 );
        else if (!( strcmp( vptr, "(none)" ) ))
                return( 0 );
        else    return( 1 );
}

