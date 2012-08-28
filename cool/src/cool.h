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
#ifndef	_cool_h	
#define	_cool_h

struct	elastic_contract
{
	struct	elastic_contract * previous;
	struct	elastic_contract * next;

	char *		contract;
	char *		hostname;
	char *		service;

	char *		profile;
	char *		provision;
	char *		account;
	char *		accountname;
	char *		node;
	char *		name;

	int		port;
	int		lastused;
	int		hitcount;
	int		allocated;
	int		usage;

	/* -------------------------------- */
	/* 3 temporary messages for control */
	/* of contract negotitation process */
	/* -------------------------------- */
	struct	occi_response * wptr;
	struct	occi_response * xptr;
	struct	occi_response * yptr;
	struct	occi_response * zptr;

};
	
struct	elastic_control 
{
	int		floor;
	int		ceiling;
	int		total;
	int		strategy;
	int		hitcount;
	int		maxrate;
	int		maxhit;
	int		lasthit;

	struct	elastic_contract * first;
	struct	elastic_contract * last;
	struct	elastic_contract * current;
};

	/* ------- */
#endif	/* _cool_h */
	/* ------- */

