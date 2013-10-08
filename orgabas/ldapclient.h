#ifndef	_ldapclient_h
#define	_ldapclient_h

#include <stdio.h>
#include <ldap.h>

#define	_DEFAULT_LDAP_HOST 		"localhost"
#define	_DEFAULT_LDAP_DOMAIN 		"ou=confAdmin"

#ifndef	private
#define	private static
#endif

#ifndef	public
#define	public	
#endif

struct	ldap_controller	
{
	LDAP		*	handle;
	LDAPMessage	*	result;
	LDAPMessage	*	message;
	BerElement 	* 	itember;
	int			error;
	int			messages;
	char		*	dn;
	char		*	atbname;
	char		*	atbvalue;
	int			synchronise;
	int			operation;
	int			pending;
	char 		*	domain;
	LDAPMod		**	modifications;
};

public	struct 	ldap_controller * 	OpenLdap( char * host, char * credentials, char * password );
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

