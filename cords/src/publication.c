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
#ifndef _publication_c_
#define _publication_c_

#include "element.h"

#include "publication.h"

/*	----------------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ p u b l i c a t i o n 	*/
/*	----------------------------------------------------	*/
public struct cords_publication * liberate_cords_publication(struct cords_publication * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->remote )
			 sptr->remote = liberate(sptr->remote);
		if ( sptr->what )
			 sptr->what = liberate(sptr->what);
		if ( sptr->where )
			 sptr->where = liberate(sptr->where);
		if ( sptr->why )
			 sptr->why = liberate(sptr->why);
		if ( sptr->who )
			 sptr->who = liberate(sptr->who);
		if ( sptr->pass )
			 sptr->pass = liberate(sptr->pass);
		if ( sptr->identity )
			 sptr->identity = liberate(sptr->identity);
		if ( sptr->zone )
			 sptr->zone = liberate(sptr->zone);
		if ( sptr->price )
			 sptr->price = liberate(sptr->price);
		if ( sptr->rating )
			 sptr->rating = liberate(sptr->rating);
		if ( sptr->operator )
			 sptr->operator = liberate(sptr->operator);
		sptr = liberate( sptr );
	}
	return((struct cords_publication *) 0);

}

/*	----------------------------------------------	*/
/*	r e s e t _ c o r d s _ p u b l i c a t i o n 	*/
/*	----------------------------------------------	*/
public struct cords_publication * reset_cords_publication(struct cords_publication * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct publication*) 0;
		sptr->next = (struct publication*) 0;
		sptr->id = (char*) 0;
		sptr->remote = (char*) 0;
		sptr->what = (char*) 0;
		sptr->where = (char*) 0;
		sptr->why = (char*) 0;
		sptr->when =  0;
		sptr->uptime =  0;
		sptr->who = (char*) 0;
		sptr->pass = (char*) 0;
		sptr->identity = (char*) 0;
		sptr->zone = (char*) 0;
		sptr->price = (char*) 0;
		sptr->rating = (char*) 0;
		sptr->operator = (char*) 0;
		sptr->pid =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	----------------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ p u b l i c a t i o n 	*/
/*	----------------------------------------------------	*/
public struct cords_publication * allocate_cords_publication()
{
	struct cords_publication * sptr;
	if (!( sptr = allocate( sizeof( struct cords_publication ) ) ))
		return( sptr );
	else	return( reset_cords_publication(sptr) );
}

/*	----------------------------------------------	*/
/*	x m l i n _ c o r d s _ p u b l i c a t i o n 	*/
/*	----------------------------------------------	*/
public int xmlin_cords_publication(struct cords_publication * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"remote") ))
		{
			if ( wptr->value ) { sptr->remote = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"what") ))
		{
			if ( wptr->value ) { sptr->what = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"where") ))
		{
			if ( wptr->value ) { sptr->where = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"why") ))
		{
			if ( wptr->value ) { sptr->why = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"when") ))
		{
			if ( wptr->value ) { sptr->when = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"uptime") ))
		{
			if ( wptr->value ) { sptr->uptime = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"who") ))
		{
			if ( wptr->value ) { sptr->who = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"pass") ))
		{
			if ( wptr->value ) { sptr->pass = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"identity") ))
		{
			if ( wptr->value ) { sptr->identity = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"zone") ))
		{
			if ( wptr->value ) { sptr->zone = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"price") ))
		{
			if ( wptr->value ) { sptr->price = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"rating") ))
		{
			if ( wptr->value ) { sptr->rating = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"operator") ))
		{
			if ( wptr->value ) { sptr->operator = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"pid") ))
		{
			if ( wptr->value ) { sptr->pid = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ p u b l i c a t i o n 	*/
/*	------------------------------------------------------	*/
public int rest_occi_cords_publication(FILE * fh,struct cords_publication * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.remote='%s'\r\n",prefix,nptr,(sptr->remote?sptr->remote:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.what='%s'\r\n",prefix,nptr,(sptr->what?sptr->what:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.where='%s'\r\n",prefix,nptr,(sptr->where?sptr->where:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.why='%s'\r\n",prefix,nptr,(sptr->why?sptr->why:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.when='%u'\r\n",prefix,nptr,sptr->when);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.uptime='%u'\r\n",prefix,nptr,sptr->uptime);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.who='%s'\r\n",prefix,nptr,(sptr->who?sptr->who:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.pass='%s'\r\n",prefix,nptr,(sptr->pass?sptr->pass:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.identity='%s'\r\n",prefix,nptr,(sptr->identity?sptr->identity:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.zone='%s'\r\n",prefix,nptr,(sptr->zone?sptr->zone:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.price='%s'\r\n",prefix,nptr,(sptr->price?sptr->price:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.rating='%s'\r\n",prefix,nptr,(sptr->rating?sptr->rating:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.operator='%s'\r\n",prefix,nptr,(sptr->operator?sptr->operator:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.pid='%u'\r\n",prefix,nptr,sptr->pid);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _publication_c_ */
