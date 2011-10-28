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
#ifndef _url_c_
#define _url_c_

#include "url.h"

/*	------------------------	*/
/*	l i b e r a t e _ u r l 	*/
/*	------------------------	*/
public struct url * liberate_url(struct url * sptr)
{
	if ( sptr )
	{
		if ( sptr->service )
			 sptr->service = liberate(sptr->service);
		if ( sptr->host )
			 sptr->host = liberate(sptr->host);
		if ( sptr->object )
			 sptr->object = liberate(sptr->object);
		if ( sptr->tls )
			 sptr->tls = liberate(sptr->tls);
		sptr = liberate( sptr );
	}
	return((struct url *) 0);

}

/*	------------------	*/
/*	r e s e t _ u r l 	*/
/*	------------------	*/
public struct url * reset_url(struct url * sptr)
{
	if ( sptr )
	{
		sptr->service = (char*) 0;
		sptr->host = (char*) 0;
		sptr->port =  0;
		sptr->object = (char*) 0;
		sptr->tls = (char*) 0;
	}
	return(sptr);

}

/*	------------------------	*/
/*	a l l o c a t e _ u r l 	*/
/*	------------------------	*/
public struct url * allocate_url()
{
	struct url * sptr;
	if (!( sptr = allocate( sizeof( struct url ) ) ))
		return( sptr );
	else	return( reset_url(sptr) );
}

#endif	/* _url_c_ */
