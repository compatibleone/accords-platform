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

		fprintf(h,"<wsdl:message name=\"AsynchMessageIn\">\n");
		fprintf(h,"<wsdl:part name=\"identifier\" type=\"string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

		fprintf(h,"<wsdl:message name=\"AsynchMessageOut\">\n");
		fprintf(h,"<wsdl:part name=\"identifier\" type=\"string\"/>\n");
		fprintf(h,"</wsdl:message>\n");

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

		/* ------------------------------- */
		/* port Types describing exchanges */
		/* ------------------------------- */

		fprintf(h,"<wsdl:portType name=\"CORCS\">\n");

		/* -------------------- */
		/* Synchronous Messages */
		/* -------------------- */

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

		/* --------------------- */
		/* Asynchronous Messages */
		/* --------------------- */

		fprintf(h,"<wsdl:operation name=\"AsynchParseManifest\">\n");
		fprintf(h,"<wsdl:input message=\"ParseManifestIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"AsynchMessageOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchParseManifestResult\">\n");
		fprintf(h,"<wsdl:input message=\"AsynchMessageIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"ParseManifestOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchParseSLA\">\n");
		fprintf(h,"<wsdl:input message=\"ParseSLAIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"AsynchMessageOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchParseSLAResult\">\n");
		fprintf(h,"<wsdl:input message=\"AsynchMessageIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"ParseSLAOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchBrokerSLA\">\n");
		fprintf(h,"<wsdl:input message=\"BrokerSLAIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"AsynchMessageOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchBrokerSLAResult\">\n");
		fprintf(h,"<wsdl:input message=\"AsynchMessageIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"BrokerSLAOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchServiceAction\">\n");
		fprintf(h,"<wsdl:input message=\"ServiceIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"AsynchMessageOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchServiceActionResult\">\n");
		fprintf(h,"<wsdl:input message=\"AsynchMessageIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"ServiceOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchRunScript\">\n");
		fprintf(h,"<wsdl:input message=\"ScriptIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"AsynchMessageOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchRunScriptResult\">\n");
		fprintf(h,"<wsdl:input message=\"AsynchMessageIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"ScriptOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"</wsdl:portType>\n");

		/* --------------------------------------- */
		/* PORT Bindings to Message Exchange Types */
		/* --------------------------------------- */
		
		fprintf(h,"<wsdl:binding name=\"SOAPCORCS\" type=\"CORCS\">\n");
		fprintf(h,"<soap:binding style=\"rpc\" ");
		fprintf(h,"transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n");

		fprintf(h,"<wsdl:operation name=\"ResolveCategory\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/Resolver\" style=\"rpc\"/>\n",host);
		fprintf(h,"<wsdl:input message=\"ResolverIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"ResolverOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		/* -------------------- */
		/* Synchronous Messages */
		/* -------------------- */

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

		fprintf(h,"<wsdl:operation name=\"BrokerSLA\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/BrokerSLA\" style=\"rpc\"/>\n",host);
		fprintf(h,"<wsdl:input message=\"BrokerSLAIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"BrokerOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"ServiceAction\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/ServiceAction\" style=\"rpc\"/>\n",host);
		fprintf(h,"<wsdl:input message=\"ServiceIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"ServiceOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"RunScript\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/RunScript\" style=\"rpc\"/>\n",host);
		fprintf(h,"<wsdl:input message=\"ScriptIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"ScriptOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		/* --------------------- */
		/* Asynchronous Messages */
		/* --------------------- */
		fprintf(h,"<wsdl:operation name=\"AsynchParseManifest\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/AsynchParseManifest\" style=\"rpc\"/>\n",host);
		fprintf(h,"<wsdl:input message=\"ParseManifestIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"AsynchMessageOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchParserManifestResult\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/AsynchParserManifestResult\" style=\"rpc\"/>\n",host);
		fprintf(h,"<wsdl:input message=\"AsynchMessageIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"ParserManifestOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchParseSLA\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/AsynchParseSLA\" style=\"rpc\"/>\n",host);
		fprintf(h,"<wsdl:input message=\"ParseSLAIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"AsynchMessageOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchParserSLAResult\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/AsynchParserSLAResult\" style=\"rpc\"/>\n",host);
		fprintf(h,"<wsdl:input message=\"AsynchMessageIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"ParserSLAOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchBrokerSLA\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/AsynchBrokerSLA\" style=\"rpc\"/>\n",host);
		fprintf(h,"<wsdl:input message=\"BrokerSLAIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"AsynchMessageOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchBrokerSLAResult\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/AsynchBrokerSLAResult\" style=\"rpc\"/>\n",host);
		fprintf(h,"<wsdl:input message=\"AsynchMessageIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"BrokerOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchServiceAction\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/AsynchServiceAction\" style=\"rpc\"/>\n",host);
		fprintf(h,"<wsdl:input message=\"ServiceIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"AsynchMessageOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchServiceActionResult\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/AsynchServiceActionResult\" style=\"rpc\"/>\n",host);
		fprintf(h,"<wsdl:input message=\"AsynchMessageIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"ServiceOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchRunScript\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/AsynchRunScript\" style=\"rpc\"/>\n",host);
		fprintf(h,"<wsdl:input message=\"ScriptIn\"/>\n");
		fprintf(h,"<wsdl:output message=\"AsynchMessageOut\"/>\n");
		fprintf(h,"</wsdl:operation>\n");

		fprintf(h,"<wsdl:operation name=\"AsynchRunScriptResult\">\n");
		fprintf(h,"<soap:operation soapAction=\"%s/AsynchRunScriptResult\" style=\"rpc\"/>\n",host);
		fprintf(h,"<wsdl:input message=\"AsynchMessageIn\"/>\n");
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

/*	-------------------------------------	*/
/*	c o r c s _ s o a p _ g e t _ w s s l 	*/
/*	-------------------------------------	*/
public	char * corcs_soap_get_wsdl()
{
	char 	host[1024];
	sprintf(host,"%s",get_identity());
	return( corcs_soap_wsdl(host) );
}

	/* ------------ */
#endif	/* _corc_wsdl_c */
	/* ------------ */

