/* ------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                   */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>    */
/* --------------------------------------------------------------------*/
/*  This is free software; you can redistribute it and/or modify it    */
/*  under the terms of the GNU Lesser General Public License as        */
/*  published by the Free Software Foundation; either version 2.1 of   */
/*  the License, or (at your option) any later version.                */
/*                                                                     */
/*  This software is distributed in the hope that it will be useful,   */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of     */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   */
/*  Lesser General Public License for more details.                    */
/*                                                                     */
/*  You should have received a copy of the GNU Lesser General Public   */
/*  License along with this software; if not, write to the Free        */
/*  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA */
/*  02110-1301 USA, or see the FSF site: http://www.fsf.org.           */
/* --------------------------------------------------------------------*/

/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _schedule_c_
#define _schedule_c_

#include "element.h"

#include "schedule.h"

/*	----------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ s c h e d u l e 	*/
/*	----------------------------------------------	*/
public struct cords_schedule * liberate_cords_schedule(struct cords_schedule * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->operation )
			 sptr->operation = liberate(sptr->operation);
		if ( sptr->account )
			 sptr->account = liberate(sptr->account);
		if ( sptr->price )
			 sptr->price = liberate(sptr->price);
		sptr = liberate( sptr );
	}
	return((struct cords_schedule *) 0);

}

/*	----------------------------------------	*/
/*	r e s e t _ c o r d s _ s c h e d u l e 	*/
/*	----------------------------------------	*/
public struct cords_schedule * reset_cords_schedule(struct cords_schedule * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->operation = (char*) 0;
		sptr->account = (char*) 0;
		sptr->price = (char*) 0;
		sptr->requested =  0;
		sptr->expected =  0;
		sptr->started =  0;
		sptr->completed =  0;
		sptr->priority =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	----------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ s c h e d u l e 	*/
/*	----------------------------------------------	*/
public struct cords_schedule * allocate_cords_schedule()
{
	struct cords_schedule * sptr;
	if (!( sptr = allocate( sizeof( struct cords_schedule ) ) ))
		return( sptr );
	else	return( reset_cords_schedule(sptr) );
}

/*	----------------------------------------	*/
/*	x m l i n _ c o r d s _ s c h e d u l e 	*/
/*	----------------------------------------	*/
public int xmlin_cords_schedule(struct cords_schedule * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"operation") ))
		{
			if ( wptr->value ) { sptr->operation = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"account") ))
		{
			if ( wptr->value ) { sptr->account = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"price") ))
		{
			if ( wptr->value ) { sptr->price = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"requested") ))
		{
			if ( wptr->value ) { sptr->requested = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"expected") ))
		{
			if ( wptr->value ) { sptr->expected = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"started") ))
		{
			if ( wptr->value ) { sptr->started = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"completed") ))
		{
			if ( wptr->value ) { sptr->completed = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"priority") ))
		{
			if ( wptr->value ) { sptr->priority = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ s c h e d u l e 	*/
/*	------------------------------------------------	*/
public int rest_occi_cords_schedule(FILE * fh,struct cords_schedule * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.operation='%s'\r\n",prefix,nptr,(sptr->operation?sptr->operation:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.account='%s'\r\n",prefix,nptr,(sptr->account?sptr->account:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.price='%s'\r\n",prefix,nptr,(sptr->price?sptr->price:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.requested='%u'\r\n",prefix,nptr,sptr->requested);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.expected='%u'\r\n",prefix,nptr,sptr->expected);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.started='%u'\r\n",prefix,nptr,sptr->started);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.completed='%u'\r\n",prefix,nptr,sptr->completed);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.priority='%u'\r\n",prefix,nptr,sptr->priority);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _schedule_c_ */
