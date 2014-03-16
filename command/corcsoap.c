#ifndef	_corcs_soap_c
#define	_corcs_soap_c
#include "command.h"

#include "corcasynch.c"
#include "corcwsdl.c"
#include "corcxml.c"

private	void *	corcs_thread_message( void * vptr );

#include "corcsthread.c"

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
/*	      c o r c s _ f a u l t _ r e s p o n s e	 		*/
/*	-------------------------------------------------------		*/
private	struct rest_response * corcs_fault_response( 
	struct rest_response * aptr, 
	int status, 
	char * reason, 
	char * detail )
{
	FILE * h;
	char *	filename;
	if (!( filename = rest_temporary_filename( "xml" ) ))
		return( aptr );
	else if (!( h = fopen( filename, "w" ) ))
		return( liberate( filename ) );
	else
	{
		soap_fault_message( h, status, reason, detail );
		fclose(h);
		return( rest_file_response( aptr, filename, "text/xml" ) );
	}
}

/*	-------------------------------------------------------		*/
/*	      c o r c s _ p a r s e r _ r e s p o n s e	 		*/
/*	-------------------------------------------------------		*/
private	struct rest_response * corcs_parser_response( struct rest_response * aptr, char * document, char * message )
{
	FILE * h;
	char *	filename;
	char *	plan;
	char 	buffer[1024];
	sprintf(buffer,"%sResponse",message);
	if (!( plan = parser_name_plan(document) ))
		return( aptr );
	else if (!( filename = rest_temporary_filename( "xml" ) ))
		return( aptr );
	else if (!( h = fopen( filename, "w" ) ))
		return( liberate( filename ) );
	else
	{
		soap_message_header( h, buffer );
		soap_inline_xml( h, plan );
		soap_message_footer( h, buffer );
		fclose(h);
		return( rest_file_response( aptr, filename, "text/xml" ) );
	}
}

/*	-------------------------------------------------------		*/
/*	      c o r c s _ c o n v e r t o r _ r e s p o n s e	 	*/
/*	-------------------------------------------------------		*/
private	struct rest_response * corcs_convertor_response( struct rest_response * aptr, char * document, char * message, int mode )
{
	FILE * h;
	char *	filename;
	char 	buffer[1024];
	char 	plan[1024];
	sprintf(buffer,"%sResponse",message);
	sprintf(plan,"%s-%s",document,(mode == 1 ? "manifest" : "agreement"));	
	if (!( filename = rest_temporary_filename( "xml" ) ))
		return( aptr );
	else if (!( h = fopen( filename, "w" ) ))
		return( liberate( filename ) );
	else
	{
		soap_message_header( h, buffer );
		soap_inline_xml( h, plan );
		soap_message_footer( h, buffer );
		fclose(h);
		return( rest_file_response( aptr, filename, "text/xml" ) );
	}
}

/*	-------------------------------------------------------		*/
/*	      c o r c s _ a s y n c h _ c a l l b a c k  		*/
/*	-------------------------------------------------------		*/
private	struct rest_response * corcs_asynch_callback( struct rest_response * aptr, struct corcs_asynch_request * qptr )
{
	char	buffer[2048];
	char *	filename;
	sprintf(buffer,"%s %u %s %s",
		qptr->identity,
		aptr->status,
		(aptr->message ? aptr->message : "MSG"),
		(aptr->body ? aptr->body : "" ));
	filename = cords_script_interpreter( qptr->callback, buffer, 1 );
	qptr->response = (struct rest_response *) 0;
	qptr->status   = _HTTP_NO_CONTENT;
	qptr->message  = allocate_string("response delivered by callback");
	return( aptr );
}

/*	-------------------------------------------------------		*/
/*	    c o r c s _ p e r s i s t e n t _ r e s u l t 	 	*/
/*	-------------------------------------------------------		*/
private	char * corcs_persistent_result( struct rest_response * aptr, char * message, struct corcs_asynch_request * qqptr )
{
	char	*	ihost;
	struct	occi_client * kptr;
	struct	occi_request * qptr;
	struct	occi_response * yptr;
	struct	occi_response * zptr;
	struct	occi_element * dptr;
	struct	xml_element * eptr;
	char  *	result=(char *) 0;
	char	buffer[2048];
	char	work1[64];
	char	work2[64];
	char 	when[64];

	if (!( ihost = occi_resolve_category_provider( "result", get_default_agent(), default_tls() ) ))
		return((char *) 0);
	else
	{
		sprintf(buffer,"%s/%s/",ihost,"result");
		liberate( ihost );
	}

	if (!( kptr = occi_create_client( buffer, get_default_agent(), default_tls()) ))
		return((char *) 0);

	else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
	{
		kptr = occi_remove_client( kptr );
		return((char *) 0);
	}
	else 
	{
		sprintf(work1,"%u",aptr->status);
		sprintf(work2,"%u",qqptr->status);
		sprintf(when,"%u",time((long*) 0));
		if ((!(dptr=occi_request_element(qptr,"occi.result.code"    , work1 			   	) ))
	     	||  (!(dptr=occi_request_element(qptr,"occi.result.name"    , qqptr->identity			) ))
	     	||  (!(dptr=occi_request_element(qptr,"occi.result.action"  , qqptr->action			) ))
	     	||  (!(dptr=occi_request_element(qptr,"occi.result.state"   , work2				) ))
	     	||  (!(dptr=occi_request_element(qptr,"occi.result.created" , when				) ))
	     	||  (!(dptr=occi_request_element(qptr,"occi.result.message" , (aptr->message?aptr->message:"") 	) ))
	     	||  (!(dptr=occi_request_element(qptr,"occi.result.body"    , (aptr->body?aptr->body:"")       	) ))
	     	||  (!(dptr=occi_request_element(qptr,"occi.result.callback", (qqptr->callback?qqptr->callback:"")) ))
	     	||  (!(dptr=occi_request_element(qptr,"occi.result.request" , message	 		        ) )))
		{
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			return((char *) 0);
		}
		else if (!( yptr = occi_client_post( kptr, qptr ) ))
		{
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			return((char *) 0);
		}
		else if (!( ihost = occi_extract_location( yptr ) ))
		{
			yptr = occi_remove_response( yptr );
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			return((char *) 0);
		}
		else
		{
			rest_add_http_prefix(buffer,2048,ihost);
			yptr = occi_remove_response( yptr );
			qptr = occi_remove_request( qptr );
			kptr = occi_remove_client( kptr );
			if (!( result = allocate_string( buffer ) ))
				return( result );
			else	return( result );
		}
	}	
}

/*	-------------------------------------------------------		*/
/*	      c o r c s _ a s y n c h _ r e s p o n s e	 		*/
/*	-------------------------------------------------------		*/
private	struct rest_response * corcs_asynch_response( struct rest_response * aptr, char * message, struct corcs_asynch_request * qptr )
{
	char 	buffer[1024];
	sprintf(buffer,"%sResponse",message);
	if (!( qptr ))
		return( aptr );
	else if (!( qptr->message = allocate_string( buffer ) ))
		return( aptr );
	else
	{
		qptr->response = aptr;
		qptr->status   = _HTTP_OK;
		corcs_persistent_result( aptr, message, qptr );
		if (!( qptr->callback ))
			return( aptr );
		else	return( corcs_asynch_callback( aptr, qptr ) );
	}
}

/*	-------------------------------------------------------		*/
/*	      c o r c s _ s c r i p t _ r e s p o n s e	 		*/
/*	-------------------------------------------------------		*/
private	struct rest_response * corcs_script_response( struct rest_response * aptr, char * document, char * message )
{
	FILE * h;
	char *	filename;
	char 	buffer[1024];
	sprintf(buffer,"%sResponse",message);
	if (!( filename = rest_temporary_filename( "xml" ) ))
		return( aptr );
	else if (!( h = fopen( filename, "w" ) ))
		return( liberate( filename ) );
	else
	{
		soap_message_header( h, buffer );
		fprintf(h,"<m:output>\n");
		soap_inline_file( h, document );
		fprintf(h,"</m:output>\n");
		soap_message_footer( h, buffer );
		fclose(h);
		return( rest_file_response( aptr, filename, "text/xml" ) );
	}
}

/*	-------------------------------------------------------		*/
/*	 c o r c s _ j s o n _ s e r v i c e _ i d e n t i t y		*/
/*	-------------------------------------------------------		*/
private	char *	corcs_json_service_identity( char * filename )
{
	char *	eptr;
	struct	data_element * dptr;
	if (!( dptr = json_parse_file( filename )))
		return((char *) 0);
	else if (!( eptr = json_atribut( dptr, "instance" ) ))
	{
		dptr = drop_data_element( dptr );
		return((char *) 0);
	}
	else if (!( eptr = allocate_string( eptr ) ))
	{
		dptr = drop_data_element( dptr );
		return((char *) 0);
	}
	else
	{
		dptr = drop_data_element( dptr );
		return(eptr);
	}
}

/*	-------------------------------------------------------		*/
/*	      c o r c s _ b r o k e r _ r e s p o n s e	 		*/
/*	-------------------------------------------------------		*/
private	struct rest_response * corcs_broker_response( struct rest_response * aptr, char * document, char * message  )
{
	FILE * h;
	char *	filename;
	char *	service;
	char 	buffer[1024];
	sprintf(buffer,"%sResponse",message);
	if (!( filename = rest_temporary_filename( "xml" ) ))
		return( aptr );
	else if (!( h = fopen( filename, "w" ) ))
		return( liberate( filename ) );
	else
	{
		soap_message_header( h, buffer );
		if (!( document = broker_name_instance( document ) ))
			fprintf(h,"<m:fault>internal error</m:fault>\n");
		else if (!( service = corcs_json_service_identity( document )))
		{
			fprintf(h,"<m:json>\n");
			soap_inline_file( h, document );
			fprintf(h,"</m:json>\n");
			document = liberate( document );
		}
		else
		{
			fprintf(h,"<m:service>%s</m:service>\n",service);
			service = liberate( service );
		}
		soap_message_footer( h, buffer );
		fclose(h);
		return( rest_file_response( aptr, filename, "text/xml" ) );
	}
}

/*	--------------------------------------------------------	*/
/*	 c o r c s _  s o a p _ s e r v i c e _ r e s p o n s e	 	*/
/*	--------------------------------------------------------	*/
private	char *	corcs_soap_service_response( char * action, char * service, char * message )
{
	FILE * h;
	char *	filename;
	char 	buffer[1024];
	strcpy(buffer,"ServiceActionResponse");

	if (!( filename = rest_temporary_filename( "xml" ) ))
		return( (char *) 0);
	else if (!( h = fopen( filename, "w" ) ))
		return( liberate( filename ) );
	else
	{
		soap_message_header( h, buffer );
		fprintf(h,"<m:service>%s</m:service>\n",service);
		fprintf(h,"<m:action>%s</m:action>\n",action);
		fprintf(h,"<m:state>%s</m:state>\n",message);
		soap_message_footer( h, buffer );
		fclose(h);
		return( filename );
	}
}

/*	-------------------------------------------------------		*/
/*	      c o r c s _  s e r v i c e _ r e s p o n s e	 	*/
/*	-------------------------------------------------------		*/
private	struct rest_response * corcs_service_response( struct rest_response * aptr, char * filename )
{
	return( rest_file_response( aptr, filename, "text/xml" ) );
}

/*	-------------------------------------	*/
/*	c o r d s _ s o a p _ r e s o l v e r	*/
/*	-------------------------------------	*/
private	struct	rest_response * corcs_soap_resolver( struct rest_response * aptr, struct xml_element * sptr, struct rest_request * rptr )
{
	char *	category=(char *) 0;
	char *	command=(char *) 0;
	char *	filename=(char *) 0;
	char *	message="ResolveCategory";
	if (!( sptr ))
		return(corcs_fault_response(aptr,800,message,"missing request"));
	else if (!( rptr ))
		return(corcs_fault_response(aptr,800,message,"missing request"));
	else if (!( category = document_element_string( sptr, "category") ))
		return(corcs_fault_response(aptr,801,message,"missing category"));
	else if (!( command = document_element_string( sptr, "command") ))
		return(corcs_fault_response(aptr,802,message,"missing command"));
	else if ( strcmp( command, "resolver" ) != 0)
		return(corcs_fault_response(aptr,803,message,"incorrect command"));
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
/*	  c o r d s _ s o a p _ m a n i f e s t _ c o n v e r 	*/
/*	-----------------------------------------------------	*/
private	struct	rest_response * corcs_soap_manifest_convertor( 
	struct 	rest_response * aptr, 
	struct 	xml_element * sptr, 
	struct 	rest_request * rptr,
	struct	corcs_asynch_request * qptr )
{
	char *	message="ConvertManifest";
	char *	command=(char *) 0;
	char *	filename=(char *) 0;
	char *	username=(char *) 0;
	char *	password=(char *) 0;
	if (!( sptr ))
		return(corcs_fault_response(aptr,800,message,"missing request"));
	else if (!( rptr ))
		return(corcs_fault_response(aptr,800,message,"missing request"));
	else if (!( filename = document_element_string( sptr, "url") ))
		return(corcs_fault_response(aptr,801,message,"missing document or url"));
	if (!( command = document_element_string( sptr, "command") ))
		return(corcs_fault_response(aptr,802,message,"missing command"));
	else if ( strcmp( command, "convert" ) != 0)
		return(corcs_fault_response(aptr,803,message,"incorrect command"));
	else
	{
		username = document_element_string( sptr, "username");
		password = document_element_string( sptr, "password");
		if (!( filename = fetch_url( filename, username, password ) ))
			return(corcs_fault_response(aptr,804,message,"cannot fetch document"));

		/* convert filename */
		/* ---------------- */
		cords_convertor_operation( filename, 1 );
		sptr = liberate_xml_element( sptr );
		if (!( aptr = corcs_convertor_response ( aptr, filename, message,1 ) ))
			return( aptr );
		else if (!( qptr ))
			return( aptr );
		else	return( corcs_asynch_response( aptr, message, qptr ) );
	}
}


/*	-----------------------------------------------------	*/
/*	  c o r d s _ s o a p _ m a n i f e s t _ p a r s e r 	*/
/*	-----------------------------------------------------	*/
private	struct	rest_response * corcs_soap_manifest_parser( 
	struct 	rest_response * aptr, 
	struct 	xml_element * sptr, 
	struct 	rest_request * rptr,
	struct	corcs_asynch_request * qptr )
{
	char *	message="ParseManifest";
	char *	command=(char *) 0;
	char *	filename=(char *) 0;
	if (!( sptr ))
		return(corcs_fault_response(aptr,800,message,"missing request"));
	else if (!( rptr ))
		return(corcs_fault_response(aptr,800,message,"missing request"));
	else if (!( filename = document_element_xml( sptr, "manifest") ))
		return(corcs_fault_response(aptr,801,message,"missing manifest"));
	if (!( command = document_element_string( sptr, "command") ))
		return(corcs_fault_response(aptr,802,message,"missing command"));
	else if ( strcmp( command, "parser") != 0)
		return(corcs_fault_response(aptr,803,message,"incorrect command"));
	else
	{
		/* parse filename */
		/* ---------------- */
		cords_parser_operation( filename );
		sptr = liberate_xml_element( sptr );
		if (!( aptr = corcs_parser_response ( aptr, filename, message ) ))
			return( aptr );
		else if (!( qptr ))
			return( aptr );
		else	return( corcs_asynch_response( aptr, message, qptr ) );
	}
}

/*	-------------------------------------	*/
/*	  c o r d s _ s o a p _ c o n v e r t	*/
/*	-------------------------------------	*/
private	struct	rest_response * corcs_soap_sla_convertor( 
	struct 	rest_response * aptr, 
	struct 	xml_element * sptr, 
	struct 	rest_request * rptr,
	struct	corcs_asynch_request * qptr )
{
	char *	message="ConvertSLA";
	char *	command=(char *) 0;
	char *	filename=(char *) 0;
	char *	username=(char *) 0;
	char *	password=(char *) 0;
	if (!( sptr ))
		return(corcs_fault_response(aptr,800,message,"missing request"));
	else if (!( rptr ))
		return(corcs_fault_response(aptr,800,message,"missing request"));
	else if (!( filename = document_element_string( sptr, "url") ))
		return(corcs_fault_response(aptr,801,message,"missing agreement"));
	else if (!( command = document_element_string( sptr, "command") ))
		return(corcs_fault_response(aptr,802,message,"missing command"));
	else if ( strcmp( command, "convert" ) != 0)
		return(corcs_fault_response(aptr,803,message,"incorrect command"));
	else
	{
		username = document_element_string( sptr, "username");
		password = document_element_string( sptr, "password");
		if (!( filename = fetch_url( filename, username, password ) ))
			return(corcs_fault_response(aptr,804,message,"cannot fetch document"));
		cords_convertor_operation( filename, 2 );
		sptr = liberate_xml_element( sptr );
		if (!( aptr = corcs_convertor_response ( aptr, filename, message,2 ) ))
			return( aptr );
		else if (!( qptr ))
			return( aptr );
		else	return( corcs_asynch_response( aptr, message, qptr ) );
	}
}

/*	-------------------------------------	*/
/*	  c o r d s _ s o a p _ p a r s e r 	*/
/*	-------------------------------------	*/
private	struct	rest_response * corcs_soap_sla_parser( 
	struct 	rest_response * aptr, 
	struct 	xml_element * sptr, 
	struct 	rest_request * rptr,
	struct	corcs_asynch_request * qptr )
{
	char *	message="ParseSLA";
	char *	command=(char *) 0;
	char *	filename=(char *) 0;
	if (!( sptr ))
		return(corcs_fault_response(aptr,800,message,"missing request"));
	else if (!( rptr ))
		return(corcs_fault_response(aptr,800,message,"missing request"));
	else if (!( filename = document_element_xml( sptr, "agreement") ))
		return(corcs_fault_response(aptr,801,message,"missing agreement"));
	else if (!( command = document_element_string( sptr, "command") ))
		return(corcs_fault_response(aptr,802,message,"missing command"));
	else if ( strcmp( command, "parser" ) != 0)
		return(corcs_fault_response(aptr,803,message,"incorrect command"));
	else
	{
		/* parse filename */
		/* -------------- */
		cords_parser_operation( filename );
		sptr = liberate_xml_element( sptr );
		if (!( aptr = corcs_parser_response ( aptr, filename, message ) ))
			return( aptr );
		else if (!( qptr ))
			return( aptr );
		else	return( corcs_asynch_response( aptr, message, qptr ) );
	}
}

/*	-----------------------------------------------------	*/
/*	      c o r d s _ s o a p _ s l a _ b r o k e r 	*/
/*	-----------------------------------------------------	*/
private	struct	rest_response * corcs_soap_sla_broker(
	struct 	rest_response * aptr, 
	struct 	xml_element * sptr, 
	struct 	rest_request * rptr,
	struct	corcs_asynch_request * qptr )
{
	char *	message="BrokerSLA";
	char *	filename=(char *) 0;
	char *	command=(char *) 0;
	char *	deployment=(char *) 0;
	char *	name=(char *) 0;
	if (!( sptr ))
		return(corcs_fault_response(aptr,800,message,"missing request"));
	else if (!( rptr ))
		return(corcs_fault_response(aptr,800,message,"missing request"));
	else if (!( filename = document_element_xml( sptr, "agreement") ))
		return(corcs_fault_response(aptr,801,message,"missing agreement"));
	else if (!( command = document_element_string( sptr, "command") ))
		return(corcs_fault_response(aptr,802,message,"missing command"));
	else if ( strcmp( command, "broker" ) != 0)
		return(corcs_fault_response(aptr,802,message,"incorrect command"));
	else
	{
		if (!( name = document_element_string( sptr, "name") ))
			name = "";

		/* test the deployment option */
		if (!( deployment = document_element_string( sptr, "deployment")))
			deployment = "1";
		else if (!( rest_valid_string( deployment ) ))
			deployment = "1";
		else if (!( strcasecmp( deployment,"IMMEDIATE" ) ))
			deployment = "1";
		else if (!( strcasecmp( deployment,"DIFFERED" ) ))
			deployment = "0";
		else if (!( strcasecmp( deployment,"YES" ) ))
			deployment = "1";
		else if (!( strcasecmp( deployment,"NO" ) ))
			deployment = "0";
		else if (!( strcasecmp( deployment,"TRUE" ) ))
			deployment = "1";
		else if (!( strcasecmp( deployment,"FALSE" ) ))
			deployment = "0";
		else	deployment = "1";

	
		/* broker filename */
		/* --------------- */
		cords_broker_operation( filename, atoi(deployment), name );
		sptr = liberate_xml_element( sptr );
		if (!( aptr = corcs_broker_response ( aptr, filename, message ) ))
			return( aptr );
		else if (!( qptr ))
			return( aptr );
		else	return( corcs_asynch_response( aptr, message, qptr ) );
	}
}

/*	-------------------------------------	*/
/*	 c o r d s _ s o a p _ s e r v i c e 	*/
/*	-------------------------------------	*/
private	struct	rest_response * corcs_soap_service( 
	struct 	rest_response * aptr, 
	struct 	xml_element * sptr, 
	struct 	rest_request * rptr,
	struct	corcs_asynch_request * qptr )
{
	char *	service=(char *) 0;
	char *	filename=(char *) 0;
	char *	action=(char *) 0;
	char *	message="ServiceAction";
	if (!( sptr ))
		return(corcs_fault_response(aptr,800,message,"missing request"));
	else if (!( rptr ))
		return(corcs_fault_response(aptr,800,message,"missing request"));
	else if (!( service = document_element_string( sptr, "service") ))
		return(corcs_fault_response(aptr,801,message,"missing service"));
	else if (!( action = document_element_string( sptr, "action") ))
		return(corcs_fault_response(aptr,802,message,"missing action"));
	else
	{
		/* invoke service action */
		/* --------------------- */
		filename = cords_service_operation( action, service );
		sptr = liberate_xml_element( sptr );
		if (!( aptr = corcs_service_response ( aptr, filename ) ))
			return( aptr );
		else if (!( qptr ))
			return( aptr );
		else	return( corcs_asynch_response( aptr, message, qptr ) );
	}
}

/*	-------------------------------------	*/
/*	  c o r d s _ s o a p _ s c r i p t 	*/
/*	-------------------------------------	*/
private	struct	rest_response * corcs_soap_script( 
	struct 	rest_response * aptr, 
	struct 	xml_element * sptr, 
	struct 	rest_request * rptr,
	struct	corcs_asynch_request * qptr )
{
	char *	filename=(char *) 0;
	char *	command=(char *) 0;
	char *	parameters=(char *) 0;
	char *	message="RunScript";
	if (!( sptr ))
		return(corcs_fault_response(aptr,800,message,"missing request"));
	else if (!( rptr ))
		return(corcs_fault_response(aptr,800,message,"missing request"));
	else if (!( filename = document_element_file( sptr, "script") ))
		return(corcs_fault_response(aptr,801,message,"missing script"));
	else if (!( command = document_element_string( sptr, "command") ))
		return(corcs_fault_response(aptr,802,message,"missing command"));
	else if (!( parameters= document_element_string( sptr, "parameters") ))
		return(corcs_fault_response(aptr,803,message,"missing parameters"));
	else if ( strcmp( command, "script" ) != 0)
		return(corcs_fault_response(aptr,804,message,"incorrect command"));
	else
	{
		/* run script */
		/* ---------- */
		filename = cords_script_interpreter( filename, parameters, 0 );
		sptr = liberate_xml_element( sptr );
		if (!( aptr = corcs_script_response ( aptr, filename, message ) ))
			return( aptr );
		else if (!( qptr ))
			return( aptr );
		else	return( corcs_asynch_response( aptr, message, qptr ) );
	}
}

/*	-------------------------------------	*/
/*	    c o r d s _ s o a p _ o c c i 	*/
/*	-------------------------------------	*/
private	struct	rest_response * corcs_soap_occi( 
	struct 	rest_response * aptr, 
	struct 	xml_element * sptr, 
	struct 	rest_request * rptr,
	struct	corcs_asynch_request * qptr )
{
	char *	category=(char *) 0;
	char *	command=(char *) 0;
	char *	message="OCCI";
	if (!( sptr ))
		return(corcs_fault_response(aptr,800,message,"missing request"));
	else if (!( rptr ))
		return(corcs_fault_response(aptr,800,message,"missing request"));
	else if (!( category = document_element_string( sptr, "category") ))
		return(corcs_fault_response(aptr,801,message,"missing category"));
	else if (!( command = document_element_string( sptr, "command") ))
		return(corcs_fault_response(aptr,802,message,"missing command"));
	else
	{
		sptr = liberate_xml_element( sptr );
		return( rest_html_response( aptr, _HTTP_OK, "OK" ) );
	}
}

/*	---------------------------------------		*/
/*	c o r c s _ t h r e a d _ m e s s a g e 	*/
/*	---------------------------------------		*/
private	void *	corcs_thread_message( void * vptr )
{
	struct corcs_thread * tptr=vptr;
	int	status;
	struct corcs_asynch_context * rptr;

	rest_log_message("thread: pthread detach");
	pthread_detach( tptr->id );
	while(tptr->status)
	{
		/* await job for thread */
		/* -------------------- */
		rest_log_message("thread: awaiting job message");
		while(1)
		{
			lock_corcs_thread(tptr);
			rptr = tptr->request;
			unlock_corcs_thread( tptr );
			if (!( rptr ))
				wait_corcs_thread(tptr);
			else	break;
		}
		
		/* process thread job */
		/* ------------------ */
		rest_log_message("thread: corcs process message start");
		rptr->result->response = (*rptr->method)(rptr->response, rptr->message,rptr->request, rptr->result);
		rest_log_message("thread: corcs process message exit");

		/* terminate thread job */
		/* -------------------- */
		lock_corcs_thread( tptr );
		tptr->request = (struct corcs_asynch_context *) 0;
		unlock_corcs_thread( tptr );
		liberate( rptr );
	}
	tptr->status = 0;
	rest_log_message("thread: corcs pthread exit");
	pthread_exit((void *) 0);
}

/*	-----------------------------------------------------	*/
/*	c o r c s _ a s y n c h r o n o u s _ r e s p o n s e	*/
/*	-----------------------------------------------------	*/
private	struct	rest_response *	corcs_asynchronous_response(
	char *	action,
	struct corcs_asynch_request * qptr 
	)
{
	FILE * h;	
	char	buffer[1024];
	char *	filename;
	struct	rest_response * aptr;

	sprintf(buffer,"%sResponse",action);

	if (!( aptr = allocate_rest_response(202,"Accepted") ))
		return( aptr );
	else if (!( filename = rest_temporary_filename( "xml" ) ))
		return( aptr );
	else if (!( h = fopen( filename, "w" ) ))
		return( liberate( filename ) );
	else
	{
		soap_message_header( h, buffer );
		fprintf(h,"<m:identity>%s</m:identity>\n",qptr->identity);
		soap_message_footer( h, buffer );
		fclose(h);
		return( rest_file_response( aptr, filename, "text/xml" ) );
	}
}

/*	---------------------------------------------------	*/
/*	c o r c s _ a s y n c h r o n o u s _ r e q u e s t	*/
/*	---------------------------------------------------	*/
private	struct	rest_response *	corcs_asynchronous_request(  
	struct rest_response * (*method)( 
		struct rest_response * aptr, 
		struct xml_element * sptr, 
		struct rest_request * rptr,
		struct corcs_asynch_request * qptr ),
	char *	action,
	struct rest_response * aptr, 
	struct xml_element * sptr, 
	struct rest_request * rptr )
{
	struct	corcs_thread * tptr;
	struct	corcs_asynch_request * qptr;
	struct	corcs_asynch_context * cptr;
	struct	rest_response * xptr;
	if (( action ) &&  ( *action == '/' )) action++;
	if (!( qptr = add_corcs_asynch_request(action) ))
		return(rest_html_response(aptr, _HTTP_BAD_REQUEST, "asynch request not yet available"));
	else if (!( cptr = allocate_asynch_context( qptr ) ))
		return(rest_html_response(aptr, _HTTP_BAD_REQUEST, "asynch request not yet available"));
	{
		qptr->callback= document_element_string( sptr, "callback");
		cptr->method  = method;
		cptr->message = sptr;
		cptr->request = rptr;
		cptr->response= aptr;
		if (!( tptr = corcs_start_thread( cptr ) ))
			return(rest_html_response(aptr, 501, "asynch request start failure"));
		else	return(corcs_asynchronous_response( action, qptr ));
	}
}

/*	-----------------------------------------------------	*/
/*	c o r c s _ a s y n c h r o n o u s _ r e s p o n s e	*/
/*	-----------------------------------------------------	*/
private	struct	rest_response *	corcs_asynchronous_result(
	char * soapaction,
	struct rest_response * aptr, 
	struct xml_element * sptr, 
	struct rest_request * rptr )
{
	struct	corcs_asynch_request * qptr;
	struct	rest_response * wptr;
	char *	identity=(char *) 0;
	char 	buffer[1024];
	char *	message="AynchResult";
	sprintf(buffer,"%sResponse",soapaction);
	if (!( sptr ))
		return(corcs_fault_response(aptr,800,message,"missing request"));
	else if (!( rptr ))
		return(corcs_fault_response(aptr,800,message,"missing request"));
	else if (!( identity = document_element_string( sptr, "identity") ))
		return(corcs_fault_response(aptr,801,message,"missing identity"));
	else if (!( qptr = find_corcs_asynch_request( identity )))
		return(corcs_fault_response(aptr,802,message,"incorrect identity"));
	else if ((wptr = qptr->response) != (struct rest_response *) 0)
	{
		qptr->response = (struct rest_response *) 0;
		qptr->status   = _HTTP_NO_CONTENT;
		qptr->message  = allocate_string("response already consumed");
		return( wptr );
	}
	else if (!( qptr->status ))
		return(rest_html_response(aptr, _HTTP_NO_CONTENT, "response not yet available"));
	else	return(rest_html_response(aptr, qptr->status, qptr->message ) );

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

	/* ------------------------------------- */
	/* check request and resolve soap action */
	/* ------------------------------------- */
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

	/* ------------------------------------------- */
	/* check request body and dispatch soap action */
	/* ------------------------------------------- */
	if (!( rptr->body ))
		return(rest_html_response(aptr, 400, "missing request body"));
	else if (!( sptr = corcs_xml_element( rptr->body ) ))
		return(rest_html_response(aptr, 400, "incorrect request body"));
	else if (!( strcmp(  command, "Resolver" ) ))
		return( corcs_soap_resolver( aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "ParseManifest" ) ))
		return( corcs_soap_manifest_parser( aptr, sptr, rptr, (struct corcs_asynch_request *) 0 ) );
	else if (!( strcmp(  command, "ConvertManifest" ) ))
		return( corcs_soap_manifest_convertor( aptr, sptr, rptr, (struct corcs_asynch_request *) 0 ) );
	else if (!( strcmp(  command, "ParseSLA" ) ))
		return( corcs_soap_sla_parser( aptr, sptr, rptr, (struct corcs_asynch_request *) 0 ) );
	else if (!( strcmp(  command, "ConvertSLA" ) ))
		return( corcs_soap_sla_convertor( aptr, sptr, rptr, (struct corcs_asynch_request *) 0 ) );
	else if (!( strcmp(  command, "BrokerSLA" ) ))
		return( corcs_soap_sla_broker( aptr, sptr, rptr, (struct corcs_asynch_request *) 0 ) );
	else if (!( strcmp(  command, "ServiceAction" ) ))
		return( corcs_soap_service( aptr, sptr, rptr, (struct corcs_asynch_request *) 0 ) );
	else if (!( strcmp(  command, "RunScript" ) ))
		return( corcs_soap_script( aptr, sptr, rptr, (struct corcs_asynch_request *) 0 ) );

	else if (!( strcmp(  command, "occi" ) ))
		return( corcs_soap_occi( aptr, sptr, rptr, (struct corcs_asynch_request *) 0 ) );

	else if (!( strcmp(  command, "AsynchParseManifest" ) ))
		return( corcs_asynchronous_request( corcs_soap_manifest_parser, command, aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "AsynchConvertManifest" ) ))
		return( corcs_asynchronous_request( corcs_soap_manifest_convertor, command, aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "AsynchParseSLA" ) ))
		return( corcs_asynchronous_request( corcs_soap_sla_parser, command, aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "AsynchConvertSLA" ) ))
		return( corcs_asynchronous_request( corcs_soap_sla_convertor, command, aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "AsynchBrokerSLA" ) ))
		return( corcs_asynchronous_request( corcs_soap_sla_broker, command, aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "AsynchServiceAction" ) ))
		return( corcs_asynchronous_request( corcs_soap_service, command, aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "AsynchRunScript" ) ))
		return( corcs_asynchronous_request( corcs_soap_script, command, aptr, sptr, rptr ) );

	else if (!( strcmp(  command, "AsynchResult" ) ))
		return( corcs_asynchronous_result( command, aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "AsynchParseManifestResult" ) ))
		return( corcs_asynchronous_result( command, aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "AsynchConvertManifestResult" ) ))
		return( corcs_asynchronous_result( command, aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "AsynchParseSLAResult" ) ))
		return( corcs_asynchronous_result( command, aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "AsynchConvertSLAResult" ) ))
		return( corcs_asynchronous_result( command, aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "AsynchBrokerSLAResult" ) ))
		return( corcs_asynchronous_result( command, aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "AsynchServiceActionResult" ) ))
		return( corcs_asynchronous_result( command, aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "AsynchRunScriptResult" ) ))
		return( corcs_asynchronous_result( command, aptr, sptr, rptr ) );

	else
	{
		sptr = liberate_xml_element( sptr );
		return(rest_html_response(aptr, 400, "incorrect rest command" ));
	}
}

#include "soapapi.c"

	/* ------------- */
#endif  /* _corcs_soap_c */
	/* ------------- */

