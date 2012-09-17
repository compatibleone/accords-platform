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
#ifndef _paas_configuration_option_c_
#define _paas_configuration_option_c_

#include "element.h"

#include "paas_configuration_option.h"

/*	--------------------------------------------------------------------	*/
/*	l i b e r a t e _ p a a s _ c o n f i g u r a t i o n _ o p t i o n 	*/
/*	--------------------------------------------------------------------	*/
public struct paas_configuration_option * liberate_paas_configuration_option(struct paas_configuration_option * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->change_severity )
			 sptr->change_severity = liberate(sptr->change_severity);
		if ( sptr->max_length )
			 sptr->max_length = liberate(sptr->max_length);
		if ( sptr->max_value )
			 sptr->max_value = liberate(sptr->max_value);
		if ( sptr->min_value )
			 sptr->min_value = liberate(sptr->min_value);
		if ( sptr->regex )
			 sptr->regex = liberate(sptr->regex);
		if ( sptr->user_defined )
			 sptr->user_defined = liberate(sptr->user_defined);
		if ( sptr->value_options )
			 sptr->value_options = liberate(sptr->value_options);
		if ( sptr->namespace )
			 sptr->namespace = liberate(sptr->namespace);
		if ( sptr->option_name )
			 sptr->option_name = liberate(sptr->option_name);
		if ( sptr->value )
			 sptr->value = liberate(sptr->value);
		sptr = liberate( sptr );
	}
	return((struct paas_configuration_option *) 0);

}

/*	--------------------------------------------------------------	*/
/*	r e s e t _ p a a s _ c o n f i g u r a t i o n _ o p t i o n 	*/
/*	--------------------------------------------------------------	*/
public struct paas_configuration_option * reset_paas_configuration_option(struct paas_configuration_option * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->change_severity = (char*) 0;
		sptr->max_length = (char*) 0;
		sptr->max_value = (char*) 0;
		sptr->min_value = (char*) 0;
		sptr->regex = (char*) 0;
		sptr->user_defined = (char*) 0;
		sptr->value_options = (char*) 0;
		sptr->namespace = (char*) 0;
		sptr->option_name = (char*) 0;
		sptr->value = (char*) 0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	--------------------------------------------------------------------	*/
/*	a l l o c a t e _ p a a s _ c o n f i g u r a t i o n _ o p t i o n 	*/
/*	--------------------------------------------------------------------	*/
public struct paas_configuration_option * allocate_paas_configuration_option()
{
	struct paas_configuration_option * sptr;
	if (!( sptr = allocate( sizeof( struct paas_configuration_option ) ) ))
		return( sptr );
	else	return( reset_paas_configuration_option(sptr) );
}

/*	--------------------------------------------------------------	*/
/*	x m l i n _ p a a s _ c o n f i g u r a t i o n _ o p t i o n 	*/
/*	--------------------------------------------------------------	*/
public int xmlin_paas_configuration_option(struct paas_configuration_option * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"change_severity") ))
		{
			if ( wptr->value ) { sptr->change_severity = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"max_length") ))
		{
			if ( wptr->value ) { sptr->max_length = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"max_value") ))
		{
			if ( wptr->value ) { sptr->max_value = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"min_value") ))
		{
			if ( wptr->value ) { sptr->min_value = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"regex") ))
		{
			if ( wptr->value ) { sptr->regex = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"user_defined") ))
		{
			if ( wptr->value ) { sptr->user_defined = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"value_options") ))
		{
			if ( wptr->value ) { sptr->value_options = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"namespace") ))
		{
			if ( wptr->value ) { sptr->namespace = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"option_name") ))
		{
			if ( wptr->value ) { sptr->option_name = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"value") ))
		{
			if ( wptr->value ) { sptr->value = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------------------------------------	*/
/*	r e s t _ o c c i _ p a a s _ c o n f i g u r a t i o n _ o p t i o n 	*/
/*	----------------------------------------------------------------------	*/
public int rest_occi_paas_configuration_option(FILE * fh,struct paas_configuration_option * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.change_severity='%s'\r\n",prefix,nptr,(sptr->change_severity?sptr->change_severity:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.max_length='%s'\r\n",prefix,nptr,(sptr->max_length?sptr->max_length:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.max_value='%s'\r\n",prefix,nptr,(sptr->max_value?sptr->max_value:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.min_value='%s'\r\n",prefix,nptr,(sptr->min_value?sptr->min_value:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.regex='%s'\r\n",prefix,nptr,(sptr->regex?sptr->regex:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.user_defined='%s'\r\n",prefix,nptr,(sptr->user_defined?sptr->user_defined:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.value_options='%s'\r\n",prefix,nptr,(sptr->value_options?sptr->value_options:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.namespace='%s'\r\n",prefix,nptr,(sptr->namespace?sptr->namespace:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.option_name='%s'\r\n",prefix,nptr,(sptr->option_name?sptr->option_name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.value='%s'\r\n",prefix,nptr,(sptr->value?sptr->value:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _paas_configuration_option_cpaas_configuration_option_c_ */
