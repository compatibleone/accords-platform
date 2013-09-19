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

/* WARNING : this file has been generated and should not be modified by hand */

#include "element.h"
#include "standard.h"

#include "FILENAME_ROOT.h"
#include "FILENAME_ROOT_occi_filter.h"

/*	----------------------------------------------------	*/
/*	l i b e r a t e _ 
[[[cog t.split_category_name()]]]
[[[end]]] 
	*/
/*	----------------------------------------------------	*/
public struct CATEGORY_NAME * liberate_CATEGORY_NAME(struct CATEGORY_NAME * sptr)
{
	if ( sptr )
	{
		[[[cog t.liberate()]]]
		[[[end]]]
		sptr = liberate( sptr );
	}
	return((struct CATEGORY_NAME *) 0);

}

/*	----------------------------------------------	*/
/*	r e s e t _ 
[[[cog t.split_category_name()]]]
[[[end]]] 
	*/
/*	----------------------------------------------	*/
public struct CATEGORY_NAME * reset_CATEGORY_NAME(struct CATEGORY_NAME * sptr)
{
	if ( sptr )
	{
		[[[cog t.reset()]]]
		[[[end]]]
	}
	return(sptr);

}

/*	----------------------------------------------------	*/
/*	a l l o c a t e _ 
[[[cog t.split_category_name()]]]
[[[end]]] 
	*/
/*	----------------------------------------------------	*/
public struct CATEGORY_NAME * allocate_CATEGORY_NAME()
{
	struct CATEGORY_NAME * sptr;
	if (!( sptr = allocate( sizeof( struct CATEGORY_NAME ) ) ))
		return( sptr );
	else	return( reset_CATEGORY_NAME(sptr) );
}
 
/*	----------------------------------------------	*/
/*	x m l i n _ 
[[[cog t.split_category_name()]]]
[[[end]]] 
	*/
/*	----------------------------------------------	*/
public int xmlin_CATEGORY_NAME(struct CATEGORY_NAME * sptr,struct xml_element * eptr)
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
/*	r e s t _ o c c i _ 
[[[cog t.split_category_name()]]]
[[[end]]] 
	*/
/*	------------------------------------------------------	*/
public int rest_occi_CATEGORY_NAME(FILE * fh,struct CATEGORY_NAME * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	[[[cog t.rest_print()]]]
	[[[end]]]
	return(0);

}

unsigned CATEGORY_NAME_count_filters(struct CATEGORY_NAME_occi_filter *filter) {
    unsigned count = 0;
    [[[cog t.count_filters()]]]
    [[[end]]]
    return count;
}

void CATEGORY_NAME_free_id_list(CATEGORY_NAME_id_list *list) {
    int i;
    for(i = 0; i < list->count; i++) {
        liberate(list->ids[i]);
    }
    liberate(list->ids);
    list->ids = NULL;
    list->count = 0;
}

void free_CATEGORY_NAME_list(CATEGORY_NAME_list *list) {
    int i;
    for(i = 0; i < list->count; i++) {
        liberate_CATEGORY_NAME(list->FILENAME_ROOTs[i]);
    }
    liberate(list->FILENAME_ROOTs);
    list->FILENAME_ROOTs = NULL;
    list->count = 0;
}
