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
#ifndef	_tls_load_c
#define	_tls_load_c

#include "standard.h"
#include "allocate.h"
#include "document.h"
#include "tlsload.h"
#include "tlsconfig.c"


struct	tls_manager
{
	struct	tls_configuration * first;
	struct	tls_configuration * last;
}	TlsMan = {
	(struct tls_configuration *) 0,
	(struct tls_configuration *) 0
	};


/*	-------------------------------------------------	*/  
/*	r e l e a s e _ t l s _ c o n f i g u r a t i o n	*/
/*	-------------------------------------------------	*/
public	struct tls_configuration * release_tls_configuration(struct tls_configuration * cptr)
{
	return((struct tls_configuration*) 0);
}

/*	-------------------------------------------------	*/  
/*	r e s o l v e _ t l s _ c o n f i g u r a t i o n	*/
/*	-------------------------------------------------	*/
public	struct tls_configuration * resolve_tls_configuration(char * filename )
{
	struct	tls_configuration * cptr;
	for (	cptr=TlsMan.first;
		cptr != (struct tls_configuration *) 0;
		cptr = cptr->next )
	{
		if (!( cptr->filename ))
			continue;
		else if (!( strcmp( cptr->filename, filename ) ))
			break;
	}
	return( cptr );
}

private struct tls_mode {
	char *mode_str;
	int mode_int;
} tls_modes[] = {
	{ "debug",				_SSL_DEBUG			},
	{ "use-ssl",			_USE_SSL			},
	{ "request-peer",		_REQUEST_PEER		},
	{ "require-peer",		_REQUIRE_PEER		},
	{ "der-key",			_DER_KEY			},
	{ "der-cert",			_DER_CERTIFICATE	},
	{ "ssl-compat",			_SSL_COMPATIBLE		},
	{ "internal",			_SSL_INTERNAL		},
	{ "engine",				_OPENSSL_ENGINE		},
	{ "accept-invalid",		_SSL_ACCEPT_INVALID	},
	{ "self-signed",		_SSL_SELF_SIGNED	},
	{ "valid-cert",			_SSL_VALID_CERT		},
	{ "same-ca",			_SSL_SAME_CA		},
	{ NULL,					0					}
};


private int tls_mode_parse(const char *str) {
	int mode = 0;
	int m;
	char *bs = allocate_string((char*)str);
	char *s = bs;
	char *ss = NULL;
	printf("TLS_MODE_PARSE(%s)\n", str);
	while((s = strtok_r(s, " ", &ss)) != NULL) {
		for(m=0; tls_modes[m].mode_str; m++) {
			if(!strcmp(s, tls_modes[m].mode_str)) {
				break;
			}
		}
		if(!tls_modes[m].mode_int) {
			printf("WARNING: unknown option '%s' (in mode='%s')\n", s, str);
		} else {
			mode |= tls_modes[m].mode_int;
		}

		s = NULL;
	}
	liberate(bs);
	if(!(mode & _SSL_MODES)) {
//		mode |= _SSL_VALID_CERT; // valid-cert is the default
		mode |= _SSL_ACCEPT_INVALID; // Be conservative for now
	}
	return mode;
}

/*	-------------------------------------------------	*/  
/*	  f l u s h _ t l s _ c o n f i g u r a t i o n		*/
/*	-------------------------------------------------	*/
public	void	flush_tls_configuration()
{
	struct	tls_configuration * cptr;
	while ((cptr=TlsMan.first) != (struct tls_configuration *) 0)
	{
		TlsMan.first = cptr->next;
		cptr = liberate_tls_configuration( cptr );
	}
}	

/*	---------------------------------------------	*/  
/*	 t l s _ c o n f i g u r a t i o n _ l o a d 	*/
/*	---------------------------------------------	*/
/*	this function loads transport layer security	*/
/*	configuration information from the provided	*/
/*	xml 	configuration file.			*/
/*	---------------------------------------------	*/  
public	struct tls_configuration * tls_configuration_load(char * filename )
{
	struct	xml_element * document;
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;
	struct	tls_configuration * cptr;
	if (( cptr = resolve_tls_configuration(filename)) != (struct tls_configuration *) 0)
		return( cptr );	
	else if (!( cptr = allocate_tls_configuration()))
	{
		if ( check_verbose() )
			printf("   Failed to Allocate TLS configuration\n");
		return( cptr );
	}
	else if (!( cptr->filename = allocate_string( filename ) ))
		return( liberate_tls_configuration( cptr ) );
	else
	{
		if (!( cptr->previous = TlsMan.last ))
			TlsMan.first = cptr;
		else	cptr->previous->next = cptr;
		TlsMan.last = cptr;
	}

	if (!( document = document_parse_file( filename )))
	{
		if ( check_verbose() )
			printf("   Failed to load TLS configuration from : %s \n",filename);

	}
	else
	{
		if ( check_verbose() )
			printf("   Loading TLS configuration from : %s \n",filename);

		if (( eptr = document_element( document, "tls" )) != (struct xml_element *) 0)
		{
			if ((aptr = document_atribut( eptr, "id" )) != (struct xml_atribut *) 0)
				cptr->id = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "name" )) != (struct xml_atribut *) 0)
				cptr->name = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "option" )) != (struct xml_atribut *) 0)
				cptr->option |= document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "authenticate" )) != (struct xml_atribut *) 0)
				cptr->authenticate = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "key" )) != (struct xml_atribut *) 0)
				cptr->key = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "certificate" )) != (struct xml_atribut *) 0)
				cptr->certificate = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "passphrase" )) != (struct xml_atribut *) 0)
				cptr->passphrase = document_atribut_password( aptr );
			if ((aptr = document_atribut( eptr, "authority" )) != (struct xml_atribut *) 0)
				cptr->authority = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "mode" )) != (struct xml_atribut *) 0)
				cptr->option |= tls_mode_parse(document_atribut_string( aptr ));
		}
		document = document_drop( document );
	}
	return(cptr);
}

/*	---------------------------------------------	*/  
/*	 t l s _ c o n f i g u r a t i o n _ u s e 	*/
/*	---------------------------------------------	*/
public	void	tls_configuration_use(struct tls_configuration * cptr )
{
	security_lock(0,"configuration");

	if ( cptr->option & _OPTIMISE_OCCI_CLIENT )
		occi_optimise_client(1);
	if ( cptr->option & _OPTIMISE_OCCI_LOCAL )
		occi_optimise_local(1);

	https_use_encryption( cptr->key );
	https_use_certificate( cptr->certificate );
	https_use_password( cptr->passphrase );
	https_use_CA_list( cptr->authority );
	security_unlock(0,"configuration");
	return;
}

#endif	/* _tls_load_c */
	/* ----------- */



