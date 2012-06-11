/* ---------------------------------------------------------------------------- */
/* Advanced Capabilities for Compatible One Resources Delivery System - ACCORDS	*/
/* module to launch a new Accords component                                     */
/* Hamid MEDJAHED & Elyes ZEKRI for (C) 2011 Prologue              		*/
/* ---------------------------------------------------------------------------- */
/*										*/
/* This is free software; you can redistribute it and/or modify it		*/
/* under the terms of the GNU Lesser General Public License as			*/
/* published by the Free Software Foundation; either version 2.1 of		*/
/* the License, or (at your option) any later version.				*/
/*										*/
/* This software is distributed in the hope that it will be useful,		*/
/* but WITHOUT ANY WARRANTY; without even the implied warranty of		*/
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU		*/
/* Lesser General Public License for more details.				*/
/*										*/
/* You should have received a copy of the GNU Lesser General Public		*/
/* License along with this software; if not, write to the Free			*/
/* Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA		*/
/* 02110-1301 USA, or see the FSF site: http://www.fsf.org.			*/
/*										*/
/* ---------------------------------------------------------------------------- */
#ifndef	_COMPONENT_c
#define	_COMPONENT_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"
#include "list.h"
#include "../../pyaccords/pysrc/crudinterf.h"
#include "../../pyaccords/pysrc/categaction.h"

char cords_action[12][256]={"start","stop","restart","snapshot","save","suspend","softboot","hardboot","resize","confirm","revert"}; 
struct accords_configuration moduleConfig;

void fillInAccordsConfiguration(struct accords_configuration *componentModule, char *moduleName)
{
        char configFile[256];
        sprintf(configFile,"%s.xml",moduleName);
        componentModule->monitor=0;                            /* monitoring information options       */
        componentModule->autopub=0;                            /* auto publication mechanisms          */
        componentModule->verbose=0;                            /* activate verbose messages            */
        componentModule->debug=0;                              /* activate debug messages              */
        componentModule->threads=0;                            /* allow use of threads                 */

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
   return(OsProcci.zone);		
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

/**************************************************************************************************/
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

/*	------------------------------------------------------------------	*/
/*			module_ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
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

/*	------------------------------------------------------------------	*/
/*			module _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	module_authorise(  void * v,struct rest_client * cptr, char * username, char * password, struct accords_configuration *componentModule)
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

/*	------------------------------------------------------------------	*/
/*			module _ e x t e n s i o n 			*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * module_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}
/******************************************************************************************************************/
/*      Function to call occi category action for the category specified in name variable                        */
/******************************************************************************************************************/
struct rest_response * callocciCategoryAction(const char *name)
{
  int i;
   for (i = 0; i < (sizeof(occiCategoryAction_map) / sizeof(occiCategoryAction_map[0])); i++) 
   {
     if (!strcmp(occiCategoryAction_map[i].name, name)) 
     {
       return occiCategoryAction_map[i].func;
     }
  }

  return (struct occi_category *) 0;
}

/******************************************************************************************************************/
/*      Function to call occi category builder for the category specified in name variable                        */
/******************************************************************************************************************/
struct occi_category * callocciCategoryBuilder(const char *name, char *a,char *b)
{
  int i;
   for (i = 0; i < (sizeof(occiCategoryBuilder_map) / sizeof(occiCategoryBuilder_map[0])); i++) 
   {
     if (!strcmp(occiCategoryBuilder_map[i].name, name) && occiCategoryBuilder_map[i].func) 
     {
       return occiCategoryBuilder_map[i].func(a,b);
     }
  }

  return (struct occi_category *) 0;
}

/******************************************************************************************************************/
/* Function to call occi category interface for the category indicated in name variable                           */
/******************************************************************************************************************/
struct occi_interface * callocciCategoryInterface(const char *name)
{
  int i;
  for (i=0; i<(sizeof(occiCategoryInterface_map)/ sizeof(occiCategoryInterface_map[0])); i++)
  {
    if(!strcmp(occiCategoryInterface_map[i].name,name))
    {
      return occiCategoryInterface_map[i].interface_Func();
    }
  }
  return 0;
}


/******************************************************************************************************************/
/*			module _ o p e r a t i o n				                                  */
/******************************************************************************************************************/
private	int	module_operation( char * nptr, struct accords_configuration *componentModule, char *moduleName, 
listc categoryName,listc categoryAct,int dim)
{
        char xlinkModule[256];
	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;
        
        int i;
        struct occi_interface categoryInterface;
        int indice;
        
        char categoryAction[256];

        elem *pelem = categoryName.first;    
        while(pelem)
        {       
          sprintf(xlinkModule,"links_%s.xml",moduleName);
          xlinkModule[strlen(xlinkModule)]=0;
	  set_autosave_cords_xlink_name(xlinkModule);
      
	  if (!( optr = callocciCategoryBuilder( pelem->value,componentModule->domain,pelem->value) ))
		   return( 27 );
	  else if (!( optr->previous = last ))
		first = optr;
	  else	optr->previous->next = optr;
	  last = optr;
	  optr->callback = callocciCategoryInterface(pelem->value);
          
          elem *pelemact=categoryAct.first;
          indice=atoi(pelemact->value);
         
          for(i=0;i<indice;i++)
          {
               sprintf(categoryAction,"%s_action%d",pelem->value,i);
               if (!( optr = occi_add_action( optr,cords_action[i],"",callocciCategoryAction(categoryAction))))
		   return( optr );
          }  
           
           pelemact=pelemact->next;
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
private	int	module(int argc, char * argv[],char *moduleName,char * categoryList,char * categoryActionNumberList)
{
	int	status=0;
	int	argi=0;
	char *	aptr;
        char * token;
       
        listc  categoryLst;
        listc  categoryAct;  
        int dim=0;
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
              dim++;
              addBack(&categoryLst,token);
              token=strtok(NULL, " ");
           }
           
           resetList(&categoryAct);
           token= strtok(categoryActionNumberList," ");
           for(; token != NULL ;)
           {
              dim++;
              addBack(&categoryAct,token);
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
		else if (!( status = module_operation(aptr,&moduleConfig,moduleName,categoryLst,categoryAct,dim) ))
			continue;
		else	break;
	  }
	  return(status);
        }
}

	/* --------- */
#endif	/* _COMPONENT_C */
	/* --------- */

