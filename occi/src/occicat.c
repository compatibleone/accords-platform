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
#ifndef _occicat_c_
#define _occicat_c_

#include "occicat.h"

/*	____________________________________________	*/
/*	l i b e r a t e _ o c c i _ c a t e g o r y 	*/
/*	____________________________________________	*/
public struct occi_category * liberate_occi_category(struct occi_category * sptr)
{
	struct occi_attribute * mptr;
	struct occi_action * aptr;
	if ( sptr )
	{
		while ((mptr = sptr->first) != (struct occi_attribute *) 0)
		{
			sptr->first = mptr->next;
			mptr = liberate_occi_attribute( mptr );
		}
		while ((aptr = sptr->firstact) != (struct occi_action *) 0)
		{
			sptr->firstact = aptr->next;
			aptr = liberate_occi_action( aptr );
		}
		if ( sptr->domain )
			 sptr->domain = liberate(sptr->domain);
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->scheme )
			 sptr->scheme = liberate(sptr->scheme);
		if ( sptr->class )
			 sptr->class = liberate(sptr->class);
		if ( sptr->title )
			 sptr->title = liberate(sptr->title);
		if ( sptr->rel )
			 sptr->rel = liberate(sptr->rel);
		if ( sptr->location )
			 sptr->location = liberate(sptr->location);
		sptr = liberate( sptr );
	}
	return((struct occi_category *) 0);

}

/*	______________________________________	*/
/*	r e s e t _ o c c i _ c a t e g o r y 	*/
/*	______________________________________	*/
public struct occi_category * reset_occi_category(struct occi_category * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct occi_category*) 0;
		sptr->next = (struct occi_category*) 0;
		sptr->first = (struct occi_attribute*) 0;
		sptr->last = (struct occi_attribute*) 0;
		sptr->firstact = (struct occi_action*) 0;
		sptr->lastact = (struct occi_action*) 0;
		sptr->domain = (char*) 0;
		sptr->access = 0;
		sptr->id = (char*) 0;
		sptr->scheme = (char*) 0;
		sptr->class = (char*) 0;
		sptr->title = (char*) 0;
		sptr->rel = (char*) 0;
		sptr->location = (char*) 0;
	}
	return(sptr);

}

/*	____________________________________________	*/
/*	a l l o c a t e _ o c c i _ c a t e g o r y 	*/
/*	____________________________________________	*/
public struct occi_category * allocate_occi_category()
{
	struct occi_category * sptr;
	if (!( sptr = allocate( sizeof( struct occi_category ) ) ))
		return( sptr );
	else	return( reset_occi_category(sptr) );
}

#endif	/* _occicat_c_ */
