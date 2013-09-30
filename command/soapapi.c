#ifndef	_soap_api_c
#define	_soap_api_c

/*	-----------------------------------------------		*/
/*	    i n v o k e _ s o a p _ r e q u e s t 		*/
/*	-----------------------------------------------		*/
private	int	invoke_soap_request( char * action, char * host, char * wsdl, char * filename )
{
	struct	rest_header * hptr;
	struct	rest_response * rptr;
	struct	xml_element * eptr;
	char 	buffer[1024];

	if ( check_verbose() )
		printf("SOAP API REQUEST POST %s %s \n",host, filename);

	sprintf(buffer,"%s%s",host,action);

	if ( check_verbose() )
		printf("SOAP ACTION %s \n",buffer);

	if (!( hptr = rest_create_header( "soapAction", buffer )))
		return( 0 );
	else if (!( rptr = rest_client_post_request( host, default_tls(), get_default_agent(), filename, hptr ) ))
		return( 0 );
	else
	{
		if ( check_verbose() )
			printf("SOAP API RESPONSE %u %s \n",rptr->status, (rptr->message ? rptr->message : "NO MESSAGE"));
		if (( rptr->body )
		&&  ((eptr = document_parse_file( rptr->body )) != (struct xml_element *) 0))
		{
			if ( check_verbose() )
				document_show_element( eptr, 0 );
			eptr = document_drop( eptr );
		}
		liberate_rest_response( rptr );
		return( 0 );
	}
}

/*	-----------------------------------------------		*/
/*	i n v o k e _ s o a p _ r e s o l v e r _ a p i		*/
/*	-----------------------------------------------		*/
private	int	invoke_soap_resolver_api( char * category, int asynch )
{
	char *	message;
	FILE *	h;

	if ( check_verbose() )
		printf("SOAP API Resolver %s \n",category);

	if (!( message = rest_temporary_filename("xml") ))
		return(0);
	else if (!( h = fopen( message, "w" ) ))
		return(0);
	else
	{
		soap_message_header( h, "ResolveCategory" );
		fprintf(h,"<command>resolver</command>\n");
		fprintf(h,"<category>%s</category>\n",category);
		soap_message_footer( h, "ResolveCategory" );
		fclose(h);
		return( invoke_soap_request( "Resolver", soap, wsdl, message ) );
	}
}

/*	-------------------------------------------	*/
/*	i n v o k e _ s o a p _ p a r s e r _ a p i	*/
/*	-------------------------------------------	*/
private	int	invoke_soap_parser_api( char * type, char * filename, int asynch )
{
	char *	message;
	FILE *	h;

	if ( check_verbose() )
		printf("SOAP API Parser %s %s \n",type,filename);
	if (!( message = rest_temporary_filename("xml") ))
		return(0);
	else if (!( h = fopen( message, "w" ) ))
		return(0);
	else
	{
		if ( asynch )
		{
			if (!( strcasecmp( type, "MANIFEST" ) ))
				type = "AsynchParseManifest";
			else	type = "AsynchParseSLA";
		}
		else
		{
			if (!( strcasecmp( type, "MANIFEST" ) ))
				type = "ParseManifest";
			else	type = "ParseSLA";
		}
		soap_message_header( h, type );
		fprintf(h,"<command>parser</command>\n");
		soap_inline_xml(h,filename);
		soap_message_footer( h, type );
		fclose(h);
		return( invoke_soap_request( type, soap, wsdl, message ) );
	}
}

/*	-------------------------------------------	*/
/*	i n v o k e _ s o a p _ b r o k e r _ a p i	*/
/*	-------------------------------------------	*/
private	int	invoke_soap_broker_api( char * type, char * filename, int asynch )
{
	char *	message;
	FILE *	h;
	if ( check_verbose() )
		printf("SOAP API Broker %s %s \n",type,filename);
	if (!( message = rest_temporary_filename("xml") ))
		return(0);
	else if (!( h = fopen( message, "w" ) ))
		return(0);
	else
	{
		if ( asynch )
			type = "AsynchBrokerSLA";
		else	type = "BrokerSLA";
		soap_message_header( h, type );
		fprintf(h,"<command>broker</command>\n");
		soap_inline_xml(h,filename);
		soap_message_footer( h, type );
		fclose(h);
		return( invoke_soap_request( type, soap, wsdl, message ) );
	}
}

/*	---------------------------------------------	*/
/*	i n v o k e _ s o a p _ s e r v i c e _ a p i	*/
/*	---------------------------------------------	*/
private	int	invoke_soap_service_api( char * action, char * service, int asynch )
{
	char *	type;
	char *	message;
	FILE *	h;
	if ( check_verbose() )
		printf("SOAP API Service %s %s \n",action, service);
	if (!( message = rest_temporary_filename("xml") ))
		return(0);
	else if (!( h = fopen( message, "w" ) ))
		return(0);
	else
	{
		if ( asynch )
			type = "AsynchServiceAction";
		else	type = "ServiceAction";
			
		soap_message_header( h, type );
		fprintf(h,"<command>service</command>\n",action);
		fprintf(h,"<service>%s</service>\n",service);
		fprintf(h,"<action>%s</action>\n",action);
		soap_message_footer( h, type );
		fclose(h);
		return( invoke_soap_request( type, soap, wsdl, message ) );
	}
}

/*	-------------------------------------------	*/
/*	i n v o k e _ s o a p _ s c r i p t _ a p i	*/
/*	-------------------------------------------	*/
private	int	invoke_soap_script_api( char * script, char * parameters, int asynch )
{
	char *	type;
	char *	message;
	FILE *	h;

	if ( check_verbose() )
		printf("SOAP API Script %s %s \n",script,(parameters ? parameters : "" ));

	if (!( message = rest_temporary_filename("xml") ))
		return(0);
	else if (!( h = fopen( message, "w" ) ))
		return(0);
	else
	{
		if ( asynch )
			type = "AsynchRunScript";
		else	type = "RunScript";
			
		soap_message_header( h, type );
		fprintf(h,"<command>script</command>\n");
		fprintf(h,"<parameters>%s</parameters>\n",parameters);
		fprintf(h,"<script>\n");
		soap_inline_file(h,script);
		fprintf(h,"</script>\n");
		soap_message_footer( h, type );
		fclose(h);
		return( invoke_soap_request( type, soap, wsdl, message ) );
	}
}

/*	-------------------------------------------	*/
/*	i n v o k e _ s o a p _ r e s u l t _ a p i	*/
/*	-------------------------------------------	*/
private	int	invoke_soap_result_api( char * identity )
{
	char *	type="AsynchResult";
	char *	message;
	FILE *	h;
		
	if ( check_verbose() )
		printf("SOAP API Result %s \n",identity);

	if (!( message = rest_temporary_filename("xml") ))
		return(0);
	else if (!( h = fopen( message, "w" ) ))
		return(0);
	else
	{
		soap_message_header( h, type );
		fprintf(h,"<identity>%s</identity>\n",identity);
		soap_message_footer( h, type );
		fclose(h);
		return( invoke_soap_request( type, soap, wsdl, message ) );
	}
}

/*	-------------------------------------	*/
/*	    i n v o k e _ s o a p _ a p i	*/
/*	-------------------------------------	*/
private	int	invoke_soap_api( char * command, char * subject, char * option, int asynch )
{
	if (!( strcmp( command, "resolver" ) ))
		return( invoke_soap_resolver_api( subject, asynch ) );
	else if (!( strcmp( command, "parser" ) ))
		return( invoke_soap_parser_api( subject, option, asynch ) );
	else if (!( strcmp( command, "broker" ) ))
		return( invoke_soap_broker_api( subject, option, asynch ) );
	else if (!( strcmp( command, "service" ) ))
		return( invoke_soap_service_api( subject, option, asynch ) );
	else if (!( strcmp( command, "script" ) ))
		return( invoke_soap_script_api( subject, option, asynch ) );
	else if (!( strcmp( command, "result" ) ))
		return( invoke_soap_result_api( subject ) );
	else
	{
		printf("SOAP API: Incorrect Command: \"%s\" \n",command);
		return(0);
	}
}

	/* ----------- */
#endif	/* _soap_api_c */
	/* ----------- */

