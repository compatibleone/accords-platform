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
#ifndef _authorise_c_
#define _authorise_c_

#include "element.h"

#include "authorise.h"

/*	--------------------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ a u t h o r i s a t i o n 	*/
/*	--------------------------------------------------------	*/
public struct cords_authorisation * liberate_cords_authorisation(struct cords_authorisation * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->account )
			 sptr->account = liberate(sptr->account);
		if ( sptr->user )
			 sptr->user = liberate(sptr->user);
		if ( sptr->pass )
			 sptr->pass = liberate(sptr->pass);
		sptr = liberate( sptr );
	}
	return((struct cords_authorisation *) 0);

}

/*	--------------------------------------------------	*/
/*	r e s e t _ c o r d s _ a u t h o r i s a t i o n 	*/
/*	--------------------------------------------------	*/
public struct cords_authorisation * reset_cords_authorisation(struct cords_authorisation * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->account = (char*) 0;
		sptr->user = (char*) 0;
		sptr->pass = (char*) 0;
		sptr->from =  0;
		sptr->till =  0;
		sptr->ttl =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	--------------------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ a u t h o r i s a t i o n 	*/
/*	--------------------------------------------------------	*/
public struct cords_authorisation * allocate_cords_authorisation()
{
	struct cords_authorisation * sptr;
	if (!( sptr = allocate( sizeof( struct cords_authorisation ) ) ))
		return( sptr );
	else	return( reset_cords_authorisation(sptr) );
}

/*	--------------------------------------------------	*/
/*	x m l i n _ c o r d s _ a u t h o r i s a t i o n 	*/
/*	--------------------------------------------------	*/
public int xmlin_cords_authorisation(struct cords_authorisation * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"account") ))
		{
			if ( wptr->value ) { sptr->account = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"user") ))
		{
			if ( wptr->value ) { sptr->user = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"pass") ))
		{
			if ( wptr->value ) { sptr->pass = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"from") ))
		{
			if ( wptr->value ) { sptr->from = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"till") ))
		{
			if ( wptr->value ) { sptr->till = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"ttl") ))
		{
			if ( wptr->value ) { sptr->ttl = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ a u t h o r i s a t i o n 	*/
/*	----------------------------------------------------------	*/
public int rest_occi_cords_authorisation(FILE * fh,struct cords_authorisation * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.account='%s'\r\n",prefix,nptr,(sptr->account?sptr->account:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.user='%s'\r\n",prefix,nptr,(sptr->user?sptr->user:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.pass='%s'\r\n",prefix,nptr,(sptr->pass?sptr->pass:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.from='%u'\r\n",prefix,nptr,sptr->from);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.till='%u'\r\n",prefix,nptr,sptr->till);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.ttl='%u'\r\n",prefix,nptr,sptr->ttl);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _authorise_c_ */
