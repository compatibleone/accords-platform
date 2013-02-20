/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _nagioshostgroup_c_
#define _nagioshostgroup_c_

#include "element.h"

#include "nagioshostgroup.h"

/*	----------------------------------------------------	*/
/*	l i b e r a t e _ n a g i o s _ h o s t _ g r o u p 	*/
/*	----------------------------------------------------	*/
public struct nagios_host_group * liberate_nagios_host_group(struct nagios_host_group * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->alias )
			 sptr->alias = liberate(sptr->alias);
		if ( sptr->notes )
			 sptr->notes = liberate(sptr->notes);
		if ( sptr->notes_url )
			 sptr->notes_url = liberate(sptr->notes_url);
		if ( sptr->action_url )
			 sptr->action_url = liberate(sptr->action_url);
		sptr = liberate( sptr );
	}
	return((struct nagios_host_group *) 0);

}

/*	----------------------------------------------	*/
/*	r e s e t _ n a g i o s _ h o s t _ g r o u p 	*/
/*	----------------------------------------------	*/
public struct nagios_host_group * reset_nagios_host_group(struct nagios_host_group * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->alias = (char*) 0;
		sptr->notes = (char*) 0;
		sptr->notes_url = (char*) 0;
		sptr->action_url = (char*) 0;
		sptr->members =  0;
		sptr->statis =  0;
	}
	return(sptr);

}

/*	----------------------------------------------------	*/
/*	a l l o c a t e _ n a g i o s _ h o s t _ g r o u p 	*/
/*	----------------------------------------------------	*/
public struct nagios_host_group * allocate_nagios_host_group()
{
	struct nagios_host_group * sptr;
	if (!( sptr = allocate( sizeof( struct nagios_host_group ) ) ))
		return( sptr );
	else	return( reset_nagios_host_group(sptr) );
}

/*	----------------------------------------------	*/
/*	x m l i n _ n a g i o s _ h o s t _ g r o u p 	*/
/*	----------------------------------------------	*/
public int xmlin_nagios_host_group(struct nagios_host_group * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"alias") ))
		{
			if ( wptr->value ) { sptr->alias = allocate_string(wptr->value); }
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
		else if (!( strcmp(wptr->name,"members") ))
		{
			if ( wptr->value ) { sptr->members = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"statis") ))
		{
			if ( wptr->value ) { sptr->statis = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------------------	*/
/*	r e s t _ o c c i _ n a g i o s _ h o s t _ g r o u p 	*/
/*	------------------------------------------------------	*/
public int rest_occi_nagios_host_group(FILE * fh,struct nagios_host_group * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.alias='%s'\r\n",prefix,nptr,(sptr->alias?sptr->alias:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.notes='%s'\r\n",prefix,nptr,(sptr->notes?sptr->notes:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.notes_url='%s'\r\n",prefix,nptr,(sptr->notes_url?sptr->notes_url:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.action_url='%s'\r\n",prefix,nptr,(sptr->action_url?sptr->action_url:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.members='%u'\r\n",prefix,nptr,sptr->members);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.statis='%u'\r\n",prefix,nptr,sptr->statis);
	return(0);

}

#endif	/* _nagioshostgroup_cnagioshostgroup_c_ */
