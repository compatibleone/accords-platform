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
#ifndef _occiatb_c_
#define _occiatb_c_

#include "occiatb.h"

/*	______________________________________________	*/
/*	l i b e r a t e _ o c c i _ a t t r i b u t e 	*/
/*	______________________________________________	*/
public struct occi_attribute * liberate_occi_attribute(struct occi_attribute * sptr)
{
	if ( sptr )
	{
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		sptr = liberate( sptr );
	}
	return((struct occi_attribute *) 0);

}

/*	________________________________________	*/
/*	r e s e t _ o c c i _ a t t r i b u t e 	*/
/*	________________________________________	*/
public struct occi_attribute * reset_occi_attribute(struct occi_attribute * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct occi_attribute*) 0;
		sptr->next = (struct occi_attribute*) 0;
		sptr->parent = (struct occi_category*) 0;
		sptr->name = (char*) 0;
		sptr->mandatory =  0;
		sptr->immutable =  0;
	}
	return(sptr);

}

/*	______________________________________________	*/
/*	a l l o c a t e _ o c c i _ a t t r i b u t e 	*/
/*	______________________________________________	*/
public struct occi_attribute * allocate_occi_attribute()
{
	struct occi_attribute * sptr;
	if (!( sptr = allocate( sizeof( struct occi_attribute ) ) ))
		return( sptr );
	else	return( reset_occi_attribute(sptr) );
}

/*	____________________________________	*/
/*	a d d _ o c c i _ a t t r i b u t e 	*/
/*	____________________________________	*/
public struct occi_attribute * add_occi_attribute(struct occi_category * pptr)
{
	struct occi_attribute * sptr;
	if (!( sptr = allocate( sizeof( struct occi_attribute ) ) ))
		return( sptr );
	else if (!( sptr = reset_occi_attribute(sptr) ))
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

/*	______________________________________	*/
/*	d r o p _ o c c i _ a t t r i b u t e 	*/
/*	______________________________________	*/
public struct occi_attribute * drop_occi_attribute(struct occi_attribute * sptr)
{
	if ( sptr )
	{
		if (!( sptr->parent )) return(sptr);
		if (!( sptr->previous ))
		{
			if (!( sptr->parent->first = sptr->next ))
				sptr->parent->last = (struct occi_attribute *) 0;
			else	sptr->parent->first->previous = (struct occi_attribute *) 0;
		}
		else if (!( sptr->previous->next = sptr->next ))
			sptr->parent->last = sptr->previous;
		if (!( sptr->next ))
		{
			if (!( sptr->parent->last = sptr->previous ))
				sptr->parent->first = (struct occi_attribute *) 0;
			else	sptr->parent->last->next = (struct occi_attribute *) 0;
		}
		else if (!( sptr->next->previous = sptr->previous ))
			sptr->parent->first = sptr->next;
		sptr = liberate_occi_attribute(sptr);
	}
	return((struct occi_attribute *) 0);
}

#endif	/* _occiatb_c_ */
