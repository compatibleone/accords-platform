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

