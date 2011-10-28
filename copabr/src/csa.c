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
#ifndef _csa_c_
#define _csa_c_

#include "csa.h"

/*	----------------------------------------------------	*/
/*	l i b e r a t e _ c o r d s c r i p t _ a c t i o n 	*/
/*	----------------------------------------------------	*/
public struct cordscript_action * liberate_cordscript_action(struct cordscript_action * sptr)
{
	if ( sptr )
	{
		if ( sptr->lvalue )
			 sptr->lvalue = liberate_cordscript_element(sptr->lvalue);
		if ( sptr->rvalue )
			 sptr->rvalue = liberate_cordscript_element(sptr->rvalue);
		sptr = liberate( sptr );
	}
	return((struct cordscript_action *) 0);

}

/*	----------------------------------------------	*/
/*	r e s e t _ c o r d s c r i p t _ a c t i o n 	*/
/*	----------------------------------------------	*/
public struct cordscript_action * reset_cordscript_action(struct cordscript_action * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct cordscript_action*) 0;
		sptr->next = (struct cordscript_action*) 0;
		sptr->type =  0;
		sptr->lvalue = (struct cordscript_element*) 0;
		sptr->rvalue = (struct cordscript_element*) 0;
		sptr->status =  0;
	}
	return(sptr);

}

/*	----------------------------------------------------	*/
/*	a l l o c a t e _ c o r d s c r i p t _ a c t i o n 	*/
/*	----------------------------------------------------	*/
public struct cordscript_action * allocate_cordscript_action()
{
	struct cordscript_action * sptr;
	if (!( sptr = allocate( sizeof( struct cordscript_action ) ) ))
		return( sptr );
	else	return( reset_cordscript_action(sptr) );
}

#endif	/* _csa_c_ */
