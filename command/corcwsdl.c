#ifndef	_corc_wsdl_c
#define	_corc_wsdl_c

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
private	char *	corcs_soap_wsdl(char * host,char * sbs)
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
		fprintf(h,"xmlns:tns=\"%s/wsdl\"\n",host);
		fprintf(h,"xmlns:cords=\"%s/manifest/\"\n",host);
		fprintf(h,"xmlns:slam=\"%s/agreement/\"\n",host);
		fprintf(h,"xmlns:xsd=\"http://www.w3c.org/2000/10/XMLSchema\"\n");
		fprintf(h,"xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n");
		fprintf(h,"xmlns:wsdl=\"http://schemas.xmlsoap.org/wsdl/\">\n");

		fprintf(h,"<xsd:import xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" namespace=\"%s/manifest/\"\n",host);
		fprintf(h,"location=\"http://www.compatibleone.fr/schemes/manifest.xsd\"/>\n");
		fprintf(h,"<xsd:import xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" namespace=\"%s/agreement/\"\n",host);
		fprintf(h,"location=\"http://www.compatibleone.fr/schemes/slam.xsd\"/>\n");

		fprintf(h,"<wsdl:types/>\n");

		fprintf(h,"<wsdl:message name=\"AsynchMessageIn\">\n");
		fprintf(h,"<wsdl:part name=\"identifier\" element=\"xsd:string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"AsynchMessageOut\">\n");
		fprintf(h,"<wsdl:part name=\"identifier\" element=\"xsd:string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"ResolverIn\">\n");
		fprintf(h,"<wsdl:part name=\"command\" element=\"xsd:string\"/>\n");
		fprintf(h,"<wsdl:part name=\"category\" element=\"xsd:string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"ResolverOut\">\n");
		fprintf(h,"<wsdl:part name=\"category\" element=\"xsd:string\"/>\n");
		fprintf(h,"<wsdl:part name=\"host\" element=\"xsd:string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"ParseManifestIn\">\n");
		fprintf(h,"<wsdl:part name=\"command\" element=\"xsd:string\"/>\n");
		fprintf(h,"<wsdl:part name=\"document\" type=\"cords:manifest\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"AsynchParseManifestIn\">\n");
		fprintf(h,"<wsdl:part name=\"command\" element=\"xsd:string\"/>\n");
		fprintf(h,"<wsdl:part name=\"document\" type=\"cords:manifest\"/>\n");
		fprintf(h,"<wsdl:part name=\"callback\" element=\"xsd:string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"ParseManifestOut\">\n");
		fprintf(h,"<wsdl:part name=\"document\" type=\"cords:manifest\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"ParseSLAIn\">\n");
		fprintf(h,"<wsdl:part name=\"command\" element=\"xsd:string\"/>\n");
		fprintf(h,"<wsdl:part name=\"document\" type=\"slam:agreement\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"AsynchParseSLAIn\">\n");
		fprintf(h,"<wsdl:part name=\"command\" element=\"xsd:string\"/>\n");
		fprintf(h,"<wsdl:part name=\"document\" type=\"slam:agreement\"/>\n");
		fprintf(h,"<wsdl:part name=\"callback\" element=\"xsd:string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"ParseSLAOut\">\n");
		fprintf(h,"<wsdl:part name=\"document\" type=\"slam:agreement\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"BrokerSLAIn\">\n");
		fprintf(h,"<wsdl:part name=\"command\" element=\"xsd:string\"/>\n");
		fprintf(h,"<wsdl:part name=\"document\" type=\"slam:agreement\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"AsynchBrokerSLAIn\">\n");
		fprintf(h,"<wsdl:part name=\"command\" element=\"xsd:string\"/>\n");
		fprintf(h,"<wsdl:part name=\"document\" type=\"slam:agreement\"/>\n");
		fprintf(h,"<wsdl:part name=\"callback\" element=\"xsd:string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"BrokerOut\">\n");
		fprintf(h,"<wsdl:part name=\"service\" element=\"xsd:string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"ServiceIn\">\n");
		fprintf(h,"<wsdl:part name=\"action\" element=\"xsd:string\"/>\n");
		fprintf(h,"<wsdl:part name=\"service\" element=\"xsd:string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"AsynchServiceIn\">\n");
		fprintf(h,"<wsdl:part name=\"action\" element=\"xsd:string\"/>\n");
		fprintf(h,"<wsdl:part name=\"service\" element=\"xsd:string\"/>\n");
		fprintf(h,"<wsdl:part name=\"callback\" element=\"xsd:string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"ServiceOut\">\n");
		fprintf(h,"<wsdl:part name=\"service\" element=\"xsd:string\"/>\n");
		fprintf(h,"<wsdl:part name=\"action\" element=\"xsd:string\"/>\n");
		fprintf(h,"<wsdl:part name=\"status\" element=\"xsd:string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"ScriptIn\">\n");
		fprintf(h,"<wsdl:part name=\"command\" element=\"xsd:string\"/>\n");
		fprintf(h,"<wsdl:part name=\"script\" element=\"xsd:string\"/>\n");
		fprintf(h,"<wsdl:part name=\"parameters\" element=\"xsd:string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"AsynchScriptIn\">\n");
		fprintf(h,"<wsdl:part name=\"command\" element=\"xsd:string\"/>\n");
		fprintf(h,"<wsdl:part name=\"script\" element=\"xsd:string\"/>\n");
		fprintf(h,"<wsdl:part name=\"parameters\" element=\"xsd:string\"/>\n");
		fprintf(h,"<wsdl:part name=\"callback\" element=\"xsd:string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"ScriptOut\">\n");
		fprintf(h,"<wsdl:part name=\"output\" element=\"xsd:string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		/* ------------------------------- */
		/* port Types describing exchanges */
		/* ------------------------------- */

		fprintf(h,"<wsdl:portType name=\"CORCS\">\n");

		/* -------------------- */
		/* Synchronous Messages */
		/* -------------------- */

		fprintf(h,"<wsdl:operation name=\"ResolveCategory\">\n");
		fprintf(h,"<wsdl:input message=\"tns:ResolverIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:ResolverOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"ParseManifest\">\n");
		fprintf(h,"<wsdl:input message=\"tns:ParseManifestIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:ParseManifestOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"ParseSLA\">\n");
		fprintf(h,"<wsdl:input message=\"tns:ParseSLAIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:ParseSLAOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"BrokerSLA\">\n");
		fprintf(h,"<wsdl:input message=\"tns:BrokerSLAIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:BrokerOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"ServiceAction\">\n");
		fprintf(h,"<wsdl:input message=\"tns:ServiceIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:ServiceOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"RunScript\">\n");
		fprintf(h,"<wsdl:input message=\"tns:ScriptIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:ScriptOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		/* --------------------- */
		/* Asynchronous Messages */
		/* --------------------- */

		fprintf(h,"<wsdl:operation name=\"AsynchParseManifest\">\n");
		fprintf(h,"<wsdl:input message=\"tns:AsynchParseManifestIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:AsynchMessageOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchParseManifestResult\">\n");
		fprintf(h,"<wsdl:input message=\"tns:AsynchMessageIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:ParseManifestOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchParseSLA\">\n");
		fprintf(h,"<wsdl:input message=\"tns:AsynchParseSLAIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:AsynchMessageOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchParseSLAResult\">\n");
		fprintf(h,"<wsdl:input message=\"tns:AsynchMessageIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:ParseSLAOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchBrokerSLA\">\n");
		fprintf(h,"<wsdl:input message=\"tns:AsynchBrokerSLAIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:AsynchMessageOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchBrokerSLAResult\">\n");
		fprintf(h,"<wsdl:input message=\"tns:AsynchMessageIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:BrokerSLAOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchServiceAction\">\n");
		fprintf(h,"<wsdl:input message=\"tns:AsynchServiceIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:AsynchMessageOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchServiceActionResult\">\n");
		fprintf(h,"<wsdl:input message=\"tns:AsynchMessageIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:ServiceOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchRunScript\">\n");
		fprintf(h,"<wsdl:input message=\"tns:AsynchScriptIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:AsynchMessageOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchRunScriptResult\">\n");
		fprintf(h,"<wsdl:input message=\"tns:AsynchMessageIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:ScriptOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"</wsdl:portType>\n");

		/* --------------------------------------- */
		/* PORT Bindings to Message Exchange Types */
		/* --------------------------------------- */
		
		fprintf(h,"<wsdl:binding name=\"SOAPCORCS\" type=\"tns:CORCS\">\n");
		fprintf(h,"<soap:binding style=\"%s\" ",sbs);
		fprintf(h,"transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n");

		fprintf(h,"<wsdl:operation name=\"ResolveCategory\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/Resolver\" style=\"%s\"/>\n",host,sbs);
		fprintf(h,"<wsdl:input message=\"tns:ResolverIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:ResolverOut\"/>\n");
		fprintf(h,"<wsdl:fault><soap:fault name=\"fault\"/></wsdl:fault>\n");
		fprintf(h,"</wsdl:operation>\n");

		/* -------------------- */
		/* Synchronous Messages */
		/* -------------------- */

		fprintf(h,"<wsdl:operation name=\"ParseManifest\">\n");
fprintf(h,"<soap:operation soapAction=\"%s/ParseManifest\" style=\"%s\"/>\n",host,sbs);
		fprintf(h,"<wsdl:input message=\"tns:ParseManifestIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:ParseManifestOut\"/>\n");
		fprintf(h,"<wsdl:fault><soap:fault name=\"fault\"/></wsdl:fault>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"ParseSLA\">\n");
fprintf(h,"<soap:operation soapAction=\"%s/ParseSLA\" style=\"%s\"/>\n",host,sbs);
		fprintf(h,"<wsdl:input message=\"tns:ParseSLAIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:ParseSLAOut\"/>\n");
		fprintf(h,"<wsdl:fault><soap:fault name=\"fault\"/></wsdl:fault>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"BrokerSLA\">\n");
fprintf(h,"<soap:operation soapAction=\"%s/BrokerSLA\" style=\"%s\"/>\n",host,sbs);
		fprintf(h,"<wsdl:input message=\"tns:BrokerSLAIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:BrokerOut\"/>\n");
		fprintf(h,"<wsdl:fault><soap:fault name=\"fault\"/></wsdl:fault>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"ServiceAction\">\n");
fprintf(h,"<soap:operation soapAction=\"%s/ServiceAction\" style=\"%s\"/>\n",host,sbs);
		fprintf(h,"<wsdl:input message=\"tns:ServiceIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:ServiceOut\"/>\n");
		fprintf(h,"<wsdl:fault><soap:fault name=\"fault\"/></wsdl:fault>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"RunScript\">\n");
fprintf(h,"<soap:operation soapAction=\"%s/RunScript\" style=\"%s\"/>\n",host,sbs);
		fprintf(h,"<wsdl:input message=\"tns:ScriptIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:ScriptOut\"/>\n");
		fprintf(h,"<wsdl:fault><soap:fault name=\"fault\"/></wsdl:fault>\n");
		fprintf(h,"</wsdl:operation>\n");

		/* --------------------- */
		/* Asynchronous Messages */
		/* --------------------- */
		fprintf(h,"<wsdl:operation name=\"AsynchParseManifest\">\n");
fprintf(h,"<soap:operation soapAction=\"%s/AsynchParseManifest\" style=\"%s\"/>\n",host,sbs);
		fprintf(h,"<wsdl:input message=\"tns:AsynchParseManifestIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:AsynchMessageOut\"/>\n");
		fprintf(h,"<wsdl:fault><soap:fault name=\"fault\"/></wsdl:fault>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchParserManifestResult\">\n");
fprintf(h,"<soap:operation soapAction=\"%s/AsynchParserManifestResult\" style=\"%s\"/>\n",host,sbs);
		fprintf(h,"<wsdl:input message=\"tns:AsynchMessageIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:ParserManifestOut\"/>\n");
		fprintf(h,"<wsdl:fault><soap:fault name=\"fault\"/></wsdl:fault>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchParseSLA\">\n");
fprintf(h,"<soap:operation soapAction=\"%s/AsynchParseSLA\" style=\"%s\"/>\n",host,sbs);
		fprintf(h,"<wsdl:input message=\"tns:AsynchParseSLAIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:AsynchMessageOut\"/>\n");
		fprintf(h,"<wsdl:fault><soap:fault name=\"fault\"/></wsdl:fault>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchParserSLAResult\">\n");
fprintf(h,"<soap:operation soapAction=\"%s/AsynchParserSLAResult\" style=\"%s\"/>\n",host,sbs);
		fprintf(h,"<wsdl:input message=\"tns:AsynchMessageIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:ParserSLAOut\"/>\n");
		fprintf(h,"<wsdl:fault><soap:fault name=\"fault\"/></wsdl:fault>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchBrokerSLA\">\n");
fprintf(h,"<soap:operation soapAction=\"%s/AsynchBrokerSLA\" style=\"%s\"/>\n",host,sbs);
		fprintf(h,"<wsdl:input message=\"tns:AsynchBrokerSLAIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:AsynchMessageOut\"/>\n");
		fprintf(h,"<wsdl:fault><soap:fault name=\"fault\"/></wsdl:fault>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchBrokerSLAResult\">\n");
fprintf(h,"<soap:operation soapAction=\"%s/AsynchBrokerSLAResult\" style=\"%s\"/>\n",host,sbs);
		fprintf(h,"<wsdl:input message=\"tns:AsynchMessageIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:BrokerOut\"/>\n");
		fprintf(h,"<wsdl:fault><soap:fault name=\"fault\"/></wsdl:fault>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchServiceAction\">\n");
fprintf(h,"<soap:operation soapAction=\"%s/AsynchServiceAction\" style=\"%s\"/>\n",host,sbs);
		fprintf(h,"<wsdl:input message=\"tns:AsynchServiceIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:AsynchMessageOut\"/>\n");
		fprintf(h,"<wsdl:fault><soap:fault name=\"fault\"/></wsdl:fault>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchServiceActionResult\">\n");
fprintf(h,"<soap:operation soapAction=\"%s/AsynchServiceActionResult\" style=\"%s\"/>\n",host,sbs);
		fprintf(h,"<wsdl:input message=\"tns:AsynchMessageIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:ServiceOut\"/>\n");
		fprintf(h,"<wsdl:fault><soap:fault name=\"fault\"/></wsdl:fault>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchRunScript\">\n");
	fprintf(h,"<soap:operation soapAction=\"%s/AsynchRunScript\" style=\"%s\"/>\n",host,sbs);
		fprintf(h,"<wsdl:input message=\"tns:AsynchScriptIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:AsynchMessageOut\"/>\n");
		fprintf(h,"<wsdl:fault><soap:fault name=\"fault\"/></wsdl:fault>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchRunScriptResult\">\n");
fprintf(h,"<soap:operation soapAction=\"%s/AsynchRunScriptResult\" style=\"%s\"/>\n",host,sbs);
		fprintf(h,"<wsdl:input message=\"tns:AsynchMessageIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"tns:ScriptOut\"/>\n");
		fprintf(h,"<wsdl:fault><soap:fault name=\"fault\"/></wsdl:fault>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"</wsdl:binding>\n");

		fprintf(h,"<wsdl:service name=\"CORCS\">\n");
		fprintf(h,"<wsdl:documentation>The Accords Platform Remote Command Service SOAP API</wsdl:documentation>\n"); 
		fprintf(h,"<wsdl:port name=\"AccordsPlatformRCS\"");
		fprintf(h," binding=\"tns:SOAPCORCS\">\n");
		fprintf(h,"<soap:address location=\"%s\"/>\n",host);
		fprintf(h,"</wsdl:port>\n");
		fprintf(h,"</wsdl:service>\n");

		fprintf(h,"</wsdl:definitions>\n");
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

