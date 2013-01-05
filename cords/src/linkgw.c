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

/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _linkgw_c_
#define _linkgw_c_

#include "element.h"

#include "linkgw.h"

/*	------------------------------	*/
/*	l i b e r a t e _ l i n k g w 	*/
/*	------------------------------	*/
public struct linkgw * liberate_linkgw(struct linkgw * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->intercloudGW )
			 sptr->intercloudGW = liberate(sptr->intercloudGW);
		if ( sptr->account )
			 sptr->account = liberate(sptr->account);
		if ( sptr->gwsrc )
			 sptr->gwsrc = liberate(sptr->gwsrc);
		if ( sptr->gwdst )
			 sptr->gwdst = liberate(sptr->gwdst);
		if ( sptr->tunnelproto )
			 sptr->tunnelproto = liberate(sptr->tunnelproto);
		if ( sptr->addressgresrc )
			 sptr->addressgresrc = liberate(sptr->addressgresrc);
		if ( sptr->addressgredst )
			 sptr->addressgredst = liberate(sptr->addressgredst);
		if ( sptr->prefix )
			 sptr->prefix = liberate(sptr->prefix);
		if ( sptr->authenticationkey )
			 sptr->authenticationkey = liberate(sptr->authenticationkey);
		if ( sptr->endpointsrc )
			 sptr->endpointsrc = liberate(sptr->endpointsrc);
		if ( sptr->endpointdst )
			 sptr->endpointdst = liberate(sptr->endpointdst);
		if ( sptr->state )
			 sptr->state = liberate(sptr->state);
		sptr = liberate( sptr );
	}
	return((struct linkgw *) 0);

}

/*	------------------------	*/
/*	r e s e t _ l i n k g w 	*/
/*	------------------------	*/
public struct linkgw * reset_linkgw(struct linkgw * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->intercloudGW = (char*) 0;
		sptr->account = (char*) 0;
		sptr->gwsrc = (char*) 0;
		sptr->gwdst = (char*) 0;
		sptr->tunnelproto = (char*) 0;
		sptr->addressgresrc = (char*) 0;
		sptr->addressgredst = (char*) 0;
		sptr->prefix = (char*) 0;
		sptr->authenticationkey = (char*) 0;
		sptr->endpointsrc = (char*) 0;
		sptr->endpointdst = (char*) 0;
		sptr->state = (char*) 0;
	}
	return(sptr);

}

/*	------------------------------	*/
/*	a l l o c a t e _ l i n k g w 	*/
/*	------------------------------	*/
public struct linkgw * allocate_linkgw()
{
	struct linkgw * sptr;
	if (!( sptr = allocate( sizeof( struct linkgw ) ) ))
		return( sptr );
	else	return( reset_linkgw(sptr) );
}

/*	------------------------	*/
/*	x m l i n _ l i n k g w 	*/
/*	------------------------	*/
public int xmlin_linkgw(struct linkgw * sptr,struct xml_element * eptr)
{
	struct xml_element * wptr;
	if (!( eptr )) return(0);
	if (!( sptr )) return(0);
	for ( wptr=eptr->first; wptr != (struct xml_element *) 0; wptr=wptr->next)
	{
		if (!( strcmp(wptr->name,"id") ))
		{
			if ( wptr->value ) { sptr->id = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"name") ))
		{
			if ( wptr->value ) { sptr->name = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"intercloudGW") ))
		{
			if ( wptr->value ) { sptr->intercloudGW = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"account") ))
		{
			if ( wptr->value ) { sptr->account = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"gwsrc") ))
		{
			if ( wptr->value ) { sptr->gwsrc = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"gwdst") ))
		{
			if ( wptr->value ) { sptr->gwdst = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"tunnelproto") ))
		{
			if ( wptr->value ) { sptr->tunnelproto = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"addressgresrc") ))
		{
			if ( wptr->value ) { sptr->addressgresrc = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"addressgredst") ))
		{
			if ( wptr->value ) { sptr->addressgredst = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"prefix") ))
		{
			if ( wptr->value ) { sptr->prefix = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"authenticationkey") ))
		{
			if ( wptr->value ) { sptr->authenticationkey = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"endpointsrc") ))
		{
			if ( wptr->value ) { sptr->endpointsrc = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"endpointdst") ))
		{
			if ( wptr->value ) { sptr->endpointdst = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = allocate_string(wptr->value); }
		}
	}
	return(0);

}

/*	--------------------------------	*/
/*	r e s t _ o c c i _ l i n k g w 	*/
/*	--------------------------------	*/
public int rest_occi_linkgw(FILE * fh,struct linkgw * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.intercloudGW='%s'\r\n",prefix,nptr,(sptr->intercloudGW?sptr->intercloudGW:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.account='%s'\r\n",prefix,nptr,(sptr->account?sptr->account:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.gwsrc='%s'\r\n",prefix,nptr,(sptr->gwsrc?sptr->gwsrc:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.gwdst='%s'\r\n",prefix,nptr,(sptr->gwdst?sptr->gwdst:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.tunnelproto='%s'\r\n",prefix,nptr,(sptr->tunnelproto?sptr->tunnelproto:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.addressgresrc='%s'\r\n",prefix,nptr,(sptr->addressgresrc?sptr->addressgresrc:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.addressgredst='%s'\r\n",prefix,nptr,(sptr->addressgredst?sptr->addressgredst:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.prefix='%s'\r\n",prefix,nptr,(sptr->prefix?sptr->prefix:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.authenticationkey='%s'\r\n",prefix,nptr,(sptr->authenticationkey?sptr->authenticationkey:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.endpointsrc='%s'\r\n",prefix,nptr,(sptr->endpointsrc?sptr->endpointsrc:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.endpointdst='%s'\r\n",prefix,nptr,(sptr->endpointdst?sptr->endpointdst:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%s'\r\n",prefix,nptr,(sptr->state?sptr->state:""));
	return(0);

}

#endif	/* _linkgw_clinkgw_c_ */
