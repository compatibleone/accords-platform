/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _volume_c_
#define _volume_c_

#include "element.h"

#include "volume.h"

/*	------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ v o l u m e 	*/
/*	------------------------------------------	*/
public struct cords_volume * liberate_cords_volume(struct cords_volume * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->size )
			 sptr->size = liberate(sptr->size);
		if ( sptr->type )
			 sptr->type = liberate(sptr->type);
		sptr = liberate( sptr );
	}
	return((struct cords_volume *) 0);

}

/*	------------------------------------	*/
/*	r e s e t _ c o r d s _ v o l u m e 	*/
/*	------------------------------------	*/
public struct cords_volume * reset_cords_volume(struct cords_volume * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->size = (char*) 0;
		sptr->type = (char*) 0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ v o l u m e 	*/
/*	------------------------------------------	*/
public struct cords_volume * allocate_cords_volume()
{
	struct cords_volume * sptr;
	if (!( sptr = allocate( sizeof( struct cords_volume ) ) ))
		return( sptr );
	else	return( reset_cords_volume(sptr) );
}

/*	------------------------------------	*/
/*	x m l i n _ c o r d s _ v o l u m e 	*/
/*	------------------------------------	*/
public int xmlin_cords_volume(struct cords_volume * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"size") ))
		{
			if ( wptr->value ) { sptr->size = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"type") ))
		{
			if ( wptr->value ) { sptr->type = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	--------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ v o l u m e 	*/
/*	--------------------------------------------	*/
public int rest_occi_cords_volume(FILE * fh,struct cords_volume * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.size='%s'\r\n",prefix,nptr,(sptr->size?sptr->size:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.type='%s'\r\n",prefix,nptr,(sptr->type?sptr->type:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _volume_cvolume_c_ */
