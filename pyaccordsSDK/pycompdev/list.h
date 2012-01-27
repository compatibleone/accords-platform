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
//char *retBack(listc *l);
//char *retFront(listc *l);
void liberateList(listc *l);

#endif
