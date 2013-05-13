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
#ifndef	_allocate_c
#define	_allocate_c

#include "allocate.h"

int	indent=0;

/*	-----------------------------------	*/
/*		a l l o c a t e			*/
/*	-----------------------------------	*/
/*	centralised memory allocation.		*/
/*	-----------------------------------	*/
private	pthread_mutex_t allocation_control = PTHREAD_MUTEX_INITIALIZER;
public	void *	allocate(int n)
{
	void	*	vptr;
	pthread_mutex_lock( &allocation_control );
	if (( vptr = malloc(n)) != (void *) 0)
		memset(vptr,0,n);
	pthread_mutex_unlock( &allocation_control );
	return( vptr );		
}

/*	-----------------------------------	*/
/*		l i b e r a t e			*/
/*	-----------------------------------	*/
/*	centralised memory release.   		*/
/*	-----------------------------------	*/
public	void *	liberate( void * v)
{
	pthread_mutex_lock( &allocation_control );
	free(v);
	pthread_mutex_unlock( &allocation_control );
	return((void *) 0);
}


/*	-----------------------------------	*/
/*	  a l l o c a t e _ s t r i n g		*/
/*	-----------------------------------	*/
/*	centralised string allocation.		*/
/*	-----------------------------------	*/
public	char *	allocate_string( char *s )
{
	char *	r;
	if (!( s ))
		return( s );
	else if (!( r = allocate(strlen(s)+1) ))
		return(r);
	else	return(strcpy(r,s));
}

/*	-----------------------------------	*/
/*		j o i n _ s t r i n g		*/
/*	-----------------------------------	*/
/*	join left string with right string	*/
/*	releasing left string.			*/
/*	-----------------------------------	*/
public	char *	join_string( char * rptr, char * sptr )
{
	char *	wptr;
	if (!( sptr ))
		return( rptr );
	else if (!( rptr ))
		return( allocate_string( sptr ) );
	else if (!( wptr = allocate( strlen( rptr ) + strlen( sptr) + 1 ) ))
		return( wptr );
	else 
	{
		strcpy( wptr, rptr );
		strcat( wptr, sptr );
		liberate( rptr );
		return( wptr );
	}
}

/*	-----------------------------------	*/
/*	      j o i n _ s t r i n g s		*/
/*	-----------------------------------	*/
/*	join left string with right string	*/
/*	releasing both string.			*/
/*	-----------------------------------	*/
public	char *	join_strings( char * rptr, char * sptr )
{
	if (!( rptr = join_string( rptr, sptr ) ))
		return( rptr );
	else
	{
		liberate( sptr );
		return( rptr );
	}
}

#endif

