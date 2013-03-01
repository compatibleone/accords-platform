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
#ifndef _workload_c_
#define _workload_c_

#include "element.h"

#include "workload.h"

/*	----------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ w o r k l o a d 	*/
/*	----------------------------------------------	*/
public struct cords_workload * liberate_cords_workload(struct cords_workload * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->description )
			 sptr->description = liberate(sptr->description);
		if ( sptr->contract )
			 sptr->contract = liberate(sptr->contract);
		sptr = liberate( sptr );
	}
	return((struct cords_workload *) 0);

}

/*	----------------------------------------	*/
/*	r e s e t _ c o r d s _ w o r k l o a d 	*/
/*	----------------------------------------	*/
public struct cords_workload * reset_cords_workload(struct cords_workload * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->description = (char*) 0;
		sptr->contract = (char*) 0;
		sptr->nature =  0;
		sptr->threads =  0;
		sptr->average =  0;
		sptr->total =  0;
		sptr->samples =  0;
		sptr->timestamp =  0;
		sptr->period =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	----------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ w o r k l o a d 	*/
/*	----------------------------------------------	*/
public struct cords_workload * allocate_cords_workload()
{
	struct cords_workload * sptr;
	if (!( sptr = allocate( sizeof( struct cords_workload ) ) ))
		return( sptr );
	else	return( reset_cords_workload(sptr) );
}

/*	----------------------------------------	*/
/*	x m l i n _ c o r d s _ w o r k l o a d 	*/
/*	----------------------------------------	*/
public int xmlin_cords_workload(struct cords_workload * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"contract") ))
		{
			if ( wptr->value ) { sptr->contract = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"nature") ))
		{
			if ( wptr->value ) { sptr->nature = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"threads") ))
		{
			if ( wptr->value ) { sptr->threads = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"average") ))
		{
			if ( wptr->value ) { sptr->average = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"total") ))
		{
			if ( wptr->value ) { sptr->total = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"samples") ))
		{
			if ( wptr->value ) { sptr->samples = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"timestamp") ))
		{
			if ( wptr->value ) { sptr->timestamp = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"period") ))
		{
			if ( wptr->value ) { sptr->period = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ w o r k l o a d 	*/
/*	------------------------------------------------	*/
public int rest_occi_cords_workload(FILE * fh,struct cords_workload * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.description='%s'\r\n",prefix,nptr,(sptr->description?sptr->description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.contract='%s'\r\n",prefix,nptr,(sptr->contract?sptr->contract:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.nature='%u'\r\n",prefix,nptr,sptr->nature);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.threads='%u'\r\n",prefix,nptr,sptr->threads);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.average='%u'\r\n",prefix,nptr,sptr->average);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.total='%u'\r\n",prefix,nptr,sptr->total);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.samples='%u'\r\n",prefix,nptr,sptr->samples);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.timestamp='%u'\r\n",prefix,nptr,sptr->timestamp);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.period='%u'\r\n",prefix,nptr,sptr->period);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _workload_cworkload_c_ */
