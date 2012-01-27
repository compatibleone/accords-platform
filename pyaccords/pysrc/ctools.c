#include "ctools.h"

void strConcat(char str1[],char str2[],char c)
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

/******************************************************************************/
void resetListe(listcc *l)
{
   l->first = NULL;
   l->last = NULL;
}
/******************************************************************************/
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
/******************************************************************************/
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

/******************************************************************************/
/*char *retBack(listc *l)
{
   char val[256];
   elem *tmp = l->last;
   if(!tmp) return NULL;
   strcpy(val,tmp->value);
   l->last = tmp->prev;
   if(l->last) l->last->next = NULL;
   else l->first = NULL;
   free(tmp);
   return val;
}*/
/******************************************************************************/
/*char *retFront(listc *l)
{
   char val[256];
   elem *tmp = l->first;
   if(!tmp) return NULL;
   strcpy(val,tmp->value);
   l->first = tmp->next;
   if(l->first)l->first->prev = NULL;
   else l->last = NULL;
   free(tmp);
   return val;
}*/
/*******************************************************************************/
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
