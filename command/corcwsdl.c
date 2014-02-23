#ifndef	_corc_wsdl_c
#define	_corc_wsdl_c

#define _ACCORDS_NAMESPACE	"http://schema.compatibleone.fr/accords/1.0/"

#define	_SCHEMA_LOCATION	"corcs.xsd"

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

/*	-----------------------------------	*/
/*	w s d l _ c l o s e _ e l e m e n t	*/
/*	-----------------------------------	*/
private	void	wsdl_close_element( FILE * h )
{
        fprintf(h,"</xsd:sequence>\n");
        fprintf(h,"</xsd:complexType>\n");
        fprintf(h,"</xsd:element>\n");
	return;
}

/*	-----------------------------------	*/
/*	 w s d l _ o p e n _ e l e m e n t	*/
/*	-----------------------------------	*/
private	void	wsdl_open_element( FILE * h, char * name )
{
	fprintf(h,"<xsd:element \n");
	fprintf(h,"\txmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"); 
	fprintf(h,"\txmlns=\"http://www.w3.org/2001/XMLSchema\"\n");
	fprintf(h,"\tname=\"%s\">\n",name);
        fprintf(h,"<xsd:complexType>\n");
        fprintf(h,"<xsd:sequence>\n");
	return;
}

/*	-------------------------------------	*/
/*	w s d l _ s t r i n g _ e l e m e n t	*/
/*	-------------------------------------	*/
private	void	wsdl_string_element( FILE * h, char * name )
{
	fprintf(h,"<xsd:element xmlns:xs=\"http://www.w3.org/2001/XMLSchema\"");
	fprintf(h," name=\"%s\" type=\"xs:string\"/>\n",name);
	return;
}
 
/*	-----------------------------------------	*/
/*	 w s d l _ c o m p l e x _ e l e m e n t	*/
/*	-----------------------------------------	*/
private	void	wsdl_complex_element( FILE * h, char * name, char * ns )
{
	fprintf(h,"<xsd:element xmlns:xs=\"http://www.w3.org/2001/XMLSchema\"");
	fprintf(h," name=\"%s\" type=\"%s\"/>\n",name,ns);
	return;
}
 
/*	-----------------------------------------	*/
/*	 w s d l _ e l e m e n t _ m e s s a g e 	*/
/*	-----------------------------------------	*/
private	void	wsdl_element_message( FILE * h, char * name )
{
	fprintf(h,"<message xmlns:wsdl=\"http://schemas.xmlsoap.org/wsdl/\" name=\"%s\">\n",name);
	fprintf(h,"<part xmlns:wsdl=\"http://schemas.xmlsoap.org/wsdl/\"");
	fprintf(h," name=\"body\" element=\"tns:%s\"/>\n",name);
	fprintf(h,"</message>\n");
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
	char *	mns="tns:cordsManifest";
	char *	ans="tns:cordsAgreement";

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
		fprintf(h,"targetNamespace=\"%s\"\n",_ACCORDS_NAMESPACE);
		fprintf(h,"xmlns=\"http://schemas.xmlsoap.org/wsdl/\"\n");
		fprintf(h,"xmlns:wsdl=\"http://schemas.xmlsoap.org/wsdl/\"\n");
		fprintf(h,"xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n");
		fprintf(h,"xmlns:tns=\"%s\"\n",_ACCORDS_NAMESPACE);
		fprintf(h,"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\n");

		/* ----------------------------------------------- */
		/* import existing namespace documents from source */
		/* ----------------------------------------------- */
		fprintf(h,"<wsdl:types>\n");
		fprintf(h,"\t<xsd:schema targetNamespace=\"%s\">\n",_ACCORDS_NAMESPACE);

		fprintf(h,"\t\t<xsd:include schemaLocation=\"%s\"/>\n",_SCHEMA_LOCATION); 
	
		wsdl_open_element(h,"AsynchMessageRequest");
		wsdl_string_element(h,"identity");
		wsdl_close_element(h);

		wsdl_open_element(h,"ResolverRequest");
		wsdl_string_element(h,"command");
		wsdl_string_element(h,"category");
		wsdl_close_element(h);

		wsdl_open_element(h,"ResolverResponse");
		wsdl_string_element(h,"category");
		wsdl_string_element(h,"hostname");
		wsdl_close_element(h);

		wsdl_open_element(h,"ParseManifestRequest");
		wsdl_string_element(h,"command");
		wsdl_complex_element(h,"manifest",mns); 
		wsdl_close_element(h);

		wsdl_open_element(h,"ConvertManifestRequest");
		wsdl_string_element(h,"command");
		wsdl_complex_element(h,"manifest",mns); 
		wsdl_close_element(h);

		wsdl_open_element(h,"ParseManifestResponse");
		wsdl_complex_element(h,"manifest",mns); 
		wsdl_close_element(h);

		wsdl_open_element(h,"ConvertManifestResponse");
		wsdl_complex_element(h,"manifest",mns); 
		wsdl_close_element(h);

		wsdl_open_element(h,"AsynchParseManifestRequest");
		wsdl_string_element(h,"command");
		wsdl_string_element(h,"callback");
		wsdl_complex_element(h,"manifest",mns); 
		wsdl_close_element(h);

		wsdl_open_element(h,"AsynchConvertManifestRequest");
		wsdl_string_element(h,"command");
		wsdl_string_element(h,"callback");
		wsdl_complex_element(h,"manifest",mns); 
		wsdl_close_element(h);

		wsdl_open_element(h,"ParseSLARequest");
		wsdl_string_element(h,"command");
		wsdl_complex_element(h,"agreement",ans); 
		wsdl_close_element(h);

		wsdl_open_element(h,"ConvertSLARequest");
		wsdl_string_element(h,"command");
		wsdl_complex_element(h,"agreement",ans); 
		wsdl_close_element(h);

		wsdl_open_element(h,"ParseSLAResponse");
		wsdl_complex_element(h,"agreement",ans); 
		wsdl_close_element(h);

		wsdl_open_element(h,"ConvertSLAResponse");
		wsdl_complex_element(h,"agreement",ans); 
		wsdl_close_element(h);

		wsdl_open_element(h,"AsynchParseSLARequest");
		wsdl_string_element(h,"command");
		wsdl_string_element(h,"callback");
		wsdl_complex_element(h,"agreement",ans); 
		wsdl_close_element(h);

		wsdl_open_element(h,"AsynchConvertSLARequest");
		wsdl_string_element(h,"command");
		wsdl_string_element(h,"callback");
		wsdl_complex_element(h,"agreement",ans); 
		wsdl_close_element(h);

		wsdl_open_element(h,"BrokerSLARequest");
		wsdl_string_element(h,"command");
		wsdl_complex_element(h,"agreement",ans); 
		wsdl_close_element(h);

		wsdl_open_element(h,"BrokerSLAResponse");
		wsdl_string_element(h,"service");
		wsdl_close_element(h);

		wsdl_open_element(h,"AsynchBrokerSLARequest");
		wsdl_string_element(h,"command");
		wsdl_string_element(h,"callback");
		wsdl_complex_element(h,"agreement",ans); 
		wsdl_close_element(h);

		wsdl_open_element(h,"ServiceActionRequest");
		wsdl_string_element(h,"action");
		wsdl_string_element(h,"service");
		wsdl_close_element(h);

		wsdl_open_element(h,"ServiceActionResponse");
		wsdl_string_element(h,"service");
		wsdl_string_element(h,"action");
		wsdl_string_element(h,"status");
		wsdl_close_element(h);

		wsdl_open_element(h,"AsynchServiceActionRequest");
		wsdl_string_element(h,"action");
		wsdl_string_element(h,"service");
		wsdl_string_element(h,"callback");
		wsdl_close_element(h);

		wsdl_open_element(h,"RunScriptRequest");
		wsdl_string_element(h,"command");
		wsdl_string_element(h,"script");
		wsdl_string_element(h,"parameters");
		wsdl_close_element(h);

		wsdl_open_element(h,"RunScriptResponse");
		wsdl_string_element(h,"output");
		wsdl_close_element(h);

		wsdl_open_element(h,"AsynchRunScriptRequest");
		wsdl_string_element(h,"command");
		wsdl_string_element(h,"script");
		wsdl_string_element(h,"parameters");
		wsdl_string_element(h,"callback");
		wsdl_close_element(h);

		fprintf(h,"\t</xsd:schema>\n");
		fprintf(h,"</wsdl:types>\n");

		wsdl_element_message(h,"ResolverRequest");
		wsdl_element_message(h,"ResolverResponse");
		wsdl_element_message(h,"ParseManifestRequest");
		wsdl_element_message(h,"ParseManifestResponse");
		wsdl_element_message(h,"ConvertManifestRequest");
		wsdl_element_message(h,"ConvertManifestResponse");
		wsdl_element_message(h,"ParseSLARequest");
		wsdl_element_message(h,"ParseSLAResponse");
		wsdl_element_message(h,"ConvertSLARequest");
		wsdl_element_message(h,"ConvertSLAResponse");
		wsdl_element_message(h,"BrokerSLARequest");
		wsdl_element_message(h,"BrokerSLAResponse");
		wsdl_element_message(h,"ServiceActionRequest");
		wsdl_element_message(h,"ServiceActionResponse");
		wsdl_element_message(h,"RunScriptRequest");
		wsdl_element_message(h,"RunScriptResponse");

		wsdl_element_message(h,"AsynchMessageRequest");
		wsdl_element_message(h,"AsynchMessageResponse");

		wsdl_element_message(h,"AsynchParseManifestRequest");
		wsdl_element_message(h,"AsynchParseSLARequest");
		wsdl_element_message(h,"AsynchConvertManifestRequest");
		wsdl_element_message(h,"AsynchConvertSLARequest");
		wsdl_element_message(h,"AsynchBrokerSLARequest");
		wsdl_element_message(h,"AsynchServiceActionRequest");
		wsdl_element_message(h,"AsynchRunScriptRequest");

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

			fprintf(h,"<operation name=\"ConvertManifest\">\n");
			fprintf(h,"<input message=\"tns:ConvertManifestRequest\"/>\n");
			fprintf(h,"<output message=\"tns:ConvertManifestResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchParseManifestResult\">\n");
			fprintf(h,"<input message=\"tns:AsynchMessageRequest\"/>\n");
			fprintf(h,"<output message=\"tns:ParseManifestResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchConvertManifestResult\">\n");
			fprintf(h,"<input message=\"tns:AsynchMessageRequest\"/>\n");
			fprintf(h,"<output message=\"tns:ParseManifestResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"ParseSLA\">\n");
			fprintf(h,"<input message=\"tns:ParseSLARequest\"/>\n");
			fprintf(h,"<output message=\"tns:ParseSLAResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"ConvertSLA\">\n");
			fprintf(h,"<input message=\"tns:ConvertSLARequest\"/>\n");
			fprintf(h,"<output message=\"tns:ConvertSLAResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchParseSLAResult\">\n");
			fprintf(h,"<input message=\"tns:AsynchMessageRequest\"/>\n");
			fprintf(h,"<output message=\"tns:ParseSLAResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchConvertSLAResult\">\n");
			fprintf(h,"<input message=\"tns:AsynchMessageRequest\"/>\n");
			fprintf(h,"<output message=\"tns:ConvertSLAResponse\"/>\n");
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

			fprintf(h,"<operation name=\"AsynchConvertManifest\">\n");
			fprintf(h,"<input message=\"tns:AsynchConvertManifestRequest\"/>\n");
			fprintf(h,"<output message=\"tns:AsynchMessageResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchParseSLA\">\n");
			fprintf(h,"<input message=\"tns:AsynchParseSLARequest\"/>\n");
			fprintf(h,"<output message=\"tns:AsynchMessageResponse\"/>\n");
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchConvertSLA\">\n");
			fprintf(h,"<input message=\"tns:AsynchConvertSLARequest\"/>\n");
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

			fprintf(h,"<operation name=\"ConvertManifest\">\n");
			fprintf(h,"<soap:operation soapAction=\"ConvertManifest\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"ParseSLA\">\n");
			fprintf(h,"<soap:operation soapAction=\"ParseSLA\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"ConvertSLA\">\n");
			fprintf(h,"<soap:operation soapAction=\"ConvertSLA\"/>\n");
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

			fprintf(h,"<operation name=\"AsynchConvertManifest\">\n");
			fprintf(h,"<soap:operation soapAction=\"AsynchConvertManifest\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchParseSLA\">\n");
			fprintf(h,"<soap:operation soapAction=\"AsynchParseSLA\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchConvertSLA\">\n");
			fprintf(h,"<soap:operation soapAction=\"AsynchConvertSLA\"/>\n");
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

			fprintf(h,"<operation name=\"AsynchConvertManifestResult\">\n");
			fprintf(h,"<soap:operation soapAction=\"AsynchConvertManifestResult\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchParseSLAResult\">\n");
			fprintf(h,"<soap:operation soapAction=\"AsynchParseSLAResult\"/>\n");
			wsdl_input_output_fault(h,sbs);
			fprintf(h,"</operation>\n");

			fprintf(h,"<operation name=\"AsynchConvertSLAResult\">\n");
			fprintf(h,"<soap:operation soapAction=\"AsynchConvertSLAResult\"/>\n");
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

