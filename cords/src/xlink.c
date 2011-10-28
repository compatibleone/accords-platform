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
#ifndef _xlink_c_
#define _xlink_c_

#include "element.h"

#include "xlink.h"

/*	----------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ x l i n k 	*/
/*	----------------------------------------	*/
public struct cords_xlink * liberate_cords_xlink(struct cords_xlink * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->source )
			 sptr->source = liberate(sptr->source);
		if ( sptr->target )
			 sptr->target = liberate(sptr->target);
		sptr = liberate( sptr );
	}
	return((struct cords_xlink *) 0);

}

/*	----------------------------------	*/
/*	r e s e t _ c o r d s _ x l i n k 	*/
/*	----------------------------------	*/
public struct cords_xlink * reset_cords_xlink(struct cords_xlink * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->source = (char*) 0;
		sptr->target = (char*) 0;
		sptr->status =  0;
	}
	return(sptr);

}

/*	----------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ x l i n k 	*/
/*	----------------------------------------	*/
public struct cords_xlink * allocate_cords_xlink()
{
	struct cords_xlink * sptr;
	if (!( sptr = allocate( sizeof( struct cords_xlink ) ) ))
		return( sptr );
	else	return( reset_cords_xlink(sptr) );
}

/*	----------------------------------	*/
/*	x m l i n _ c o r d s _ x l i n k 	*/
/*	----------------------------------	*/
public int xmlin_cords_xlink(struct cords_xlink * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"source") ))
		{
			if ( wptr->value ) { sptr->source = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"target") ))
		{
			if ( wptr->value ) { sptr->target = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"status") ))
		{
			if ( wptr->value ) { sptr->status = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ x l i n k 	*/
/*	------------------------------------------	*/
public int rest_occi_cords_xlink(FILE * fh,struct cords_xlink * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.source='%s'\r\n",prefix,nptr,(sptr->source?sptr->source:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.target='%s'\r\n",prefix,nptr,(sptr->target?sptr->target:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.status='%u'\r\n",prefix,nptr,sptr->status);
	return(0);

}

#endif	/* _xlink_c_ */
