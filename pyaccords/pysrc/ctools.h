#ifndef _CTOOLS_H_
#define _CTOOLS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void strConcat(char str1[], char str2[],char c);



#endif

