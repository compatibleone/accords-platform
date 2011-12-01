/* ---------------------------------------------------------------------------- */
/* Advanced Capabilities for Compatible One Resources Delivery System - ACCORDS	*/
/* (C) 2011 by Iain James Marshall <ijm667@hotmail.com>				*/
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

        initialise_occi_resolver( tls , (char *) 0, (char *) 0, (char *) 0 );
        
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



