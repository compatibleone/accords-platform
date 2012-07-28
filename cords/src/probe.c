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
#ifndef _probe_c_
#define _probe_c_

#include "element.h"

#include "probe.h"

/*	----------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ p r o b e 	*/
/*	----------------------------------------	*/
public struct cords_probe * liberate_cords_probe(struct cords_probe * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->metric )
			 sptr->metric = liberate(sptr->metric);
		if ( sptr->connection )
			 sptr->connection = liberate(sptr->connection);
		if ( sptr->expression )
			 sptr->expression = liberate(sptr->expression);
		sptr = liberate( sptr );
	}
	return((struct cords_probe *) 0);

}

/*	----------------------------------	*/
/*	r e s e t _ c o r d s _ p r o b e 	*/
/*	----------------------------------	*/
public struct cords_probe * reset_cords_probe(struct cords_probe * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->metric = (char*) 0;
		sptr->connection = (char*) 0;
		sptr->expression = (char*) 0;
		sptr->samples =  0;
		sptr->period =  0;
		sptr->pid =  0;
		sptr->packets =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	----------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ p r o b e 	*/
/*	----------------------------------------	*/
public struct cords_probe * allocate_cords_probe()
{
	struct cords_probe * sptr;
	if (!( sptr = allocate( sizeof( struct cords_probe ) ) ))
		return( sptr );
	else	return( reset_cords_probe(sptr) );
}

/*	----------------------------------	*/
/*	x m l i n _ c o r d s _ p r o b e 	*/
/*	----------------------------------	*/
public int xmlin_cords_probe(struct cords_probe * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"metric") ))
		{
			if ( wptr->value ) { sptr->metric = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"connection") ))
		{
			if ( wptr->value ) { sptr->connection = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"expression") ))
		{
			if ( wptr->value ) { sptr->expression = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"samples") ))
		{
			if ( wptr->value ) { sptr->samples = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"period") ))
		{
			if ( wptr->value ) { sptr->period = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"pid") ))
		{
			if ( wptr->value ) { sptr->pid = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"packets") ))
		{
			if ( wptr->value ) { sptr->packets = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ p r o b e 	*/
/*	------------------------------------------	*/
public int rest_occi_cords_probe(FILE * fh,struct cords_probe * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.metric='%s'\r\n",prefix,nptr,(sptr->metric?sptr->metric:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.connection='%s'\r\n",prefix,nptr,(sptr->connection?sptr->connection:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.expression='%s'\r\n",prefix,nptr,(sptr->expression?sptr->expression:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.samples='%u'\r\n",prefix,nptr,sptr->samples);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.period='%u'\r\n",prefix,nptr,sptr->period);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.pid='%u'\r\n",prefix,nptr,sptr->pid);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.packets='%u'\r\n",prefix,nptr,sptr->packets);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _probe_c_ */
