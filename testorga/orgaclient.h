#ifndef	_orgaclient_h
#define _orgaclient_h

#include "rest.h"
#include "restclient.h"

#ifndef	public	
#define	public
#endif

#ifndef	private
#define	private static
#endif


struct	orga_user
{
	char *	id;
	char * 	name;
	char *	password;
	char *	role;
	char *	email;
};


struct	orga_account
{
	char * 	id;
	char *	name;
	char *	number;
	char *	balence;
};

struct	orga_transaction
{
	char *	id;
	char *	type;
	char *	account;
	char *	label;
	char *	quantity;
	char *	value;
	char *	currency;
};

struct	orga_subscription 
{
	char *	name;
	char *	password;
	char *	tenant;
	char *	account;
	char *	host;
	char *	base;
	char *	version;
};

/* ----------------------- */
/* subscription interfaces */
/* ----------------------- */
public	struct orga_subscription * orga_initialise_subscription( char * account );
public	struct orga_subscription * orga_liberate_subscription( struct orga_subscription * sptr );

/* -------------------------- */
/* user management interfaces */
/* -------------------------- */
public struct rest_response *  orga_create_user  ( struct orga_subscription * sptr, struct orga_user * uptr );
public struct rest_response *  orga_update_user  ( struct orga_subscription * sptr, struct orga_user * uptr );
public struct rest_response *  orga_retrieve_user( struct orga_subscription * sptr, struct orga_user * uptr );
public struct rest_response *  orga_list_user( struct orga_subscription * sptr, struct orga_user * uptr );
public struct rest_response *  orga_delete_user  ( struct orga_subscription * sptr, struct orga_user * uptr );

/* ----------------------------- */
/* account management interfaces */
/* ----------------------------- */
public struct rest_response *  orga_create_account  ( struct orga_subscription * sptr, struct orga_account * aptr );
public struct rest_response *  orga_update_account  ( struct orga_subscription * sptr, struct orga_account * aptr );
public struct rest_response *  orga_retrieve_account( struct orga_subscription * sptr, struct orga_account * aptr );
public struct rest_response *  orga_list_account( struct orga_subscription * sptr, struct orga_account * aptr );
public struct rest_response *  orga_delete_account  ( struct orga_subscription * sptr, struct orga_account * aptr );

/* --------------------------------- */
/* transaction management interfaces */
/* --------------------------------- */
public struct rest_response *  orga_create_transaction  ( struct orga_subscription * sptr, struct orga_transaction * tptr );
public struct rest_response *  orga_update_transaction  ( struct orga_subscription * sptr, struct orga_transaction * tptr );
public struct rest_response *  orga_retrieve_transaction( struct orga_subscription * sptr, struct orga_transaction * tptr );
public struct rest_response *  orga_list_transaction( struct orga_subscription * sptr, struct orga_transaction * uptr );
public struct rest_response *  orga_delete_transaction  ( struct orga_subscription * sptr, struct orga_transaction * tptr );

	/* ------------- */
#endif 	/* _orgaclient_h */
	/* ------------- */
