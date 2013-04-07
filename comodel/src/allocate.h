#ifndef	_allocate_h
#define	_allocate_h

#include "standard.h"
#include <memory.h>

public	void	*	allocate(int);
public	void	*	liberate(void *);
public	char 	*	allocate_string( char * );
public	char 	*	join_strings( char * rptr, char * sptr );
public	char 	*	join_string( char * rptr, char * sptr );

	/* ----------- */
#endif	/* _allocate_h */
	/* ----------- */


