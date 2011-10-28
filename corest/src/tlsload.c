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
				cptr->option = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "authenticate" )) != (struct xml_atribut *) 0)
				cptr->authenticate = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "key" )) != (struct xml_atribut *) 0)
				cptr->key = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "certificate" )) != (struct xml_atribut *) 0)
				cptr->certificate = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "passphrase" )) != (struct xml_atribut *) 0)
				cptr->passphrase = document_atribut_string( aptr );
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
	https_use_encryption( cptr->key );
	https_use_certificate( cptr->certificate );
	https_use_password( cptr->passphrase );
	return;
}

#endif	/* _tls_load_c */
	/* ----------- */



