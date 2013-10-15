#ifndef	_ldapclient_h
#define	_ldapclient_h

#include <stdio.h>
#include <ldap.h>

#define	_DEFAULT_LDAP_HOST 		"localhost"

#ifndef	LDAPS_PORT
#define	LDAPS_PORT	636
#endif

#ifndef	LDAP_PORT
#define	LDAP_PORT	389
#endif

#ifndef	private
#define	private static
#endif

#ifndef	public
#define	public	
#endif

struct	ldap_controller	
{
	char 		*	host;
	char 		*	credentials;
	char 		*	password;
	int			port;
	LDAP		*	handle;
	LDAPMessage	*	result;
	LDAPMessage	*	message;
	BerElement 	* 	itember;
	char			url[2048];
	int			error;
	int			messages;
	char		*	dn;
	char		*	atbname;
	char		*	atbvalue;
	int			synchronise;
	int			operation;
	int			pending;
	int			version;
	char 		*	domain;
	LDAPControl	*	server;
	LDAPControl	*	client;
	char *			tls;
	struct	tls_configuration * tlsconf;
	char *			keyfile;
	char *			certfile;
	int			tlsmode;
	LDAPMod		**	modifications;
};

public	struct 	ldap_controller * 	OpenLdap( char * host, int port, char * credentials, char * password , char * tls);
public	void				CloseLdap( struct ldap_controller * lptr );
public	struct 	ldap_controller * 	LdapAtbValue(struct ldap_controller * lptr );
public	struct 	ldap_controller * 	FirstLdapAtb(struct ldap_controller * lptr, char * criteria );
public	struct 	ldap_controller * 	NextLdapAtb(struct ldap_controller * lptr );
public	int				DropLdapUpdate(struct ldap_controller * lptr );
public	int				StartLdapUpdate(struct ldap_controller * lptr, char * domain );
public	int				StartLdapDelete(struct ldap_controller * lptr, char * domain );
public	int				AddLdapStringAtb(struct ldap_controller * lptr, char * vptr, char * nptr );
public	int				AddLdapWordAtb(struct ldap_controller * lptr, int v, char * nptr );
public	int				FlushLdapUpdate(struct ldap_controller * lptr );
public	int				DeleteLdap( struct ldap_controller * lptr, char * criteria );

	/* ------------- */
#endif	/* _ldapclient_h */
	/* ------------- */

