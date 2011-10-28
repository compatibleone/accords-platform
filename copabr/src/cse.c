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
#ifndef _cse_c_
#define _cse_c_

#include "cse.h"

/*	------------------------------------------------------	*/
/*	l i b e r a t e _ c o r d s c r i p t _ e l e m e n t 	*/
/*	------------------------------------------------------	*/
public struct cordscript_element * liberate_cordscript_element(struct cordscript_element * sptr)
{
	if ( sptr )
	{
		if ( sptr->prefix )
			 sptr->prefix = liberate(sptr->prefix);
		if ( sptr->value )
			 sptr->value = liberate(sptr->value);
		sptr = liberate( sptr );
	}
	return((struct cordscript_element *) 0);

}

/*	------------------------------------------------	*/
/*	r e s e t _ c o r d s c r i p t _ e l e m e n t 	*/
/*	------------------------------------------------	*/
public struct cordscript_element * reset_cordscript_element(struct cordscript_element * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct cordscript_element*) 0;
		sptr->next = (struct cordscript_element*) 0;
		sptr->type =  0;
		sptr->prefix = (char*) 0;
		sptr->value = (char*) 0;
	}
	return(sptr);

}

/*	------------------------------------------------------	*/
/*	a l l o c a t e _ c o r d s c r i p t _ e l e m e n t 	*/
/*	------------------------------------------------------	*/
public struct cordscript_element * allocate_cordscript_element()
{
	struct cordscript_element * sptr;
	if (!( sptr = allocate( sizeof( struct cordscript_element ) ) ))
		return( sptr );
	else	return( reset_cordscript_element(sptr) );
}

#endif	/* _cse_c_ */
