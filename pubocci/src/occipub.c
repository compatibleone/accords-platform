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
#ifndef _occipub_c_
#define _occipub_c_

#include "occipub.h"

/*	--------------------------------------------------	*/
/*	l i b e r a t e _ o c c i _ p u b l i c a t i o n 	*/
/*	--------------------------------------------------	*/
public struct occi_publication * liberate_occi_publication(struct occi_publication * sptr)
{
	if ( sptr )
	{
		if ( sptr->category )
			 sptr->category = liberate(sptr->category);
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		sptr = liberate( sptr );
	}
	return((struct occi_publication *) 0);

}

/*	--------------------------------------------	*/
/*	r e s e t _ o c c i _ p u b l i c a t i o n 	*/
/*	--------------------------------------------	*/
public struct occi_publication * reset_occi_publication(struct occi_publication * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct occi_publication*) 0;
		sptr->next = (struct occi_publication*) 0;
		sptr->parent = (struct occi_publisher*) 0;
		sptr->category = (char*) 0;
		sptr->id = (char*) 0;
	}
	return(sptr);

}

/*	--------------------------------------------------	*/
/*	a l l o c a t e _ o c c i _ p u b l i c a t i o n 	*/
/*	--------------------------------------------------	*/
public struct occi_publication * allocate_occi_publication()
{
	struct occi_publication * sptr;
	if (!( sptr = allocate( sizeof( struct occi_publication ) ) ))
		return( sptr );
	else	return( reset_occi_publication(sptr) );
}

/*	----------------------------------------	*/
/*	a d d _ o c c i _ p u b l i c a t i o n 	*/
/*	----------------------------------------	*/
public struct occi_publication * add_occi_publication(struct occi_publisher * pptr)
{
	struct occi_publication * sptr;
	if (!( sptr = allocate( sizeof( struct occi_publication ) ) ))
		return( sptr );
	else if (!( sptr = reset_occi_publication(sptr) ))
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

/*	------------------------------------------	*/
/*	d r o p _ o c c i _ p u b l i c a t i o n 	*/
/*	------------------------------------------	*/
public struct occi_publication * drop_occi_publication(struct occi_publication * sptr)
{
	if ( sptr )
	{
		if (!( sptr->parent )) return(sptr);
		if (!( sptr->previous ))
		{
			if (!( sptr->parent->first = sptr->next ))
				sptr->parent->last = (struct occi_publication *) 0;
			else	sptr->parent->first->previous = (struct occi_publication *) 0;
		}
		else if (!( sptr->previous->next = sptr->next ))
			sptr->parent->last = sptr->previous;
		if (!( sptr->next ))
		{
			if (!( sptr->parent->last = sptr->previous ))
				sptr->parent->first = (struct occi_publication *) 0;
			else	sptr->parent->last->next = (struct occi_publication *) 0;
		}
		else if (!( sptr->next->previous = sptr->previous ))
			sptr->parent->first = sptr->next;
		sptr = liberate_occi_publication(sptr);
	}
	return((struct occi_publication *) 0);
}

#endif	/* _occipub_c_ */
