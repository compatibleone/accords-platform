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

#ifndef _invoice_c_
#define _invoice_c_

#include "element.h"

#include "invoice.h"

/*	--------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ i n v o i c e 	*/
/*	--------------------------------------------	*/
public struct cords_invoice * liberate_cords_invoice(struct cords_invoice * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->number )
			 sptr->number = liberate(sptr->number);
		if ( sptr->account )
			 sptr->account = liberate(sptr->account);
		if ( sptr->date )
			 sptr->date = liberate(sptr->date);
		if ( sptr->authorization )
			 sptr->authorization = liberate(sptr->authorization);
		if ( sptr->total )
			 sptr->total = liberate(sptr->total);
		if ( sptr->taxe )
			 sptr->taxe = liberate(sptr->taxe);
		if ( sptr->reduction )
			 sptr->reduction = liberate(sptr->reduction);
		if ( sptr->rate )
			 sptr->rate = liberate(sptr->rate);
		sptr = liberate( sptr );
	}
	return((struct cords_invoice *) 0);

}

/*	--------------------------------------	*/
/*	r e s e t _ c o r d s _ i n v o i c e 	*/
/*	--------------------------------------	*/
public struct cords_invoice * reset_cords_invoice(struct cords_invoice * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->number = (char*) 0;
		sptr->account = (char*) 0;
		sptr->date = (char*) 0;
		sptr->authorization = (char*) 0;
		sptr->total = (char*) 0;
		sptr->taxe = (char*) 0;
		sptr->reduction = (char*) 0;
		sptr->rate = (char*) 0;
		sptr->lines =  0;
		sptr->status =  0;
	}
	return(sptr);

}

/*	--------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ i n v o i c e 	*/
/*	--------------------------------------------	*/
public struct cords_invoice * allocate_cords_invoice()
{
	struct cords_invoice * sptr;
	if (!( sptr = allocate( sizeof( struct cords_invoice ) ) ))
		return( sptr );
	else	return( reset_cords_invoice(sptr) );
}

/*	--------------------------------------	*/
/*	x m l i n _ c o r d s _ i n v o i c e 	*/
/*	--------------------------------------	*/
public int xmlin_cords_invoice(struct cords_invoice * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"number") ))
		{
			if ( wptr->value ) { sptr->number = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"account") ))
		{
			if ( wptr->value ) { sptr->account = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"date") ))
		{
			if ( wptr->value ) { sptr->date = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"authorization") ))
		{
			if ( wptr->value ) { sptr->authorization = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"total") ))
		{
			if ( wptr->value ) { sptr->total = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"taxe") ))
		{
			if ( wptr->value ) { sptr->taxe = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"reduction") ))
		{
			if ( wptr->value ) { sptr->reduction = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"rate") ))
		{
			if ( wptr->value ) { sptr->rate = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"lines") ))
		{
			if ( wptr->value ) { sptr->lines = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"status") ))
		{
			if ( wptr->value ) { sptr->status = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ i n v o i c e 	*/
/*	----------------------------------------------	*/
public int rest_occi_cords_invoice(FILE * fh,struct cords_invoice * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.number='%s'\r\n",prefix,nptr,(sptr->number?sptr->number:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.account='%s'\r\n",prefix,nptr,(sptr->account?sptr->account:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.date='%s'\r\n",prefix,nptr,(sptr->date?sptr->date:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.authorization='%s'\r\n",prefix,nptr,(sptr->authorization?sptr->authorization:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.total='%s'\r\n",prefix,nptr,(sptr->total?sptr->total:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.taxe='%s'\r\n",prefix,nptr,(sptr->taxe?sptr->taxe:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.reduction='%s'\r\n",prefix,nptr,(sptr->reduction?sptr->reduction:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.rate='%s'\r\n",prefix,nptr,(sptr->rate?sptr->rate:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.lines='%u'\r\n",prefix,nptr,sptr->lines);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.status='%u'\r\n",prefix,nptr,sptr->status);
	return(0);

}

#endif	/* _invoice_c_ */
