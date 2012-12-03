/*-------------------------------------------------------------------------------*/
/* Accords Platform                                                              */
/* copyright 2012 ,Hamid MEDJAHE (hmedjahed@prologue.fr)    Prologue             */
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
 
#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"
#include "../jcompmgr/jlist.h"
#include "../../jaccords/jsrc/jlistaction.h"
#include "../../jaccords/jsrc/jcrudinterf.h"
#include "../../jaccords/jsrc/jcategaction.h"
#include "../../jaccords/jsrc/jcategactionname.h"
#include "../../jaccords/jsrc/jcategactionnumber.h"
#include "../../jaccords/jsrc/jcategaccess.h"
#include "../../jaccords/jsrc/jlistoccibuilder.h"

struct accords_configuration moduleConfig;
public	int	check_debug();
public	int	check_verbose();
public	char *	default_publisher();		
public	char *	default_operator();
public	char *	default_tls();
public	char *	default_zone();
public	int	failure( int e, char * m1, char * m2 );
private	int banner();
private	struct rest_server * module_initialise(  void * v,struct rest_server * sptr );
private	int module_authorise(void * v, struct rest_client * cptr, char * username, char * password, struct accords_configuration *componentModule);
private	struct rest_extension * module_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr);
private	int	module_operation( char * nptr, struct accords_configuration *componentModule, char *moduleName, listc categoryName);
private	int	module(int argc, char * argv[],char *moduleName,char * categoryList);

void fillInAccordsConfiguration(struct accords_configuration *componentModule, char *moduleName)
{
        char configFile[1024];
        sprintf(configFile,"%s.xml",moduleName);
        componentModule->monitor = 0;                            /* monitoring information options       */
        componentModule->autopub = 0;                            /* auto publication mechanisms          */
        componentModule->verbose = 0;                            /* activate verbose messages            */
        componentModule->debug = 0;                              /* activate debug messages              */
        componentModule->threads = 0;                            /* allow use of threads                 */
        componentModule->ipv6 = 0;

        componentModule->tls=(char *) 0;                       /* security configuration filename      */
        componentModule->identity=(char *) 0;                  /* identity for publication of service  */
        componentModule-> publisher=_CORDS_DEFAULT_PUBLISHER;  /* identity of publication service      */
        componentModule->operator=_CORDS_DEFAULT_OPERATOR;     /* service provider operator identity   */
        componentModule->user=_CORDS_DEFAULT_USER;             /* service user name                    */
        componentModule->password=_CORDS_DEFAULT_PASSWORD;     /* authentication code                  */
        componentModule->resthost="http";                      /* root name of host                    */
        componentModule->restport=0;                           /* server tcp port                      */
        componentModule->chathost="xmpp";;                     /* xmpp chat host url                   */
        componentModule->chatport=8000;                        /* xmpp chat host prot                  */
        componentModule->domain="domain";                      /* category domain                      */
        componentModule->config=configFile;                    /* configuration file                   */
        componentModule->zone="europe";
	componentModule->storage = "storage";
        componentModule->firstcat=(struct occi_category *) 0;
        componentModule->lastcat=(struct  occi_category *) 0;

}

public	int	check_debug()
{
	return(moduleConfig.debug);		
}

public	int	check_verbose()		
{	
        return(moduleConfig.verbose);		
}

public	char *	default_publisher()	
{
 	return(moduleConfig.publisher);		
}

public	char *	default_operator()	
{	
        return(moduleConfig.operator);		
}

public	char *	default_tls()		
{	
        return(moduleConfig.tls);		
}

public	char *	default_zone()
{	
   return(moduleConfig.zone);		
}

public	int	failure( int e, char * m1, char * m2 )
{
	if ( e )
	{
		printf("\n*** failure %u",e);
		if ( m1 )
			printf(" : %s",m1);
		if ( m2 )
			printf(" : %s",m2);
		printf(" **`\n");
	}
	return( e );
}

/*-----------------------------------------------------------------------------------*/
#include "occibuilder.h"

private	int banner()
{
	printf("\n   CompatibleOne Elasticity Services COES : Version 1.0a.0.03");
	printf("\n   Beta Version : 28/11/2011");
	printf("\n   Copyright (c) 2011 Iain James Marshall, Prologue");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*------------------------------------------------------------------------------------*/
/*			module_ i n i t i a l i s e			              */
/*------------------------------------------------------------------------------------*/
private	struct rest_server * module_initialise(  void * v,struct rest_server * sptr )
{
	struct	rest_extension * xptr;
	if (!( xptr = rest_add_extension( sptr ) ))
		return((struct rest_server *) 0);
	else
	{
		xptr->net = (struct connection *) 0;
		return( sptr );
	}
}

/*------------------------------------------------------------------------------------*/
/*			module _ a u t h o r i s e 			              */
/*------------------------------------------------------------------------------------*/
private	int module_authorise(
             void * v,
             struct rest_client * cptr, 
             char * username, 
             char * password, 
             struct accords_configuration *componentModule)
{
	if ( strcmp( username, componentModule->user ) )
		return(0);
	else if ( strcmp( password, componentModule->password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*--------------------------------------------------------------------------------------------*/
/*			module _ e x t e n s i o n 			                      */
/*--------------------------------------------------------------------------------------------*/
private	struct rest_extension * module_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*---------------------------------------------------------------------------------------------*/
/* Function for category access type                                                           */
/*---------------------------------------------------------------------------------------------*/
int callocciCategoryAccess(const char *name)
{
	int i;
	for (i=0; i<= (sizeof(occiCategoryAccess_map)/ sizeof(occiCategoryAccess_map[0])); i++)
	{
		if(!strcmp(occiCategoryAccess_map[i].name, name))
		{
		  return occiCategoryAccess_map[i].access;
		}
	}
        
	return 0;
}
/*---------------------------------------------------------------------------------------------*/
/*      Function to call occi category action for the category specified in name variable      */
/*---------------------------------------------------------------------------------------------*/
struct rest_response * callocciCategoryAction(const char *name)
{
  	int i;
  	for (i = 0; i <= (sizeof(occiCategoryAction_map) / sizeof(occiCategoryAction_map[0])); i++) 
   	{
     		if (!strcmp(occiCategoryAction_map[i].name, name)) 
     		{
       			return occiCategoryAction_map[i].func;
     		}
  	}

  	return (struct rest_response *) 0;
}

/*---------------------------------------------------------------------------------------------*/
/*      Function to get category action number for the category specified in name variable     */
/*---------------------------------------------------------------------------------------------*/
int callocciCategoryActionNumber(const char *name)
{
  	int i;
   	for (i = 0; i <= (sizeof(occiCategoryActionNumber_map) / sizeof(occiCategoryActionNumber_map[0])); i++) 
   	{
    		if (!strcmp(occiCategoryActionNumber_map[i].name, name)) 
     		{
       			return occiCategoryActionNumber_map[i].func();
     		}
  	}

  	return 0;
}

/*---------------------------------------------------------------------------------------------*/
/*      Function get  category action name for the category specified in name variable         */
/*---------------------------------------------------------------------------------------------*/
char * callocciCategoryActionName(const char *name, int a)
{
  	int i;
   	for (i = 0; i <= (sizeof(occiCategoryActionName_map) / sizeof(occiCategoryActionName_map[0])); i++) 
   	{
     		if (!strcmp(occiCategoryActionName_map[i].name, name) && occiCategoryActionName_map[i].func) 
     		{
       			return occiCategoryActionName_map[i].func(a);
     		}
  	}

  	return (char *) 0;
}

/*--------------------------------------------------------------------------------------------*/
/*      Function to call occi category builder for the category specified in name variable    */
/*--------------------------------------------------------------------------------------------*/
struct occi_category * callocciCategoryBuilder(const char *name, char *a,char *b)
{
  	int i;
   	for (i = 0; i <= (sizeof(occiCategoryBuilder_map) / sizeof(occiCategoryBuilder_map[0])); i++) 
   	{
     		if (!strcmp(occiCategoryBuilder_map[i].name, name) && occiCategoryBuilder_map[i].func) 
     		{
       			return occiCategoryBuilder_map[i].func(a,b);
     		}
  	}

  	return (struct occi_category *) 0;
}

/*-------------------------------------------------------------------------------------------*/
/* Function to call occi category interface for the category indicated in name variable      */
/*-------------------------------------------------------------------------------------------*/
struct occi_interface * callocciCategoryInterface(const char *name)
{
  	int i;
  	for (i=0; i<= (sizeof(occiCategoryInterface_map)/ sizeof(occiCategoryInterface_map[0])); i++)
  	{
    		if(!strcmp(occiCategoryInterface_map[i].name,name))
    		{	
      			return occiCategoryInterface_map[i].interface_Func();
    		}
  	}
  	return 0;
}


/*---------------------------------------------------------------------------------------------*/
/*			module _ o p e r a t i o n				               */
/*---------------------------------------------------------------------------------------------*/
private	int	module_operation( 
                 char * nptr, 
                 struct accords_configuration *componentModule, 
                 char *moduleName, 
                 listc categoryName)
{
	char xlinkModule[1024];
	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;
        
        int i;
        struct occi_interface categoryInterface;
        int numOfact; 
        char categoryAction[1024];
	char packetfile[256];
        char *actionName;
        int categoryAccess=0;

        elem *pelem = categoryName.first;        
        sprintf(xlinkModule,"links_%s.xml",moduleName);
        xlinkModule[strlen(xlinkModule)]=0;
	set_autosave_cords_xlink_name(xlinkModule);
	sprintf(packetfile,"packet_%s.xml",moduleName);
 
        while(pelem)
        {       
          

	  	if (!( optr = callocciCategoryBuilder( pelem->value,componentModule->domain,pelem->value) ))
		   	return( 27 );
	  	else if (!( optr->previous = last ))
			first = optr;
	  	else	optr->previous->next = optr;
	  	last = optr;
	  	optr->callback = callocciCategoryInterface(pelem->value);

          	if(!(strcmp(pelem->value, "packet")))
		{
			set_autosave_cords_packet_name(packetfile);
		}
                categoryAccess = callocciCategoryAccess(pelem->value);
                if(categoryAccess)
		{
			optr->access = categoryAccess;
		}
		

          	numOfact = callocciCategoryActionNumber(pelem->value);
          	for(i=0;i<numOfact;i++)
          	{
               		actionName = allocate_string(callocciCategoryActionName(pelem->value,i));
               		sprintf(categoryAction,"%s_%s",pelem->value,actionName);
               		if (!( optr = occi_add_action( optr,actionName,"",callocciCategoryAction(categoryAction))))
		   		return( 27 );
          	}  
           
           	pelem=pelem->next;  
        } 
	
        rest_initialise_log( componentModule->monitor );

	if (!( componentModule->identity ))
        {
		return( occi_server(  nptr, componentModule->restport, componentModule->tls, componentModule->threads, first, (char *) 0 ) );
        }
	else
	{
		initialise_occi_publisher( componentModule->publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			componentModule->user, componentModule->password,
			componentModule->identity,  nptr, 
			componentModule->restport, componentModule->tls, 
			componentModule->threads, first ) );
	}
    
}

/*	------------------------------------------------------------------	*/
/*				Module 				        	*/
/*	------------------------------------------------------------------	*/
private	int	module(int argc, char * argv[], char *moduleName, char * categoryList)
{
	int	status=0;
	int	argi=0;
	char *	aptr;
        char *  token;
        listc  categoryLst;
      
        if ( argc == 1 )
        {
		return( banner() );
        }
        else
        {
  
        	resetList(&categoryLst);
           	token= strtok(categoryList," ");
           	for(; token != NULL ;)
           	{
              		addBack(&categoryLst,token);
              		token=strtok(NULL, " ");
           	} 

           	//loud configuration module
           	fillInAccordsConfiguration( &moduleConfig,moduleName);
           	load_accords_configuration( &moduleConfig, moduleName );
             
           	while ( argi < argc )
	   	{
			if (!( aptr = argv[++argi] ))
				break;
			else if ( *aptr == '-' )
			{
				aptr++;
				switch( *(aptr++) )
				{
					case	'v'	:
						moduleConfig.verbose=1;
						continue;
					case	'd'	:
						moduleConfig.debug = 0xFFFF;
						continue;
					case	'-'	:
						if (!( argi = accords_configuration_option( aptr, argi, argv )))
						break;
						else	continue;
				}
				status = 30;
				break;
			}
			else if (!( status = module_operation(aptr, &moduleConfig, moduleName, categoryLst) ))
				continue;
			else	break;
	  	}
	  	return(status);
        }
}

int module_main(int argc, char * argv[], char moduleName[], char categoryNameList[])
{
	if(argc == 1)
		return banner();
	else return module(argc, argv, moduleName, categoryNameList);
}

