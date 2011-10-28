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
#ifndef _occic_c_
#define _occic_c_

#include "occic.h"

/*	----------------------------------------	*/
/*	l i b e r a t e _ o c c i _ c l i e n t 	*/
/*	----------------------------------------	*/
public struct occi_client * liberate_occi_client(struct occi_client * sptr)
{
	if ( sptr )
	{
		if ( sptr->host )
			 sptr->host = liberate(sptr->host);
		if ( sptr->agent )
			 sptr->agent = liberate(sptr->agent);
		if ( sptr->tls )
			 sptr->tls = liberate(sptr->tls);
		if ( sptr->mimetype )
			 sptr->mimetype = liberate(sptr->mimetype);
		sptr = liberate( sptr );
	}
	return((struct occi_client *) 0);

}

/*	----------------------------------	*/
/*	r e s e t _ o c c i _ c l i e n t 	*/
/*	----------------------------------	*/
public struct occi_client * reset_occi_client(struct occi_client * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct occi_client*) 0;
		sptr->next = (struct occi_client*) 0;
		sptr->parent = (struct occi_manager*) 0;
		sptr->host = (char*) 0;
		sptr->agent = (char*) 0;
		sptr->tls = (char*) 0;
		sptr->mimetype = (char*) 0;
		sptr->categories =  0;
		sptr->started =  0;
		sptr->requests =  0;
		sptr->failures =  0;
	}
	return(sptr);

}

/*	----------------------------------------	*/
/*	a l l o c a t e _ o c c i _ c l i e n t 	*/
/*	----------------------------------------	*/
public struct occi_client * allocate_occi_client()
{
	struct occi_client * sptr;
	if (!( sptr = allocate( sizeof( struct occi_client ) ) ))
		return( sptr );
	else	return( reset_occi_client(sptr) );
}

/*	------------------------------	*/
/*	a d d _ o c c i _ c l i e n t 	*/
/*	------------------------------	*/
public struct occi_client * add_occi_client(struct occi_manager * pptr)
{
	struct occi_client * sptr;
	if (!( sptr = allocate( sizeof( struct occi_client ) ) ))
		return( sptr );
	else if (!( sptr = reset_occi_client(sptr) ))
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
/*	d r o p _ o c c i _ c l i e n t 	*/
/*	--------------------------------	*/
public struct occi_client * drop_occi_client(struct occi_client * sptr)
{
	if ( sptr )
	{
		if (!( sptr->parent )) return(sptr);
		if (!( sptr->previous ))
		{
			if (!( sptr->parent->first = sptr->next ))
				sptr->parent->last = (struct occi_client *) 0;
			else	sptr->parent->first->previous = (struct occi_client *) 0;
		}
		else if (!( sptr->previous->next = sptr->next ))
			sptr->parent->last = sptr->previous;
		if (!( sptr->next ))
		{
			if (!( sptr->parent->last = sptr->previous ))
				sptr->parent->first = (struct occi_client *) 0;
			else	sptr->parent->last->next = (struct occi_client *) 0;
		}
		else if (!( sptr->next->previous = sptr->previous ))
			sptr->parent->first = sptr->next;
		sptr = liberate_occi_client(sptr);
	}
	return((struct occi_client *) 0);
}

#endif	/* _occic_c_ */
