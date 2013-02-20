/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _manifest_c_
#define _manifest_c_

#include "element.h"

#include "manifest.h"

/*	----------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ m a n i f e s t 	*/
/*	----------------------------------------------	*/
public struct cords_manifest * liberate_cords_manifest(struct cords_manifest * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->plan )
			 sptr->plan = liberate(sptr->plan);
		if ( sptr->nodes )
			 sptr->nodes = liberate(sptr->nodes);
		if ( sptr->configuration )
			 sptr->configuration = liberate(sptr->configuration);
		if ( sptr->release )
			 sptr->release = liberate(sptr->release);
		if ( sptr->interface )
			 sptr->interface = liberate(sptr->interface);
		if ( sptr->account )
			 sptr->account = liberate(sptr->account);
		if ( sptr->security )
			 sptr->security = liberate(sptr->security);
		if ( sptr->zone )
			 sptr->zone = liberate(sptr->zone);
		if ( sptr->description )
			 sptr->description = liberate(sptr->description);
		sptr = liberate( sptr );
	}
	return((struct cords_manifest *) 0);

}

/*	----------------------------------------	*/
/*	r e s e t _ c o r d s _ m a n i f e s t 	*/
/*	----------------------------------------	*/
public struct cords_manifest * reset_cords_manifest(struct cords_manifest * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->plan = (char*) 0;
		sptr->nodes = (char*) 0;
		sptr->configuration = (char*) 0;
		sptr->release = (char*) 0;
		sptr->interface = (char*) 0;
		sptr->account = (char*) 0;
		sptr->security = (char*) 0;
		sptr->zone = (char*) 0;
		sptr->description = (char*) 0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	----------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ m a n i f e s t 	*/
/*	----------------------------------------------	*/
public struct cords_manifest * allocate_cords_manifest()
{
	struct cords_manifest * sptr;
	if (!( sptr = allocate( sizeof( struct cords_manifest ) ) ))
		return( sptr );
	else	return( reset_cords_manifest(sptr) );
}

/*	----------------------------------------	*/
/*	x m l i n _ c o r d s _ m a n i f e s t 	*/
/*	----------------------------------------	*/
public int xmlin_cords_manifest(struct cords_manifest * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"plan") ))
		{
			if ( wptr->value ) { sptr->plan = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"nodes") ))
		{
			if ( wptr->value ) { sptr->nodes = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"configuration") ))
		{
			if ( wptr->value ) { sptr->configuration = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"release") ))
		{
			if ( wptr->value ) { sptr->release = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"interface") ))
		{
			if ( wptr->value ) { sptr->interface = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"account") ))
		{
			if ( wptr->value ) { sptr->account = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"security") ))
		{
			if ( wptr->value ) { sptr->security = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"zone") ))
		{
			if ( wptr->value ) { sptr->zone = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"description") ))
		{
			if ( wptr->value ) { sptr->description = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ m a n i f e s t 	*/
/*	------------------------------------------------	*/
public int rest_occi_cords_manifest(FILE * fh,struct cords_manifest * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.plan='%s'\r\n",prefix,nptr,(sptr->plan?sptr->plan:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.nodes='%s'\r\n",prefix,nptr,(sptr->nodes?sptr->nodes:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.configuration='%s'\r\n",prefix,nptr,(sptr->configuration?sptr->configuration:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.release='%s'\r\n",prefix,nptr,(sptr->release?sptr->release:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.interface='%s'\r\n",prefix,nptr,(sptr->interface?sptr->interface:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.account='%s'\r\n",prefix,nptr,(sptr->account?sptr->account:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.security='%s'\r\n",prefix,nptr,(sptr->security?sptr->security:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.zone='%s'\r\n",prefix,nptr,(sptr->zone?sptr->zone:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.description='%s'\r\n",prefix,nptr,(sptr->description?sptr->description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _manifest_cmanifest_c_ */
