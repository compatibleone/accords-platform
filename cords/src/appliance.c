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
#ifndef _appliance_c_
#define _appliance_c_

#include "element.h"

#include "appliance.h"

/*	------------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ a p p l i a n c e 	*/
/*	------------------------------------------------	*/
public struct cords_appliance * liberate_cords_appliance(struct cords_appliance * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->carrier )
			 sptr->carrier = liberate(sptr->carrier);
		if ( sptr->profile )
			 sptr->profile = liberate(sptr->profile);
		if ( sptr->infrastructure )
			 sptr->infrastructure = liberate(sptr->infrastructure);
		if ( sptr->image )
			 sptr->image = liberate(sptr->image);
		sptr = liberate( sptr );
	}
	return((struct cords_appliance *) 0);

}

/*	------------------------------------------	*/
/*	r e s e t _ c o r d s _ a p p l i a n c e 	*/
/*	------------------------------------------	*/
public struct cords_appliance * reset_cords_appliance(struct cords_appliance * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->carrier = (char*) 0;
		sptr->profile = (char*) 0;
		sptr->infrastructure = (char*) 0;
		sptr->image = (char*) 0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	------------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ a p p l i a n c e 	*/
/*	------------------------------------------------	*/
public struct cords_appliance * allocate_cords_appliance()
{
	struct cords_appliance * sptr;
	if (!( sptr = allocate( sizeof( struct cords_appliance ) ) ))
		return( sptr );
	else	return( reset_cords_appliance(sptr) );
}

/*	------------------------------------------	*/
/*	x m l i n _ c o r d s _ a p p l i a n c e 	*/
/*	------------------------------------------	*/
public int xmlin_cords_appliance(struct cords_appliance * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"carrier") ))
		{
			if ( wptr->value ) { sptr->carrier = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"profile") ))
		{
			if ( wptr->value ) { sptr->profile = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"infrastructure") ))
		{
			if ( wptr->value ) { sptr->infrastructure = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"image") ))
		{
			if ( wptr->value ) { sptr->image = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	--------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ a p p l i a n c e 	*/
/*	--------------------------------------------------	*/
public int rest_occi_cords_appliance(FILE * fh,struct cords_appliance * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.carrier='%s'\r\n",prefix,nptr,(sptr->carrier?sptr->carrier:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.profile='%s'\r\n",prefix,nptr,(sptr->profile?sptr->profile:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.infrastructure='%s'\r\n",prefix,nptr,(sptr->infrastructure?sptr->infrastructure:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.image='%s'\r\n",prefix,nptr,(sptr->image?sptr->image:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _appliance_c_ */
