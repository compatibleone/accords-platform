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

#ifndef	_json_h
#define	_json_h

#include "standard.h"
#include "allocate.h"
#include "dataelement.h"

public	int			json_show( struct data_element * dptr );
public	int			json_serialise( struct data_element * dptr, char * filename );
public	struct data_element *	json_parse_file( char * filename );
public	int			json_set_echo( int v );
public	struct data_element * 	drop_data_element( struct data_element * dptr );
public	char *			json_atribut( struct data_element * dptr, char * nptr );
public	struct data_element * 	json_element( struct data_element * dptr, char * nptr );

	/* ------- */
#endif	/* _json_h */
	/* ------- */

