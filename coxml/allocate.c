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

//#define mem_debug

#include "allocate.h"

int	indent=0;

private	int		areport=0;
private	unsigned long	acounter=0;
private	unsigned long	lcounter=0;

/*	-----------------------------------	*/
/*	  a l l o c a t i o n _ t r a c e	*/
/*	-----------------------------------	*/
public	void	allocation_trace( int mode )
{
	areport = mode;
	return;
}

private	pthread_mutex_t allocation_control = PTHREAD_MUTEX_INITIALIZER;

struct mem_header {
    int secure;
    size_t size;
    char data[];
};

#define mem_hdr(v) ((struct mem_header *)(v - sizeof(struct mem_header)))

#ifdef mem_debug
private int alloc_size = 0;
private int salloc_size = 0;
private int alloc_count = 0;
private int salloc_count = 0;

#define addref(v) { if(v->secure) { alloc_count++; alloc_size += v->size; } else { salloc_count++; salloc_size += v->size; } }
#define subref(v) { if(v->secure) { alloc_count--; alloc_size -= v->size; } else { salloc_count--; salloc_size -= v->size; } }
#define printmem(s,v) { printf("MEM_DEBUG: %s %p %p %d %6lu [%3d %6d %3d %6d]\n", s, v, v->data, v->secure, v->size, alloc_count, alloc_size, salloc_count, salloc_size); }
#else
#define addref(v)
#define subref(v)
#define printmem(s,v)
#endif

/*	-----------------------------------	*/
/*		a l l o c a t e	_ s e c u r e	*/
/*	-----------------------------------	*/
/*	centralised secure memory allocation.	*/
/*	-----------------------------------	*/
public	void *	allocate_secure(int n)
{
	struct mem_header	*	vptr;
	pthread_mutex_lock( &allocation_control );
	if (( vptr = malloc(sizeof(struct mem_header)+n)) != (void *) 0) {
        vptr->secure = 1;
        vptr->size = n;
        /* initialize data */
		memset(vptr->data,0,n);
        /* disallow memory swapping */
        int res = mlock(vptr, sizeof(struct mem_header)+n);
        if(res < 0) {
            perror("Warning: Could not lock secure memory");
        }
		addref(vptr);
		printmem("allocate", vptr);
    }
	pthread_mutex_unlock( &allocation_control );
	return( (void *)vptr->data );		
}

/*	-----------------------------------	*/
/*		l i b e r a t e	_ s e c u r e   */
/*	-----------------------------------	*/
/*	centralised secure memory release.  */
/*	-----------------------------------	*/
public	void *	liberate_secure( void * v)
{
	if( v) {
		struct mem_header	*	vptr = mem_hdr(v);
		pthread_mutex_lock( &allocation_control );
		subref(vptr);
		printmem("liberate", vptr);
    	if(vptr->secure) {
    	    int n = sizeof(struct mem_header) + vptr->size;
    	    /* burn data */
    	    memset(vptr, 0, n);
    	    /* allow memory swapping */
    	    munlock(vptr, n);
    	}
		lcounter++;
		if ( areport )
			printf("%lu liberate %u %lu\n",lcounter,0,v);
		free(vptr);
		pthread_mutex_unlock( &allocation_control );
	}
	return((void *) 0);
}


/*	-----------------------------------	*/
/*		a l l o c a t e			*/
/*	-----------------------------------	*/
/*	centralised memory allocation.		*/
/*	-----------------------------------	*/
public	void *	allocate(int n)
{
	struct mem_header	*	vptr;
	pthread_mutex_lock( &allocation_control );
	if (( vptr = malloc(sizeof(struct mem_header)+n)) != (void *) 0) {
		acounter++;
        vptr->secure = 0;
        vptr->size = n;
		memset(vptr->data,0,n);
		if ( areport )
			printf("%lu allocate %u %lu\n",acounter,n,vptr);
		addref(vptr);
		printmem("allocate", vptr);
    }
	pthread_mutex_unlock( &allocation_control );
	return( (void *)vptr->data );		
}

/*	-----------------------------------	*/
/*		l i b e r a t e			*/
/*	-----------------------------------	*/
/*	centralised memory release.   		*/
/*	-----------------------------------	*/
public	void *	liberate( void * v)
{
    return liberate_secure(v);
}


/*	-----------------------------------	*/
/*	  a l l o c a t e _ s t r i n g _ s e c u r e  */
/*	-----------------------------------	*/
/*	centralised string allocation.		*/
/*	-----------------------------------	*/
public	char *	allocate_string_secure( char *s )
{
	char *	r;
	if (!( s ))
		return( s );
	else if (!( r = allocate_secure(strlen(s)+1) ))
		return(r);
	else	return(strcpy(r,s));
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

/*	-----------------------------------------------------------------------    */
/*	      j o i n _ s t r i n g _ s e p                                        */
/*	-----------------------------------------------------------------------    */
/*	join left string with right string adding sep in between if left           */
/*	is not empty. Left string is released.                                     */
/*	-----------------------------------------------------------------------    */
public	char *	join_string_sep( char * rptr, char * sptr, char * sep )
{
  char * wptr;

  if (! (rptr) )
    return ( allocate_string(sptr) );
  else if (! (wptr = join_string(rptr, sep)))
    return wptr;
  else
    return ( join_string(wptr, sptr) );
}

#endif

