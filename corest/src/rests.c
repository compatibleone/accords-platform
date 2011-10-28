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
#ifndef _rests_c_
#define _rests_c_

#include "rests.h"

/*	----------------------------------------	*/
/*	l i b e r a t e _ r e s t _ s e r v e r 	*/
/*	----------------------------------------	*/
public struct rest_server * liberate_rest_server(struct rest_server * sptr)
{
	struct rest_client * mptr;
	if ( sptr )
	{
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->user )
			 sptr->user = liberate(sptr->user);
		if ( sptr->password )
			 sptr->password = liberate(sptr->password);
		if ( sptr->headers )
			 sptr->headers = liberate_rest_header(sptr->headers);
		while ((mptr = sptr->first) != (struct rest_client *) 0)
		{
			sptr->first = mptr->next;
			mptr = liberate_rest_client( mptr );
		}
		if ( sptr->active )
			 sptr->active = liberate_rest_client(sptr->active);
		sptr = liberate( sptr );
	}
	return((struct rest_server *) 0);

}

/*	----------------------------------	*/
/*	r e s e t _ r e s t _ s e r v e r 	*/
/*	----------------------------------	*/
public struct rest_server * reset_rest_server(struct rest_server * sptr)
{
	if ( sptr )
	{
		sptr->port =  0;
		sptr->name = (char*) 0;
		sptr->user = (char*) 0;
		sptr->password = (char*) 0;
		sptr->seconds =  0;
		sptr->wakeup =  0;
		sptr->fraction =  0;
		sptr->workers =  0;
		sptr->threads =  0;
		sptr->headers = (struct rest_header*) 0;
		sptr->first = (struct rest_client*) 0;
		sptr->last = (struct rest_client*) 0;
		sptr->active = (struct rest_client*) 0;
	}
	return(sptr);

}

/*	----------------------------------------	*/
/*	a l l o c a t e _ r e s t _ s e r v e r 	*/
/*	----------------------------------------	*/
public struct rest_server * allocate_rest_server()
{
	struct rest_server * sptr;
	if (!( sptr = allocate( sizeof( struct rest_server ) ) ))
		return( sptr );
	else	return( reset_rest_server(sptr) );
}

#endif	/* _rests_c_ */
