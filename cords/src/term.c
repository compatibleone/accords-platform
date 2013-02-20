/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _term_c_
#define _term_c_

#include "element.h"

#include "term.h"

/*	--------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ t e r m 	*/
/*	--------------------------------------	*/
public struct cords_term * liberate_cords_term(struct cords_term * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->description )
			 sptr->description = liberate(sptr->description);
		if ( sptr->type )
			 sptr->type = liberate(sptr->type);
		if ( sptr->manifest )
			 sptr->manifest = liberate(sptr->manifest);
		if ( sptr->provider )
			 sptr->provider = liberate(sptr->provider);
		sptr = liberate( sptr );
	}
	return((struct cords_term *) 0);

}

/*	--------------------------------	*/
/*	r e s e t _ c o r d s _ t e r m 	*/
/*	--------------------------------	*/
public struct cords_term * reset_cords_term(struct cords_term * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->description = (char*) 0;
		sptr->type = (char*) 0;
		sptr->manifest = (char*) 0;
		sptr->provider = (char*) 0;
		sptr->variables =  0;
		sptr->guarantees =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	--------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ t e r m 	*/
/*	--------------------------------------	*/
public struct cords_term * allocate_cords_term()
{
	struct cords_term * sptr;
	if (!( sptr = allocate( sizeof( struct cords_term ) ) ))
		return( sptr );
	else	return( reset_cords_term(sptr) );
}

/*	--------------------------------	*/
/*	x m l i n _ c o r d s _ t e r m 	*/
/*	--------------------------------	*/
public int xmlin_cords_term(struct cords_term * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"description") ))
		{
			if ( wptr->value ) { sptr->description = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"type") ))
		{
			if ( wptr->value ) { sptr->type = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"manifest") ))
		{
			if ( wptr->value ) { sptr->manifest = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"provider") ))
		{
			if ( wptr->value ) { sptr->provider = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"variables") ))
		{
			if ( wptr->value ) { sptr->variables = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"guarantees") ))
		{
			if ( wptr->value ) { sptr->guarantees = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ t e r m 	*/
/*	----------------------------------------	*/
public int rest_occi_cords_term(FILE * fh,struct cords_term * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.description='%s'\r\n",prefix,nptr,(sptr->description?sptr->description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.type='%s'\r\n",prefix,nptr,(sptr->type?sptr->type:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.manifest='%s'\r\n",prefix,nptr,(sptr->manifest?sptr->manifest:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.provider='%s'\r\n",prefix,nptr,(sptr->provider?sptr->provider:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.variables='%u'\r\n",prefix,nptr,sptr->variables);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.guarantees='%u'\r\n",prefix,nptr,sptr->guarantees);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _term_cterm_c_ */
