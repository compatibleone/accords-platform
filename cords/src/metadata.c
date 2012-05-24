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
#ifndef _metadata_c_
#define _metadata_c_

#include "element.h"

#include "metadata.h"

/*	----------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ m e t a d a t a 	*/
/*	----------------------------------------------	*/
public struct cords_metadata * liberate_cords_metadata(struct cords_metadata * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->value )
			 sptr->value = liberate(sptr->value);
		if ( sptr->target )
			 sptr->target = liberate(sptr->target);
		sptr = liberate( sptr );
	}
	return((struct cords_metadata *) 0);

}

/*	----------------------------------------	*/
/*	r e s e t _ c o r d s _ m e t a d a t a 	*/
/*	----------------------------------------	*/
public struct cords_metadata * reset_cords_metadata(struct cords_metadata * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->state =  0;
		sptr->name = (char*) 0;
		sptr->value = (char*) 0;
		sptr->target = (char*) 0;
	}
	return(sptr);

}

/*	----------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ m e t a d a t a 	*/
/*	----------------------------------------------	*/
public struct cords_metadata * allocate_cords_metadata()
{
	struct cords_metadata * sptr;
	if (!( sptr = allocate( sizeof( struct cords_metadata ) ) ))
		return( sptr );
	else	return( reset_cords_metadata(sptr) );
}

/*	----------------------------------------	*/
/*	x m l i n _ c o r d s _ m e t a d a t a 	*/
/*	----------------------------------------	*/
public int xmlin_cords_metadata(struct cords_metadata * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"name") ))
		{
			if ( wptr->value ) { sptr->name = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"value") ))
		{
			if ( wptr->value ) { sptr->value = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"target") ))
		{
			if ( wptr->value ) { sptr->target = allocate_string(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ m e t a d a t a 	*/
/*	------------------------------------------------	*/
public int rest_occi_cords_metadata(FILE * fh,struct cords_metadata * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.value='%s'\r\n",prefix,nptr,(sptr->value?sptr->value:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.target='%s'\r\n",prefix,nptr,(sptr->target?sptr->target:""));
	return(0);

}

#endif	/* _metadata_c_ */
