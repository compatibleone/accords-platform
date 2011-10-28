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
#ifndef _occim_c_
#define _occim_c_

#include "occim.h"

/*	------------------------------------------	*/
/*	l i b e r a t e _ o c c i _ m a n a g e r 	*/
/*	------------------------------------------	*/
public struct occi_manager * liberate_occi_manager(struct occi_manager * sptr)
{
	struct occi_client * mptr;
	if ( sptr )
	{
		if ( sptr->headers )
			 sptr->headers = liberate_rest_header(sptr->headers);
		while ((mptr = sptr->first) != (struct occi_client *) 0)
		{
			sptr->first = mptr->next;
			mptr = liberate_occi_client( mptr );
		}
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->mimetype )
			 sptr->mimetype = liberate(sptr->mimetype);
		sptr = liberate( sptr );
	}
	return((struct occi_manager *) 0);

}

/*	------------------------------------	*/
/*	r e s e t _ o c c i _ m a n a g e r 	*/
/*	------------------------------------	*/
public struct occi_manager * reset_occi_manager(struct occi_manager * sptr)
{
	if ( sptr )
	{
		sptr->headers = (struct rest_header*) 0;
		sptr->first = (struct occi_client*) 0;
		sptr->last = (struct occi_client*) 0;
		sptr->name = (char*) 0;
		sptr->mimetype = (char*) 0;
		sptr->clients =  0;
		sptr->optimise =  0;
	}
	return(sptr);

}

/*	------------------------------------------	*/
/*	a l l o c a t e _ o c c i _ m a n a g e r 	*/
/*	------------------------------------------	*/
public struct occi_manager * allocate_occi_manager()
{
	struct occi_manager * sptr;
	if (!( sptr = allocate( sizeof( struct occi_manager ) ) ))
		return( sptr );
	else	return( reset_occi_manager(sptr) );
}

#endif	/* _occim_c_ */
