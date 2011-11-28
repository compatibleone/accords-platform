#ifndef	_occi_login_c
#define	_occi_login_c

#include "standard.h"
#include "rest.h"
#include "occi.h"
#include "cp.h"

/*	---------------------------------------------------------	*/
/*			l o g i n _ o c c i _ u s e r 			*/
/*	---------------------------------------------------------	*/
public	char * login_occi_user( char * user,	char * password, char * agent, char * tls )
{
	char	buffer[4096];
	struct	occi_element  * header;
	struct	occi_element  * eptr;
	struct	occi_response * yptr;
	struct	occi_response * uptr;
	char *	tptr=(char *) 0;
	char *	vptr=(char *) 0;
	char *	category = _CORDS_AUTHORISATION;
	if (!( yptr = cords_retrieve_named_instance_list( _CORDS_USER, "occi.user.name", user, agent,tls ) ))
		return( (char *) 0 );
	else if (!( uptr = cords_retrieve_named_instance( yptr, agent,tls )))
		return( (char *) 0 );
	else
	{
		/* ------------------------------------------------- */
		/* scan the list of elements and verify the password */
		/* ------------------------------------------------- */
		for (	eptr = uptr->first;
			eptr != (struct occi_element *) 0;
			eptr = eptr->next )
		{
			if ( strcmp( eptr->name, "occi.user.password"  ) != 0)
				continue;
			else if (!( strcmp( eptr->value, password) ))
				break;
		}
		yptr = occi_remove_response( yptr );
		if (!( eptr ))
		{
			uptr = occi_remove_response( uptr );
			return((char *) 0);
		}
		/* ----------------------------------------------- */
		/* create authorization for the authenticated user */
		/* ----------------------------------------------- */
		sprintf(buffer,"%s%s",uptr->host,uptr->name);
		if (!( header = occi_create_element( "occi.authorisation.user", buffer ) ))
		{
			uptr = occi_remove_response( uptr );
			return((char *) 0);
		}
		else if (!( yptr = cords_create_instance( category, agent, header, tls ) ))
		{
			uptr = occi_remove_response( uptr );
			return((char *) 0);
		}
		else if (!( vptr = cords_extract_location( yptr ) ))
		{
			uptr = occi_remove_response( uptr );
			yptr = occi_remove_response( yptr );
			return((char *) 0);
		}
		else if (!( vptr = occi_category_id( vptr ) ))
		{
			uptr = occi_remove_response( uptr );
			yptr = occi_remove_response( yptr );
			return((char *) 0);
		}
		else
		{
			uptr = occi_remove_response( uptr );
			sprintf(buffer,"%s/%s/%s",yptr->host,category,vptr);
			yptr = occi_remove_response( yptr );
			return( allocate_string( buffer ) );
		}		
	}
}

/*	---------------------------------------------------------	*/
/*		      l o g o u t _ o c c i _ u s e r 			*/
/*	---------------------------------------------------------	*/
public	char  *	logout_occi_user( char * user,	char * password, char * agent, char * token, char * tls )
{
	struct	occi_client * cptr;
	char *	category = _CORDS_AUTHORISATION;

	if ( token )
	{
		cords_delete_instance( token, category, agent,tls );
		token = liberate( token );
	}
	return( token );
}

	
#endif	/* _occi_login */
	/* ----------- */

