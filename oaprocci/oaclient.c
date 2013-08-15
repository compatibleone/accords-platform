#ifndef	_oaclient_c
#define	_oaclient_c

#include "oaclient.h"
#include "encode64.h"
#include "onapp_helpers.h"

private	int	rate_recovery=120;	/* time to wait for rate limiting recovery */

#define BASE64_ENCODE_ONAPP_AUTHORIZATION

char * create_onapp_authentication(char * user, char * password)
{
	static size_t const buffersize = 1024;
	char buffer[buffersize];
	char encoded[buffersize];
	char * rstrip;
	int nchars;
	char * authentication = 0;

	if ( user == NULL )
		return authentication;
	if ( password == NULL )
		return authentication;

	nchars = snprintf(buffer,buffersize, "%s:%s", user, password);

#ifdef BASE64_ENCODE_ONAPP_AUTHORIZATION
	// No check for memory overrun in encoded...
	if (!( EncodeBase64(encoded, buffer, strlen( buffer )) ))
		return( authentication );

	int const lenEncoded = strlen(encoded);

	/*  POST on OnApp has been seen to fail with excessive newlines in authentication header. */
	rstrip = encoded + lenEncoded - 1;
	while (*rstrip == '\n') {
		*rstrip = '\0';
		--rstrip;
	}

	/* Need to be in format "Basic <64bit encoded auth token>" for ControlPanel 3. */
	snprintf(buffer, buffersize, "Basic %s", encoded);
	authentication = allocate_string(buffer);
#else // !BASE64_ENCODE_ONAPP_AUTHORIZATION
	authentication = allocate_string(buffer);
#endif // BASE64_ENCODE_ONAPP_AUTHORIZATION
	return authentication;
}


/*	------------------------------------------------------------ */
/*			o a _ a u t h e n t i c a t e ()	 */
/*	------------------------------------------------------------ */
public	struct	rest_header   *	oa_authenticate	(struct oa_config *poaconfig)
{
	struct rest_header * hptr=(struct rest_header * ) 0;
	struct os_response * rptr;
	struct url * uptr;
	char * nptr;
	int status;
	char * eptr;
	char buffer[256];

	char * authentication = create_onapp_authentication(poaconfig->user, poaconfig->password);
	if (authentication == NULL)
		return hptr;

	hptr = rest_create_header( "Authorization", authentication);

	return hptr;
}

/*	------------------------------------------------------------ */
/*		 		o a _ c h e c k 		 */
/*	------------------------------------------------------------ */
private	struct	oa_response * oa_check( struct rest_response * aptr )
{
	static char const virtual_machines_text[] = "/virtual_machines/";
	static size_t const size_virtual_machines_text = sizeof(virtual_machines_text) / sizeof(*virtual_machines_text);
	static char const virtual_machines_dummy_json[] = "{\"virtual_machine\": {\"identifier\": \"%s\" } }";
	// -2 because we're going to replace the %s with the id, -1 to remove the NULL terminator.
	static size_t const size_virtual_machines_dummy_json = (sizeof(virtual_machines_dummy_json) - 3) / sizeof(*virtual_machines_dummy_json);
	struct	oa_response * rptr=(struct oa_response *) 0;
	struct	rest_header * hptr=(struct rest_header *) 0;
	struct	url * purl=(struct url *) 0;
	char * vmid;
	size_t size_vmid;
	size_t size_format_vmid;

	if ( aptr )
	{
		//if ( check_verbose() )
		if (1)
		{
			printf("===OnApp Client Response : %s %u %s===\n",aptr->version,aptr->status,aptr->message);

			FILE *h = stdout;
			fprintf(h, " Type:    %d\n", aptr->type);
			for (hptr=aptr->first;
				 hptr != (struct rest_header *) 0;
				 hptr = hptr->next )
			{
				if (!( hptr->name ))
					continue;
				else
					fprintf(h, "   %s: %s\n",hptr->name,hptr->value);
			}
			fprintf(h, " Body:\n '%s'\n", aptr->body );
			if (aptr->type == _FILE_BODY)
			{
				write_filecontent_to_filehandle(aptr->body, stdout, " Body:\n '", "'\n");
			}
		}
	}
	else
		return ( rptr );

	if (!( rptr = allocate( sizeof( struct oa_response ) ) ))
		return( rptr );
	else
	{
		rptr->nature = _TEXT_NONE;
		rptr->content= (char *) 0;
		rptr->xmlroot = (struct xml_element *) 0;
		rptr->jsonroot = (struct data_element *) 0;
		rptr->response = aptr;
		if (!( aptr->body )) {
			// Iiiiiii ain't go nooooo booooddyyyyyy.

			if (( hptr = rest_resolve_header( aptr->first, _HTTP_LOCATION ) )) {
				if ( hptr->value != NULL ) {
					if (!( purl = analyse_url( hptr->value ) ))
						return rptr;
					else if (!( purl = validate_url( purl )))
						return rptr;
					else if ( strncmp(purl->object, virtual_machines_text, size_virtual_machines_text - 1) != 0 ) {
						purl = liberate_url( purl );
						return rptr;
					}
					else {
						size_vmid = size_virtual_machines_dummy_json + strlen(purl->object + size_virtual_machines_text - 1) + 1;
						if ( !(vmid = allocate(size_vmid)) ) {
							purl = liberate_url( purl );
							return rptr;
						}
						else {
							size_format_vmid = snprintf(vmid, size_vmid
									, virtual_machines_dummy_json
									, (purl->object + size_virtual_machines_text - 1)
							);

							purl = liberate_url( purl );

							if ( size_format_vmid < (size_vmid - 1) ) { // If failed to make up some convincing looking JSON
								vmid = liberate ( vmid );
								printf( "Failed to concoct JSON for OnApp CreateVM\n");

							} // Ends if failed to make up some convincing looking JSON
							else { // Else concocted some JSON
								rptr->content = liberate(rptr->content);
								rptr->content = allocate_string("text/json");
								rptr->nature = _TEXT_JSON;
								rptr->jsonroot = json_parse_string ( vmid );
								vmid = liberate ( vmid );
								return rptr;
							} // Ends else concocted some JSON
						}
					}
				}
			}
			return(rptr);
		}

		if (!( hptr = rest_resolve_header( aptr->first, "Content-Type" ) ))
			return(rptr);
		else if (!( rptr->content = allocate_string( hptr->value ) ))
			return( rptr );
		else if ((!( strprefix( rptr->content, "text/json" 	)))
		     ||  (!( strprefix( rptr->content, "application/json" 	)))
		     ||  (!( strprefix( rptr->content, "x-application/json"))))
		{
			rptr->nature = _TEXT_JSON;
			rptr->jsonroot = json_parse_file( aptr->body );
			return( rptr );
		}
		else if ((!( strprefix( rptr->content, "text/xml" 		)))
		     ||  (!( strprefix( rptr->content, "application/xml" 	)))
		     ||  (!( strprefix( rptr->content, "x-application/xml"))))
		{
			rptr->nature = _TEXT_XML;
			rptr->xmlroot = document_parse_file( aptr->body );
			return( rptr );
		}
		else	return( rptr );

	}
}

/*	--------------------------------------------------------- */
/*		o a _ c l i e n t _ r a t e _ l i m i t e d	 */
/*	--------------------------------------------------------- */
private	int	oa_client_rate_limited( struct rest_response * rptr )
{
	if (!( rptr ))
		return(0);
	else if ( rptr->status < 400 )
		return(0);
	else if ( rptr->status != 413 )
		return(0);
	{
		/* -------------------------- */
		/* rate limiting is in effect */
		/* -------------------------- */
		rptr = liberate_rest_response( rptr );
		sleep(rate_recovery);
		return( 1 );
	}
}


/*	------------------------------------------------------------ */
/*		 o a _ c l i e n t _ p o s t _ r e q u e s t	 */
/*	------------------------------------------------------------ */
public	struct	oa_response *
	oa_client_post_request(
		char * target, char * tls, char * nptr, char * filename, struct rest_header * hptr )
{
	struct	rest_response * rptr;
	struct	rest_header   * copy=(struct rest_header *) 0;
	char                  * body=(char *) 0;

	if (1)
	{
		struct rest_header *headeriter;
		fprintf(stdout, "===OnApp Request===\n");
		fprintf(stdout, "%s (%s) filename='%s'\n", target, nptr, filename);
		for (headeriter=hptr; headeriter != NULL; headeriter=headeriter->next)
		{
			fprintf(stdout, " %s : %s\n", headeriter->name, headeriter->value);
		}
		write_filecontent_to_filehandle(filename, stdout, "Body:\n'", "'\n");
	}

	if (( hptr )
	&&  (!( copy = rest_duplicate_headers( hptr ) )))
		return((struct oa_response *) 0);

	if (( filename )
	&&  (!( body = allocate_string( filename ) )))
	{
		copy = liberate_rest_headers( copy );
		return((struct oa_response *) 0);
	}

	if (tls != NULL && *tls == '\0')
	{
		tls = NULL;
	}

	while (( rptr = rest_client_post_request( target, tls, nptr, filename, hptr )) != (struct rest_response *) 0)
	{
		if (!( oa_client_rate_limited( rptr ) ))
			break;
		if ( copy  )
			if (!( hptr = rest_duplicate_headers( copy ) ))
				return((struct oa_response *) 0);
		if ( body )
			if (!( filename = allocate_string( body ) ))
				return((struct oa_response *) 0);

	}

	if (( filename ) &&  ( body ))
		body = liberate( body );

	if (( hptr ) &&  ( copy ))
		copy = liberate_rest_headers( copy );

	return( oa_check( rptr ) );
}

/*	------------------------------------------------------------ */
/*		 o a _ c l i e n t _ d e l e t e _ r e q u e s t         */
/*	------------------------------------------------------------ */
public	struct	oa_response *
	oa_client_delete_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	struct	rest_response * rptr;
	struct	rest_header   * copy=(struct rest_header *) 0;

	if (1)
	{
		struct rest_header *headeriter;
		fprintf(stdout, "===OnApp Request===\n");
		fprintf(stdout, "%s (%s)\n", target, nptr);
		for (headeriter=hptr; headeriter != NULL; headeriter=headeriter->next)
		{
			fprintf(stdout, " %s : %s\n", headeriter->name, headeriter->value);
		}
	}

	if (( hptr )
	&&  (!( copy = rest_duplicate_headers( hptr ) )))
		return((struct oa_response *) 0);

	if (tls != NULL && *tls == '\0')
	{
		tls = NULL;
	}

	while (( rptr = rest_client_delete_request( target, tls, nptr, hptr )) != (struct rest_response *) 0)
	{
		if (!( oa_client_rate_limited( rptr ) ))
			break;
		if ( copy  )
			if (!( hptr = rest_duplicate_headers( copy ) ))
				return((struct oa_response *) 0);

	}

	if (( hptr ) &&  ( copy ))
		copy = liberate_rest_headers( copy );

	return( oa_check( rptr ) );
}

/*	------------------------------------------------------------ */
/*			o a _ c r e a t e _  o p e r a t i o n	 */
/*	------------------------------------------------------------ */
private	struct	oa_response *	oa_create_operation(struct oa_config * poaconfig, char * buffer, char * filename )
{
	struct	oa_response	*	rptr=(struct oa_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	char * agent_hackage = poaconfig->agent ? poaconfig->agent : _CORDS_OA_AGENT;

	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	if (!( hptr = oa_authenticate(poaconfig) ))
		return( rptr );
	else if (!( uptr = analyse_url( poaconfig->host )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = oa_client_post_request( nptr, poaconfig->tls, agent_hackage, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------ */
/*			o a _ d e l e t e _  o p e r a t i o n	 */
/*	------------------------------------------------------------ */
private	struct	oa_response * oa_delete_operation(struct oa_config * poaconfig, char * buffer)
{
	struct	oa_response	*	rptr=(struct oa_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	char * agent_hackage = poaconfig->agent ? poaconfig->agent : _CORDS_OA_AGENT;

	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	if (!( hptr = oa_authenticate(poaconfig) ))
		return( rptr );
	else if (!( uptr = analyse_url( poaconfig->host )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = oa_client_delete_request( nptr, poaconfig->tls, agent_hackage, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------ */
/*			o a _ c r e a t e _  s e r v e r                     */
/*	------------------------------------------------------------ */
struct	oa_response * oa_create_server(struct oa_config * poaconfig, char * filename )
{
	char	buffer[1024];
	sprintf(buffer,"/virtual_machines.json");
	return( oa_create_operation( poaconfig,  buffer, filename ) );
}

/*	------------------------------------------------------------ */
/*	        o a _ s t a r t _ s e r v e r                        */
/*	------------------------------------------------------------ */
struct oa_response * oa_start_server(struct oa_config * poaconfig, struct onapp *ponapp)
{
	char buffer[1024];
	struct oa_response *response = (struct oa_response *)NULL;

	if (!string_is_null_or_empty(ponapp->local_id)) {
		// https://<some.cp>/virtual_machines/<some_id>/startup.json
		snprintf(buffer, 1024, "/virtual_machines/%s/startup.json", ponapp->local_id);
		response = oa_create_operation (poaconfig, buffer, NULL);
	}

	return response;
}

/*	------------------------------------------------------------ */
/*			o a _ s t o p _ s e r v e r                          */
/*	------------------------------------------------------------ */
struct oa_response * oa_stop_server(struct oa_config * poaconfig, struct onapp *ponapp)
{
	char buffer[1024];
	struct oa_response *response = (struct oa_response *)NULL;

	if (!string_is_null_or_empty(ponapp->local_id)) {
		// https://<some.cp>/virtual_machines/<some_id>/stop.json
		snprintf(buffer, 1024, "/virtual_machines/%s/stop.json", ponapp->local_id);
		response = oa_create_operation (poaconfig, buffer, NULL);
	}

	return response;
}

/*	------------------------------------------------------------ */
/*	        o a _ s h u t d o w n _ s e r v e r                  */
/*	------------------------------------------------------------ */
struct oa_response * oa_shutdown_server(struct oa_config * poaconfig, struct onapp *ponapp)
{
	char buffer[1024];
	struct oa_response *response = (struct oa_response *)NULL;

	if (!string_is_null_or_empty(ponapp->local_id)) {
		// https://<some.cp>/virtual_machines/<some_id>/shutdown.json
		snprintf(buffer, 1024, "/virtual_machines/%s/shutdown.json", ponapp->local_id);
		response = oa_create_operation (poaconfig, buffer, NULL);
	}

	return response;
}

/*	------------------------------------------------------------ */
/*	        o a _ r e s t a r t _ s e r v e r                    */
/*	------------------------------------------------------------ */
struct oa_response * oa_restart_server(struct oa_config * poaconfig, struct onapp *ponapp)
{
	char buffer[1024];
	struct oa_response *response = (struct oa_response *)NULL;

	if (!string_is_null_or_empty(ponapp->local_id)) {
		// https://<some.cp>/virtual_machines/<some_id>/reboot.json
		snprintf(buffer, 1024, "/virtual_machines/%s/reboot.json", ponapp->local_id);
		response = oa_create_operation (poaconfig, buffer, NULL);
	}

	return response;
}

/*	------------------------------------------------------------ */
/*	        o a _ d e s t r o y _ s e r v e r                      */
/*	------------------------------------------------------------ */
struct oa_response * oa_destroy_server(struct oa_config * poaconfig, struct onapp *ponapp)
{
	char buffer[1024];
	struct oa_response *response = (struct oa_response *)NULL;

	if (!string_is_null_or_empty(ponapp->local_id)) {
		// https://<some.cp>/virtual_machines/<some_id>.json?convert_last_backup=0&destroy_all_backups=1
		snprintf(buffer, 1024, "/virtual_machines/%s.json?convert_last_backup=0&destroy_all_backups=1", ponapp->local_id);
		response = oa_delete_operation (poaconfig, buffer);
	}

	return response;
}

/*	------------------------------------------------------------ */
/*		l i b e r a t e _ o a _ r e s p o n s e	                 */
/*	------------------------------------------------------------ */
public	struct oa_response * liberate_oa_response( struct oa_response * rptr )
{
	if ( rptr )
	{
		if ( rptr->response )
			rptr->response = liberate_rest_response( rptr->response );
		if ( rptr->jsonroot )
			rptr->jsonroot = drop_data_element( rptr->jsonroot );
		if ( rptr->xmlroot )
			rptr->xmlroot = document_drop( rptr->xmlroot );
		if ( rptr->content )
			liberate( rptr->content );
		liberate( rptr );
	}
	return((struct oa_response *) 0);
}

char * oa_create_server_json_request(
		  struct oa_config * poaconfig
		, struct onapp *ponapp
		)
{
	static long int const default_template = 1;

	char	encoded[8192];
	char *	eptr=encoded;
	char *	filename;
	char * temp;
	char * end_of_temp;
	long int template;

	FILE *	h;
	int	bytes;
	struct	rest_header * hptr;
	//if (!( hptr = oa_authenticate(poaconfig) ))
	if ( 0 )
		return((char *) 0);
	else if (!( filename = rest_temporary_filename("json")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		// TODO Logging
		// [CONNECT-195]
		// Template is image name after ':'
		if (ponapp->image == NULL) // If no image
		{
			template = default_template;

		} // Ends if no image
		else // Else image
		{
			temp = strchr(ponapp->image, ':');

			if (temp == NULL) // If no colon
			{
				template = default_template;

			} // Ends if no colon
			else // Else colon
			{
				++temp;
				if (*temp == '\0') // If nothing after colon
				{
					// LOG
					template = default_template;

				} // Ends if nothing after colon
				else // Else string after colon
				{
					if (strncmp(temp, "default", 7) == 0) // If default
					{
						// "default" means something special.
						template = default_template;

					} // Ends if default
					else // Else not default
					{
						end_of_temp = NULL;
						template = strtol(temp, &end_of_temp, 10);

						if (end_of_temp == temp) // If no digits
						{
							// LOG
							template = default_template;

						} // Ends if no digits
						else if (*end_of_temp != '\0') // Else non-digits
						{
							// LOG
							template = default_template;

						} // Ends else non-digits
					} // Ends else not default
				} // Ends if string after colon
			} // Ends else colon
		} // Ends else image
		// https://onappdev.atlassian.net/wiki/display/3api/Create+VM.
		fprintf(h, "{\n  \"virtual_machine\": {\n");
		// onapp fields not currently sent to or retrieved from OnApp
		//	char *	firewall;
		//	char *	image;
		//	char *	agent;
		//	char *	user_id;
		//	char *	zone;
		//	char *	access;
		//	char *	build_state;
		//	int	state;
		//	char *	location;
		//	char *	provider;
		//	int	created_at;
		//	char *	profile;
		//	char *	description;
		//	char *	price;
		//	char *	password;
		//	char *	ipaddress;
		//	char *	account;
		//	char *	name;
		//	char *	region;
		//	char *	username;
		//	char *	status;
		//	char *	host_name;
		//	char *	original;

		// For KVM hypervisor the CPU priority value is always 100. For XEN, set a custom value. The default value for XEN is 1.
		fprintf(h, "    \"cpus\": \"%d\",\n", ponapp->cpus);
		fprintf(h, "    \"hostname\": \"%s\",\n", ponapp->hostname); // Make this up ?
		fprintf(h, "    \"memory\": \"%s\",\n", ponapp->memory);
		// TEMPLATE ???
		fprintf(h, "    \"template_id\": \"%ld\",\n", template);
		fprintf(h, "    \"primary_disk_size\": \"%s\",\n", ponapp->disk_size);
		fprintf(h, "    \"rate_limit\": \"%s\",\n", "none"); // TODO
		// [CONNECT-130] - Include hypervisor zone, network zone, and storage zone.
		fprintf(h, "    \"hypervisor_group_id\": \"%s\",\n", poaconfig->local_id);
		fprintf(h, "    \"data_store_group_primary_id\": \"%d\",\n", poaconfig->store_zone_id);
		fprintf(h, "    \"primary_network_group_id\": \"%d\",\n", poaconfig->net_zone_id);

		// [CONNECT-194] - onapp_extras.
		fprintf(h, "    \"initial_root_password_encryption_key\": \"%s\",\n", ponapp->initial_root_password_encryption_key);
		fprintf(h, "    \"initial_root_password\": \"%s\",\n", ponapp->initial_root_password);
		fprintf(h, "    \"required_ip_address_assignment\": \"%d\",\n", ponapp->required_ip_address_assignment);
		fprintf(h, "    \"type_of_format\": \"%s\",\n", ponapp->disk_type_of_format);
		fprintf(h, "    \"label\": \"%s\",\n", ponapp->label);
		fprintf(h, "    \"required_automatic_backup\": \"%d\",\n", ponapp->required_automatic_backup);
		fprintf(h, "    \"swap_disk_size\": \"%s\",\n", ponapp->swap_size);
		fprintf(h, "    \"required_virtual_machine_build\": \"%d\",\n", ponapp->required_virtual_machine_build);
		fprintf(h, "    \"required_virtual_machine_startup\": \"%d\",\n", ponapp->required_virtual_machine_startup);
		fprintf(h, "    \"cpu_shares\": \"%s\",\n", ponapp->cpu_shares);
		fprintf(h, "    \"admin_note\": \"%s\"\n", ponapp->description);

//		fprintf(h, "    \"cpu_shares\": \"%d\",\n", 4);
//		fprintf(h, "    \"cpus\": \"%d\",\n", 1);
//		fprintf(h, "    \"hostname\": \"%s\",\n", "oaproccitest1");
//		fprintf(h, "    \"memory\": \"%d\",\n", 128);
//		fprintf(h, "    \"template_id\": \"%d\",\n", 1);
//		fprintf(h, "    \"primary_disk_size\": \"%d\",\n", 5);
//		fprintf(h, "    \"label\": \"%s\",\n", "aptest5");
//		fprintf(h, "    \"swap_disk_size\": \"%d\",\n", 1);
//		fprintf(h, "    \"required_automatic_backup\": \"%d\",\n", 1);
//		fprintf(h, "    \"rate_limit\": \"%s\",\n", "none");
//		fprintf(h, "    \"required_ip_address_assignment\": \"%d\",\n", 1);
//		fprintf(h, "    \"required_virtual_machine_build\": \"%d\",\n", 0);
//		fprintf(h, "    \"admin_note\": \"%s\",\n", "Admin comment");

//		fprintf(h, "    \"hypervisor_group_id\": \"%d\",\n", 2);
//		fprintf(h, "    \"allowed_hot_migrate\": \"%s\",\n", "true");
//		fprintf(h, "    \"note\": \"%s\",\n", "Note");
//		fprintf(h, "    \"required_virtual_machine_startup\": \"%d\",\n", 0);
//		fprintf(h, "    \"selected_ip_address_id\": \"%d\",\n", 1);
//		fprintf(h, "    \"primary_network_group_id\": \"%d\",\n", 1);
//		fprintf(h, "    \"primary_network_id\": \"%d\",\n", 1);
//		fprintf(h, "    \"data_store_group_swap_id\": \"%d\",\n", 7);
//		fprintf(h, "    \"initial_root_password_encryption_key\": \"%s\",\n", "asdfgh");
//		fprintf(h, "    \"initial_root_password\": \"%s\",\n", "qwe123");
//		fprintf(h, "    \"hypervisor_id\": \"%d\",\n", 1);
//		fprintf(h, "    \"licensing_server_id\": \"%d\",\n", 38);
//		fprintf(h, "    \"licensing_type\": \"%s\",\n", "kms");
//		fprintf(h, "    \"licensing_key\": \"%s\",\n", "keyexample");
//		fprintf(h, "    \"type_of_format\": \"%s\",\n", "ext4");
//		fprintf(h, "    \"primary_disk_min_iops\": \"%d\",\n", 600);
//		fprintf(h, "    \"swap_disk_min_iops\": \"%d\",\n", 600);
//		fprintf(h, "    \"data_store_group_primary_id\": \"%d\"\n", 2);

		fprintf(h, "  }\n}");
//		{
//		  "virtual_machine": {
//		    "hypervisor_group_id": "2",
//		    "allowed_hot_migrate": "true",
//		    "note": "Note",
//		    "admin_note": "Admin comment",
//		    "required_virtual_machine_startup": "0",
//		    "required_virtual_machine_build": "0",
//		    "required_ip_address_assignment": "1",
//		    "rate_limit": "none",
//		    "required_automatic_backup": "1",
//		    "selected_ip_address_id": "1",
//		    "primary_network_group_id": "1",
//		    "primary_network_id": "1",
//		    "data_store_group_swap_id": "7",
//		    "swap_disk_size": "1",
//		    "label": "aptest5",
//		    "template_id": "1",
//		    "memory": "256",
//		    "initial_root_password_encryption_key": "asdfgh",
//		    "initial_root_password": "qwe123",
//		    "hypervisor_id": "1",
//		    "hostname": "aptest2",
//		    "cpus": "1",
//		    "cpu_shares": "10",
//		    "licensing_server_id": "38",
//		    "licensing_type": "kms",
//		    "licensing_key": "keyexample",
//		    "primary_disk_size": "5",
//		    "type_of_format": "ext4",
//		    "primary_disk_min_iops": "600",
//		    "swap_disk_min_iops": "600",
//		    "data_store_group_primary_id": "2"
//		  }
//		}

		fclose(h);
		fprintf(stdout, "Created OnApp CreateVM json in: '%s'\n", filename);
		write_filecontent_to_filehandle(filename, stdout, "Created OnApp CreateVM json in: '", "'\n");
		return( filename );
	}
}

char * oa_create_server_request(
		  struct oa_config * poaconfig
		, struct onapp *ponapp)
{
	return oa_create_server_json_request(poaconfig, ponapp);
}

/*	------------------------------------------------------------ */
/*		o a _ a l l o c a t e _ c o n f i g	 */
/*	------------------------------------------------------------ */
public	struct	oa_config * oa_allocate_config()
{
	struct	oa_config * sptr;
	if (!( sptr = (struct oa_config *) allocate( sizeof( struct oa_config ) ) ))
		return( sptr );
	else
	{
	 	memset( sptr, 0, sizeof( struct oa_config ));
		return( sptr );
	}
}

public struct oa_config * oa_initialise_client( char * user, char * pass, char * host, char * agent, char * version, char * tls )
{
	struct oa_config * sptr=(struct oa_config *) 0;
	if (!( sptr = oa_allocate_config()))
		return( sptr );

	return( sptr );

	//return((struct oa_config *) 0);
}


	/* ----------- */
#endif 	/* _oaclient_c */
	/* ----------- */



