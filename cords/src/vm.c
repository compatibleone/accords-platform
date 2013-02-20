/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _vm_c_
#define _vm_c_

#include "element.h"

#include "vm.h"

/*	----------------------------------	*/
/*	l i b e r a t e _ c o r d s _ v m 	*/
/*	----------------------------------	*/
public struct cords_vm * liberate_cords_vm(struct cords_vm * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->image )
			 sptr->image = liberate(sptr->image);
		if ( sptr->provider )
			 sptr->provider = liberate(sptr->provider);
		if ( sptr->application )
			 sptr->application = liberate(sptr->application);
		if ( sptr->url )
			 sptr->url = liberate(sptr->url);
		if ( sptr->price )
			 sptr->price = liberate(sptr->price);
		sptr = liberate( sptr );
	}
	return((struct cords_vm *) 0);

}

/*	----------------------------	*/
/*	r e s e t _ c o r d s _ v m 	*/
/*	----------------------------	*/
public struct cords_vm * reset_cords_vm(struct cords_vm * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->image = (char*) 0;
		sptr->provider = (char*) 0;
		sptr->application = (char*) 0;
		sptr->url = (char*) 0;
		sptr->price = (char*) 0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	----------------------------------	*/
/*	a l l o c a t e _ c o r d s _ v m 	*/
/*	----------------------------------	*/
public struct cords_vm * allocate_cords_vm()
{
	struct cords_vm * sptr;
	if (!( sptr = allocate( sizeof( struct cords_vm ) ) ))
		return( sptr );
	else	return( reset_cords_vm(sptr) );
}

/*	----------------------------	*/
/*	x m l i n _ c o r d s _ v m 	*/
/*	----------------------------	*/
public int xmlin_cords_vm(struct cords_vm * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"image") ))
		{
			if ( wptr->value ) { sptr->image = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"provider") ))
		{
			if ( wptr->value ) { sptr->provider = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"application") ))
		{
			if ( wptr->value ) { sptr->application = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"url") ))
		{
			if ( wptr->value ) { sptr->url = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"price") ))
		{
			if ( wptr->value ) { sptr->price = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ v m 	*/
/*	------------------------------------	*/
public int rest_occi_cords_vm(FILE * fh,struct cords_vm * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.image='%s'\r\n",prefix,nptr,(sptr->image?sptr->image:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.provider='%s'\r\n",prefix,nptr,(sptr->provider?sptr->provider:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.application='%s'\r\n",prefix,nptr,(sptr->application?sptr->application:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.url='%s'\r\n",prefix,nptr,(sptr->url?sptr->url:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.price='%s'\r\n",prefix,nptr,(sptr->price?sptr->price:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _vm_cvm_c_ */
