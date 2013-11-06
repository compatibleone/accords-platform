#ifndef	_orga_config_c
#define	_orga_config_c

#include "document.h"

private	struct	orga_client_configuration
{
	char *	account;
	char *	host;
	char *	version;
	char *	user;
	char *	password;
	char *	operator;
	char *	provider;
	char *	tls;
	struct	orga_subscription * subscription;
} OrgaSubscription = { 
	(char *) 0, 
	(char *) 0, 
	(char *) 0, 
	(char *) 0, 
	(char *) 0, 
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(struct orga_subscription *) 0
	};

/*	---------------------------------------------		*/
/*	 l o a d _ o r g a _ s u b s c r i p t i o n		*/
/*	---------------------------------------------		*/
private	int	load_orga_subscription( char * filename, char * provider )
{
	struct	xml_element * document;
	struct	xml_element * eptr;
	struct	xml_element * element;
	if (!( document = document_parse_file( filename ) ))
		return( 0 );
	else if (!( eptr = document_element( document, "orga_client" ) ))
	{ 
		document = document_drop( document );
		return( 0 );
	}
	else if (!( OrgaSubscription.provider = allocate_string( provider ) ))
	{ 
		document = document_drop( document );
		return( 0 );
	}
	else
	{
		if ((element = document_element( eptr, "account" )) != (struct xml_element *) 0)
			OrgaSubscription.account = allocate_string( (element->value ? element->value : "" ) );
		if ((element = document_element( eptr, "host" )) != (struct xml_element *) 0)
			OrgaSubscription.host = allocate_string( (element->value ? element->value : "" ) );
		if ((element = document_element( eptr, "version" )) != (struct xml_element *) 0)
			OrgaSubscription.version = allocate_string( (element->value ? element->value : "" ) );
		if ((element = document_element( eptr, "user" )) != (struct xml_element *) 0)
			OrgaSubscription.user = allocate_string( (element->value ? element->value : "" ) );
		if ((element = document_element( eptr, "password" )) != (struct xml_element *) 0)
			OrgaSubscription.password = allocate_string( (element->value ? element->value : "" ) );
		if ((element = document_element( eptr, "operator" )) != (struct xml_element *) 0)
			OrgaSubscription.operator = allocate_string( (element->value ? element->value : "" ) );
		if ((element = document_element( eptr, "tls" )) != (struct xml_element *) 0)
			OrgaSubscription.tls = allocate_string( (element->value ? element->value : "" ) );
		document = document_drop( document );
		return( 1 );
	}
}

/*	---------------------------------------------		*/
/*	c h e c k _ o r g a _ s u b s c r i p t i o n		*/
/*	---------------------------------------------		*/
public	struct	orga_subscription * check_orga_subscription( char * filename, char * provider )
{
	if (!( OrgaSubscription.subscription ))
	{
		if (!( load_orga_subscription( filename, provider ) ))
			return( OrgaSubscription.subscription );
		else 	OrgaSubscription.subscription = orga_initialise_subscription(
				OrgaSubscription.host, 
				OrgaSubscription.account, 
				OrgaSubscription.password, 
				OrgaSubscription.operator, 
				OrgaSubscription.provider,
				OrgaSubscription.version, 
				OrgaSubscription.tls );
	}
	return( OrgaSubscription.subscription );
}

	/* -------------- */
#endif	/* _orga_config_c */
	/* -------------- */

