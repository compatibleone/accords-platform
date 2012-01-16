/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _authitem_c_
#define _authitem_c_

#include "element.h"

#include "authitem.h"

/*	----------------------------------------------------------------	*/
/*	l i b e r a t e _ o c c i _ a u t h o r i z a t i o n _ i t e m 	*/
/*	----------------------------------------------------------------	*/
public struct occi_authorization_item * liberate_occi_authorization_item(struct occi_authorization_item * sptr)
{
	if ( sptr )
	{
		if ( sptr->token )
			 sptr->token = liberate(sptr->token);
		sptr = liberate( sptr );
	}
	return((struct occi_authorization_item *) 0);

}

/*	----------------------------------------------------------	*/
/*	r e s e t _ o c c i _ a u t h o r i z a t i o n _ i t e m 	*/
/*	----------------------------------------------------------	*/
public struct occi_authorization_item * reset_occi_authorization_item(struct occi_authorization_item * sptr)
{
	if ( sptr )
	{
		sptr->parent = (struct occi_authorization_cache*) 0;
		sptr->previous = (struct occi_authorization_item*) 0;
		sptr->next = (struct occi_authorization_item*) 0;
		sptr->token = (char*) 0;
		sptr->ttl =  0;
	}
	return(sptr);

}

/*	----------------------------------------------------------------	*/
/*	a l l o c a t e _ o c c i _ a u t h o r i z a t i o n _ i t e m 	*/
/*	----------------------------------------------------------------	*/
public struct occi_authorization_item * allocate_occi_authorization_item()
{
	struct occi_authorization_item * sptr;
	if (!( sptr = allocate( sizeof( struct occi_authorization_item ) ) ))
		return( sptr );
	else	return( reset_occi_authorization_item(sptr) );
}

/*	------------------------------------------------------	*/
/*	a d d _ o c c i _ a u t h o r i z a t i o n _ i t e m 	*/
/*	------------------------------------------------------	*/
public struct occi_authorization_item * add_occi_authorization_item(struct occi_authorization_cache * pptr)
{
	struct occi_authorization_item * sptr;
	if (!( sptr = allocate( sizeof( struct occi_authorization_item ) ) ))
		return( sptr );
	else if (!( sptr = reset_occi_authorization_item(sptr) ))
		return( sptr );
	else
	{
		if (!( sptr->previous = pptr->last ))
			pptr->first = sptr;
		else	sptr->previous->next = sptr;
		pptr->last = sptr;
		sptr->parent = pptr;
		return( sptr );
	}

}

/*	--------------------------------------------------------	*/
/*	d r o p _ o c c i _ a u t h o r i z a t i o n _ i t e m 	*/
/*	--------------------------------------------------------	*/
public struct occi_authorization_item * drop_occi_authorization_item(struct occi_authorization_item * sptr)
{
	if ( sptr )
	{
		if (!( sptr->parent )) return(sptr);
		if (!( sptr->previous ))
		{
			if (!( sptr->parent->first = sptr->next ))
				sptr->parent->last = (struct occi_authorization_item *) 0;
			else	sptr->parent->first->previous = (struct occi_authorization_item *) 0;
		}
		else if (!( sptr->previous->next = sptr->next ))
			sptr->parent->last = sptr->previous;
		if (!( sptr->next ))
		{
			if (!( sptr->parent->last = sptr->previous ))
				sptr->parent->first = (struct occi_authorization_item *) 0;
			else	sptr->parent->last->next = (struct occi_authorization_item *) 0;
		}
		else if (!( sptr->next->previous = sptr->previous ))
			sptr->parent->first = sptr->next;
		sptr = liberate_occi_authorization_item(sptr);
	}
	return((struct occi_authorization_item *) 0);
}

/*	----------------------------------------------------------	*/
/*	x m l i n _ o c c i _ a u t h o r i z a t i o n _ i t e m 	*/
/*	----------------------------------------------------------	*/
public int xmlin_occi_authorization_item(struct occi_authorization_item * sptr,struct xml_element * eptr)
{
	struct xml_element * wptr;
	if (!( eptr )) return(0);
	if (!( sptr )) return(0);
	for ( wptr=eptr->first; wptr != (struct xml_element *) 0; wptr=wptr->next)
	{
		if (!( strcmp(wptr->name,"token") ))
		{
			if ( wptr->value ) { sptr->token = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"ttl") ))
		{
			if ( wptr->value ) { sptr->ttl = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------------------------------	*/
/*	r e s t _ o c c i _ o c c i _ a u t h o r i z a t i o n _ i t e m 	*/
/*	------------------------------------------------------------------	*/
public int rest_occi_occi_authorization_item(FILE * fh,struct occi_authorization_item * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.token='%s'\r\n",prefix,nptr,(sptr->token?sptr->token:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.ttl='%u'\r\n",prefix,nptr,sptr->ttl);
	return(0);

}

#endif	/* _authitem_c_ */
