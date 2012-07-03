/* ------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                   */
/*  (C) 2012 by Hamid MEDJAHED (Prologue) <ijm667@hotmail.com>         */
/* --------------------------------------------------------------------*/
/*  This is free software; you can redistribute it and/or modify it    */
/*  under the terms of the GNU Lesser General PUBLIC License as        */
/*  published by the Free Software Foundation; either version 2.1 of   */
/*  the License, or (at your option) any later version.                */
/*                                                                     */
/*  This software is distributed in the hope that it will be useful,   */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of     */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   */
/*  Lesser General PUBLIC License for more details.                    */
/*                                                                     */
/*  You should have received a copy of the GNU Lesser General PUBLIC   */
/*  License along with this software; if not, write to the Free        */
/*  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA */
/*  02110-1301 USA, or see the FSF site: http://www.fsf.org.           */
/* ------------------------------------------------------------------- */
#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

typedef struct el
{
 char value[256];
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
