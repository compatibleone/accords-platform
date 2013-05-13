/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>     */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License"); 	*/
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at 				*/
/*  									*/
/*  http://www.apache.org/licenses/LICENSE-2.0 				*/
/*  									*/
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 	*/
/* implied. 								*/
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License. 					*/
/* -------------------------------------------------------------------- */
#ifndef	_conetsdomain_c
#define	_conetsdomain_c

/*	-------------------------------------------	*/
/* 	  c r e a t e  _ n e w _ d o m a i n	 	*/
/*	-------------------------------------------	*/
public struct occi_response * create_new_domain(char * domain, char * agent, char * tls)
{
      
   	struct	occi_element 	*	eptr=(struct occi_element*) 0;
	struct	occi_element 	*	eptr2=(struct occi_element*) 0;
	struct	occi_element 	*	eptr3=(struct occi_element*) 0;
        struct	occi_element 	*	eptr4=(struct occi_element*) 0;
	struct	occi_client  	*	cptr=(struct occi_client*) 0;
	struct	occi_response	*	aptr=(struct occi_response*) 0;
	struct	occi_response	*	aaptr=(struct occi_response*) 0;
	struct  occi_request    *       rptr=(struct occi_request*)0;
        char	buffer[4096];
        char    timebuff[1024];
        time_t  timestamp;      
 
	if ( check_verbose() )
		printf("   OCCI Conets ( %s, %s )\n","domain",agent);

        initialise_occi_resolver( _DEFAULT_PUBLISHER , (char *) 0, (char *) 0, (char *) 0 );
        
	if (!( aptr = occi_resolver( "domain" , agent ) ))
	{ 
                 aptr = occi_remove_response( aptr );
 		 return((struct occi_response *)0);
        }
	
                eptr=aptr->first;
		sprintf(buffer,"%s/%s/",eptr->value,"domain");
		if ( cptr = occi_create_client( buffer, agent, tls ) ) 
		{ 
                  if ((rptr = occi_create_request( 
	         		cptr, 
		        	cptr->target->object, 
				_OCCI_NORMAL )) != (struct occi_request *) 0)
		     {	
		        timestamp = time(NULL);
                        sprintf(timebuff,"%ld",timestamp);
			if (    ((eptr2  = occi_request_element( rptr,"occi.domain.name" , domain )))
                             && ((eptr3  = occi_request_element( rptr,"occi.domain.timestamp", timebuff  )))
                             && ((eptr4  = occi_request_element( rptr,"occi.domain.owner", Conets.user   ))) )

				
			{
				aaptr = occi_client_post( cptr, rptr  );
				rptr  = occi_remove_request( rptr );
			}
		      }	
		  cptr = occi_remove_client( cptr );
                 }
	aptr = occi_remove_response( aaptr );
        return (aaptr );
}

#endif	/* _conetsdomain_c */
	/* --------------- */



