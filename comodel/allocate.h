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

#ifndef	_allocate_h
#define	_allocate_h

#include "standard.h"
#include <memory.h>

public	void	*	allocate(int);
public	void	*	liberate(void *);
public	char 	*	allocate_string( char * );
public	char 	*	join_strings( char * rptr, char * sptr );
public	char 	*	join_string( char * rptr, char * sptr );
public  char    *       join_string_sep( char * rptr, char * sptr, char * sep );

	/* ----------- */
#endif	/* _allocate_h */
	/* ----------- */


