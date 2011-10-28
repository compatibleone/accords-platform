/* ------------------------------------------------------------------------------------	*/
/*				 CompatibleOne Cloudware				*/
/* ------------------------------------------------------------------------------------ */
/*											*/
/* Ce fichier fait partie de ce(tte) oeuvre de Iain James Marshall et est mise a 	*/
/* disposition selon les termes de la licence Creative Commons Paternit‚ : 		*/
/*											*/
/*			 	Pas d'Utilisation Commerciale 				*/
/*				Pas de Modification 					*/
/*				3.0 non transcrit.					*/
/*											*/
/* ------------------------------------------------------------------------------------ */
/* 			Copyright (c) 2011 Iain James Marshall for Prologue 		*/
/*				   All rights reserved					*/
/* ------------------------------------------------------------------------------------ */
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

