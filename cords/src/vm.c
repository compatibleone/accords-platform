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
#ifndef _vm_c_
#define _vm_c_

#include "element.h"

#include "vm.h"

/*	----------------------------------	*/
/*	l i b e r a t e _ c o r d s _ v m 	*/
/*	----------------------------------	*/
public struct cords_vm * liberate_cords_vm(struct cords_vm * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->image )
			 sptr->image = liberate(sptr->image);
		if ( sptr->provider )
			 sptr->provider = liberate(sptr->provider);
		if ( sptr->storage )
			 sptr->storage = liberate(sptr->storage);
		if ( sptr->reference )
			 sptr->reference = liberate(sptr->reference);
		sptr = liberate( sptr );
	}
	return((struct cords_vm *) 0);

}

/*	----------------------------	*/
/*	r e s e t _ c o r d s _ v m 	*/
/*	----------------------------	*/
public struct cords_vm * reset_cords_vm(struct cords_vm * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->status =  0;
		sptr->image = (char*) 0;
		sptr->provider = (char*) 0;
		sptr->storage = (char*) 0;
		sptr->reference = (char*) 0;
	}
	return(sptr);

}

/*	----------------------------------	*/
/*	a l l o c a t e _ c o r d s _ v m 	*/
/*	----------------------------------	*/
public struct cords_vm * allocate_cords_vm()
{
	struct cords_vm * sptr;
	if (!( sptr = allocate( sizeof( struct cords_vm ) ) ))
		return( sptr );
	else	return( reset_cords_vm(sptr) );
}

/*	----------------------------	*/
/*	x m l i n _ c o r d s _ v m 	*/
/*	----------------------------	*/
public int xmlin_cords_vm(struct cords_vm * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"status") ))
		{
			if ( wptr->value ) { sptr->status = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"image") ))
		{
			if ( wptr->value ) { sptr->image = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"provider") ))
		{
			if ( wptr->value ) { sptr->provider = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"storage") ))
		{
			if ( wptr->value ) { sptr->storage = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"reference") ))
		{
			if ( wptr->value ) { sptr->reference = allocate_string(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ v m 	*/
/*	------------------------------------	*/
public int rest_occi_cords_vm(FILE * fh,struct cords_vm * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.status='%u'\r\n",prefix,nptr,sptr->status);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.image='%s'\r\n",prefix,nptr,(sptr->image?sptr->image:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.provider='%s'\r\n",prefix,nptr,(sptr->provider?sptr->provider:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.storage='%s'\r\n",prefix,nptr,(sptr->storage?sptr->storage:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.reference='%s'\r\n",prefix,nptr,(sptr->reference?sptr->reference:""));
	return(0);

}

#endif	/* _vm_c_ */
