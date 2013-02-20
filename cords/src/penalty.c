/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _penalty_c_
#define _penalty_c_

#include "element.h"

#include "penalty.h"

/*	--------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ p e n a l t y 	*/
/*	--------------------------------------------	*/
public struct cords_penalty * liberate_cords_penalty(struct cords_penalty * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->account )
			 sptr->account = liberate(sptr->account);
		if ( sptr->agreement )
			 sptr->agreement = liberate(sptr->agreement);
		if ( sptr->contract )
			 sptr->contract = liberate(sptr->contract);
		if ( sptr->control )
			 sptr->control = liberate(sptr->control);
		if ( sptr->data )
			 sptr->data = liberate(sptr->data);
		sptr = liberate( sptr );
	}
	return((struct cords_penalty *) 0);

}

/*	--------------------------------------	*/
/*	r e s e t _ c o r d s _ p e n a l t y 	*/
/*	--------------------------------------	*/
public struct cords_penalty * reset_cords_penalty(struct cords_penalty * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->account = (char*) 0;
		sptr->agreement = (char*) 0;
		sptr->contract = (char*) 0;
		sptr->control = (char*) 0;
		sptr->data = (char*) 0;
		sptr->sequence =  0;
		sptr->timestamp =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	--------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ p e n a l t y 	*/
/*	--------------------------------------------	*/
public struct cords_penalty * allocate_cords_penalty()
{
	struct cords_penalty * sptr;
	if (!( sptr = allocate( sizeof( struct cords_penalty ) ) ))
		return( sptr );
	else	return( reset_cords_penalty(sptr) );
}

/*	--------------------------------------	*/
/*	x m l i n _ c o r d s _ p e n a l t y 	*/
/*	--------------------------------------	*/
public int xmlin_cords_penalty(struct cords_penalty * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"account") ))
		{
			if ( wptr->value ) { sptr->account = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"agreement") ))
		{
			if ( wptr->value ) { sptr->agreement = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"contract") ))
		{
			if ( wptr->value ) { sptr->contract = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"control") ))
		{
			if ( wptr->value ) { sptr->control = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"data") ))
		{
			if ( wptr->value ) { sptr->data = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"sequence") ))
		{
			if ( wptr->value ) { sptr->sequence = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"timestamp") ))
		{
			if ( wptr->value ) { sptr->timestamp = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ p e n a l t y 	*/
/*	----------------------------------------------	*/
public int rest_occi_cords_penalty(FILE * fh,struct cords_penalty * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.account='%s'\r\n",prefix,nptr,(sptr->account?sptr->account:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.agreement='%s'\r\n",prefix,nptr,(sptr->agreement?sptr->agreement:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.contract='%s'\r\n",prefix,nptr,(sptr->contract?sptr->contract:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.control='%s'\r\n",prefix,nptr,(sptr->control?sptr->control:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.data='%s'\r\n",prefix,nptr,(sptr->data?sptr->data:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.sequence='%u'\r\n",prefix,nptr,sptr->sequence);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.timestamp='%u'\r\n",prefix,nptr,sptr->timestamp);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _penalty_cpenalty_c_ */
