#ifndef	_ldap_user_c
#define	_ldap_user_c

#include "ldapclient.h"
#include "ldapclient.c"

struct	ldap_configuration
{
	char *	host;
	char *	base;
	char *	user;
	char *	pass;
} LdapConfig = 	{
	(char *) 0,
	(char *) 0,
	(char *) 0
	};

struct	ldap_user
{
	char *	id;
	char *	name;
	char *	password;
	char *	email;
	char *	role;
	int	state;
};

#define	_LDAP_USERS "ou=users,dc=easi-clouds,dc=eu"

/*	-------------------------	*/
/*	s e t _ l d a p _ h o s t 	*/
/*	-------------------------	*/
public	char *	set_ldap_host(char * v)	
{
	if ( LdapConfig.host )
		LdapConfig.host = liberate( LdapConfig.host );
	if (!( v ))
		return( v );
	else	return((LdapConfig.host=allocate_string(v)));
} 

/*	-------------------------	*/
/*	s e t _ l d a p _ b a s e  	*/
/*	-------------------------	*/
public	char *	set_ldap_base(char * v)	
{
	if ( LdapConfig.base )
		LdapConfig.base = liberate( LdapConfig.base );
	if (!( v ))
		return( v );
	else	return((LdapConfig.host=allocate_string(v)));
} 

/*	-------------------------	*/
/*	s e t _ l d a p _ u s e r 	*/
/*	-------------------------	*/
public	char *	set_ldap_user(char * v)	
{
	if ( LdapConfig.user )
		LdapConfig.user = liberate( LdapConfig.user );
	if (!( v ))
		return( v );
	else	return((LdapConfig.user=allocate_string(v)));
} 

/*	-------------------------	*/
/*	s e t _ l d a p _ p a s s	*/
/*	-------------------------	*/
public	char *	set_ldap_pass(char * v)	
{
	if ( LdapConfig.pass )
		LdapConfig.pass = liberate( LdapConfig.pass );
	if (!( v ))
		return( v );
	else	return((LdapConfig.pass=allocate_string(v)));
} 

/*	-------------------------	*/
/*	g e t _ l d a p _ b a s e 	*/
/*	-------------------------	*/
private	char * get_ldap_base()	{	return( LdapConfig.base );	}

/*	-------------------------	*/
/*	g e t _ l d a p _ h o s t 	*/
/*	-------------------------	*/
private	char * get_ldap_host()	{	return( LdapConfig.host );	}

/*	-------------------------	*/
/*	g e t _ l d a p _ u s e r 	*/
/*	-------------------------	*/
private	char * get_ldap_user()	{	return( LdapConfig.user );	}

/*	-------------------------	*/
/*	g e t _ l d a p _ p a s s 	*/
/*	-------------------------	*/
private	char * get_ldap_pass()	{	return( LdapConfig.pass );	}
	
/*	-----------------------------------	*/
/*	l i b e r a t e _ l d a p _ u s e r	*/
/*	-----------------------------------	*/
private	struct ldap_user * liberate_ldap_user(struct	ldap_user * uptr)
{
	if ( uptr )
	{
		if ( uptr->id )
			uptr->id = liberate( uptr->id );
		if ( uptr->name )
			uptr->name = liberate( uptr->name );
		if ( uptr->password )
			uptr->password = liberate( uptr->password );
		if ( uptr->email )
			uptr->email = liberate( uptr->email );
		if ( uptr->role )
			uptr->role = liberate( uptr->role );
		liberate( uptr );
	}
	return((struct ldap_user *) 0);
}

/*	-----------------------------------	*/
/*	a l l o c a t e _ l d a p _ u s e r	*/
/*	-----------------------------------	*/
private	struct ldap_user * allocate_ldap_user()
{
	struct	ldap_user * uptr;
	if (!( uptr = (struct ldap_user *) allocate( sizeof( struct ldap_user ) ) ))
		return( uptr );
	else
	{
		memset( uptr, 0, sizeof(struct ldap_user ) );
		return( uptr );
	}
}

/*	-------------------------------		*/
/*	R e t r i e v e L d a p U s e r 	*/
/*	-------------------------------		*/
private	struct	ldap_user * RetrieveLdapUser( struct ldap_controller * lptr, char * id )
{
	char *	bptr;
	struct	ldap_user * uptr;
	char 	criteria[1024];
	if (!( bptr = get_ldap_base()))
		return((struct ldap_user *) 0);
	else	sprintf(criteria,"cn=%s, %s",id, bptr );
	if (!( lptr = FirstLdapAtb( lptr,criteria )))
		return((struct ldap_user *) 0);
	else if (!( uptr = allocate_ldap_user()))
		return( uptr );
	else
	{
		do	
		{
			if (!( lptr->atbname ))
				break;
			else if (!( strcmp( lptr->atbname, "givenName") ))
			{
				if (!( uptr->name = allocate_string( lptr->atbvalue ) ))
					break;
				else	continue;
			}
			else if (!( strcmp( lptr->atbname, "cn") ))
			{
				if (!( uptr->id = allocate_string( lptr->atbvalue ) ))
					break;
				else	continue;
			}
			else if (!( strcmp( lptr->atbname, "userPassword") ))
			{
				if (!( uptr->password = allocate_string( lptr->atbvalue ) ))
					break;
				else	continue;
			}
			else if (!( strcmp( lptr->atbname, "mail") ))
			{
				if (!( uptr->email = allocate_string( lptr->atbvalue ) ))
					break;
				else	continue;
			}
			else if (!( strcmp( lptr->atbname, "sn") ))
			{
				if (!( uptr->role = allocate_string( lptr->atbvalue ) ))
					break;
				else	continue;
			}
			else if (!( strcmp( lptr->atbname, "state") ))
			{
				uptr->state = atoi( lptr->atbvalue );
				continue;
			}
		}
		while (( lptr = NextLdapAtb( lptr )) != (struct ldap_controller *) 0);
		return( uptr );
	}
}

/*	-------------------------------		*/
/*	D e l e t e   L d a p   U s e r 	*/
/*	-------------------------------		*/
private	int	DeleteLdapUser( struct ldap_controller * lptr, char * id )
{
	int	e;
	char *	bptr;
	struct	ldap_user * uptr;
	char 	criteria[1024];
	if (!( bptr = get_ldap_base()))
		return(78);
	else
	{
		sprintf(criteria,"cn=%s, %s",id, bptr );
		if ((e = ldap_delete_s( lptr->handle, criteria )) != LDAP_SUCCESS ) 
			return( e );
		else	return(0);
	}
}

/*	------------------------------------------------------------------	*/
/*			o c c i _ o r g a _ u s e r _ g e t			*/
/*	------------------------------------------------------------------	*/
private struct rest_response * occi_ldap_user_get(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
{
	struct	ldap_controller * lptr=(struct ldap_controller *) 0;
	struct rest_response * aptr=(struct rest_response *) 0;
	struct	ldap_user * uptr;

	if (!(aptr = rest_allocate_response( cptr )))
		return( aptr );
	else if (!( lptr = OpenLdap( get_ldap_host(), get_ldap_user(), get_ldap_pass() ) ))
		return( rest_html_response( aptr, 401, "Not Authorised") );
	else if (!( uptr = RetrieveLdapUser( lptr, "id" ) ))
	{
		CloseLdap( lptr );
		return( rest_html_response( aptr, 404, "Not Found") );
	}
	else
	{
		CloseLdap( lptr );
		uptr = liberate_ldap_user( uptr );
		return( rest_html_response( aptr, 405, "Not Supported") );
	}
}

/*	------------------------------------------------------------------	*/
/*			o c c i _ o r g a _ u s e r _ p o s t 			*/
/*	------------------------------------------------------------------	*/
private struct rest_response * occi_ldap_user_post(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
{
	struct	ldap_controller * lptr=(struct ldap_controller *) 0;
	struct rest_response * aptr=(struct rest_response *) 0;

	if (!(aptr = rest_allocate_response( cptr )))
		return( aptr );
	else if (!( lptr = OpenLdap( get_ldap_host(), get_ldap_user(), get_ldap_pass() ) ))
		return( rest_html_response( aptr, 401, "Not Authorised") );
	else
	{
		CloseLdap( lptr );
		return( rest_html_response( aptr, 405, "Not Supported") );
	}
}

/*	------------------------------------------------------------------	*/
/*			o c c i _ o r g a _ u s e r _ p u t 			*/
/*	------------------------------------------------------------------	*/
private struct rest_response * occi_ldap_user_put(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
{
	struct	ldap_user * uptr;
	struct	ldap_controller * lptr=(struct ldap_controller *) 0;
	struct rest_response * aptr=(struct rest_response *) 0;

	if (!(aptr = rest_allocate_response( cptr )))
		return( aptr );
	else if (!( lptr = OpenLdap( get_ldap_host(), get_ldap_user(), get_ldap_pass() ) ))
		return( rest_html_response( aptr, 401, "Not Authorised") );
	else if (!( uptr = RetrieveLdapUser( lptr, "id" ) ))
	{
		CloseLdap( lptr );
		return( rest_html_response( aptr, 404, "Not Found") );
	}
	else
	{
		CloseLdap( lptr );
		uptr = liberate_ldap_user( uptr );
		return( rest_html_response( aptr, 405, "Not Supported") );
	}
}

/*	------------------------------------------------------------------	*/
/*			o c c i _ o r g a _ u s e r _ d e l e t e 		*/
/*	------------------------------------------------------------------	*/
private struct rest_response * occi_ldap_user_delete(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
{
	int	status;
	struct	ldap_controller * lptr=(struct ldap_controller *) 0;
	struct rest_response * aptr=(struct rest_response *) 0;

	if (!(aptr = rest_allocate_response( cptr )))
		return( aptr );
	else if (!( lptr = OpenLdap( get_ldap_host(), get_ldap_user(), get_ldap_pass() ) ))
		return( rest_html_response( aptr, 401, "Not Authorised") );
	else if (( status = DeleteLdapUser( lptr, "id" )) != 0)
	{
		CloseLdap( lptr );
		return( rest_html_response( aptr, 404, "Not Found") );
	}
	else
	{
		CloseLdap( lptr );
		return( rest_html_response( aptr, 405, "Not Supported") );
	}
}

/*	------------------------------------------------------------------	*/
/*			o c c i _ o r g a _ u s e r _ h e a d			*/
/*	------------------------------------------------------------------	*/
private struct rest_response * occi_ldap_user_head(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
{
	struct	ldap_controller * lptr=(struct ldap_controller *) 0;
	struct rest_response * aptr=(struct rest_response *) 0;

	if (!(aptr = rest_allocate_response( cptr )))
		return( aptr );
	else if (!( lptr = OpenLdap( get_ldap_host(), get_ldap_user(), get_ldap_pass() ) ))
		return( rest_html_response( aptr, 401, "Not Authorised") );
	else
	{
		CloseLdap( lptr );
		return( rest_html_response( aptr, 405, "Not Supported") );
	}
}

/*	------------------------------------------------------------------	*/
/*		r e d i r e c t _ o c c i _ l d a p _ u s e r _ m t		*/
/*	------------------------------------------------------------------	*/
private void	redirect_occi_ldap_user_mt( struct rest_interface * iptr )
{
	iptr->get = occi_ldap_user_get;
	iptr->post = occi_ldap_user_post;
	iptr->put = occi_ldap_user_put;
	iptr->delete = occi_ldap_user_delete;
	iptr->head = occi_ldap_user_head;
	return;
}

/*	------------------------------------------------------------------	*/
/*		    o c c i _ o r g a _ u s e r _ b u i l d e r			*/
/*	------------------------------------------------------------------	*/
private	struct	occi_category * occi_ldap_user_builder( char * domain, char * category )
{
	char * c="http://scheme.compatibleone.fr/scheme/compatible#";
	char * d="kind";
	char * e="http://scheme.ogf.org/occi/resource#";
	char * f="ORGA OCCI LDAP resource USER";
	struct occi_category * optr;
	if (!( optr = occi_create_category(domain, category,c,d,e,f) )) 
	{ 
		return(optr); 
	}
	else 	
	{
		redirect_occi_ldap_user_mt( optr->interface );
		if (!( optr = occi_add_attribute(optr, "name",0,0) ))
			return(optr);
		else if (!( optr = occi_add_attribute(optr, "when",0,0) ))
			return(optr);
		else if (!( optr = occi_add_attribute(optr, "state",0,0) ))
			return(optr);
		else if (!( optr = occi_add_attribute(optr, "role",0,0) ))
			return(optr);
		else if (!( optr = occi_add_attribute(optr, "password",0,0) ))
			return(optr);
		else if (!( optr = occi_add_attribute(optr, "email",0,0) ))
			return(optr);
		else if (!( optr = occi_add_attribute(optr, "account",0,0) ))
			return(optr);
		else if (!( optr = occi_add_attribute(optr, "authorization",0,0) ))
			return(optr);
		return(optr);
	}
}

	/* ------------ */
#endif	/* _ldap_user_c */
	/* ------------ */

