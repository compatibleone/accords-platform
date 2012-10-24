/*-------------------------------------------------------------------------------*/
/* ACCORDS Platform                                                       	 */
/* module to generate a new gategory for Accords platform                        */
/* copyright 2012 ,Hamid MEDJAHE    (hmedjahed@prologue.fr)    Prologue          */
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

#include "compmgr.h"

/*----------------------------------------------------------------------------------*/
/*               Function to remove a component                                     */
/*----------------------------------------------------------------------------------*/
int deleteJModule(char moduleName[])
{
	char pathff[DIM];
	char pathf[1024];
	strcpy(pathf,JPATH);
	sprintf(pathff,"rm -r %s/%s",pathf,moduleName);
	if (system(pathff)!= 0 )
 	{
		printf( "Error in delete module:No such component name\n" );
		return 0;
	}
	return 1;
}

/*-----------------------------------------------------------------------------------*/
/* Function to delete a category from accords platform                               */
/* pathf: char * path of the directory project                                       */
/* categoryName: char * the name of the category                                     */
/* return 1 if succeeded                                                             */
/*-----------------------------------------------------------------------------------*/
int deleteJCategory(char categoryName[],int indice,int flag)
{
	char cordsh[DIM];
	char cordshname[DIM];
	char occibuilder[DIM];
	char occibuilderb[DIM];
	char occibuildername[DIM];
	char cordsbase[DIM];
	char cordsbasename[DIM];
	char occicords[DIM];
	char occicordsname[DIM];
	char pathfc[DIM];
	char pathfh[DIM];
	char occipath[DIM];
	char jinc[DIM];
	char jincname[DIM];
	char pathjc[DIM];
	char pathjp[DIM];
	char pathjcl[DIM];
	char pathji[DIM];
	char pathjstruct[DIM];
	char pathjstructname[DIM];
	char pathactc[DIM];
	char pathactj[DIM];
	char pathactji[DIM];
	char jListcateg[DIM];
	char jListcategname[DIM];
	char pathactcname[DIM];
	char pathactclist[DIM];
	char pathactstructname[DIM];
	char pathactstruct[DIM];
	char pathactnstructname[DIM];
	char pathactnstruct[DIM];
	char pathactbstructname[DIM];
	char pathactbstruct[DIM];
	char pathf[DIM];
        char pathaccess[DIM];

	strcpy(pathf,JPATH);
 	sprintf(pathactcname,"%sAction.c",categoryName);
 	sprintf(pathactclist,"%s/%s",pathf,J_ACT_LIST);
        
        sprintf(pathaccess,"%s/%s",pathf, J_CATEGACCESS_STRUCT);

 	sprintf(pathactstructname,"%s_",categoryName);
 	sprintf(pathactstruct,"%s/%s",pathf,J_ACT_STRUCT);
 
 	sprintf(pathactnstructname,"%s_",categoryName);
 	sprintf(pathactnstruct,"%s/%s",pathf,J_ACT_NAME_STRUCT);

 	sprintf(pathactbstructname,"%s_",categoryName);
 	sprintf(pathactbstruct,"%s/%s",pathf,J_ACT_NUMBER_STRUCT);

 	sprintf(jListcategname,"%s.h",categoryName);
 	sprintf(jListcateg,"%s/%s",pathf,JLISTCATEG_FILE);

 
 	sprintf(pathjc,"%s/%s/%s/%sInterface.c",pathf,JACCORDS,JACCORDSS,categoryName);
 	sprintf(pathjcl,"%s/%s/%s/J%sBase.java",pathf,JACCORDS,JACCORDSS,categoryName);
 	sprintf(pathjp,"%s/%s/J%sInterface.java",pathf,JACCORDS,categoryName);
 	sprintf(pathji,"%s/%s/%s/J%sInt.java",pathf,JACCORDS,JACCORDSS,categoryName);

 	sprintf(pathactc,"%s/%s/%s/%sAction.c",pathf,JACCORDS,JACCORDSS,categoryName);
 	sprintf(pathactj,"%s/%s/J%sAction.java",pathf,JACCORDS,categoryName);
 	sprintf(pathactji,"%s/%s/%s/J%sAct.java",pathf,JACCORDS,JACCORDSS,categoryName);

 	sprintf(jincname,"%sInterface.c",categoryName);
 	sprintf(jinc,"%s/%s",pathf,J_CRUD_INCLUDE);

 	sprintf(pathjstructname,"%s",categoryName);
 	sprintf(pathjstruct,"%s/%s",pathf,J_CRUD_STRUCT);

 
 	sprintf(pathfh,"%s/%s/%s.h",pathf,CORDS_SRC,categoryName);
 	sprintf(pathfc,"%s/%s/%s.c",pathf,CORDS_SRC,categoryName);
 	sprintf(occipath,"%s/%s/occi%s.c",pathf,OCCI_PATH,categoryName);
 
 	sprintf(cordsh,"%s/%s",pathf,INCLUDE_CORDS_H);
 	sprintf(occibuilder,"%s/%s",pathf,INCLUDE_OCCI_BUILDER);
 	sprintf(occibuilderb,"%s/%s",pathf,INCLUDE_OCCI_PROVIDER_BUILDER);
 
 	sprintf(cordshname,"%s.h",categoryName);
 	sprintf(occibuildername,"_%s_",categoryName);
 
 	sprintf(cordsbase,"%s/%s",pathf,INCLUDE_CORDS_BASE);
 	sprintf(cordsbasename,"%s.c",categoryName);

 	sprintf(occicords,"%s/%s",pathf,OCCI_CORDS);
 	sprintf(occicordsname,"occi%s.c",categoryName);
 
	 if( remove( pathfh) != 0 )
 	{
   		printf( "Error in delete category (CORDS_SRC H):No such category name\n" );
   		return 0;
 	}
 	else if( remove( pathfc) != 0 )
 	{
  		printf( "Error in delete category (CORDS_SRC C):No such category name\n" );
  		return 0;
 	}
 	else if( remove( occipath) != 0 )
	{
  		printf( "Error in delete category (OCCI_PATH):No such category name\n" );
  		return 0;
 	}
 	else if( remove( pathjc) != 0 )
 	{
   		printf( "Error in delete category (INTERFACE C):No such category name\n" );
   		return 0;
 	}
 	else if( remove( pathji) != 0 )
 	{
  		printf( "Error in delete category (CATEGORY PY):No such category name\n" );
  		return 0;
 	}
 	else if( remove( pathjp) != 0 )
 	{
  		printf( "Error in delete category (INTERFACE PY):No such category name\n" );
  		return 0;
 	}
 	else if( remove( pathjcl) != 0 )
 	{
  		printf( "Error in delete category( CLASS PY):No such category name\n" );
  		return 0;
 	}
 
	if(indice)
 	{
  		if( remove( pathactc) != 0 )
  		{
   			printf( "Error in delete category( ACTION C):No such file name\n" );
   			return 0;
  		}
  		else if( remove( pathactji) != 0 )
  		{
   			printf( "Error in delete category( ACTION PYI):No such file name\n" );
   			return 0;
  		}
  		else if( remove( pathactj) != 0 )
  		{
   			printf( "Error in delete category( ACTION PY):No such file name\n" );
   			return 0;
  		}
  
 		if(!(deleteInFile(pathactclist,pathactcname)))
     			return 0; 
  		else if(!(deleteInFile(pathactstruct,pathactstructname)))
     			return 0; 
  		else if(!(deleteInFile(pathactnstruct,pathactnstructname)))
     			return 0;
  		else if(!(deleteInFile(pathactbstruct,pathactbstructname)))
     			return 0;  
 	}

 	if(!flag)
 	{
  		if(!(deleteInFile(cordsh,cordshname)))
     			return 0;
  		else if(!(deleteInFile(occibuilder,occibuildername)))
     			return 0;
 	}

 	if(!(deleteInFile(cordsbase,cordsbasename)))
    		return 0;
 	else if(!(deleteInFile(occicords,occicordsname)))
    		return 0;
 	else if(!(deleteInFile(jinc,jincname)))
    		return 0;
 	else if(!(deleteInFile(pathjstruct,pathjstructname)))
    		return 0;
 	else if(!(deleteInFile(jListcateg,jListcategname)))
    		return 0; 
 	else if(!(deleteInFile(occibuilderb,occibuildername)))
    		return 0;
        else if(!(deleteInFile(pathaccess, categoryName)))
		return 0;
 	else
    		return 1;
}


/*-------------------------------------------------------------------------------------------------------*/
/* Fucntion to generate a category in accords paltform                                                   */
/* categoryName: (char*) the name of the category                                                        */
/* categoryAttributes: (char*) the list of the attribute as a string delimited by a comma (,)            */
/* categoryAccess: (char*) the access type of the category                                               */
/* pathf: (char*) a path name for the directory project                                                  */
/* return 1 if succeeded                                                                                 */
/*-------------------------------------------------------------------------------------------------------*/
int generateJAccordsCategory(
	char *categoryName,
	char *categoryAttributes, 
	char *categoryActions, 
	char * categoryAccess, 
	int flag)
{
	FILE *f;
	char *token=NULL;
	int dim=0;
	char pathff[DIM];
	char pathact[DIM];
	char pathactname[DIM];
	char pathactnumber[DIM];
        char pathcategaccess[DIM];
	listc categoryAtr;
	listc categoryAtrB;
	listc categoryAct;
	char occipath[DIM];
	char categoryAttributesB[1024]="id";
	int indice = 0;
        int indiceA = 0;
	char pathf[1024];
 	strcpy(pathf,JPATH);
	sprintf(pathff,"%s/%s/%s.h",pathf,CORDS_SRC,categoryName);
	sprintf(occipath,"%s/%s/occi%s.c",pathf,OCCI_PATH,categoryName);

 	if(categoryActions[0] != '\0') indice = 1;
        if(categoryAccess[0] != '\0') indiceA = 1;
	/*---------------------------------*/
	/*create category.h file           */
	/*---------------------------------*/
	if((f=fopen(pathff,"w"))==NULL)
	{
		printf("Error create category file H: No such file or directory: %s\n",pathff);
		return 0;
	}
	else
 	{
    		if(!flag)
    		{
       			fprintf(f,"#ifndef _cords_%s_h_\n",categoryName);
       			fprintf(f,"#define _cords_%s_h_\n",categoryName);
       			fprintf(f,"\n");
       			fprintf(f,"struct cords_%s\n",categoryName);
    		}
    		else
    		{
       			fprintf(f,"#ifndef _%s_h_\n",categoryName);
       			fprintf(f,"#define _%s_h_\n",categoryName);
       			fprintf(f,"\n");
       			fprintf(f,"struct %s\n",categoryName);  
    		}
    		fprintf(f,"{\n");
    		resetList(&categoryAtr);
    		strConcat(categoryAttributesB,categoryAttributes,' ');
    		token= strtok(categoryAttributesB," ");
    		for(; token != NULL ;)
    		{
      			fprintf(f,"\tchar * %s;\n",token);
      			dim++;
      			addBack(&categoryAtr,token);
      			token=strtok(NULL, " ");
    		}
    		fprintf(f,"};\n");
    		fprintf(f,"\n");
    		fprintf(f,"#endif");
    		fclose(f);
 	}   
  	/*-----------------------------------------*/
  	/* for generating category interface files */
  	/*-----------------------------------------*/
  	resetList(&categoryAtrB);
  	token= strtok(categoryAttributes," ");
  	for(;token != NULL;)
  	{
       		addBack(&categoryAtrB,token);
       		token=strtok(NULL," ");
  	}
    
  	/*----------------------------------------*/
  	/* for generating category actions files  */
  	/*----------------------------------------*/
  	if(indice)
  	{
     		resetList(&categoryAct);
     		token= strtok(categoryActions," ");
     		for(;token != NULL;)
     		{
        		addBack(&categoryAct,token);
        		token=strtok(NULL," ");
     		}
     
     		sprintf(pathact,"%s/%s",JPATH,J_ACT_STRUCT);
     		sprintf(pathactname,"%s/%s",JPATH,J_ACT_NAME_STRUCT);
     		sprintf(pathactnumber,"%s/%s",JPATH,J_ACT_NUMBER_STRUCT);

     		if(!(generateJCategoryActionCfile(categoryName,categoryAtrB,categoryAct,flag,pathf)))
         		return 0;
     		else if(!(generateCategoryActionJfile(categoryName,categoryAtrB,categoryAct,pathf)))
         		return 0;
     		else if(!(generateJCategoryActionStruct(categoryName,categoryAct,0,pathact)))
         		return 0;
     		else if(!(generateJCategoryActionStruct(categoryName,categoryAct,1,pathactname)))
         		return 0;
     		else if(!(generateJCategoryActionStruct(categoryName,categoryAct,2,pathactnumber)))
         		return 0;
  	}
        
        if(indiceA)
        {
           sprintf(pathcategaccess,"%s/%s",JPATH, J_CATEGACCESS_STRUCT);
           if(!generateJCategoryTypeStruct(categoryName,categoryAccess, pathcategaccess))
           	return 0;          
        }
      
        
  	if(!(createJCategoryCordsCfile(categoryName,categoryAtr,dim,flag,pathff)))         // create category.c file 
      		return 0;
  	else if(!(createJCategoryOcciFile(categoryName,categoryAtr,dim,flag,occipath)))    // create occicategory.c file 
      		return 0; 
  	else if(!(insertCategory(pathf,categoryName,indice,flag)))                         // inserte include files  
      		return 0;
  	else if(!(generateCategoryJSourcefile(categoryName,categoryAtrB,pathf)))           // generate java files   
      		return 0;
	else if(!(generateJCategoryInterfaceCfile(categoryName,categoryAtrB,flag,pathf)))  // generate C files       
      		return 0;
  	else if(!(generateJCategoryInterfceStructFile(pathf)))                             // genearte struct c files  
      		return 0;
  	else  
        
      		return dim;
 
}

int generateJCategoryTypeStruct(char *categoryName, char * categoryAccess, char pathf[])
{
	FILE *fIn;
	FILE *fOut;
  	int a=0;
  	char line[256];
  	char pathtmp[1024];
  	char strcats[20];

  	sprintf(pathtmp,"%s/jaccords/jsrc/categaccess.tmp",JPATH);
  
  	if((fIn=fopen(pathf,"r"))==NULL)
  	{
   		printf("Error in generate category Action struct file: No such file or directory\n");
   		return 0;
  	}
  	if((fOut=fopen(pathtmp,"w")) == NULL)
  	{
   		fclose(fIn);
   		printf("Error in generate category Action struct file tmp: No such file or directory\n");
   		return 0;
  	}
 
  	while( fgets(line,256, fIn) != NULL)
  	{
    		if(searchWord(categoryName,line))
    		{
       			a=1;
       			fprintf(fOut,"%s",line);
    		}
    		else
    		{
       			str_sub(line,0,1,strcats);
       			if(!(strcmp(strcats,"};"))) 
       			{
          			break;
       			}
       			else 
       			{
         			fprintf(fOut,"%s",line);
       			}
    		}
  	}
  
  	if(!a)
  	{
		fprintf(fOut,"\t{\"%s\", %s},\n",categoryName,categoryAccess);
 	}
  
  	fprintf(fOut,"};\n");
  	fprintf(fOut,"#endif\n");
  	fclose(fIn);
  	fclose(fOut);
  	if(rename(pathtmp,pathf) < 0)
     	return 0;
  	else
     	return 1;
}


int generateJCategoryActionStruct(char *categoryName, listc categoryAct, int n, char pathf[])
{
	FILE *fIn;
	FILE *fOut;
  	int a=0;
  	char line[256];
  	char pathtmp[1024];
  	char strcats[20];

  	sprintf(pathtmp,"%s/jaccords/jsrc/listaction.tmp",JPATH);
  
  	if((fIn=fopen(pathf,"r"))==NULL)
  	{
   		printf("Error in generate category Action struct file: No such file or directory\n");
   		return 0;
  	}
  	if((fOut=fopen(pathtmp,"w")) == NULL)
  	{
   		fclose(fIn);
   		printf("Error in generate category Action struct file tmp: No such file or directory\n");
   		return 0;
  	}
 
  	while( fgets(line,256, fIn) != NULL)
  	{
    		if(searchWord(categoryName,line))
    		{
       			a=1;
       			fprintf(fOut,"%s",line);
    		}
    		else
    		{
       			str_sub(line,0,1,strcats);
       			if(!(strcmp(strcats,"};"))) 
       			{
          			break;
       			}
       			else 
       			{
         			fprintf(fOut,"%s",line);
       			}
    		}
  	}
  
  	if(!a)
  	{
    		elem *pelem=categoryAct.first;
    		switch(n)
    		{
         		case 0:
               			while(pelem)
	      			{
		 			fprintf(fOut,"\t{\"%s_%s\", %s_%s },\n",categoryName,pelem->value,pelem->value,categoryName); 
		 			pelem = pelem->next;
	      			}
	     			 break;
         		case 1:
              			fprintf(fOut,"\t{\"%s\", %s_getname },\n",categoryName,categoryName);
              			break;
         		case 2:
              			fprintf(fOut,"\t{\"%s\", %s_getnumber },\n",categoryName,categoryName);
              			break;
    		}
 	}
  
  	fprintf(fOut,"};\n");
  	fprintf(fOut,"#endif\n");
  	fclose(fIn);
  	fclose(fOut);
  	if(rename(pathtmp,pathf) < 0)
     	return 0;
  	else
     	return 1;
}

/*------------------------------------------------------------------------------------------------*/
/* Function to generate category Actions C file                                                   */
/*------------------------------------------------------------------------------------------------*/
int generateJCategoryActionCfile(
	char *categoryName,
	listc categoryAtr,
	listc categoryAct, 
	int flag, 
	char pathf[])
{
	FILE * f;
 	char name[1024];
 	int i=0;

 	sprintf(name,"%s/%s/%s/%sAction.c",pathf,JACCORDS,JACCORDSS,categoryName);
	if((f=fopen(name,"w"))==NULL)
 	{
   		printf("Error generateCategoryActionsCfile: No such file or directory\n");
   		return 0;
 	}
 	else
 	{
    		fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
   		fprintf(f,"/* ACCORDS Platform                                                              */\n"); 
    		fprintf(f,"/* copyright 2012, Hamid MEDJAHE (hmedjahed@prologue.fr)    Prologue             */\n");
    		fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
    		fprintf(f,"/* Licensed under the Apache License, Version 2.0 (the \"License\");               */\n");
    		fprintf(f,"/* you may not use this file except in compliance with the License.              */\n");
    		fprintf(f,"/* You may obtain a copy of the License at                                       */\n");
    		fprintf(f,"/*                                                                               */\n");
    		fprintf(f,"/*       http://www.apache.org/licenses/LICENSE-2.0                              */\n");
    		fprintf(f,"/*                                                                               */\n");
    		fprintf(f,"/* Unless required by applicable law or agreed to in writing, software           */\n");
    		fprintf(f,"/* distributed under the License is distributed on an \"AS IS\" BASIS,             */\n");
    		fprintf(f,"/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.      */\n");
    		fprintf(f,"/* See the License for the specific language governing permissions and           */\n");
    		fprintf(f,"/* limitations under the License.                                                */\n");
    		fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
                fprintf(f,"\n\n/* jaccordsSDK: This code has been generated by jaccordsSDK                      */\n"); 
                fprintf(f,"#ifndef _%saction_c\n",categoryName);
		fprintf(f,"#define _%saction_c\n\n",categoryName);
    		fprintf(f,"#include \"../../occi/src/occi.h\"\n");
                fprintf(f,"#include \"jlistcateg.h\"\n",categoryName);
    		fprintf(f,"#include \"jctools.h\"\n");
    		fprintf(f,"#include \"jtools.h\"\n\n");
  
    		elem *pelem = categoryAct.first;
    		while (pelem)
    		{  
       			fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
       			fprintf(f,"/*           %s %s action    */\n",categoryName,pelem->value);
       			fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
       			fprintf(f,"struct rest_response * %s_%s(\n",pelem->value,categoryName);
       			fprintf(f,"\tstruct occi_category * optr,\n"); 
       			fprintf(f,"\tstruct rest_client * cptr,\n"); 
       			fprintf(f,"\tstruct rest_request * rptr,\n"); 
       			fprintf(f,"\tstruct rest_response * aptr,\n"); 
       			fprintf(f,"\tvoid * vptr )\n");
       			fprintf(f,"{\n");
       			if(!flag) fprintf(f,"\tstruct cords_%s * pptr;\n",categoryName);
       			else fprintf(f,"\tstruct %s * pptr;\n",categoryName);
       			fprintf(f,"\tchar sendstr[1024]=\" \";\n");
       			fprintf(f,"\tchar strtmp[1024]=\" \";\n");
       			fprintf(f,"\tint status;\n");
       			fprintf(f,"\tchar message[1024];\n");
       			fprintf(f,"\tchar srcdir[1024];\n");
       			fprintf(f,"\tchar * response = NULL;\n");
       			fprintf(f,"\tchar * token;\n");
       			fprintf(f,"\tlistcc restResponse;\n");
       			fprintf(f,"\tJNIEnv *env;\n");
                        fprintf(f,"\tjclass cls;\n");
			fprintf(f,"\tjmethodID mid;\n");
			fprintf(f,"\tjobject retval;\n");
			fprintf(f,"\tjstring jmessage;\n");
			fprintf(f,"\tchar * retString;\n");

       			fprintf(f,"\tif (!( pptr = vptr ))\n");
       			fprintf(f,"\t\treturn( rest_html_response( aptr, 404, \"Invalid Action\" ) );\n");
       			fprintf(f,"\telse\n");
 			fprintf(f,"\t{\n");
       			elem *pelemm = categoryAtr.first;
       			fprintf(f,"\t\tif(!(strValid(pptr->%s))) strcpy(sendstr,\" \");\n",pelemm->value);
       			fprintf(f,"\t\telse strcpy(sendstr,pptr->%s);\n",pelemm->value);
       			pelemm=pelemm->next;
       			while(pelemm)
       			{
        			fprintf(f,"\t\tif(!(strValid(pptr->%s))){\n",pelemm->value);
        			fprintf(f,"\t\t\tstrcpy(strtmp,\" \");\n");
        			fprintf(f,"\t\t\tstrConcat(sendstr,strtmp,',');\n");
        			fprintf(f,"\t\t}\n");
        			fprintf(f,"\t\telse strConcat(sendstr,pptr->%s,',');\n",pelemm->value);
        			pelemm=pelemm->next;
       			}
       			fprintf(f,"\t\t//           JNI interface\n");
		        fprintf(f,"\t\tenv = Getenv();\n");
			fprintf(f,"\t\tif(env)\n");
			fprintf(f,"\t\t{\n");
			fprintf(f,"\t\t\tcls = (*env)->FindClass(env,\"J%sAct\");\n",categoryName);
			fprintf(f,"\t\t\tif(cls != 0)\n");		
			fprintf(f,"\t\t\t{\n");
			fprintf(f,"\t\t\t\tmid = (*env)->GetStaticMethodID(env,cls,\"%s\",\"(Ljava/lang/String;)Ljava/lang/String;\");\n",pelem->value);      
			fprintf(f,"\t\t\t\tif(mid != 0)\n");
			fprintf(f,"\t\t\t\t{\n");
			fprintf(f,"\t\t\t\t\tjmessage = (jstring)(*env)->NewStringUTF(env,(const char *) sendstr);\n");
			fprintf(f,"\t\t\t\t\tretval = (*env)->CallStaticObjectMethod(env, cls, mid, jmessage);\n");
			fprintf(f,"\t\t\t\t\tretString = (char*) (*env)->GetStringUTFChars(env,retval, NULL);\n");       			
       			fprintf(f,"\t\t\t\t\tresetListe(&restResponse);\n");
       			fprintf(f,"\t\t\t\t\ttoken= strtok(retString,\",\");\n");
       			fprintf(f,"\t\t\t\t\tfor(; token != NULL ;)\n");
       			fprintf(f,"\t\t\t\t\t{\n");
                        fprintf(f,"\t\t\t\t\t\taddBacke(&restResponse,token);\n");
       			fprintf(f,"\t\t\t\t\t\ttoken=strtok(NULL, \",\");\n");
       			fprintf(f,"\t\t\t\t\t}\n");
       			fprintf(f,"\t\t\t\t\telemm *pelem = restResponse.first;\n");
       			fprintf(f,"\t\t\t\t\tif(pelem){\n");
       			fprintf(f,"\t\t\t\t\t\tstatus = atoi(pelem->value);\n");
       			fprintf(f,"\t\t\t\t\t\tpelem = pelem->next;\n");
       			fprintf(f,"\t\t\t\t\t}\n");
       			fprintf(f,"\t\t\t\t\tif(pelem){\n");
       			fprintf(f,"\t\t\t\t\t\tstrcpy(message, pelem->value);\n");
       			fprintf(f,"\t\t\t\t\t\tpelem = pelem->next;\n");
       			fprintf(f,"\t\t\t\t\t}\n");
                        fprintf(f,"\t\t\t\t\t(*env)->ReleaseStringUTFChars(env, retval, retString);\n");
			fprintf(f,"\t\t\t\t\tDetachenv();\n");
       			fprintf(f,"\t\t\t\t\treturn( rest_html_response( aptr, status, message ) );\n");
       			fprintf(f,"\t\t\t\t}\n");
			fprintf(f,"\t\t\t\telse\n");
			fprintf(f,"\t\t\t\t{\n");
			fprintf(f,"\t\t\t\t\tprintf(\"Error in J%sAct.class: GetMethodID returned 0.\\n\");\n",categoryName);
			fprintf(f,"\t\t\t\t}\n");
			fprintf(f,"\t\t\t}\n");
			fprintf(f,"\t\t\telse\n");
			fprintf(f,"\t\t\t{\n");
			fprintf(f,"\t\t\t\tprintf(\"Error in J%sAct.class: FindClass function returned 0.\\n\");\n",categoryName);
			fprintf(f,"\t\t\t}\n");
			fprintf(f,"\t\t}\n");
                        fprintf(f,"\t\telse\n");
			fprintf(f,"\t\t{\n");
			fprintf(f,"\t\t\tprintf(\"Error in J%sAct : creating JVM failed.\\n\");\n",categoryName);
			fprintf(f,"\t\t}\n");
                        fprintf(f,"\t}\n");
       			fprintf(f,"}\n\n");
       			pelem=pelem->next;
    		}
    
    		fprintf(f,"char *%s_getname(int a)\n",categoryName);
    		fprintf(f,"{\n");
    		fprintf(f,"\tstatic char action[256];\n");
    		fprintf(f,"\tswitch(a)\n");
    		fprintf(f,"\t{\n");
    		elem *pelemn = categoryAct.first;
    		while(pelemn)
    		{
     			fprintf(f,"\t\tcase %d:\n",i);
     			fprintf(f,"\t\t\tstrcpy(action,\"%s\");\n",pelemn->value);
     			fprintf(f,"\t\t\tbreak;\n");
     			pelemn=pelemn->next;
     			i++;
    		}
    		fprintf(f,"\t\tdefault:\n");
    		fprintf(f,"\t\t\tstrcpy(action,\"%s\");\n",categoryName);
    		fprintf(f,"\t\t\tbreak;\n");
    		fprintf(f,"\t}\n");
    		fprintf(f,"\treturn action;\n");
    		fprintf(f,"}\n\n");
    
    		fprintf(f,"int %s_getnumber()\n",categoryName);
    		fprintf(f,"{\n");
    		fprintf(f,"\treturn %d;\n",i);
    		fprintf(f,"}\n\n");
		fprintf(f,"#endif\n");
    		fclose(f);
  	}
 	return 1;
}

/*-------------------------------------------------------------------------------------------------------*/
/* function to generate java category Actions source file                                                */
/*-------------------------------------------------------------------------------------------------------*/
int generateCategoryActionJfile(char *categoryName,listc categoryAtr,listc categoryAct,char pathf[])
{
	FILE * f;
	char name1[1024];
	char name2[1024];
	int j,i=0;
	char listAtr[1024]="( tokens[0]";
	char lista[1024];
	char listResult[1024];
	char listr[1024];
	char strm[1024];

	sprintf(name1,"%s/%s/%s/J%sAct.java",pathf,JACCORDS,JACCORDSS,categoryName);
 	sprintf(name2,"%s/%s/J%sAction.java",pathf,JACCORDS,categoryName);
 
  	elem *pelemm=categoryAtr.first;
  	i=0;
  	while(pelemm)
  	{
		pelemm=pelemm->next;
        	i++;
  	}

 	if((f=fopen(name1,"w"))==NULL)
  	{
   		printf("Error generateCategoryActJfile: No such file or directory\n");
   		return 0;
  	}
  	else
  	{
   		fprintf(f,"/*----------------------------------------------------------------------------*/\n");
                fprintf(f,"/* Accords Platform                                                           */\n");
   		fprintf(f,"/* copyright 2012, Hamid MEDJAHED (hmedjahed@prologue.fr) Prologue            */\n");
                fprintf(f,"/*----------------------------------------------------------------------------*/\n");
   		fprintf(f,"/* Licensed under the Apache License, Version 2.0 (the \"License\");            */\n");
   		fprintf(f,"/* you may not use this file except in compliance with the License.           */\n");
   		fprintf(f,"/* You may obtain a copy of the License at                                    */\n");
   		fprintf(f,"/*                                                                            */\n");
   		fprintf(f,"/*       http://www.apache.org/licenses/LICENSE-2.0                           */\n");
   		fprintf(f,"/*                                                                            */\n");
   		fprintf(f,"/* Unless required by applicable law or agreed to in writing, software        */\n");
   		fprintf(f,"/* distributed under the License is distributed on an \"AS IS\" BASIS,          */\n");
   		fprintf(f,"/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   */\n");
   		fprintf(f,"/* See the License for the specific language governing permissions and        */\n");
   		fprintf(f,"/* limitations under the License.                                             */\n");
   		fprintf(f,"/*----------------------------------------------------------------------------*/\n");
                fprintf(f,"\n\n/* jaccordsSDK: This code has been generated by jaccordsSDK                   */\n"); 

   		fprintf(f,"public class J%sAct{\n",categoryName);
		fprintf(f,"\tstatic J%sBase objA;\n",categoryName);
		fprintf(f,"\tstatic J%sBase objB;\n",categoryName);
		
		for(j=1;j<i;j++)
   		{
    			sprintf(lista,"tokens[%d]",j);
    			strConcat(listAtr,lista,',');
   		}
   		strConcat(listAtr,")",' ');

                elem *pelemmm=categoryAtr.first;
   		sprintf(listResult,"objB.%s+\",\"",pelemmm->value);
   		pelemmm = pelemmm->next;
   		for(j=1;j<i-1;j++)
   		{
     			sprintf(listr,"objB.%s+\",\"",pelemmm->value);
     			strConcat(listResult,listr,'+');
     			pelemmm=pelemmm->next;
   		}	
   		sprintf(listr,"objB.%s",pelemmm->value);
                strConcat(listResult,listr,'+');

                elem *pelemact=categoryAct.first;
           	while(pelemact)
   		{
      			fprintf(f,"\tpublic static String %s(String str){\n",pelemact->value);
			fprintf(f,"\t\tString dlim = \"[,]+\";\n");
     			fprintf(f,"\t\tString [] tokens = str.split(dlim);\n");
     			fprintf(f,"\t\tobjA = new J%sBase%s;\n",categoryName,listAtr);
     			fprintf(f,"\t\tStringBuilder strb = new StringBuilder(\"200,ok\");\n");
			fprintf(f,"\t\tobjB = J%sAction.%s_%s(objA,strb);\n",categoryName,categoryName,pelemact->value);
                        fprintf(f,"\t\tString stra = strb.toString();\n");
			//fprintf(f,"\t\tString stresponse=%s+stra;\n",listResult);
     			//fprintf(f,"\t\treturn stresponse;\n\n");
                        fprintf(f,"\t\treturn stra;\n");
                        fprintf(f,"\t}\n\n");
     			pelemact=pelemact->next;
   		}
		fprintf(f,"}\n");
   		fclose(f);
  	}
 
  	if((f=fopen(name2,"w"))==NULL)
  	{
   		printf("Error generateCategoryActionJfile: No such file or directory\n");
   		return 0;
  	}
  	else
  	{
		fprintf(f,"/*----------------------------------------------------------------------------*/\n");
                fprintf(f,"/* Accords Platform                                                           */\n");
   		fprintf(f,"/* copyright 2012, Hamid MEDJAHED (hmedjahed@prologue.fr) Prologue            */\n");
                fprintf(f,"/*----------------------------------------------------------------------------*/\n");
   		fprintf(f,"/* Licensed under the Apache License, Version 2.0 (the \"License\");            */\n");
   		fprintf(f,"/* you may not use this file except in compliance with the License.           */\n");
   		fprintf(f,"/* You may obtain a copy of the License at                                    */\n");
   		fprintf(f,"/*                                                                            */\n");
   		fprintf(f,"/*       http://www.apache.org/licenses/LICENSE-2.0                           */\n");
   		fprintf(f,"/*                                                                            */\n");
   		fprintf(f,"/* Unless required by applicable law or agreed to in writing, software        */\n");
   		fprintf(f,"/* distributed under the License is distributed on an \"AS IS\" BASIS,          */\n");
   		fprintf(f,"/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   */\n");
   		fprintf(f,"/* See the License for the specific language governing permissions and        */\n");
   		fprintf(f,"/* limitations under the License.                                             */\n");
   		fprintf(f,"/*----------------------------------------------------------------------------*/\n");
                fprintf(f,"\n\n/* jaccordsSDK: This code has been generated by jaccordsSDK                   */\n"); 
             
   		fprintf(f,"/**\n");
   		fprintf(f,"* Note:strMessage is a  String to describe the occi response status and message\n"); 
                fprintf(f,"* J%s is a java class to interface the accords category :%s.\n",categoryName,categoryName);
   		fprintf(f,"* -Attributes of this category are members of this class.\n");
   		fprintf(f,"* -List of attributes:\n\n");
   		elem *pelemm1=categoryAtr.first;
   		while(pelemm1)
   		{
        		fprintf(f,"\t\t- %s \n",pelemm1->value);
        		pelemm1=pelemm1->next;
   		}
   		
   		fprintf(f,"*/\n");
    		fprintf(f,"public class J%sAction{\n",categoryName);
   		elem *pelemaction=categoryAct.first;
   		while(pelemaction)
   		{
     			fprintf(f,"\tpublic static J%sBase %s_%s(J%sBase obj, StringBuilder strMessage){\n",categoryName,categoryName,pelemaction->value,categoryName);
     			fprintf(f,"\t\t/*Implement here your function*/\n\n");
     			fprintf(f,"\t\treturn obj;\n");
                        fprintf(f,"\t}\n\n");
     			pelemaction=pelemaction->next;
   		}
   		fprintf(f,"}\n");
		fclose(f);
              
  	}
  
 	return 1;

}

/*------------------------------------------------------------------------------------------------*/
/* Function to generate category interface membership file                                        */
/*------------------------------------------------------------------------------------------------*/
int generateJCategoryInterfaceCfile(char *categoryName, listc categoryAtr, int flag, char pathf[])
{
 	FILE * f;
 	int j,i=0;
 	int k=0;
 	char name[1024];
 	char funcName[4][20]={"create","retrieve","update","delete"};
 
 	sprintf(name,"%s/%s/%s/%sInterface.c",pathf,JACCORDS,JACCORDSS,categoryName);
 	if((f=fopen(name,"w"))==NULL)
 	{
   		printf("Error generateJCategoryInterfaceCfile: No such file or directory\n");
   		return 0;
 	}
 	else
 	{
  		fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
  		fprintf(f,"/* ACCORDS Platform                                                              */\n");
  		fprintf(f,"/* copyright 2012, Hamid MEDJAHE (hmedjahed@prologue.fr)    Prologue             */\n");
  		fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
  		fprintf(f,"/* Licensed under the Apache License, Version 2.0 (the \"License\");               */\n");
  		fprintf(f,"/* you may not use this file except in compliance with the License.              */\n");
  		fprintf(f,"/* You may obtain a copy of the License at                                       */\n");
  		fprintf(f,"/*                                                                               */\n");
  		fprintf(f,"/*       http://www.apache.org/licenses/LICENSE-2.0                              */\n");
  		fprintf(f,"/*                                                                               */\n");
  		fprintf(f,"/* Unless required by applicable law or agreed to in writing, software           */\n");
  		fprintf(f,"/* distributed under the License is distributed on an \"AS IS\" BASIS,             */\n");
  		fprintf(f,"/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.      */\n");
  		fprintf(f,"/* See the License for the specific language governing permissions and           */\n");
  		fprintf(f,"/* limitations under the License.                                                */\n");
  		fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
                fprintf(f,"\n\n/* jaccordsSDK: This code has been generated by jaccordsSDK                      */\n"); 
		fprintf(f,"#ifndef _%sinterface_c\n",categoryName);
		fprintf(f,"#define _%sinterface_c\n\n",categoryName);
  		fprintf(f,"#include \"../../occi/src/occi.h\"\n");
  		fprintf(f,"#include \"jctools.h\"\n");
  		fprintf(f,"#include \"jlistcateg.h\"\n");
  		fprintf(f,"#include \"jtools.h\"\n\n");
  		for(j=0;j<4;j++)
  		{
      			fprintf(f,"private int %s_%s(struct occi_category * optr, void * vptr)\n",categoryName,funcName[j]);
      			fprintf(f,"{\n");
      			fprintf(f,"\tstruct occi_kind_node * nptr;\n");
      			if(!flag) fprintf(f,"\tstruct cords_%s * pptr;\n",categoryName);
      			else fprintf(f,"\tstruct %s * pptr;\n",categoryName);
      			fprintf(f,"\tchar sendstr[1024];\n");
      			fprintf(f,"\tchar strtmp[1024];\n");
      			fprintf(f,"\tchar srcdir[1024];\n");
      			fprintf(f,"\tchar * response = NULL;\n");
      			fprintf(f,"\tchar * token;\n");
      			fprintf(f,"\tlistcc categoryAtr;\n");
       			fprintf(f,"\tJNIEnv *env;\n");
                        fprintf(f,"\tjclass cls;\n");
			fprintf(f,"\tjmethodID mid;\n");
			fprintf(f,"\tjobject retval;\n");
			fprintf(f,"\tjstring jmessage;\n");
			fprintf(f,"\tchar * retString;\n");
      			
      			fprintf(f,"\tif (!( nptr = vptr ))\n");
      			fprintf(f,"\t\treturn(0);\n");
      			fprintf(f,"\telse if (!( pptr = nptr->contents ))\n");
      			fprintf(f,"\t\treturn(0);\n\n");

      			elem *pelem = categoryAtr.first;
      			fprintf(f,"\tif(!(strValid(pptr->%s))) strcpy(sendstr,\" \");\n",pelem->value);
      			fprintf(f,"\telse strcpy(sendstr,pptr->%s);\n",pelem->value);
      			pelem=pelem->next;
      			while(pelem)
      			{
        			fprintf(f,"\tif(!(strValid(pptr->%s))){\n",pelem->value);
        			fprintf(f,"\t\tstrcpy(strtmp,\" \");\n");
        			fprintf(f,"\t\tstrConcat(sendstr,strtmp,',');\n");
        			fprintf(f,"\t}\n");
        			fprintf(f,"\telse strConcat(sendstr,pptr->%s,',');\n",pelem->value);
        			pelem=pelem->next;
        			i++;
      			}
      			i++;
      			
      
      			fprintf(f,"\t//           JNI interface\n");
		        fprintf(f,"\tenv = Getenv();\n");
			fprintf(f,"\tif(env)\n");
			fprintf(f,"\t{\n");
			fprintf(f,"\t\tcls = (*env)->FindClass(env,\"J%sInt\");\n",categoryName);
			fprintf(f,"\t\tif(cls != 0)\n");		
			fprintf(f,"\t\t{\n");
			fprintf(f,"\t\t\tmid = (*env)->GetStaticMethodID(env,cls,\"%s\",\"(Ljava/lang/String;)Ljava/lang/String;\");\n",funcName[j]);      
			fprintf(f,"\t\t\tif(mid != 0)\n");
			fprintf(f,"\t\t\t{\n");
			fprintf(f,"\t\t\t\tjmessage = (jstring)(*env)->NewStringUTF(env,(const char *) sendstr);\n");
			fprintf(f,"\t\t\t\tretval = (*env)->CallStaticObjectMethod(env, cls, mid, jmessage);\n");
			fprintf(f,"\t\t\t\tretString = (char*) (*env)->GetStringUTFChars(env,retval, NULL);\n");  
                        fprintf(f,"\t\t\t\tresetListe(&categoryAtr);\n");  
      			fprintf(f,"\t\t\t\ttoken= strtok(retString,\",\");\n");
      			fprintf(f,"\t\t\t\tfor(; token != NULL ;)\n");
      			fprintf(f,"\t\t\t\t{\n");
      			fprintf(f,"\t\t\t\t\taddBacke(&categoryAtr,token);\n");
      			fprintf(f,"\t\t\t\t\ttoken=strtok(NULL, \",\");\n");
      			fprintf(f,"\t\t\t\t}\n");
      			fprintf(f,"\t\t\t\telemm *pelem = categoryAtr.first;\n");

      			elem *pelemm = categoryAtr.first;
      			k++;
      			while(pelemm)
      			{	
        			fprintf(f,"\t\t\t\tif(pelem){\n");
        			fprintf(f,"\t\t\t\t\tif(strValid(pelem->value)) pptr->%s = pelem->value;\n",pelemm->value);
        			if(k<i)fprintf(f,"\t\t\t\t\tpelem = pelem->next;\n");
        			fprintf(f,"\t\t\t\t}\n");
        			pelemm=pelemm->next;
				k++;
      			}
                        fprintf(f,"\t\t\t\t(*env)->ReleaseStringUTFChars(env, retval,retString);\n");
			fprintf(f,"\t\t\tDetachenv();\n");
     		        fprintf(f,"\t\t\t\treturn 1;\n");	
       			fprintf(f,"\t\t\t}\n");
			fprintf(f,"\t\t\telse\n");
			fprintf(f,"\t\t\t{\n");
			fprintf(f,"\t\t\t\tprintf(\"Error in J%sInt.class: GetMethodID returned 0.\\n\");\n",categoryName);
                        fprintf(f,"\t\t\t\treturn 0;\n");
			fprintf(f,"\t\t\t}\n");
			fprintf(f,"\t\t}\n");
			fprintf(f,"\t\telse\n");
			fprintf(f,"\t\t{\n");
			fprintf(f,"\t\t\tprintf(\"Error in J%sInt.class: FindClass function returned 0.\\n\");\n",categoryName);
                        fprintf(f,"\t\t\treturn 0;\n");
			fprintf(f,"\t\t}\n");
			fprintf(f,"\t}\n");
                        fprintf(f,"\telse\n");
			fprintf(f,"\t{\n");
			fprintf(f,"\t\tprintf(\"Error in J%sInt : creating JVM failed.\\n\");\n",categoryName);
                        fprintf(f,"\t\treturn 0;\n");
			fprintf(f,"\t}\n");
      			fprintf(f,"\treturn 1;\n");
     			fprintf(f,"}\n\n"); 
     			k=0;
     			i=0;
    		}
		fprintf(f,"#endif\n");
      		fclose(f);
 	}
   	return 1;
}
/*------------------------------------------------------------------------------------------------*/
/* Function to generate category Interfces file                                                   */
/* pathf: (char*) path to the project directory name                                              */
/* return 1 if succeeded                                                                          */
/*------------------------------------------------------------------------------------------------*/
int generateJCategoryInterfceStructFile(char pathf[])
{
 	FILE * f;
 	FILE * h;
 	char categoryTmp[1024];
	char categoryName[1024];
 	char strtmp[1024];
 	char line[1024];
 	char name1[1024];
 	char name2[1024];
 	listc categoryList;
 	sprintf(name1,"%s/%s",pathf,JLISTCATEG_FILE);
 	sprintf(name2,"%s/%s/%s/jcrudinterf.h",pathf,JACCORDS,JACCORDSS); 
  
  	if((f=fopen(name1,"r"))==NULL)
  	{
   		printf("Error generateJCategoryInterfaceHfile: No such file or directory\n");
   	return 0;
  	}

  	if((h=fopen(name2,"w"))==NULL)
  	{
   		fclose(f);
  		printf("Error generateCategoryInterface :No such file or directory\n");
   		return 0;
  	}
 
  	resetList(&categoryList); 
  	fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
  	fprintf(f,"/* ACCORDS Platform                                                              */\n");
  	fprintf(f,"/* copyright 2012, Hamid MEDJAHE (hmedjahed@prologue.fr)    Prologue             */\n");
  	fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
  	fprintf(f,"/* Licensed under the Apache License, Version 2.0 (the \"License\");               */\n");
  	fprintf(f,"/* you may not use this file except in compliance with the License.              */\n");
  	fprintf(f,"/* You may obtain a copy of the License at                                       */\n");
  	fprintf(f,"/*                                                                               */\n");
  	fprintf(f,"/*       http://www.apache.org/licenses/LICENSE-2.0                              */\n");
  	fprintf(f,"/*                                                                               */\n");
  	fprintf(f,"/* Unless required by applicable law or agreed to in writing, software           */\n");
  	fprintf(f,"/* distributed under the License is distributed on an \"AS IS\" BASIS,             */\n");
  	fprintf(f,"/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.      */\n");
  	fprintf(f,"/* See the License for the specific language governing permissions and           */\n");
  	fprintf(f,"/* limitations under the License.                                                */\n");
  	fprintf(f,"/*-------------------------------------------------------------------------------*/\n"); 
  	fprintf(h,"#ifndef _JCATEGINTERF_H_\n");
  	fprintf(h,"#define _JCATEGINTERF_H_\n");
  	fprintf(h,"#include \"jcategcrud.h\"\n");
  	fprintf(h,"\n");
 
  	while(fgets(line,sizeof(line), f))
  	{
    
    		line[strlen(line)]=0;
    		str_sub(line,0,7,strtmp);
    		if(strcmp(strtmp,"#include")==0)
    		{
     			strcpy(categoryTmp,getCategoryName(line,"\"",1));
     			str_sub(categoryTmp,0,strlen(categoryTmp)-3,categoryName);
    
     			fprintf(h,"struct occi_interface *%s_interface_Func(){\n",categoryName);
     			fprintf(h,"\tstruct occi_interface %s_interface={%s_create,%s_retrieve,%s_update,%s_delete};\n",categoryName,categoryName,categoryName,categoryName,categoryName);
     			fprintf(h,"\tstruct occi_interface *optr = (struct occi_interface *) malloc (sizeof(struct occi_interface));//begin %s\n",categoryName);
     			fprintf(h,"\toptr->create = %s_interface.create;\n",categoryName);
     			fprintf(h,"\toptr->retrieve = %s_interface.retrieve;\n",categoryName);
     			fprintf(h,"\toptr->update = %s_interface.update;\n",categoryName);
     			fprintf(h,"\toptr->delete = %s_interface.delete;\n",categoryName);
     			fprintf(h,"\treturn optr;}//end %s\n",categoryName);
     			addBack(&categoryList,categoryName);
   		}
  	}
  	fprintf(h,"\nstruct{\n");
  	fprintf(h,"\tconst char* name;\n");
  	fprintf(h,"\tstruct occi_interface *(*interface_Func)();\n");
  	fprintf(h,"}occiCategoryInterface_map[]={\n");
  	elem *pelem = categoryList.first;    
  	while(pelem)
  	{  
     		fprintf(h,"\t{ \"%s\" , %s_interface_Func },\n",pelem->value,pelem->value);
     		pelem = pelem->next;  
  	}
  	
  	fprintf(h,"};\n");
  	fprintf(h,"#endif\n");
  	fclose(f);
  	fclose(h);
  	return 1;
}

/*---------------------------------------------------------------------------------------------------------------*/
/* function to generate java source file                                                                       */
/*---------------------------------------------------------------------------------------------------------------*/
int generateCategoryJSourcefile(char *categoryName,listc categoryAtr,char pathf[])
{
 	FILE * f;
 	int j,i=0;
 	char funcName[4][20]={"create","retrieve","update","delete"}; 
 	char classAtr[1024];
        char listAtr[1024]="( tokens[0]";
 	char lista[1024];
 	char listResult[1024];
 	char listr[1024];
 	char name1[1024];
 	char name2[1024];
 	char name3[1024];
 	char strm[1024];
         
 	sprintf(name1,"%s/%s/%s/J%sBase.java",pathf,JACCORDS,JACCORDSS,categoryName);
 	sprintf(name2,"%s/%s/%s/J%sInt.java",pathf,JACCORDS,JACCORDSS,categoryName);
 	sprintf(name3,"%s/%s/J%sInterface.java",pathf,JACCORDS,categoryName);
 	if((f=fopen(name1,"w"))==NULL)
 	{
   		printf("Error generateCategoryJsourcefile: No such file or directory\n");
   		return 0;
 	}
 	else
 	{
		fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
  		fprintf(f,"/* ACCORDS Platform                                                              */\n");
  		fprintf(f,"/* copyright 2012, Hamid MEDJAHE (hmedjahed@prologue.fr)    Prologue             */\n");
  		fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
  		fprintf(f,"/* Licensed under the Apache License, Version 2.0 (the \"License\");               */\n");
  		fprintf(f,"/* you may not use this file except in compliance with the License.              */\n");
  		fprintf(f,"/* You may obtain a copy of the License at                                       */\n");
  		fprintf(f,"/*                                                                               */\n");
  		fprintf(f,"/*       http://www.apache.org/licenses/LICENSE-2.0                              */\n");
  		fprintf(f,"/*                                                                               */\n");
  		fprintf(f,"/* Unless required by applicable law or agreed to in writing, software           */\n");
  		fprintf(f,"/* distributed under the License is distributed on an \"AS IS\" BASIS,             */\n");
  		fprintf(f,"/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.      */\n");
  		fprintf(f,"/* See the License for the specific language governing permissions and           */\n");
  		fprintf(f,"/* limitations under the License.                                                */\n");
  		fprintf(f,"/*-------------------------------------------------------------------------------*/\n"); 
                fprintf(f,"\n\n/* jaccordsSDK: This code has been generated by jaccordsSDK                      */\n"); 

  		fprintf(f,"//This Class defines the %s category structure\n",categoryName);
   		elem *pelem = categoryAtr.first;
                sprintf(classAtr,"( String j%s",pelem->value);
                pelem = pelem->next;
   		while(pelem)
   		{
                        sprintf(lista,"String j%s",pelem->value);
    			strConcat(classAtr,lista,',');
    			pelem=pelem->next;
    			i++;
   		}
   		strConcat(classAtr,")",' ');
   		fprintf(f,"public class J%sBase{\n",categoryName);
 		elem *pelemm=categoryAtr.first;
   		i=0;
   		while(pelemm)
   		{
        		fprintf(f,"\tString %s;\n",pelemm->value);
        		pelemm=pelemm->next;
        		i++;
   		}
            
                fprintf(f,"\tpublic J%sBase%s{\n",categoryName,classAtr);
		elem *pelems=categoryAtr.first;
   		i=0;
   		while(pelems)
   		{
        		fprintf(f,"\t\tthis.%s = j%s;\n",pelems->value, pelems->value);
        		pelems=pelems->next;
        		i++;
   		}
                

     		fprintf(f,"\t}\n");
                fprintf(f,"}\n");
   		fclose(f);
  	}
  
  	if((f=fopen(name2,"w"))==NULL)
  	{
   		printf("Error generateCategoryJSourcefile: No such file or directory\n");
   		return 0;
  	}
  	else
  	{
		fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
  		fprintf(f,"/* ACCORDS Platform                                                              */\n");
  		fprintf(f,"/* copyright 2012, Hamid MEDJAHE (hmedjahed@prologue.fr)    Prologue             */\n");
  		fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
  		fprintf(f,"/* Licensed under the Apache License, Version 2.0 (the \"License\");               */\n");
  		fprintf(f,"/* you may not use this file except in compliance with the License.              */\n");
  		fprintf(f,"/* You may obtain a copy of the License at                                       */\n");
  		fprintf(f,"/*                                                                               */\n");
  		fprintf(f,"/*       http://www.apache.org/licenses/LICENSE-2.0                              */\n");
  		fprintf(f,"/*                                                                               */\n");
  		fprintf(f,"/* Unless required by applicable law or agreed to in writing, software           */\n");
  		fprintf(f,"/* distributed under the License is distributed on an \"AS IS\" BASIS,             */\n");
  		fprintf(f,"/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.      */\n");
  		fprintf(f,"/* See the License for the specific language governing permissions and           */\n");
  		fprintf(f,"/* limitations under the License.                                                */\n");
  		fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
                fprintf(f,"\n\n/* jaccordsSDK: This code has been generated by jaccordsSDK                      */\n"); 
   		fprintf(f,"public class J%sInt{\n",categoryName);
                fprintf(f,"\tstatic J%sBase objA;\n",categoryName);
		fprintf(f,"\tstatic J%sBase objB;\n\n",categoryName);
		

		for(j=1;j<i;j++)
   		{
    			sprintf(lista,"tokens[%d]",j);
    			strConcat(listAtr,lista,',');
   		}
   		strConcat(listAtr,")",' ');
               
                elem *pelemmm=categoryAtr.first;
   		sprintf(listResult,"objB.%s+\",\"",pelemmm->value);
   		pelemmm=pelemmm->next;
   		for(j=1;j<i-1;j++)
   		{
     			sprintf(listr,"objB.%s+\",\"",pelemmm->value);
     			strConcat(listResult,listr,'+');
     			pelemmm=pelemmm->next;
   		}
                sprintf(listr,"objB.%s",pelemmm->value);	
                strConcat(listResult,listr,'+');
   		
           	for(i=0;i<4;i++)
   		{
      			fprintf(f,"\tpublic static String %s(String str){\n",funcName[i]);
			fprintf(f,"\t\tString dlim = \"[,]+\";\n");
     			fprintf(f,"\t\tString [] tokens = str.split(dlim);\n");
     			fprintf(f,"\t\tobjA = new J%sBase%s;\n",categoryName,listAtr);
			fprintf(f,"\t\tobjB = J%sInterface.%s_%s(objA);\n",categoryName, categoryName, funcName[i]);
			fprintf(f,"\t\tString stresponse = %s;\n",listResult);
     			fprintf(f,"\t\treturn stresponse;\n");
                        fprintf(f,"\t}\n\n");
   		}
		fprintf(f,"}\n");
   		fclose(f);
  	}
 
  	if((f=fopen(name3,"w"))==NULL)
  	{
   		printf("Error generateCategoryJSourcefile: No such file or directory\n");
   		return 0;
  	}
  	else
  	{
		fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
  		fprintf(f,"/* ACCORDS Platform                                                              */\n");
  		fprintf(f,"/* copyright 2012, Hamid MEDJAHE (hmedjahed@prologue.fr)    Prologue             */\n");
  		fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
  		fprintf(f,"/* Licensed under the Apache License, Version 2.0 (the \"License\");               */\n");
  		fprintf(f,"/* you may not use this file except in compliance with the License.              */\n");
  		fprintf(f,"/* You may obtain a copy of the License at                                       */\n");
  		fprintf(f,"/*                                                                               */\n");
  		fprintf(f,"/*       http://www.apache.org/licenses/LICENSE-2.0                              */\n");
  		fprintf(f,"/*                                                                               */\n");
  		fprintf(f,"/* Unless required by applicable law or agreed to in writing, software           */\n");
  		fprintf(f,"/* distributed under the License is distributed on an \"AS IS\" BASIS,             */\n");
  		fprintf(f,"/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.      */\n");
  		fprintf(f,"/* See the License for the specific language governing permissions and           */\n");
  		fprintf(f,"/* limitations under the License.                                                */\n");
  		fprintf(f,"/*-------------------------------------------------------------------------------*/\n"); 
                fprintf(f,"\n\n/* jaccordsSDK: This code has been generated by jaccordsSDK                      */\n");
   		fprintf(f,"/**\n");
                fprintf(f,"* Note: %s is a java class to interface the accords category :%s.\n",categoryName,categoryName);
   		fprintf(f,"*-Attributes of this category are members of this class.\n");
   		fprintf(f,"*-List of attributes:\n\n");
   		elem *pelemm1=categoryAtr.first;
   		while(pelemm1)
   		{
        		fprintf(f,"\t\t- %s \n",pelemm1->value);
        		pelemm1=pelemm1->next;
   		}
   		
   		fprintf(f,"*/\n\n");
  		fprintf(f,"public class J%sInterface{\n",categoryName);
   		for(i=0;i<4;i++)
   		{
     			fprintf(f,"\tpublic static J%sBase %s_%s(J%sBase obj){\n",categoryName,categoryName,funcName[i],categoryName);
     			fprintf(f,"\t\t/*Implement here your function*/\n\n");
     			fprintf(f,"\t\treturn obj;\n");
                        fprintf(f,"\t}\n\n");
   		}
   		fprintf(f,"}\n");
		fclose(f);
  		return 1;
  	}
}



/*-----------------------------------------------------------------------------------------------------------------*/
/* Function to generate the occicategory.c file                                                                    */
/* categoryName:(char*) the name of the category                                                                   */
/* categoryAttributes: list of attributes                                                                          */
/* pathf:(char*) path name file                                                                                    */
/* dim: (int) number of attributes                                                                                 */
/* return 1 if succeeded                                                                                           */
/*-----------------------------------------------------------------------------------------------------------------*/
int createJCategoryOcciFile(char *categoryName, listc categoryAttributes, int dim,int flag, char pathf[])
{
 	char pathfc[1024];
 	char pathfcc[1024];
 	char **nameAtr; 
 	FILE *f;
 	int i;
 
 	str_sub(pathf,0,strlen(pathf)-3,pathfc);
 	sprintf(pathfcc,"%s.c",pathfc);
 
 	nameAtr = (char**) malloc(sizeof (char*) * dim);
  	for(i = 0; i < dim; i++)
      	nameAtr[i] = (char*) malloc(sizeof (char) * DIM);

 	enJTete(pathfcc);

 	if((f=fopen(pathfcc,"a"))==NULL)
 	{
  		printf("Error create category occi file C: No such file or directory\n");
  		return 0;
 	}
 	else
 	{
    		i=0;
    		elem *pelem = categoryAttributes.first;    
    		while(pelem)
    		{       
     			strcpy(nameAtr[i],pelem->value);     
     			pelem = pelem->next; 
     			i++;   
   	 	}
    		
    		fprintf(f,"#ifndef _occi%s_c_\n",categoryName);
    		fprintf(f,"#define _occi%s_c_\n\n",categoryName);
    		fprintf(f,"#include \"%s.h\"\n\n",categoryName);
    		fprintf(f,"/*	--------------------------------	*/\n");
    		if(!flag) fprintf(f,"/*	o c c i _ c o r d s _ %s 	        */\n",categoryName);
    		else  if(!flag) fprintf(f,"/*	o c c i _ %s 	        */\n",categoryName);
    		fprintf(f,"/*	--------------------------------	*/\n");

    		fprintf(f,"/*	--------------------------------------------------------------------	*/\n");
    		fprintf(f,"/*	o c c i   c a t e g o r y   m a n a g e m e n t   s t r u c t u r e 	*/\n");
    		fprintf(f,"/*	--------------------------------------------------------------------	*/\n");
    		if(!flag)
    		{
       			fprintf(f,"struct cords_%s * allocate_cords_%s();\n",categoryName,categoryName);
       			fprintf(f,"struct cords_%s * liberate_cords_%s(struct cords_%s * optr);\n",categoryName,categoryName,categoryName);
       			fprintf(f,"private pthread_mutex_t list_cords_%s_control=PTHREAD_MUTEX_INITIALIZER;\n",categoryName);
       			fprintf(f,"private struct occi_kind_node * cords_%s_first = (struct occi_kind_node *) 0;\n",categoryName);
       			fprintf(f,"private struct occi_kind_node * cords_%s_last  = (struct occi_kind_node *) 0;\n",categoryName);
       			fprintf(f,"public struct  occi_kind_node * occi_first_cords_%s_node() { return( cords_%s_first ); }\n\n",categoryName,categoryName);
    		}
    		else
    		{
       			fprintf(f,"struct %s * allocate_%s();\n",categoryName,categoryName);
       			fprintf(f,"struct %s * liberate_%s(struct %s * optr);\n",categoryName,categoryName,categoryName);
       			fprintf(f,"private pthread_mutex_t list_%s_control=PTHREAD_MUTEX_INITIALIZER;\n",categoryName);
       			fprintf(f,"private struct occi_kind_node * %s_first = (struct occi_kind_node *) 0;\n",categoryName);
       			fprintf(f,"private struct occi_kind_node * %s_last  = (struct occi_kind_node *) 0;\n",categoryName);
       			fprintf(f,"public struct  occi_kind_node * occi_first_%s_node() { return( %s_first ); }\n\n",categoryName,categoryName);
    		}
    		fprintf(f,"/*	----------------------------------------------	*/\n");
    		fprintf(f,"//	o c c i   c a t e g o r y   d r o p   n o d e 	\n");
    		fprintf(f,"/*	----------------------------------------------	*/\n");
    		if(!flag) fprintf(f,"private struct occi_kind_node * ll_drop_cords_%s_node(struct occi_kind_node * nptr) {\n",categoryName);
    		else fprintf(f,"private struct occi_kind_node * ll_drop_%s_node(struct occi_kind_node * nptr) {\n",categoryName);
    		fprintf(f,"\tif ( nptr ) {\n");
    		fprintf(f,"\t\tif (!( nptr->previous ))\n");
    		if(!flag) fprintf(f,"\t\t\tcords_%s_first = nptr->next;\n",categoryName);
    		else fprintf(f,"\t\t\t%s_first = nptr->next;\n",categoryName);
    		fprintf(f,"\t\telse	nptr->previous->next = nptr->next;\n");
    		fprintf(f,"\t\tif (!( nptr->next ))\n");
    		if(!flag) fprintf(f,"\t\t\tcords_%s_last = nptr->previous;\n",categoryName);
    		else fprintf(f,"\t\t\t%s_last = nptr->previous;\n",categoryName);
    		fprintf(f,"\t\telse	nptr->next->previous = nptr->previous;\n");
    		fprintf(f,"\t\t\tliberate_occi_kind_node( nptr );\n");
    		fprintf(f,"\t}\n");
    		fprintf(f,"\treturn((struct occi_kind_node *)0);\n");
    		fprintf(f,"}\n");
    		if(!flag) 
    		{
       			fprintf(f,"private struct occi_kind_node * drop_cords_%s_node(struct occi_kind_node * nptr) {\n",categoryName);
       			fprintf(f,"\tpthread_mutex_lock( &list_cords_%s_control );\n",categoryName);
       			fprintf(f,"\tnptr = ll_drop_cords_%s_node( nptr );\n",categoryName);
       			fprintf(f,"\tpthread_mutex_unlock( &list_cords_%s_control );\n",categoryName);
    		}
    		else 
    		{
      			fprintf(f,"private struct occi_kind_node * drop_%s_node(struct occi_kind_node * nptr) {\n",categoryName);
      			fprintf(f,"\tpthread_mutex_lock( &list_%s_control );\n",categoryName);
      			fprintf(f,"\tnptr = ll_drop_%s_node( nptr );\n",categoryName);
      			fprintf(f,"\tpthread_mutex_unlock( &list_%s_control );\n",categoryName);
    		}  
    		fprintf(f,"\treturn(nptr);\n");
    		fprintf(f,"}\n\n");

    		fprintf(f,"/*	--------------------------------------------------	*/\n");
    		fprintf(f,"//	o c c i   c a t e g o r y   l o c a t e   n o d e 	\n");
    		fprintf(f,"/*	--------------------------------------------------	*/\n");
    		if(!flag) fprintf(f,"private struct occi_kind_node * ll_locate_cords_%s_node(char * id) {\n",categoryName);
    		else  fprintf(f,"private struct occi_kind_node * ll_locate_%s_node(char * id) {\n",categoryName);
    		fprintf(f,"\tstruct occi_kind_node * nptr;\n");
    		if(!flag)
    		{
       			fprintf(f,"\tstruct cords_%s * pptr;\n",categoryName);
       			fprintf(f,"\tfor ( nptr = cords_%s_first;\n",categoryName);
    		}
    		else
    		{
       			fprintf(f,"\tstruct %s * pptr;\n",categoryName);
       			fprintf(f,"\tfor ( nptr = %s_first;\n",categoryName);
    		}
    		fprintf(f,"\t\tnptr != (struct occi_kind_node *) 0;\n");
    		fprintf(f,"\t\tnptr = nptr->next ) {\n");
    		fprintf(f,"\t\tif (!( pptr = nptr->contents )) continue;\n");
    		fprintf(f,"\t\telse if (!( pptr->id )) continue;\n");
    		fprintf(f,"\t\telse if (!( strcmp(pptr->id,id) )) break;\n");
    		fprintf(f,"\t}\n");
    		fprintf(f,"\treturn( nptr );\n");
    		fprintf(f,"}\n\n");
    		if(!flag) fprintf(f,"private struct occi_kind_node * locate_cords_%s_node(char * id) {\n",categoryName);
    		else fprintf(f,"private struct occi_kind_node * locate_%s_node(char * id) {\n",categoryName);
    		fprintf(f,"\tstruct occi_kind_node * nptr;\n");
    		if(!flag)
    		{
       			fprintf(f,"\tpthread_mutex_lock( &list_cords_%s_control );\n",categoryName);
       			fprintf(f,"\tnptr = ll_locate_cords_%s_node(id);\n",categoryName);
       			fprintf(f,"\tpthread_mutex_unlock( &list_cords_%s_control );\n",categoryName);
    		}
    		else
    		{
       			fprintf(f,"\tpthread_mutex_lock( &list_%s_control );\n",categoryName);
       			fprintf(f,"\tnptr = ll_locate_%s_node(id);\n",categoryName);
       			fprintf(f,"\tpthread_mutex_unlock( &list_%s_control );\n",categoryName);
    		}
    		fprintf(f,"\treturn( nptr );\n");
    		fprintf(f,"}\n\n");

    		fprintf(f,"/*	--------------------------------------------	*/\n");
    		fprintf(f,"//	o c c i   c a t e g o r y   a d d   n o d e 	\n");
    		fprintf(f,"/*	--------------------------------------------	*/\n");
    		if(!flag) fprintf(f,"private struct occi_kind_node * ll_add_cords_%s_node(int mode) {\n",categoryName);
    		else fprintf(f,"private struct occi_kind_node * ll_add_%s_node(int mode) {\n",categoryName);
    		fprintf(f,"\tstruct occi_kind_node * nptr;\n");
    		if(!flag) fprintf(f,"\tstruct cords_%s * pptr;\n",categoryName);
    		else fprintf(f,"\tstruct %s * pptr;\n",categoryName);
    		fprintf(f,"\tif (!( nptr = allocate_occi_kind_node() ))\n");
    		fprintf(f,"\t\treturn( nptr );\n");
    		fprintf(f,"\telse	{\n");
    		if(!flag) fprintf(f,"\t\tif (!( nptr->contents = allocate_cords_%s()))\n",categoryName);
    		else fprintf(f,"\t\tif (!( nptr->contents = allocate_%s()))\n",categoryName);
    		fprintf(f,"\t\t\treturn( liberate_occi_kind_node(nptr) );\n");
    		fprintf(f,"\t\tif (!( pptr = nptr->contents ))\n");
    		fprintf(f,"\t\t\treturn( liberate_occi_kind_node(nptr) );\n");
    		fprintf(f,"\t\telse if (( mode != 0 ) && (!( pptr->id = occi_allocate_uuid())))\n");
    		fprintf(f,"\t\t\treturn( liberate_occi_kind_node(nptr) );\n");
    		fprintf(f,"\t\telse{\n");
   		if(!flag) fprintf(f,"\t\t\tif (!( nptr->previous = cords_%s_last ))\n",categoryName);
    		else fprintf(f,"\t\t\tif (!( nptr->previous = %s_last ))\n",categoryName);
    		if(!flag) fprintf(f,"\t\t\t\tcords_%s_first = nptr;\n",categoryName);
    		else fprintf(f,"\t\t\t\t%s_first = nptr;\n",categoryName);
    		fprintf(f,"\t\t\telse	nptr->previous->next = nptr;\n");
            	if(!flag) fprintf(f,"\t\t\tcords_%s_last = nptr;\n",categoryName);
		else fprintf(f,"\t\t\t%s_last = nptr;\n",categoryName);
		fprintf(f,"\t\t\treturn( nptr );\n");
		fprintf(f,"\t\t}\n");
	    	fprintf(f,"\t}\n");
	    	fprintf(f,"}\n\n");
	    	if(!flag) fprintf(f,"private struct occi_kind_node * add_cords_%s_node(int mode) {\n",categoryName);
	    	else fprintf(f,"private struct occi_kind_node * add_%s_node(int mode) {\n",categoryName);
	    	fprintf(f,"\tstruct occi_kind_node * nptr;\n");
	    	if(!flag)
    		{
       			fprintf(f,"\tpthread_mutex_lock( &list_cords_%s_control );\n",categoryName);
       			fprintf(f,"\tnptr = ll_add_cords_%s_node( mode );\n",categoryName);
       			fprintf(f,"\tpthread_mutex_unlock( &list_cords_%s_control );\n",categoryName);
    		}
    		else
    		{
       			fprintf(f,"\tpthread_mutex_lock( &list_%s_control );\n",categoryName);
       			fprintf(f,"\tnptr = ll_add_%s_node( mode );\n",categoryName);
       			fprintf(f,"\tpthread_mutex_unlock( &list_%s_control );\n",categoryName);
    		}
    		fprintf(f,"\treturn(nptr);\n");
    		fprintf(f,"}\n\n");

    		fprintf(f,"/*	------------------------------------------------------------------------------------------	*/\n");
    		fprintf(f,"/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   l o a d 	*/\n");
    		fprintf(f,"/*	------------------------------------------------------------------------------------------	*/\n");
    		if(!flag)
    		{
       			fprintf(f,"private char*autosave_cords_%s_name=\"cords_%s.xml\";\n",categoryName,categoryName);
       			fprintf(f,"private void autoload_cords_%s_nodes() {\n",categoryName);
       			fprintf(f,"\tchar * fn=autosave_cords_%s_name;	struct occi_kind_node * nptr;\n",categoryName);
       			fprintf(f,"\tstruct cords_%s * pptr;\n",categoryName);
    		}
    		else
    		{
       			fprintf(f,"private char*autosave_%s_name=\"%s.xml\";\n",categoryName,categoryName);
       			fprintf(f,"private void autoload_%s_nodes() {\n",categoryName);
       			fprintf(f,"\tchar * fn=autosave_%s_name;	struct occi_kind_node * nptr;\n",categoryName);
       			fprintf(f,"\tstruct %s * pptr;\n",categoryName);
    		}
    		fprintf(f,"\tstruct xml_element * document;\n");
    		fprintf(f,"\tstruct xml_element * eptr;\n");
    		fprintf(f,"\tstruct xml_element * vptr;\n");
    		fprintf(f,"\tstruct xml_atribut  * aptr;\n");
    		fprintf(f,"\tif (!( document = document_parse_file(fn)))\n");
    		fprintf(f,"\t\treturn;\n");
    		if(!flag) fprintf(f,"\tif ((eptr = document_element(document,\"cords_%ss\")) != (struct xml_element *) 0) {\n",categoryName);
    		else fprintf(f,"\tif ((eptr = document_element(document,\"%ss\")) != (struct xml_element *) 0) {\n",categoryName);
    		fprintf(f,"\t\tfor (vptr=eptr->first; vptr != (struct xml_element *) 0; vptr=vptr->next) {\n");
    		fprintf(f,"\t\t\tif (!( vptr->name )) continue;\n");
    		if(!flag)
    		{
       			fprintf(f,"\t\t\telse if ( strcmp( vptr->name, \"cords_%s\" ) ) continue;\n",categoryName);
       			fprintf(f,"\t\t\telse if (!( nptr = add_cords_%s_node(0))) break;\n",categoryName);
    		}
    		else
    		{
       			fprintf(f,"\t\t\telse if ( strcmp( vptr->name, \"%s\" ) ) continue;\n",categoryName);
       			fprintf(f,"\t\t\telse if (!( nptr = add_%s_node(0))) break;\n",categoryName);
    		}
    		fprintf(f,"\t\t\telse if (!( pptr = nptr->contents )) break;\n");
    		for(i=0;i<dim;i++)
    		{
     			fprintf(f,"\t\t\tif ((aptr = document_atribut( vptr, \"%s\" )) != (struct xml_atribut *) 0)\n",nameAtr[i]);
     			fprintf(f,"\t\t\t\tpptr->%s = document_atribut_string(aptr);\n",nameAtr[i]);
    		}
    		fprintf(f,"\t\t}\n");
    		fprintf(f,"\t}\n");
    		fprintf(f,"\tdocument = document_drop( document );\n");
    		fprintf(f,"\treturn;\n");
    		fprintf(f,"}\n\n");
    		fprintf(f,"/*	------------------------------------------------------------------------------------------	*/\n");
    		fprintf(f,"/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   s a v e 	*/\n");
    		fprintf(f,"/*	------------------------------------------------------------------------------------------	*/\n");
    		if(!flag)
    		{
       			fprintf(f,"public  void set_autosave_cords_%s_name(char * fn) {\n",categoryName);
       			fprintf(f,"\tautosave_cords_%s_name = fn;	return;\n",categoryName);
       			fprintf(f,"}\n");
       			fprintf(f,"public  void autosave_cords_%s_nodes() {\n",categoryName);
       			fprintf(f,"\tchar * fn=autosave_cords_%s_name;	struct occi_kind_node * nptr;\n",categoryName);
       			fprintf(f,"\tstruct cords_%s * pptr;\n",categoryName);
    		}
    		else
    		{
       			fprintf(f,"public  void set_autosave_%s_name(char * fn) {\n",categoryName);
       			fprintf(f,"\tautosave_%s_name = fn;	return;\n",categoryName);
       			fprintf(f,"}\n");
       			fprintf(f,"public  void autosave_%s_nodes() {\n",categoryName);
       			fprintf(f,"\tchar * fn=autosave_%s_name;	struct occi_kind_node * nptr;\n",categoryName);
       			fprintf(f,"\tstruct %s * pptr;\n",categoryName);
    		}
    		fprintf(f,"\tFILE * h;\n");
    		if(!flag) fprintf(f,"\tpthread_mutex_lock( &list_cords_%s_control );\n",categoryName);
    		else fprintf(f,"\tpthread_mutex_lock( &list_%s_control );\n",categoryName);
    		fprintf(f,"\tif (( h = fopen(fn,\"w\")) != (FILE *) 0) {\n");
    		if(!flag) 
    		{
       			fprintf(f,"\tfprintf(h,\"<cords_%ss>\\n\");\n",categoryName);
       			fprintf(f,"\tfor ( nptr = cords_%s_first;\n",categoryName);
    		}
    		else
    		{
       			fprintf(f,"\tfprintf(h,\"<%ss>\\n\");\n",categoryName);
       			fprintf(f,"\tfor ( nptr = %s_first;\n",categoryName);
    		}
    		fprintf(f,"\t\tnptr != (struct occi_kind_node *) 0;\n");
    		fprintf(f,"\t\tnptr = nptr->next ) {\n");
    		fprintf(f,"\t\tif (!( pptr = nptr->contents )) continue;\n");
    		if(!flag) fprintf(f,"\t\tfprintf(h,\"<cords_%s\\n\");\n",categoryName);
    		else fprintf(f,"\t\tfprintf(h,\"<%s\\n\");\n",categoryName);
    		for(i=0;i<dim;i++)
    		{
      			fprintf(f,"\t\tfprintf(h,\" %s=%%c\",0x0022);\n",nameAtr[i]);
      			fprintf(f,"\t\tfprintf(h,\"%%s\",(pptr->%s?pptr->%s:\"\"));\n",nameAtr[i],nameAtr[i]);
      			fprintf(f,"\t\tfprintf(h,\"%%c\",0x0022);\n"); 
    		}
    		fprintf(f,"\t\tfprintf(h,\" />\\n\");\n");
    		fprintf(f,"\t\t}\n");
    		if(!flag) fprintf(f,"\tfprintf(h,\"</cords_%ss>\\n\");\n",categoryName);
    		else fprintf(f,"\tfprintf(h,\"</%ss>\\n\");\n",categoryName);
    		fprintf(f,"\tfclose(h);\n");
    		fprintf(f,"\t}\n");
    		if(!flag) fprintf(f,"\tpthread_mutex_unlock( &list_cords_%s_control );\n",categoryName);
    		else fprintf(f,"\tpthread_mutex_unlock( &list_%s_control );\n",categoryName);
    		fprintf(f,"\treturn;\n");
    		fprintf(f,"}\n\n");

    		fprintf(f,"/*	------------------------------------------------------------------------------------------	*/\n");
    		fprintf(f,"/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   s e t   f i e l d 	*/\n");
    		fprintf(f,"/*	------------------------------------------------------------------------------------------	*/\n");
    		if(!flag) fprintf(f,"private void set_cords_%s_field(\n",categoryName);
    		else fprintf(f,"private void set_%s_field(\n",categoryName);
    		fprintf(f,"struct occi_category * cptr,void * optr, char * nptr, char * vptr)\n");
    		fprintf(f,"{\n");
    		if(!flag) fprintf(f,"\tstruct cords_%s * pptr;\n",categoryName);
    		else fprintf(f,"\tstruct %s * pptr;\n",categoryName);
    		fprintf(f,"\tchar prefix[1024];\n");
    		fprintf(f,"\tif (!( pptr = optr )) return;\n");
    		fprintf(f,"\tsprintf(prefix,\"%%s.%%s.\",cptr->domain,cptr->id);\n");
    		fprintf(f,"\tif (!( strncmp( nptr, prefix, strlen(prefix) ) )) {\n");
    		fprintf(f,"\t\tnptr += strlen(prefix);\n");
    		for(i=1;i<dim;i++)
    		{
      			fprintf(f,"\t\tif (!( strcmp( nptr, \"%s\" ) ))\n",nameAtr[i]);
      			fprintf(f,"\t\t\tpptr->%s = allocate_string(vptr);\n",nameAtr[i]);
    		}
    		fprintf(f,"\t}\n"); 
    		fprintf(f,"\treturn;\n");
    		fprintf(f,"}\n\n");

    		fprintf(f,"/*	--------------------------------------------------	*/\n");
    		fprintf(f,"/*	o c c i   c a t e g o r y   f i l t e r   i n f o 	*/\n");
    		fprintf(f,"/*	--------------------------------------------------	*/\n");
    		if(!flag) fprintf(f,"private struct cords_%s * filter_cords_%s_info(\n",categoryName,categoryName);
    		else fprintf(f,"private struct %s * filter_%s_info(\n",categoryName,categoryName);
    		fprintf(f,"\tstruct occi_category * optr,\n");
    		fprintf(f,"\tstruct rest_request  * rptr,\n");
    		fprintf(f,"\tstruct rest_response * aptr) {\n");
    		if(!flag)
    		{
       			fprintf(f,"\tstruct cords_%s * pptr;\n",categoryName);
       			fprintf(f,"\tif (!( pptr = allocate_cords_%s()))\n",categoryName);
    		}
    		else
    		{
      	 		fprintf(f,"\tstruct %s * pptr;\n",categoryName);
       			fprintf(f,"\tif (!( pptr = allocate_%s()))\n",categoryName);

    		}
    			fprintf(f,"\treturn( pptr );\n");
    			if(!flag) fprintf(f,"\telse if (!( occi_process_atributs(optr, rptr, aptr, pptr, set_cords_%s_field) ))\n",categoryName);
    		else fprintf(f,"\telse if (!( occi_process_atributs(optr, rptr, aptr, pptr, set_%s_field) ))\n",categoryName);
    		if(!flag) fprintf(f,"\treturn( liberate_cords_%s(pptr));\n",categoryName);
    		else fprintf(f,"\treturn( liberate_%s(pptr));\n",categoryName);
    		fprintf(f,"\telse	return( pptr );\n");
    		fprintf(f,"}\n\n");

    		fprintf(f,"/*	--------------------------------------------------	*/\n");
    		fprintf(f,"/*	o c c i   c a t e g o r y   f i l t e r   p a s s 	*/\n");
    		fprintf(f,"/*	--------------------------------------------------	*/\n");
    		if(!flag)
    		{
       			fprintf(f,"private int pass_cords_%s_filter(\n",categoryName);
       			fprintf(f,"\tstruct cords_%s * pptr,struct cords_%s * fptr) {\n",categoryName,categoryName);
    		}
    		else
    		{
       			fprintf(f,"private int pass_%s_filter(\n",categoryName);
       			fprintf(f,"\tstruct %s * pptr,struct %s * fptr) {\n",categoryName,categoryName);
    		}
    		for(i=0;i<dim;i++)
    		{
     			fprintf(f,"\tif (( fptr->%s )\n",nameAtr[i]);
     			fprintf(f,"\t&&  (strlen( fptr->%s ) != 0)) {\n",nameAtr[i]);
     			fprintf(f,"\t\tif (!( pptr->%s ))\n",nameAtr[i]);
     			fprintf(f,"\t\t\treturn(0);\n");
     			fprintf(f,"\t\telse if ( strcmp(pptr->%s,fptr->%s) != 0)\n",nameAtr[i],nameAtr[i]);
     			fprintf(f,"\t\treturn(0);\n");
     			fprintf(f,"\t}\n");
    		}
    		fprintf(f,"\treturn(1);\n");
    		fprintf(f,"}\n\n");

    		fprintf(f,"/*	----------------------------------------------------------------------------------------	*/\n");
    		fprintf(f,"/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   r e s p o n s e 	*/\n");
    		fprintf(f,"/*	----------------------------------------------------------------------------------------	*/\n");
    		if(!flag) fprintf(f,"private struct rest_response * cords_%s_occi_response(\n",categoryName);
    		else fprintf(f,"private struct rest_response * %s_occi_response(\n",categoryName);
    		fprintf(f,"struct occi_category * optr, struct rest_client * cptr,\n");
    		fprintf(f,"struct rest_request * rptr, struct rest_response * aptr,\n");
    		if(!flag) fprintf(f,"struct cords_%s * pptr)\n",categoryName);
    		else fprintf(f,"struct %s * pptr)\n",categoryName);
    		fprintf(f,"{\n");
    		fprintf(f,"\tstruct rest_header * hptr;\n");
    		fprintf(f,"\tsprintf(cptr->buffer,\"occi.core.id=%%s\",pptr->id);\n");
    		fprintf(f,"\tif (!( hptr = rest_response_header( aptr, \"X-OCCI-Attribute\",cptr->buffer) ))\n");
    		fprintf(f,"\t\treturn( rest_html_response( aptr, 500, \"Server Failure\" ) );\n");
    		for(i=1;i<dim;i++)
    		{
       			fprintf(f,"\tsprintf(cptr->buffer,\"%%s.%%s.%s=%%s\",optr->domain,optr->id,pptr->%s);\n",nameAtr[i],nameAtr[i]);
       			fprintf(f,"\tif (!( hptr = rest_response_header( aptr, \"X-OCCI-Attribute\",cptr->buffer) ))\n");
       			fprintf(f,"\t\treturn( rest_html_response( aptr, 500, \"Server Failure\" ) );\n");
    		}
    		fprintf(f,"\tif ( occi_render_links( aptr, pptr->id ) != 0)\n");
    		fprintf(f,"\t\treturn( rest_html_response( aptr, 500, \"Server Link Failure\" ) );\n");
    		fprintf(f,"\telse	if (!( occi_success( aptr ) ))\n");
    		fprintf(f,"\t\treturn( rest_response_status( aptr, 500, \"Server Failure\" ) );\n");
    		fprintf(f,"\telse	return( rest_response_status( aptr, 200, \"OK\" ) );\n");
    		fprintf(f,"}\n\n");

    		fprintf(f,"/*	----------------------------------------------------------------------------------------	*/\n");
    		fprintf(f,"/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t   i t e m 	*/\n");
    		fprintf(f,"/*	----------------------------------------------------------------------------------------	*/\n");
    		if(!flag) fprintf(f,"private struct rest_response * cords_%s_get_item(\n",categoryName);
    		else fprintf(f,"private struct rest_response * %s_get_item(\n",categoryName);
    		fprintf(f,"struct occi_category * optr, struct rest_client * cptr,\n");
    		fprintf(f,"struct rest_request * rptr, struct rest_response * aptr, char * id)\n");
    		fprintf(f,"{\n");
    		fprintf(f,"\tstruct rest_header * hptr;\n");
    		fprintf(f,"\tstruct occi_interface * iptr;\n");
    		fprintf(f,"\tstruct occi_kind_node * nptr;\n");
    		if(!flag) fprintf(f,"\tstruct cords_%s * pptr;\n",categoryName);
    		else fprintf(f,"\tstruct %s * pptr;\n",categoryName);
    		fprintf(f,"\tiptr = optr->callback;\n");
    		if(!flag) fprintf(f,"\tif (!( nptr = locate_cords_%s_node(id)))\n",categoryName);
    		else fprintf(f,"\tif (!( nptr = locate_%s_node(id)))\n",categoryName);
    		fprintf(f,"\t\treturn( rest_html_response( aptr, 404, \"Not Found\") );\n");
    		fprintf(f,"\telse if (!( pptr = nptr->contents ))\n");
    		fprintf(f,"\t\treturn( rest_html_response( aptr, 404, \"Not Found\") );\n");
    		fprintf(f,"\tif (( iptr ) && (iptr->retrieve)) (*iptr->retrieve)(optr,nptr);\n");
    		if(!flag)
    		{
      			fprintf(f,"\tautosave_cords_%s_nodes();\n",categoryName);
      			fprintf(f,"\treturn( cords_%s_occi_response(optr,cptr,rptr,aptr,pptr));\n",categoryName);
    		}
    		else
    		{
      			fprintf(f,"\tautosave_%s_nodes();\n",categoryName);
      			fprintf(f,"\treturn( %s_occi_response(optr,cptr,rptr,aptr,pptr));\n",categoryName);
    		}
    		fprintf(f,"}\n\n");

    		fprintf(f,"/*	------------------------------------------------------------------------------------------	*/\n");
    		fprintf(f,"/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   l i n k 	*/\n");
    		fprintf(f,"/*	------------------------------------------------------------------------------------------	*/\n");
    		if(!flag) fprintf(f,"private struct rest_response * cords_%s_post_link(\n",categoryName);
    		else fprintf(f,"private struct rest_response * %s_post_link(\n",categoryName);
    		fprintf(f,"\tstruct occi_category * optr, struct rest_client * cptr,\n");
    		fprintf(f,"\tstruct rest_request * rptr, struct rest_response * aptr,char * id)\n");
    		fprintf(f,"{\n");
    		fprintf(f,"\tstruct rest_header * hptr;\n");
    		fprintf(f,"\tstruct occi_interface * iptr;\n");
    		fprintf(f,"\tstruct occi_kind_node * nptr;\n");
    		if(!flag) fprintf(f,"\tstruct cords_%s * pptr;\n",categoryName);
    		else fprintf(f,"\tstruct %s * pptr;\n",categoryName);
    		fprintf(f,"\tchar * reqhost;\n");
    		if(!flag) fprintf(f,"\tif (!( nptr = locate_cords_%s_node(id)))\n",categoryName);
    		else fprintf(f,"\tif (!( nptr = locate_%s_node(id)))\n",categoryName);
    		fprintf(f,"\t\treturn( rest_html_response( aptr, 404, \"Not Found\") );\n");
    		fprintf(f,"\telse if (!( pptr = nptr->contents ))\n");
    		fprintf(f,"\t\treturn( rest_html_response( aptr, 404, \"Not Found\") );\n");
    		fprintf(f,"\telse	return( rest_html_response( aptr, 400, \"Bad Request\") );\n");
    		fprintf(f,"}\n\n");

    		fprintf(f,"/*	--------------------------------------------------------------------------------------------	*/\n");
    		fprintf(f,"/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   m i x i n 	*/\n");
    		fprintf(f,"/*	--------------------------------------------------------------------------------------------	*/\n");
    		if(!flag) fprintf(f,"private struct rest_response * cords_%s_post_mixin(\n",categoryName);
    		else fprintf(f,"private struct rest_response * %s_post_mixin(\n",categoryName);
    		fprintf(f,"struct occi_category * optr, struct rest_client * cptr,\n");
    		fprintf(f,"struct rest_request * rptr, struct rest_response * aptr,char * id)\n");
    		fprintf(f,"{\n");
    		fprintf(f,"\tstruct rest_header * hptr;\n");
    		fprintf(f,"\tstruct occi_interface * iptr;\n");
    		fprintf(f,"\tstruct occi_kind_node * nptr;\n");
    		if(!flag) fprintf(f,"\tstruct cords_%s * pptr;\n",categoryName);
    		else fprintf(f,"\tstruct %s * pptr;\n",categoryName);
    		fprintf(f,"\tchar * reqhost;\n");
    		if(!flag) fprintf(f,"\tif (!( nptr = locate_cords_%s_node(id)))\n",categoryName);
    		else fprintf(f,"\tif (!( nptr = locate_%s_node(id)))\n",categoryName);
    		fprintf(f,"\t\treturn( rest_html_response( aptr, 404, \"Not Found\") );\n");
    		fprintf(f,"\telse if (!( pptr = nptr->contents ))\n");
    		fprintf(f,"\t\treturn( rest_html_response( aptr, 404, \"Not Found\") );\n");
    		fprintf(f,"\telse	return( rest_html_response( aptr, 400, \"Bad Request\"));\n");
    		fprintf(f,"}\n\n");


    		fprintf(f,"/*	----------------------------------------------------------------------------------------------	*/\n");
    		fprintf(f,"/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   a c t i o n 	*/\n");
    		fprintf(f,"/*	----------------------------------------------------------------------------------------------	*/\n");
    		if(!flag) fprintf(f,"private struct rest_response * cords_%s_post_action(\n",categoryName);
    		else fprintf(f,"private struct rest_response * %s_post_action(\n",categoryName);
    		fprintf(f,"struct occi_category * optr, struct rest_client * cptr,\n");
    		fprintf(f,"struct rest_request * rptr, struct rest_response * aptr,char * id)\n");
    		fprintf(f,"{\n");
    		fprintf(f,"\tstruct rest_header * hptr;\n");
    		fprintf(f,"\tstruct occi_interface * iptr;\n");
    		fprintf(f,"\tstruct occi_action * fptr;\n");
    		fprintf(f,"\tstruct occi_kind_node * nptr;\n");
    		if(!flag) fprintf(f,"\tstruct cords_%s * pptr;\n",categoryName);
    		else fprintf(f,"\tstruct %s * pptr;\n",categoryName);
    		fprintf(f,"\tchar * reqhost;\n");
    		fprintf(f,"\tchar * mptr;\n");
    		if(!flag) fprintf(f,"\tif (!( nptr = locate_cords_%s_node(id)))\n",categoryName);
    		else fprintf(f,"\tif (!( nptr = locate_%s_node(id)))\n",categoryName);
    		fprintf(f,"\t\treturn( rest_html_response( aptr, 404, \"Not Found\") );\n");
    		fprintf(f,"\telse if (!( pptr = nptr->contents ))\n");
    		fprintf(f,"\t\treturn( rest_html_response( aptr, 404, \"Not Found\") );\n");
    		fprintf(f,"\tmptr = (rptr->parameters+strlen(\"action=\"));\n");
    		fprintf(f,"\tfor ( fptr=optr->firstact;\n");
    		fprintf(f,"\t\tfptr != (struct occi_action *) 0;\n");
    		fprintf(f,"\t\tfptr = fptr->next )\n");
    		fprintf(f,"\t\tif (!( strncmp( mptr, fptr->name, strlen( fptr->name )) ))\n");
    		fprintf(f,"\t\t\treturn( occi_invoke_action(fptr,optr,cptr,rptr,aptr,pptr) );\n");
    		fprintf(f,"\treturn( rest_html_response( aptr, 400, \"Incorrect Action Request\"));\n");
    		fprintf(f,"}\n\n");

    		fprintf(f,"/*	------------------------------------------------------------------------------------------	*/\n");
    		fprintf(f,"/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   i t e m 	*/\n");
    		fprintf(f,"/*	------------------------------------------------------------------------------------------	*/\n");
    		if(!flag) fprintf(f,"private struct rest_response * cords_%s_post_item(\n",categoryName);
    		else fprintf(f,"private struct rest_response * %s_post_item(\n",categoryName);
    		fprintf(f,"\tstruct occi_category * optr, struct rest_client * cptr,\n");
    		fprintf(f,"\tstruct rest_request * rptr, struct rest_response * aptr)\n");
    		fprintf(f,"{\n");
    		fprintf(f,"\tstruct rest_header * hptr;\n");
    		fprintf(f,"\tstruct occi_interface * iptr;\n");
    		fprintf(f,"\tstruct occi_kind_node * nptr;\n");
    		if(!flag) fprintf(f,"\tstruct cords_%s * pptr;\n",categoryName);
    		else fprintf(f,"\tstruct %s * pptr;\n",categoryName);
    		fprintf(f,"\tchar * reqhost;\n");
    		fprintf(f,"\tiptr = optr->callback;\n");
    		fprintf(f,"\tif (!( reqhost = rest_request_host( rptr ) ))\n");
    		fprintf(f,"\t\treturn( rest_html_response( aptr, 400, \"Bad Request\" ) );\n");
    		if(!flag) fprintf(f,"\tif (!( nptr = add_cords_%s_node(1)))\n",categoryName);
    		else fprintf(f,"\tif (!( nptr = add_%s_node(1)))\n",categoryName);
    		fprintf(f,"\t\treturn( rest_html_response( aptr, 500, \"Server Failure\") );\n");
    		fprintf(f,"\telse if (!( pptr = nptr->contents ))\n");
    		fprintf(f,"\t\treturn( rest_html_response( aptr, 500, \"Server Failure\") );\n");
    		if(!flag) fprintf(f,"\tif (!( occi_process_atributs( optr, rptr,aptr, pptr, set_cords_%s_field ) ))\n",categoryName);
    		else fprintf(f,"\tif (!( occi_process_atributs( optr, rptr,aptr, pptr, set_%s_field ) ))\n",categoryName);
    		fprintf(f,"\t\treturn( rest_html_response( aptr, 500, \"Server Failure\") );\n");
    		fprintf(f,"\tif (( iptr ) && (iptr->create)) (*iptr->create)(optr,nptr);\n");
    		if(!flag) fprintf(f,"\tautosave_cords_%s_nodes();\n",categoryName);
    		else fprintf(f,"\tautosave_%s_nodes();\n",categoryName);
    		fprintf(f,"\tsprintf(cptr->buffer,\"%%s%%s%%s\",reqhost,optr->location,pptr->id);\n");
    		fprintf(f,"\tif (!( hptr = rest_response_header( aptr, \"X-OCCI-Location\",cptr->buffer) ))\n");
    		fprintf(f,"\t\treturn( rest_html_response( aptr, 500, \"Server Failure\" ) );\n");
    		fprintf(f,"\telse if (!( occi_success( aptr ) ))\n");
    		fprintf(f,"\t\treturn( rest_response_status( aptr, 500, \"Server Failure\" ) );\n");
    		fprintf(f,"\telse	return( rest_response_status( aptr, 200, \"OK\" ) );\n");
    		fprintf(f,"}\n\n");

    		fprintf(f,"/*	----------------------------------------------------------------------------------------	*/\n");
    		fprintf(f,"/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p u t   i t e m 	*/\n");
    		fprintf(f,"/*	----------------------------------------------------------------------------------------	*/\n");
    		if(!flag) fprintf(f,"private struct rest_response * cords_%s_put_item(\n",categoryName);
    		else fprintf(f,"private struct rest_response * %s_put_item(\n",categoryName);
    		fprintf(f,"\tstruct occi_category * optr, struct rest_client * cptr,\n");
    		fprintf(f,"\tstruct rest_request * rptr, struct rest_response * aptr,char * id)\n");
    		fprintf(f,"{\n");  
    		fprintf(f,"\tstruct rest_header * hptr;\n");
    		fprintf(f,"\tstruct occi_interface * iptr;\n");
    		fprintf(f,"\tstruct occi_kind_node * nptr;\n");
		if(!flag) fprintf(f,"\tstruct cords_%s * pptr;\n",categoryName);
		else fprintf(f,"\tstruct %s * pptr;\n",categoryName);
		fprintf(f,"\tiptr = optr->callback;\n");
		if(!flag) fprintf(f,"\tif (!( nptr = locate_cords_%s_node(id)))\n",categoryName);
		else fprintf(f,"\tif (!( nptr = locate_%s_node(id)))\n",categoryName);
		fprintf(f,"\t\treturn( rest_html_response( aptr, 404, \"Not Found\") );\n");
		fprintf(f,"\telse if (!( pptr = nptr->contents ))\n");
		fprintf(f,"\t\treturn( rest_html_response( aptr, 404, \"Not Found\") );\n");
		if(!flag) fprintf(f,"\tif (!( occi_process_atributs(optr,rptr,aptr, pptr, set_cords_%s_field ) ))\n",categoryName);
		else fprintf(f,"\tif (!( occi_process_atributs(optr,rptr,aptr, pptr, set_%s_field ) ))\n",categoryName);
		fprintf(f,"\t\treturn( rest_html_response( aptr, 500, \"Server Failure\") );\n");
		fprintf(f,"\tif (( iptr ) && (iptr->update)) (*iptr->update)(optr,nptr);\n");
		if(!flag) 
		{
			fprintf(f,"\tautosave_cords_%s_nodes();\n",categoryName);
			fprintf(f,"\treturn( cords_%s_occi_response(optr,cptr,rptr,aptr,pptr));\n",categoryName);
		}
		else
		{
			fprintf(f,"\tautosave_%s_nodes();\n",categoryName);
			fprintf(f,"\treturn( %s_occi_response(optr,cptr,rptr,aptr,pptr));\n",categoryName);
		}
		fprintf(f,"}\n\n");

		fprintf(f,"/*	------------------------------------------------------------------------------------------	*/\n");
		fprintf(f,"/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d   i t e m 	*/\n");
		fprintf(f,"/*	------------------------------------------------------------------------------------------	*/\n");
		if(!flag) fprintf(f,"private struct rest_response * cords_%s_head_item(\n",categoryName);
		else fprintf(f,"private struct rest_response * %s_head_item(\n",categoryName);
		fprintf(f,"\tstruct occi_category * optr, struct rest_client * cptr,\n");
		fprintf(f,"\tstruct rest_request * rptr, struct rest_response * aptr,char * id)\n");
		fprintf(f,"{\n");
		fprintf(f,"\tstruct rest_header * hptr;\n");
		fprintf(f,"\tstruct occi_kind_node * nptr;\n");
		if(!flag)
		{
		       fprintf(f,"\tstruct cords_%s * pptr;\n",categoryName);
		       fprintf(f,"\tif (!( nptr = locate_cords_%s_node(id)))\n",categoryName);
		}
		else
		{
			fprintf(f,"\tstruct %s * pptr;\n",categoryName);
			fprintf(f,"\tif (!( nptr = locate_%s_node(id)))\n",categoryName);
		}
		fprintf(f,"\t\treturn( rest_html_response( aptr, 404, \"Not Found\") );\n");;
		fprintf(f,"\telse if (!( pptr = nptr->contents ))\n");
		fprintf(f,"\t\treturn( rest_html_response( aptr, 404, \"Not Found\") );\n");
		fprintf(f,"\telse	return( rest_html_response( aptr, 400, \"Bad Request\") );\n");
		fprintf(f,"}\n\n");

		fprintf(f,"/*	----------------------------------------------------------------------------------------------	*/\n");
		fprintf(f,"/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e   i t e m 	*/\n");
		fprintf(f,"/*	----------------------------------------------------------------------------------------------	*/\n");
		if(!flag) fprintf(f,"private struct rest_response * cords_%s_delete_item(\n",categoryName);
		else fprintf(f,"private struct rest_response * %s_delete_item(\n",categoryName);
		fprintf(f,"\tstruct occi_category * optr, struct rest_client * cptr,\n");
		fprintf(f,"\tstruct rest_request * rptr, struct rest_response * aptr, char * id)\n");
		fprintf(f,"{\n");
		fprintf(f,"\tstruct rest_header * hptr;\n");
		fprintf(f,"\tstruct occi_interface * iptr;\n");
		fprintf(f,"\tstruct occi_kind_node * nptr;\n");
		if(!flag)
		{
			fprintf(f,"\tstruct cords_%s * pptr;\n",categoryName);
			fprintf(f,"\tiptr = optr->callback;\n");
			fprintf(f,"\tif (!( nptr = locate_cords_%s_node(id)))\n",categoryName);
		}
		else
		{
			fprintf(f,"\tstruct %s * pptr;\n",categoryName);
			fprintf(f,"\tiptr = optr->callback;\n");
			fprintf(f,"\tif (!( nptr = locate_%s_node(id)))\n",categoryName);
		}
		fprintf(f,"\t\treturn( rest_html_response( aptr, 404, \"Not Found\") );\n");
		fprintf(f,"\tif (( iptr ) && (iptr->delete)) (*iptr->delete)(optr,nptr);\n");
		if(!flag)
		{
			fprintf(f,"\tdrop_cords_%s_node( nptr );\n",categoryName);
			fprintf(f,"\tautosave_cords_%s_nodes();\n",categoryName);
		}
		else
		{
			fprintf(f,"\tdrop_%s_node( nptr );\n",categoryName);
			fprintf(f,"\tautosave_%s_nodes();\n",categoryName);
		}

		fprintf(f,"\tif (!( occi_success( aptr ) ))\n");
		fprintf(f,"\t\treturn( rest_response_status( aptr, 500, \"Server Failure\" ) );\n");
		fprintf(f,"\telse	return( rest_response_status( aptr, 200, \"OK\" ) );\n");
		fprintf(f,"}\n\n");

		fprintf(f,"/*	----------------------------------------------------------------------------------------	*/\n");
		fprintf(f,"/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t   l i s t 	*/\n");
		fprintf(f,"/*	----------------------------------------------------------------------------------------	*/\n");
		if(!flag) fprintf(f,"private struct rest_response * cords_%s_get_list(\n",categoryName);
		else fprintf(f,"private struct rest_response * %s_get_list(\n",categoryName);
		fprintf(f,"\tstruct occi_category * optr, struct rest_client * cptr,\n");
		fprintf(f,"\tstruct rest_request * rptr, struct rest_response * aptr)\n");
		fprintf(f,"{\n");
		fprintf(f,"\tstruct rest_header * hptr;\n");
		fprintf(f,"\tstruct occi_kind_node * sptr;\n");
		if(!flag)
		{
			fprintf(f,"\tstruct cords_%s * pptr;\n",categoryName);
			fprintf(f,"\tstruct cords_%s * fptr;\n",categoryName);
		}
		else
		{
			fprintf(f,"\tstruct %s * pptr;\n",categoryName);
			fprintf(f,"\tstruct %s * fptr;\n",categoryName);
		}
		fprintf(f,"\tchar * reqhost;\n");
		fprintf(f,"\tif (!( reqhost = rest_request_host( rptr ) ))\n");
		fprintf(f,"\t\treturn( rest_html_response( aptr, 400, \"Bad Request\" ) );\n");
		if(!flag) fprintf(f,"\telse if (!( fptr = filter_cords_%s_info( optr, rptr, aptr ) ))\n",categoryName);
		else  fprintf(f,"\telse if (!( fptr = filter_%s_info( optr, rptr, aptr ) ))\n",categoryName);
		fprintf(f,"\t\treturn( rest_html_response( aptr, 400, \"Bad Request\" ) );\n");
		if(!flag) fprintf(f,"\tfor ( sptr = cords_%s_first;\n",categoryName);
		else fprintf(f,"\tfor ( sptr = %s_first;\n",categoryName);
		fprintf(f,"\t\tsptr != (struct occi_kind_node *) 0;\n");
		fprintf(f,"\t\tsptr = sptr->next ) {\n");
		fprintf(f,"\t\tif (!( pptr = sptr->contents ))\n");
		fprintf(f,"\t\t\tcontinue;\n");
		if(!flag) fprintf(f,"\t\tif (!( pass_cords_%s_filter( pptr, fptr ) ))\n",categoryName);
		else fprintf(f,"\t\tif (!( pass_%s_filter( pptr, fptr ) ))\n",categoryName);
		fprintf(f,"\t\t\tcontinue;\n");
		fprintf(f,"\t\tsprintf(cptr->buffer,\"%%s%%s%%s\",reqhost,optr->location,pptr->id);\n");
		fprintf(f,"\t\tif (!( hptr = rest_response_header( aptr, \"X-OCCI-Location\",cptr->buffer) ))\n");
		fprintf(f,"\t\t\treturn( rest_html_response( aptr, 500, \"Server Failure\" ) );\n");
		fprintf(f,"\t\t}\n");
		fprintf(f,"\tif (!( occi_success( aptr ) ))\n");
		fprintf(f,"\t\treturn( rest_response_status( aptr, 500, \"Server Failure\" ) );\n");
		fprintf(f,"\telse	return( rest_response_status( aptr, 200, \"OK\" ) );\n");
		fprintf(f,"}\n\n");

		fprintf(f,"/*	--------------------------------------------------------------------------------------------	*/\n");
		fprintf(f,"/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e   a l l 	*/\n");
		fprintf(f,"/*	--------------------------------------------------------------------------------------------	*/\n");
		if(!flag) fprintf(f,"private struct rest_response * cords_%s_delete_all(\n",categoryName);
		else fprintf(f,"private struct rest_response * %s_delete_all(\n",categoryName);
		fprintf(f,"\tstruct occi_category * optr, struct rest_client * cptr,\n");
		fprintf(f,"\tstruct rest_request * rptr, struct rest_response * aptr)\n");
		fprintf(f,"{\n");
		fprintf(f,"\tstruct rest_header * hptr;\n");
		fprintf(f,"\tstruct occi_interface * iptr;\n");
		fprintf(f,"\tstruct occi_kind_node * nptr;\n");
		fprintf(f,"\tstruct occi_kind_node * sptr;\n");
		if(!flag)
		{
			fprintf(f,"\tstruct cords_%s * pptr;\n",categoryName);
			fprintf(f,"\tstruct cords_%s * fptr;\n",categoryName);
		}
		else
		{
			fprintf(f,"\tstruct %s * pptr;\n",categoryName);
			fprintf(f,"\tstruct %s * fptr;\n",categoryName);
		}
		fprintf(f,"\tiptr = optr->callback;\n");
		if(!flag) fprintf(f,"\tif (!( fptr = filter_cords_%s_info( optr, rptr, aptr ) ))\n",categoryName);
		else  fprintf(f,"\tif (!( fptr = filter_%s_info( optr, rptr, aptr ) ))\n",categoryName);
		fprintf(f,"\t\treturn( rest_html_response( aptr, 400, \"Bad Request\" ) );\n");
		if(!flag) fprintf(f,"\tnptr=cords_%s_first;\n",categoryName);
		else fprintf(f,"\tnptr=%s_first;\n",categoryName);
		fprintf(f,"\twhile (nptr != (struct occi_kind_node *) 0) {\n");
		fprintf(f,"\t\tif ((!( pptr = nptr->contents ))\n");
		if(!flag) fprintf(f,"\t\t||  (!( pass_cords_%s_filter( pptr, fptr ) ))) {\n",categoryName);
		else fprintf(f,"\t\t||  (!( pass_%s_filter( pptr, fptr ) ))) {\n",categoryName);
		fprintf(f,"\t\t\tnptr = nptr->next;\n");
		fprintf(f,"\t\t\tcontinue;\n");
		fprintf(f,"\t\t\t}\n");
		fprintf(f,"\t\telse	{\n");
		fprintf(f,"\t\t\tif (( iptr ) && (iptr->delete)) { (*iptr->delete)(optr,nptr); }\n");
		fprintf(f,"\t\t\tsptr = nptr->next;\n");
		if(!flag) fprintf(f,"\t\t\tdrop_cords_%s_node( nptr );\n",categoryName);
		else fprintf(f,"\t\t\tdrop_%s_node( nptr );\n",categoryName);
		fprintf(f,"\t\t\tnptr = sptr;\n");
		fprintf(f,"\t\t\t}\n");
		fprintf(f,"\t\t}\n");
		if(!flag) fprintf(f,"\tautosave_cords_%s_nodes();\n",categoryName);
		else fprintf(f,"\tautosave_%s_nodes();\n",categoryName);
		fprintf(f,"\tif (!( occi_success( aptr ) ))\n");
		fprintf(f,"\t\treturn( rest_response_status( aptr, 500, \"Server Failure\" ) );\n");
		fprintf(f,"\telse	return( rest_response_status( aptr, 200, \"OK\" ) );\n");
		fprintf(f,"}\n\n");

		fprintf(f,"/*	------------------------------------------------------------------------------	*/\n");
		fprintf(f,"/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t 	*/\n");
		fprintf(f,"/*	------------------------------------------------------------------------------	*/\n");
		if(!flag) fprintf(f,"private struct rest_response * occi_cords_%s_get(void * vptr, struct rest_client * cptr, struct rest_request * rptr)\n",categoryName);
		else fprintf(f,"private struct rest_response * occi_%s_get(void * vptr, struct rest_client * cptr, struct rest_request * rptr)\n",categoryName);
		fprintf(f,"{\n");
		fprintf(f,"\tstruct rest_response * aptr;\n");
		fprintf(f,"\tstruct rest_header   * hptr;\n");
		fprintf(f,"\tstruct occi_category * optr;\n");
		fprintf(f,"\tchar * ctptr;\n");
		fprintf(f,"\tchar * mptr;\n");
		fprintf(f,"\tif (!( hptr = rest_resolve_header( rptr->first, \"Content-Type\" ) ))\n");
		fprintf(f,"\t\tctptr = \"text/occi\";\n");
		fprintf(f,"\telse	ctptr = hptr->value;\n");
		fprintf(f,"\tif (!( optr = vptr ))\n"); 
		fprintf(f,"\t\treturn( rest_bad_request(vptr,cptr,rptr) );\n");
		fprintf(f,"\tif(!(aptr = rest_allocate_response( cptr )))\n");
		fprintf(f,"\t\treturn( aptr );\n");
		fprintf(f,"\telse if (!(strcmp( rptr->object, optr->location ) ))\n");
		if(!flag) fprintf(f,"\t\treturn( cords_%s_get_list( optr, cptr, rptr, aptr ) );\n",categoryName);
		else fprintf(f,"\t\treturn( %s_get_list( optr, cptr, rptr, aptr ) );\n",categoryName);
		fprintf(f,"\telse if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))\n");
		if(!flag) fprintf(f,"\t\treturn( cords_%s_get_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );\n",categoryName);
		else fprintf(f,"\t\treturn( %s_get_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );\n",categoryName);
		fprintf(f,"\telse	return( rest_html_response( aptr, 400, \"Bad Request\") );\n");
		fprintf(f,"}\n\n");

		fprintf(f,"/*	--------------------------------------------------------------------------------	*/\n");
		fprintf(f,"/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d 	*/\n");
		fprintf(f,"/*	--------------------------------------------------------------------------------	*/\n");
		if(!flag) fprintf(f,"private struct rest_response * occi_cords_%s_head(void * vptr, struct rest_client * cptr, struct rest_request * rptr)\n",categoryName);
		else fprintf(f,"private struct rest_response * occi_%s_head(void * vptr, struct rest_client * cptr, struct rest_request * rptr)\n",categoryName);
		fprintf(f,"{\n");
		fprintf(f,"\tstruct rest_response * aptr;\n");
		fprintf(f,"\tstruct rest_header   * hptr;\n");
		fprintf(f,"\tstruct occi_category * optr;\n");
		fprintf(f,"\tchar * ctptr;\n");
		fprintf(f,"\tchar * mptr;\n");
		fprintf(f,"\tif (!( hptr = rest_resolve_header( rptr->first, \"Content-Type\" ) ))\n");
		fprintf(f,"\t\tctptr = \"text/occi\";\n");
		fprintf(f,"\telse	ctptr = hptr->value;\n");
		fprintf(f,"\tif (!( optr = vptr )) \n");
		fprintf(f,"\t\treturn( rest_bad_request(vptr,cptr,rptr) );\n");
		fprintf(f,"\tif(!(aptr = rest_allocate_response( cptr )))\n");
		fprintf(f,"\t\treturn( aptr );\n");
		fprintf(f,"\telse if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))\n");
		if(!flag) fprintf(f,"\t\treturn( cords_%s_head_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );\n",categoryName);
		else fprintf(f,"\t\treturn( %s_head_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );\n",categoryName);
		fprintf(f,"\telse	return( rest_html_response( aptr, 400, \"Bad Request\") );\n");
		fprintf(f,"}\n\n");

		fprintf(f,"/*	--------------------------------------------------------------------------------	*/\n");
		fprintf(f,"/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t 	*/\n");
		fprintf(f,"/*	--------------------------------------------------------------------------------	*/\n");
		if(!flag) fprintf(f,"private struct rest_response * occi_cords_%s_post(void * vptr, struct rest_client * cptr, struct rest_request * rptr)\n",categoryName);
		else fprintf(f,"private struct rest_response * occi_%s_post(void * vptr, struct rest_client * cptr, struct rest_request * rptr)\n",categoryName);
		fprintf(f,"{\n");
		fprintf(f,"\tstruct rest_response * aptr;\n");
		fprintf(f,"\tstruct rest_header   * hptr;\n");
		fprintf(f,"\tstruct occi_category * optr;\n");
		fprintf(f,"\tchar * ctptr;\n");
		fprintf(f,"\tchar * mptr;\n");
		fprintf(f,"\tif (!( hptr = rest_resolve_header( rptr->first, \"Content-Type\" ) ))\n");
		fprintf(f,"\t\tctptr = \"text/occi\";\n");
		fprintf(f,"\telse	ctptr = hptr->value;\n");
		fprintf(f,"\tif (!( optr = vptr )) \n");
		fprintf(f,"\t\treturn( rest_bad_request(vptr,cptr,rptr) );\n");
		fprintf(f,"\tif(!(aptr = rest_allocate_response( cptr )))\n");
		fprintf(f,"\t\treturn( aptr );\n");
		fprintf(f,"\telse if (!( strcmp( rptr->object, optr->location ) ))\n");
		if(!flag) fprintf(f,"\t\treturn( cords_%s_post_item( optr, cptr, rptr, aptr ) );\n",categoryName);
		else fprintf(f,"\t\treturn( %s_post_item( optr, cptr, rptr, aptr ) );\n",categoryName);
		fprintf(f,"\telse if ( strncmp( rptr->object, optr->location,strlen(optr->location)) != 0)\n");
		fprintf(f,"\t\treturn( rest_html_response( aptr, 400, \"Bad Request\") );\n");
		fprintf(f,"\telse if (!( rptr->parameters ))\n");
		fprintf(f,"\t\treturn( rest_html_response( aptr, 400, \"Bad Request\") );\n");
		fprintf(f,"\telse if (!( strncmp( rptr->parameters, \"action=\", strlen(\"action=\")) ))\n");
		if(!flag)
		{
			fprintf(f,"\t\treturn( cords_%s_post_action( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );\n",categoryName);
			fprintf(f,"\telse if (!( strncmp( rptr->parameters, \"mixin=\", strlen(\"mixin=\")) ))\n");
			fprintf(f,"\t\treturn( cords_%s_post_mixin( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );\n",categoryName);
			fprintf(f,"\telse if (!( strncmp( rptr->parameters, \"link=\", strlen(\"link=\")) ))\n");
			fprintf(f,"\t\treturn( cords_%s_post_link( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );\n",categoryName);
		}
		else
		{
			fprintf(f,"\t\treturn( %s_post_action( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );\n",categoryName);
			fprintf(f,"\telse if (!( strncmp( rptr->parameters, \"mixin=\", strlen(\"mixin=\")) ))\n");
			fprintf(f,"\t\treturn( %s_post_mixin( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );\n",categoryName);
			fprintf(f,"\telse if (!( strncmp( rptr->parameters, \"link=\", strlen(\"link=\")) ))\n");
			fprintf(f,"\t\treturn( %s_post_link( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );\n",categoryName);

		}
		fprintf(f,"\telse	return( rest_html_response( aptr, 400, \"Bad Request\") );\n");
		fprintf(f,"}\n\n");

		fprintf(f,"/*	------------------------------------------------------------------------------	*/\n");
		fprintf(f,"/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p u t 	*/\n");
		fprintf(f,"/*	------------------------------------------------------------------------------	*/\n");
		if(!flag) fprintf(f,"private struct rest_response * occi_cords_%s_put(void * vptr, struct rest_client * cptr, struct rest_request * rptr)\n",categoryName);
		else fprintf(f,"private struct rest_response * occi_%s_put(void * vptr, struct rest_client * cptr, struct rest_request * rptr)\n",categoryName);
		fprintf(f,"{\n");
		fprintf(f,"\tstruct rest_response * aptr;\n");
		fprintf(f,"\tstruct rest_header   * hptr;\n");
		fprintf(f,"\tstruct occi_category * optr;\n");
		fprintf(f,"\tchar * ctptr;\n");
		fprintf(f,"\tchar * mptr;\n");
		fprintf(f,"\tif (!( hptr = rest_resolve_header( rptr->first, \"Content-Type\" ) ))\n");
		fprintf(f,"\t\tctptr = \"text/occi\";\n");
		fprintf(f,"\telse	ctptr = hptr->value;\n");
		fprintf(f,"\tif (!( optr = vptr )) \n");
		fprintf(f,"\t\treturn( rest_bad_request(vptr,cptr,rptr) );\n");
		fprintf(f,"\tif(!(aptr = rest_allocate_response( cptr )))\n");
		fprintf(f,"\t\treturn( aptr );\n");
		fprintf(f,"\telse if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))\n");
		if(!flag) fprintf(f,"\t\treturn( cords_%s_put_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );\n",categoryName);
		else fprintf(f,"\t\treturn( %s_put_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );\n",categoryName);
		fprintf(f,"\telse	return( rest_html_response( aptr, 400, \"Bad Request\") );\n");
		fprintf(f,"}\n\n");

		fprintf(f,"/*	------------------------------------------------------------------------------------	*/\n");
		fprintf(f,"/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e 	*/\n");
		fprintf(f,"/*	------------------------------------------------------------------------------------	*/\n");
		if(!flag) fprintf(f,"private struct rest_response * occi_cords_%s_delete(void * vptr, struct rest_client * cptr, struct rest_request * rptr)\n",categoryName);
		else fprintf(f,"private struct rest_response * occi_%s_delete(void * vptr, struct rest_client * cptr, struct rest_request * rptr)\n",categoryName);
		fprintf(f,"{\n");
		fprintf(f,"\tstruct rest_response * aptr;\n");
		fprintf(f,"\tstruct rest_header   * hptr;\n");
		fprintf(f,"\tstruct occi_category * optr;\n");
		fprintf(f,"\tchar * ctptr;\n");
		fprintf(f,"\tchar * mptr;\n");
		fprintf(f,"\tif (!( hptr = rest_resolve_header( rptr->first, \"Content-Type\" ) ))\n");
		fprintf(f,"\t\tctptr = \"text/occi\";\n");
		fprintf(f,"\telse	ctptr = hptr->value;\n");
		fprintf(f,"\tif (!( optr = vptr )) \n");
		fprintf(f,"\t\treturn( rest_bad_request(vptr,cptr,rptr) );\n");
		fprintf(f,"\tif(!(aptr = rest_allocate_response( cptr )))\n");
		fprintf(f,"\t\treturn( aptr );\n");
		fprintf(f,"\telse if (!(strcmp( rptr->object, optr->location ) ))\n");
		if(!flag)
		{
			fprintf(f,"\t\treturn( cords_%s_delete_all( optr, cptr, rptr, aptr ) );\n",categoryName);
			fprintf(f,"\telse if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))\n");
			fprintf(f,"\t\treturn( cords_%s_delete_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );\n",categoryName);
		}
		else
		{
			fprintf(f,"\t\treturn( %s_delete_all( optr, cptr, rptr, aptr ) );\n",categoryName);
			fprintf(f,"\telse if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))\n");
			fprintf(f,"\t\treturn( %s_delete_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );\n",categoryName);
		}
		fprintf(f,"\telse	return( rest_html_response( aptr, 400, \"Bad Request\") );\n");
		fprintf(f,"}\n\n");

		fprintf(f,"/*	--------------------------------------------------------------------------------------*/\n");
		fprintf(f,"/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   r e d i r e c t i o n       */\n");
		fprintf(f,"/*	--------------------------------------------------------------------------------------*/\n");

		if(!flag)
		{
			fprintf(f,"private void	redirect_occi_cords_%s_mt( struct rest_interface * iptr )\n",categoryName);
			fprintf(f,"{\n");
			fprintf(f,"\tiptr->get = occi_cords_%s_get;\n",categoryName);
			fprintf(f,"\tiptr->post = occi_cords_%s_post;\n",categoryName);
			fprintf(f,"\tiptr->put = occi_cords_%s_put;\n",categoryName);
			fprintf(f,"\tiptr->delete = occi_cords_%s_delete;\n",categoryName);
			fprintf(f,"\tiptr->head = occi_cords_%s_head;\n",categoryName);
		}
		else
		{
			fprintf(f,"private void	redirect_occi_%s_mt( struct rest_interface * iptr )\n",categoryName);
			fprintf(f,"{\n");
			fprintf(f,"\tiptr->get = occi_%s_get;\n",categoryName);
			fprintf(f,"\tiptr->post = occi_%s_post;\n",categoryName);
			fprintf(f,"\tiptr->put = occi_%s_put;\n",categoryName);
			fprintf(f,"\tiptr->delete = occi_%s_delete;\n",categoryName);
			fprintf(f,"\tiptr->head = occi_%s_head;\n",categoryName);
		}
		fprintf(f,"\treturn;\n");
		fprintf(f,"}\n\n");
		
                fprintf(f,"/*	------------------------------------	*/\n");
		fprintf(f,"/*	c r u d   d e l e t e   a c t i o n 	*/\n");
		fprintf(f,"/*	------------------------------------	*/\n");
		if(!flag) fprintf(f,"private struct rest_response * delete_action_cords_%s(struct occi_category * optr,\n",categoryName); 
                else fprintf(f,"private struct rest_response * delete_action_%s(struct occi_category * optr,\n",categoryName); 
		fprintf(f,"struct rest_client * cptr,\n");  
		fprintf(f,"struct rest_request * rptr,\n");  
		fprintf(f,"struct rest_response * aptr,\n");  
		fprintf(f,"void * vptr )\n");
		fprintf(f,"{\n");
		fprintf(f,"\taptr = liberate_rest_response( aptr );\n");
		if(!flag) fprintf(f,"\treturn( occi_cords_%s_delete(optr,cptr,rptr));\n",categoryName);
                else fprintf(f,"\treturn( occi_%s_delete(optr,cptr,rptr));\n",categoryName);
 		fprintf(f,"}\n");

		fprintf(f,"/*	------------------------------------------	*/\n");
		fprintf(f,"/*	o c c i   c a t e g o r y   b u i l d e r 	*/\n");
		fprintf(f,"/*	------------------------------------------	*/\n");
		if(!flag) fprintf(f,"/* occi category rest instance builder for : occi_cords_%s */\n",categoryName);
		else fprintf(f,"/* occi category rest instance builder for : occi_%s */\n",categoryName);
		fprintf(f,"public struct occi_category * occi_cords_%s_builder(char * a,char * b) {\n",categoryName);
		fprintf(f,"\tchar * c=\"http://scheme.compatibleone.fr/scheme/compatible#\";\n");
		fprintf(f,"\tchar * d=\"kind\";\n");
		fprintf(f,"\tchar * e=\"http://scheme.ogf.org/occi/resource#\";\n");
		if(!flag) fprintf(f,"\tchar * f=\"CompatibleOne OCCI resource cords_%s\";\n",categoryName);
		else fprintf(f,"\tchar * f=\"CompatibleOne OCCI resource %s\";\n",categoryName);
		fprintf(f,"\tstruct occi_category * optr;\n");
		fprintf(f,"\tif (!( optr = occi_create_category(a,b,c,d,e,f) )) { return(optr); }\n");
		fprintf(f,"\telse {\n");
		if(!flag) fprintf(f,"\t\tredirect_occi_cords_%s_mt(optr->interface);\n",categoryName);
		else fprintf(f,"\t\tredirect_occi_%s_mt(optr->interface);\n",categoryName);
		for(i=1;i<dim;i++)
		{
			fprintf(f,"\t\tif (!( optr = occi_add_attribute(optr, \"%s\",0,0) ))\n",nameAtr[i]);
			fprintf(f,"\t\t\treturn(optr);\n");
		}
		if(!flag)fprintf(f,"\t\tif(!( optr = occi_add_action( optr, \"DELETE\",\"\",delete_action_cords_%s)))\n",categoryName);
		else fprintf(f,"\t\tif(!( optr = occi_add_action( optr, \"DELETE\","",delete_action_%s)))\n",categoryName);
		fprintf(f,"\t\t\treturn (optr);\n");
		if(!flag) fprintf(f,"\t\tautoload_cords_%s_nodes();\n",categoryName);
		else fprintf(f,"\t\tautoload_%s_nodes();\n",categoryName);
		fprintf(f,"\t\t\treturn(optr);\n");
		fprintf(f,"\t}\n");
		fprintf(f,"}\n\n");

		fprintf(f,"/*	------------------------------------------------	*/\n");
		if(!flag) fprintf(f,"/*	c o r d s _ %s _ o c c i _ h e a d e r s 	*/\n",categoryName);
		else fprintf(f,"/*	%s _ o c c i _ h e a d e r s 	*/\n",categoryName);
		fprintf(f,"/*	------------------------------------------------	*/\n");
		if(!flag) fprintf(f,"public struct rest_header *  cords_%s_occi_headers(struct cords_%s * sptr)\n",categoryName,categoryName);
		else fprintf(f,"public struct rest_header *  %s_occi_headers(struct %s * sptr)\n",categoryName,categoryName);
		fprintf(f,"{\n");
		fprintf(f,"\tstruct rest_header * first=(struct rest_header *) 0;\n");
		fprintf(f,"\tstruct rest_header * last=(struct rest_header *) 0;\n");
		fprintf(f,"\tstruct rest_header * hptr=(struct rest_header *) 0;\n");
		fprintf(f,"\tchar buffer[8192];\n");
		fprintf(f,"\tif (!( sptr )) return(0);\n");
		fprintf(f,"\tif (!( hptr = allocate_rest_header()))\n");
		fprintf(f,"\t\treturn(hptr);\n");
		fprintf(f,"\t\telse	if (!( hptr->previous = last))\n");
		fprintf(f,"\t\t\tfirst = hptr;\n");
		fprintf(f,"\t\telse	hptr->previous->next = hptr;\n");
		fprintf(f,"\t\tlast = hptr;\n");
		fprintf(f,"\tif (!( hptr->name = allocate_string(\"Category\")))\n");
		fprintf(f,"\t\treturn(first);\n");
		if(!flag) fprintf(f,"\tsprintf(buffer,\"cords_%s; scheme='http://scheme.compatibleone.fr/scheme/compatible#'; class='kind';\\r\\n\");\n",categoryName);
		else fprintf(f,"\tsprintf(buffer,\"%s; scheme='http://scheme.compatibleone.fr/scheme/compatible#'; class='kind';\\r\\n\");\n",categoryName);
		fprintf(f,"\tif (!( hptr->value = allocate_string(buffer)))\n");
		fprintf(f,"\t\treturn(first);\n");
		fprintf(f,"\tif (!( hptr = allocate_rest_header()))\n");
		fprintf(f,"\t\treturn(first);\n");
		fprintf(f,"\t\telse	if (!( hptr->previous = last))\n");
		fprintf(f,"\t\t\tfirst = hptr;\n");
		fprintf(f,"\t\telse	hptr->previous->next = hptr;\n");
		fprintf(f,"\t\tlast = hptr;\n");
		fprintf(f,"\tif (!( hptr->name = allocate_string(\"X-OCCI-Attribute\")))\n");
		fprintf(f,"\t\treturn(first);\n");
		for(i=1;i<dim-1;i++)
		{
			if(!flag) fprintf(f,"\tsprintf(buffer,\"occi.cords_%s.%s='%%s'\\r\\n\",(sptr->%s?sptr->%s:\"\"));\n",categoryName,nameAtr[i],nameAtr[i],nameAtr[i]);
			else fprintf(f,"\tsprintf(buffer,\"occi.%s.%s='%%s'\\r\\n\",(sptr->%s?sptr->%s:\"\"));\n",categoryName,nameAtr[i],nameAtr[i],nameAtr[i]);
			fprintf(f,"\tif (!( hptr->value = allocate_string(buffer)))\n");
			fprintf(f,"\t\treturn(first);\n");
			fprintf(f,"\tif (!( hptr = allocate_rest_header()))\n");
			fprintf(f,"\t\treturn(first);\n");
			fprintf(f,"\t\telse	if (!( hptr->previous = last))\n");
			fprintf(f,"\t\tfirst = hptr;\n");
			fprintf(f,"\t\telse	hptr->previous->next = hptr;\n");
			fprintf(f,"\t\tlast = hptr;\n");
			fprintf(f,"\tif (!( hptr->name = allocate_string(\"X-OCCI-Attribute\")))\n");
			fprintf(f,"\t\treturn(first);\n");
		}
		if(dim>0)
		{
			if(!flag) fprintf(f,"\tsprintf(buffer,\"occi.cords_%s.%s='%%s'\\r\\n\",(sptr->%s?sptr->%s:\"\"));\n",categoryName,nameAtr[dim-1],nameAtr[dim-1],nameAtr[i]);
			else fprintf(f,"\tsprintf(buffer,\"occi.%s.%s='%%s'\\r\\n\",(sptr->%s?sptr->%s:\"\"));\n",categoryName,nameAtr[dim-1],nameAtr[dim-1],nameAtr[i]);
			fprintf(f,"\tif (!( hptr->value = allocate_string(buffer)))\n");
			fprintf(f,"\t\treturn(first);\n");
			fprintf(f,"\treturn(first);\n");
		}
		fprintf(f,"}\n\n");
		fprintf(f,"#endif	/* _%s_c_ */\n",categoryName);
		fclose(f);
		free(nameAtr);

		return 1;
 	}
 
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/* Function to generate category.c file                                                                                    */
/* categoryName:(char*) the name of the category                                                                           */
/* categoryAttributes: list of attributes                                                                                  */
/* dim:(int) number of attributes                                                                                          */
/* pathf:(char*) file name                                                                                                 */
/*-------------------------------------------------------------------------------------------------------------------------*/
int createJCategoryCordsCfile(char *categoryName, listc categoryAttributes, int dim, int flag, char pathf[])
{
	char pathfc[1024];
	char pathfcc[1024];
	char **nameAtr; 
	FILE *f;
	int i;

	str_sub(pathf,0,strlen(pathf)-3,pathfc);
	sprintf(pathfcc,"%s.c",pathfc);
	nameAtr = (char**) malloc(sizeof (char*) * dim);
	for(i = 0; i < dim; i++)
	nameAtr[i] = (char*) malloc(sizeof (char) * DIM);

	enJTete(pathfcc);
	i=0;
	elem *pelem = categoryAttributes.first;    
	while(pelem)
	{       
		strcpy(nameAtr[i],pelem->value);     
		pelem = pelem->next; 
		i++;   
	}
	

	if((f=fopen(pathfcc,"a"))==NULL)
	{
		printf("Error create category file C: No such file or directory\n");
		return 0;
	}
	else
	{
		fprintf(f,"\n");
		fprintf(f,"#ifndef _%s_c_\n",categoryName);
		fprintf(f,"#define _%s_c_\n",categoryName);
		fprintf(f,"#include \"element.h\"\n");
		fprintf(f,"#include \"%s.h\"\n\n",categoryName);

		/*---------------------------*/
		/*liberate category function */
		/*---------------------------*/
		fprintf(f,"/*----------------------------------------------------------------------------------------*/\n");
		if(!flag) fprintf(f,"/*                               Liberate_cords_%s       */\n",categoryName);
		else fprintf(f,"/*                               Liberate_%s\n       */",categoryName);
		fprintf(f,"/*----------------------------------------------------------------------------------------*/\n");
		if(!flag) fprintf(f,"public struct cords_%s * liberate_cords_%s(struct cords_%s * sptr)\n",categoryName,categoryName,categoryName);
		else fprintf(f,"public struct %s * liberate_%s(struct %s * sptr)\n",categoryName,categoryName,categoryName);

		fprintf(f,"{\n");
		fprintf(f,"\tif( sptr )\n");
		fprintf(f,"\t{\n");
		for(i=0; i<dim; i++)
		{
		fprintf(f,"\t\tif(sptr->%s)\n",nameAtr[i]);
		fprintf(f,"\t\t\tsptr->%s = liberate(sptr->%s);\n",nameAtr[i],nameAtr[i]);
		}
		fprintf(f,"\t\tsptr = liberate(sptr);\n");
		fprintf(f,"\t}\n");
		if(!flag) fprintf(f,"\treturn((struct cords_%s * )0);\n",categoryName);
		else fprintf(f,"\treturn((struct %s * )0);\n",categoryName);
		fprintf(f,"}\n\n");

		/*------------------------*/
		/*reset category function */
		/*------------------------*/
		fprintf(f,"/*-----------------------------------------------------------------------------------------*/\n");
		if(!flag) fprintf(f,"/*                            Reset_cords_%s   */\n",categoryName);
		else fprintf(f,"/*                            Reset_%s   */\n",categoryName);
		fprintf(f,"/*-----------------------------------------------------------------------------------------*/\n");
		if(!flag) fprintf(f,"public struct cords_%s * reset_cords_%s(struct cords_%s * sptr)\n",categoryName,categoryName,categoryName);
		else fprintf(f,"public struct %s * reset_%s(struct %s * sptr)\n",categoryName,categoryName,categoryName);
		fprintf(f,"{\n");
		fprintf(f,"\tif( sptr )\n");
		fprintf(f,"\t{\n");
		for(i=0;i<dim; i++)
		{
			fprintf(f,"\t\tsptr->%s = (char*) 0;\n",nameAtr[i]);
		}
		fprintf(f,"\t}\n");
		fprintf(f,"\treturn (sptr);\n");
		fprintf(f,"}\n\n");

		/*----------------------------*/
		/*allocate category function  */
		/*----------------------------*/
		fprintf(f,"/*-----------------------------------------------------------------------------------------*/\n");
		if(!flag)fprintf(f,"/*                            Allocate_cords_%s     */\n",categoryName);
		else fprintf(f,"/*                            Allocate_%s     */\n",categoryName);
		fprintf(f,"/*-----------------------------------------------------------------------------------------*/\n");
		if(!flag) fprintf(f,"public struct cords_%s * allocate_cords_%s()\n",categoryName,categoryName);
		else fprintf(f,"public struct %s * allocate_%s()\n",categoryName,categoryName);
		fprintf(f,"{\n");
		if(!flag) 
		{
			fprintf(f,"\tstruct cords_%s * sptr;\n",categoryName);
			fprintf(f,"\tif (!( sptr = allocate( sizeof( struct cords_%s ) ) ))\n",categoryName);
		}
		else 
		{
			fprintf(f,"\tstruct %s * sptr;\n",categoryName);
			fprintf(f,"\tif (!( sptr = allocate( sizeof( struct %s ) ) ))\n",categoryName);
		}
		fprintf(f,"\t\treturn( sptr );\n");
		if(!flag) fprintf(f,"\telse  return( reset_cords_%s(sptr) );\n",categoryName);
		else fprintf(f,"\telse  return( reset_%s(sptr) );\n",categoryName);
		fprintf(f,"}\n\n");

		/*--------------------------*/ 
		/*xmlin cords category      */
		/*--------------------------*/
		fprintf(f,"/*------------------------------------------------------------------------------------------*/\n");
		if(!flag) fprintf(f,"/*                             Xmlin_cords_%s      */\n",categoryName);
		else fprintf(f,"/*                             Xmlin_%s      */\n",categoryName);
		fprintf(f,"/*------------------------------------------------------------------------------------------*/\n");
		if(!flag) fprintf(f,"public int xmlin_cords_%s(struct cords_%s * sptr,struct xml_element * eptr)\n",categoryName,categoryName);
		else fprintf(f,"public int xmlin_%s(struct %s * sptr,struct xml_element * eptr)\n",categoryName,categoryName);
		fprintf(f,"{\n");
		fprintf(f,"\tstruct xml_element * wptr;\n");
		fprintf(f,"\tif (!( eptr )) return(0);\n");
		fprintf(f,"\tif (!( sptr )) return(0);\n");
		fprintf(f,"\tfor ( wptr=eptr->first; wptr != (struct xml_element *) 0; wptr=wptr->next)\n");
		fprintf(f,"\t{\n");
		fprintf(f,"\t\tif (!( strcmp(wptr->name,\"%s\") ))\n",nameAtr[0]);
		fprintf(f,"\t\t{\n");
		fprintf(f,"\t\t\tif ( wptr->value ) { sptr->%s = allocate_string(wptr->value); }\n",nameAtr[0]);
		fprintf(f,"\t\t}\n");

		for(i=1;i<dim;i++)
		{
			fprintf(f,"\t\telse if (!( strcmp(wptr->name,\"%s\") ))\n",nameAtr[i]);
			fprintf(f,"\t\t{\n");
			fprintf(f,"\t\t\tif ( wptr->value ) { sptr->%s = allocate_string(wptr->value); }\n",nameAtr[i]);
			fprintf(f,"\t\t}\n");
		} 
		fprintf(f,"\t}\n");
		fprintf(f,"\treturn(0);\n");
		fprintf(f,"}\n\n");

		/*----------------------------*/
		/*reset occi_cords_ category  */
		/*----------------------------*/
		fprintf(f,"/*---------------------------------------------------------------------------------------*/\n");
		if(!flag) fprintf(f,"/*                         Rest_occi_cords_%s  */\n",categoryName);
		else fprintf(f,"/*                         Rest_occi_%s  */\n",categoryName);   
		fprintf(f,"/*---------------------------------------------------------------------------------------*/\n");
		if(!flag) fprintf(f,"public int rest_occi_cords_%s(FILE * fh, struct cords_%s * sptr,char * prefix, char * nptr)\n",categoryName,categoryName);
		else fprintf(f,"public int rest_occi_%s(FILE * fh, struct %s * sptr,char * prefix, char * nptr)\n",categoryName,categoryName);
		fprintf(f,"{\n");
		fprintf(f,"\tstruct xml_element * wptr;\n");
		fprintf(f,"\tif (!( sptr )) return(0);\n");
		fprintf(f,"\tfprintf(fh,\"POST /%%s/ HTTP/1.1\\r\\n\",nptr);\n");
		fprintf(f,"\tfprintf(fh,\"Category: %%s; scheme='http://scheme.%%s.org/occi/%%s#'; class='kind';\\r\\n\",nptr,prefix,prefix);\n");
		for(i=0;i<dim;i++)
		{
			fprintf(f,"\tfprintf(fh,\"X-OCCI-Attribute: %%s.%%s.%s='%%s'\\r\\n\",prefix,nptr,(sptr->%s?sptr->%s:\"\"));\n",nameAtr[i],nameAtr[i],nameAtr[i]);
		}
		fprintf(f,"\treturn(0);\n");
		fprintf(f,"}\n\n");
		fprintf(f,"#endif\n");
		fclose(f);
		free(nameAtr);
		return 1;
	}
}


int  enJTete(char pathf[])
{
 	FILE *f;
 	if((f=fopen(pathf,"w+"))==NULL)
 	{
  		printf("Error create category file C: No such file or directory\n");
  		return 0;
	 }
 	else
 	{
		  fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
		  fprintf(f,"/* ACCORDS PLATFORM                                                              */\n");
		  fprintf(f,"/* copyright 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>         */\n");
		  fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
		  fprintf(f,"/* Licensed under the Apache License, Version 2.0 (the \"License\");               */\n");
		  fprintf(f,"/* you may not use this file except in compliance with the License.              */\n");
		  fprintf(f,"/* You may obtain a copy of the License at                                       */\n");
		  fprintf(f,"/*                                                                               */\n");
		  fprintf(f,"/*       http://www.apache.org/licenses/LICENSE-2.0                              */\n");
		  fprintf(f,"/*                                                                               */\n");
		  fprintf(f,"/* Unless required by applicable law or agreed to in writing, software           */\n");
		  fprintf(f,"/* distributed under the License is distributed on an \"AS IS\" BASIS,             */\n");
		  fprintf(f,"/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.      */\n");
		  fprintf(f,"/* See the License for the specific language governing permissions and           */\n");
		  fprintf(f,"/* limitations under the License.                                                */\n");
		  fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
		  fprintf(f,"\n\n/* jaccordsSDK (Hamid Medjahed Prologue <hmedjahed@prologue.fr>:                 */\n");
		  fprintf(f,"/* This has been generated and should not be modified by hand                    */\n");
		  fclose(f);
 	}
 	return 1;
}


/*---------------------------------------------------------------------------------------------------------------*/
/* Function to gerate a java module for accords platform                                                         */
/* moduleName:(char*) the name of the module                                                                     */
/* categoryNameList: (char*) the list of categories handled by this module                                       */ 
/* pathf: (char*) path name of the project directory                                                             */
/* return 1 if succeeded                                                                                         */
/*---------------------------------------------------------------------------------------------------------------*/
int generateJModuleFile(char * moduleName, char * categoryNameList)
{
   	char pathfd[DIM];
   	char pathff[DIM];
  	 char pathf[1024];
  	 FILE *f;
   
  	 strcpy(pathf,JPATH);
  	 sprintf(pathfd,"%s/%s",pathf,moduleName);
   	if(mkdir(pathfd,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)==-1)
   	{
    		printf("Error in generateModuleFile: No such directory %s\n",pathfd);
    		return 0;
   	}
   	else
   	{
     		sprintf(pathff,"%s/J%s.java",pathfd,moduleName);
     		if((f=fopen(pathff,"w"))==NULL)
     		{
       			printf("Error generatModuleFile : No such file or directory: %s\n",pathff);
       			return 0;
     		}	
     		else
     		{
			fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
   			fprintf(f,"/* ACCORDS Platform                                                              */\n"); 
    			fprintf(f,"/* copyright 2012, Hamid MEDJAHE (hmedjahed@prologue.fr)    Prologue             */\n");
    			fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
    			fprintf(f,"/* Licensed under the Apache License, Version 2.0 (the \"License\");               */\n");
    			fprintf(f,"/* you may not use this file except in compliance with the License.              */\n");
    			fprintf(f,"/* You may obtain a copy of the License at                                       */\n");
    			fprintf(f,"/*                                                                               */\n");
    			fprintf(f,"/*       http://www.apache.org/licenses/LICENSE-2.0                              */\n");
    			fprintf(f,"/*                                                                               */\n");
    			fprintf(f,"/* Unless required by applicable law or agreed to in writing, software           */\n");
    			fprintf(f,"/* distributed under the License is distributed on an \"AS IS\" BASIS,             */\n");
    			fprintf(f,"/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.      */\n");
    			fprintf(f,"/* See the License for the specific language governing permissions and           */\n");
    			fprintf(f,"/* limitations under the License.                                                */\n");
    			fprintf(f,"/*-------------------------------------------------------------------------------*/\n");
                	fprintf(f,"\n\n/* jaccordsSDK: This code has been generated by jaccordsSDK                      */\n"); 
                
                	fprintf(f,"public class J%s{\n",moduleName);
   			fprintf(f,"\tstatic{\n");
			fprintf(f,"\t\ttry{\n");
			fprintf(f,"\t\t\tSystem.loadLibrary(\"JcompDev\");\n"); 
			fprintf(f,"\t\t}catch( UnsatisfiedLinkError e){\n");
			fprintf(f,"\t\t\tSystem.out.println(\"Could not load library JcompDEV, add it to LD_LIBRARY_PATH?\");\n");
			fprintf(f,"\t\t\tSystem.exit(1);\n");
			fprintf(f,"\t\t}\n");
			fprintf(f,"\t}\n\n");	 
                	fprintf(f,"\tpublic static void main(String[] argv){\n");
			fprintf(f,"\t\tJcompDev comp = new JcompDev();\n");
                	fprintf(f,"\t\tcomp.launchModule(argv.length,argv,\"%s\",\"%s\");\n",moduleName,categoryNameList);
			fprintf(f,"\t}\n");
			fprintf(f,"}\n");
      			fclose(f);
      			return 1;
     		}

   	}	
}

