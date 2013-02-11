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
#ifndef _paas_environment_c_
#define _paas_environment_c_

#include "element.h"

#include "paas_environment.h"

/*	--------------------------------------------------	*/
/*	l i b e r a t e _ p a a s _ e n v i r o n m e n t 	*/
/*	--------------------------------------------------	*/
public struct paas_environment * liberate_paas_environment(struct paas_environment * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->template )
			 sptr->template = liberate(sptr->template);
		if ( sptr->paas_environment_template )
			 sptr->paas_environment_template = liberate(sptr->paas_environment_template);
		sptr = liberate( sptr );
	}
	return((struct paas_environment *) 0);

}

/*	--------------------------------------------	*/
/*	r e s e t _ p a a s _ e n v i r o n m e n t 	*/
/*	--------------------------------------------	*/
public struct paas_environment * reset_paas_environment(struct paas_environment * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->template = (char*) 0;
		sptr->paas_environment_template = (char*) 0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	--------------------------------------------------	*/
/*	a l l o c a t e _ p a a s _ e n v i r o n m e n t 	*/
/*	--------------------------------------------------	*/
public struct paas_environment * allocate_paas_environment()
{
	struct paas_environment * sptr;
	if (!( sptr = allocate( sizeof( struct paas_environment ) ) ))
		return( sptr );
	else	return( reset_paas_environment(sptr) );
}

/*	--------------------------------------------	*/
/*	x m l i n _ p a a s _ e n v i r o n m e n t 	*/
/*	--------------------------------------------	*/
public int xmlin_paas_environment(struct paas_environment * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"template") ))
		{
			if ( wptr->value ) { sptr->template = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"paas_environment_template") ))
		{
			if ( wptr->value ) { sptr->paas_environment_template = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------------------	*/
/*	r e s t _ o c c i _ p a a s _ e n v i r o n m e n t 	*/
/*	----------------------------------------------------	*/
public int rest_occi_paas_environment(FILE * fh,struct paas_environment * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.template='%s'\r\n",prefix,nptr,(sptr->template?sptr->template:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.paas_environment_template='%s'\r\n",prefix,nptr,(sptr->paas_environment_template?sptr->paas_environment_template:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _paas_environment_cpaas_environment_c_ */
