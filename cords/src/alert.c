/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _alert_c_
#define _alert_c_

#include "element.h"

#include "alert.h"

/*	----------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ a l e r t 	*/
/*	----------------------------------------	*/
public struct cords_alert * liberate_cords_alert(struct cords_alert * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->source )
			 sptr->source = liberate(sptr->source);
		if ( sptr->subject )
			 sptr->subject = liberate(sptr->subject);
		if ( sptr->created )
			 sptr->created = liberate(sptr->created);
		if ( sptr->resolved )
			 sptr->resolved = liberate(sptr->resolved);
		if ( sptr->nature )
			 sptr->nature = liberate(sptr->nature);
		if ( sptr->status )
			 sptr->status = liberate(sptr->status);
		sptr = liberate( sptr );
	}
	return((struct cords_alert *) 0);

}

/*	----------------------------------	*/
/*	r e s e t _ c o r d s _ a l e r t 	*/
/*	----------------------------------	*/
public struct cords_alert * reset_cords_alert(struct cords_alert * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->source = (char*) 0;
		sptr->subject = (char*) 0;
		sptr->created = (char*) 0;
		sptr->resolved = (char*) 0;
		sptr->nature = (char*) 0;
		sptr->status = (char*) 0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	----------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ a l e r t 	*/
/*	----------------------------------------	*/
public struct cords_alert * allocate_cords_alert()
{
	struct cords_alert * sptr;
	if (!( sptr = allocate( sizeof( struct cords_alert ) ) ))
		return( sptr );
	else	return( reset_cords_alert(sptr) );
}

/*	----------------------------------	*/
/*	x m l i n _ c o r d s _ a l e r t 	*/
/*	----------------------------------	*/
public int xmlin_cords_alert(struct cords_alert * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"source") ))
		{
			if ( wptr->value ) { sptr->source = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"subject") ))
		{
			if ( wptr->value ) { sptr->subject = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"created") ))
		{
			if ( wptr->value ) { sptr->created = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"resolved") ))
		{
			if ( wptr->value ) { sptr->resolved = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"nature") ))
		{
			if ( wptr->value ) { sptr->nature = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"status") ))
		{
			if ( wptr->value ) { sptr->status = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ a l e r t 	*/
/*	------------------------------------------	*/
public int rest_occi_cords_alert(FILE * fh,struct cords_alert * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.source='%s'\r\n",prefix,nptr,(sptr->source?sptr->source:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.subject='%s'\r\n",prefix,nptr,(sptr->subject?sptr->subject:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.created='%s'\r\n",prefix,nptr,(sptr->created?sptr->created:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.resolved='%s'\r\n",prefix,nptr,(sptr->resolved?sptr->resolved:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.nature='%s'\r\n",prefix,nptr,(sptr->nature?sptr->nature:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.status='%s'\r\n",prefix,nptr,(sptr->status?sptr->status:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _alert_calert_c_ */
