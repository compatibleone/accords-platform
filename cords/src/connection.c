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
#ifndef _connection_c_
#define _connection_c_

#include "element.h"

#include "connection.h"

/*	--------------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ c o n n e c t i o n 	*/
/*	--------------------------------------------------	*/
public struct cords_connection * liberate_cords_connection(struct cords_connection * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->start )
			 sptr->start = liberate(sptr->start);
		if ( sptr->finish )
			 sptr->finish = liberate(sptr->finish);
		if ( sptr->account )
			 sptr->account = liberate(sptr->account);
		if ( sptr->session )
			 sptr->session = liberate(sptr->session);
		if ( sptr->control )
			 sptr->control = liberate(sptr->control);
		if ( sptr->monitor )
			 sptr->monitor = liberate(sptr->monitor);
		sptr = liberate( sptr );
	}
	return((struct cords_connection *) 0);

}

/*	--------------------------------------------	*/
/*	r e s e t _ c o r d s _ c o n n e c t i o n 	*/
/*	--------------------------------------------	*/
public struct cords_connection * reset_cords_connection(struct cords_connection * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->start = (char*) 0;
		sptr->finish = (char*) 0;
		sptr->account = (char*) 0;
		sptr->session = (char*) 0;
		sptr->control = (char*) 0;
		sptr->monitor = (char*) 0;
		sptr->pid =  0;
		sptr->probes =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	--------------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ c o n n e c t i o n 	*/
/*	--------------------------------------------------	*/
public struct cords_connection * allocate_cords_connection()
{
	struct cords_connection * sptr;
	if (!( sptr = allocate( sizeof( struct cords_connection ) ) ))
		return( sptr );
	else	return( reset_cords_connection(sptr) );
}

/*	--------------------------------------------	*/
/*	x m l i n _ c o r d s _ c o n n e c t i o n 	*/
/*	--------------------------------------------	*/
public int xmlin_cords_connection(struct cords_connection * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"start") ))
		{
			if ( wptr->value ) { sptr->start = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"finish") ))
		{
			if ( wptr->value ) { sptr->finish = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"account") ))
		{
			if ( wptr->value ) { sptr->account = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"session") ))
		{
			if ( wptr->value ) { sptr->session = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"control") ))
		{
			if ( wptr->value ) { sptr->control = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"monitor") ))
		{
			if ( wptr->value ) { sptr->monitor = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"pid") ))
		{
			if ( wptr->value ) { sptr->pid = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"probes") ))
		{
			if ( wptr->value ) { sptr->probes = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ c o n n e c t i o n 	*/
/*	----------------------------------------------------	*/
public int rest_occi_cords_connection(FILE * fh,struct cords_connection * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.start='%s'\r\n",prefix,nptr,(sptr->start?sptr->start:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.finish='%s'\r\n",prefix,nptr,(sptr->finish?sptr->finish:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.account='%s'\r\n",prefix,nptr,(sptr->account?sptr->account:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.session='%s'\r\n",prefix,nptr,(sptr->session?sptr->session:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.control='%s'\r\n",prefix,nptr,(sptr->control?sptr->control:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.monitor='%s'\r\n",prefix,nptr,(sptr->monitor?sptr->monitor:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.pid='%u'\r\n",prefix,nptr,sptr->pid);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.probes='%u'\r\n",prefix,nptr,sptr->probes);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _connection_c_ */
