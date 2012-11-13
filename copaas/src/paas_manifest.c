/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>     */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License"); 	*/
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at 				*/
/*  									*/
/*  http://www.apache.org/licenses/LICENSE-2.0 				*/
/*  									*/
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 	*/
/* implied. 								*/
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License. 					*/
/* -------------------------------------------------------------------- */

/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _paas_manifest_c_
#define _paas_manifest_c_

#include "element.h"

#include "paas_manifest.h"

/*	--------------------------------------------	*/
/*	l i b e r a t e _ p a a s _ m a n i f e s t 	*/
/*	--------------------------------------------	*/
public struct paas_manifest * liberate_paas_manifest(struct paas_manifest * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->access )
			 sptr->access = liberate(sptr->access);
		if ( sptr->scope )
			 sptr->scope = liberate(sptr->scope);
		if ( sptr->paas_description )
			 sptr->paas_description = liberate(sptr->paas_description);
		if ( sptr->paas_application )
			 sptr->paas_application = liberate(sptr->paas_application);
		sptr = liberate( sptr );
	}
	return((struct paas_manifest *) 0);

}

/*	--------------------------------------	*/
/*	r e s e t _ p a a s _ m a n i f e s t 	*/
/*	--------------------------------------	*/
public struct paas_manifest * reset_paas_manifest(struct paas_manifest * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->access = (char*) 0;
		sptr->scope = (char*) 0;
		sptr->paas_description = (char*) 0;
		sptr->paas_application = (char*) 0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	--------------------------------------------	*/
/*	a l l o c a t e _ p a a s _ m a n i f e s t 	*/
/*	--------------------------------------------	*/
public struct paas_manifest * allocate_paas_manifest()
{
	struct paas_manifest * sptr;
	if (!( sptr = allocate( sizeof( struct paas_manifest ) ) ))
		return( sptr );
	else	return( reset_paas_manifest(sptr) );
}

/*	--------------------------------------	*/
/*	x m l i n _ p a a s _ m a n i f e s t 	*/
/*	--------------------------------------	*/
public int xmlin_paas_manifest(struct paas_manifest * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"access") ))
		{
			if ( wptr->value ) { sptr->access = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"scope") ))
		{
			if ( wptr->value ) { sptr->scope = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"paas_description") ))
		{
			if ( wptr->value ) { sptr->paas_description = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"paas_application") ))
		{
			if ( wptr->value ) { sptr->paas_application = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------------	*/
/*	r e s t _ o c c i _ p a a s _ m a n i f e s t 	*/
/*	----------------------------------------------	*/
public int rest_occi_paas_manifest(FILE * fh,struct paas_manifest * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.access='%s'\r\n",prefix,nptr,(sptr->access?sptr->access:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.scope='%s'\r\n",prefix,nptr,(sptr->scope?sptr->scope:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.paas_description='%s'\r\n",prefix,nptr,(sptr->paas_description?sptr->paas_description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.paas_application='%s'\r\n",prefix,nptr,(sptr->paas_application?sptr->paas_application:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _paas_manifest_cpaas_manifest_c_ */
