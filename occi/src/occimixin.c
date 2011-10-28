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
#ifndef _occimixin_c_
#define _occimixin_c_

#include "occimixin.h"

/*	--------------------------------------	*/
/*	l i b e r a t e _ o c c i _ m i x i n 	*/
/*	--------------------------------------	*/
public struct occi_mixin * liberate_occi_mixin(struct occi_mixin * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		sptr = liberate( sptr );
	}
	return((struct occi_mixin *) 0);

}

/*	--------------------------------	*/
/*	r e s e t _ o c c i _ m i x i n 	*/
/*	--------------------------------	*/
public struct occi_mixin * reset_occi_mixin(struct occi_mixin * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct occi_mixin*) 0;
		sptr->next = (struct occi_mixin*) 0;
		sptr->id = (char*) 0;
	}
	return(sptr);

}

/*	--------------------------------------	*/
/*	a l l o c a t e _ o c c i _ m i x i n 	*/
/*	--------------------------------------	*/
public struct occi_mixin * allocate_occi_mixin()
{
	struct occi_mixin * sptr;
	if (!( sptr = allocate( sizeof( struct occi_mixin ) ) ))
		return( sptr );
	else	return( reset_occi_mixin(sptr) );
}

#endif	/* _occimixin_c_ */
