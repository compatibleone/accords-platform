/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _paas_manifest_c_
#define _paas_manifest_c_

#include "element.h"

#include "paas_manifest.h"

/*	--------------------------------------------	*/
/*	l i b e r a t e _ p a a s _ m a n i f e s t 	*/
/*	--------------------------------------------	*/
public struct paas_manifest * liberate_paas_manifest(struct paas_manifest * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->xmlns )
			 sptr->xmlns = liberate(sptr->xmlns);
		sptr = liberate( sptr );
	}
	return((struct paas_manifest *) 0);

}

/*	--------------------------------------	*/
/*	r e s e t _ p a a s _ m a n i f e s t 	*/
/*	--------------------------------------	*/
public struct paas_manifest * reset_paas_manifest(struct paas_manifest * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->xmlns = (char*) 0;
		sptr->status =  0;
	}
	return(sptr);

}

/*	--------------------------------------------	*/
/*	a l l o c a t e _ p a a s _ m a n i f e s t 	*/
/*	--------------------------------------------	*/
public struct paas_manifest * allocate_paas_manifest()
{
	struct paas_manifest * sptr;
	if (!( sptr = allocate( sizeof( struct paas_manifest ) ) ))
		return( sptr );
	else	return( reset_paas_manifest(sptr) );
}

/*	--------------------------------------	*/
/*	x m l i n _ p a a s _ m a n i f e s t 	*/
/*	--------------------------------------	*/
public int xmlin_paas_manifest(struct paas_manifest * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"xmlns") ))
		{
			if ( wptr->value ) { sptr->xmlns = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"status") ))
		{
			if ( wptr->value ) { sptr->status = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------------	*/
/*	r e s t _ o c c i _ p a a s _ m a n i f e s t 	*/
/*	----------------------------------------------	*/
public int rest_occi_paas_manifest(FILE * fh,struct paas_manifest * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.xmlns='%s'\r\n",prefix,nptr,(sptr->xmlns?sptr->xmlns:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.status='%u'\r\n",prefix,nptr,sptr->status);
	return(0);

}

#endif	/* _paas_manifest_cpaas_manifest_c_ */