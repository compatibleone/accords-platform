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
#ifndef _linknode_c_
#define _linknode_c_

#include "linknode.h"

/*	----------------------------------------------	*/
/*	l i b e r a t e _ o c c i _ l i n k _ n o d e 	*/
/*	----------------------------------------------	*/
public struct occi_link_node * liberate_occi_link_node(struct occi_link_node * sptr)
{
	if ( sptr )
	{
		if ( sptr->source )
			 sptr->source = liberate(sptr->source);
		if ( sptr->target )
			 sptr->target = liberate(sptr->target);
		sptr = liberate( sptr );
	}
	return((struct occi_link_node *) 0);

}

/*	----------------------------------------	*/
/*	r e s e t _ o c c i _ l i n k _ n o d e 	*/
/*	----------------------------------------	*/
public struct occi_link_node * reset_occi_link_node(struct occi_link_node * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct occi_link_node*) 0;
		sptr->next = (struct occi_link_node*) 0;
		sptr->contents = (void**) 0;
		sptr->source = (char*) 0;
		sptr->target = (char*) 0;
	}
	return(sptr);

}

/*	----------------------------------------------	*/
/*	a l l o c a t e _ o c c i _ l i n k _ n o d e 	*/
/*	----------------------------------------------	*/
public struct occi_link_node * allocate_occi_link_node()
{
	struct occi_link_node * sptr;
	if (!( sptr = allocate( sizeof( struct occi_link_node ) ) ))
		return( sptr );
	else	return( reset_occi_link_node(sptr) );
}

#endif	/* _linknode_c_ */
