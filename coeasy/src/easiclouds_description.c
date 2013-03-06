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
#ifndef _easiclouds_description_c_
#define _easiclouds_description_c_

#include "element.h"

#include "easiclouds_description.h"

/*	--------------------------------------------------------------	*/
/*	l i b e r a t e _ e a s i c l o u d s _ d e s c r i p t i o n 	*/
/*	--------------------------------------------------------------	*/
public struct easiclouds_description * liberate_easiclouds_description(struct easiclouds_description * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->label )
			 sptr->label = liberate(sptr->label);
		if ( sptr->value )
			 sptr->value = liberate(sptr->value);
		sptr = liberate( sptr );
	}
	return((struct easiclouds_description *) 0);

}

/*	--------------------------------------------------------	*/
/*	r e s e t _ e a s i c l o u d s _ d e s c r i p t i o n 	*/
/*	--------------------------------------------------------	*/
public struct easiclouds_description * reset_easiclouds_description(struct easiclouds_description * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->label = (char*) 0;
		sptr->value = (char*) 0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	--------------------------------------------------------------	*/
/*	a l l o c a t e _ e a s i c l o u d s _ d e s c r i p t i o n 	*/
/*	--------------------------------------------------------------	*/
public struct easiclouds_description * allocate_easiclouds_description()
{
	struct easiclouds_description * sptr;
	if (!( sptr = allocate( sizeof( struct easiclouds_description ) ) ))
		return( sptr );
	else	return( reset_easiclouds_description(sptr) );
}

/*	--------------------------------------------------------	*/
/*	x m l i n _ e a s i c l o u d s _ d e s c r i p t i o n 	*/
/*	--------------------------------------------------------	*/
public int xmlin_easiclouds_description(struct easiclouds_description * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"label") ))
		{
			if ( wptr->value ) { sptr->label = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"value") ))
		{
			if ( wptr->value ) { sptr->value = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------------------------------	*/
/*	r e s t _ o c c i _ e a s i c l o u d s _ d e s c r i p t i o n 	*/
/*	----------------------------------------------------------------	*/
public int rest_occi_easiclouds_description(FILE * fh,struct easiclouds_description * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.label='%s'\r\n",prefix,nptr,(sptr->label?sptr->label:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.value='%s'\r\n",prefix,nptr,(sptr->value?sptr->value:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _easiclouds_description_ceasiclouds_description_c_ */
