#ifndef	_corc_xml_c
#define	_corc_xml_c

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
/*	d o c u m e n t _ e l e m e n t _ x m l   	*/ 
/*	---------------------------------------------	*/
private	char *	document_element_xml( struct xml_element * xptr, char * nptr )
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

/*	---------------------------------------------	*/
/*	d o c u m e n t _ e l e m e n t _ f i l e  	*/ 
/*	---------------------------------------------	*/
private	char *	document_element_file( struct xml_element * xptr, char * nptr )
{
	char *	sptr;
	int	c;
	FILE * h;
	struct	xml_element *eptr;
	char *	filename;
	if (!( eptr = nested_document_element( xptr, nptr )))
		return((char *) 0);
	else if (!( sptr = eptr->value ))
		return( (char *) 0);
	else if (!( filename = rest_temporary_filename( "txt" ) ))
		return( filename );
	else if (!( h = fopen( filename, "w" ) ))
		return( liberate( filename ) );
	else
	{
		while ( *sptr )
		{
			if ((c = *(sptr++)) != '&' )
				fputc(c,h);
			else
			{
				switch ((c = *(sptr++)))
				{
				case	'a'	:
					sptr+= 3;
					fputc('&',h);
					continue;
				case	'l'	:
					sptr+= 2;
					fputc('<',h);
					continue;
				case	'g'	:
					sptr+= 2;
					fputc('>',h);
					continue;
				default		:
					fputc('&',h);
					if ( c )
						fputc(c,h);
					continue;
				}
			}
		}
		fclose(h);
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
	fprintf(h,"<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\">\n");

	fprintf(h,"<soapenv:Body>\n");
	fprintf(h,"<m:%s xmlns:m=\"%s/wsdl\">\n",nptr,get_identity());
	return;
}

/*	---------------------------------------		*/
/*	  s o a p _ f a u l t _ m e s s a g e 		*/
/*	---------------------------------------		*/
private	void	soap_fault_message( FILE * h, int status, char * reason, char * detail )
{
	fprintf(h,"<?xml version=\"1.0\"?>\n");
	fprintf(h,"<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\">\n");
	fprintf(h,"<soapenv:Body>\n");
	fprintf(h,"<soapenv:Fault>\n");
	fprintf(h,"<soapenv:Code>\n");
	fprintf(h,"<soapenv:Value>%u</soapenv:Value>\n",status);
	fprintf(h,"</soapenv:Code>\n");
	fprintf(h,"<soapenv:Reason>\n");
	fprintf(h,"<soapenv:Text>%s</soapenv:Text>\n",reason);
	fprintf(h,"</soapenv:Reason>\n");
	fprintf(h,"<soapenv:Detail>\n");
	fprintf(h,"<soapenv:Text>%s</soapenv:Text>\n",detail);
	fprintf(h,"</soapenv:Detail>\n");
	fprintf(h,"</soapenv:Fault>\n");
	fprintf(h,"</soapenv:Body>\n");
	fprintf(h,"</soapenv:Envelope>\n");
	return;
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
		{
			switch ( c )
			{
			case	'&'	: fprintf(h,"&amp;"); continue;
			case	'<'	: fprintf(h,"&lt;"); continue;
			case	'>'	: fprintf(h,"&gt;"); continue;
			default		:
				fputc(c,h);
			}
		}
		fclose(s);
		return(0);
	}
}

	/* ----------- */
#endif	/* _corc_xml_c */
	/* ----------- */

