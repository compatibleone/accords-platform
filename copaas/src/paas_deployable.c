/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _paas_deployable_c_
#define _paas_deployable_c_

#include "element.h"

#include "paas_deployable.h"

/*	------------------------------------------------	*/
/*	l i b e r a t e _ p a a s _ d e p l o y a b l e 	*/
/*	------------------------------------------------	*/
public struct paas_deployable * liberate_paas_deployable(struct paas_deployable * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->content_type )
			 sptr->content_type = liberate(sptr->content_type);
		if ( sptr->location )
			 sptr->location = liberate(sptr->location);
		if ( sptr->multitenancy_level )
			 sptr->multitenancy_level = liberate(sptr->multitenancy_level);
		sptr = liberate( sptr );
	}
	return((struct paas_deployable *) 0);

}

/*	------------------------------------------	*/
/*	r e s e t _ p a a s _ d e p l o y a b l e 	*/
/*	------------------------------------------	*/
public struct paas_deployable * reset_paas_deployable(struct paas_deployable * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->content_type = (char*) 0;
		sptr->location = (char*) 0;
		sptr->multitenancy_level = (char*) 0;
		sptr->status =  0;
	}
	return(sptr);

}

/*	------------------------------------------------	*/
/*	a l l o c a t e _ p a a s _ d e p l o y a b l e 	*/
/*	------------------------------------------------	*/
public struct paas_deployable * allocate_paas_deployable()
{
	struct paas_deployable * sptr;
	if (!( sptr = allocate( sizeof( struct paas_deployable ) ) ))
		return( sptr );
	else	return( reset_paas_deployable(sptr) );
}

/*	------------------------------------------	*/
/*	x m l i n _ p a a s _ d e p l o y a b l e 	*/
/*	------------------------------------------	*/
public int xmlin_paas_deployable(struct paas_deployable * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"content_type") ))
		{
			if ( wptr->value ) { sptr->content_type = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"location") ))
		{
			if ( wptr->value ) { sptr->location = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"multitenancy_level") ))
		{
			if ( wptr->value ) { sptr->multitenancy_level = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"status") ))
		{
			if ( wptr->value ) { sptr->status = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	--------------------------------------------------	*/
/*	r e s t _ o c c i _ p a a s _ d e p l o y a b l e 	*/
/*	--------------------------------------------------	*/
public int rest_occi_paas_deployable(FILE * fh,struct paas_deployable * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.content_type='%s'\r\n",prefix,nptr,(sptr->content_type?sptr->content_type:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.location='%s'\r\n",prefix,nptr,(sptr->location?sptr->location:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.multitenancy_level='%s'\r\n",prefix,nptr,(sptr->multitenancy_level?sptr->multitenancy_level:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.status='%u'\r\n",prefix,nptr,sptr->status);
	return(0);

}

#endif	/* _paas_deployable_cpaas_deployable_c_ */
