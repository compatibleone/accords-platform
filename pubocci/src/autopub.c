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
#ifndef _autopub_c_
#define _autopub_c_

#include "autopub.h"

/*	--------------------------------------------------	*/
/*	l i b e r a t e _ a u t o _ p u b l i c a t i o n 	*/
/*	--------------------------------------------------	*/
public struct auto_publication * liberate_auto_publication(struct auto_publication * sptr)
{
	if ( sptr )
	{
		if ( sptr->user )
			 sptr->user = liberate(sptr->user);
		if ( sptr->password )
			 sptr->password = liberate(sptr->password);
		if ( sptr->url )
			 sptr->url = liberate(sptr->url);
		if ( sptr->agent )
			 sptr->agent = liberate(sptr->agent);
		sptr = liberate( sptr );
	}
	return((struct auto_publication *) 0);

}

/*	--------------------------------------------	*/
/*	r e s e t _ a u t o _ p u b l i c a t i o n 	*/
/*	--------------------------------------------	*/
public struct auto_publication * reset_auto_publication(struct auto_publication * sptr)
{
	if ( sptr )
	{
		sptr->item =  0;
		sptr->user = (char*) 0;
		sptr->password = (char*) 0;
		sptr->url = (char*) 0;
		sptr->agent = (char*) 0;
	}
	return(sptr);

}

/*	--------------------------------------------------	*/
/*	a l l o c a t e _ a u t o _ p u b l i c a t i o n 	*/
/*	--------------------------------------------------	*/
public struct auto_publication * allocate_auto_publication()
{
	struct auto_publication * sptr;
	if (!( sptr = allocate( sizeof( struct auto_publication ) ) ))
		return( sptr );
	else	return( reset_auto_publication(sptr) );
}

#endif	/* _autopub_c_ */
