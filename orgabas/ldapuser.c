#ifndef	_ldap_user_c
#define	_ldap_user_c

#include "ldapclient.h"
#include "ldapclient.c"

private	struct	accords_configuration LdapConfig;

struct	ldap_user
{
	char *	id;
	char *	name;
	char *	password;
	char *	email;
	char *	role;
	char *	account;
	char *	authorization;
	int	when;
	int	state;
};

/*	---------------------------------------------	*/
/*	l o a d _ l d a p _ c o n f i g u r a t i o n	*/
/*	---------------------------------------------	*/
public	void	load_ldap_configuration( char * filename, char * section ) 
{	
	memset( &LdapConfig, 0, sizeof( struct accords_configuration ) );
	LdapConfig.config = allocate_string( filename );
	LdapConfig.verbose = 1;
	load_accords_configuration( &LdapConfig, section);
}

/*	------------------------------	*/
/* "ou=users,dc=easi-clouds,dc=eu"	*/
/*	------------------------------	*/

/*	-------------------------	*/
/*	g e t _ l d a p _ b a s e  	*/
/*	-------------------------	*/
private	char * get_ldap_base()	{	return( LdapConfig.storage );	}
private	char * get_ldap_tls()	{	return( LdapConfig.tls );	}

/*	-------------------------	*/
/*	g e t _ l d a p _ h o s t 	*/
/*	-------------------------	*/
private	char * get_ldap_host()	{	return( LdapConfig.resthost );	}

/*	-------------------------	*/
/*	g e t _ l d a p _ p o r t 	*/
/*	-------------------------	*/
private	int	get_ldap_port()	{	return( LdapConfig.restport );	}

/*	-------------------------	*/
/*	g e t _ l d a p _ u s e r 	*/
/*	-------------------------	*/
private	char * get_ldap_user()	{	return( LdapConfig.user );	}

/*	-------------------------	*/
/*	g e t _ l d a p _ p a s s 	*/
/*	-------------------------	*/
private	char * get_ldap_pass()	{	return( LdapConfig.password );	}
	
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

/*		-----------------------------------------------		*/
/*		a d d _ l d a p _ u s e r _ a t t r i b u t e s		*/
/*		-----------------------------------------------		*/
private	int	add_ldap_user_attributes( struct ldap_controller * lptr, struct ldap_user * uptr )
{
	int	status=0;
	if ( uptr->id )
		if ((status = AddLdapStringAtb( lptr, uptr->id, "cn" )) != 0 )
			return( status );
	if ( uptr->name )
		if ((status = AddLdapStringAtb( lptr, uptr->name, "givenName" )) != 0 )
			return( status );
	if ( uptr->password )
		if ((status = AddLdapStringAtb( lptr, uptr->password, "userPassword" )) != 0 )
			return( status );
	if ( uptr->email )
		if ((status = AddLdapStringAtb( lptr, uptr->email, "mail" )) != 0 )
			return( status );
	if ( uptr->role )
		if ((status = AddLdapStringAtb( lptr, uptr->role, "sn" )) != 0 )
			return( status );
	return(0);
}

/*	-------------------------------		*/
/*	R e t r i e v e L d a p U s e r 	*/
/*	-------------------------------		*/
private	struct	ldap_user * RetrieveLdapUser( struct ldap_controller * lptr, char * id )
{
	char *	bptr;
	struct	ldap_user * uptr;
	char 	criteria[1024];
	if (!( id ))
		return((struct ldap_user *) 0);
	else if (!( bptr = get_ldap_base()))
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
/*	R e t r i e v e L d a p U s e r 	*/
/*	-------------------------------		*/
private	struct	rest_header * RetrieveLdapUsers( struct ldap_controller * lptr )
{
	char *	bptr;
	struct	rest_header * hptr=(struct	rest_header *) 0;
	struct	rest_header * root=(struct	rest_header *) 0;
	struct	rest_header * foot=(struct	rest_header *) 0;

	if (!( bptr = get_ldap_base()))
		return((struct rest_header *) 0);
	if (!( lptr = FirstLdapAtb( lptr, bptr )))
		return((struct rest_header *) 0);
	else
	{
		do	
		{
			if (!( strcmp( lptr->atbname, "cn") ))
			{
				if (!( hptr = rest_create_header( "location", lptr->atbvalue ) ))
					break;
				else if (!( hptr->previous = foot ))
					root = hptr;
				else
					foot->next = hptr;
				foot = hptr;
				continue;
			}

		}
		while (( lptr = NextLdapAtb( lptr )) != (struct ldap_controller *) 0);
		return( root );
	}
}

/*	----------------------------------	*/
/*		U p d a t e L D a p U s e r		*/
/*	----------------------------------	*/
private	struct ldap_user * UpdateLdapUser( struct ldap_controller * lptr, struct ldap_user * uptr )
{
	char *	bptr;
	char 	criteria[1024];
	if (!( uptr->id ))
		return( liberate_ldap_user( uptr ) );
	else if (!( bptr = get_ldap_base()))
		return((struct ldap_user *) 0);
	else	
		sprintf(criteria,"cn=%s, %s",uptr->id, bptr );

	if ( StartLdapUpdate( lptr, criteria ) != 0 )
		return( liberate_ldap_user( uptr ) );
	else if ( add_ldap_user_attributes( lptr, uptr ) != 0 )
		return( liberate_ldap_user( uptr ) );
	else if ( FlushLdapUpdate( lptr ) != 0 )
		return( liberate_ldap_user( uptr ) );
	else
		return( uptr );
}

/*	----------------------------------	*/
/*		C r e a t e L D a p U s e r		*/
/*	----------------------------------	*/
private	struct ldap_user * CreateLdapUser( struct ldap_controller * lptr, struct ldap_user * uptr )
{
	char *	bptr;
	char 	criteria[1024];
	if (!( uptr->id ))
		return( liberate_ldap_user( uptr ) );
	else if (!( bptr = get_ldap_base()))
		return((struct ldap_user *) 0);
	else	
		sprintf(criteria,"cn=%s, %s",uptr->id, bptr );

	if ( StartLdapUpdate( lptr, criteria ) != 0 )
		return( liberate_ldap_user( uptr ) );
	else if ( add_ldap_user_attributes( lptr, uptr ) != 0 )
		return( liberate_ldap_user( uptr ) );
	else if ( FlushLdapUpdate( lptr ) != 0 )
		return( liberate_ldap_user( uptr ) );
	else
		return( uptr );
}

/*	----------------------------------	*/
/*		B u i l d L D a p U s e r		*/
/*	----------------------------------	*/
private	struct ldap_user * BuildLdapUser( struct rest_header * hptr, struct ldap_user * uptr )
{
	char * 	vptr;
	char 	oname[1024];
	int		i=0;
	int		allocated=0;

	/* ------------------------------------ */
	/* allocate a new ldap user if required */
	/* ------------------------------------ */
	if (!( uptr ))
	{
		if (!( uptr = allocate_ldap_user() ))
			return( uptr );
		else if (!( uptr->id = rest_allocate_uuid() ))
			return( liberate_ldap_user( uptr ) );
		else
			allocated=1;
	}

	/* ------------------------------------------------- */
	/* walk list of headers detecting member expressions */
	/* ------------------------------------------------- */
	for ( 	;
			hptr !=(struct rest_header *) 0;
			hptr = hptr->next  )
	{
		/* -------------------------------------- */
		/* detect valid occi attribute expression */
		/* -------------------------------------- */
		if (!( hptr->name ))
			continue;
		else if (!( vptr = hptr->value ))
			continue;
		else if ( strcasecmp( hptr->name,"X-OCCI-Attribute" ) != 0)
			continue;
		else
		{
			/* --------------------------------- */
			/* step over attribute name to value */
			/* --------------------------------- */
			for (i=0; i < 1023; i++ )
				if ((oname[i] = *(vptr+i)) == '=')
					break;
			oname[i++] = 0;
			vptr += i;

			/* --------------------------------- */
			/* prepare value and store to member */
			/* --------------------------------- */
			if (!( vptr = allocate_string( vptr ) ))
				return( liberate_ldap_user( uptr ) );
			else if (!( vptr = occi_unquoted_value( vptr ) ))
				return( liberate_ldap_user( uptr ) );

			else if (!( strcmp( oname, "id" ) ))
			{
				if (!( allocated ))
				{
					if ( uptr->id ) uptr->id = liberate( uptr->id );
					if (!( uptr->id = vptr ))
						return( liberate_ldap_user( uptr ) );
					else
						continue;
				}
				else
				{
					vptr = liberate( vptr );
					continue;
				}
			}
			else if (!( strcmp( oname, "name" ) ))
			{
				if ( uptr->name ) uptr->name = liberate( uptr->name );
				if (!( uptr->name = vptr ))
					return( liberate_ldap_user( uptr ) );
				else
					continue;
			}
			else if (!( strcmp( oname, "password" ) ))
			{
				if ( uptr->password ) uptr->password = liberate( uptr->password );
				if (!( uptr->password = vptr ))
					return( liberate_ldap_user( uptr ) );
				else
					continue;
			}
			else if (!( strcmp( oname, "email" ) ))
			{
				if ( uptr->email ) uptr->email = liberate( uptr->email );
				if (!( uptr->email = vptr ))
					return( liberate_ldap_user( uptr ) );
				else
					continue;
			}
			else if (!( strcmp( oname, "role" ) ))
			{
				if ( uptr->role ) uptr->role = liberate( uptr->role );
				if (!( uptr->role = vptr ))
					return( liberate_ldap_user( uptr ) );
				else
					continue;
			}
			else if (!( strcmp( oname, "account" ) ))
			{
				if ( uptr->account ) uptr->account = liberate( uptr->account );
				if (!( uptr->account = vptr ))
					return( liberate_ldap_user( uptr ) );
				else
					continue;
			}
			else if (!( strcmp( oname, "authorization" ) ))
			{
				if ( uptr->authorization ) uptr->authorization = liberate( uptr->authorization );
				if (!( uptr->authorization = allocate_string( vptr ) ))
					return( liberate_ldap_user( uptr ) );
				else
					continue;
			}
			else
			{
				vptr = liberate( vptr );
				continue;
			}
		}
	}
	return( uptr );
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
	if (!( id ))
		return( 118 );
	else if (!( bptr = get_ldap_base()))
		return(78);
	else
	{
		sprintf(criteria,"cn=%s, %s",id, bptr );
		return(DeleteLdap( lptr, criteria ));
	}
}

/*		---------------------------------------------		*/
/*		o c c i _ l d a p _ u s e r _ r e s p o n s e		*/
/*		---------------------------------------------		*/
private	struct rest_response * occi_ldap_user_response( 
	struct occi_category * optr, 
	struct rest_response * aptr, 
	struct rest_client * cptr,
	struct rest_request * rptr, 
	struct ldap_user * uptr )
{
	struct rest_header * hptr;
	sprintf(cptr->buffer,"occi.core.id=%c%s%c",0x0022,uptr->id,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.name=%c%s%c",optr->domain,optr->id,0x0022,uptr->name,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.email=%c%s%c",optr->domain,optr->id,0x0022,uptr->email,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.role=%c%s%c",optr->domain,optr->id,0x0022,uptr->role,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.account=%c%s%c",optr->domain,optr->id,0x0022,uptr->account,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.authorization=%c%s%c",optr->domain,optr->id,0x0022,uptr->authorization,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.password=%c%s%c",optr->domain,optr->id,0x0022,uptr->password,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.when=%c%u%c",optr->domain,optr->id,0x0022,uptr->when,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.state=%c%u%c",optr->domain,optr->id,0x0022,uptr->state,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	else	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );	;
}

/*		---------------------------------------------		*/
/*		o c c i _ l d a p _ u s e r _ r e s p o n s e		*/
/*		---------------------------------------------		*/
private	struct rest_response * 	occi_ldap_location_response( 
	struct occi_category * optr, 
	struct rest_response * aptr, 
	struct rest_client * cptr,
	struct rest_request * rptr, 
	struct rest_header * hptr )
{
	return(aptr);
}

/*	------------------------------------------------------------------	*/
/*			o c c i _ o r g a _ u s e r _ i t e m			*/
/*	------------------------------------------------------------------	*/
private struct rest_response * occi_ldap_user_item(
	struct occi_category * optr, 
	struct rest_response * aptr, 
	struct rest_client * cptr, 
	struct rest_request * rptr, 
	char * id)
{
	struct	ldap_user * uptr = (struct ldap_user *) 0;
	struct	ldap_controller * lptr=(struct ldap_controller *) 0;

	if (!( lptr = OpenLdap( get_ldap_host(), get_ldap_port(), 
				get_ldap_user(), get_ldap_pass(), 
				get_ldap_tls() ) ))
		return( rest_html_response( aptr, 401, "Not Authorised") );
	else if (!( uptr = RetrieveLdapUser( lptr, id ) ))
	{
		CloseLdap( lptr );
		return( rest_html_response( aptr, 404, "Not Found") );
	}
	else
	{
		CloseLdap( lptr );
		aptr = occi_ldap_user_response( optr, aptr, cptr, rptr, uptr );
		uptr = liberate_ldap_user( uptr );
		return( aptr );
	}
}

/*	------------------------------------------------------------------	*/
/*			o c c i _ o r g a _ u s e r _ l i s t 			*/
/*	------------------------------------------------------------------	*/
private struct rest_response * occi_ldap_user_list(
	struct occi_category * optr, 
	struct rest_response * aptr, 
	struct rest_client * cptr, 
	struct rest_request * rptr)
{
	struct	rest_header * hptr=(struct rest_header *) 0;
	struct	ldap_controller * lptr=(struct ldap_controller *) 0;

	if (!( lptr = OpenLdap( get_ldap_host(), get_ldap_port(), 
				get_ldap_user(), get_ldap_pass(), 
				get_ldap_tls() ) ))
		return( rest_html_response( aptr, 401, "Not Authorised") );
	else if (!( hptr = RetrieveLdapUsers( lptr ) ))
	{
		CloseLdap( lptr );
		return( rest_html_response( aptr, 404, "Not Found") );
	}
	else
	{
		CloseLdap( lptr );
		aptr = occi_ldap_location_response( optr, aptr, cptr, rptr, hptr );
		return( aptr );
	}
}

/*	------------------------------------------------------------------	*/
/*			o c c i _ o r g a _ u s e r _ g e t			*/
/*	------------------------------------------------------------------	*/
private struct rest_response * occi_ldap_user_get(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
{
	struct	occi_category * optr=(struct occi_category *) 0;
	struct 	rest_response * aptr=(struct rest_response *) 0;
	char *	id=(char *) 0;

	if (!(aptr = rest_allocate_response( cptr )))
		return( aptr );
	else if (!( optr = vptr ))
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( strcmp( rptr->object, optr->location ) ))
		return( occi_ldap_user_list( optr, aptr, cptr, rptr ) );
	else if (!( id = (rptr->object+strlen(optr->location) )))
		return( occi_ldap_user_list( optr, aptr, cptr, rptr ) );
	else
		return( occi_ldap_user_item( optr, aptr, cptr, rptr, id ) );
}

/*	------------------------------------------------------------------	*/
/*			o c c i _ o r g a _ u s e r _ p o s t 			*/
/*	------------------------------------------------------------------	*/
private struct rest_response * occi_ldap_user_post(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
{
	struct	occi_category * optr=(struct occi_category *) 0;
	struct	ldap_user * uptr = (struct ldap_user *) 0;
	struct	ldap_controller * lptr=(struct ldap_controller *) 0;
	struct rest_response * aptr=(struct rest_response *) 0;

	if (!(aptr = rest_allocate_response( cptr )))
		return( aptr );
	else if (!( optr = vptr ))
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( lptr = OpenLdap( get_ldap_host(), get_ldap_port(), 
				get_ldap_user(), get_ldap_pass(), 
				get_ldap_tls() ) ))
		return( rest_html_response( aptr, 401, "Not Authorised") );
	else if (!( uptr = BuildLdapUser( rptr->first, uptr ) ))
	{
		CloseLdap( lptr );
		return( rest_html_response( aptr, 400, "Bad Request") );
	}
	else if (!( uptr = CreateLdapUser( lptr, uptr )))
	{
		CloseLdap( lptr );
		return( rest_html_response( aptr, 500, "Server Failure") );
	}
	else
	{
		CloseLdap( lptr );
		uptr = liberate_ldap_user( uptr );
		return( rest_response_status( aptr, 200, "OK" ) );
	}
}

/*	------------------------------------------------------------------	*/
/*			o c c i _ o r g a _ u s e r _ p u t 			*/
/*	------------------------------------------------------------------	*/
private struct rest_response * occi_ldap_user_put(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
{
	struct	occi_category * optr=(struct occi_category *) 0;
	struct	ldap_user * uptr = (struct ldap_user *) 0;
	struct	ldap_controller * lptr=(struct ldap_controller *) 0;
	struct 	rest_response * aptr=(struct rest_response *) 0;
	char *	id=(char *) 0;

	if (!(aptr = rest_allocate_response( cptr )))
		return( aptr );
	else if (!( optr = vptr ))
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( id = (rptr->object+strlen(optr->location) )))
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( lptr = OpenLdap( get_ldap_host(), get_ldap_port(), 
				get_ldap_user(), get_ldap_pass(), 
				get_ldap_tls() ) ))
		return( rest_html_response( aptr, 401, "Not Authorised") );
	else if (!( uptr = RetrieveLdapUser( lptr, id ) ))
	{
		CloseLdap( lptr );
		return( rest_html_response( aptr, 404, "Not Found") );
	}
	else if (!( uptr = BuildLdapUser( rptr->first, uptr ) ))
	{
		CloseLdap( lptr );
		return( rest_html_response( aptr, 400, "Bad Request") );
	}
	else if (!( uptr = UpdateLdapUser( lptr, uptr ) ))
	{
		CloseLdap( lptr );
		return( rest_html_response( aptr, 500, "Server Failure") );
	}
	else
	{
		CloseLdap( lptr );
		aptr = occi_ldap_user_response( optr, aptr, cptr, rptr, uptr );
		uptr = liberate_ldap_user( uptr );
		return( aptr );
	}
}

/*	------------------------------------------------------------------	*/
/*			o c c i _ o r g a _ u s e r _ d e l e t e 		*/
/*	------------------------------------------------------------------	*/
private struct rest_response * occi_ldap_user_delete(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
{
	struct	occi_category * optr=(struct occi_category *) 0;
	struct	ldap_user * uptr = (struct ldap_user *) 0;
	int		status;
	struct	ldap_controller * lptr=(struct ldap_controller *) 0;
	struct	rest_response * aptr=(struct rest_response *) 0;
	char *	id=(char *) 0;

	if (!(aptr = rest_allocate_response( cptr )))
		return( aptr );
	else if (!( optr = vptr ))
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( id = (rptr->object+strlen(optr->location) )))
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( lptr = OpenLdap( get_ldap_host(), get_ldap_port(), 
				get_ldap_user(), get_ldap_pass(), 
				get_ldap_tls() ) ))
		return( rest_html_response( aptr, 401, "Not Authorised") );
	else if (( status = DeleteLdapUser( lptr, id )) != 0)
	{
		CloseLdap( lptr );
		return( rest_html_response( aptr, 404, "Not Found") );
	}
	else
	{
		CloseLdap( lptr );
		return( rest_response_status( aptr, 200, "OK" ) );
	}
}

/*	------------------------------------------------------------------	*/
/*			o c c i _ o r g a _ u s e r _ h e a d			*/
/*	------------------------------------------------------------------	*/
private struct rest_response * occi_ldap_user_head(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
{
	struct	occi_category * optr=(struct occi_category *) 0;
	struct	ldap_user * uptr = (struct ldap_user *) 0;
	struct	ldap_controller * lptr=(struct ldap_controller *) 0;
	struct 	rest_response * aptr=(struct rest_response *) 0;
	char *	id=(char *) 0;

	if (!(aptr = rest_allocate_response( cptr )))
		return( aptr );
	else if (!( optr = vptr ))
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( id = (rptr->object+strlen(optr->location) )))
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( lptr = OpenLdap( get_ldap_host(), get_ldap_port(), 
				get_ldap_user(), get_ldap_pass(), 
				get_ldap_tls() ) ))
		return( rest_html_response( aptr, 401, "Not Authorised") );
	else if (!( uptr = RetrieveLdapUser( lptr, id ) ))
	{
		CloseLdap( lptr );
		return( rest_html_response( aptr, 404, "Not Found") );
	}
	else
	{
		CloseLdap( lptr );
		aptr = occi_ldap_user_response( optr, aptr, cptr, rptr, uptr );
		uptr = liberate_ldap_user( uptr );
		return( aptr );
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

