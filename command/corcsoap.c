#ifndef	_corcs_soap_c
#define	_corcs_soap_c
#include "command.h"
/*	-----------------------------------	*/
/*	   c o r c s _ s o a p _ w s d l 	*/
/*	-----------------------------------	*/
/*	Reminder:				*/
/*	Service defines ports which 		*/
/*	define their binding which 		*/
/*	in turn defines their port		*/
/*	Type which defines the operation	*/
/*	which indicate their messages		*/
/*	which are described in terms of 	*/
/*	their parts.				*/
/*	S->P->B->PT->O->M->Part			*/
/*	------------------------------------	*/
private	char *	corcs_soap_wsdl(char * host)
{
	char * filename;
	FILE * h;
	if (!( filename = rest_temporary_filename("wsdl") ))
		return( filename );
	else if (!( h = fopen( filename, "w" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=\"1.0\"?>\n");
		fprintf(h,"<wsdl:definitions name=\"CompatibleOneRemoteCommandServer\"\n");
		fprintf(h,"targetNamespace=\"%s/wsdl\"\n",host);
		fprintf(h,"xmlns:cords=\"%s/manifest/\"\n",host);
		fprintf(h,"xmlns:slam=\"%s/agreement/\"\n",host);
		fprintf(h,"xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n");
		fprintf(h,"xmlns:wsdl=\"http://schemas.xmlsoap.org/wsdl/\">\n");

		fprintf(h,"<import namespace=\"%s/manifest/\"\n",host);
		fprintf(h,"location=\"http://www.compatibleone.fr/schemes/manifest.xsd\"/>\n");
		fprintf(h,"<import namespace=\"%s/agreement/\"\n",host);
		fprintf(h,"location=\"http://www.compatibleone.fr/schemes/slam.xsd\"/>\n");

		fprintf(h,"<wsdl:types/>\n");

		fprintf(h,"<wsdl:message name=\"ResolverIn\">\n");
		fprintf(h,"<wsdl:part name=\"command\" type=\"string\"/>\n");
		fprintf(h,"<wsdl:part name=\"category\" type=\"string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"ResolverOut\">\n");
		fprintf(h,"<wsdl:part name=\"category\" type=\"string\"/>\n");
		fprintf(h,"<wsdl:part name=\"host\" type=\"string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"ParseManifestIn\">\n");
		fprintf(h,"<wsdl:part name=\"command\" type=\"string\"/>\n");
		fprintf(h,"<wsdl:part name=\"document\" type=\"cords:manifest\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"ParseManifestOut\">\n");
		fprintf(h,"<wsdl:part name=\"document\" type=\"cords:manifest\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"ParseSLAIn\">\n");
		fprintf(h,"<wsdl:part name=\"command\" type=\"string\"/>\n");
		fprintf(h,"<wsdl:part name=\"document\" type=\"slam:agreement\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"ParseSLAOut\">\n");
		fprintf(h,"<wsdl:part name=\"document\" type=\"slam:agreement\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"BrokerManifestIn\">\n");
		fprintf(h,"<wsdl:part name=\"command\" type=\"string\"/>\n");
		fprintf(h,"<wsdl:part name=\"document\" type=\"cords:manifest\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"BrokerSLAIn\">\n");
		fprintf(h,"<wsdl:part name=\"command\" type=\"string\"/>\n");
		fprintf(h,"<wsdl:part name=\"document\" type=\"slam:agreement\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"BrokerOut\">\n");
		fprintf(h,"<wsdl:part name=\"service\" type=\"string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"ServiceIn\">\n");
		fprintf(h,"<wsdl:part name=\"action\" type=\"string\"/>\n");
		fprintf(h,"<wsdl:part name=\"service\" type=\"string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"ServiceOut\">\n");
		fprintf(h,"<wsdl:part name=\"service\" type=\"string\"/>\n");
		fprintf(h,"<wsdl:part name=\"state\" type=\"string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"ScriptIn\">\n");
		fprintf(h,"<wsdl:part name=\"command\" type=\"string\"/>\n");
		fprintf(h,"<wsdl:part name=\"script\" type=\"file\"/>\n");
		fprintf(h,"<wsdl:part name=\"parameters\" type=\"string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"ScriptOut\">\n");
		fprintf(h,"<wsdl:part name=\"output\" type=\"string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:portType name=\"CORCS\">\n");
		fprintf(h,"<wsdl:operation name=\"ResolveCategory\">\n");
		fprintf(h,"<wsdl:input message=\"ResolverIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"ResolverOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");
		fprintf(h,"<wsdl:operation name=\"ParseManifest\">\n");
		fprintf(h,"<wsdl:input message=\"ParseManifestIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"ParseManifestOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");
		fprintf(h,"<wsdl:operation name=\"ParseSLA\">\n");
		fprintf(h,"<wsdl:input message=\"ParseSLAIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"ParseSLAOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");
		fprintf(h,"<wsdl:operation name=\"BrokerManifest\">\n");
		fprintf(h,"<wsdl:input message=\"BrokerManifestIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"BrokerOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");
		fprintf(h,"<wsdl:operation name=\"BrokerSLA\">\n");
		fprintf(h,"<wsdl:input message=\"BrokerSLAIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"BrokerOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");
		fprintf(h,"<wsdl:operation name=\"ServiceAction\">\n");
		fprintf(h,"<wsdl:input message=\"ServiceIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"ServiceOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");
		fprintf(h,"<wsdl:operation name=\"RunScript\">\n");
		fprintf(h,"<wsdl:input message=\"ScriptIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"ScriptOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");
		fprintf(h,"</wsdl:portType>\n");

		fprintf(h,"<wsdl:binding name=\"SOAPCORCS\" type=\"CORCS\">\n");
		fprintf(h,"<soap:binding style=\"rpc\" ");
		fprintf(h,"transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n");
		fprintf(h,"<wsdl:operation name=\"ResolveCategory\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/resolver\" style=\"rpc\"/>\n",host);
		fprintf(h,"<wsdl:input message=\"ResolverIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"ResolverOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");
		fprintf(h,"<wsdl:operation name=\"ParseManifest\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/ParseManifest\" style=\"rpc\"/>\n",host);
		fprintf(h,"<wsdl:input message=\"ParseManifestIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"ParseManifestOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");
		fprintf(h,"<wsdl:operation name=\"ParseSLA\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/ParseSLA\" style=\"rpc\"/>\n",host);
		fprintf(h,"<wsdl:input message=\"ParseSLAIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"ParseSLAOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");
		fprintf(h,"<wsdl:operation name=\"BrokerManifest\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/BrokerManifest\" style=\"rpc\"/>\n",host);
		fprintf(h,"<wsdl:input message=\"BrokerManifestIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"BrokerOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");
		fprintf(h,"<wsdl:operation name=\"BrokerSLA\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/BrokerSLA\" style=\"rpc\"/>\n",host);
		fprintf(h,"<wsdl:input message=\"BrokerSLAIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"BrokerOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");
		fprintf(h,"<wsdl:operation name=\"ServiceAction\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/service\" style=\"rpc\"/>\n",host);
		fprintf(h,"<wsdl:input message=\"ServiceIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"ServiceOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");
		fprintf(h,"<wsdl:operation name=\"RunScript\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/script\" style=\"rpc\"/>\n",host);
		fprintf(h,"<wsdl:input message=\"ScriptIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"ScriptOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");
		fprintf(h,"</wsdl:binding>\n");

		fprintf(h,"<wsdl:service name=\"CORCS\">\n");
		fprintf(h,"<wsdl:documentation>The Accords Platform Remote Command Service SOAP API</wsdl:documentation>\n"); 
		fprintf(h,"<wsdl:port name=\"AccordsPlatformRCS\"");
		fprintf(h," binding=\"SOAPCORCS\">\n");
		fprintf(h,"<soap:address location=\"%s\"/>\n",host);
		fprintf(h,"</wsdl:port>\n");
		fprintf(h,"</wsdl:service>\n");

		fprintf(h,"</wsdl:definitions>\n");
		fclose(h);
		return( filename );
	}
}

/*	---------------------------------------		*/
/*	l i b e r a t e _ x m l _ e l e m e n t		*/ 
/*	---------------------------------------		*/
private	struct	xml_element * 	liberate_xml_element( struct xml_element * sptr )
{
	document_drop( sptr );
	return((struct xml_element *) 0);
}

/*	---------------------------------------------	*/
/*	d o c u m e n t _ e l e m e n t _ s t r i n g 	*/ 
/*	---------------------------------------------	*/
private	char *	document_element_string( struct xml_element * xptr, char * nptr )
{
	struct	xml_element *eptr;
	if (!( eptr = nested_document_element( xptr, nptr )))
		return((char *) 0);
	else	return( allocate_string( eptr->value ) );
}

/*	---------------------------------------------	*/
/*	d o c u m e n t _ e l e m e n t _ f i l e  	*/ 
/*	---------------------------------------------	*/
private	char *	document_element_file( struct xml_element * xptr, char * nptr )
{
	struct	xml_element *eptr;
	char *	filename;
	if (!( eptr = nested_document_element( xptr, nptr )))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename( "xml" ) ))
		return( filename );
	else	
	{
		document_serialise_file( eptr, filename );
		return( filename );
	}
}

/*	---------------------------------	*/
/*	c o r d s _ x m l _ e l e m e n t	*/ 
/*	---------------------------------	*/
private	struct	xml_element * 	corcs_xml_element( char * filename )
{
	if (!( filename ))
 		return((struct xml_element *) 0);
	else	return( document_parse_file( filename ) );
}


/*	---------------------------------------		*/
/*	s o a p _ r e s p o n s e _ f o o t e r		*/
/*	---------------------------------------		*/
private	void	soap_message_footer( FILE * h, char * nptr )
{
	fprintf(h,"</m:%s>\n",nptr);
	fprintf(h,"</soapenv:Body>\n");
	fprintf(h,"</soapenv:Envelope>\n");
}

/*	---------------------------------------		*/
/*	s o a p _ r e s p o n s e _ h e a d e r		*/
/*	---------------------------------------		*/
private	void	soap_message_header( FILE * h, char * nptr )
{
	fprintf(h,"<?xml version=\"1.0\"?>\n");
	fprintf(h,"<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope\">\n");

	fprintf(h,"<soapenv:Body>\n");
	fprintf(h,"<m:%s xmlns:m=\"%s/wsdl\">\n",nptr,get_identity());
	return;
}

/*	-------------------------------------------------------		*/
/*	c o r c s _ r e s o l v e r _ s o a p _ r e s p o n s e		*/
/*	-------------------------------------------------------		*/
private	char * corcs_resolver_soap_response( char * category, struct occi_response * rptr )
{
	FILE * h;
	char *	filename;
	struct	occi_element * eptr;
	if (!( filename = rest_temporary_filename( "xml" ) ))
		return( filename );
	else if (!( h = fopen( filename, "w" ) ))
		return( liberate( filename ) );
	else
	{
		soap_message_header( h, "ResolveCategoryResponse" );
		fprintf(h,"<m:category>%s</m:category>\n",category);
		for (	eptr = rptr->first;
			eptr != (struct occi_element *) 0;
			eptr = eptr->next )
		{
			if (!( eptr->value ))
				continue;
			else 	fprintf(h,"<m:host>%s</m:host>\n",eptr->value);
		}
		soap_message_footer( h, "ResolveCategoryResponse" );
		fclose(h);
		rptr = occi_remove_response( rptr );
		return( filename );
	}
}

/*	-------------------------------------------------------		*/
/*	      c o r c s _ p a r s e r _ r e s p o n s e	 		*/
/*	-------------------------------------------------------		*/
private	struct rest_response * corcs_parser_response( struct rest_response * aptr, char * document, char * message )
{
	FILE * h;
	char *	filename;
	if (!( filename = rest_temporary_filename( "xml" ) ))
		return( aptr );
	else if (!( h = fopen( filename, "w" ) ))
		return( liberate( filename ) );
	else
	{
		soap_message_header( h, message );
		fprintf(h,"<m:document>%s</m:document>\n",document);
		soap_message_footer( h, message );
		fclose(h);
		return( rest_file_response( aptr, filename, "text/xml" ) );
	}
}

/*	-------------------------------------------------------		*/
/*	      c o r c s _ b r o k e r _ r e s p o n s e	 		*/
/*	-------------------------------------------------------		*/
private	struct rest_response * corcs_broker_response( struct rest_response * aptr, char * service, char * message  )
{
	FILE * h;
	char *	filename;
	if (!( filename = rest_temporary_filename( "xml" ) ))
		return( aptr );
	else if (!( h = fopen( filename, "w" ) ))
		return( liberate( filename ) );
	else
	{
		soap_message_header( h, message );
		fprintf(h,"<m:service>%s</m:service>\n",service);
		soap_message_footer( h, message );
		fclose(h);
		return( rest_file_response( aptr, filename, "text/xml" ) );
	}
}

/*	-------------------------------------	*/
/*	c o r d s _ s o a p _ r e s o l v e r	*/
/*	-------------------------------------	*/
private	struct	rest_response * corcs_soap_resolver( struct rest_response * aptr, struct xml_element * sptr, struct rest_request * rptr )
{
	char *	category=(char *) 0;
	char *	command=(char *) 0;
	char *	filename=(char *) 0;
	if (!( sptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( rptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( category = document_element_string( sptr, "category") ))
		return(rest_html_response(aptr, 400, "missing category"));
	else if (!( command = document_element_string( sptr, "command") ))
		return(rest_html_response(aptr, 400, "missing command"));
	else if ( strcmp( command, "resolver" ) != 0)
		return(rest_html_response(aptr, 400, "incorrect command"));
	else
	{
		/* resolve category */
		/* ---------------- */
		if ((filename = cords_resolver_operation( category )) != (char *) 0)
			cords_resolver_response ( aptr, filename );
		sptr = liberate_xml_element( sptr );
		return( aptr );
	}
}

/*	-----------------------------------------------------	*/
/*	  c o r d s _ s o a p _ m a n i f e s t _ p a r s e r 	*/
/*	-----------------------------------------------------	*/
private	struct	rest_response * corcs_soap_manifest_parser( struct rest_response * aptr,struct xml_element * sptr, struct rest_request * rptr )
{
	char *	message="ParseManifest";
	char *	command=(char *) 0;
	char *	filename=(char *) 0;
	if (!( sptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( rptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( filename = document_element_file( sptr, "filename") ))
		return(rest_html_response(aptr, 400, "missing filename"));
	else if (!( command = document_element_string( sptr, "command") ))
		return(rest_html_response(aptr, 400, "missing command"));
	else if ( strcmp( command, "parser" ) != 0)
		return(rest_html_response(aptr, 400, "incorrect command"));
	else
	{
		/* parse filename */
		/* -------------- */
		cords_parser_operation( filename );
		sptr = liberate_xml_element( sptr );
		return( corcs_parser_response ( aptr, filename, message ) );
	}
}

/*	-------------------------------------	*/
/*	  c o r d s _ s o a p _ p a r s e r 	*/
/*	-------------------------------------	*/
private	struct	rest_response * corcs_soap_sla_parser( struct rest_response * aptr,struct xml_element * sptr, struct rest_request * rptr )
{
	char *	message="ParseSLA";
	char *	command=(char *) 0;
	char *	filename=(char *) 0;
	if (!( sptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( rptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( filename = document_element_file( sptr, "filename") ))
		return(rest_html_response(aptr, 400, "missing filename"));
	else if (!( command = document_element_string( sptr, "command") ))
		return(rest_html_response(aptr, 400, "missing command"));
	else if ( strcmp( command, "parser" ) != 0)
		return(rest_html_response(aptr, 400, "incorrect command"));
	else
	{
		/* parse filename */
		/* -------------- */
		cords_parser_operation( filename );
		sptr = liberate_xml_element( sptr );
		return( corcs_parser_response ( aptr, filename, message ) );
	}
}

/*	-----------------------------------------------------	*/
/*	  c o r d s _ s o a p _ m a n i f e s t _ b r o k e r 	*/
/*	-----------------------------------------------------	*/
private	struct	rest_response * corcs_soap_manifest_broker( struct rest_response * aptr, struct xml_element * sptr, struct rest_request * rptr )
{
	char *	message="BrokerManifest";
	char *	filename=(char *) 0;
	char *	command=(char *) 0;
	if (!( sptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( rptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( filename = document_element_file( sptr, "filename") ))
		return(rest_html_response(aptr, 400, "missing filename"));
	else if (!( command = document_element_string( sptr, "command") ))
		return(rest_html_response(aptr, 400, "missing command"));
	else if ( strcmp( command, "broker" ) != 0)
		return(rest_html_response(aptr, 400, "incorrect command"));
	else
	{
		/* broker filename */
		/* --------------- */
		cords_broker_operation( filename );
		sptr = liberate_xml_element( sptr );
		return( corcs_broker_response ( aptr, filename, message ) );
	}
}

/*	-----------------------------------------------------	*/
/*	      c o r d s _ s o a p _ s l a _ b r o k e r 	*/
/*	-----------------------------------------------------	*/
private	struct	rest_response * corcs_soap_sla_broker( struct rest_response * aptr, struct xml_element * sptr, struct rest_request * rptr )
{
	char *	message="BokerSLA";
	char *	filename=(char *) 0;
	char *	command=(char *) 0;
	if (!( sptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( rptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( filename = document_element_file( sptr, "filename") ))
		return(rest_html_response(aptr, 400, "missing filename"));
	else if (!( command = document_element_string( sptr, "command") ))
		return(rest_html_response(aptr, 400, "missing command"));
	else if ( strcmp( command, "broker" ) != 0)
		return(rest_html_response(aptr, 400, "incorrect command"));
	else
	{
		/* broker filename */
		/* --------------- */
		cords_broker_operation( filename );
		sptr = liberate_xml_element( sptr );
		return( corcs_broker_response ( aptr, filename, message ) );
	}
}

/*	-------------------------------------	*/
/*	 c o r d s _ s o a p _ s e r v i c e 	*/
/*	-------------------------------------	*/
private	struct	rest_response * corcs_soap_service( struct rest_response * aptr, struct xml_element * sptr, struct rest_request * rptr )
{
	char *	service=(char *) 0;
	char *	filename=(char *) 0;
	char *	action=(char *) 0;
	if (!( sptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( rptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( service = document_element_string( sptr, "service") ))
		return(rest_html_response(aptr, 400, "missing service"));
	else if (!( action = document_element_string( sptr, "action") ))
		return(rest_html_response(aptr, 400, "missing action"));
	else
	{
		/* invoke service action */
		/* --------------------- */
		if ((filename = cords_service_operation( service, action )) != (char *) 0)
			cords_service_response ( aptr, filename );
		sptr = liberate_xml_element( sptr );
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------	*/
/*	  c o r d s _ s o a p _ s c r i p t 	*/
/*	-------------------------------------	*/
private	struct	rest_response * corcs_soap_script( struct rest_response * aptr, struct xml_element * sptr, struct rest_request * rptr )
{
	char *	filename=(char *) 0;
	char *	command=(char *) 0;
	char *	parameters=(char *) 0;
	if (!( sptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( rptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( filename = document_element_file( sptr, "filename") ))
		return(rest_html_response(aptr, 400, "missing filename"));
	else if (!( command = document_element_string( sptr, "command") ))
		return(rest_html_response(aptr, 400, "missing command"));
	else if (!( parameters= document_element_string( sptr, "parameters") ))
		return(rest_html_response(aptr, 400, "missing parameters"));
	else if ( strcmp( command, "script" ) != 0)
		return(rest_html_response(aptr, 400, "incorrect command"));
	else
	{
		/* run script */
		/* ---------- */
		if ((filename = cords_script_interpreter( filename, parameters )) != (char *) 0)
			cords_script_response ( aptr, filename );
		sptr = liberate_xml_element( sptr );
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------	*/
/*	    c o r d s _ s o a p _ o c c i 	*/
/*	-------------------------------------	*/
private	struct	rest_response * corcs_soap_occi( struct rest_response * aptr, struct xml_element * sptr, struct rest_request * rptr )
{
	char *	category=(char *) 0;
	char *	command=(char *) 0;
	if (!( sptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( rptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( category = document_element_string( sptr, "category") ))
		return(rest_html_response(aptr, 400, "missing category"));
	else if (!( command = document_element_string( sptr, "command") ))
		return(rest_html_response(aptr, 400, "missing command"));
	else if ( strcmp( command, "resolver" ) != 0)
		return(rest_html_response(aptr, 400, "incorrect command"));
	else
	{
		sptr = liberate_xml_element( sptr );
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------	*/
/*	    c o r d s _ s o a p _ p o s t 	*/
/*	-------------------------------------	*/
public	struct rest_response * corcs_soap_post( struct rest_response * aptr, char * command, struct rest_request * rptr )
{
	struct	xml_element * sptr=(struct xml_element *) 0;
	struct	rest_header * hptr;
	char *	wptr;
	struct	url * uptr;
	if (!( rptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( hptr = rest_resolve_header( rptr->first, "soapAction" )))
		return(rest_html_response(aptr, 400, "missing soapAction"));
	else if (!( hptr->value ))
		return(rest_html_response(aptr, 400, "missing soapAction"));
	else if (!( wptr = allocate_string( hptr->value ) ))
		return(rest_html_response(aptr, 400, "missing soapAction"));
	else if (!( wptr = occi_unquoted_value( wptr ) ))
		return(rest_html_response(aptr, 400, "missing soapAction"));
	else if (!( uptr = analyse_url(  wptr ) ))
		return(rest_html_response(aptr, 400, "missing soapAction"));
	else if (!( command = uptr->object ))
		return(rest_html_response(aptr, 400, "missing soapAction"));
	else if ( check_verbose() )
		printf("   Soap Action: %s \n",wptr);

	command_server_accept( "application/soap" );

	if (!( rptr->body ))
		return(rest_html_response(aptr, 400, "missing request body"));
	else if (!( sptr = corcs_xml_element( rptr->body ) ))
		return(rest_html_response(aptr, 400, "incorrect request body"));
	else if (!( strcmp(  command, "/resolver" ) ))
		return( corcs_soap_resolver( aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "/ParseManifest" ) ))
		return( corcs_soap_manifest_parser( aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "/ParseSLA" ) ))
		return( corcs_soap_sla_parser( aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "/BrokerManifest" ) ))
		return( corcs_soap_manifest_broker( aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "/BrokerSLA" ) ))
		return( corcs_soap_manifest_broker( aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "/service" ) ))
		return( corcs_soap_service( aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "/script" ) ))
		return( corcs_soap_script( aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "/occi" ) ))
		return( corcs_soap_occi( aptr, sptr, rptr ) );
	else
	{
		sptr = liberate_xml_element( sptr );
		return(rest_html_response(aptr, 400, "incorrect rest command" ));
	}
}

/*	-------------------------------------	*/
/*	c o r c s _ s o a p _ g e t _ w s s l 	*/
/*	-------------------------------------	*/
public	char * corcs_soap_get_wsdl()
{
	char 	host[1024];
	sprintf(host,"%s",get_identity());
	return( corcs_soap_wsdl(host) );
}

/*	-------------------------------------	*/
/*	   s o a p _ i n l i n e _ x m l 	*/
/*	-------------------------------------	*/
private	int	soap_inline_xml( FILE * h, char * filename )
{
	struct	xml_element * eptr;

	if (!( eptr = corcs_xml_element( filename )))
		return( 0 );
	else
	{
		document_serialise_element( h, eptr, 0 );
		document_drop( eptr );
		return( 0 );
	}
}

/*	-------------------------------------	*/
/*	   s o a p _ i n l i n e _ f i l e	*/
/*	-------------------------------------	*/
private	int	soap_inline_file( FILE * h, char * filename )
{
	FILE *	s;
	int	c;
	if (!( s = fopen( filename, "r" ) ))
		return( 0 );
	else
	{
		while ((c = fgetc(s)) > 0)
			fputc(c,h);
		fclose(h);
		return(0);
	}
}

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
		printf("   SOAP POST   %s %s \n",host, filename);

	sprintf(buffer,"%s%s",host,action);

	if ( check_verbose() )
		printf("   Soap Action %s \n",buffer);

	if (!( hptr = rest_create_header( "soapAction", buffer )))
		return( 0 );
	else if (!( rptr = rest_client_post_request( host, default_tls(), get_default_agent(), filename, hptr ) ))
		return( 0 );
	else
	{
		printf("   SOAP RESPONSE %u %s \n",rptr->status, (rptr->message ? rptr->message : "NO MESSAGE"));
		if (( rptr->body )
		&&  ((eptr = document_parse_file( rptr->body )) != (struct xml_element *) 0))
		{
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
private	int	invoke_soap_resolver_api( char * category )
{
	char *	message;
	FILE *	h;

	if ( check_verbose() )
		printf("   SOAP API Resolver %s \n",category);

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
		return( invoke_soap_request( "resolver", soap, wsdl, message ) );
	}
}

/*	-------------------------------------------	*/
/*	i n v o k e _ s o a p _ p a r s e r _ a p i	*/
/*	-------------------------------------------	*/
private	int	invoke_soap_parser_api( char * type, char * filename )
{
	char *	message;
	FILE *	h;
	if ( check_verbose() )
		printf("   SOAP API Parser %s %s \n",type,filename);
	if (!( message = rest_temporary_filename("xml") ))
		return(0);
	else if (!( h = fopen( message, "w" ) ))
		return(0);
	else
	{
		if (!( strcasecmp( type, "MANIFEST" ) ))
			type = "ParseManifest";
		else	type = "ParseSLA";
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
private	int	invoke_soap_broker_api( char * type, char * filename )
{
	char *	message;
	FILE *	h;
	if ( check_verbose() )
		printf("   SOAP API Broker %s %s \n",type,filename);
	if (!( message = rest_temporary_filename("xml") ))
		return(0);
	else if (!( h = fopen( message, "w" ) ))
		return(0);
	else
	{
		if (!( strcasecmp( type, "MANIFEST" ) ))
			type = "BrokerManifest";
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
private	int	invoke_soap_service_api( char * action, char * service )
{
	char *	message;
	FILE *	h;
	if ( check_verbose() )
		printf("   SOAP API Service %s %s \n",action, service);
	if (!( message = rest_temporary_filename("xml") ))
		return(0);
	else if (!( h = fopen( message, "w" ) ))
		return(0);
	else
	{
		soap_message_header( h, "ServiceAction" );
		fprintf(h,"<command>%s</command>\n",action);
		fprintf(h,"<service>%s</service>\n",service);
		soap_message_footer( h, "ServiceAction" );
		fclose(h);
		return( invoke_soap_request( "service", soap, wsdl, message ) );
	}
}

/*	-------------------------------------------	*/
/*	i n v o k e _ s o a p _ s c r i p t _ a p i	*/
/*	-------------------------------------------	*/
private	int	invoke_soap_script_api( char * script, char * parameters )
{
	char *	message;
	FILE *	h;
	if ( check_verbose() )
		printf("   SOAP API Script %s %s \n",script,(parameters ? parameters : "" ));
	if (!( message = rest_temporary_filename("xml") ))
		return(0);
	else if (!( h = fopen( message, "w" ) ))
		return(0);
	else
	{
		soap_message_header( h, "RunScript" );
		fprintf(h,"<command>script</command>\n");
		fprintf(h,"<parameters>%s</parameters>\n",parameters);
		fprintf(h,"<script>\n");
		soap_inline_file(h,script);
		fprintf(h,"</script>\n");
		soap_message_footer( h, "RunScript" );
		fclose(h);
		return( invoke_soap_request( "script", soap, wsdl, message ) );
	}
}

/*	-------------------------------------	*/
/*	    i n v o k e _ s o a p _ a p i	*/
/*	-------------------------------------	*/
private	int	invoke_soap_api( char * command, char * subject, char * option )
{
	if (!( strcmp( command, "resolver" ) ))
		return( invoke_soap_resolver_api( subject ) );
	else if (!( strcmp( command, "parser" ) ))
		return( invoke_soap_parser_api( subject, option ) );
	else if (!( strcmp( command, "broker" ) ))
		return( invoke_soap_broker_api( subject, option ) );
	else if (!( strcmp( command, "service" ) ))
		return( invoke_soap_service_api( subject, option ) );
	else if (!( strcmp( command, "script" ) ))
		return( invoke_soap_script_api( subject, option ) );
	else
	{
		if ( check_verbose() )
			printf("   SOAP API: Incorrect Command: \"%s\" \n",command);
		return(0);
	}
}

	/* ------------- */
#endif  /* _corcs_soap_c */
	/* ------------- */

