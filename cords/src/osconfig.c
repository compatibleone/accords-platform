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
#ifndef _osconfig_c_
#define _osconfig_c_

#include "element.h"

#include "osconfig.h"

/*	------------------------------------	*/
/*	l i b e r a t e _ o s _ c o n f i g 	*/
/*	------------------------------------	*/
public struct os_config * liberate_os_config(struct os_config * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->description )
			 sptr->description = liberate(sptr->description);
		if ( sptr->user )
			 sptr->user = liberate(sptr->user);
		if ( sptr->password )
			 sptr->password = liberate(sptr->password);
		if ( sptr->authenticate )
			 sptr->authenticate = liberate(sptr->authenticate);
		if ( sptr->agent )
			 sptr->agent = liberate(sptr->agent);
		if ( sptr->host )
			 sptr->host = liberate(sptr->host);
		if ( sptr->version )
			 sptr->version = liberate(sptr->version);
		if ( sptr->namespace )
			 sptr->namespace = liberate(sptr->namespace);
		if ( sptr->base )
			 sptr->base = liberate(sptr->base);
		if ( sptr->tls )
			 sptr->tls = liberate(sptr->tls);
		sptr = liberate( sptr );
	}
	return((struct os_config *) 0);

}

/*	------------------------------	*/
/*	r e s e t _ o s _ c o n f i g 	*/
/*	------------------------------	*/
public struct os_config * reset_os_config(struct os_config * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->description = (char*) 0;
		sptr->user = (char*) 0;
		sptr->password = (char*) 0;
		sptr->authenticate = (char*) 0;
		sptr->agent = (char*) 0;
		sptr->host = (char*) 0;
		sptr->version = (char*) 0;
		sptr->namespace = (char*) 0;
		sptr->base = (char*) 0;
		sptr->tls = (char*) 0;
		sptr->current =  0;
	}
	return(sptr);

}

/*	------------------------------------	*/
/*	a l l o c a t e _ o s _ c o n f i g 	*/
/*	------------------------------------	*/
public struct os_config * allocate_os_config()
{
	struct os_config * sptr;
	if (!( sptr = allocate( sizeof( struct os_config ) ) ))
		return( sptr );
	else	return( reset_os_config(sptr) );
}

/*	------------------------------	*/
/*	x m l i n _ o s _ c o n f i g 	*/
/*	------------------------------	*/
public int xmlin_os_config(struct os_config * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"description") ))
		{
			if ( wptr->value ) { sptr->description = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"user") ))
		{
			if ( wptr->value ) { sptr->user = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"password") ))
		{
			if ( wptr->value ) { sptr->password = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"authenticate") ))
		{
			if ( wptr->value ) { sptr->authenticate = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"agent") ))
		{
			if ( wptr->value ) { sptr->agent = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"host") ))
		{
			if ( wptr->value ) { sptr->host = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"version") ))
		{
			if ( wptr->value ) { sptr->version = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"namespace") ))
		{
			if ( wptr->value ) { sptr->namespace = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"base") ))
		{
			if ( wptr->value ) { sptr->base = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"tls") ))
		{
			if ( wptr->value ) { sptr->tls = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"current") ))
		{
			if ( wptr->value ) { sptr->current = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	--------------------------------------	*/
/*	r e s t _ o c c i _ o s _ c o n f i g 	*/
/*	--------------------------------------	*/
public int rest_occi_os_config(FILE * fh,struct os_config * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.description='%s'\r\n",prefix,nptr,(sptr->description?sptr->description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.user='%s'\r\n",prefix,nptr,(sptr->user?sptr->user:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.password='%s'\r\n",prefix,nptr,(sptr->password?sptr->password:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.authenticate='%s'\r\n",prefix,nptr,(sptr->authenticate?sptr->authenticate:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.agent='%s'\r\n",prefix,nptr,(sptr->agent?sptr->agent:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.host='%s'\r\n",prefix,nptr,(sptr->host?sptr->host:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.version='%s'\r\n",prefix,nptr,(sptr->version?sptr->version:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.namespace='%s'\r\n",prefix,nptr,(sptr->namespace?sptr->namespace:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.base='%s'\r\n",prefix,nptr,(sptr->base?sptr->base:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.tls='%s'\r\n",prefix,nptr,(sptr->tls?sptr->tls:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.current='%u'\r\n",prefix,nptr,sptr->current);
	return(0);

}

#endif	/* _osconfig_cosconfig_c_ */
