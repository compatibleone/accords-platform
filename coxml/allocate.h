#ifndef	_allocate_h
#define	_allocate_h

#include "standard.h"
#include <memory.h>

public	void	*	allocate_secure(int);
public	void	*	liberate_secure(void *);
public	void	*	allocate(int);
public	void	*	liberate(void *);
public	char 	*	allocate_string_secure( char * );
public	char 	*	allocate_string( char * );
public	char 	*	join_strings( char * rptr, char * sptr );
public	char 	*	join_string( char * rptr, char * sptr );
public  char    *       join_string_sep( char * rptr, char * sptr, char * sep );

	/* ----------- */
#endif	/* _allocate_h */
	/* ----------- */


