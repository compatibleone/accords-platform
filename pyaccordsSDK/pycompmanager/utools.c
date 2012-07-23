/* ----------------------------------------------------------------------------- */
/* ACCORDS Platform                                                        	 */
/* module to interface Accords platform  for Python                              */
/* Copyright 2012  Hamid MEDJAHED (hmedjahed@prologue.fr) Prologue               */
/* ----------------------------------------------------------------------------- */
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


#include "utools.h"


/*-----------------------------------------------------------------------------------------------------------*/
/* Function to inserte category include files in cords.h occibuilder.h and cordsbase.c                       */
/* pathf: char * the project directory name                                                                  */
/* categoryName: char * the name of the category                                                             */
/* return 1 if successfully inserted                                                                         */
/*-----------------------------------------------------------------------------------------------------------*/
int insertCategory(char pathf[], char categoryName[],int indice,int flag)
{
 char cordsh[TAILLE];
 char cordshname[TAILLE];
 char occibuilder[TAILLE];
 char occibuilderb[TAILLE];
 char occibuildername[TAILLE];
 char cordsbase[TAILLE];
 char cordsbasename[TAILLE];
 char occicords[TAILLE];
 char occicordsname[TAILLE];
 char pyint[TAILLE];
 char pyintname[TAILLE];
 char pyListcateg[TAILLE];
 char pyListcategname[TAILLE];
 char pylistactionname[TAILLE];
 char pylistaction[TAILLE];
 
 if(indice==1)
 {
  sprintf(pylistactionname,"#include \"%sAction.c\"",categoryName);
  sprintf(pylistaction,"%s/%s",pathf,PY_ACT_LIST);
 }

 sprintf(pyListcategname,"#include \"%s.h\"",categoryName);
 sprintf(pyListcateg,"%s/%s",pathf,LISTCATEG_FILE);

 sprintf(cordsh,"%s/%s",pathf,INCLUDE_CORDS_H);
 if(flag==0) sprintf(occibuilder,"%s/%s",pathf,INCLUDE_OCCI_BUILDER);
 sprintf(occibuilderb,"%s/%s",pathf,INCLUDE_OCCI_PROVIDER_BUILDER);
 
 sprintf(cordshname,"#include \"%s.h\"",categoryName);
 sprintf(occibuildername,"public struct occi_category * occi_cords_%s_builder(char * a,char *b);",categoryName);
 
 sprintf(cordsbase,"%s/%s",pathf,INCLUDE_CORDS_BASE);
 sprintf(cordsbasename,"#include \"%s.c\"",categoryName);

 sprintf(occicords,"%s/%s",pathf,OCCI_CORDS);
 sprintf(occicordsname,"#include \"occi%s.c\"",categoryName);
 
 sprintf(pyint,"%s/%s",pathf,PY_CRUD_INCLUDE);
 sprintf(pyintname,"#include \"%sInterface.c\"",categoryName);
 


 if(!flag)insertInFile(cordsh,cordshname,categoryName,0,flag);
 if(!flag)insertInFile(occibuilder,occibuildername,categoryName,1,flag);
 insertInFile(occibuilderb,occibuildername,categoryName,1,flag);

 insertInFile(cordsbase,cordsbasename,categoryName,0,flag);
 insertInFile(occicords,occicordsname,categoryName,0,flag);

 insertInFile(pyint,pyintname,categoryName,0,flag);

 insertInFile(pyListcateg,pyListcategname,categoryName,0,flag);

 if (indice) insertInFile(pylistaction,pylistactionname,categoryName,0,flag);


 return 1;
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function to insert a line in a file                                                                       */
/* pathf: char *  the path file name                                                                         */
/* categoryName: char * the name of the category                                                             */
/* indoice: int  to select a processing                                                                      */
/* retutn 1 if successfully done                                                                             */
/*-----------------------------------------------------------------------------------------------------------*/
int insertInFile(char pathf[],char categoryName[],char categoryNames[],int indice, int flag)
{
  int a=0;
  FILE *fIn;
  FILE *fOut;
  char line[TAILLE];
  char strcats[10];
  listc categoryN;
  

  if(indice)
  {
    resetList(&categoryN);
  }

  if((fIn=fopen(pathf,"r"))==NULL)
  {
   printf("Error inserInFile: No such file or directory\n");
   return 0;
  }

  if((fOut=fopen("text.tmp","w"))==NULL)
  {
   fclose(fIn);
   printf("Error inserInFile :No such file or directory\n");
   return 0;
  }

 
  while(fgets(line,sizeof(line), fIn))
  {
   line[strlen(line)]=0;
   if(searchWord(categoryNames,line)==1)
   {
    a=1;
    fprintf(fOut,"%s",line);
    if(indice==1)
    {
      if(line[0]=='p') 
      {
               addBack(&categoryN,getCategoryName(line,"_",3));
      }
    }
   }
   else
   {
      if(indice)
       {
         if(line[0]=='c' && line[1]=='o') break;
         else
         { 
           fprintf(fOut,"%s",line);
            if(line[0]=='p') 
            {
               addBack(&categoryN,getCategoryName(line,"_",3));
            }
          } 
   
       }
       else
       {
        str_sub(line,0,5,strcats);
        strcats[strlen(strcats)]=0;
        if((strcmp(strcats,"#endif"))==0) break;
        else fprintf(fOut,"%s",line);
       }
   }
   
  }  
  if(!a)
  {
   fprintf(fOut,"%s\n",categoryName);
   if(indice) addBack(&categoryN,getCategoryName(categoryName,"_",3));
  }
  if(indice)
  {
   fprintf(fOut,"const static struct {\n");
   fprintf(fOut,"\tconst char *name;\n");
   fprintf(fOut,"\tpublic struct occi_category * (*func)(char *a,char * b);\n");
   if (!flag) fprintf(fOut,"} occiCategoryBuilder_map2[]={\n");
   else  fprintf(fOut,"} occiCategoryBuilder_map[]={\n");


   elem *pelem = categoryN.first;
   while(pelem)
   {          
     fprintf(fOut,"\t{ \"%s\", occi_cords_%s_builder },\n",pelem->value,pelem->value); 
     pelem = pelem->next;
   }
   
   free(pelem);
   fprintf(fOut,"};\n");
  }
  
  fprintf(fOut,"#endif\n\n\n");
  fprintf(fOut,"\n");
  fclose(fIn);
  fclose(fOut);
  rename("text.tmp",pathf);
  return 1;
}


/*------------------------------------------------------------------------------------------------*/
/* Function to delete a line in a file                                                            */
/* pathf: char* file name                                                                         */
/* categoryName: char * name of the category                                                      */
/* return 1 if succeeded                                                                          */
/*------------------------------------------------------------------------------------------------*/
int deleteInFile(char pathf[],char categoryName[])
{
  FILE *fIn;
  FILE *fOut;
  char line[TAILLE];

  if((fIn=fopen(pathf,"r"))==NULL)
  {
   printf("Error deleteInFile: No such file or directory\n");
   return 0;
  }
  if((fOut=fopen("text.tmp","w"))==NULL)
  {
   fclose(fIn);
   printf("Error delteInFile :No such file or directory\n");
   return 0;
  }
 
  while(fgets(line,sizeof line, fIn))
  {
   if(searchWord(categoryName,line)!=1)
   {
    fprintf(fOut,"%s",line);
   }
  }
  fclose(fIn);
  fclose(fOut);
  rename("text.tmp",pathf);
  return 1;
}



/*--------------------------------------------------------------------------------------------------*/
/* Function to parse a string to get the category name                                              */
/* strCat: (char*) string to be parsed                                                              */
/* tok: (char*) delimeter for parssing                                                              */
/* p:(int) position of the name                                                                     */
/* return a string the categoryName                                                                 */
/*--------------------------------------------------------------------------------------------------*/
char *getCategoryName(char strCt[],char * tok,int p)
{
 char *token=NULL;
 int i=0;
 char *result=NULL;
 token= strtok(strCt,tok);
 for(; token != NULL ;)
 {  
      if (i==p) result=token;
      i++;
      token=strtok(NULL, tok);

 }
 return result;

}

/*------------------------------------------------------------------------------------------------*/
/* Function to search a word in a string                                                          */
/* mot: char * word name                                                                          */
/* line: char * the string                                                                        */
/* return 1 if the word exist otherwise 0                                                         */
/*------------------------------------------------------------------------------------------------*/
int searchWord(char *mot,char *line)
{
    char car = 0;
    long i = 0;
    int j=0;

    while(car != '\n')
    {
	car = line[j];
	if(mot[i] == car)	
	{
              i++;
	      if(mot[i] == 0 || line[j]=='\n') 
	      {
		 return 1; // if found , return 1 
	      }		
        }
        else
	{
	   i = 0;
	}
      j++;
     }

    return 0;	// if not found , return 0
}

/*-------------------------------------------------------------------------------------------------------*/
/* Function to get the number of line in a file                                                          */
/* pathf: (char*) path name of the file                                                                  */
/* return int the nubmer of line                                                                         */
/*-------------------------------------------------------------------------------------------------------*/
int getLineNumber(char pathf[])  
{ 
   FILE* fp = fopen (pathf, "r" );
   unsigned long count = 0;
   char line[1024];

   if (fp != NULL)
   {      
      while (fgets(line,1024,fp) != NULL)
      {
         if (line[0]=='p')
         {
            count++;
         }
      }
      fclose (fp), fp = NULL;
   }
   return count;
} 




/*------------------------------------------------------------------------------------------------------------------*/
/* Function to extract a substring from a string                                                                    */
/* s: (char*) string sourcs                                                                                         */
/* start: (int) the extracted string start                                                                          */
/* end: (int) the extracted string end                                                                              */
/* new_s: (char*) the extracted string                                                                              */
/*------------------------------------------------------------------------------------------------------------------*/
void str_sub(const char *s, unsigned int start, unsigned int end, char new_s[])
{
 if (s && (start < end))
 {
 /* (1)*/
 int i;
 /* (2)*/
  for (i = start; i <= end; i++)
  {
   /* (3)*/
   new_s[i-start] = s[i];
  }
 }
 new_s[end-start+1]=0;
}

/*--------------------------------------------------------------------------------------------------------------------*/
/* Function to concatenate two string str1, str2 with  a dilimeter d                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
void strConcat(char str1[],char str2[], char d)
{
    int i=0,j=0;
   
   
    while(str1[i]!='\0'){
         i++;
    }
    str1[i]=d;
    i++;
    while(str2[j]!='\0'){
         str1[i] = str2[j];   
         i++;
         j++;
    }
    
    str1[i] = '\0';
}
