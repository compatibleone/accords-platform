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
#ifndef	_conetsis_c
#define	_conetsip_c

/*	-------------------------------------------	*/
/* 	      c r e a t e _ i p a d d r e s s  		*/
/*	-------------------------------------------	*/
private	int	create_ipaddress(struct occi_category * optr, void * vptr)
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
        
        if (*(pptr->version)=='\0')
         pptr->version=_DEFAULT_VERSION;
        if (*(pptr->type)=='\0')
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
        if (*(pptr->domain)=='\0')
          pptr->domain=_DEFAULT_DOMAIN;
        
        domainame = allocate_string( retrieve_domain(pptr->domain, conets_agent, default_tls()) );
 	if (!domainame)
            if (!create_new_domain(pptr->domain, conets_agent, default_tls()))
		failure (44, "create", "domain"); 
 	 
        return(0);
        
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ i p a d d r e s s  	*/
/*	-------------------------------------------	*/
private	int	retrieve_ipaddress(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_ipaddress * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ i p a d d r e s s 	 	*/
/*	-------------------------------------------	*/
private	int	update_ipaddress(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_ipaddress * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ i p a d d r e s s  		*/
/*	-------------------------------------------	*/
private	int	delete_ipaddress(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_ipaddress * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

private	struct	occi_interface	ipaddress_interface = {
	create_ipaddress,
	retrieve_ipaddress,
	update_ipaddress,
	delete_ipaddress
	};

#endif	/* _conetsip_c */
	/* ----------- */



