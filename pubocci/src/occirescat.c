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
#ifndef _occirescat_c_
#define _occirescat_c_

#include "occirescat.h"

/*	--------------------------------------------------------------	*/
/*	l i b e r a t e _ o c c i _ r e s o l v e d _ c a t e g o r y 	*/
/*	--------------------------------------------------------------	*/
public struct occi_resolved_category * liberate_occi_resolved_category(struct occi_resolved_category * sptr)
{
	struct occi_resolved_agency * mptr;
	if ( sptr )
	{
		if ( sptr->category )
			 sptr->category = liberate(sptr->category);
		while ((mptr = sptr->first) != (struct occi_resolved_agency *) 0)
		{
			sptr->first = mptr->next;
			mptr = liberate_occi_resolved_agency( mptr );
		}
		sptr = liberate( sptr );
	}
	return((struct occi_resolved_category *) 0);

}

/*	--------------------------------------------------------	*/
/*	r e s e t _ o c c i _ r e s o l v e d _ c a t e g o r y 	*/
/*	--------------------------------------------------------	*/
public struct occi_resolved_category * reset_occi_resolved_category(struct occi_resolved_category * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct occi_resolved_category*) 0;
		sptr->next = (struct occi_resolved_category*) 0;
		sptr->parent = (struct occi_resolver_manager*) 0;
		sptr->category = (char*) 0;
		sptr->first = (struct occi_resolved_agency*) 0;
		sptr->last = (struct occi_resolved_agency*) 0;
	}
	return(sptr);

}

/*	--------------------------------------------------------------	*/
/*	a l l o c a t e _ o c c i _ r e s o l v e d _ c a t e g o r y 	*/
/*	--------------------------------------------------------------	*/
public struct occi_resolved_category * allocate_occi_resolved_category()
{
	struct occi_resolved_category * sptr;
	if (!( sptr = allocate( sizeof( struct occi_resolved_category ) ) ))
		return( sptr );
	else	return( reset_occi_resolved_category(sptr) );
}

/*	----------------------------------------------------	*/
/*	a d d _ o c c i _ r e s o l v e d _ c a t e g o r y 	*/
/*	----------------------------------------------------	*/
public struct occi_resolved_category * add_occi_resolved_category(struct occi_resolver_manager * pptr)
{
	struct occi_resolved_category * sptr;
	if (!( sptr = allocate( sizeof( struct occi_resolved_category ) ) ))
		return( sptr );
	else if (!( sptr = reset_occi_resolved_category(sptr) ))
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

/*	------------------------------------------------------	*/
/*	d r o p _ o c c i _ r e s o l v e d _ c a t e g o r y 	*/
/*	------------------------------------------------------	*/
public struct occi_resolved_category * drop_occi_resolved_category(struct occi_resolved_category * sptr)
{
	if ( sptr )
	{
		if (!( sptr->parent )) return(sptr);
		if (!( sptr->previous ))
		{
			if (!( sptr->parent->first = sptr->next ))
				sptr->parent->last = (struct occi_resolved_category *) 0;
			else	sptr->parent->first->previous = (struct occi_resolved_category *) 0;
		}
		else if (!( sptr->previous->next = sptr->next ))
			sptr->parent->last = sptr->previous;
		if (!( sptr->next ))
		{
			if (!( sptr->parent->last = sptr->previous ))
				sptr->parent->first = (struct occi_resolved_category *) 0;
			else	sptr->parent->last->next = (struct occi_resolved_category *) 0;
		}
		else if (!( sptr->next->previous = sptr->previous ))
			sptr->parent->first = sptr->next;
		sptr = liberate_occi_resolved_category(sptr);
	}
	return((struct occi_resolved_category *) 0);
}

#endif	/* _occirescat_c_ */
