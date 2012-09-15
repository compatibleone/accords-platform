/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _paas_link_c_
#define _paas_link_c_

#include "element.h"

#include "paas_link.h"

/*	------------------------------------	*/
/*	l i b e r a t e _ p a a s _ l i n k 	*/
/*	------------------------------------	*/
public struct paas_link * liberate_paas_link(struct paas_link * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->type )
			 sptr->type = liberate(sptr->type);
		if ( sptr->expression )
			 sptr->expression = liberate(sptr->expression);
		sptr = liberate( sptr );
	}
	return((struct paas_link *) 0);

}

/*	------------------------------	*/
/*	r e s e t _ p a a s _ l i n k 	*/
/*	------------------------------	*/
public struct paas_link * reset_paas_link(struct paas_link * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->type = (char*) 0;
		sptr->expression = (char*) 0;
		sptr->status =  0;
	}
	return(sptr);

}

/*	------------------------------------	*/
/*	a l l o c a t e _ p a a s _ l i n k 	*/
/*	------------------------------------	*/
public struct paas_link * allocate_paas_link()
{
	struct paas_link * sptr;
	if (!( sptr = allocate( sizeof( struct paas_link ) ) ))
		return( sptr );
	else	return( reset_paas_link(sptr) );
}

/*	------------------------------	*/
/*	x m l i n _ p a a s _ l i n k 	*/
/*	------------------------------	*/
public int xmlin_paas_link(struct paas_link * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"type") ))
		{
			if ( wptr->value ) { sptr->type = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"expression") ))
		{
			if ( wptr->value ) { sptr->expression = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"status") ))
		{
			if ( wptr->value ) { sptr->status = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	--------------------------------------	*/
/*	r e s t _ o c c i _ p a a s _ l i n k 	*/
/*	--------------------------------------	*/
public int rest_occi_paas_link(FILE * fh,struct paas_link * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.type='%s'\r\n",prefix,nptr,(sptr->type?sptr->type:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.expression='%s'\r\n",prefix,nptr,(sptr->expression?sptr->expression:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.status='%u'\r\n",prefix,nptr,sptr->status);
	return(0);

}

#endif	/* _paas_link_cpaas_link_c_ */
