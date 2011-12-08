#ifndef _iprange_c_
#define _iprange_c_

#include <stdio.h>
#include "element.h"

#include "iprange.h"

/*	--------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ i p r a n g e 	*/
/*	--------------------------------------------	*/
struct cords_iprange * liberate_cords_iprange(struct cords_iprange * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->version )
			 sptr->version = liberate(sptr->version);
		if ( sptr->type )
			 sptr->type = liberate(sptr->type);
		if ( sptr->value )
			 sptr->value = liberate(sptr->value);
		sptr = liberate( sptr );
	}
	return((struct cords_iprange *) 0);

}

/*	--------------------------------------	*/
/*	r e s e t _ c o r d s _ i p r a n g e 	*/
/*	--------------------------------------	*/
struct cords_iprange * reset_cords_iprange(struct cords_iprange * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->version = (char*) 0;
		sptr->type = (char*) 0;
		sptr->value = (char*) 0;
	}
	return(sptr);

}

/*	--------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ i p r a n g e 	*/
/*	--------------------------------------------	*/
struct cords_iprange * allocate_cords_iprange()
{
	struct cords_iprange * sptr;
	if (!( sptr = allocate( sizeof( struct cords_iprange ) ) ))
		return( sptr );
	else	return( reset_cords_iprange(sptr) );
}

/*	--------------------------------------	*/
/*	x m l i n _ c o r d s _ i p r a n g e 	*/
/*	--------------------------------------	*/
int xmlin_cords_iprange(struct cords_iprange * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"version") ))
		{
			if ( wptr->value ) { sptr->version = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"type") ))
		{
			if ( wptr->value ) { sptr->type = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"value") ))
		{
			if ( wptr->value ) { sptr->value = allocate_string(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ i p r a n g e 	*/
/*	----------------------------------------------	*/
int rest_occi_cords_iprange(FILE * fh,struct cords_iprange * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.version='%s'\r\n",prefix,nptr,(sptr->version?sptr->version:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.type='%s'\r\n",prefix,nptr,(sptr->type?sptr->type:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.value='%s'\r\n",prefix,nptr,(sptr->value?sptr->value:""));
	return(0);

}

#endif	/* _iprange_c_ */
