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

	char *		agreement;
	char *		parentservice;
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
	int		startduration;
	int		stopduration;

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
	/* ------------------------------------ */
	/* will be provided through environment	*/
	/* ------------------------------------ */
	int		floor;
	int		ceiling;
	int		total;
	int		strategy;
	int		upper;
	int		lower;
	int		unit;
	int		period;

	/* ------------------------------------ */
	/* will be calculated during operation  */
	/* ------------------------------------ */
	int		units;
	int		firstunit;
	int		lastunit;

	/* ------------------------------------ */
	/* to be retrieved from first contract  */
	/* ------------------------------------ */
	char	*	contract;
	char 	*	contractname;
	char 	*	elasticname;
	char	*	parentservice;
	char	*	agreement;
	int		total_start_duration;
	int		total_stop_duration;
	int		average_start_duration;
	int		average_stop_duration;

	/* ------------------------------------ */
	/* operational load balancing variables */
	/* ------------------------------------ */
	int		hitcount;
	int		maxhit;
	int		lasthit;

	struct	elastic_contract * first;
	struct	elastic_contract * last;
	struct	elastic_contract * current;

	int	*	unitdata;

};

	/* ------- */
#endif	/* _cool_h */
	/* ------- */

