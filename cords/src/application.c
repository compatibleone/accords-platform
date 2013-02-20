/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _application_c_
#define _application_c_

#include "element.h"

#include "application.h"

/*	----------------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ a p p l i c a t i o n 	*/
/*	----------------------------------------------------	*/
public struct cords_application * liberate_cords_application(struct cords_application * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->image )
			 sptr->image = liberate(sptr->image);
		if ( sptr->provider )
			 sptr->provider = liberate(sptr->provider);
		if ( sptr->price )
			 sptr->price = liberate(sptr->price);
		if ( sptr->url )
			 sptr->url = liberate(sptr->url);
		if ( sptr->account )
			 sptr->account = liberate(sptr->account);
		if ( sptr->provision )
			 sptr->provision = liberate(sptr->provision);
		sptr = liberate( sptr );
	}
	return((struct cords_application *) 0);

}

/*	----------------------------------------------	*/
/*	r e s e t _ c o r d s _ a p p l i c a t i o n 	*/
/*	----------------------------------------------	*/
public struct cords_application * reset_cords_application(struct cords_application * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->state =  0;
		sptr->image = (char*) 0;
		sptr->provider = (char*) 0;
		sptr->price = (char*) 0;
		sptr->url = (char*) 0;
		sptr->account = (char*) 0;
		sptr->provision = (char*) 0;
		sptr->created =  0;
		sptr->started =  0;
		sptr->completed =  0;
		sptr->duration =  0;
	}
	return(sptr);

}

/*	----------------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ a p p l i c a t i o n 	*/
/*	----------------------------------------------------	*/
public struct cords_application * allocate_cords_application()
{
	struct cords_application * sptr;
	if (!( sptr = allocate( sizeof( struct cords_application ) ) ))
		return( sptr );
	else	return( reset_cords_application(sptr) );
}

/*	----------------------------------------------	*/
/*	x m l i n _ c o r d s _ a p p l i c a t i o n 	*/
/*	----------------------------------------------	*/
public int xmlin_cords_application(struct cords_application * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"image") ))
		{
			if ( wptr->value ) { sptr->image = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"provider") ))
		{
			if ( wptr->value ) { sptr->provider = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"price") ))
		{
			if ( wptr->value ) { sptr->price = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"url") ))
		{
			if ( wptr->value ) { sptr->url = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"account") ))
		{
			if ( wptr->value ) { sptr->account = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"provision") ))
		{
			if ( wptr->value ) { sptr->provision = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"created") ))
		{
			if ( wptr->value ) { sptr->created = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"started") ))
		{
			if ( wptr->value ) { sptr->started = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"completed") ))
		{
			if ( wptr->value ) { sptr->completed = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"duration") ))
		{
			if ( wptr->value ) { sptr->duration = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ a p p l i c a t i o n 	*/
/*	------------------------------------------------------	*/
public int rest_occi_cords_application(FILE * fh,struct cords_application * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.image='%s'\r\n",prefix,nptr,(sptr->image?sptr->image:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.provider='%s'\r\n",prefix,nptr,(sptr->provider?sptr->provider:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.price='%s'\r\n",prefix,nptr,(sptr->price?sptr->price:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.url='%s'\r\n",prefix,nptr,(sptr->url?sptr->url:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.account='%s'\r\n",prefix,nptr,(sptr->account?sptr->account:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.provision='%s'\r\n",prefix,nptr,(sptr->provision?sptr->provision:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.created='%u'\r\n",prefix,nptr,sptr->created);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.started='%u'\r\n",prefix,nptr,sptr->started);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.completed='%u'\r\n",prefix,nptr,sptr->completed);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.duration='%u'\r\n",prefix,nptr,sptr->duration);
	return(0);

}

#endif	/* _application_capplication_c_ */
