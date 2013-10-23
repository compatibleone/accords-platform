#ifndef	_corc_wsdl_c
#define	_corc_wsdl_c

/*	-----------------------------------	*/
/*	w s d l _ s o a p _ e n c o d i n g	*/
/*	-----------------------------------	*/
private void	wsdl_soap_encoding( FILE * h, char * sbs )
{
	fprintf(h,"<soap:body encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"\n");
	if (!( strcmp( sbs, "rpc" ) ))
	{
		fprintf(h,"namespace=\"urn:compatibleone:corcsservice\"\n");
		fprintf(h,"use=\"encoded\"/>\n");
	}
	else
	{
		fprintf(h,"namespace=\"urn:compatibleone:corcsservice\"\n");
		fprintf(h,"use=\"literal\"/>\n");
	}
	return;
}

/*	------------------------------------------------	*/
/*	  w s d l _ i n p u t _ o u t p u t _ f a u l t 	*/
/*	------------------------------------------------	*/
private void	wsdl_input_output_fault(FILE * h, char * sbs )
{
	fprintf(h,"<input>\n");
	wsdl_soap_encoding(h,sbs);
	fprintf(h,"</input>\n");
	fprintf(h,"<output>\n");
	wsdl_soap_encoding(h,sbs);
	fprintf(h,"</output>\n");
	fprintf(h,"<fault><soap:fault name=\"fault\"/></fault>\n"); 
	return;
}

private	char *	corcs_soap_wsdl(char * host,char * sbs)
{
	char * filename;
	char *	sbb;
	FILE * h;
	if (!( filename = rest_temporary_filename("wsdl") ))
		return( filename );
	else if (!( h = fopen( filename, "w" ) ))
		return( liberate( filename ) );
	else
	{
		if (!( strcmp( sbs, "rpc" ) ))
			sbb = "wsrpc";
		else	sbb = "wsdl";

		fprintf(h,"<?xml version=\"1.0\"?>\n");
		fprintf(h,"<definitions name=\"CorcsService\"\n");
		fprintf(h,"targetNamespace=\"%s/resolver.wsdl\"\n",host);
		fprintf(h,"xmlns=\"http://schemas.xmlsoap.org/wsdl/\"\n");
		fprintf(h,"xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n");
		fprintf(h,"xmlns:tns=\"%s/resolver.wsdl\"\n",host);
		fprintf(h,"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\n");

		fprintf(h,"<types>\n");
		fprintf(h,"<xsd:schema targetNameSpace=\"%s\" ",host);
		fprintf(h,"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\n");

		/* ------------- */
		/* manifest type */
		/* ------------- */
		fprintf(h,"<xsd:complexType name=\"manifest\">\n");
		fprintf(h,"<xsd:sequence>\n");
		fprintf(h,"<xsd:element name=\"name\" type=\"xsd:string\"/>\n");
		fprintf(h,"</xsd:sequence>\n");
		fprintf(h,"</xsd:complexType>\n");

		/* -------------- */
		/* agreement type */
		/* -------------- */
		fprintf(h,"<xsd:complexType name=\"agreement\">\n");
		fprintf(h,"<xsd:sequence>\n");
		fprintf(h,"<xsd:element name=\"name\" type=\"xsd:string\"/>\n");
		fprintf(h,"<xsd:element name=\"initiator\" type=\"xsd:string\"/>\n");
		fprintf(h,"<xsd:element name=\"responder\" type=\"xsd:string\"/>\n");
		fprintf(h,"<xsd:element name=\"expires\" type=\"xsd:string\"/>\n");
		fprintf(h,"<xsd:element name=\"template\" type=\"xsd:string\"/>\n");
		fprintf(h,"</xsd:sequence>\n");
		fprintf(h,"</xsd:complexType>\n");
		fprintf(h,"</xsd:schema>\n");
		fprintf(h,"</types>\n");

		fprintf(h,"<message name=\"AsynchMessageRequest\">\n");
		fprintf(h,"<part name=\"identity\" type=\"xsd:string\"/>\n");
		fprintf(h,"</message>\n");

		fprintf(h,"<message name=\"AsynchMessageResponse\">\n");
		fprintf(h,"<part name=\"identity\" type=\"xsd:string\"/>\n");
		fprintf(h,"</message>\n");

		fprintf(h,"<message name=\"ResolverRequest\">\n");
		fprintf(h,"<part name=\"command\" type=\"xsd:string\"/>\n");
		fprintf(h,"<part name=\"category\" type=\"xsd:string\"/>\n");
		fprintf(h,"</message>\n");

		fprintf(h,"<message name=\"ResolverResponse\">\n");
		fprintf(h,"<part name=\"category\" type=\"xsd:string\"/>\n");
		fprintf(h,"<part name=\"hostname\" type=\"xsd:string\"/>\n");
		fprintf(h,"</message>\n");

		fprintf(h,"<message name=\"ParseManifestRequest\">\n");
		fprintf(h,"<part name=\"command\" type=\"xsd:string\"/>\n");
		fprintf(h,"<part name=\"manifest\" type=\"tns:manifest\"/>\n");
		fprintf(h,"</message>\n");

		fprintf(h,"<message name=\"AsynchParseManifestRequest\">\n");
		fprintf(h,"<part name=\"command\" type=\"xsd:string\"/>\n");
		fprintf(h,"<part name=\"manifest\" type=\"tns:manifest\"/>\n");
		fprintf(h,"<part name=\"callback\" type=\"xsd:string\"/>\n");
		fprintf(h,"</message>\n");

		fprintf(h,"<message name=\"ParseManifestResponse\">\n");
		fprintf(h,"<part name=\"manifest\" type=\"xsd:string\"/>\n");
		fprintf(h,"</message>\n");

		fprintf(h,"<message name=\"ParseSLARequest\">\n");
		fprintf(h,"<part name=\"command\" type=\"xsd:string\"/>\n");
		fprintf(h,"<part name=\"agreement\" type=\"tns:agreement\"/>\n");
		fprintf(h,"</message>\n");

		fprintf(h,"<message name=\"AsynchParseSLARequest\">\n");
		fprintf(h,"<part name=\"command\" type=\"xsd:string\"/>\n");
		fprintf(h,"<part name=\"agreement\" type=\"tns:agreement\"/>\n");
		fprintf(h,"<part name=\"callback\" type=\"xsd:string\"/>\n");
		fprintf(h,"</message>\n");

		fprintf(h,"<message name=\"ParseSLAResponse\">\n");
		fprintf(h,"<part name=\"agreement\" type=\"xsd:string\"/>\n");
		fprintf(h,"</message>\n");

		fprintf(h,"<message name=\"BrokerSLARequest\">\n");
		fprintf(h,"<part name=\"command\" type=\"xsd:string\"/>\n");
		fprintf(h,"<part name=\"agreement\" type=\"tns:agreement\"/>\n");
		fprintf(h,"</message>\n");

		fprintf(h,"<message name=\"AsynchBrokerSLARequest\">\n");
		fprintf(h,"<part name=\"command\" type=\"xsd:string\"/>\n");
		fprintf(h,"<part name=\"agreement\" type=\"tns:agreement\"/>\n");
		fprintf(h,"<part name=\"callback\" type=\"xsd:string\"/>\n");
		fprintf(h,"</message>\n");

		fprintf(h,"<message name=\"BrokerSLAResponse\">\n");
		fprintf(h,"<part name=\"service\" type=\"xsd:string\"/>\n");
		fprintf(h,"</message>\n");

		fprintf(h,"<message name=\"ServiceActionRequest\">\n");
		fprintf(h,"<part name=\"action\" type=\"xsd:string\"/>\n");
		fprintf(h,"<part name=\"service\" type=\"xsd:string\"/>\n");
		fprintf(h,"</message>\n");

		fprintf(h,"<message name=\"AsynchServiceActionRequest\">\n");
		fprintf(h,"<part name=\"action\" type=\"xsd:string\"/>\n");
		fprintf(h,"<part name=\"service\" type=\"xsd:string\"/>\n");
		fprintf(h,"<part name=\"callback\" type=\"xsd:string\"/>\n");
		fprintf(h,"</message>\n");

		fprintf(h,"<message name=\"ServiceActionResponse\">\n");
		fprintf(h,"<part name=\"service\" type=\"xsd:string\"/>\n");
		fprintf(h,"<part name=\"action\" type=\"xsd:string\"/>\n");
		fprintf(h,"<part name=\"status\" type=\"xsd:string\"/>\n");
		fprintf(h,"</message>\n");

		fprintf(h,"<message name=\"RunScriptRequest\">\n");
		fprintf(h,"<part name=\"command\" type=\"xsd:string\"/>\n");
		fprintf(h,"<part name=\"script\" type=\"xsd:string\"/>\n");
		fprintf(h,"<part name=\"parameters\" type=\"xsd:string\"/>\n");
		fprintf(h,"</message>\n");

		fprintf(h,"<message name=\"AsynchRunScriptRequest\">\n");
		fprintf(h,"<part name=\"command\" type=\"xsd:string\"/>\n");
		fprintf(h,"<part name=\"script\" type=\"xsd:string\"/>\n");
		fprintf(h,"<part name=\"parameters\" type=\"xsd:string\"/>\n");
		fprintf(h,"<part name=\"callback\" type=\"xsd:string\"/>\n");
		fprintf(h,"</message>\n");

		fprintf(h,"<message name=\"RunScriptResponse\">\n");
		fprintf(h,"<part name=\"output\" type=\"xsd:string\"/>\n");
		fprintf(h,"</message>\n");

		/* ------------------------------- */
		/* port Types describing exchanges */
		/* ------------------------------- */

		fprintf(h,"<portType name=\"CorcsPortType\">\n");

			fprintf(h,"<operation name=\"Resolver\">\n");
			fprintf(h,"<input message=\"tns:ResolverRequest\"/>\n");
			fprintf(h,"<output message=\"tns:ResolverResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"ParseManifest\">\n");
			fprintf(h,"<input message=\"tns:ParseManifestRequest\"/>\n");
			fprintf(h,"<output message=\"tns:ParseManifestResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchParseManifestResult\">\n");
			fprintf(h,"<input message=\"tns:AsynchMessageRequest\"/>\n");
			fprintf(h,"<output message=\"tns:ParseManifestResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"ParseSLA\">\n");
			fprintf(h,"<input message=\"tns:ParseSLARequest\"/>\n");
			fprintf(h,"<output message=\"tns:ParseSLAResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchParseSLAResult\">\n");
			fprintf(h,"<input message=\"tns:AsynchMessageRequest\"/>\n");
			fprintf(h,"<output message=\"tns:ParseSLAResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"BrokerSLA\">\n");
			fprintf(h,"<input message=\"tns:BrokerSLARequest\"/>\n");
			fprintf(h,"<output message=\"tns:BrokerSLAResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchBrokerSLAResult\">\n");
			fprintf(h,"<input message=\"tns:AsynchMessageRequest\"/>\n");
			fprintf(h,"<output message=\"tns:BrokerSLAResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"ServiceAction\">\n");
			fprintf(h,"<input message=\"tns:ServiceActionRequest\"/>\n");
			fprintf(h,"<output message=\"tns:ServiceActionResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchServiceActionResult\">\n");
			fprintf(h,"<input message=\"tns:AsynchMessageRequest\"/>\n");
			fprintf(h,"<output message=\"tns:ServiceActionResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"RunScript\">\n");
			fprintf(h,"<input message=\"tns:RunScriptRequest\"/>\n");
			fprintf(h,"<output message=\"tns:RunScriptResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchRunScriptResult\">\n");
			fprintf(h,"<input message=\"tns:AsynchMessageRequest\"/>\n");
			fprintf(h,"<output message=\"tns:RunScriptResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchParseManifest\">\n");
			fprintf(h,"<input message=\"tns:AsynchParseManifestRequest\"/>\n");
			fprintf(h,"<output message=\"tns:AsynchMessageResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchParseSLA\">\n");
			fprintf(h,"<input message=\"tns:AsynchParseSLARequest\"/>\n");
			fprintf(h,"<output message=\"tns:AsynchMessageResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchBrokerSLA\">\n");
			fprintf(h,"<input message=\"tns:AsynchBrokerSLARequest\"/>\n");
			fprintf(h,"<output message=\"tns:AsynchMessageResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchServiceAction\">\n");
			fprintf(h,"<input message=\"tns:AsynchServiceActionRequest\"/>\n");
			fprintf(h,"<output message=\"tns:AsynchMessageResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchRunScript\">\n");
			fprintf(h,"<input message=\"tns:AsynchRunScriptRequest\"/>\n");
			fprintf(h,"<output message=\"tns:AsynchMessageResponse\"/>\n");
			fprintf(h,"</operation>\n");

		fprintf(h,"</portType>\n");

		/* --------------------------------------- */
		/* PORT Bindings to Message Exchange Types */
		/* --------------------------------------- */
		
		fprintf(h,"<binding name=\"CorcsBinding\" type=\"tns:CorcsPortType\">\n");
		fprintf(h,"<soap:binding style=\"%s\" ",sbs);
		fprintf(h,"transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n");

			fprintf(h,"<operation name=\"Resolver\">\n");
			fprintf(h,"<soap:operation soapAction=\"Resolver\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"ParseManifest\">\n");
			fprintf(h,"<soap:operation soapAction=\"ParseManifest\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"ParseSLA\">\n");
			fprintf(h,"<soap:operation soapAction=\"ParseSLA\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"BrokerSLA\">\n");
			fprintf(h,"<soap:operation soapAction=\"BrokerSLA\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"ServiceAction\">\n");
			fprintf(h,"<soap:operation soapAction=\"ServiceAction\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"RunScript\">\n");
			fprintf(h,"<soap:operation soapAction=\"RunScript\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchParseManifest\">\n");
			fprintf(h,"<soap:operation soapAction=\"AsynchParseManifest\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchParseSLA\">\n");
			fprintf(h,"<soap:operation soapAction=\"AsynchParseSLA\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchBrokerSLA\">\n");
			fprintf(h,"<soap:operation soapAction=\"AsynchBrokerSLA\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchServiceAction\">\n");
			fprintf(h,"<soap:operation soapAction=\"AsynchServiceAction\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchRunScript\">\n");
			fprintf(h,"<soap:operation soapAction=\"AsynchRunScript\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchParseManifestResult\">\n");
			fprintf(h,"<soap:operation soapAction=\"AsynchParseManifestResult\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchParseSLAResult\">\n");
			fprintf(h,"<soap:operation soapAction=\"AsynchParseSLAResult\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchBrokerSLAResult\">\n");
			fprintf(h,"<soap:operation soapAction=\"AsynchBrokerSLAResult\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchServiceActionResult\">\n");
			fprintf(h,"<soap:operation soapAction=\"AsynchServiceActionResult\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchRunScriptResult\">\n");
			fprintf(h,"<soap:operation soapAction=\"AsynchRunScriptResult\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

		fprintf(h,"</binding>\n");

		fprintf(h,"<service name=\"CorcsService\">\n");
		fprintf(h,"<documentation>WSDL File for CorcsService</documentation>\n"); 
		fprintf(h,"<port binding=\"tns:CorcsBinding\" name=\"CorcsPort\">\n");
		fprintf(h,"<soap:address location=\"%s/Corcs/\"/>\n",host);
		fprintf(h,"</port>\n");
		fprintf(h,"</service>\n");

		fprintf(h,"</definitions>\n");
		fclose(h);
		return( filename );
	}
}

/*	-------------------------------------	*/
/*	c o r c s _ s o a p _ g e t _ w s rpc 	*/
/*	-------------------------------------	*/
public	char * corcs_soap_get_wsrpc()
{
	char 	host[1024];
	sprintf(host,"%s",get_identity());
	return( corcs_soap_wsdl(host,"rpc") );
}

/*	-------------------------------------	*/
/*	c o r c s _ s o a p _ g e t _ w s d l 	*/
/*	-------------------------------------	*/
public	char * corcs_soap_get_wsdl()
{
	char 	host[1024];
	sprintf(host,"%s",get_identity());
	return( corcs_soap_wsdl(host,"document") );
}

	/* ------------ */
#endif	/* _corc_wsdl_c */
	/* ------------ */

