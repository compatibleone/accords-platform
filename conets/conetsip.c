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
#ifndef	_conetsis_c
#define	_conetsip_c

/*	-------------------------------------------	*/
/* 	      c r e a t e _ i p a d d r e s s  		*/
/*	-------------------------------------------	*/
private	int	create_ipaddress(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_ipaddress * pptr;
	time_t  timestamp;
        char timebuff[256];
        char * range; 
        char * domainame;
	

	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
        
	if (!( pptr->version ))
         	pptr->version=_DEFAULT_VERSION;
        else if (*(pptr->version)=='\0')
         	pptr->version=_DEFAULT_VERSION;

	if (!( pptr->type ))
        	pptr->type=_DEFAULT_TYPE;
        else if (*(pptr->type)=='\0')
		pptr->type=_DEFAULT_TYPE;

        range = allocate_string(retrieve_iprange(pptr->version, pptr->type, conets_agent, default_tls()));
       
        if( !strcmp(pptr->version, "v6") )
	    pptr->value = allocate_string ( get_ipv6_from_range(range,conets_agent, default_tls() ) );
     	else //v4 (default)
	    pptr->value = allocate_string ( get_ipv4_from_range(range,conets_agent, default_tls() ) );

        timestamp = time(NULL);
        sprintf(timebuff,"%ld",timestamp);

	pptr->timestamp = allocate_string(timebuff);
	pptr->owner = allocate_string(Conets.user);

	if (!( pptr->domain ))
        	pptr->domain=_DEFAULT_DOMAIN;
        else if (*(pptr->domain)=='\0')
		pptr->domain=_DEFAULT_DOMAIN;
        
        if (!( domainame = allocate_string( retrieve_domain(pptr->domain, conets_agent, default_tls()) ) ))
            if (!create_new_domain(pptr->domain, conets_agent, default_tls()))
		failure (44, "create", "domain"); 
 	 
        return(0);
        
}


private	struct	occi_interface	ipaddress_interface = {
	create_ipaddress,
	NULL,
	NULL,
	NULL
	};

#endif	/* _conetsip_c */
	/* ----------- */



