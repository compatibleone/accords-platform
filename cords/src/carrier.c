/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _carrier_c_
#define _carrier_c_

#include "element.h"

#include "carrier.h"

/*	--------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ c a r r i e r 	*/
/*	--------------------------------------------	*/
public struct cords_carrier * liberate_cords_carrier(struct cords_carrier * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->category )
			 sptr->category = liberate(sptr->category);
		if ( sptr->profile )
			 sptr->profile = liberate(sptr->profile);
		if ( sptr->tarification )
			 sptr->tarification = liberate(sptr->tarification);
		sptr = liberate( sptr );
	}
	return((struct cords_carrier *) 0);

}

/*	--------------------------------------	*/
/*	r e s e t _ c o r d s _ c a r r i e r 	*/
/*	--------------------------------------	*/
public struct cords_carrier * reset_cords_carrier(struct cords_carrier * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->category = (char*) 0;
		sptr->profile = (char*) 0;
		sptr->tarification = (char*) 0;
	}
	return(sptr);

}

/*	--------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ c a r r i e r 	*/
/*	--------------------------------------------	*/
public struct cords_carrier * allocate_cords_carrier()
{
	struct cords_carrier * sptr;
	if (!( sptr = allocate( sizeof( struct cords_carrier ) ) ))
		return( sptr );
	else	return( reset_cords_carrier(sptr) );
}

/*	--------------------------------------	*/
/*	x m l i n _ c o r d s _ c a r r i e r 	*/
/*	--------------------------------------	*/
public int xmlin_cords_carrier(struct cords_carrier * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"category") ))
		{
			if ( wptr->value ) { sptr->category = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"profile") ))
		{
			if ( wptr->value ) { sptr->profile = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"tarification") ))
		{
			if ( wptr->value ) { sptr->tarification = allocate_string(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ c a r r i e r 	*/
/*	----------------------------------------------	*/
public int rest_occi_cords_carrier(FILE * fh,struct cords_carrier * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.category='%s'\r\n",prefix,nptr,(sptr->category?sptr->category:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.profile='%s'\r\n",prefix,nptr,(sptr->profile?sptr->profile:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.tarification='%s'\r\n",prefix,nptr,(sptr->tarification?sptr->tarification:""));
	return(0);

}

#endif	/* _carrier_ccarrier_c_ */
