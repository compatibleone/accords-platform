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
#ifndef _paas_application_configuration_template_c_
#define _paas_application_configuration_template_c_

#include "element.h"

#include "paas_application_configuration_template.h"

/*	------------------------------------------------------------------------------------------------	*/
/*	l i b e r a t e _ p a a s _ a p p l i c a t i o n _ c o n f i g u r a t i o n _ t e m p l a t e 	*/
/*	------------------------------------------------------------------------------------------------	*/
public struct paas_application_configuration_template * liberate_paas_application_configuration_template(struct paas_application_configuration_template * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->date_created )
			 sptr->date_created = liberate(sptr->date_created);
		if ( sptr->date_updated )
			 sptr->date_updated = liberate(sptr->date_updated);
		if ( sptr->description )
			 sptr->description = liberate(sptr->description);
		if ( sptr->uri )
			 sptr->uri = liberate(sptr->uri);
		sptr = liberate( sptr );
	}
	return((struct paas_application_configuration_template *) 0);

}

/*	------------------------------------------------------------------------------------------	*/
/*	r e s e t _ p a a s _ a p p l i c a t i o n _ c o n f i g u r a t i o n _ t e m p l a t e 	*/
/*	------------------------------------------------------------------------------------------	*/
public struct paas_application_configuration_template * reset_paas_application_configuration_template(struct paas_application_configuration_template * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->date_created = (char*) 0;
		sptr->date_updated = (char*) 0;
		sptr->description = (char*) 0;
		sptr->uri = (char*) 0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	------------------------------------------------------------------------------------------------	*/
/*	a l l o c a t e _ p a a s _ a p p l i c a t i o n _ c o n f i g u r a t i o n _ t e m p l a t e 	*/
/*	------------------------------------------------------------------------------------------------	*/
public struct paas_application_configuration_template * allocate_paas_application_configuration_template()
{
	struct paas_application_configuration_template * sptr;
	if (!( sptr = allocate( sizeof( struct paas_application_configuration_template ) ) ))
		return( sptr );
	else	return( reset_paas_application_configuration_template(sptr) );
}

/*	------------------------------------------------------------------------------------------	*/
/*	x m l i n _ p a a s _ a p p l i c a t i o n _ c o n f i g u r a t i o n _ t e m p l a t e 	*/
/*	------------------------------------------------------------------------------------------	*/
public int xmlin_paas_application_configuration_template(struct paas_application_configuration_template * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"date_created") ))
		{
			if ( wptr->value ) { sptr->date_created = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"date_updated") ))
		{
			if ( wptr->value ) { sptr->date_updated = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"description") ))
		{
			if ( wptr->value ) { sptr->description = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"uri") ))
		{
			if ( wptr->value ) { sptr->uri = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	--------------------------------------------------------------------------------------------------	*/
/*	r e s t _ o c c i _ p a a s _ a p p l i c a t i o n _ c o n f i g u r a t i o n _ t e m p l a t e 	*/
/*	--------------------------------------------------------------------------------------------------	*/
public int rest_occi_paas_application_configuration_template(FILE * fh,struct paas_application_configuration_template * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.date_created='%s'\r\n",prefix,nptr,(sptr->date_created?sptr->date_created:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.date_updated='%s'\r\n",prefix,nptr,(sptr->date_updated?sptr->date_updated:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.description='%s'\r\n",prefix,nptr,(sptr->description?sptr->description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.uri='%s'\r\n",prefix,nptr,(sptr->uri?sptr->uri:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _paas_application_configuration_template_cpaas_application_configuration_template_c_ */
