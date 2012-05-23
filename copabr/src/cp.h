/* ---------------------------------------------------------------------------- */
/* Advanced Capabilities for Compatible One Resources Delivery System - ACCORDS	*/
/* (C) 2011 by Iain James Marshall <ijm667@hotmail.com>				*/
/* ---------------------------------------------------------------------------- */
/*										*/
/* This is free software; you can redistribute it and/or modify it		*/
/* under the terms of the GNU Lesser General Public License as			*/
/* published by the Free Software Foundation; either version 2.1 of		*/
/* the License, or (at your option) any later version.				*/
/*										*/
/* This software is distributed in the hope that it will be useful,		*/
/* but WITHOUT ANY WARRANTY; without even the implied warranty of		*/
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU		*/
/* Lesser General Public License for more details.				*/
/*										*/
/* You should have received a copy of the GNU Lesser General Public		*/
/* License along with this software; if not, write to the Free			*/
/* Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA		*/
/* 02110-1301 USA, or see the FSF site: http://www.fsf.org.			*/
/*										*/
/* ---------------------------------------------------------------------------- */
#ifndef	_cp_h
#define	_cp_h

#include "standard.h"
#include "document.h"
#include "occiresolver.h"
#include "cordslang.h"

/*	--------------------------	*/
/*	public function prototypes
/*	--------------------------	*/

public	void	cords_document_xsd( struct xml_element * xsd );
public	struct	occi_response *	cords_select_category(struct xml_element * document, char * agent, char * tls);
public	struct	occi_response *	cords_retrieve_category(struct xml_element * document, char * id, char * agent, char * tls);
public	struct	occi_response * cords_resolve_category(struct xml_element * document, struct xml_atribut * kptr, char * agent, char * tls);
public	struct	occi_response * cords_create_category( struct xml_element * document, char * agent, char * tls);
public	struct	occi_response * cords_invoke_action( char * resource, char * action, char * agent, char * tls );
public	struct	occi_response * ll_cords_invoke_action( char * resource, char * action, char * agent, char * tls );
public	struct	occi_response * cords_retrieve_named_instance_list(char * category,char * name, char * value,char * agent, char * tls);
public	struct	occi_response * cords_retrieve_named_instance( struct occi_response * yptr, char * agent, char * tls );
public	struct	occi_response * cords_create_instance( 
char * category, char * agent, struct occi_element * eptr, char * tls );

public	int	cords_delete_instance( char * uri, char * category, char * agent, char * tls );
public	int	cords_resolve_location( struct occi_response * zptr, struct xml_element * document ); 
public	int	cords_resolve_last_location( struct occi_response * zptr, struct xml_element * document );
public	int 	cords_parse_element(
		char * domain,
		struct xml_element * xst, 
		struct xml_element * document, 
		char * agent, char * tls, int level);
public	int	cords_terminate_level( struct xml_element * dptr, char * agent, char * tls );

public	struct	xml_element  * 	cords_serialise_document( struct xml_element * document, char * result );
public	struct	xml_element  *  cords_drop_document( struct xml_element * document );
public	struct	xml_element  * 	cords_document_parser( 
		char * host, char * filename, char * agent, char * tls, int xsd );

public  char *	cords_extract_location( struct occi_response * rptr );
public	char *	cords_build_host( struct occi_response * zptr, char * id );

#endif	/* _cp_h */
	/* ----- */








