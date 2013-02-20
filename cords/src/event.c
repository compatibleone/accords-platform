/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _event_c_
#define _event_c_

#include "element.h"

#include "event.h"

/*	----------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ e v e n t 	*/
/*	----------------------------------------	*/
public struct cords_event * liberate_cords_event(struct cords_event * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->source )
			 sptr->source = liberate(sptr->source);
		if ( sptr->target )
			 sptr->target = liberate(sptr->target);
		if ( sptr->nature )
			 sptr->nature = liberate(sptr->nature);
		if ( sptr->description )
			 sptr->description = liberate(sptr->description);
		if ( sptr->compliment )
			 sptr->compliment = liberate(sptr->compliment);
		sptr = liberate( sptr );
	}
	return((struct cords_event *) 0);

}

/*	----------------------------------	*/
/*	r e s e t _ c o r d s _ e v e n t 	*/
/*	----------------------------------	*/
public struct cords_event * reset_cords_event(struct cords_event * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->source = (char*) 0;
		sptr->target = (char*) 0;
		sptr->nature = (char*) 0;
		sptr->description = (char*) 0;
		sptr->compliment = (char*) 0;
		sptr->timestamp =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	----------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ e v e n t 	*/
/*	----------------------------------------	*/
public struct cords_event * allocate_cords_event()
{
	struct cords_event * sptr;
	if (!( sptr = allocate( sizeof( struct cords_event ) ) ))
		return( sptr );
	else	return( reset_cords_event(sptr) );
}

/*	----------------------------------	*/
/*	x m l i n _ c o r d s _ e v e n t 	*/
/*	----------------------------------	*/
public int xmlin_cords_event(struct cords_event * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"source") ))
		{
			if ( wptr->value ) { sptr->source = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"target") ))
		{
			if ( wptr->value ) { sptr->target = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"nature") ))
		{
			if ( wptr->value ) { sptr->nature = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"description") ))
		{
			if ( wptr->value ) { sptr->description = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"compliment") ))
		{
			if ( wptr->value ) { sptr->compliment = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"timestamp") ))
		{
			if ( wptr->value ) { sptr->timestamp = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ e v e n t 	*/
/*	------------------------------------------	*/
public int rest_occi_cords_event(FILE * fh,struct cords_event * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.source='%s'\r\n",prefix,nptr,(sptr->source?sptr->source:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.target='%s'\r\n",prefix,nptr,(sptr->target?sptr->target:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.nature='%s'\r\n",prefix,nptr,(sptr->nature?sptr->nature:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.description='%s'\r\n",prefix,nptr,(sptr->description?sptr->description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.compliment='%s'\r\n",prefix,nptr,(sptr->compliment?sptr->compliment:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.timestamp='%u'\r\n",prefix,nptr,sptr->timestamp);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _event_cevent_c_ */
