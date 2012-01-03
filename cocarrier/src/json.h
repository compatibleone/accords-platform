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

