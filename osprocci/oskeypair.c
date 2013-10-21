#ifndef	_os_key_pair_c
#define	_os_key_pair_c

private	int	use_keypairs=1;

#define	_REMOTE_SSH_EXEC "ssh -o StrictHostKeyChecking=no -q"

/*	-------------------------------------------------	*/
/*	 s s h _ l a u n c h _ u s i n g _ k e y p a i r	*/
/*	-------------------------------------------------	*/
public	int	ssh_launch_using_keypair( char * username, char * keyfile, char * hostname, char * command )
{
	int	status=0;
	char *	syntax=(char *) 0;
	if ((!( username))
	||  (!( keyfile ))
	||  (!( hostname))
	||  (!( command )))
		return( 118 );
	if (!( syntax = allocate( 
			strlen( _REMOTE_SSH_EXEC) + 
			strlen( keyfile 	) + 
			strlen( username	) +
			strlen( hostname	) +
			strlen( command 	) + 64 ) ))
		return( 27 );
	else
	{
		sprintf(syntax,"%s -i %s %s@%s %c%s%c",
			_REMOTE_SSH_EXEC,
			keyfile,username,hostname,
			0x0022, command, 0x0022 );
		status = system( syntax );
		syntax = liberate( syntax );
		if ( status != -1 )
			return( 0 );
		else	return( errno );
	}
} 

/*	-------------------------------------------------	*/
/*	  o s _ l a u n c h _ u s i n g _ k e y p a i r		*/
/*	-------------------------------------------------	*/
public	int	os_launch_using_keypair( struct openstack * pptr, char * username, char * command )
{
	if (!( pptr ))
		return( 118 );
	else if (!( command ))
		return( 118 );
	else if (!( pptr->hostname ))
		return( 118 );
	else if (!( pptr->keyfile ))
		return( 118 );
	else	return( ssh_launch_using_keypair( username, pptr->keyfile, pptr->hostname, command ) );
}

/*	-----------------------------------------------		*/
/*	g e n e r a t e _ p r i v a t e _ k e y f i l e		*/
/*	-----------------------------------------------		*/
private	int	generate_private_keyfile( char * pathname, char * kptr )
{
	FILE *	h;
	int	status=0;
	int	mode = 0;
	int	bytes=64;
	if (!( rest_valid_string( kptr ) ))
		return( 0 );
	else if (!( rest_valid_string( pathname ) ))
		return( 0 );
	else if (!( h = fopen( pathname, "w" ) ))
		return( 0 );
	else
	{
		while ( *kptr != 0 )
		{
			switch ( mode )
			{
			case	0	:
				if ( *kptr != '-' )
					mode++;
				else	fprintf(h,"%c",*(kptr++));
				continue;
			case	1	:
				if ( *kptr == '-' )
					mode++;
				else	fprintf(h,"%c",*(kptr++));
				continue;
			case	2	:
				if ( *kptr != '-' )
					mode++;
				else	fprintf(h,"%c",*(kptr++));
				continue;
			case	3	:
				if ( *kptr != 'n' )
				{
					status = 0;
					break;
				}
				else
				{
					kptr++;
					fprintf(h,"\n");
					mode++;
					bytes=64;
				}
				continue;
			case	4	:
				if (!( bytes ))
				{
					if ( *kptr != 'n' )
					{
						status = 0;
						break;
					}
					else
					{
						kptr++;
						fprintf(h,"\n");
						bytes = 64;
					}
				}
				else if ( *kptr == '=' )
					mode++;
				else
				{
					fprintf(h,"%c",*(kptr++));
					bytes--;
				}
				continue;
			case	5	:
				if ( *kptr != '=' )
					mode++;
				else	fprintf(h,"%c",*(kptr++));
				continue;
			case	6	:
				if ( *kptr != 'n' )
				{
					status = 0;
					break;
				}
				else
				{
					kptr++;
					fprintf(h,"\n");
					mode++;
				}
				continue;
			case	7	:
				if ( *kptr != '-' )
					mode++;
				else	fprintf(h,"%c",*(kptr++));
				continue;
			case	8	:
				if ( *kptr == '-' )
					mode++;
				else	fprintf(h,"%c",*(kptr++));
				continue;
			case	9	:
				if ( *kptr != '-' )
					mode++;
				else	fprintf(h,"%c",*(kptr++));
				continue;
			case	10	:
				if ( *kptr != 'n' )
					status = 0;
				else
				{
					kptr++;
					status = 1;
					fprintf(h,"\n");
				}
				break;
			}
		}
		fclose(h);
		if (!( status )) unlink( pathname );
		return(status);
	}
}

/*	-----------------------------------------------		*/
/*	r e s o l v e _ c o n t r a c t _ k e y p a i r		*/
/*	-----------------------------------------------		*/
public	int	resolve_contract_keypair(struct os_subscription * sptr, struct openstack * pptr)
{
	char *	kptr=(char *) 0;
	char *	filename=(char *) 0;
	char	pathname[1024];
	char 	buffer[1024];
	char *	operator=(char *) 0;
	char *	account=(char *) 0;
	struct	os_response * rptr=(struct os_response *) 0;
	struct	data_element * eptr=(struct data_element *) 0;
	int	iteration=0;
	int	mode=0;
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

		if (!( rptr = os_get_keypair( sptr, buffer ) ))
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
			else	break;
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
	else if (( rptr->response->status > 299 ))
	{
		rptr = liberate_os_response( rptr );
		return( 1147 );
	}

	/* ----------------------------------------------- */
	/* retrieve the private key data from the response */
	/* ----------------------------------------------- */
	if ((!( rptr->jsonroot ))
	||  (!( eptr = json_element( rptr->jsonroot, "private_key" ) ))
	||  (!( kptr = eptr->value )))
	{
		rptr = liberate_os_response( rptr );
		return( 1178 );
	}
 
	/* ------------------------------------------------------ */
	/* the keypair has been created and must be saved to file */
	/* ------------------------------------------------------ */
	sprintf(pathname,"security/%s.key",pptr->keyname);
	if (!( generate_private_keyfile( pathname, kptr ) ))
	{
		rptr = liberate_os_response( rptr );
		return( 1148 );
	}
	else
	{
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


