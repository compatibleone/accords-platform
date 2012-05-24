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
#ifndef _parameter_c_
#define _parameter_c_

#include "element.h"

#include "parameter.h"

/*	------------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ p a r a m e t e r 	*/
/*	------------------------------------------------	*/
public struct cords_parameter * liberate_cords_parameter(struct cords_parameter * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->service )
			 sptr->service = liberate(sptr->service);
		if ( sptr->contract )
			 sptr->contract = liberate(sptr->contract);
		if ( sptr->provider )
			 sptr->provider = liberate(sptr->provider);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->value )
			 sptr->value = liberate(sptr->value);
		sptr = liberate( sptr );
	}
	return((struct cords_parameter *) 0);

}

/*	------------------------------------------	*/
/*	r e s e t _ c o r d s _ p a r a m e t e r 	*/
/*	------------------------------------------	*/
public struct cords_parameter * reset_cords_parameter(struct cords_parameter * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->service = (char*) 0;
		sptr->contract = (char*) 0;
		sptr->provider = (char*) 0;
		sptr->name = (char*) 0;
		sptr->value = (char*) 0;
		sptr->type =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	------------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ p a r a m e t e r 	*/
/*	------------------------------------------------	*/
public struct cords_parameter * allocate_cords_parameter()
{
	struct cords_parameter * sptr;
	if (!( sptr = allocate( sizeof( struct cords_parameter ) ) ))
		return( sptr );
	else	return( reset_cords_parameter(sptr) );
}

/*	------------------------------------------	*/
/*	x m l i n _ c o r d s _ p a r a m e t e r 	*/
/*	------------------------------------------	*/
public int xmlin_cords_parameter(struct cords_parameter * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"service") ))
		{
			if ( wptr->value ) { sptr->service = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"contract") ))
		{
			if ( wptr->value ) { sptr->contract = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"provider") ))
		{
			if ( wptr->value ) { sptr->provider = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"name") ))
		{
			if ( wptr->value ) { sptr->name = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"value") ))
		{
			if ( wptr->value ) { sptr->value = allocate_string(wptr->value); }
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

/*	--------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ p a r a m e t e r 	*/
/*	--------------------------------------------------	*/
public int rest_occi_cords_parameter(FILE * fh,struct cords_parameter * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.service='%s'\r\n",prefix,nptr,(sptr->service?sptr->service:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.contract='%s'\r\n",prefix,nptr,(sptr->contract?sptr->contract:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.provider='%s'\r\n",prefix,nptr,(sptr->provider?sptr->provider:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.value='%s'\r\n",prefix,nptr,(sptr->value?sptr->value:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.type='%u'\r\n",prefix,nptr,sptr->type);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _parameter_c_ */
