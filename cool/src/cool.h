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

	int		isactive;
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
	char 	*	security;
	int		occi;
	int		port;
	int		floor;
	int		ceiling;
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
	char	*	parentservice;
	char	*	agreement;

	int		total;
	int		active;
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

