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
#ifndef	_tls_load_h
#define	_tls_load_h

#include "tlsconfig.h"

public	struct tls_configuration * tls_configuration_load(char * filename );
public	void			   tls_configuration_use( struct tls_configuration * cptr );
public 	struct tls_configuration * liberate_tls_configuration(struct tls_configuration * sptr);
public	struct tls_configuration * release_tls_configuration(struct tls_configuration * cptr);
public 	struct tls_configuration * allocate_tls_configuration();

#endif


