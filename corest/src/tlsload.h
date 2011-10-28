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
#ifndef	_tls_load_h
#define	_tls_load_h

#include "tlsconfig.h"

public	struct tls_configuration * tls_configuration_load(char * filename );
public	void			   tls_configuration_use( struct tls_configuration * cptr );
public 	struct tls_configuration * liberate_tls_configuration(struct tls_configuration * sptr);
public	struct tls_configuration * release_tls_configuration(struct tls_configuration * cptr);
public 	struct tls_configuration * allocate_tls_configuration();

#endif


