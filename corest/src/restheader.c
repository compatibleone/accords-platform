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
#ifndef _restheader_c_
#define _restheader_c_

#include "restheader.h"

/*	----------------------------------------	*/
/*	l i b e r a t e _ r e s t _ h e a d e r 	*/
/*	----------------------------------------	*/
public struct rest_header * liberate_rest_header(struct rest_header * sptr)
{
	if ( sptr )
	{
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->value )
			 sptr->value = liberate(sptr->value);
		sptr = liberate( sptr );
	}
	return((struct rest_header *) 0);

}

/*	----------------------------------	*/
/*	r e s e t _ r e s t _ h e a d e r 	*/
/*	----------------------------------	*/
public struct rest_header * reset_rest_header(struct rest_header * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct rest_header*) 0;
		sptr->next = (struct rest_header*) 0;
		sptr->parent = (struct rest_request*) 0;
		sptr->name = (char*) 0;
		sptr->value = (char*) 0;
	}
	return(sptr);

}

/*	----------------------------------------	*/
/*	a l l o c a t e _ r e s t _ h e a d e r 	*/
/*	----------------------------------------	*/
public struct rest_header * allocate_rest_header()
{
	struct rest_header * sptr;
	if (!( sptr = allocate( sizeof( struct rest_header ) ) ))
		return( sptr );
	else	return( reset_rest_header(sptr) );
}

/*	------------------------------	*/
/*	a d d _ r e s t _ h e a d e r 	*/
/*	------------------------------	*/
public struct rest_header * add_rest_header(struct rest_request * pptr)
{
	struct rest_header * sptr;
	if (!( sptr = allocate( sizeof( struct rest_header ) ) ))
		return( sptr );
	else if (!( sptr = reset_rest_header(sptr) ))
		return( sptr );
	else
	{
		if (!( sptr->previous = pptr->last ))
			pptr->first = sptr;
		else	sptr->previous->next = sptr;
		pptr->last = sptr;
		sptr->parent = pptr;
		return( sptr );
	}

}

/*	--------------------------------	*/
/*	d r o p _ r e s t _ h e a d e r 	*/
/*	--------------------------------	*/
public struct rest_header * drop_rest_header(struct rest_header * sptr)
{
	if ( sptr )
	{
		if (!( sptr->parent )) return(sptr);
		if (!( sptr->previous ))
		{
			if (!( sptr->parent->first = sptr->next ))
				sptr->parent->last = (struct rest_header *) 0;
			else	sptr->parent->first->previous = (struct rest_header *) 0;
		}
		else if (!( sptr->previous->next = sptr->next ))
			sptr->parent->last = sptr->previous;
		if (!( sptr->next ))
		{
			if (!( sptr->parent->last = sptr->previous ))
				sptr->parent->first = (struct rest_header *) 0;
			else	sptr->parent->last->next = (struct rest_header *) 0;
		}
		else if (!( sptr->next->previous = sptr->previous ))
			sptr->parent->first = sptr->next;
		sptr = liberate_rest_header(sptr);
	}
	return((struct rest_header *) 0);
}

#endif	/* _restheader_c_ */
