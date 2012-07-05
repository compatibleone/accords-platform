/* ------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                   */
/*  (C) 2012 by Hamid MEDJAHED (Prologue) <ijm667@hotmail.com>         */
/* --------------------------------------------------------------------*/
/*  This is free software; you can redistribute it and/or modify it    */
/*  under the terms of the GNU Lesser General Public License as        */
/*  published by the Free Software Foundation; either version 2.1 of   */
/*  the License, or (at your option) any later version.                */
/*                                                                     */
/*  This software is distributed in the hope that it will be useful,   */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of     */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   */
/*  Lesser General Public License for more details.                    */
/*                                                                     */
/*  You should have received a copy of the GNU Lesser General Public   */
/*  License along with this software; if not, write to the Free        */
/*  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA */
/*  02110-1301 USA, or see the FSF site: http://www.fsf.org.           */
/* ------------------------------------------------------------------- */

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
   elem *nouv = (elem*)malloc(sizeof(elem));
   if(!nouv) exit(EXIT_FAILURE);
   strcpy(nouv->value,val);
   nouv->prev = l->last;
   nouv->next = NULL;
   if(l->last) l->last->next = nouv;
   else l->first = nouv;
   l->last = nouv;        
}
/******************************************************************************/
void addFront(listc *l, char *val)
{
   elem *nouv = (elem*)malloc(sizeof(elem));
   if(!nouv) exit(EXIT_FAILURE);
   strcpy(nouv->value,val);
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
