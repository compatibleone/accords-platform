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
#ifndef _occilink_c_
#define _occilink_c_

#include "occilink.h"

/*	------------------------------------	*/
/*	l i b e r a t e _ o c c i _ l i n k 	*/
/*	------------------------------------	*/
public struct occi_link * liberate_occi_link(struct occi_link * sptr)
{
	if ( sptr )
	{
		if ( sptr->from )
			 sptr->from = liberate(sptr->from);
		if ( sptr->to )
			 sptr->to = liberate(sptr->to);
		sptr = liberate( sptr );
	}
	return((struct occi_link *) 0);

}

/*	------------------------------	*/
/*	r e s e t _ o c c i _ l i n k 	*/
/*	------------------------------	*/
public struct occi_link * reset_occi_link(struct occi_link * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct occi_link*) 0;
		sptr->next = (struct occi_link*) 0;
		sptr->from = (char*) 0;
		sptr->to = (char*) 0;
	}
	return(sptr);

}

/*	------------------------------------	*/
/*	a l l o c a t e _ o c c i _ l i n k 	*/
/*	------------------------------------	*/
public struct occi_link * allocate_occi_link()
{
	struct occi_link * sptr;
	if (!( sptr = allocate( sizeof( struct occi_link ) ) ))
		return( sptr );
	else	return( reset_occi_link(sptr) );
}

#endif	/* _occilink_c_ */
