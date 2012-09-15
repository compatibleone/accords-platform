/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _paas_version_instance_c_
#define _paas_version_instance_c_

#include "element.h"

#include "paas_version_instance.h"

/*	------------------------------------------------------------	*/
/*	l i b e r a t e _ p a a s _ v e r s i o n _ i n s t a n c e 	*/
/*	------------------------------------------------------------	*/
public struct paas_version_instance * liberate_paas_version_instance(struct paas_version_instance * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->date_instantiated )
			 sptr->date_instantiated = liberate(sptr->date_instantiated);
		if ( sptr->description )
			 sptr->description = liberate(sptr->description);
		if ( sptr->state )
			 sptr->state = liberate(sptr->state);
		if ( sptr->default_instance )
			 sptr->default_instance = liberate(sptr->default_instance);
		sptr = liberate( sptr );
	}
	return((struct paas_version_instance *) 0);

}

/*	------------------------------------------------------	*/
/*	r e s e t _ p a a s _ v e r s i o n _ i n s t a n c e 	*/
/*	------------------------------------------------------	*/
public struct paas_version_instance * reset_paas_version_instance(struct paas_version_instance * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->date_instantiated = (char*) 0;
		sptr->description = (char*) 0;
		sptr->state = (char*) 0;
		sptr->default_instance = (char*) 0;
		sptr->status =  0;
	}
	return(sptr);

}

/*	------------------------------------------------------------	*/
/*	a l l o c a t e _ p a a s _ v e r s i o n _ i n s t a n c e 	*/
/*	------------------------------------------------------------	*/
public struct paas_version_instance * allocate_paas_version_instance()
{
	struct paas_version_instance * sptr;
	if (!( sptr = allocate( sizeof( struct paas_version_instance ) ) ))
		return( sptr );
	else	return( reset_paas_version_instance(sptr) );
}

/*	------------------------------------------------------	*/
/*	x m l i n _ p a a s _ v e r s i o n _ i n s t a n c e 	*/
/*	------------------------------------------------------	*/
public int xmlin_paas_version_instance(struct paas_version_instance * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"date_instantiated") ))
		{
			if ( wptr->value ) { sptr->date_instantiated = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"description") ))
		{
			if ( wptr->value ) { sptr->description = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"default_instance") ))
		{
			if ( wptr->value ) { sptr->default_instance = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"status") ))
		{
			if ( wptr->value ) { sptr->status = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	--------------------------------------------------------------	*/
/*	r e s t _ o c c i _ p a a s _ v e r s i o n _ i n s t a n c e 	*/
/*	--------------------------------------------------------------	*/
public int rest_occi_paas_version_instance(FILE * fh,struct paas_version_instance * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.date_instantiated='%s'\r\n",prefix,nptr,(sptr->date_instantiated?sptr->date_instantiated:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.description='%s'\r\n",prefix,nptr,(sptr->description?sptr->description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%s'\r\n",prefix,nptr,(sptr->state?sptr->state:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.default_instance='%s'\r\n",prefix,nptr,(sptr->default_instance?sptr->default_instance:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.status='%u'\r\n",prefix,nptr,sptr->status);
	return(0);

}

#endif	/* _paas_version_instance_cpaas_version_instance_c_ */
