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
#define	_CORDSCRIPT_MONITOR	2

public	struct	cordscript_action *	cordscript_parse_statement( char * statement );
public	int				cordscript_parse_method( char * token );
public	char *				cordscript_method( int	symbol );

#endif	/* _cords_script_parser_h */
	/* ---------------------- */

