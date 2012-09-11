/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>     */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License"); 	*/
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at 				*/
/*  									*/
/*  http://www.apache.org/licenses/LICENSE-2.0 				*/
/*  									*/
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 	*/
/* implied. 								*/
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License. 					*/
/* -------------------------------------------------------------------- */
#ifndef	_document_h
#define	_document_h

#include "standard.h"
#include "xmlparse.h"
#include "element.h"

public	struct	xml_element * document_parse_file( char * sptr );
public	struct	xml_element * document_drop( struct xml_element * document );
public	struct	xml_element * document_element( struct xml_element * document, char * token );
public	struct	xml_element * nested_document_element( struct xml_element * document, char * token );
public	struct	xml_atribut * document_atribut( struct xml_element * document, char * token );
public	struct	xml_element * document_prefix_element( struct xml_element * dptr, char * nptr );
public	struct	xml_element * document_add_element( struct xml_element * dptr, char * nptr );
public	struct	xml_atribut * document_add_atribut( struct xml_element * eptr, char * nptr, char * vptr );
public	char *	document_atribut_string( struct xml_atribut * aptr );
public	int	document_atribut_value( struct xml_atribut * aptr );
public	int	document_serialise_file( struct xml_element * eptr, char * filename );
public	int	document_remove_element( struct xml_element * eptr );
public	int	document_append_element( struct xml_element * dptr, struct xml_element * eptr );
public	int	document_append_elements( struct xml_element * dptr, struct xml_element * eptr );



#endif	/* _document_h */
	/* ----------- */




