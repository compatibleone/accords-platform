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
#ifndef _job_c_
#define _job_c_

#include "element.h"

#include "job.h"

/*	------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ j o b 	*/
/*	------------------------------------	*/
public struct cords_job * liberate_cords_job(struct cords_job * sptr)
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
	return((struct cords_job *) 0);

}

/*	------------------------------	*/
/*	r e s e t _ c o r d s _ j o b 	*/
/*	------------------------------	*/
public struct cords_job * reset_cords_job(struct cords_job * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->description = (char*) 0;
		sptr->contract = (char*) 0;
		sptr->floor =  0;
		sptr->ceiling =  0;
		sptr->threshold =  0;
		sptr->strategy =  0;
		sptr->workloads =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ j o b 	*/
/*	------------------------------------	*/
public struct cords_job * allocate_cords_job()
{
	struct cords_job * sptr;
	if (!( sptr = allocate( sizeof( struct cords_job ) ) ))
		return( sptr );
	else	return( reset_cords_job(sptr) );
}

/*	------------------------------	*/
/*	x m l i n _ c o r d s _ j o b 	*/
/*	------------------------------	*/
public int xmlin_cords_job(struct cords_job * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"floor") ))
		{
			if ( wptr->value ) { sptr->floor = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"ceiling") ))
		{
			if ( wptr->value ) { sptr->ceiling = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"threshold") ))
		{
			if ( wptr->value ) { sptr->threshold = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"strategy") ))
		{
			if ( wptr->value ) { sptr->strategy = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"workloads") ))
		{
			if ( wptr->value ) { sptr->workloads = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	--------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ j o b 	*/
/*	--------------------------------------	*/
public int rest_occi_cords_job(FILE * fh,struct cords_job * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.description='%s'\r\n",prefix,nptr,(sptr->description?sptr->description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.contract='%s'\r\n",prefix,nptr,(sptr->contract?sptr->contract:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.floor='%u'\r\n",prefix,nptr,sptr->floor);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.ceiling='%u'\r\n",prefix,nptr,sptr->ceiling);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.threshold='%u'\r\n",prefix,nptr,sptr->threshold);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.strategy='%u'\r\n",prefix,nptr,sptr->strategy);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.workloads='%u'\r\n",prefix,nptr,sptr->workloads);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _job_cjob_c_ */
