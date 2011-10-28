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
#ifndef _kindnode_c_
#define _kindnode_c_

#include "kindnode.h"

/*	----------------------------------------------	*/
/*	l i b e r a t e _ o c c i _ k i n d _ n o d e 	*/
/*	----------------------------------------------	*/
public struct occi_kind_node * liberate_occi_kind_node(struct occi_kind_node * sptr)
{
	if ( sptr )
	{
		if ( sptr->firstmixin )
			 sptr->firstmixin = liberate_occi_mixin(sptr->firstmixin);
		if ( sptr->lastmixin )
			 sptr->lastmixin = liberate_occi_mixin(sptr->lastmixin);
		if ( sptr->firstlink )
			 sptr->firstlink = liberate_occi_link(sptr->firstlink);
		if ( sptr->lastlink )
			 sptr->lastlink = liberate_occi_link(sptr->lastlink);
		sptr = liberate( sptr );
	}
	return((struct occi_kind_node *) 0);

}

/*	----------------------------------------	*/
/*	r e s e t _ o c c i _ k i n d _ n o d e 	*/
/*	----------------------------------------	*/
public struct occi_kind_node * reset_occi_kind_node(struct occi_kind_node * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct occi_kind_node*) 0;
		sptr->next = (struct occi_kind_node*) 0;
		sptr->contents = (void*) 0;
		sptr->firstmixin = (struct occi_mixin*) 0;
		sptr->lastmixin = (struct occi_mixin*) 0;
		sptr->firstlink = (struct occi_link*) 0;
		sptr->lastlink = (struct occi_link*) 0;
	}
	return(sptr);

}

/*	----------------------------------------------	*/
/*	a l l o c a t e _ o c c i _ k i n d _ n o d e 	*/
/*	----------------------------------------------	*/
public struct occi_kind_node * allocate_occi_kind_node()
{
	struct occi_kind_node * sptr;
	if (!( sptr = allocate( sizeof( struct occi_kind_node ) ) ))
		return( sptr );
	else	return( reset_occi_kind_node(sptr) );
}

#endif	/* _kindnode_c_ */
