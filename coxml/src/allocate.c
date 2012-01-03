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
		return( sptr );
	else if (!( wptr = allocate( strlen( rptr ) + strlen( sptr ) ) ))
		return( wptr );
	else 
	{
		strcat( strcpy( wptr, rptr ), sptr );
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

