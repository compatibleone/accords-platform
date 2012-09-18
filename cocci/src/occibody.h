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

#ifndef	_occi_body_h
#define	_occi_body_h

#include "standard.h"
#include "rest.h"
#include "occi.h"
#include "restclient.h"
#include "url.h"
#include "urlpublic.h"

public	char *	occi_html_capacities( struct occi_category * cptr, struct rest_response  * aptr );
public	char *	occi_text_capacities( struct occi_category * cptr, struct rest_response  * aptr );
public	char *	occi_response_body( char * accepts, struct occi_category * cptr, struct rest_header * hptr );
public	int	accept_string_includes( char * sptr, char * tptr );

	/* ------------ */
#endif	/* _occi_body_h */
	/* ------------ */























