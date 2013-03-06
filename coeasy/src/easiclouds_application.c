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
#ifndef _easiclouds_application_c_
#define _easiclouds_application_c_

#include "element.h"

#include "easiclouds_application.h"

/*	--------------------------------------------------------------	*/
/*	l i b e r a t e _ e a s i c l o u d s _ a p p l i c a t i o n 	*/
/*	--------------------------------------------------------------	*/
public struct easiclouds_application * liberate_easiclouds_application(struct easiclouds_application * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		sptr = liberate( sptr );
	}
	return((struct easiclouds_application *) 0);

}

/*	--------------------------------------------------------	*/
/*	r e s e t _ e a s i c l o u d s _ a p p l i c a t i o n 	*/
/*	--------------------------------------------------------	*/
public struct easiclouds_application * reset_easiclouds_application(struct easiclouds_application * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->state =  0;
		sptr->nodes =  0;
		sptr->links =  0;
		sptr->extras =  0;
	}
	return(sptr);

}

/*	--------------------------------------------------------------	*/
/*	a l l o c a t e _ e a s i c l o u d s _ a p p l i c a t i o n 	*/
/*	--------------------------------------------------------------	*/
public struct easiclouds_application * allocate_easiclouds_application()
{
	struct easiclouds_application * sptr;
	if (!( sptr = allocate( sizeof( struct easiclouds_application ) ) ))
		return( sptr );
	else	return( reset_easiclouds_application(sptr) );
}

/*	--------------------------------------------------------	*/
/*	x m l i n _ e a s i c l o u d s _ a p p l i c a t i o n 	*/
/*	--------------------------------------------------------	*/
public int xmlin_easiclouds_application(struct easiclouds_application * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"nodes") ))
		{
			if ( wptr->value ) { sptr->nodes = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"links") ))
		{
			if ( wptr->value ) { sptr->links = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"extras") ))
		{
			if ( wptr->value ) { sptr->extras = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------------------------------	*/
/*	r e s t _ o c c i _ e a s i c l o u d s _ a p p l i c a t i o n 	*/
/*	----------------------------------------------------------------	*/
public int rest_occi_easiclouds_application(FILE * fh,struct easiclouds_application * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.nodes='%u'\r\n",prefix,nptr,sptr->nodes);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.links='%u'\r\n",prefix,nptr,sptr->links);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.extras='%u'\r\n",prefix,nptr,sptr->extras);
	return(0);

}

#endif	/* _easiclouds_application_ceasiclouds_application_c_ */
