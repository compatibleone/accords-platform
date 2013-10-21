#ifndef	_os_key_pair_c
#define	_os_key_pair_c

private	int	use_keypairs=1;

/*	-----------------------------------------------		*/
/*	r e s o l v e _ c o n t r a c t _ k e y p a i r		*/
/*	-----------------------------------------------		*/
public	int	resolve_contract_keypair(struct os_subscription * sptr, struct openstack * pptr)
{
	char *	filename=(char *) 0;
	char	pathname[1024];
	char 	buffer[1024];
	char *	operator=(char *) 0;
	char *	account=(char *) 0;
	struct	os_response * rptr=(struct os_response *) 0;
	struct	data_element * eptr=(struct data_element *) 0;
	int	iteration=0;
	FILE *	h=(FILE *) 0;

	/* ---------------------------- */
	/* are use of keypairs required */
	/* ---------------------------- */
	if (!( use_keypairs ))
		return( 0 );

	/* --------------------------------------- */
	/* is a private key file already available */
	/* --------------------------------------- */
	else if ( rest_valid_string( pptr->keyfile ) != 0 )
		return( 0 );
	else 	pptr->keyname = liberate( pptr->keyname );

	if (!( operator = get_operator_profile() ))
		operator = "accords";
	else if (!( account = pptr->account ))
		account = "account";

	/* --------------------------------------- */
	/* create a key name and check for unicity */
	/* --------------------------------------- */
	while (!( rest_valid_string( pptr->keyname ) ))
	{

		sprintf(buffer,"%s-%s-%u",operator,pptr->accountname,++iteration);

		if (!( rptr = os_get_keypair( sptr, pptr->keyname ) ))
			return( 1156 );
		else if ( rptr->response->status == 200 )
		{
			sprintf(pathname,"security/%s.key",buffer);
			if (!( h = fopen( pathname, "r" ) ))
			{
				rptr = liberate_os_response( rptr );
				continue;
			}
			else
			{
				fclose(h);
				rptr = liberate_os_response( rptr );
				if (!( pptr->keyname = allocate_string( buffer ) ))
					return( 1127 );
				else if (!( pptr->keyfile = allocate_string( pathname ) ))
					return( 1127 );
				{
					autosave_openstack_nodes();
					return( 0 );
				}
			}
		}
		else
		{
			rptr = liberate_os_response( rptr );
			if (!( pptr->keyname = allocate_string( buffer ) ))
				return( 1127 );
			else	continue;
		}
	}

	/* --------------------------------------- */
	/* attempt to retrieve an existing keypair */
	/* --------------------------------------- */
	if (!( filename = os_keypair_request( sptr, pptr->keyname, (char *) 0) ))
		return( 1146 );
	else if (!( rptr = os_create_keypair( sptr, filename ) ))
		return( 1147 );
	else if (!( rptr->response ))
		return( 1147 );
	else if (!( rptr->response->status > 299 ))
	{
		rptr = liberate_os_response( rptr );
		return( 1147 );
	}

	/* ----------------------------------------------- */
	/* retrieve the private key data from the response */
	/* ----------------------------------------------- */
	if ((!( rptr->jsonroot ))
	||  (!( eptr = json_element( rptr->jsonroot, "private_key" ) )))
	{
		rptr = liberate_os_response( rptr );
		return( 1178 );
	}
 
	/* ------------------------------------------------------ */
	/* the keypair has been created and must be saved to file */
	/* ------------------------------------------------------ */
	sprintf(pathname,"security/%s.key",pptr->keyname);
	if (!( h = fopen( pathname, "w" ) ))
	{
		rptr = liberate_os_response( rptr );
		return( 1148 );
	}
	else
	{
		/* ------------------------------ */
		/* write private key data to file */
		/* ------------------------------ */
		fprintf(h,"%s\n",eptr->value);
		fclose(h);
		if (!( pptr->keyfile = allocate_string( pathname ) ))
		{
			unlink( buffer );
			rptr = liberate_os_response( rptr );
			return( 1149 );
		}
		else
		{
			rptr = liberate_os_response( rptr );

			/* --------------------------------- */
			/* set ultra restrictive permissions */
			/* --------------------------------- */
			chmod(pptr->keyfile,0400);
			autosave_openstack_nodes();
			return( 0 );
		}
	}
}

#endif	/* _os_key_pair_c */
	/* -------------- */


