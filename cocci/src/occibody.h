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

#ifndef	_occi_body_h
#define	_occi_body_h

#include "standard.h"
#include "rest.h"
#include "occi.h"
#include "restclient.h"
#include "url.h"
#include "urlpublic.h"

public	char *	occi_html_capacities( struct occi_category * cptr, struct rest_header  * hptr );
public	char *	occi_response_body( char * accepts, struct occi_category * cptr, struct rest_header * hptr );
public	int	accept_string_includes( char * sptr, char * tptr );

	/* ------------ */
#endif	/* _occi_body_h */
	/* ------------ */























