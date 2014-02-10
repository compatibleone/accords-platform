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
#ifndef _occicat_c_
#define _occicat_c_

#include "occicat.h"

public	char *	occi_html_body(struct occi_category * cptr,struct rest_header  * hptr );

/*	--------------------------------------------	*/
/*	l i b e r a t e _ o c c i _ c a t e g o r y 	*/
/*	--------------------------------------------	*/
public struct occi_category * liberate_occi_category(struct occi_category * sptr)
{
	struct occi_attribute * mptr;
	struct occi_action * aptr;
	if ( sptr )
	{
		while ((mptr = sptr->first) != (struct occi_attribute *) 0)
		{
			sptr->first = mptr->next;
			mptr = liberate_occi_attribute( mptr );
		}
		while ((aptr = sptr->firstact) != (struct occi_action *) 0)
		{
			sptr->firstact = aptr->next;
			aptr = liberate_occi_action( aptr );
		}
		if ( sptr->domain )
			 sptr->domain = liberate(sptr->domain);
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->scheme )
			 sptr->scheme = liberate(sptr->scheme);
		if ( sptr->version )
			 sptr->version = liberate(sptr->version);
		if ( sptr->class )
			 sptr->class = liberate(sptr->class);
		if ( sptr->title )
			 sptr->title = liberate(sptr->title);
		if ( sptr->rel )
			 sptr->rel = liberate(sptr->rel);
		if ( sptr->location )
			 sptr->location = liberate(sptr->location);
		if ( sptr->price )
			 sptr->price = liberate(sptr->price);
		if ( sptr->rating )
			 sptr->rating = liberate(sptr->rating);
		if ( sptr->interface )
			sptr->interface = liberate( sptr->interface );
		sptr = liberate( sptr );
	}
	return((struct occi_category *) 0);

}

/*	--------------------------------------	*/
/*	r e s e t _ o c c i _ c a t e g o r y 	*/
/*	--------------------------------------	*/
public struct occi_category * reset_occi_category(struct occi_category * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct occi_category*) 0;
		sptr->next = (struct occi_category*) 0;
		sptr->first = (struct occi_attribute*) 0;
		sptr->last = (struct occi_attribute*) 0;
		sptr->firstact = (struct occi_action*) 0;
		sptr->lastact = (struct occi_action*) 0;
		sptr->domain = (char*) 0;
		sptr->id = (char*) 0;
		sptr->scheme = (char*) 0;
		sptr->version = (char*) 0;
		sptr->class = (char*) 0;
		sptr->title = (char*) 0;
		sptr->rel = (char*) 0;
		sptr->location = (char*) 0;
		sptr->price = (char*) 0;
		sptr->rating = (char*) 0;
		sptr->access =  0;
		sptr->interface = (void*) 0;
		sptr->callback = (void*) 0;
		sptr->payload = (void*) 0;
		sptr->html_rendering = occi_html_body;
	}
	return(sptr);

}

/*	--------------------------------------------	*/
/*	a l l o c a t e _ o c c i _ c a t e g o r y 	*/
/*	--------------------------------------------	*/
public struct occi_category * allocate_occi_category()
{
	struct occi_category * sptr;
	if (!( sptr = allocate( sizeof( struct occi_category ) ) ))
		return( sptr );
	else	return( reset_occi_category(sptr) );
}

#endif	/* _occicat_c_ */
