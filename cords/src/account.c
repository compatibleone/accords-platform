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

#ifndef _account_c_
#define _account_c_

#include "element.h"

#include "account.h"

/*	--------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ a c c o u n t 	*/
/*	--------------------------------------------	*/
public struct cords_account * liberate_cords_account(struct cords_account * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->number )
			 sptr->number = liberate(sptr->number);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->date )
			 sptr->date = liberate(sptr->date);
		if ( sptr->security )
			 sptr->security = liberate(sptr->security);
		if ( sptr->legal )
			 sptr->legal = liberate(sptr->legal);
		sptr = liberate( sptr );
	}
	return((struct cords_account *) 0);

}

/*	--------------------------------------	*/
/*	r e s e t _ c o r d s _ a c c o u n t 	*/
/*	--------------------------------------	*/
public struct cords_account * reset_cords_account(struct cords_account * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->number = (char*) 0;
		sptr->name = (char*) 0;
		sptr->date = (char*) 0;
		sptr->security = (char*) 0;
		sptr->legal = (char*) 0;
		sptr->users =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	--------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ a c c o u n t 	*/
/*	--------------------------------------------	*/
public struct cords_account * allocate_cords_account()
{
	struct cords_account * sptr;
	if (!( sptr = allocate( sizeof( struct cords_account ) ) ))
		return( sptr );
	else	return( reset_cords_account(sptr) );
}

/*	--------------------------------------	*/
/*	x m l i n _ c o r d s _ a c c o u n t 	*/
/*	--------------------------------------	*/
public int xmlin_cords_account(struct cords_account * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"name") ))
		{
			if ( wptr->value ) { sptr->name = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"date") ))
		{
			if ( wptr->value ) { sptr->date = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"security") ))
		{
			if ( wptr->value ) { sptr->security = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"legal") ))
		{
			if ( wptr->value ) { sptr->legal = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"users") ))
		{
			if ( wptr->value ) { sptr->users = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ a c c o u n t 	*/
/*	----------------------------------------------	*/
public int rest_occi_cords_account(FILE * fh,struct cords_account * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.number='%s'\r\n",prefix,nptr,(sptr->number?sptr->number:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.date='%s'\r\n",prefix,nptr,(sptr->date?sptr->date:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.security='%s'\r\n",prefix,nptr,(sptr->security?sptr->security:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.legal='%s'\r\n",prefix,nptr,(sptr->legal?sptr->legal:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.users='%u'\r\n",prefix,nptr,sptr->users);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _account_c_ */
