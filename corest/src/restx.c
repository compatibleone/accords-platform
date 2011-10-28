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
#ifndef _restx_c_
#define _restx_c_

#include "restx.h"

/*	----------------------------------------------	*/
/*	l i b e r a t e _ r e s t _ e x t e n s i o n 	*/
/*	----------------------------------------------	*/
public struct rest_extension * liberate_rest_extension(struct rest_extension * sptr)
{
	if ( sptr )
	{
		sptr = liberate( sptr );
	}
	return((struct rest_extension *) 0);

}

/*	----------------------------------------	*/
/*	r e s e t _ r e s t _ e x t e n s i o n 	*/
/*	----------------------------------------	*/
public struct rest_extension * reset_rest_extension(struct rest_extension * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct rest_extension*) 0;
		sptr->next = (struct rest_extension*) 0;
		sptr->payload = (void*) 0;
		sptr->type =  0;
	}
	return(sptr);

}

/*	----------------------------------------------	*/
/*	a l l o c a t e _ r e s t _ e x t e n s i o n 	*/
/*	----------------------------------------------	*/
public struct rest_extension * allocate_rest_extension()
{
	struct rest_extension * sptr;
	if (!( sptr = allocate( sizeof( struct rest_extension ) ) ))
		return( sptr );
	else	return( reset_rest_extension(sptr) );
}

#endif	/* _restx_c_ */
