[[[cog import codegen.cog_common as t; t.init_models(model_dir, cog_category_file) ]]]
[[[end]]]
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
#ifndef _
[[[cog t.filename_root()]]]
[[[end]]] 
_c_
#define _
[[[cog t.filename_root()]]]
[[[end]]] 
_c_

#include "element.h"

#include "
[[[cog t.filename_root()]]]
[[[end]]] 
.h"
#include "
[[[cog t.filename_root()]]]
[[[end]]] 
_occi_filter.h"

/*	----------------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ p u b l i c a t i o n 	*/
/*	----------------------------------------------------	*/
public struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * liberate_
[[[cog t.category_name()]]]
[[[end]]] 
(struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * sptr)
{
	if ( sptr )
	{
		[[[cog t.liberate()]]]
		[[[end]]]
		sptr = liberate( sptr );
	}
	return((struct 
[[[cog t.category_name()]]]
[[[end]]] 
 *) 0);

}

/*	----------------------------------------------	*/
/*	r e s e t _ c o r d s _ p u b l i c a t i o n 	*/
/*	----------------------------------------------	*/
public struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * reset_
[[[cog t.category_name()]]]
[[[end]]] 
(struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * sptr)
{
	if ( sptr )
	{
		[[[cog t.reset()]]]
		[[[end]]]
	}
	return(sptr);

}

/*	----------------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ p u b l i c a t i o n 	*/
/*	----------------------------------------------------	*/
public struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * allocate_
[[[cog t.category_name()]]]
[[[end]]] 
()
{
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * sptr;
	if (!( sptr = allocate( sizeof( struct 
[[[cog t.category_name()]]]
[[[end]]] 
 ) ) ))
		return( sptr );
	else	return( reset_
[[[cog t.category_name()]]]
[[[end]]] 
(sptr) );
}
[[[cog t.authorization_special_code()]]]
[[[end]]]
 
/*	----------------------------------------------	*/
/*	x m l i n _ c o r d s _ p u b l i c a t i o n 	*/
/*	----------------------------------------------	*/
public int xmlin_
[[[cog t.category_name()]]]
[[[end]]] 
(struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * sptr,struct xml_element * eptr)
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
		[[[cog t.xmlin()]]]
		[[[end]]]
	}
	return(0);

}

/*	------------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ p u b l i c a t i o n 	*/
/*	------------------------------------------------------	*/
public int rest_occi_
[[[cog t.category_name()]]]
[[[end]]] 
(FILE * fh,struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	[[[cog t.rest_print()]]]
	[[[end]]]
	return(0);

}

#endif	/* _
[[[cog t.filename_root()]]]
[[[end]]] 
_c_ */
