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

