#ifndef	_corcs_soap_c
#define	_corcs_soap_c
#include "command.h"
/*	-----------------------------		*/
/*	c o r c s _ s o a p _ w s d l 		*/
/*	-----------------------------		*/
private	char *	corcs_soap_wsdl()
{
	char * filename;
	FILE * h;
	if (!( filename = rest_temporary_filename("wsdl") ))
		return( filename );
	else if (!( h = fopen( filename, "w" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<wsdl version=\"1.1\">\n");

		fprintf(h,"<definitions>\n");
		fprintf(h,"</definition/>\n");

		fprintf(h,"<ports>\n");
		fprintf(h,"</ports>\n");

		fprintf(h,"<bindings>\n");
		fprintf(h,"</bindings>\n");

		fprintf(h,"<endpoints>\n");
		fprintf(h,"</endpoints>\n");

		fprintf(h,"</wsdl>\n");
		fclose(h);
		return( filename );
	}
}

/*	---------------------------------------		*/
/*	l i b e r a t e _ x m l _ e l e m e n t		*/ 
/*	---------------------------------------		*/
private	struct	xml_element * 	liberate_xml_element( struct xml_element * sptr )
{
	if ( sptr )
	{
		liberate( sptr );
	}
	return((struct xml_element *) 0);
}

/*	---------------------------------------------	*/
/*	d o c u m e n t _ e l e m e n t _ s t r i n g 	*/ 
/*	---------------------------------------------	*/
private	char *	document_element_string( struct xml_element * xptr, char * nptr )
{
	struct	xml_element *eptr;
	if (!( eptr = document_element( xptr, nptr )))
		return((char *) 0);
	else	return( eptr->value );
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
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------	*/
/*	  c o r d s _ s o a p _ p a r s e r 	*/
/*	-------------------------------------	*/
private	struct	rest_response * corcs_soap_parser( struct rest_response * aptr,struct xml_element * sptr, struct rest_request * rptr )
{
	char *	filename=(char *) 0;
	char *	command=(char *) 0;
	if (!( sptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( rptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( filename = document_element_string( sptr, "filename") ))
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
		cords_parser_response ( aptr, filename );
		sptr = liberate_xml_element( sptr );
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------	*/
/*	  c o r d s _ s o a p _ b r o k e r 	*/
/*	-------------------------------------	*/
private	struct	rest_response * corcs_soap_broker( struct rest_response * aptr, struct xml_element * sptr, struct rest_request * rptr )
{
	char *	filename=(char *) 0;
	char *	command=(char *) 0;
	if (!( sptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( rptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( filename = document_element_string( sptr, "filename") ))
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
		cords_broker_response ( aptr, filename );
		sptr = liberate_xml_element( sptr );
		return( rest_html_response( aptr, 200, "OK" ) );
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
	else if (!( filename = document_element_string( sptr, "filename") ))
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
	if (!( rptr ))
		return(rest_html_response(aptr, 400, "missing request"));
	else if (!( rptr->body ))
		return(rest_html_response(aptr, 400, "missing request body"));
	else if (!( sptr = corcs_xml_element( rptr->body ) ))
		return(rest_html_response(aptr, 400, "incorrect request body"));
	else if (!( strcmp(  command, "/resolver" ) ))
		return( corcs_soap_resolver( aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "/parser" ) ))
		return( corcs_soap_parser( aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "/broker" ) ))
		return( corcs_soap_broker( aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "/service" ) ))
		return( corcs_soap_service( aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "/script" ) ))
		return( corcs_soap_script( aptr, sptr, rptr ) );
	else if (!( strcmp(  command, "/occi" ) ))
		return( corcs_soap_occi( aptr, sptr, rptr ) );
	else
	{
		sptr = liberate_xml_element( sptr );
		return(rest_html_response(aptr, 400, "incorrect request command"));
	}
}

/*	-------------------------------------	*/
/*	     c o r d s _ s o a p _ g e t 	*/
/*	-------------------------------------	*/
public	struct rest_response * corcs_soap_get( struct rest_response * aptr, struct rest_request * rptr )
{
	char *	filename;
	if (!( filename = corcs_soap_wsdl() ))
		return(rest_html_response(aptr, 500, "wsdl generation failure"));
	else if (!( aptr = allocate_rest_response(200,"OK") ))
		return(rest_html_response(aptr, 500, "response failure"));
	else
	{
		rest_response_body( aptr, filename, _FILE_BODY );
		return( aptr  );
	}
}


#endif  /* _corcs_soap_c */
	/* ------------- */

