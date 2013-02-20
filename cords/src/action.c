/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _action_c_
#define _action_c_

#include "element.h"

#include "action.h"

/*	------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ a c t i o n 	*/
/*	------------------------------------------	*/
public struct cords_action * liberate_cords_action(struct cords_action * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->type )
			 sptr->type = liberate(sptr->type);
		if ( sptr->expression )
			 sptr->expression = liberate(sptr->expression);
		sptr = liberate( sptr );
	}
	return((struct cords_action *) 0);

}

/*	------------------------------------	*/
/*	r e s e t _ c o r d s _ a c t i o n 	*/
/*	------------------------------------	*/
public struct cords_action * reset_cords_action(struct cords_action * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct cords_action*) 0;
		sptr->next = (struct cords_action*) 0;
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->type = (char*) 0;
		sptr->expression = (char*) 0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ a c t i o n 	*/
/*	------------------------------------------	*/
public struct cords_action * allocate_cords_action()
{
	struct cords_action * sptr;
	if (!( sptr = allocate( sizeof( struct cords_action ) ) ))
		return( sptr );
	else	return( reset_cords_action(sptr) );
}

/*	------------------------------------	*/
/*	x m l i n _ c o r d s _ a c t i o n 	*/
/*	------------------------------------	*/
public int xmlin_cords_action(struct cords_action * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"type") ))
		{
			if ( wptr->value ) { sptr->type = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"expression") ))
		{
			if ( wptr->value ) { sptr->expression = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	--------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ a c t i o n 	*/
/*	--------------------------------------------	*/
public int rest_occi_cords_action(FILE * fh,struct cords_action * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.type='%s'\r\n",prefix,nptr,(sptr->type?sptr->type:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.expression='%s'\r\n",prefix,nptr,(sptr->expression?sptr->expression:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _action_caction_c_ */
