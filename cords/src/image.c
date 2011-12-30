/* ---------------------------------------------------------------------------- */
/* Advanced Capabilities for Compatible One Resources Delivery System - ACCORDS	*/
/* (C) 2011 by Iain James Marshall <ijm667@hotmail.com>				*/
/* ---------------------------------------------------------------------------- */
/*										*/
/* This is free software; you can redistribute it and/or modify it		*/
/* under the terms of the GNU Lesser General Public License as			*/
/* published by the Free Software Foundation; either version 2.1 of		*/
/* the License, or (at your option) any later version.				*/
/*										*/
/* This software is distributed in the hope that it will be useful,		*/
/* but WITHOUT ANY WARRANTY; without even the implied warranty of		*/
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU		*/
/* Lesser General Public License for more details.				*/
/*										*/
/* You should have received a copy of the GNU Lesser General Public		*/
/* License along with this software; if not, write to the Free			*/
/* Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA		*/
/* 02110-1301 USA, or see the FSF site: http://www.fsf.org.			*/
/*										*/
/* ---------------------------------------------------------------------------- */

/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _image_c_
#define _image_c_

#include "element.h"

#include "image.h"

/*	----------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ i m a g e 	*/
/*	----------------------------------------	*/
public struct cords_image * liberate_cords_image(struct cords_image * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->system )
			 sptr->system = liberate(sptr->system);
		if ( sptr->price )
			 sptr->price = liberate(sptr->price);
		if ( sptr->created )
			 sptr->created = liberate(sptr->created);
		if ( sptr->updated )
			 sptr->updated = liberate(sptr->updated);
		if ( sptr->firstpack )
			 sptr->firstpack = liberate_cords_package(sptr->firstpack);
		if ( sptr->lastpack )
			 sptr->lastpack = liberate_cords_package(sptr->lastpack);
		sptr = liberate( sptr );
	}
	return((struct cords_image *) 0);

}

/*	----------------------------------	*/
/*	r e s e t _ c o r d s _ i m a g e 	*/
/*	----------------------------------	*/
public struct cords_image * reset_cords_image(struct cords_image * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->system = (char*) 0;
		sptr->price = (char*) 0;
		sptr->packages =  0;
		sptr->created = (char*) 0;
		sptr->updated = (char*) 0;
		sptr->firstpack = (struct cords_package*) 0;
		sptr->lastpack = (struct cords_package*) 0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	----------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ i m a g e 	*/
/*	----------------------------------------	*/
public struct cords_image * allocate_cords_image()
{
	struct cords_image * sptr;
	if (!( sptr = allocate( sizeof( struct cords_image ) ) ))
		return( sptr );
	else	return( reset_cords_image(sptr) );
}

/*	----------------------------------	*/
/*	x m l i n _ c o r d s _ i m a g e 	*/
/*	----------------------------------	*/
public int xmlin_cords_image(struct cords_image * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"system") ))
		{
			if ( wptr->value ) { sptr->system = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"price") ))
		{
			if ( wptr->value ) { sptr->price = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"packages") ))
		{
			if ( wptr->value ) { sptr->packages = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"created") ))
		{
			if ( wptr->value ) { sptr->created = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"updated") ))
		{
			if ( wptr->value ) { sptr->updated = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ i m a g e 	*/
/*	------------------------------------------	*/
public int rest_occi_cords_image(FILE * fh,struct cords_image * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.system='%s'\r\n",prefix,nptr,(sptr->system?sptr->system:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.price='%s'\r\n",prefix,nptr,(sptr->price?sptr->price:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.packages='%u'\r\n",prefix,nptr,sptr->packages);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.created='%s'\r\n",prefix,nptr,(sptr->created?sptr->created:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.updated='%s'\r\n",prefix,nptr,(sptr->updated?sptr->updated:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _image_c_ */
