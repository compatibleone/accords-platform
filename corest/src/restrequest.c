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
#ifndef _restrequest_c_
#define _restrequest_c_

#include "restrequest.h"

/*	------------------------------------------	*/
/*	l i b e r a t e _ r e s t _ r e q u e s t 	*/
/*	------------------------------------------	*/
public struct rest_request * liberate_rest_request(struct rest_request * sptr)
{
	struct rest_header * mptr;
	if ( sptr )
	{
		while ((mptr = sptr->first) != (struct rest_header *) 0)
		{
			sptr->first = mptr->next;
			mptr = liberate_rest_header( mptr );
		}
		if ( sptr->method )
			 sptr->method = liberate(sptr->method);
		if ( sptr->object )
			 sptr->object = liberate(sptr->object);
		if ( sptr->parameters )
			 sptr->parameters = liberate(sptr->parameters);
		if ( sptr->version )
			 sptr->version = liberate(sptr->version);
		if ( sptr->body )
			 sptr->body = liberate(sptr->body);
		sptr = liberate( sptr );
	}
	return((struct rest_request *) 0);

}

/*	------------------------------------	*/
/*	r e s e t _ r e s t _ r e q u e s t 	*/
/*	------------------------------------	*/
public struct rest_request * reset_rest_request(struct rest_request * sptr)
{
	if ( sptr )
	{
		sptr->first = (struct rest_header*) 0;
		sptr->last = (struct rest_header*) 0;
		sptr->method = (char*) 0;
		sptr->object = (char*) 0;
		sptr->parameters = (char*) 0;
		sptr->version = (char*) 0;
		sptr->type =  0;
		sptr->body = (char*) 0;
	}
	return(sptr);

}

/*	------------------------------------------	*/
/*	a l l o c a t e _ r e s t _ r e q u e s t 	*/
/*	------------------------------------------	*/
public struct rest_request * allocate_rest_request()
{
	struct rest_request * sptr;
	if (!( sptr = allocate( sizeof( struct rest_request ) ) ))
		return( sptr );
	else	return( reset_rest_request(sptr) );
}

#endif	/* _restrequest_c_ */
