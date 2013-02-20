/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _agreement_c_
#define _agreement_c_

#include "element.h"

#include "agreement.h"

/*	------------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ a g r e e m e n t 	*/
/*	------------------------------------------------	*/
public struct cords_agreement * liberate_cords_agreement(struct cords_agreement * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->description )
			 sptr->description = liberate(sptr->description);
		if ( sptr->initiator )
			 sptr->initiator = liberate(sptr->initiator);
		if ( sptr->responder )
			 sptr->responder = liberate(sptr->responder);
		if ( sptr->serviceprovider )
			 sptr->serviceprovider = liberate(sptr->serviceprovider);
		if ( sptr->initiation )
			 sptr->initiation = liberate(sptr->initiation);
		if ( sptr->expiration )
			 sptr->expiration = liberate(sptr->expiration);
		if ( sptr->templateid )
			 sptr->templateid = liberate(sptr->templateid);
		if ( sptr->templatename )
			 sptr->templatename = liberate(sptr->templatename);
		sptr = liberate( sptr );
	}
	return((struct cords_agreement *) 0);

}

/*	------------------------------------------	*/
/*	r e s e t _ c o r d s _ a g r e e m e n t 	*/
/*	------------------------------------------	*/
public struct cords_agreement * reset_cords_agreement(struct cords_agreement * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->description = (char*) 0;
		sptr->initiator = (char*) 0;
		sptr->responder = (char*) 0;
		sptr->serviceprovider = (char*) 0;
		sptr->initiation = (char*) 0;
		sptr->expiration = (char*) 0;
		sptr->templateid = (char*) 0;
		sptr->templatename = (char*) 0;
		sptr->termss =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	------------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ a g r e e m e n t 	*/
/*	------------------------------------------------	*/
public struct cords_agreement * allocate_cords_agreement()
{
	struct cords_agreement * sptr;
	if (!( sptr = allocate( sizeof( struct cords_agreement ) ) ))
		return( sptr );
	else	return( reset_cords_agreement(sptr) );
}

/*	------------------------------------------	*/
/*	x m l i n _ c o r d s _ a g r e e m e n t 	*/
/*	------------------------------------------	*/
public int xmlin_cords_agreement(struct cords_agreement * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"description") ))
		{
			if ( wptr->value ) { sptr->description = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"initiator") ))
		{
			if ( wptr->value ) { sptr->initiator = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"responder") ))
		{
			if ( wptr->value ) { sptr->responder = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"serviceprovider") ))
		{
			if ( wptr->value ) { sptr->serviceprovider = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"initiation") ))
		{
			if ( wptr->value ) { sptr->initiation = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"expiration") ))
		{
			if ( wptr->value ) { sptr->expiration = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"templateid") ))
		{
			if ( wptr->value ) { sptr->templateid = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"templatename") ))
		{
			if ( wptr->value ) { sptr->templatename = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"termss") ))
		{
			if ( wptr->value ) { sptr->termss = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	--------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ a g r e e m e n t 	*/
/*	--------------------------------------------------	*/
public int rest_occi_cords_agreement(FILE * fh,struct cords_agreement * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.description='%s'\r\n",prefix,nptr,(sptr->description?sptr->description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.initiator='%s'\r\n",prefix,nptr,(sptr->initiator?sptr->initiator:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.responder='%s'\r\n",prefix,nptr,(sptr->responder?sptr->responder:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.serviceprovider='%s'\r\n",prefix,nptr,(sptr->serviceprovider?sptr->serviceprovider:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.initiation='%s'\r\n",prefix,nptr,(sptr->initiation?sptr->initiation:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.expiration='%s'\r\n",prefix,nptr,(sptr->expiration?sptr->expiration:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.templateid='%s'\r\n",prefix,nptr,(sptr->templateid?sptr->templateid:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.templatename='%s'\r\n",prefix,nptr,(sptr->templatename?sptr->templatename:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.termss='%u'\r\n",prefix,nptr,sptr->termss);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _agreement_cagreement_c_ */
