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
#ifndef	_document_h
#define	_document_h

#include "standard.h"
#include "xmlparse.h"
#include "element.h"

public	struct	xml_element * document_parse_file( char * sptr );
public	struct	xml_element * document_drop( struct xml_element * document );
public	struct	xml_element * document_element( struct xml_element * document, char * token );
public	struct	xml_atribut * document_atribut( struct xml_element * document, char * token );
public	struct	xml_element * document_prefix_element( struct xml_element * dptr, char * nptr );
public	struct	xml_element * document_add_element( struct xml_element * dptr, char * nptr );
public	struct	xml_atribut * document_add_atribut( struct xml_element * eptr, char * nptr, char * vptr );
public	char *	document_atribut_string( struct xml_atribut * aptr );
public	int	document_atribut_value( struct xml_atribut * aptr );
public	int	document_serialise_file( struct xml_element * eptr, char * filename );


#endif	/* _document_h */
	/* ----------- */




