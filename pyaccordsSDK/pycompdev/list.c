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
   strncpy(nouv->value,val,255);
   nouv->value[255]='\0';
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
