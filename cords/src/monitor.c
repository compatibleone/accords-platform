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
#ifndef _monitor_c_
#define _monitor_c_

#include "element.h"

#include "monitor.h"

/*	--------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ m o n i t o r 	*/
/*	--------------------------------------------	*/
public struct cords_monitor * liberate_cords_monitor(struct cords_monitor * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->agreement )
			 sptr->agreement = liberate(sptr->agreement);
		if ( sptr->account )
			 sptr->account = liberate(sptr->account);
		if ( sptr->session )
			 sptr->session = liberate(sptr->session);
		if ( sptr->connection )
			 sptr->connection = liberate(sptr->connection);
		if ( sptr->report )
			 sptr->report = liberate(sptr->report);
		sptr = liberate( sptr );
	}
	return((struct cords_monitor *) 0);

}

/*	--------------------------------------	*/
/*	r e s e t _ c o r d s _ m o n i t o r 	*/
/*	--------------------------------------	*/
public struct cords_monitor * reset_cords_monitor(struct cords_monitor * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->agreement = (char*) 0;
		sptr->account = (char*) 0;
		sptr->session = (char*) 0;
		sptr->connection = (char*) 0;
		sptr->report = (char*) 0;
		sptr->controls =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	--------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ m o n i t o r 	*/
/*	--------------------------------------------	*/
public struct cords_monitor * allocate_cords_monitor()
{
	struct cords_monitor * sptr;
	if (!( sptr = allocate( sizeof( struct cords_monitor ) ) ))
		return( sptr );
	else	return( reset_cords_monitor(sptr) );
}

/*	--------------------------------------	*/
/*	x m l i n _ c o r d s _ m o n i t o r 	*/
/*	--------------------------------------	*/
public int xmlin_cords_monitor(struct cords_monitor * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"agreement") ))
		{
			if ( wptr->value ) { sptr->agreement = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"account") ))
		{
			if ( wptr->value ) { sptr->account = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"session") ))
		{
			if ( wptr->value ) { sptr->session = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"connection") ))
		{
			if ( wptr->value ) { sptr->connection = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"report") ))
		{
			if ( wptr->value ) { sptr->report = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"controls") ))
		{
			if ( wptr->value ) { sptr->controls = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ m o n i t o r 	*/
/*	----------------------------------------------	*/
public int rest_occi_cords_monitor(FILE * fh,struct cords_monitor * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.agreement='%s'\r\n",prefix,nptr,(sptr->agreement?sptr->agreement:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.account='%s'\r\n",prefix,nptr,(sptr->account?sptr->account:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.session='%s'\r\n",prefix,nptr,(sptr->session?sptr->session:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.connection='%s'\r\n",prefix,nptr,(sptr->connection?sptr->connection:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.report='%s'\r\n",prefix,nptr,(sptr->report?sptr->report:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.controls='%u'\r\n",prefix,nptr,sptr->controls);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _monitor_cmonitor_c_ */
