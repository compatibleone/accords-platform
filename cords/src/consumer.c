/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _consumer_c_
#define _consumer_c_

#include "element.h"

#include "consumer.h"

/*	----------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ c o n s u m e r 	*/
/*	----------------------------------------------	*/
public struct cords_consumer * liberate_cords_consumer(struct cords_consumer * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->identity )
			 sptr->identity = liberate(sptr->identity);
		if ( sptr->operator )
			 sptr->operator = liberate(sptr->operator);
		if ( sptr->price )
			 sptr->price = liberate(sptr->price);
		if ( sptr->nature )
			 sptr->nature = liberate(sptr->nature);
		sptr = liberate( sptr );
	}
	return((struct cords_consumer *) 0);

}

/*	----------------------------------------	*/
/*	r e s e t _ c o r d s _ c o n s u m e r 	*/
/*	----------------------------------------	*/
public struct cords_consumer * reset_cords_consumer(struct cords_consumer * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->identity = (char*) 0;
		sptr->operator = (char*) 0;
		sptr->price = (char*) 0;
		sptr->nature = (char*) 0;
		sptr->connections =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	----------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ c o n s u m e r 	*/
/*	----------------------------------------------	*/
public struct cords_consumer * allocate_cords_consumer()
{
	struct cords_consumer * sptr;
	if (!( sptr = allocate( sizeof( struct cords_consumer ) ) ))
		return( sptr );
	else	return( reset_cords_consumer(sptr) );
}

/*	----------------------------------------	*/
/*	x m l i n _ c o r d s _ c o n s u m e r 	*/
/*	----------------------------------------	*/
public int xmlin_cords_consumer(struct cords_consumer * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"identity") ))
		{
			if ( wptr->value ) { sptr->identity = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"operator") ))
		{
			if ( wptr->value ) { sptr->operator = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"price") ))
		{
			if ( wptr->value ) { sptr->price = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"nature") ))
		{
			if ( wptr->value ) { sptr->nature = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"connections") ))
		{
			if ( wptr->value ) { sptr->connections = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ c o n s u m e r 	*/
/*	------------------------------------------------	*/
public int rest_occi_cords_consumer(FILE * fh,struct cords_consumer * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.identity='%s'\r\n",prefix,nptr,(sptr->identity?sptr->identity:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.operator='%s'\r\n",prefix,nptr,(sptr->operator?sptr->operator:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.price='%s'\r\n",prefix,nptr,(sptr->price?sptr->price:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.nature='%s'\r\n",prefix,nptr,(sptr->nature?sptr->nature:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.connections='%u'\r\n",prefix,nptr,sptr->connections);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _consumer_cconsumer_c_ */
