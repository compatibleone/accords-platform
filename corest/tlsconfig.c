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
#ifndef _tlsconfig_c_
#define _tlsconfig_c_

#include "tlsconfig.h"

/*	----------------------------------------------------	*/
/*	l i b e r a t e _ t l s _ c o n f i g u r a t i o n 	*/
/*	----------------------------------------------------	*/
public struct tls_configuration * liberate_tls_configuration(struct tls_configuration * sptr)
{
	if ( sptr )
	{
		if ( sptr->filename )
			 sptr->filename = liberate(sptr->filename);
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->key )
			 sptr->key = liberate(sptr->key);
		if ( sptr->certificate )
			 sptr->certificate = liberate(sptr->certificate);
		if ( sptr->passphrase )
			 sptr->passphrase = liberate(sptr->passphrase);
		sptr = liberate( sptr );
	}
	return((struct tls_configuration *) 0);

}

/*	----------------------------------------------	*/
/*	r e s e t _ t l s _ c o n f i g u r a t i o n 	*/
/*	----------------------------------------------	*/
public struct tls_configuration * reset_tls_configuration(struct tls_configuration * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct tls_configuration*) 0;
		sptr->next = (struct tls_configuration*) 0;
		sptr->filename = (char*) 0;
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->key = (char*) 0;
		sptr->certificate = (char*) 0;
		sptr->passphrase = (char*) 0;
		sptr->option =  0;
		sptr->authenticate =  0;
	}
	return(sptr);

}

/*	----------------------------------------------------	*/
/*	a l l o c a t e _ t l s _ c o n f i g u r a t i o n 	*/
/*	----------------------------------------------------	*/
public struct tls_configuration * allocate_tls_configuration()
{
	struct tls_configuration * sptr;
	if (!( sptr = allocate( sizeof( struct tls_configuration ) ) ))
		return( sptr );
	else	return( reset_tls_configuration(sptr) );
}

#endif	/* _tlsconfig_c_ */
