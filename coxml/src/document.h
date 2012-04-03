/* ------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                   */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>    */
/* --------------------------------------------------------------------*/
/*  This is free software; you can redistribute it and/or modify it    */
/*  under the terms of the GNU Lesser General Public License as        */
/*  published by the Free Software Foundation; either version 2.1 of   */
/*  the License, or (at your option) any later version.                */
/*                                                                     */
/*  This software is distributed in the hope that it will be useful,   */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of     */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   */
/*  Lesser General Public License for more details.                    */
/*                                                                     */
/*  You should have received a copy of the GNU Lesser General Public   */
/*  License along with this software; if not, write to the Free        */
/*  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA */
/*  02110-1301 USA, or see the FSF site: http://www.fsf.org.           */
/* --------------------------------------------------------------------*/
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
public	int	document_remove_element( struct xml_element * eptr );
public	int	document_append_element( struct xml_element * dptr, struct xml_element * eptr );
public	int	document_append_elements( struct xml_element * dptr, struct xml_element * eptr );



#endif	/* _document_h */
	/* ----------- */




