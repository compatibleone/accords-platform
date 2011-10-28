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
#ifndef _tlsconfig_c_
#define _tlsconfig_c_

#include "tlsconfig.h"

/*	----------------------------------------------------	*/
/*	l i b e r a t e _ t l s _ c o n f i g u r a t i o n 	*/
/*	----------------------------------------------------	*/
public struct tls_configuration * liberate_tls_configuration(struct tls_configuration * sptr)
{
	if ( sptr )
	{
		if ( sptr->filename )
			 sptr->filename = liberate(sptr->filename);
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->key )
			 sptr->key = liberate(sptr->key);
		if ( sptr->certificate )
			 sptr->certificate = liberate(sptr->certificate);
		if ( sptr->passphrase )
			 sptr->passphrase = liberate(sptr->passphrase);
		sptr = liberate( sptr );
	}
	return((struct tls_configuration *) 0);

}

/*	----------------------------------------------	*/
/*	r e s e t _ t l s _ c o n f i g u r a t i o n 	*/
/*	----------------------------------------------	*/
public struct tls_configuration * reset_tls_configuration(struct tls_configuration * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct tls_configuration*) 0;
		sptr->next = (struct tls_configuration*) 0;
		sptr->filename = (char*) 0;
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->key = (char*) 0;
		sptr->certificate = (char*) 0;
		sptr->passphrase = (char*) 0;
		sptr->option =  0;
		sptr->authenticate =  0;
	}
	return(sptr);

}

/*	----------------------------------------------------	*/
/*	a l l o c a t e _ t l s _ c o n f i g u r a t i o n 	*/
/*	----------------------------------------------------	*/
public struct tls_configuration * allocate_tls_configuration()
{
	struct tls_configuration * sptr;
	if (!( sptr = allocate( sizeof( struct tls_configuration ) ) ))
		return( sptr );
	else	return( reset_tls_configuration(sptr) );
}

#endif	/* _tlsconfig_c_ */
