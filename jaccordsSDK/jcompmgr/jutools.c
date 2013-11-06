/* ----------------------------------------------------------------------------- */
/* ACCORDS Platform                                                        	 */
/* module to interface Accords platform  for Java                                */
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


#include "jutools.h"
#include "allocate.h"

/*-----------------------------------------------------------------------------------------------------------*/
/* Function to inserte category include files in cords.h occibuilder.h and cordsbase.c                       */
/* pathf: char * the project directory name                                                                  */
/* categoryName: char * the name of the category                                                             */
/* return 1 if successfully inserted                                                                         */
/*-----------------------------------------------------------------------------------------------------------*/
int jinsertCategory(char pathf[], char categoryName[],int indice,int flag)
{
 	char cordsh[DIM];
 	char cordshname[DIM];
 	char occibuilder[DIM];
	char occibuilderb[DIM];
	char occibuildername[DIM];
        char occibuildernameb[DIM];
	char cordsbase[DIM];
	char cordsbasename[DIM];
	char occicords[DIM];
	char occicordsname[DIM];
	char jint[DIM];
	char jintname[DIM];
	char jListcateg[DIM];
	char jListcategname[DIM];
	char jlistactionname[DIM];
	char jlistaction[DIM];
 
	if(indice)
	{
		sprintf(jlistactionname,"#include \"%sAction.c\"",categoryName);
		sprintf(jlistaction,"%s/%s",pathf,J_ACT_LIST);
	}

	sprintf(jListcategname,"#include \"%s.h\"",categoryName);
	sprintf(jListcateg,"%s/%s",pathf,JLISTCATEG_FILE);

	sprintf(cordsh,"%s/%s",pathf,INCLUDE_CORDS_H);
	if(!flag) sprintf(occibuilder,"%s/%s",pathf,INCLUDE_OCCI_BUILDER);
	sprintf(occibuilderb,"%s/%s",pathf,INCLUDE_OCCI_PROVIDER_BUILDER);

	sprintf(cordshname,"#include \"%s.h\"",categoryName);
	sprintf(occibuildername,"public struct occi_category * occi_cords_%s_builder(char * a,char *b);",categoryName);
        sprintf(occibuildernameb,"public struct occi_category * occi_cords_%s_builder(char * a,char *b);",categoryName);


	sprintf(cordsbase,"%s/%s",pathf,INCLUDE_CORDS_BASE);
	sprintf(cordsbasename,"#include \"%s.c\"",categoryName);

	sprintf(occicords,"%s/%s",pathf,OCCI_CORDS);
	sprintf(occicordsname,"#include \"occi%s.c\"",categoryName);

	sprintf(jint,"%s/%s",pathf,J_CRUD_INCLUDE);
	sprintf(jintname,"#include \"%sInterface.c\"",categoryName);
 
	if(indice) 
	{
		if(!(jinsertInFile(jlistaction,jlistactionname,categoryName)))
			return 0;
	}

	if(!flag)
	{
		if(!(jinsertInFile(cordsh,cordshname,categoryName)))
		 	return 0;
		else if(!(jinsertInFileOcciStruct(occibuilder,occibuildername,categoryName)))
		 	return 0;
	}
 
	if(!(jinsertInFileJStruct(occibuilderb,occibuildernameb,categoryName)))
		return 0;
	else if(!(jinsertInFile(cordsbase,cordsbasename,categoryName)))
		return 0;
	else if(!(jinsertInFile(occicords,occicordsname,categoryName)))
		return 0;
	else if(!(jinsertInFile(jint,jintname,categoryName)))
		return 0;
	else if(!(jinsertInFile(jListcateg,jListcategname,categoryName)))
		return 0;
	else
		return 1;
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function to insert a line in a file                                                                       */
/* pathf: char *  the path file name                                                                         */
/* categoryName: char * the name of the category                                                             */
/* retutn 1 if successfully done                                                                             */
/*-----------------------------------------------------------------------------------------------------------*/
int jinsertInFile(char pathf[],char categoryName[],char categoryNames[])
{
	int a=0;
	FILE *fIn;
	FILE *fOut;
	char line[DIM];
	char strcats[10];
	listc categoryN;
	char pathtmp[DIM];

	resetList(&categoryN);

	sprintf(pathtmp,"%s/jaccords/jsrc/myfile.tmp",JPATH);

	if((fIn=fopen(pathf,"r"))==NULL)
	{
		printf("Error inserInFile: No such file or directory\n");
			return 0;
	}

	if((fOut=fopen(pathtmp,"w"))==NULL)
	{
		fclose(fIn);
		printf("Error inserInFile :No such file or directory\n");
			return 0;
	}


	while(fgets(line,sizeof(line), fIn) != NULL)
	{
		line[strlen(line)]=0;
		if(jsearchWord(categoryNames,line)==1)
		{
			 a=1;
			 fprintf(fOut,"%s",line);
		}
		else
		{
			 jstr_sub(line,0,5,strcats);
			 strcats[strlen(strcats)]=0;
			 if(!(strcmp(strcats,"#endif"))) break;
			 else fprintf(fOut,"%s",line);
		}

	}  

	if(!a)
	{
		fprintf(fOut,"%s\n",categoryName);
	}

	fprintf(fOut,"#endif\n\n\n");
	fprintf(fOut,"\n");
	fclose(fIn);
	fclose(fOut);
	if( rename(pathtmp,pathf) < 0 )
		return 0;
	else 
		return 1;
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function to insert a line in a struct file                                                                */
/* pathf: char *  the path file name                                                                         */
/* categoryName: char * the name of the category                                                             */
/* retutn 1 if successfully done                                                                             */
/*-----------------------------------------------------------------------------------------------------------*/
int jinsertInFileOcciStruct(char pathf[],char categoryName[],char categoryNames[])
{
	int a=0;
	FILE *fIn;
	FILE *fOut;
	char line[DIM];
	char pathtmp[DIM];
	listc categoryN;
	char * ctemp;

	sprintf(pathtmp,"%s/jaccords/jsrc/occibuilder.tmp",JPATH);

	resetList(&categoryN);

	if((fIn=fopen(pathf,"r"))==NULL)
	{
		printf("Error inserInFile: No such file or directory\n");
		return 0;
	}

	if((fOut=fopen(pathtmp,"w"))==NULL)
	{
		fclose(fIn);
		printf("Error inserInFile :No such file or directory\n");
		return 0;
	}

	while(fgets(line,DIM, fIn) != NULL)
	{
		line[strlen(line)]=0;
		if(jsearchWord(categoryNames,line)==1)
		{
			  a=1;
			  fprintf(fOut,"%s",line);
			  if(line[0]=='p') 
			  {
			  		addBack(&categoryN,getCategoryName(line,"_",3));
			  } 
		}
		else
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
	} 

	if(!a)
	{
		fprintf(fOut,"%s\n",categoryName);
		ctemp = allocate_string(getCategoryName(categoryName,"_",3));
		if (ctemp)
			addBack(&categoryN,ctemp);
	}

	fprintf(fOut,"const static struct {\n");
	fprintf(fOut,"\tconst char *name;\n");
	fprintf(fOut,"\tpublic struct occi_category * (*func)(char *a,char * b);\n");
	fprintf(fOut,"} occiCategoryBuilder_map2[]={\n");
	elem *pelem = categoryN.first;
	while(pelem)
	{          
		fprintf(fOut,"\t{ \"%s\", occi_cords_%s_builder },\n",pelem->value,pelem->value); 
		pelem = pelem->next;
	}

	fprintf(fOut,"};\n\n");
	fprintf(fOut,"#endif\n");
	fclose(fIn);
	fclose(fOut);
	if( rename(pathtmp,pathf) < 0)
		return 0;
	else
		return 1;
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function to insert a line in a jstruct file                                                              */
/* pathf: char *  the path file name                                                                         */
/* categoryName: char * the name of the category                                                             */
/* retutn 1 if successfully done                                                                             */
/*-----------------------------------------------------------------------------------------------------------*/
int insertInFileJStruct(char pathf[],char categoryName[],char categoryNames[])
{
	int a=0;
	FILE *fIn;
	FILE *fOut;
	char line[DIM];
	char strcats[10];
	char pathtmp[DIM];
	listc categoryN;
	char * ctemp;

	sprintf(pathtmp,"%s/jaccords/jsrc/jlistoccibuilder.tmp",JPATH);

	resetList(&categoryN);

	if((fIn=fopen(pathf,"r"))==NULL)
	{
		printf("Error inserInFile: No such file or directory\n");
		return 0;
	}

	if((fOut=fopen(pathtmp,"w"))==NULL)
	{
		fclose(fIn);
		printf("Error inserInFile :No such file or directory\n");
		return 0;
	}

	while(fgets(line,sizeof(line), fIn) != NULL)
	{
		line[strlen(line)]=0;
		if(jsearchWord(categoryNames,line)==1)
		{
			 a=1;
			 fprintf(fOut,"%s",line);
			 if(line[0]=='p') 
			 {
			       		addBack(&categoryN,getCategoryName(line,"_",3));
			 }
		}
		else
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
	}

	if(!a)
	{
		fprintf(fOut,"%s\n",categoryName);
		ctemp = allocate_string(getCategoryName(categoryName,"_",3));
		if (ctemp)
		addBack(&categoryN,ctemp);
	}

	fprintf(fOut,"const static struct {\n");
	fprintf(fOut,"\tconst char *name;\n");
	fprintf(fOut,"\tpublic struct occi_category * (*func)(char *a,char * b);\n");
	fprintf(fOut,"} occiCategoryBuilder_map[]={\n");

	elem *pelem = categoryN.first;
	while(pelem)
	{          
		fprintf(fOut,"\t{ \"%s\", occi_cords_%s_builder },\n",pelem->value,pelem->value); 
		pelem = pelem->next;
	}

	fprintf(fOut,"};\n\n");
	fprintf(fOut,"#endif\n");
	fclose(fIn);
	fclose(fOut);
	if( rename(pathtmp,pathf) < 0)
		return 0;
	else
		return 1;
}

/*------------------------------------------------------------------------------------------------*/
/* Function to delete a line in a file                                                            */
/* pathf: char* file name                                                                         */
/* categoryName: char * name of the category                                                      */
/* return 1 if succeeded                                                                          */
/*------------------------------------------------------------------------------------------------*/
int jdeleteInFile(char pathf[],char categoryName[])
{
	FILE *fIn;
	FILE *fOut;
	char line[DIM];
	char pathtmp[DIM];

	sprintf(pathtmp,"%s/jaccords/jsrc/delfile.tmp",JPATH);
	if((fIn=fopen(pathf,"r"))==NULL)
	{
		printf("Error deleteInFile: No such file or directory\n");
		return 0;
	}
	if((fOut=fopen(pathtmp,"w"))==NULL)
	{
		fclose(fIn);
		printf("Error delteInFile :No such file or directory\n");
		return 0;
	}

	while(fgets(line,sizeof line, fIn))
	{
		if(jsearchWord(categoryName,line)!=1)
		{
			fprintf(fOut,"%s",line);
		}
	}
	fclose(fIn);
	fclose(fOut);
	if( rename(pathtmp,pathf) < 0)
		return 0;
	else
		return 1;
}



/*--------------------------------------------------------------------------------------------------*/
/* Function to parse a string to get the category name                                              */
/* strCat: (char*) string to be parsed                                                              */
/* tok: (char*) delimeter for parssing                                                              */
/* p:(int) position of the name                                                                     */
/* return a string the categoryName                                                                 */
/*--------------------------------------------------------------------------------------------------*/
char *jgetCategoryName(char strCt[],char * tok,int p)
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
int jsearchWord(char *mot,char *line)
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
int jgetLineNumber(char pathf[])  
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
void jstr_sub(const char *s, unsigned int start, unsigned int end, char new_s[])
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

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function to extract a substring from a string                                                                     */
/* s: (char*) string sourcs                                                                                          */
/* start: (int) the extracted string start                                                                           */
/* end: (int) the extracted string end                                                                               */
/*-------------------------------------------------------------------------------------------------------------------*/
char *jsub_str(const char *s, unsigned int start, unsigned int end)
{
	char *new_s=NULL;
	int i;
	if (s != NULL && (start < end))
	{
		/* (1)*/
		new_s = malloc (sizeof(*new_s) * (end-start + 2));
		if(new_s != NULL)
		{
			 /* (2)*/
			for (i = start; i <= end; i++)
			{
			 	/* (3)*/   new_s[i-start] = s[i];
			}
			new_s[i-start] = '\0';
		}
		else
		{
			printf("Error: memory on");
			exit(EXIT_FAILURE);
		}

	}
	return new_s;
}

/*--------------------------------------------------------------------------------------------------------------------*/
/* Function to concatenate two string str1, str2 with  a dilimeter d                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
void jstrConcat(char str1[],char str2[], char d)
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
