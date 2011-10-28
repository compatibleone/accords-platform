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
#ifndef	_cordsscript_parser_h
#define	_cordscript_parser_h

#include "global.h"
#include "cb.h"
#include "cse.h"
#include "csa.h"

#define	_MAX_CORDSCRIPT_TOKEN	1024

#define	_CORDSCRIPT_LITERAL	0
#define	_CORDSCRIPT_PROPERTY	1
#define	_CORDSCRIPT_METHOD	2

#define	_CORDSCRIPT_AFFECT	0
#define	_CORDSCRIPT_CONFIGURE	1

public	struct	cordscript_action *	cordscript_parse_statement( char * statement );

#endif	/* _cords_script_parser_h */
	/* ---------------------- */

