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

#ifndef	_strukt_h
#define	_strukt_h

#include "allocate.h"

#define	_OCCI_KIND	1
#define	_OCCI_LINK	2
#define	_OCCI_ACTION	3
#define	_OCCI_MIXIN	4

struct	strukt_context	{
	int	schema;
	char *	license;
	char *	scheme;
	char *	klass;
	char *	rel;
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

#endif	







