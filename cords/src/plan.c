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
#ifndef _plan_c_
#define _plan_c_

#include "element.h"

#include "plan.h"

/*	--------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ p l a n 	*/
/*	--------------------------------------	*/
public struct cords_plan * liberate_cords_plan(struct cords_plan * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->manifest )
			 sptr->manifest = liberate(sptr->manifest);
		if ( sptr->validation )
			 sptr->validation = liberate(sptr->validation);
		if ( sptr->tarification )
			 sptr->tarification = liberate(sptr->tarification);
		if ( sptr->access )
			 sptr->access = liberate(sptr->access);
		sptr = liberate( sptr );
	}
	return((struct cords_plan *) 0);

}

/*	--------------------------------	*/
/*	r e s e t _ c o r d s _ p l a n 	*/
/*	--------------------------------	*/
public struct cords_plan * reset_cords_plan(struct cords_plan * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->manifest = (char*) 0;
		sptr->validation = (char*) 0;
		sptr->tarification = (char*) 0;
		sptr->access = (char*) 0;
		sptr->created =  0;
		sptr->services =  0;
		sptr->status =  0;
	}
	return(sptr);

}

/*	--------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ p l a n 	*/
/*	--------------------------------------	*/
public struct cords_plan * allocate_cords_plan()
{
	struct cords_plan * sptr;
	if (!( sptr = allocate( sizeof( struct cords_plan ) ) ))
		return( sptr );
	else	return( reset_cords_plan(sptr) );
}

/*	--------------------------------	*/
/*	x m l i n _ c o r d s _ p l a n 	*/
/*	--------------------------------	*/
public int xmlin_cords_plan(struct cords_plan * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"manifest") ))
		{
			if ( wptr->value ) { sptr->manifest = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"validation") ))
		{
			if ( wptr->value ) { sptr->validation = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"tarification") ))
		{
			if ( wptr->value ) { sptr->tarification = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"access") ))
		{
			if ( wptr->value ) { sptr->access = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"created") ))
		{
			if ( wptr->value ) { sptr->created = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"services") ))
		{
			if ( wptr->value ) { sptr->services = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"status") ))
		{
			if ( wptr->value ) { sptr->status = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ p l a n 	*/
/*	----------------------------------------	*/
public int rest_occi_cords_plan(FILE * fh,struct cords_plan * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.manifest='%s'\r\n",prefix,nptr,(sptr->manifest?sptr->manifest:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.validation='%s'\r\n",prefix,nptr,(sptr->validation?sptr->validation:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.tarification='%s'\r\n",prefix,nptr,(sptr->tarification?sptr->tarification:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.access='%s'\r\n",prefix,nptr,(sptr->access?sptr->access:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.created='%u'\r\n",prefix,nptr,sptr->created);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.services='%u'\r\n",prefix,nptr,sptr->services);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.status='%u'\r\n",prefix,nptr,sptr->status);
	return(0);

}

#endif	/* _plan_c_ */
