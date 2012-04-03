/* ------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                   */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>    */
/* --------------------------------------------------------------------*/
/*  This is free software; you can redistribute it and/or modify it    */
/*  under the terms of the GNU Lesser General Public License as        */
/*  published by the Free Software Foundation; either version 2.1 of   */
/*  the License, or (at your option) any later version.                */
/*                                                                     */
/*  This software is distributed in the hope that it will be useful,   */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of     */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   */
/*  Lesser General Public License for more details.                    */
/*                                                                     */
/*  You should have received a copy of the GNU Lesser General Public   */
/*  License along with this software; if not, write to the Free        */
/*  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA */
/*  02110-1301 USA, or see the FSF site: http://www.fsf.org.           */
/* --------------------------------------------------------------------*/

#ifndef	_cpxsd_c
#define	_cpxsd_c

#include "cpxsd.h"

private	struct	xml_element * enforceXsd=(struct xml_element *) 0;

/*	---------------------------------------------------	*/
/*		c o r d s _ d o c u m e n t _ x s d 		*/
/*	---------------------------------------------------	*/
public	void	cords_document_xsd( struct xml_element * xsd )
{
	enforceXsd = xsd;
	return;
}

/*	---------------------------------------------------	*/
/*			x s d _ t y p e 			*/
/*	---------------------------------------------------	*/
/*	localises the named type definition structure of an	*/
/*	element.						*/
/*	---------------------------------------------------	*/
public	struct	xml_element * xsd_type( struct xml_element * xsd, char * nptr )
{
	struct	xml_element * wptr=(struct xml_element *) 0;
	struct	xml_element * tptr=(struct xml_element *) 0;
	struct	xml_atribut * aptr;
	char 		    * vptr;
	if ( check_debug() ) { printf("xsd:type( %s )\n",nptr); }
	for (	wptr = document_element( xsd, _XSD_COMPLEX );
		wptr != (struct xml_element *) 0;
		wptr = wptr->next )
	{
		if ( strcasecmp( wptr->name, _XSD_COMPLEX ) )
			continue;
		else if (!( aptr = document_atribut( wptr, _XSD_NAME ) ))
			continue;
		else if (!( aptr->value ))
			continue;
		else if (!( vptr = occi_unquoted_value( aptr->value ) ))
			continue;
		else if (!( strcmp( vptr, nptr ) ))
		{
			liberate( vptr );
			return( wptr );
		}
		else
		{
			liberate( vptr );
			continue;
		}
	}
	if ( check_debug() ) { printf("xsd:failure( attribute %s )\n",nptr); }
	return( wptr );
}

/*	---------------------------------------------------	*/
/*		f i r s t _ x s d _ e l e m e n t 		*/
/*	---------------------------------------------------	*/
public	struct	xml_element * first_xsd_element( struct xml_element * xsd )
{
	if (!( xsd ))
		return( xsd );
	else if (!( xsd->first ))
		return( xsd->first );
	else if (!( strcmp( xsd->first->name, _XSD_SEQUENCE ) ))
		xsd = xsd->first;
	if (!( xsd ))
		return(xsd);
	else	return( xsd->first );
}


/*	---------------------------------------------------	*/
/*		x s d _ e l e m e n t _ t y p e 		*/
/*	---------------------------------------------------	*/
public	struct	xml_element * xsd_element_type( struct xml_element * wptr )
{
	struct	xml_element * tptr;
	struct	xml_atribut * aptr;
	char		    * vptr;
	if (!( aptr = document_atribut( wptr, _XSD_TYPE ) ))
		return((struct xml_element *) 0);
	else if (!( aptr->value ))
		return((struct xml_element *) 0);
	else if (!( vptr = occi_unquoted_value( aptr->value ) ))
		return((struct xml_element *) 0);
	else if (!( strncmp( vptr, "xsd:", strlen( "xsd:" ) ) ))
	{
		liberate( vptr );
		return( wptr );
	}
	else
	{
		tptr = xsd_type( enforceXsd, vptr );
		liberate( vptr );
		return( tptr );
	}
}

/*	---------------------------------------------------	*/
/*			x s d _ e l e m e n t 			*/
/*	---------------------------------------------------	*/
/*	returns the type description structure of a named	*/
/*	element of the xsd 					*/
/*	---------------------------------------------------	*/
public	struct	xml_element * xsd_element( struct xml_element * xsd, char * nptr )
{
	struct	xml_element * wptr=(struct xml_element *) 0;
	struct	xml_element * tptr=(struct xml_element *) 0;
	struct	xml_atribut * aptr;
	char 		    * vptr;
	if ( check_debug() ) { printf("xsd:element( %s )\n",nptr); }
	if (!( xsd ))
		return( xsd );
	else if (!( xsd->first ))
		return( xsd->first );
	else if (!( strcmp( xsd->first->name, _XSD_SEQUENCE ) ))
		xsd = xsd->first;

	for (	wptr = document_element( xsd, _XSD_ELEMENT );
		wptr != (struct xml_element *) 0;
		wptr = wptr->next )
	{
		if ( strcasecmp( wptr->name, _XSD_ELEMENT ) )
			continue;
		else if (!( aptr = document_atribut( wptr, _XSD_NAME ) ))
			continue;
		else if (!( aptr->value ))
			continue;
		else if (!( vptr = occi_unquoted_value( aptr->value ) ))
			continue;
		else if (!( strcmp( vptr, nptr ) ))
		{
			liberate(vptr);
			return( xsd_element_type( wptr ) );
		}
		else 	continue;

	}
	if ( check_debug() ) { printf("xsd:failure( element %s )\n",nptr); }
	return((struct xml_element *) 0);
}

/*	---------------------------------------------------	*/
/*			x s d _ a t r i b u t 			*/
/*	---------------------------------------------------	*/
public	struct	xml_element *	xsd_atribut( struct xml_element * xsd, char * nptr )
{
	struct	xml_element * wptr=(struct xml_element *) 0;
	struct	xml_element * tptr=(struct xml_element *) 0;
	struct	xml_atribut * aptr;
	char		    * vptr;
	if ( check_debug() ) { printf("xsd:attribute( %s )\n",nptr); }
	for (	wptr = document_element( xsd, _XSD_ATRIBUT );
		wptr != (struct xml_element *) 0;
		wptr = wptr->next )
	{
		if ( strcasecmp( wptr->name, _XSD_ATRIBUT ) )
			continue;
		else if (!( aptr = document_atribut( wptr, _XSD_NAME ) ))
			continue;
		else if (!( aptr->value ))
			continue;
		else if (!( vptr = occi_unquoted_value( aptr->value ) ))
			continue;
		else if (!( strcmp( vptr, nptr ) ))
		{
			liberate( vptr );
			return( wptr );
		}
		else	liberate( vptr );
	}
	if ( check_debug() ) { printf("xsd:failure( attribute %s )\n",nptr); }
	return((struct xml_element *) 0);
}

/*	---------------------------------------------------	*/
/*	      x s d _ v a l i d a t e _ e l e m e n t		*/
/*	---------------------------------------------------	*/
public	struct xml_element * xsd_validate_element( 
	struct xml_element * xsd,
	struct xml_element * eptr )
{
	struct	xml_atribut * aptr;
	struct	xml_element * wptr;
	struct	xml_element * bptr;
	if (!( xsd ))
		return( eptr );
	else if (!( eptr ))
		return( eptr );
	else if (!( wptr = xsd_element( xsd, eptr->name ) ))
		return( wptr );
	else
	{
		for (	aptr=eptr->firstatb;
			aptr != (struct xml_atribut *) 0;
			aptr = aptr->next )
		{
			if (!( bptr = xsd_atribut( wptr, aptr->name ) ))
				return((struct xml_element *) 0);
		}
		return( eptr );
	}
}

/*	---------------------------------------------------	*/
/*		d o c u m e n t _ p a r s e _ u r l		*/
/*	---------------------------------------------------	*/
public	struct xml_element * document_parse_url( char * url )
{
	struct	xml_element * document=(struct xml_element *) 0;
	struct	rest_header * hptr;
	struct	rest_response * rptr;
	if (!( strncmp( url,"file:///",strlen("file:///") ) ))
		return((document = document_parse_file( (url + (strlen("file:///") - 1 ) ) ) ));
	else if (!( rptr = rest_client_get_request( url, (char *) 0,"Xsd Client", (struct rest_header *) 0) ))
		return( document );
	else if ( rptr->status != 200 )
		return( document );
	else if (!( hptr = rest_resolve_header( rptr->first, _HTTP_CONTENT_LENGTH ) ))
		return( document );
	else if (!( hptr->value ))
		return( document );
	else if (!( atoi( hptr->value ) ))
		return( document );
	else if (!( rptr->body ))
		return( document );
	else	return((document = document_parse_file( rptr->body )));
}

/*	---------------------------------------------------	*/
/*		d o c u m e n t _ l o a d _ x s d		*/
/*	---------------------------------------------------	*/
public	struct xml_element * document_load_xsd( struct xml_element * document )
{
	struct	xml_element 	* eptr;
	struct	xml_element 	* wptr;
	struct	xml_element 	* xsd=(struct xml_element *) 0;
	struct	xml_atribut 	* aptr=(struct xml_atribut *) 0;
	char 			* sptr=(char *) 0;
	if (!( aptr = document_atribut( document,"xmlns" ) ))
	{
		if (!( sptr = allocate_string( _CORDS_NS ) ))
		{
			cords_append_error( document, 707,"namespace failure");
			return((struct xml_element *) 0 );
		}
	}
	else if (!( aptr->value ))
	{
		if (!( sptr = allocate_string( _CORDS_NS ) ))
		{
			cords_append_error( document, 708,"namespace failure");
			return((struct xml_element *) 0 );
		}
	}
	else if (!( sptr = occi_unquoted_value( aptr->value ) ))
	{
		cords_append_error( document, 709,"namespace failure");
		return((struct xml_element *) 0 );
	}

	if (!( xsd = document_parse_url( sptr ) ))
	{
		cords_append_error( document, 709,"namespace failure");
		liberate( sptr );
		return((struct xml_element *) 0 );
	}
	else
	{
		liberate( sptr );
		/* -------------------------------------------------- */
		/* process the included and redefined schema elements */
		/* -------------------------------------------------- */
		while (((eptr = document_element( xsd, "include" )) != (struct xml_element *) 0)
		||     ((eptr = document_element( xsd, "redefine" )) != (struct xml_element *) 0)
		||     ((eptr = document_element( xsd, "xsd:include" )) != (struct xml_element *) 0)
		||     ((eptr = document_element( xsd, "xsd:redefine" )) != (struct xml_element *) 0))
		{
			if (!( aptr = document_atribut( eptr, "schemaLocation" )))
			{
				cords_append_error( document, 709,"incorrect include");
				break;
			}
			else if (!( aptr->value ))
			{
				cords_append_error( document, 709,"incorrect include");
				break;
			}
			else if (!( sptr = occi_unquoted_value( aptr->value ) ))
			{
				cords_append_error( document, 709,"incorrect include");
				break;
			}
			else if (!( wptr = document_parse_url( sptr ) ))
			{
				cords_append_error( document, 709,"namespace failure");
				liberate( sptr );
				break;
			}
			else
			{
				document_append_elements( eptr, wptr->first );
				document_remove_element( eptr );
				wptr->first = wptr->last = (struct xml_element *) 0;
				liberate_element( wptr );
				liberate( sptr );
				continue;
			}
		}
		/* ------------------ */
		/* ok its been loaded */
		/* ------------------ */
		return( xsd );
	}
}

	/* -------- */
#endif	/* _cpxsd_c */
	/* -------- */

