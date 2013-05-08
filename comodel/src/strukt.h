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

#ifndef	_strukt_h
#define	_strukt_h

#include <stdio.h>
#include <memory.h>
#include <string.h>
#define	public
#define	private static
#include "allocate.h"
#include "item.h"

#define	_OCCI_KIND	1
#define	_OCCI_LINK	2
#define	_STRUKT_OCCI_ACTION	3
#define	_STRUKT_OCCI_MIXIN	4

struct	strukt_context	{
	int	schema;
	char *	license;
	char *	scheme;
	char *	klass;
	char *	rel;
	char *	prefix;
	int	gencrud;
	int	genrest;
	int	genocci;
	int	genxml;
	int	gensql;
	int	echo;

	int	verbose;
	int	next;
	int	state;
	int	ungotc;

	int	isptr;	
	int	dimension;
	int	hasfirst;
	int	hasparent;

	int	hasprevious;
	int	hasnext;

	FILE *	target;
	char *	basic;
	struct 	item * first;
	struct 	item * last;
	
	char	name[256];
	char	member[256];
	char	special[256];
	char 	parent[256];
	char 	premier[256];
	};

public	void	title( FILE * h, char * sptr );
public	int	generate();
public	int	add_member();
public	int	process( char * nptr );
public	int	schema( char * nptr );
public	void	reset_strukt();

	/* --------- */
#endif	/* _strukt_h */
	/* --------- */










