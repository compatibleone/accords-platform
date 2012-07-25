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
#ifndef	_relax_h	
#define	_relax_h

struct	elastic_contract
{
	struct	elastic_contract * previous;
	struct	elastic_contract * next;

	char *	contract;
	char *	node;
	char *	hostname;
	char *	service;
	char *	provider;

	int		port;
	int		lastused;
	int		hitcount;
	int		allocated;
	int		usage;
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

	/* -------- */
#endif	/* _relax_h */
	/* -------- */

