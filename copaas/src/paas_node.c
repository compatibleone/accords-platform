/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _paas_node_c_
#define _paas_node_c_

#include "element.h"

#include "paas_node.h"

/*	------------------------------------	*/
/*	l i b e r a t e _ p a a s _ n o d e 	*/
/*	------------------------------------	*/
public struct paas_node * liberate_paas_node(struct paas_node * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->content_type )
			 sptr->content_type = liberate(sptr->content_type);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->version )
			 sptr->version = liberate(sptr->version);
		if ( sptr->provider )
			 sptr->provider = liberate(sptr->provider);
		sptr = liberate( sptr );
	}
	return((struct paas_node *) 0);

}

/*	------------------------------	*/
/*	r e s e t _ p a a s _ n o d e 	*/
/*	------------------------------	*/
public struct paas_node * reset_paas_node(struct paas_node * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->content_type = (char*) 0;
		sptr->name = (char*) 0;
		sptr->version = (char*) 0;
		sptr->provider = (char*) 0;
		sptr->status =  0;
	}
	return(sptr);

}

/*	------------------------------------	*/
/*	a l l o c a t e _ p a a s _ n o d e 	*/
/*	------------------------------------	*/
public struct paas_node * allocate_paas_node()
{
	struct paas_node * sptr;
	if (!( sptr = allocate( sizeof( struct paas_node ) ) ))
		return( sptr );
	else	return( reset_paas_node(sptr) );
}

/*	------------------------------	*/
/*	x m l i n _ p a a s _ n o d e 	*/
/*	------------------------------	*/
public int xmlin_paas_node(struct paas_node * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"content_type") ))
		{
			if ( wptr->value ) { sptr->content_type = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"name") ))
		{
			if ( wptr->value ) { sptr->name = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"version") ))
		{
			if ( wptr->value ) { sptr->version = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"provider") ))
		{
			if ( wptr->value ) { sptr->provider = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"status") ))
		{
			if ( wptr->value ) { sptr->status = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	--------------------------------------	*/
/*	r e s t _ o c c i _ p a a s _ n o d e 	*/
/*	--------------------------------------	*/
public int rest_occi_paas_node(FILE * fh,struct paas_node * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.content_type='%s'\r\n",prefix,nptr,(sptr->content_type?sptr->content_type:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.version='%s'\r\n",prefix,nptr,(sptr->version?sptr->version:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.provider='%s'\r\n",prefix,nptr,(sptr->provider?sptr->provider:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.status='%u'\r\n",prefix,nptr,sptr->status);
	return(0);

}

#endif	/* _paas_node_cpaas_node_c_ */
