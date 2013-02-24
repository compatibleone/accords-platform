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
#ifndef _nagioshostextinfo_c_
#define _nagioshostextinfo_c_

#include "element.h"

#include "nagioshostextinfo.h"

/*	----------------------------------------------------------	*/
/*	l i b e r a t e _ n a g i o s _ h o s t _ e x t _ i n f o 	*/
/*	----------------------------------------------------------	*/
public struct nagios_host_ext_info * liberate_nagios_host_ext_info(struct nagios_host_ext_info * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->host_name )
			 sptr->host_name = liberate(sptr->host_name);
		if ( sptr->notes )
			 sptr->notes = liberate(sptr->notes);
		if ( sptr->notes_url )
			 sptr->notes_url = liberate(sptr->notes_url);
		if ( sptr->action_url )
			 sptr->action_url = liberate(sptr->action_url);
		if ( sptr->icon_image )
			 sptr->icon_image = liberate(sptr->icon_image);
		if ( sptr->icon_image_alt )
			 sptr->icon_image_alt = liberate(sptr->icon_image_alt);
		if ( sptr->vrml_image )
			 sptr->vrml_image = liberate(sptr->vrml_image);
		if ( sptr->statusmap_image )
			 sptr->statusmap_image = liberate(sptr->statusmap_image);
		if ( sptr->twod_coords )
			 sptr->twod_coords = liberate(sptr->twod_coords);
		if ( sptr->threed_coords )
			 sptr->threed_coords = liberate(sptr->threed_coords);
		sptr = liberate( sptr );
	}
	return((struct nagios_host_ext_info *) 0);

}

/*	----------------------------------------------------	*/
/*	r e s e t _ n a g i o s _ h o s t _ e x t _ i n f o 	*/
/*	----------------------------------------------------	*/
public struct nagios_host_ext_info * reset_nagios_host_ext_info(struct nagios_host_ext_info * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->host_name = (char*) 0;
		sptr->notes = (char*) 0;
		sptr->notes_url = (char*) 0;
		sptr->action_url = (char*) 0;
		sptr->icon_image = (char*) 0;
		sptr->icon_image_alt = (char*) 0;
		sptr->vrml_image = (char*) 0;
		sptr->statusmap_image = (char*) 0;
		sptr->twod_coords = (char*) 0;
		sptr->threed_coords = (char*) 0;
		sptr->status =  0;
	}
	return(sptr);

}

/*	----------------------------------------------------------	*/
/*	a l l o c a t e _ n a g i o s _ h o s t _ e x t _ i n f o 	*/
/*	----------------------------------------------------------	*/
public struct nagios_host_ext_info * allocate_nagios_host_ext_info()
{
	struct nagios_host_ext_info * sptr;
	if (!( sptr = allocate( sizeof( struct nagios_host_ext_info ) ) ))
		return( sptr );
	else	return( reset_nagios_host_ext_info(sptr) );
}

/*	----------------------------------------------------	*/
/*	x m l i n _ n a g i o s _ h o s t _ e x t _ i n f o 	*/
/*	----------------------------------------------------	*/
public int xmlin_nagios_host_ext_info(struct nagios_host_ext_info * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"host_name") ))
		{
			if ( wptr->value ) { sptr->host_name = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"notes") ))
		{
			if ( wptr->value ) { sptr->notes = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"notes_url") ))
		{
			if ( wptr->value ) { sptr->notes_url = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"action_url") ))
		{
			if ( wptr->value ) { sptr->action_url = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"icon_image") ))
		{
			if ( wptr->value ) { sptr->icon_image = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"icon_image_alt") ))
		{
			if ( wptr->value ) { sptr->icon_image_alt = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"vrml_image") ))
		{
			if ( wptr->value ) { sptr->vrml_image = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"statusmap_image") ))
		{
			if ( wptr->value ) { sptr->statusmap_image = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"twod_coords") ))
		{
			if ( wptr->value ) { sptr->twod_coords = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"threed_coords") ))
		{
			if ( wptr->value ) { sptr->threed_coords = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"status") ))
		{
			if ( wptr->value ) { sptr->status = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------------------------	*/
/*	r e s t _ o c c i _ n a g i o s _ h o s t _ e x t _ i n f o 	*/
/*	------------------------------------------------------------	*/
public int rest_occi_nagios_host_ext_info(FILE * fh,struct nagios_host_ext_info * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.host_name='%s'\r\n",prefix,nptr,(sptr->host_name?sptr->host_name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.notes='%s'\r\n",prefix,nptr,(sptr->notes?sptr->notes:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.notes_url='%s'\r\n",prefix,nptr,(sptr->notes_url?sptr->notes_url:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.action_url='%s'\r\n",prefix,nptr,(sptr->action_url?sptr->action_url:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.icon_image='%s'\r\n",prefix,nptr,(sptr->icon_image?sptr->icon_image:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.icon_image_alt='%s'\r\n",prefix,nptr,(sptr->icon_image_alt?sptr->icon_image_alt:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.vrml_image='%s'\r\n",prefix,nptr,(sptr->vrml_image?sptr->vrml_image:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.statusmap_image='%s'\r\n",prefix,nptr,(sptr->statusmap_image?sptr->statusmap_image:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.twod_coords='%s'\r\n",prefix,nptr,(sptr->twod_coords?sptr->twod_coords:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.threed_coords='%s'\r\n",prefix,nptr,(sptr->threed_coords?sptr->threed_coords:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.status='%u'\r\n",prefix,nptr,sptr->status);
	return(0);

}

#endif	/* _nagioshostextinfo_cnagioshostextinfo_c_ */
