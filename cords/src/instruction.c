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
#ifndef _instruction_c_
#define _instruction_c_

#include "element.h"

#include "instruction.h"

/*	----------------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ i n s t r u c t i o n 	*/
/*	----------------------------------------------------	*/
public struct cords_instruction * liberate_cords_instruction(struct cords_instruction * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->provision )
			 sptr->provision = liberate(sptr->provision);
		if ( sptr->target )
			 sptr->target = liberate(sptr->target);
		if ( sptr->method )
			 sptr->method = liberate(sptr->method);
		if ( sptr->type )
			 sptr->type = liberate(sptr->type);
		if ( sptr->symbol )
			 sptr->symbol = liberate(sptr->symbol);
		if ( sptr->source )
			 sptr->source = liberate(sptr->source);
		if ( sptr->property )
			 sptr->property = liberate(sptr->property);
		if ( sptr->value )
			 sptr->value = liberate(sptr->value);
		sptr = liberate( sptr );
	}
	return((struct cords_instruction *) 0);

}

/*	----------------------------------------------	*/
/*	r e s e t _ c o r d s _ i n s t r u c t i o n 	*/
/*	----------------------------------------------	*/
public struct cords_instruction * reset_cords_instruction(struct cords_instruction * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->provision = (char*) 0;
		sptr->target = (char*) 0;
		sptr->method = (char*) 0;
		sptr->type = (char*) 0;
		sptr->symbol = (char*) 0;
		sptr->source = (char*) 0;
		sptr->property = (char*) 0;
		sptr->value = (char*) 0;
		sptr->status =  0;
	}
	return(sptr);

}

/*	----------------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ i n s t r u c t i o n 	*/
/*	----------------------------------------------------	*/
public struct cords_instruction * allocate_cords_instruction()
{
	struct cords_instruction * sptr;
	if (!( sptr = allocate( sizeof( struct cords_instruction ) ) ))
		return( sptr );
	else	return( reset_cords_instruction(sptr) );
}

/*	----------------------------------------------	*/
/*	x m l i n _ c o r d s _ i n s t r u c t i o n 	*/
/*	----------------------------------------------	*/
public int xmlin_cords_instruction(struct cords_instruction * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"provision") ))
		{
			if ( wptr->value ) { sptr->provision = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"target") ))
		{
			if ( wptr->value ) { sptr->target = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"method") ))
		{
			if ( wptr->value ) { sptr->method = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"type") ))
		{
			if ( wptr->value ) { sptr->type = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"symbol") ))
		{
			if ( wptr->value ) { sptr->symbol = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"source") ))
		{
			if ( wptr->value ) { sptr->source = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"property") ))
		{
			if ( wptr->value ) { sptr->property = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"value") ))
		{
			if ( wptr->value ) { sptr->value = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"status") ))
		{
			if ( wptr->value ) { sptr->status = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ i n s t r u c t i o n 	*/
/*	------------------------------------------------------	*/
public int rest_occi_cords_instruction(FILE * fh,struct cords_instruction * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.provision='%s'\r\n",prefix,nptr,(sptr->provision?sptr->provision:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.target='%s'\r\n",prefix,nptr,(sptr->target?sptr->target:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.method='%s'\r\n",prefix,nptr,(sptr->method?sptr->method:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.type='%s'\r\n",prefix,nptr,(sptr->type?sptr->type:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.symbol='%s'\r\n",prefix,nptr,(sptr->symbol?sptr->symbol:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.source='%s'\r\n",prefix,nptr,(sptr->source?sptr->source:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.property='%s'\r\n",prefix,nptr,(sptr->property?sptr->property:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.value='%s'\r\n",prefix,nptr,(sptr->value?sptr->value:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.status='%u'\r\n",prefix,nptr,sptr->status);
	return(0);

}

#endif	/* _instruction_c_ */
