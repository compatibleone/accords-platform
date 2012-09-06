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
#ifndef _packet_c_
#define _packet_c_

#include "element.h"

#include "packet.h"

/*	------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ p a c k e t 	*/
/*	------------------------------------------	*/
public struct cords_packet * liberate_cords_packet(struct cords_packet * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->connection )
			 sptr->connection = liberate(sptr->connection);
		if ( sptr->probe )
			 sptr->probe = liberate(sptr->probe);
		if ( sptr->start )
			 sptr->start = liberate(sptr->start);
		if ( sptr->finish )
			 sptr->finish = liberate(sptr->finish);
		if ( sptr->metric )
			 sptr->metric = liberate(sptr->metric);
		if ( sptr->data )
			 sptr->data = liberate(sptr->data);
		sptr = liberate( sptr );
	}
	return((struct cords_packet *) 0);

}

/*	------------------------------------	*/
/*	r e s e t _ c o r d s _ p a c k e t 	*/
/*	------------------------------------	*/
public struct cords_packet * reset_cords_packet(struct cords_packet * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->connection = (char*) 0;
		sptr->probe = (char*) 0;
		sptr->start = (char*) 0;
		sptr->finish = (char*) 0;
		sptr->metric = (char*) 0;
		sptr->data = (char*) 0;
		sptr->sequence =  0;
		sptr->samples =  0;
		sptr->status =  0;
	}
	return(sptr);

}

/*	------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ p a c k e t 	*/
/*	------------------------------------------	*/
public struct cords_packet * allocate_cords_packet()
{
	struct cords_packet * sptr;
	if (!( sptr = allocate( sizeof( struct cords_packet ) ) ))
		return( sptr );
	else	return( reset_cords_packet(sptr) );
}

/*	------------------------------------	*/
/*	x m l i n _ c o r d s _ p a c k e t 	*/
/*	------------------------------------	*/
public int xmlin_cords_packet(struct cords_packet * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"connection") ))
		{
			if ( wptr->value ) { sptr->connection = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"probe") ))
		{
			if ( wptr->value ) { sptr->probe = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"start") ))
		{
			if ( wptr->value ) { sptr->start = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"finish") ))
		{
			if ( wptr->value ) { sptr->finish = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"metric") ))
		{
			if ( wptr->value ) { sptr->metric = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"data") ))
		{
			if ( wptr->value ) { sptr->data = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"sequence") ))
		{
			if ( wptr->value ) { sptr->sequence = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"samples") ))
		{
			if ( wptr->value ) { sptr->samples = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"status") ))
		{
			if ( wptr->value ) { sptr->status = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	--------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ p a c k e t 	*/
/*	--------------------------------------------	*/
public int rest_occi_cords_packet(FILE * fh,struct cords_packet * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.connection='%s'\r\n",prefix,nptr,(sptr->connection?sptr->connection:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.probe='%s'\r\n",prefix,nptr,(sptr->probe?sptr->probe:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.start='%s'\r\n",prefix,nptr,(sptr->start?sptr->start:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.finish='%s'\r\n",prefix,nptr,(sptr->finish?sptr->finish:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.metric='%s'\r\n",prefix,nptr,(sptr->metric?sptr->metric:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.data='%s'\r\n",prefix,nptr,(sptr->data?sptr->data:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.sequence='%u'\r\n",prefix,nptr,sptr->sequence);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.samples='%u'\r\n",prefix,nptr,sptr->samples);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.status='%u'\r\n",prefix,nptr,sptr->status);
	return(0);

}

#endif	/* _packet_cpacket_c_ */
