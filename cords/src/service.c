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
#ifndef _service_c_
#define _service_c_

#include "element.h"

#include "service.h"

/*	--------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ s e r v i c e 	*/
/*	--------------------------------------------	*/
public struct cords_service * liberate_cords_service(struct cords_service * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->manifest )
			 sptr->manifest = liberate(sptr->manifest);
		if ( sptr->plan )
			 sptr->plan = liberate(sptr->plan);
		if ( sptr->account )
			 sptr->account = liberate(sptr->account);
		if ( sptr->sla )
			 sptr->sla = liberate(sptr->sla);
		if ( sptr->price )
			 sptr->price = liberate(sptr->price);
		if ( sptr->session )
			 sptr->session = liberate(sptr->session);
		sptr = liberate( sptr );
	}
	return((struct cords_service *) 0);

}

/*	--------------------------------------	*/
/*	r e s e t _ c o r d s _ s e r v i c e 	*/
/*	--------------------------------------	*/
public struct cords_service * reset_cords_service(struct cords_service * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->manifest = (char*) 0;
		sptr->plan = (char*) 0;
		sptr->account = (char*) 0;
		sptr->sla = (char*) 0;
		sptr->price = (char*) 0;
		sptr->session = (char*) 0;
		sptr->when =  0;
		sptr->contracts =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	--------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ s e r v i c e 	*/
/*	--------------------------------------------	*/
public struct cords_service * allocate_cords_service()
{
	struct cords_service * sptr;
	if (!( sptr = allocate( sizeof( struct cords_service ) ) ))
		return( sptr );
	else	return( reset_cords_service(sptr) );
}

/*	--------------------------------------	*/
/*	x m l i n _ c o r d s _ s e r v i c e 	*/
/*	--------------------------------------	*/
public int xmlin_cords_service(struct cords_service * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"plan") ))
		{
			if ( wptr->value ) { sptr->plan = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"account") ))
		{
			if ( wptr->value ) { sptr->account = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"sla") ))
		{
			if ( wptr->value ) { sptr->sla = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"price") ))
		{
			if ( wptr->value ) { sptr->price = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"session") ))
		{
			if ( wptr->value ) { sptr->session = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"when") ))
		{
			if ( wptr->value ) { sptr->when = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"contracts") ))
		{
			if ( wptr->value ) { sptr->contracts = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ s e r v i c e 	*/
/*	----------------------------------------------	*/
public int rest_occi_cords_service(FILE * fh,struct cords_service * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.manifest='%s'\r\n",prefix,nptr,(sptr->manifest?sptr->manifest:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.plan='%s'\r\n",prefix,nptr,(sptr->plan?sptr->plan:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.account='%s'\r\n",prefix,nptr,(sptr->account?sptr->account:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.sla='%s'\r\n",prefix,nptr,(sptr->sla?sptr->sla:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.price='%s'\r\n",prefix,nptr,(sptr->price?sptr->price:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.session='%s'\r\n",prefix,nptr,(sptr->session?sptr->session:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.when='%u'\r\n",prefix,nptr,sptr->when);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.contracts='%u'\r\n",prefix,nptr,sptr->contracts);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _service_c_ */
