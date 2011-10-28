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
#ifndef _occiact_c_
#define _occiact_c_

#include "occiact.h"

/*	________________________________________	*/
/*	l i b e r a t e _ o c c i _ a c t i o n 	*/
/*	________________________________________	*/
public struct occi_action * liberate_occi_action(struct occi_action * sptr)
{
	struct occi_parameter * mptr;
	if ( sptr )
	{
		while ((mptr = sptr->first) != (struct occi_parameter *) 0)
		{
			sptr->first = mptr->next;
			mptr = liberate_occi_parameter( mptr );
		}
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		sptr = liberate( sptr );
	}
	return((struct occi_action *) 0);

}

/*	__________________________________	*/
/*	r e s e t _ o c c i _ a c t i o n 	*/
/*	__________________________________	*/
public struct occi_action * reset_occi_action(struct occi_action * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct occi_action*) 0;
		sptr->next = (struct occi_action*) 0;
		sptr->parent = (struct occi_category*) 0;
		sptr->first = (struct occi_parameter*) 0;
		sptr->last = (struct occi_parameter*) 0;
		sptr->name = (char*) 0;
	}
	return(sptr);

}

/*	________________________________________	*/
/*	a l l o c a t e _ o c c i _ a c t i o n 	*/
/*	________________________________________	*/
public struct occi_action * allocate_occi_action()
{
	struct occi_action * sptr;
	if (!( sptr = allocate( sizeof( struct occi_action ) ) ))
		return( sptr );
	else	return( reset_occi_action(sptr) );
}

/*	______________________________	*/
/*	a d d _ o c c i _ a c t i o n 	*/
/*	______________________________	*/
public struct occi_action * add_occi_action(struct occi_category * pptr)
{
	struct occi_action * sptr;
	if (!( sptr = allocate( sizeof( struct occi_action ) ) ))
		return( sptr );
	else if (!( sptr = reset_occi_action(sptr) ))
		return( sptr );
	else
	{
		if (!( sptr->previous = pptr->lastact ))
			pptr->firstact = sptr;
		else	sptr->previous->next = sptr;
		pptr->lastact = sptr;
		sptr->parent = pptr;
		return( sptr );
	}

}

/*	________________________________	*/
/*	d r o p _ o c c i _ a c t i o n 	*/
/*	________________________________	*/
public struct occi_action * drop_occi_action(struct occi_action * sptr)
{
	if ( sptr )
	{
		if (!( sptr->parent )) return(sptr);
		if (!( sptr->previous ))
		{
			if (!( sptr->parent->firstact = sptr->next ))
				sptr->parent->lastact = (struct occi_action *) 0;
			else	sptr->parent->firstact->previous = (struct occi_action *) 0;
		}
		else if (!( sptr->previous->next = sptr->next ))
			sptr->parent->lastact = sptr->previous;
		if (!( sptr->next ))
		{
			if (!( sptr->parent->lastact = sptr->previous ))
				sptr->parent->firstact = (struct occi_action *) 0;
			else	sptr->parent->lastact->next = (struct occi_action *) 0;
		}
		else if (!( sptr->next->previous = sptr->previous ))
			sptr->parent->firstact = sptr->next;
		sptr = liberate_occi_action(sptr);
	}
	return((struct occi_action *) 0);
}

#endif	/* _occiact_c_ */
