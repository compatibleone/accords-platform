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
#ifndef _timer_c_
#define _timer_c_

#include "element.h"

#include "timer.h"

/*	----------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ t i m e r 	*/
/*	----------------------------------------	*/
public struct cords_timer * liberate_cords_timer(struct cords_timer * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->description )
			 sptr->description = liberate(sptr->description);
		if ( sptr->account )
			 sptr->account = liberate(sptr->account);
		if ( sptr->price )
			 sptr->price = liberate(sptr->price);
		if ( sptr->period )
			 sptr->period = liberate(sptr->period);
		if ( sptr->nature )
			 sptr->nature = liberate(sptr->nature);
		if ( sptr->created )
			 sptr->created = liberate(sptr->created);
		if ( sptr->activated )
			 sptr->activated = liberate(sptr->activated);
		if ( sptr->activation )
			 sptr->activation = liberate(sptr->activation);
		if ( sptr->source )
			 sptr->source = liberate(sptr->source);
		if ( sptr->target )
			 sptr->target = liberate(sptr->target);
		sptr = liberate( sptr );
	}
	return((struct cords_timer *) 0);

}

/*	----------------------------------	*/
/*	r e s e t _ c o r d s _ t i m e r 	*/
/*	----------------------------------	*/
public struct cords_timer * reset_cords_timer(struct cords_timer * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->description = (char*) 0;
		sptr->account = (char*) 0;
		sptr->price = (char*) 0;
		sptr->period = (char*) 0;
		sptr->nature = (char*) 0;
		sptr->created = (char*) 0;
		sptr->activated = (char*) 0;
		sptr->activation = (char*) 0;
		sptr->source = (char*) 0;
		sptr->target = (char*) 0;
		sptr->reference =  0;
		sptr->hitcount =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	----------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ t i m e r 	*/
/*	----------------------------------------	*/
public struct cords_timer * allocate_cords_timer()
{
	struct cords_timer * sptr;
	if (!( sptr = allocate( sizeof( struct cords_timer ) ) ))
		return( sptr );
	else	return( reset_cords_timer(sptr) );
}

/*	----------------------------------	*/
/*	x m l i n _ c o r d s _ t i m e r 	*/
/*	----------------------------------	*/
public int xmlin_cords_timer(struct cords_timer * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"description") ))
		{
			if ( wptr->value ) { sptr->description = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"account") ))
		{
			if ( wptr->value ) { sptr->account = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"price") ))
		{
			if ( wptr->value ) { sptr->price = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"period") ))
		{
			if ( wptr->value ) { sptr->period = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"nature") ))
		{
			if ( wptr->value ) { sptr->nature = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"created") ))
		{
			if ( wptr->value ) { sptr->created = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"activated") ))
		{
			if ( wptr->value ) { sptr->activated = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"activation") ))
		{
			if ( wptr->value ) { sptr->activation = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"source") ))
		{
			if ( wptr->value ) { sptr->source = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"target") ))
		{
			if ( wptr->value ) { sptr->target = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"reference") ))
		{
			if ( wptr->value ) { sptr->reference = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"hitcount") ))
		{
			if ( wptr->value ) { sptr->hitcount = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ t i m e r 	*/
/*	------------------------------------------	*/
public int rest_occi_cords_timer(FILE * fh,struct cords_timer * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.description='%s'\r\n",prefix,nptr,(sptr->description?sptr->description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.account='%s'\r\n",prefix,nptr,(sptr->account?sptr->account:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.price='%s'\r\n",prefix,nptr,(sptr->price?sptr->price:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.period='%s'\r\n",prefix,nptr,(sptr->period?sptr->period:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.nature='%s'\r\n",prefix,nptr,(sptr->nature?sptr->nature:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.created='%s'\r\n",prefix,nptr,(sptr->created?sptr->created:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.activated='%s'\r\n",prefix,nptr,(sptr->activated?sptr->activated:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.activation='%s'\r\n",prefix,nptr,(sptr->activation?sptr->activation:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.source='%s'\r\n",prefix,nptr,(sptr->source?sptr->source:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.target='%s'\r\n",prefix,nptr,(sptr->target?sptr->target:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.reference='%u'\r\n",prefix,nptr,sptr->reference);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.hitcount='%u'\r\n",prefix,nptr,sptr->hitcount);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _timer_ctimer_c_ */
