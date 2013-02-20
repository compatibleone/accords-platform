/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _compute_c_
#define _compute_c_

#include "element.h"

#include "compute.h"

/*	--------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ c o m p u t e 	*/
/*	--------------------------------------------	*/
public struct cords_compute * liberate_cords_compute(struct cords_compute * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->architecture )
			 sptr->architecture = liberate(sptr->architecture);
		if ( sptr->cores )
			 sptr->cores = liberate(sptr->cores);
		if ( sptr->speed )
			 sptr->speed = liberate(sptr->speed);
		if ( sptr->memory )
			 sptr->memory = liberate(sptr->memory);
		if ( sptr->hostname )
			 sptr->hostname = liberate(sptr->hostname);
		sptr = liberate( sptr );
	}
	return((struct cords_compute *) 0);

}

/*	--------------------------------------	*/
/*	r e s e t _ c o r d s _ c o m p u t e 	*/
/*	--------------------------------------	*/
public struct cords_compute * reset_cords_compute(struct cords_compute * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->architecture = (char*) 0;
		sptr->cores = (char*) 0;
		sptr->speed = (char*) 0;
		sptr->memory = (char*) 0;
		sptr->hostname = (char*) 0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	--------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ c o m p u t e 	*/
/*	--------------------------------------------	*/
public struct cords_compute * allocate_cords_compute()
{
	struct cords_compute * sptr;
	if (!( sptr = allocate( sizeof( struct cords_compute ) ) ))
		return( sptr );
	else	return( reset_cords_compute(sptr) );
}

/*	--------------------------------------	*/
/*	x m l i n _ c o r d s _ c o m p u t e 	*/
/*	--------------------------------------	*/
public int xmlin_cords_compute(struct cords_compute * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"architecture") ))
		{
			if ( wptr->value ) { sptr->architecture = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"cores") ))
		{
			if ( wptr->value ) { sptr->cores = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"speed") ))
		{
			if ( wptr->value ) { sptr->speed = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"memory") ))
		{
			if ( wptr->value ) { sptr->memory = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"hostname") ))
		{
			if ( wptr->value ) { sptr->hostname = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ c o m p u t e 	*/
/*	----------------------------------------------	*/
public int rest_occi_cords_compute(FILE * fh,struct cords_compute * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.architecture='%s'\r\n",prefix,nptr,(sptr->architecture?sptr->architecture:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.cores='%s'\r\n",prefix,nptr,(sptr->cores?sptr->cores:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.speed='%s'\r\n",prefix,nptr,(sptr->speed?sptr->speed:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.memory='%s'\r\n",prefix,nptr,(sptr->memory?sptr->memory:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.hostname='%s'\r\n",prefix,nptr,(sptr->hostname?sptr->hostname:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _compute_ccompute_c_ */
