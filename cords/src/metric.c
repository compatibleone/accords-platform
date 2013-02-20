/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _metric_c_
#define _metric_c_

#include "element.h"

#include "metric.h"

/*	------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ m e t r i c 	*/
/*	------------------------------------------	*/
public struct cords_metric * liberate_cords_metric(struct cords_metric * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->units )
			 sptr->units = liberate(sptr->units);
		if ( sptr->period )
			 sptr->period = liberate(sptr->period);
		if ( sptr->samples )
			 sptr->samples = liberate(sptr->samples);
		if ( sptr->expression )
			 sptr->expression = liberate(sptr->expression);
		sptr = liberate( sptr );
	}
	return((struct cords_metric *) 0);

}

/*	------------------------------------	*/
/*	r e s e t _ c o r d s _ m e t r i c 	*/
/*	------------------------------------	*/
public struct cords_metric * reset_cords_metric(struct cords_metric * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->units = (char*) 0;
		sptr->period = (char*) 0;
		sptr->samples = (char*) 0;
		sptr->expression = (char*) 0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ m e t r i c 	*/
/*	------------------------------------------	*/
public struct cords_metric * allocate_cords_metric()
{
	struct cords_metric * sptr;
	if (!( sptr = allocate( sizeof( struct cords_metric ) ) ))
		return( sptr );
	else	return( reset_cords_metric(sptr) );
}

/*	------------------------------------	*/
/*	x m l i n _ c o r d s _ m e t r i c 	*/
/*	------------------------------------	*/
public int xmlin_cords_metric(struct cords_metric * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"units") ))
		{
			if ( wptr->value ) { sptr->units = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"period") ))
		{
			if ( wptr->value ) { sptr->period = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"samples") ))
		{
			if ( wptr->value ) { sptr->samples = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"expression") ))
		{
			if ( wptr->value ) { sptr->expression = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	--------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ m e t r i c 	*/
/*	--------------------------------------------	*/
public int rest_occi_cords_metric(FILE * fh,struct cords_metric * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.units='%s'\r\n",prefix,nptr,(sptr->units?sptr->units:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.period='%s'\r\n",prefix,nptr,(sptr->period?sptr->period:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.samples='%s'\r\n",prefix,nptr,(sptr->samples?sptr->samples:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.expression='%s'\r\n",prefix,nptr,(sptr->expression?sptr->expression:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _metric_cmetric_c_ */
