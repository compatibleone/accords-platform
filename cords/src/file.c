/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _file_c_
#define _file_c_

#include "element.h"

#include "file.h"

/*	--------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ f i l e 	*/
/*	--------------------------------------	*/
public struct cords_file * liberate_cords_file(struct cords_file * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->type )
			 sptr->type = liberate(sptr->type);
		if ( sptr->permissions )
			 sptr->permissions = liberate(sptr->permissions);
		sptr = liberate( sptr );
	}
	return((struct cords_file *) 0);

}

/*	--------------------------------	*/
/*	r e s e t _ c o r d s _ f i l e 	*/
/*	--------------------------------	*/
public struct cords_file * reset_cords_file(struct cords_file * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->state =  0;
		sptr->length =  0;
		sptr->name = (char*) 0;
		sptr->type = (char*) 0;
		sptr->permissions = (char*) 0;
	}
	return(sptr);

}

/*	--------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ f i l e 	*/
/*	--------------------------------------	*/
public struct cords_file * allocate_cords_file()
{
	struct cords_file * sptr;
	if (!( sptr = allocate( sizeof( struct cords_file ) ) ))
		return( sptr );
	else	return( reset_cords_file(sptr) );
}

/*	--------------------------------	*/
/*	x m l i n _ c o r d s _ f i l e 	*/
/*	--------------------------------	*/
public int xmlin_cords_file(struct cords_file * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"length") ))
		{
			if ( wptr->value ) { sptr->length = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"name") ))
		{
			if ( wptr->value ) { sptr->name = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"type") ))
		{
			if ( wptr->value ) { sptr->type = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"permissions") ))
		{
			if ( wptr->value ) { sptr->permissions = allocate_string(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ f i l e 	*/
/*	----------------------------------------	*/
public int rest_occi_cords_file(FILE * fh,struct cords_file * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.length='%u'\r\n",prefix,nptr,sptr->length);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.type='%s'\r\n",prefix,nptr,(sptr->type?sptr->type:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.permissions='%s'\r\n",prefix,nptr,(sptr->permissions?sptr->permissions:""));
	return(0);

}

#endif	/* _file_cfile_c_ */
