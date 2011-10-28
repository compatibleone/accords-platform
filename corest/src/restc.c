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
#ifndef _restc_c_
#define _restc_c_

#include "restc.h"

/*	----------------------------------------	*/
/*	l i b e r a t e _ r e s t _ c l i e n t 	*/
/*	----------------------------------------	*/
public struct rest_client * liberate_rest_client(struct rest_client * sptr)
{
	if ( sptr )
	{
		if ( sptr->agent )
			 sptr->agent = liberate(sptr->agent);
		if ( sptr->buffer )
			 sptr->buffer = liberate(sptr->buffer);
		if ( sptr->user )
			 sptr->user = liberate(sptr->user);
		if ( sptr->pass )
			 sptr->pass = liberate(sptr->pass);
		if ( sptr->headers )
			 sptr->headers = liberate_rest_header(sptr->headers);
		sptr = liberate( sptr );
	}
	return((struct rest_client *) 0);

}

/*	----------------------------------	*/
/*	r e s e t _ r e s t _ c l i e n t 	*/
/*	----------------------------------	*/
public struct rest_client * reset_rest_client(struct rest_client * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct rest_client*) 0;
		sptr->next = (struct rest_client*) 0;
		sptr->parent = (struct rest_server*) 0;
		sptr->agent = (char*) 0;
		sptr->buffer = (char*) 0;
		sptr->buffersize =  0;
		sptr->bytes =  0;
		sptr->consumed =  0;
		sptr->user = (char*) 0;
		sptr->pass = (char*) 0;
		sptr->headers = (struct rest_header*) 0;
	}
	return(sptr);

}

/*	----------------------------------------	*/
/*	a l l o c a t e _ r e s t _ c l i e n t 	*/
/*	----------------------------------------	*/
public struct rest_client * allocate_rest_client()
{
	struct rest_client * sptr;
	if (!( sptr = allocate( sizeof( struct rest_client ) ) ))
		return( sptr );
	else	return( reset_rest_client(sptr) );
}

/*	------------------------------	*/
/*	a d d _ r e s t _ c l i e n t 	*/
/*	------------------------------	*/
public struct rest_client * add_rest_client(struct rest_server * pptr)
{
	struct rest_client * sptr;
	if (!( sptr = allocate( sizeof( struct rest_client ) ) ))
		return( sptr );
	else if (!( sptr = reset_rest_client(sptr) ))
		return( sptr );
	else
	{
		if (!( sptr->previous = pptr->last ))
			pptr->first = sptr;
		else	sptr->previous->next = sptr;
		pptr->last = sptr;
		sptr->parent = pptr;
		return( sptr );
	}

}

/*	--------------------------------	*/
/*	d r o p _ r e s t _ c l i e n t 	*/
/*	--------------------------------	*/
public struct rest_client * drop_rest_client(struct rest_client * sptr)
{
	if ( sptr )
	{
		if (!( sptr->parent )) return(sptr);
		if (!( sptr->previous ))
		{
			if (!( sptr->parent->first = sptr->next ))
				sptr->parent->last = (struct rest_client *) 0;
			else	sptr->parent->first->previous = (struct rest_client *) 0;
		}
		else if (!( sptr->previous->next = sptr->next ))
			sptr->parent->last = sptr->previous;
		if (!( sptr->next ))
		{
			if (!( sptr->parent->last = sptr->previous ))
				sptr->parent->first = (struct rest_client *) 0;
			else	sptr->parent->last->next = (struct rest_client *) 0;
		}
		else if (!( sptr->next->previous = sptr->previous ))
			sptr->parent->first = sptr->next;
		sptr = liberate_rest_client(sptr);
	}
	return((struct rest_client *) 0);
}

#endif	/* _restc_c_ */
