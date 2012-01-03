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
#ifndef	_restworkers_c
#define	_restworkers_c

#include <sys/types.h>
#include <sys/wait.h>

#include "restworker.c"

/*	------------------------------------------------	*/
/*	    	r e s t _ n e w _ w o r k e r 			*/
/*	------------------------------------------------	*/
/*	creates a manager child worker for any eventual		*/
/*	usage. The child list will be control purged		*/
/*	from time to time to release zombie processes.		*/
/*	------------------------------------------------	*/
int	useworkers=0;
struct rest_worker *	rest_new_worker(struct rest_server * sptr)
{
	struct	rest_worker * wptr;
	if (!( useworkers ))
		return((struct rest_worker *) 0);
	if (!( wptr = add_rest_worker( sptr ) ))
		return( wptr );
	else
	{
		switch ((wptr->pid = fork()))
		{
			case	0	:
				return( wptr );
			case	-1	:
				return( drop_rest_worker( wptr ) );
			default		:
				return((struct rest_worker *) 0);
		}
	}
}

/*	------------------------------------------------	*/
/*	    r e s t _ l i b e r a t e _ w o r k e r s		*/
/*	------------------------------------------------	*/
struct rest_worker *	rest_liberate_workers(struct rest_worker * xptr)
{
	struct	rest_worker * wptr;
	while ((wptr = xptr) != (struct rest_worker *) 0)
	{
		xptr = xptr->next;
		liberate_rest_worker( wptr );
	}
	return( wptr );
}

/*	------------------------------------------------	*/
/*	    r e s t _ r e l e a	s e _ z o m b i e s  		*/
/*	------------------------------------------------	*/
int 	rest_release_zombies( struct rest_server * sptr )
{
	struct	rest_worker * wptr;
	struct	rest_worker * xptr;
	if (!( sptr ))
		return(0);
	else if (!( wptr = sptr->firstw ))
		return(0);
	else
	{
		while ( wptr )
		{
			if ( wptr->pid > 0 )
			{
				if ( waitpid( wptr->pid, &wptr->status, WNOHANG ) == wptr->pid )
				{
					xptr = wptr->next;
					drop_rest_worker( wptr );
					wptr = xptr;
				}
				else	wptr = wptr->next;
			}
			else
			{
				xptr = wptr->next;
				drop_rest_worker( wptr );
				wptr = xptr;
			}
		}
		return(0);
	}
}


#endif /* _restworkers_c */

