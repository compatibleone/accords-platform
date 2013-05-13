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
#ifndef	_cordsscript_parser_h
#define	_cordscript_parser_h

#include "global.h"
#include "cb.h"
#include "cse.h"
#include "csa.h"

#define	_CORDSCRIPT_PREFIX	"cordscript:"

#define	_MAX_CORDSCRIPT_TOKEN	1024

#define	_CORDSCRIPT_LITERAL	0
#define	_CORDSCRIPT_PROPERTY	1
#define	_CORDSCRIPT_METHOD	2

#define	_CORDSCRIPT_AFFECT	0
#define	_CORDSCRIPT_CONFIGURE	1
#define	_CORDSCRIPT_MONITOR	2
#define	_CORDSCRIPT_SYSTEM 	3
#define	_CORDSCRIPT_FORK   	4
#define	_CORDSCRIPT_KILL   	5
#define	_CORDSCRIPT_NEW   	6
#define	_CORDSCRIPT_DELETE     	7
#define	_CORDSCRIPT_START   	8
#define	_CORDSCRIPT_STOP   	9
#define	_CORDSCRIPT_SAVE   	10
#define	_CORDSCRIPT_SNAPSHOT  	11
#define	_CORDSCRIPT_BUILD   	12
#define	_CORDSCRIPT_INSTANCE   	13
#define	_CORDSCRIPT_RESOLVE   	14
#define	_CORDSCRIPT_INVOKE   	15

public	struct	cordscript_action *	cordscript_parse_statement( char * statement );
public	int				cordscript_parse_method( char * token );
public	char *				cordscript_method( int	symbol );
public 	struct cordscript_action * 	liberate_cordscript_actions(struct cordscript_action * sptr);

#endif	/* _cords_script_parser_h */
	/* ---------------------- */



