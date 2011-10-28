/* ------------------------------------------------------------------------------------	*/
/*				 CompatibleOne Cloudware				*/
/* ------------------------------------------------------------------------------------ */
/*											*/
/* Ce fichier fait partie de ce(tte) oeuvre de Iain James Marshall et est mise a 	*/
/* disposition selon les termes de la licence Creative Commons Paternit‚ : 		*/
/*											*/
/*			 	Pas d'Utilisation Commerciale 				*/
/*				Pas de Modification 					*/
/*				3.0 non transcrit.					*/
/*											*/
/* ------------------------------------------------------------------------------------ */
/* 			Copyright (c) 2011 Iain James Marshall for Prologue 		*/
/*				   All rights reserved					*/
/* ------------------------------------------------------------------------------------ */
#ifndef _transaction_c_
#define _transaction_c_

#include "element.h"

#include "transaction.h"

/*	----------------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ t r a n s a c t i o n 	*/
/*	----------------------------------------------------	*/
public struct cords_transaction * liberate_cords_transaction(struct cords_transaction * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->authorisation )
			 sptr->authorisation = liberate(sptr->authorisation);
		if ( sptr->source )
			 sptr->source = liberate(sptr->source);
		if ( sptr->target )
			 sptr->target = liberate(sptr->target);
		if ( sptr->description )
			 sptr->description = liberate(sptr->description);
		if ( sptr->value )
			 sptr->value = liberate(sptr->value);
		if ( sptr->currency )
			 sptr->currency = liberate(sptr->currency);
		sptr = liberate( sptr );
	}
	return((struct cords_transaction *) 0);

}

/*	----------------------------------------------	*/
/*	r e s e t _ c o r d s _ t r a n s a c t i o n 	*/
/*	----------------------------------------------	*/
public struct cords_transaction * reset_cords_transaction(struct cords_transaction * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->authorisation = (char*) 0;
		sptr->source = (char*) 0;
		sptr->target = (char*) 0;
		sptr->description = (char*) 0;
		sptr->value = (char*) 0;
		sptr->currency = (char*) 0;
		sptr->when =  0;
		sptr->validity =  0;
		sptr->type =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	----------------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ t r a n s a c t i o n 	*/
/*	----------------------------------------------------	*/
public struct cords_transaction * allocate_cords_transaction()
{
	struct cords_transaction * sptr;
	if (!( sptr = allocate( sizeof( struct cords_transaction ) ) ))
		return( sptr );
	else	return( reset_cords_transaction(sptr) );
}

/*	----------------------------------------------	*/
/*	x m l i n _ c o r d s _ t r a n s a c t i o n 	*/
/*	----------------------------------------------	*/
public int xmlin_cords_transaction(struct cords_transaction * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"authorisation") ))
		{
			if ( wptr->value ) { sptr->authorisation = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"source") ))
		{
			if ( wptr->value ) { sptr->source = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"target") ))
		{
			if ( wptr->value ) { sptr->target = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"description") ))
		{
			if ( wptr->value ) { sptr->description = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"value") ))
		{
			if ( wptr->value ) { sptr->value = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"currency") ))
		{
			if ( wptr->value ) { sptr->currency = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"when") ))
		{
			if ( wptr->value ) { sptr->when = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"validity") ))
		{
			if ( wptr->value ) { sptr->validity = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"type") ))
		{
			if ( wptr->value ) { sptr->type = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ t r a n s a c t i o n 	*/
/*	------------------------------------------------------	*/
public int rest_occi_cords_transaction(FILE * fh,struct cords_transaction * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.authorisation='%s'\r\n",prefix,nptr,(sptr->authorisation?sptr->authorisation:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.source='%s'\r\n",prefix,nptr,(sptr->source?sptr->source:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.target='%s'\r\n",prefix,nptr,(sptr->target?sptr->target:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.description='%s'\r\n",prefix,nptr,(sptr->description?sptr->description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.value='%s'\r\n",prefix,nptr,(sptr->value?sptr->value:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.currency='%s'\r\n",prefix,nptr,(sptr->currency?sptr->currency:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.when='%u'\r\n",prefix,nptr,sptr->when);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.validity='%u'\r\n",prefix,nptr,sptr->validity);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.type='%u'\r\n",prefix,nptr,sptr->type);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _transaction_c_ */
