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
#ifndef _transaction_c_
#define _transaction_c_

#include "element.h"

#include "transaction.h"

/*	----------------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ t r a n s a c t i o n 	*/
/*	----------------------------------------------------	*/
public struct cords_transaction * liberate_cords_transaction(struct cords_transaction * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->authorization )
			 sptr->authorization = liberate(sptr->authorization);
		if ( sptr->source )
			 sptr->source = liberate(sptr->source);
		if ( sptr->account )
			 sptr->account = liberate(sptr->account);
		if ( sptr->description )
			 sptr->description = liberate(sptr->description);
		if ( sptr->reference )
			 sptr->reference = liberate(sptr->reference);
		if ( sptr->price )
			 sptr->price = liberate(sptr->price);
		sptr = liberate( sptr );
	}
	return((struct cords_transaction *) 0);

}

/*	----------------------------------------------	*/
/*	r e s e t _ c o r d s _ t r a n s a c t i o n 	*/
/*	----------------------------------------------	*/
public struct cords_transaction * reset_cords_transaction(struct cords_transaction * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->authorization = (char*) 0;
		sptr->source = (char*) 0;
		sptr->account = (char*) 0;
		sptr->description = (char*) 0;
		sptr->reference = (char*) 0;
		sptr->price = (char*) 0;
		sptr->when =  0;
		sptr->validity =  0;
		sptr->type =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	----------------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ t r a n s a c t i o n 	*/
/*	----------------------------------------------------	*/
public struct cords_transaction * allocate_cords_transaction()
{
	struct cords_transaction * sptr;
	if (!( sptr = allocate( sizeof( struct cords_transaction ) ) ))
		return( sptr );
	else	return( reset_cords_transaction(sptr) );
}

/*	----------------------------------------------	*/
/*	x m l i n _ c o r d s _ t r a n s a c t i o n 	*/
/*	----------------------------------------------	*/
public int xmlin_cords_transaction(struct cords_transaction * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"authorization") ))
		{
			if ( wptr->value ) { sptr->authorization = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"source") ))
		{
			if ( wptr->value ) { sptr->source = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"account") ))
		{
			if ( wptr->value ) { sptr->account = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"description") ))
		{
			if ( wptr->value ) { sptr->description = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"reference") ))
		{
			if ( wptr->value ) { sptr->reference = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"price") ))
		{
			if ( wptr->value ) { sptr->price = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"when") ))
		{
			if ( wptr->value ) { sptr->when = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"validity") ))
		{
			if ( wptr->value ) { sptr->validity = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"type") ))
		{
			if ( wptr->value ) { sptr->type = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ t r a n s a c t i o n 	*/
/*	------------------------------------------------------	*/
public int rest_occi_cords_transaction(FILE * fh,struct cords_transaction * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.authorization='%s'\r\n",prefix,nptr,(sptr->authorization?sptr->authorization:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.source='%s'\r\n",prefix,nptr,(sptr->source?sptr->source:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.account='%s'\r\n",prefix,nptr,(sptr->account?sptr->account:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.description='%s'\r\n",prefix,nptr,(sptr->description?sptr->description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.reference='%s'\r\n",prefix,nptr,(sptr->reference?sptr->reference:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.price='%s'\r\n",prefix,nptr,(sptr->price?sptr->price:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.when='%u'\r\n",prefix,nptr,sptr->when);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.validity='%u'\r\n",prefix,nptr,sptr->validity);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.type='%u'\r\n",prefix,nptr,sptr->type);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _transaction_c_ */
