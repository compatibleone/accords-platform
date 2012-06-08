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
#ifndef	_cords_parser_c
#define	_cords_parser_c

#include "cp.h"
#include "csp.h"
#include "cpxsd.h"

private	int	cords_append_error( struct xml_element * dptr, int status, char * message);

#include "cpxsd.c"


/*	---------------------------------------------------	*/
/*		c o r d s _ a p p e n d _ c h o i c e		*/
/*	---------------------------------------------------	*/
/*	apends a choice description element to the current	*/
/*	element being processed.				*/
/*	---------------------------------------------------	*/
private	int	cords_append_choice( struct xml_element * dptr, int status, char * message)
{
	char	value[64];
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;
	sprintf(value,"%u",status);
	if (!( eptr = document_prefix_element(dptr,_CORDS_CHOICE)))
		return( status );
	else if (!( aptr = document_add_atribut( eptr, "code",value)))
		return( status );
	else if (!( aptr = document_add_atribut( eptr, "message", message)))
		return( status );
	else	return( 0 );
}

/*	---------------------------------------------------	*/
/*		c o r d s _ a p p e n d _ e r r o r		*/
/*	---------------------------------------------------	*/
/*	apends an error description element to the current	*/
/*	element being processed.				*/
/*	---------------------------------------------------	*/
private	int	cords_append_error( struct xml_element * dptr, int status, char * message)
{
	char	value[64];
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;
	sprintf(value,"%u",status);
	if (!( eptr = document_prefix_element(dptr,_CORDS_ERROR)))
		return( status );
	else if (!( aptr = document_add_atribut( eptr, "code",value)))
		return( status );
	else if (!( aptr = document_add_atribut( eptr, "message", message)))
		return( status );
	else	return( 0 );
}

/*	---------------------------------------------------	*/
/*		c o r d s _ a p p e n d _ w a r n i n g		*/
/*	---------------------------------------------------	*/
/*	apends a warning description element to the current	*/
/*	element being processed.				*/
/*	---------------------------------------------------	*/
private	int	cords_append_warning( struct xml_element * dptr, int status, char * message)
{
	char	value[64];
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;
	sprintf(value,"%u",status);
	if (!( eptr = document_prefix_element(dptr,_CORDS_WARNING)))
		return( status );
	else if (!( aptr = document_add_atribut( eptr, "code",value)))
		return( status );
	else if (!( aptr = document_add_atribut( eptr, "message", message)))
		return( status );
	else	return( 0 );
}

/*	---------------------------------------------------	*/
/*	     c o r d s _ a p p e n d _ c h o i c e s  		*/
/*	---------------------------------------------------	*/
/*	this function will scan the element list of an occi	*/
/*	response and will extract all "location" items that	*/
/*	ere encountered and add choice elements to the tree	*/
/*	---------------------------------------------------	*/
private	int	cords_append_choices( struct occi_response * rptr, struct xml_element * dptr )
{
	struct	rest_response * aptr;
	struct	occi_element  * eptr;
	struct	rest_header   * hptr;
	struct	rest_header   * jptr;
	if (!( aptr = rptr->response ))
	{
		for (	eptr = rptr->first;
			eptr != (struct occi_element *) 0;
			eptr = eptr->next )
			if (!( strcmp( eptr->name, "location" )))
				cords_append_choice( dptr, 737, eptr->value );
		return(737);
	}
	else if (!( hptr = aptr->first))
		return(0);
	else
	{
		while ((jptr = rest_resolve_header( hptr, _OCCI_LOCATION )) != (struct rest_header *) 0)
		{
			cords_append_choice( dptr, 737, jptr->value );
			hptr = jptr->next;
		}
		return( 737 );
	}
}

/*	-------------------------------------------------------------------	*/
/*	c o r d s _ r e t r i e v e _ n a m e d _ i n s t a n c e _ l i s t	*/
/*	-------------------------------------------------------------------	*/
/*	this function retrieves a list of named instances of the category 	*/
/*	indicated by the parameter values assuming the identity of the 		*/
/*	indicated agent.							*/
/*	-----------------------------------------------------------------	*/
public	struct	occi_response * cords_retrieve_named_instance_list(
		char * category,
		char * namename,
		char * namevalue,
		char * agent, char * tls)
{
	struct	occi_response * zptr=(struct occi_response *) 0;
	struct	occi_response * yptr=(struct occi_response *) 0;
	struct	occi_element  * fptr=(struct occi_element  *) 0;
	struct	occi_element  * eptr=(struct occi_element  *) 0;
	struct	occi_client   * kptr=(struct occi_client   *) 0;
	struct	occi_request  * qptr=(struct occi_request  *) 0;
	char	buffer[4096];

	if (!( zptr = occi_resolver( category, agent ) ))
	 	return( zptr );
	else
	{
		for (	eptr = zptr->first;
			eptr != (struct occi_element*) 0;
			eptr = eptr->next )
		{
			if (!( eptr->name ))
				continue;
			else if (!( eptr->value ))
				continue;
			sprintf(buffer,"%s/%s/",eptr->value,category);
			if (!( kptr = occi_create_client( buffer, agent, tls ) ))
				continue;
			else if (!(qptr = occi_create_request( 
					kptr, 
					kptr->target->object, 
					_OCCI_NORMAL )))
				continue;
			else if (!( fptr = occi_request_element( 
					qptr,
					namename, 
					namevalue ) ))
				continue;
			else if (!( yptr = occi_client_get( kptr, qptr ) ))
				continue;
			else 	break;
		}
	}
	zptr = occi_remove_response ( zptr );
	return( yptr);
}

/*	---------------------------------------------------	*/
/*	      c o r d s _ d e l e t e _ i n s t a n c e		*/
/*	---------------------------------------------------	*/
/*	delete an OCCI instance as specified by uri.		*/
/*	---------------------------------------------------	*/
public	int	cords_delete_instance( char * uri, char * category, char * agent, char * tls )
{
	struct	occi_client * cptr;
	struct	occi_request * rptr;
	struct	occi_response * aptr;

	if (!( cptr = occi_create_client( uri, agent, tls ) ))
	{
		return( 27 );
	}

	else if (!(rptr = occi_create_request( cptr, category, _OCCI_NORMAL )))
	{
		cptr = occi_remove_client( cptr );
		return( 27 );
	}
	else if (!( aptr = occi_client_delete( cptr, rptr ) )) 
	{
		rptr = occi_remove_request( rptr );
		cptr = occi_remove_client( cptr );
		return( 27 );
	}
	else
	{
		aptr = occi_remove_response( aptr );
		rptr = occi_remove_request( rptr );
		cptr = occi_remove_client( cptr );
		return( 0 );
	}
}


/*	---------------------------------------------------	*/
/*	      c o r d s _ c r e a t e _ i n s t a n c e		*/
/*	---------------------------------------------------	*/
/*	create an OCCI instance of the specified category.	*/
/*	---------------------------------------------------	*/
public	struct	occi_response 	* cords_create_instance(
		char * category, char * agent, struct occi_element * header, char * tls )
{
	char	buffer[4096];
	struct	occi_element  	* eptr;
	struct	occi_element  	* fptr;
	struct	occi_response 	* aptr;
	struct	occi_response 	* zptr;
	struct	occi_request 	* rptr;
	struct	occi_client	* cptr;

	if ( check_debug() )
		printf("cords_create_instance(%s,%s)\n",category,agent);

	if (!( aptr = occi_resolver( category, agent ) ))
		return((struct occi_response *) 0);

	for (	eptr = aptr->first;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		/* -------------------------------------------- */
		/*   for each valid  subscription proposition   */
		/* -------------------------------------------- */
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;

		/* -------------------------------------------- */
		/* connect to host , prepare and submit request */
		/* -------------------------------------------- */
		sprintf(buffer,"%s/%s/",eptr->value,category);
		if (!( cptr = occi_create_client( buffer, agent, tls ) ))
			continue;
		else if (!(rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
			continue;
		else 
		{
			if (( rptr->first = header ) != (struct occi_element *) 0)
			{
				while (header->next)
					header = header->next;
			}
			rptr->last = header;

			if (!( zptr = occi_client_post( cptr, rptr ) ))
				continue;
			/* -------------------------------------------- */
			/*    until a successful result is obtained	*/
			/* -------------------------------------------- */
			else	break;
		}
	}
	aptr = occi_remove_response( aptr );
	return( zptr );
}

/*	---------------------------------------------------	*/
/*		c o r d s _ b u i l d _ h o s t			*/
/*	---------------------------------------------------	*/
public	char *	cords_build_host( struct occi_response * zptr, char * id )
{
	char	*	result;
	if (!( id = occi_category_id( id ) ))
		return( id );
	else if (!( result = allocate(strlen( id )+strlen( zptr->host )+strlen( zptr->name )+16)))
	{
		liberate(id);
		return( result );
	}
	else
	{
		sprintf( result, "%s%s%s",zptr->host,zptr->name, id );
		liberate( id );
		return( result );
	}
}

/*	-------------------------------------------------------------------	*/
/*	    c o r d s _ r e t r i e v e _ n a m e d _ i n s t a n c e 		*/
/*	-------------------------------------------------------------------	*/
/*	this function retrieves a named instance of the category indicated 	*/
/*	by the parameters and assuming the identity of the indicated agent.	*/
/*	-------------------------------------------------------------------	*/
public	struct	occi_response * cords_retrieve_named_instance( 
		struct occi_response * yptr, char * agent, char * tls )
{
	struct	occi_client   * kptr=(struct occi_client   *) 0;
	struct	occi_request  * qptr=(struct occi_request  *) 0;
	struct	occi_response * zptr=(struct occi_response *) 0;
	struct	occi_element  * fptr=(struct occi_element  *) 0;
	char	*	host;
	for (	fptr = yptr->first;
		fptr != (struct occi_element*) 0;
		fptr = fptr->next )
	{
		if (!( fptr->name ))
			continue;
		else if (!( fptr->value ))
			continue;
		else if (!( host = cords_build_host( yptr, fptr->value) ))
			continue;
		else if (!( kptr = occi_create_client( host, _CORDS_CONTRACT_AGENT, tls ) ))
			continue;
		else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
			continue;
		else if (!( zptr = occi_client_get( kptr, qptr ) ))
		{
			qptr = occi_remove_request( qptr );
			continue;
		}
		else 	break;
	}
	return( zptr );
}

/*	---------------------------------------------------	*/
/*	      c o r d s _ c o u n t _ l o c a t i o n s		*/
/*	---------------------------------------------------	*/
/*	this function will scan the element list of an occi	*/
/*	response and will count either the "location" items	*/
/*	encountered. If no element list is available then	*/
/*	the header list of the eventual rest response will	*/
/*	be counted for X-OCCI-Location fields. 			*/
/*	---------------------------------------------------	*/
private	int	cords_count_locations( struct occi_response * rptr )
{
	int			locations=0;
	struct	rest_response * aptr;
	struct	occi_element  * eptr;
	struct	rest_header   * hptr;
	struct	rest_header   * jptr;
	if (!( aptr = rptr->response ))
	{
		for (	eptr = rptr->first;
			eptr != (struct occi_element *) 0;
			eptr = eptr->next )
			if (!( strcmp( eptr->name, "location" )))
				locations++;
		return(locations);
	}
	else if (!( hptr = aptr->first))
		return(0);
	else
	{
		while ((jptr = rest_resolve_header( hptr, _OCCI_LOCATION )) != (struct rest_header *) 0)
		{
			locations++;
			hptr = jptr->next;
		}
		return( locations );
	}
}

/*	---------------------------------------------------	*/
/*	    c o r d s _ r e s o l v e _ l o c a t i o n		*/
/*	---------------------------------------------------	*/
/*	this function will localise an instance identifier 	*/
/*	within the OCCI or REST response and append it onto	*/
/*	the attribute list of the indicated element of the 	*/
/*	CORDS XML document.					*/
/*	---------------------------------------------------	*/
public	int	cords_resolve_location( struct occi_response * zptr, struct xml_element * document )
{
	char			* vptr;
	struct	xml_atribut	* aptr;
	char	buffer[8192];
	if ( cords_count_locations( zptr ) > 1 )
		return( cords_append_choices( zptr, document ) );
	else if (!( vptr = occi_extract_location( zptr ) ))
		return(713);
	else if (!( vptr = occi_category_id( vptr ) ))
		return(714);
	else
	{
		sprintf(buffer,"%s/%s/%s",zptr->host,document->name,vptr);
		if (!( aptr = document_add_atribut( document, _CORDS_ID, buffer ) ))
			return(715);
		else
		{
			liberate( vptr );
			return(0);
		}
	}
}

/*	-----------------------------------------------------	*/
/*	c o r d s _ r e s o l v e _ l a s t _ l o c a t i o n	*/
/*	-----------------------------------------------------	*/
/*	special case for the plan category instance where new	*/
/*	plans resulting from a parsed manifest will repalce 	*/
/*	a previous version.					*/
/*	-----------------------------------------------------	*/
public	int	cords_resolve_last_location( struct occi_response * zptr, struct xml_element * document )
{
	char			* vptr;
	struct	xml_atribut	* aptr;
	char	buffer[8192];
	if ( cords_count_locations( zptr ) > 1 )
		vptr = allocate_string( zptr->last->value );
	else if (!( vptr = occi_extract_location( zptr ) ))
		return(713);

	if (!( vptr = occi_category_id( vptr ) ))
		return(714);
	else
	{
		sprintf(buffer,"%s/%s/%s",zptr->host,document->name,vptr);
		if (!( aptr = document_add_atribut( document, _CORDS_ID, buffer ) ))
			return(715);
		else
		{
			liberate( vptr );
			return(0);
		}
	}
}

/*	---------------------------------------------------	*/
/*	    c o r d s _ p e r f o r m _ s t i t c h u p 	*/
/*	---------------------------------------------------	*/
/*	this function will localise an instance identifier 	*/
/*	within the OCCI or REST response and stitch it onto	*/
/*	the attribute list of the indicated element of the 	*/
/*	CORDS XML document with the provided atribute name	*/
/*	---------------------------------------------------	*/
private	int	cords_perform_stitchup( 
	struct occi_response * zptr, 
	struct xml_element * document, 
	struct xml_element * source, 
	char * target )
{
	char			* vptr;
	struct	xml_atribut	* aptr;
	char	buffer[8192];
	if ( cords_count_locations( zptr ) > 1 )
		return( cords_append_choices( zptr, document ) );
	else if (!( vptr = occi_extract_location( zptr ) ))
		return(713);
	else if (!( vptr = occi_category_id( vptr ) ))
		return(714);
	else
	{
		sprintf(buffer,"%s/%s/%s",zptr->host,source->name,vptr);
		if (!( aptr = document_add_atribut( document, target, buffer ) ))
			return(715);
		else
		{
			liberate( vptr );
			return(0);
		}
	}
}

/*	---------------------------------------------------	*/
/*	    c o r d s _ r e q u e s t _ e l e m e n t   	*/
/*	---------------------------------------------------	*/
/*	this function appends a well formed OCCI element to	*/
/*	the element list of the OCCI request. The elements	*/
/*	will be transformed to OCCI/REST/HTTP Header Fields	*/
/*	prior to transmission of the request.			*/
/*	---------------------------------------------------	*/
private struct occi_element * cords_request_element( 
		struct	occi_request * rptr,
		char * cname, 
		struct xml_atribut * kptr)
{
	struct	occi_element * eptr;
	char *	vptr;
	char	buffer[8192];
	if ((!( kptr ))
	||  (!( kptr->name )))
		return((struct occi_element *) 0);
	else if (!( strcmp( kptr->name,_CORDS_ID) ))
		sprintf(buffer,"occi.core.%s",kptr->name);
	else if (!( strcmp( kptr->name,"status") ))
		sprintf(buffer,"occi.core.%s",kptr->name);
	else	sprintf(buffer,"occi.%s.%s",cname,kptr->name);

	if (!( vptr = occi_unquoted_value( kptr->value ) ))
		return((struct occi_element *) 0);
	else 
	{
		eptr = occi_request_element( rptr, buffer, vptr );
		liberate( vptr );
		return( eptr );
	}
}

/*	---------------------------------------------------	*/
/*	    c o r d s _ e x t e r n a l _ e l e m e n t   	*/
/*	---------------------------------------------------	*/
/*	this function appends a well formed OCCI element to	*/
/*	the element list of the OCCI request. The element	*/
/*	will describe a external instance to which the 		*/
/*	source instance is linked.				*/
/*	---------------------------------------------------	*/
private	struct occi_element * cords_external_element( 
		struct	occi_request * rptr,
		char * 	nptr,
		char * 	vptr )
{
	struct	occi_element * eptr;
	char	buffer[8192];
	if (!( nptr ))
		return((struct occi_element *) 0);
	else	sprintf(buffer,"occi.external.%s",nptr);
	if (!( vptr ))
		return((struct occi_element *) 0);
	else	return( occi_request_element( rptr, buffer, vptr ));
}

/*	---------------------------------------------------	*/
/*		c o r d s _ c o r e _ e l e m e n t   		*/
/*	---------------------------------------------------	*/
/*	this function appends a well formed OCCI element to	*/
/*	the element list of the OCCI request. The element	*/
/*	will describe a simple named core attribute for use 	*/
/*	by link and mixin expressions.				*/
/*	---------------------------------------------------	*/
private	struct occi_element * cords_core_element( 
		struct	occi_request * rptr,
		char * 	nptr,
		char * 	vptr )
{
	char	buffer[8192];
	if (!( nptr ))
		return((struct occi_element *) 0);
	else	sprintf(buffer,"occi.core.%s",nptr);
	if (!( vptr ))
		return((struct occi_element *) 0);
	else	return( occi_request_element( rptr, buffer, vptr ));
}

/*	---------------------------------------------------	*/
/*	    	c o r d s _ l i n k _ t a r g e t		*/
/*	---------------------------------------------------	*/
/*	this function makes the choice between using a core	*/
/*	link target or an external link target by examining	*/
/*	the link source and target urls for host and port.	*/
/*	---------------------------------------------------	*/
private	struct occi_element * cords_link_target(
		struct	occi_request * rptr,
		char * 	from,
		char * 	to )
{
	struct	occi_element 	* eptr;
	struct	url 		* fptr;
	struct	url 		* tptr;
	if (!( from ))
		return((struct occi_element *) 0);
	else if (!( fptr = analyse_url( from ) ))
		return((struct occi_element *) 0);
	else if (!( fptr = validate_url( fptr )))
		return((struct occi_element *) 0);
	if (!( to ))
		return((struct occi_element *) 0);
	else if (!( tptr = analyse_url( to ) ))
	{
		liberate_url( fptr );
		return((struct occi_element *) 0);
	}
	else if (!( tptr = validate_url( fptr )))
	{
		liberate_url( fptr );
		return((struct occi_element *) 0);
	}
	else
	{
		if ( fptr->port != tptr->port )
			eptr = cords_external_element( rptr, "target", to );
		else if (!( strcmp( fptr->host, tptr->host ) ))
			eptr = cords_core_element( rptr, "target", to );
		else	eptr = cords_external_element( rptr, "target", to );
		liberate_url( fptr );
		liberate_url( tptr );
		return( eptr );
	}
}

/*	---------------------------------------------------	*/
/*	     c o r d s _ c a t e g o r y _ h e a d e r		*/
/*	---------------------------------------------------	*/
/*	this function builds a list of OCCI elements that	*/
/*	will be used to describe the instance in terms of 	*/
/*	its attribute values. These elements may be used to	*/
/*	create a selection filter or simply to define the	*/
/*	instance fields for creation of a new instance.		*/
/*	---------------------------------------------------	*/
private	struct	occi_element * cords_request_header( 
		struct occi_request * rptr, 
		struct xml_element * document )
{
	struct	occi_element * hptr= (struct occi_element *) 0;
	struct	xml_atribut * aptr;
	char	buffer[8192];

	for (	aptr=document->firstatb;
		aptr != (struct xml_atribut *) 0;
		aptr = aptr->next )
		
	{
		if (!( aptr->name ))
			continue;
		else if (!( hptr = cords_request_element( rptr, document->name, aptr ) ))
			break;
	}
	return( rptr->first );
}


/*	---------------------------------------------------	*/
/*	     c o r d s _ s e l e c t _ c a t e g o r y		*/
/*	---------------------------------------------------	*/
/*	this function will resolve the collection of hosts	*/
/*	that have published their identity as a agency of	*/
/*	the service defined by the OCCI category name. Each	*/
/*	of the hosts will be tried in turn until one one of	*/
/*	them returns a succsessful result providing a list	*/
/*	of instance identifiers matching the collection of	*/
/*	criteria specified by the request.    			*/
/*	The instance identifiers will be returned using the 	*/
/*	standard X-OCCI-Location header fields.			*/
/*	---------------------------------------------------	*/
public	struct occi_response * cords_select_category(struct xml_element * document, char * agent, char * tls)
{
	char	buffer[8192];
	struct 	occi_element	* hptr;
	struct	occi_element  	* eptr;
	struct	occi_response 	* aptr;
	struct	occi_response 	* zptr;
	struct	occi_request 	* rptr;
	struct	occi_client	* cptr;

	if ( check_debug() )
		printf("cords_select_category(%s)\n",document->name);

	if (!( document ))
		return((struct occi_response *) 0);
	else if (!( document->firstatb ))
		return((struct occi_response *) 0);
	else if (!( aptr = occi_resolver( document->name, agent ) ))
		return((struct occi_response *) 0);

	for (	eptr = aptr->first;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;
		sprintf(buffer,"%s/%s/",eptr->value,document->name);
		if (!( cptr = occi_create_client( buffer, agent, tls ) ))
			continue;
		else if (!(rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
			continue;
		else if (( document->firstatb ) 
		     && (!(hptr = cords_request_header( rptr, document ))))
			continue;
		else if (!( zptr = occi_client_get( cptr, rptr ) ))
			continue;
		else
		{
			hptr = occi_remove_elements( hptr );
			return(zptr);
		}
	}
	hptr = occi_remove_elements( hptr );
	return((struct occi_response *) 0);
}

/*	---------------------------------------------------	*/
/*	   c o r d s _ r e t r i e v e _ c a t e g o r y	*/
/*	---------------------------------------------------	*/
/*	this function will resolve the collection of hosts	*/
/*	that have published their identity as a agency of	*/
/*	the service defined by the OCCI category name. Each	*/
/*	of the hosts will be tried in turn until one one of	*/
/*	them returns a successful result indicating that an	*/
/*	instance of the indicated category has been found  	*/
/*	with its instance identifier matching the provided  	*/
/*	value. The complete instance will be returned as a 	*/
/*	result of this function call completing successfuly 	*/
/*	---------------------------------------------------	*/
public	struct occi_response * cords_retrieve_category( 
		struct xml_element * document, char * id, char * agent, char * tls )
{
	char	buffer[8192];
	struct	occi_element  	* eptr;
	struct	occi_response 	* aptr;
	struct	occi_response 	* zptr;
	struct	occi_request 	* rptr;
	struct	occi_client	* cptr;

	if ( check_debug() )
		printf("cords_retrieve_category(%s,%s)\n",document->name,id);
	
	if (!( strncmp( id, "http", strlen("http") )))
		return( occi_simple_get( id, agent, tls ) );
	else if (!( strncmp( id, "https", strlen("https")) ))
		return( occi_simple_get( id, agent, tls ) );

	if (!( aptr = occi_resolver( document->name, agent ) ))
		return((struct occi_response *) 0);

	for (	eptr = aptr->first;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;
		sprintf(buffer,"%s/%s/%s",eptr->value,document->name,id);
		if (!( cptr = occi_create_client( buffer, agent, tls ) ))
			continue;
		else if (!(rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
			continue;
		else if (!( zptr = occi_client_get( cptr, rptr ) ))
			continue;
		else	return(zptr);
	}
	return((struct occi_response *) 0);
}

/*	---------------------------------------------------	*/
/*	    c o r d s _ r e s o l v e _ c a t e g o r y 	*/
/*	---------------------------------------------------	*/
/*	this function will resolve the collection of hosts	*/
/*	that have published their identity as a agency of	*/
/*	the service defined by the OCCI category name. Each	*/
/*	of the hosts will be tried in turn until one one of	*/
/*	them returns a successful result indicating that at	*/
/*	least one instance has been located at least one of	*/
/*	its attribute values matching at least one of values	*/
/*	specified in the initial request. The corresponding	*/
/*	instance identifiers will be returned in the result 	*/
/*	identifed by the usual X-OCCI-Location header field	*/
/*	---------------------------------------------------	*/
public	struct occi_response * cords_resolve_category( 
		struct xml_element * document, 
		struct xml_atribut * kptr, char * agent, char * tls )
{
	char	buffer[8192];
	struct	occi_element  	* eptr;
	struct	occi_element  	* fptr;
	struct	occi_response 	* aptr;
	struct	occi_response 	* zptr;
	struct	occi_request 	* rptr;
	struct	occi_client	* cptr;

	if ( check_debug() )
		printf("cords_resolve_category(%s,%s=%s)\n",document->name,kptr->name,kptr->value);

	if (!( aptr = occi_resolver( document->name, agent ) ))
		return((struct occi_response *) 0);

	for (	eptr = aptr->first;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;
		sprintf(buffer,"%s/%s/",eptr->value,document->name);
		if (!( cptr = occi_create_client( buffer, agent, tls ) ))
			continue;
		else if (!(rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
			continue;
		else if (!( fptr = cords_request_element( rptr, document->name, kptr ) ))
			continue;
		else if (!( zptr = occi_client_get( cptr, rptr ) ))
			continue;
		else	return(zptr);
	}
	return((struct occi_response *) 0);
}

/*	---------------------------------------------------	*/
/*	     c o r d s _ c r e a t e _ c a t e g o r y		*/
/*	---------------------------------------------------	*/
/*	this function will resolve the collection of hosts	*/
/*	that have published their identity as a agency of	*/
/*	the service defined by the OCCI category name. Each	*/
/*	of the hosts will be tried in turn until one one of	*/
/*	them returns a successful result indicating that a	*/
/*	new instance of the required category has just been	*/
/*	created by the corresponding service agency.		*/
/*	---------------------------------------------------	*/
public	struct	occi_response 	* cords_create_category( struct xml_element * document, char * agent, char * tls )
{
	char	buffer[8192];
	struct	occi_element  	* eptr;
	struct	occi_element  	* fptr;
	struct	occi_response 	* aptr;
	struct	occi_response 	* zptr;
	struct	occi_request 	* rptr;
	struct	occi_client	* cptr;
	struct	xml_atribut	* account;
	if ( check_debug() )
		printf("cords_create_category(%s,%s)\n",document->name,agent);

	if (!( aptr = occi_resolver( document->name, agent ) ))
		return((struct occi_response *) 0);
	else	account = document_atribut( document, _CORDS_ACCOUNT );

	for (	eptr = aptr->first;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		/* -------------------------------------------- */
		/*   for each valid  subscription proposition   */
		/* -------------------------------------------- */
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;

		/* -------------------------------------------- */
		/* connect to host , prepare and submit request */
		/* -------------------------------------------- */
		sprintf(buffer,"%s/%s/",eptr->value,document->name);
		if (!( cptr = occi_create_client( buffer, agent, tls ) ))
			continue;
		else if (!(rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
			continue;

		else if (( document->firstatb ) 
		     && (!(fptr = cords_request_header( rptr, document ))))
			continue;
		else if (( account ) 
		     && ( account->value )
		     && (!( rptr = occi_set_request_account( rptr, account->value ) )))
			continue;
		else if (!( zptr = occi_client_post( cptr, rptr ) ))
			continue;
		/* -------------------------------------------- */
		/*    until a successful result is obtained	*/
		/* -------------------------------------------- */

		else	return(zptr);
	}
	return((struct occi_response *) 0);
}

/*	---------------------------------------------------	*/
/*	     c o r d s _ c r e a t e _ c a t e g o r y		*/
/*	---------------------------------------------------	*/
/*	this function will resolve the collection of hosts	*/
/*	that have published their identity as a agency of	*/
/*	the service defined by the OCCI category name. Each	*/
/*	of the hosts will be tried in turn until one one of	*/
/*	them returns a successful result indicating that a	*/
/*	new instance of the required category has just been	*/
/*	created by the corresponding service agency.		*/
/*	---------------------------------------------------	*/
public	struct	occi_response 	* cords_update_category( 
		struct xml_element * document, char * id, char * agent, char * tls )
{
	struct	occi_element  	* fptr;
	struct	occi_response 	* zptr;
	struct	occi_request 	* rptr;
	struct	occi_client	* cptr;

	if ( check_debug() )
		printf("cords_update_category(%s/%s,%s)\n",document->name,id,agent);

	if (!( cptr = occi_create_client( id, agent, tls ) ))
		return((struct occi_response *) 0);
	else if (!(rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
		return((struct occi_response *) 0);
	else if (( document->firstatb ) 
	     && (!(fptr = cords_request_header( rptr, document ))))
		return((struct occi_response *) 0);
	else if (!( zptr = occi_client_put( cptr, rptr ) ))
		return((struct occi_response *) 0);
	else	return(zptr);
}

/*	---------------------------------------------------	*/
/*	 c o r d s _ i n s t a n c e _ i d e n t i f i e r	*/
/*	---------------------------------------------------	*/
/*	collects all sub element id values and set as the 	*/
/*	atribut of the same name of the current element.	*/
/*	---------------------------------------------------	*/	
private	int	cords_instance_identifier( struct xml_element * dptr, char * element )
{
	struct	xml_atribut * aptr;
	struct	xml_atribut * bptr;
	struct	xml_element * eptr;

	if (((eptr = document_element( dptr, element )) != (struct xml_element *) 0)
	&&  ((aptr = document_atribut( eptr, _CORDS_ID )) != (struct xml_atribut *) 0))
	{
		if (!(bptr = document_atribut( dptr, element )))
		{
			if (!( bptr = document_add_atribut( dptr, element, aptr->value ) ))
				return(727);
		}
		else
		{
			if ( bptr->value )
				bptr->value = liberate( bptr->value );
			if (!( bptr->value = allocate_string( aptr->value ) ))
				return( 727 );
		}
	}
	return(0);
}

/*	---------------------------------------------------	*/
/*	    c o r d s _ i n s t a n c e _ m e m b e r
/*	---------------------------------------------------	*/
/*	set the value of an instance member as provided.	*/
/*	---------------------------------------------------	*/	
private	int	cords_instance_member( struct xml_element * dptr, char * element, char * value )
{
	struct	xml_atribut * bptr;

	if (!(bptr = document_atribut( dptr, element )))
	{
		if (!( bptr = document_add_atribut( dptr, element, value ) ))
			return(727);
	}
	else
	{
		if ( bptr->value )
			bptr->value = liberate( bptr->value );
		if (!( bptr->value = allocate_string( value ) ))
			return( 727 );
	}
	return(0);
}

/*	---------------------------------------------------	*/
/*	   l l _ c o r d s _ i n v o k e _ a c t i o n 		*/
/*	---------------------------------------------------	*/
public	struct	occi_response * ll_cords_invoke_action( char * resource, char * action, char * agent, char * tls )
{
	struct	occi_request 	* rptr;
	struct	occi_client	* cptr;
	char	actihost[4096];
	sprintf(actihost,"%s?action=%s",resource,action);
	if (!( cptr = occi_create_client( actihost, agent, tls ) ))
		return((struct occi_response *) 0);
	else if (!(rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
		return((struct occi_response *) 0);
	else 	return( occi_client_post( cptr, rptr ) );
}

/*	---------------------------------------------------	*/
/*	 	c o r d s _ i n v o k e _ a c t i o n 		*/
/*	---------------------------------------------------	*/
/*	issues a POST request for the invocation of the   	*/
/*	indicated action on the indicated resource instance	*/
/*	---------------------------------------------------	*/	
public	struct	occi_response * cords_schedule_action( char * resource, char * action, char * agent, char * tls )
{
	struct	occi_element 	* eptr;
	struct	occi_response 	* zptr;
	char	buffer[1024];
	char *	scheduler=(char *) 0;
	if (!( resource ))
		return((struct occi_response *) 0);
	else if (!( scheduler = occi_resolve_category_provider( _CORDS_SCHEDULE, agent, tls ) ))
		return( ll_cords_invoke_action( resource, action, agent, tls ) );
	else 	sprintf(buffer,"%s?action=%s",resource,action);

	if (!( eptr = occi_create_element( "occi.schedule.operation", buffer ) ))
		return( ll_cords_invoke_action( resource, action, agent, tls ) );
	else	sprintf(buffer,"%s/%s/",scheduler,_CORDS_SCHEDULE);

	if (!( zptr = occi_simple_post( buffer, eptr, agent, tls ) ))
	{
		eptr = occi_remove_elements( eptr );
		return( ll_cords_invoke_action( resource, action, agent, tls ) );
	}
	else
	{
		eptr = occi_remove_elements( eptr );
		return(zptr);
	}
}

/*	---------------------------------------------------	*/
/*	 	c o r d s _ i n v o k e _ a c t i o n 		*/
/*	---------------------------------------------------	*/
/*	issues a POST request for the invocation of the   	*/
/*	indicated action on the indicated resource instance	*/
/*	---------------------------------------------------	*/	
private	int	use_scheduler=1;
public	struct	occi_response * cords_invoke_action( char * resource, char * action, char * agent, char * tls )
{
	if (!( use_scheduler ))
		return( ll_cords_invoke_action( resource, action, agent, tls ) );
	else if (!( action ))	
		return((struct occi_response *) 0);
	else if (!( strcasecmp( action, _CORDS_START ) ))
		return( cords_schedule_action( resource, action, agent, tls ) );
	else if (!( strcasecmp( action, _CORDS_BUILD ) ))
		return( cords_schedule_action( resource, action, agent, tls ) );
	else	return( ll_cords_invoke_action( resource, action, agent, tls ) );
}
 
/*	---------------------------------------------------	*/
/*		c o r d s _ r e m o v e _ l i n k s		*/
/*	---------------------------------------------------	*/
private	int	cords_remove_links( struct xml_element * document, char * agent, char * tls )
{
	struct	occi_response * zptr;
	struct	xml_atribut * aptr;
	if (!(aptr = document_atribut( document, _CORDS_ID )))
		return( cords_append_error(document,701,"unresolved element") );
	else if (!( zptr = cords_delete_links( aptr->value, agent,tls ) ))
		return( cords_append_error(document,702,"deleting links") );
	else	zptr = occi_remove_response( zptr );
	return(0);
}



/*	---------------------------------------------------	*/
/*	 	    c o r d s _ a d d _ l i n k s		*/
/*	---------------------------------------------------	*/
private	int	cords_add_links( struct xml_element * document, char * element, char * agent, char * tls )
{
	struct	xml_atribut * aptr;
	struct	xml_atribut * bptr;
	struct	xml_element * eptr;
	struct	occi_response * zptr;
	int			count=0;
	char	buffer[512];
	char	value[64];
	
	if (!(aptr = document_atribut( document, _CORDS_ID )))
		return( cords_append_error(document,701,"unresolved element") );

	for (	eptr = document->first;
		eptr != (struct xml_element *) 0;
		eptr = eptr->next )
	{
		if ( strcmp( eptr->name, element ) != 0)
			continue;
		else if (!(bptr = document_atribut( eptr, _CORDS_ID )))
			return( cords_append_error(document,701,"unresolved element") );

		if (!( count ))
			cords_remove_links( document, agent, tls );

		if (!( zptr = cords_create_link( aptr->value, bptr->value, agent, tls ) ))
			return( cords_append_error(eptr,703,"creating link") );
		else
		{
			zptr = occi_remove_response( zptr );
			count++;
		}
	}

	if (!( count ))
		return(0);
	else
	{
		sprintf(buffer,"%ss",element);
		sprintf(value,"%u",count);
		return( cords_instance_member( document, buffer, value ) );
	}
}

/*	---------------------------------------------------	*/
/*	 	c o r d s _ a p p e n d _ l i n k s		*/
/*	---------------------------------------------------	*/
/*	collects all sub element id values of the provided	*/
/*	type and declares them as links from the current	*/
/*	element.						*/
/*	---------------------------------------------------	*/	
private	int	cords_append_links( struct xml_element * document, char * element, char * agent, char * tls )
{
	cords_remove_links( document, agent, tls );

	return( cords_add_links( document, element, agent, tls ) );
}

/*	---------------------------------------------------	*/
/*		c o r d s _ b u i l d _ p l a n			*/
/*	---------------------------------------------------	*/
/*	this function will be called at the end of parsing	*/
/*	when no errors nor choices remain and the result of	*/
/*	the process can be consolidated by a provisioning 	*/
/*	plan instance. The identity of which will be placed	*/
/*	in the request instance to perform the "stitch up".	*/
/*	---------------------------------------------------	*/
private	int	cords_build_plan( struct xml_element * document, char * agent, char * tls )
{
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;
	struct	xml_atribut * bptr;
	struct	occi_response * zptr;
	char	when[32];
	int	status;

	sprintf(when,"%u",time((long*)0));

	/* ----------------------------------------------- */
	/* build the plan document and append the atributs */
	/* ----------------------------------------------- */
	if (!( aptr = document_atribut( document, _CORDS_ID ) ))
		return( cords_append_error(document,701,"unresolved element") );
	else if (!( eptr = allocate_element()))
		return( cords_append_error( document, 900, "creating plan" ) );
	else if (!( eptr->name = allocate_string( _CORDS_PLAN )))
	{
		eptr = document_drop( eptr );
		return( cords_append_error( document, 900, "creating plan" ) );
	}
	else if (!( bptr = document_add_atribut( eptr, _CORDS_MANIFEST , aptr->value)))
	{
		eptr = document_drop( eptr );
		return( cords_append_error( document, 900, "creating plan" ) );
	}
	else if (!( aptr = document_atribut( document, _CORDS_NAME ) ))
	{
		eptr = document_drop( eptr );
		return( cords_append_error(document,901,"missing name") );
	}
	else if (!( bptr = document_add_atribut( eptr, _CORDS_NAME , aptr->value)))
	{
		eptr = document_drop( eptr );
		return( cords_append_error( document, 900, "creating plan" ) );
	}
	else if (!( bptr = document_add_atribut( eptr, _CORDS_ACCESS, "public" )))
	{
		eptr = document_drop( eptr );
		return( cords_append_error( document, 900, "creating plan" ) );
	}
	else if (!( bptr = document_add_atribut( eptr, _CORDS_TARIFICATION, "none" )))
	{
		eptr = document_drop( eptr );
		return( cords_append_error( document, 900, "creating plan" ) );
	}
	else if (!( bptr = document_add_atribut( eptr, _CORDS_VALIDATION, agent )))
	{
		eptr = document_drop( eptr );
		return( cords_append_error( document, 900, "creating plan" ) );
	}
	else if (!( bptr = document_add_atribut( eptr, _CORDS_CREATED, when )))
	{
		eptr = document_drop( eptr );
		return( cords_append_error( document, 900, "creating plan" ) );
	}
	/* ----------------------------------------------- */
	/* create the plan category instance and stitch up */
	/* ----------------------------------------------- */
	else if (!( zptr = cords_create_category( eptr, agent, tls ) ))
	{
		eptr = document_drop( eptr );
		return( cords_append_error( document, 706, "creating category" ) );
	}
	else
	{
		status = cords_perform_stitchup( zptr, document, eptr, _CORDS_PLAN );
		eptr = document_drop( eptr );
		zptr = occi_remove_response( zptr );
		return( status );
	}
}

/*	--------------------------------------------------------	*/
/*	 c o r d s _ r e s o l v e _ m a n i f e s t _ p l a n		*/
/*	--------------------------------------------------------	*/
/*				CORDS V2				*/
/*	--------------------------------------------------------	*/
/*	when a node is encountered for which the type is defined	*/
/*	as something other than "simple" then the type value is		*/
/*	a manifest or plan identifier which must be resolved		*/
/*	to obtain the fully qualified plan category instance ID		*/
/*	to be used to formally define the complex type of the 		*/
/*	resulting node of the manifest.					*/
/*	--------------------------------------------------------	*/
private	char *	cords_resolve_manifest_plan( char * manifestname, char * agent, char * tls )
{
	struct	xml_element * document;
	struct	xml_atribut * aptr;
	struct	occi_response * zptr;
	char *	result=(char *) 0;

	if (!( document = allocate_element()))
		result = (char *) 0;
	else if (!( document->name = allocate_string( _CORDS_PLAN )))
		result = (char *) 0;
	else if (!( aptr = document_add_atribut( document, _CORDS_NAME, manifestname )))
		result = (char *) 0;
	else if (!( zptr = cords_resolve_category( document, aptr, agent,tls ) ))
		result = (char *) 0;
	else if ( cords_resolve_last_location( zptr, document ) != 0)
		result = (char *) 0 ;
	else if (!( aptr = document_atribut( document, _CORDS_ID ) ))
		result = (char * ) 0;
	else	result = allocate_string( aptr->value );

	document = cords_drop_document( document );

	return( result );
}

/*	---------------------------------------------------	*/
/*	   c o r d s _ t e r m i n a t e _ r e q u e s t	*/
/*	---------------------------------------------------	*/
private	int	cords_terminate_request( struct xml_element * dptr, char * agent,char * tls )
{
	int	status;
	struct	xml_atribut * aptr;

	if ((status = cords_instance_identifier( dptr, _CORDS_CONFIGURATION )) != 0)
		return(cords_append_error(dptr,status,_CORDS_CONFIGURATION));

	else if ((status = cords_instance_identifier( dptr, _CORDS_INTERFACE )) != 0)
		return(cords_append_error(dptr,status,_CORDS_INTERFACE));

	else if ((status = cords_instance_identifier( dptr, _CORDS_ACCOUNT )) != 0)
		return(cords_append_error(dptr,status,_CORDS_ACCOUNT));

	else if ((status = cords_instance_identifier( dptr, _CORDS_SECURITY )) != 0)
		return(cords_append_error(dptr,status,_CORDS_SECURITY));

	else if ((status = cords_append_links(dptr,_CORDS_NODE,agent,tls)) != 0)
		return(cords_append_error(dptr,status,"node linkage failure"));

	else if ((status = cords_build_plan( dptr, agent,tls )) != 0)
		return(cords_append_error(dptr,status,"plan creation failure"));

	else if (!( aptr = document_atribut( dptr, _CORDS_ID ) ))
		return(cords_append_error(dptr,701,"unresolved manifest ID element"));

	else if (!( cords_update_category( dptr, aptr->value, agent,tls ) ))
		return(cords_append_error(dptr,704,"updating manifest category"));

	else	return(0);
}

/*	---------------------------------------------------------	*/
/*	c o r d s _ b u i l d _ a p p l i c a t i o n _ i m a g e	*/
/*	---------------------------------------------------------	*/
private	int	cords_build_application_image( char * iptr, char * pptr, char * agent,char * tls )
{
	char *	host;
	char 	buffer[1024];
	struct	occi_client * cptr;
	struct	occi_request * rptr;
	struct	occi_response * zptr;
	struct	occi_response * yptr;
	struct	occi_element * dptr;
	/* ---------------------------------------------------------------------- */
	/* locate and application category service provider which should be COIPS */
	/* ---------------------------------------------------------------------- */
	if (!( host = occi_resolve_category_provider( _CORDS_APPLICATION, agent, tls ) ))
	{
		liberate( pptr );
		return(0);
	}
	else
	{
		sprintf(buffer,"%s/%s/",host,_CORDS_APPLICATION);

		if (!( cptr = occi_create_client( buffer, agent, tls ) ))
		{
			liberate( pptr );
			return(0);
		}
		else if (!(rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
		{
			liberate( pptr );
			return(0);
		}
		else if ((!(dptr=occi_request_element(rptr,"occi.application.image"   , iptr ) ))
		     ||  (!(dptr=occi_request_element(rptr,"occi.application.provider", pptr ) )))
		{
			liberate( pptr );
			rptr = occi_remove_request( rptr );
			return(0);
		}
		/* -------------------------------------------------- */
		/* attempt to retrieve the application image instance */
		/* -------------------------------------------------- */
		else if (( yptr = occi_client_get( cptr, rptr )) != (struct occi_response *) 0)
		{
			if (( yptr->response )
			&&  ( yptr->response->status == 200 )
			&&  ( yptr->first != (struct occi_element *) 0))
			{
				yptr = occi_remove_response( yptr );
				return(0);
			}
			else yptr = occi_remove_response( yptr );
		}
		
		/* -------------------- */
		/* build a new instance */
		/* -------------------- */
		if (!(zptr = occi_client_post( cptr, rptr )))
		{
			rptr = occi_remove_request( rptr );
			liberate( pptr );
			return( 0 );
		}
		else if (!( host = occi_extract_location( zptr ) ))
		{
			zptr = occi_remove_response( zptr );
			rptr = occi_remove_request( rptr );
			liberate( pptr );
			return( 0 );
		}
		else
		{
			sprintf(buffer,"%s://%s",rest_http_prefix(),host);
			zptr = occi_remove_response( zptr );
			rptr = occi_remove_request( rptr );
			if (!( zptr = cords_invoke_action( buffer, "build", agent, tls ) ))
			{
				liberate( pptr );
				return( 0 );
			}
			else
			{
				zptr = occi_remove_response( zptr );
				liberate( pptr );
				return( 0 );
			}

		}
	}
}

/*	---------------------------------------------------	*/
/*	    c o r d s _ i m a g e _ p r o d u c t i o n		*/
/*	---------------------------------------------------	*/
/*	the parser will attempt to schedule the application	*/
/*	image production for a valid image element with ID	*/
/*	for a valid node with a specific provider type.		*/
/*	otherwise image production will be delayed till the	*/
/*	provider type has been resolved.			*/
/*	---------------------------------------------------	*/
private	int	cords_image_production( struct xml_element * dptr, char * agent,char * tls )
{
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;
	struct	xml_atribut * iptr;
	char *	pptr;
	/* ----------------------------------------------- */	
	/* ensure valid image and parent is a NODE element */
	/* ----------------------------------------------- */
	if (!( dptr ))
		return(0);
	else if (!( eptr = dptr->parent ))
		return(0);
	else if (!( eptr->name ))
		return(0);
	else if ( strcmp( eptr->name, _CORDS_NODE ) )
		return(0);
	else if (!( iptr = document_atribut( dptr, _CORDS_ID ) ))
		return(0);
	else if (!( iptr->value ))
		return(0);

	/* ------------------------------------------------ */
	/* detect and control valid node provider attribute *
	/* ------------------------------------------------ */
	else if (!( aptr = document_atribut( eptr, _CORDS_PROVIDER ) ))
		return(0);
	else if (!( aptr->value ))
		return(0);
	else if (!( pptr = occi_unquoted_value( aptr->value ) ))
		return(0);
	else if (!( strcmp( pptr, _CORDS_ANY ) ))
		return(0);

	/* ------------------------------------------------ */
	/* attempt to locate an aleady produced application */
	/* ------------------------------------------------ */
	else	return(cords_build_application_image( iptr->value, pptr, agent, tls ));
}

/*	---------------------------------------------------	*/
/*	      c o r d s _ t e r m i n a t e _ i m a g e		*/
/*	---------------------------------------------------	*/
private	int	cords_terminate_image( struct xml_element * dptr, char * agent,char * tls )
{
	int	status;
	struct	xml_atribut * aptr;

	if ((status = cords_instance_identifier( dptr, _CORDS_SYSTEM )) != 0)
		return(cords_append_error(dptr,status,_CORDS_SYSTEM ));

	else if ((status = cords_append_links(dptr,_CORDS_PACKAGE,agent,tls)) != 0)
		return(cords_append_error(dptr,status,"linkage failure"));
	else if (!( aptr = document_atribut( dptr, _CORDS_ID ) ))
		return(cords_append_error(dptr,701,"unresolved element"));

	else if (!( cords_update_category( dptr, aptr->value, agent,tls ) ))
		return(cords_append_error(dptr,704,"updating category"));

	else	return( cords_image_production( dptr, agent, tls ) );
}

/*	---------------------------------------------------	*/
/*	   c o r d s _ t e r m i n a t e _ n e t w o r k	*/
/*	---------------------------------------------------	*/
private	int	cords_terminate_network( struct xml_element * dptr, char * agent,char * tls )
{
	int	status;
	struct	xml_atribut * aptr;

	if ((status = cords_append_links(dptr,_CORDS_PORT,agent,tls)) != 0)
		return(cords_append_error(dptr,status,"linkage failure"));
	else if (!( aptr = document_atribut( dptr, _CORDS_ID ) ))
		return(cords_append_error(dptr,701,"unresolved element"));
	else if (!( cords_update_category( dptr, aptr->value, agent,tls ) ))
		return(cords_append_error(dptr,704,"updating category"));
	else	return( 0 );
}

/*	---------------------------------------------------	*/
/*	   c o r d s _ t e r m i n a t e _ p a c k a g e	*/
/*	---------------------------------------------------	*/
private	int	cords_terminate_package( struct xml_element * dptr, char * agent,char * tls )
{
	int	status;
	struct	xml_atribut * aptr;

	if ((status = cords_append_links(dptr,_CORDS_PORT,agent,tls)) != 0)
		return(cords_append_error(dptr,status,"linkage failure"));
	else if (!( aptr = document_atribut( dptr, _CORDS_ID ) ))
		return(cords_append_error(dptr,701,"unresolved element"));
	else if (!( cords_update_category( dptr, aptr->value, agent,tls ) ))
		return(cords_append_error(dptr,704,"updating category"));
	else	return( 0 );
}

/*	-----------------------------------------------------------------	*/
/*	      c o r d s _ t e r m i n a t e _ i n f r a s t r u c t u r e	*/
/*	-----------------------------------------------------------------	*/
private	int	cords_terminate_infrastructure( struct xml_element * dptr, char * agent,char * tls )
{
	int	status;
	struct	xml_atribut * aptr;

	if ((status = cords_instance_identifier( dptr, _CORDS_COMPUTE )) != 0)
		return(cords_append_error(dptr,status,_CORDS_COMPUTE));
	else if ((status = cords_instance_identifier( dptr, _CORDS_STORAGE )) != 0)
		return(cords_append_error(dptr,status,_CORDS_STORAGE));
	else if ((status = cords_instance_identifier( dptr, _CORDS_NETWORK )) != 0)
		return(cords_append_error(dptr,status,_CORDS_NETWORK));
	else if (!( aptr = document_atribut( dptr, _CORDS_ID ) ))
		return(cords_append_error(dptr,701,"unresolved element"));
	else if (!( cords_update_category( dptr, aptr->value, agent,tls ) ))
		return(cords_append_error(dptr,704,"updating category"));
	else	return(0);
}


/*	---------------------------------------------------	*/
/*	      c o r d s _ t e r m i n a t e _ n o d e 		*/
/*	---------------------------------------------------	*/
/* 	this function handles the termination of a manifest 	*/
/*	node parsing operation. Two cases are possible that	*/
/*	depend on the value of the type attribute defined	*/
/*	for the node. The "simple" type will require that 	*/
/*	infrastructure and image references have been found	*/
/*	for the qualification of the node description. 		*/
/*	Complex cases exist where the node type defines the	*/
/*	manifest which is to be used to describe the node.	*/
/*	The provisioning plan of the manifest must be found	*/
/*	and its category instance identifier used to define	*/
/*	the fully qualified complex type of the node.		*/
/*	---------------------------------------------------	*/
private	int	cords_terminate_node( struct xml_element * dptr, char * agent,char * tls )
{
	int	status;
	struct	xml_atribut * aptr;
	char 		    * nodetype="simple";

	/* ----------------------------------------------------------------------- */
	/* attempt to retrieve an eventual node type which may be more than simple */
	/* ----------------------------------------------------------------------- */
	if (( aptr = document_atribut( dptr, _CORDS_TYPE )) != (struct xml_atribut *) 0)
	{
		if (!( nodetype = aptr->value ))
			nodetype = "simple";
		else if (!( nodetype = occi_unquoted_value( nodetype ) ))
			nodetype = "simple";
	}

	if (!( strcmp(nodetype,"simple") ))
	{
		/* ------------------------------------------------------------------- */
		/* simple nodes are defined by their required infrastructure and image */
		/* ------------------------------------------------------------------- */
		if ((status = cords_instance_identifier( dptr, _CORDS_INFRASTRUCTURE )) != 0)
			return(cords_append_error(dptr,status,_CORDS_INFRASTRUCTURE));
		else if ((status = cords_instance_identifier( dptr, _CORDS_IMAGE )) != 0)
			return(cords_append_error(dptr,status, _CORDS_IMAGE ));
	}
	else
	{
		/* --------------------------------------------------------------- */
		/* complex nodes are defined by their named target manifest / plan */
		/* --------------------------------------------------------------- */
		if (!( nodetype = cords_resolve_manifest_plan( nodetype, agent, tls ) ))
			return(cords_append_error(dptr,status, _CORDS_MANIFEST ));
		else
		{
			/* ---------------------- */
			/* substitute name for ID */
			/* ---------------------- */
			aptr->value = liberate( aptr->value );
			aptr->value = nodetype;
		}
	}			

	/* ------------------------------ */
	/* update TYPE atribut if missing */
	/* ------------------------------ */
	if (!( aptr ))
		if (!( aptr = document_add_atribut( dptr, _CORDS_TYPE, nodetype ) ))
			return(cords_append_error(dptr,701,"cannot append type"));


	/* -------------------------------------------------- */
	/* standard category instance identifier verification */
	/* -------------------------------------------------- */
	if (!( aptr = document_atribut( dptr, _CORDS_ID ) ))
		return(cords_append_error(dptr,701,"unresolved element"));
	else if (!( cords_update_category( dptr, aptr->value, agent,tls ) ))
		return(cords_append_error(dptr,704,"updating category"));
	else	return( 0 );
}

/*	-----------------------------------------------------	*/
/*	c o r d s_t e r m i n a t e_c o n f i g u r a t i o n 	*/
 /*	-----------------------------------------------------	*/
private	int	cords_terminate_configuration( struct xml_element * dptr, char * agent,char * tls )
{
	int	status;
	struct	xml_atribut * aptr;
	if ((status = cords_append_links(dptr,_CORDS_ACTION,agent,tls)) != 0)
		return(cords_append_error(dptr,status,"linkage failure"));
	else if (!( aptr = document_atribut( dptr, _CORDS_ID ) ))
		return(cords_append_error(dptr,701,"unresolved element"));
	else if (!( cords_update_category( dptr, aptr->value, agent,tls ) ))
		return(cords_append_error(dptr,704,"updating category"));
	else	return(0);
}

/*	-----------------------------------------------------	*/
/*	  c o r d s _ t e r m i n a t e _ i n t e r f a c e 	*/
 /*	-----------------------------------------------------	*/
private	int	cords_terminate_interface( struct xml_element * dptr, char * agent,char * tls )
{
	int	status;
	struct	xml_atribut * aptr;
	if ((status = cords_append_links(dptr,_CORDS_ACTION,agent,tls)) != 0)
		return(cords_append_error(dptr,status,"interface action linkage failure"));
	else if (!( aptr = document_atribut( dptr, _CORDS_ID ) ))
		return(cords_append_error(dptr,701,"unresolved interface ID element"));
	else if (!( cords_update_category( dptr, aptr->value, agent,tls ) ))
		return(cords_append_error(dptr,704,"updating interface category"));
	else	return(0);
}

/*	---------------------------------------------------	*/
/*	   c o r d s _ t e r m i n a t e _ a c c o u n t	*/
/*	---------------------------------------------------	*/
private	int	cords_terminate_account( struct xml_element * dptr, char * agent,char * tls )
{
	int	status;
	struct	xml_atribut * aptr;
	if (( status = cords_append_links(dptr,_CORDS_USER,agent,tls)) != 0)
		return(cords_append_error(dptr,status,"linkage failure"));
	else if (!( aptr = document_atribut( dptr, _CORDS_ID ) ))
		return(cords_append_error(dptr,701,"unresolved element"));
	else if (!( cords_update_category( dptr, aptr->value, agent,tls ) ))
		return(cords_append_error(dptr,704,"updating category"));
	else	return(0);
}

/*	---------------------------------------------------	*/
/*	  c o r d s _ t e r m i n a t e _ s e c u r i t y 	*/
/*	---------------------------------------------------	*/
private	int	cords_terminate_security( struct xml_element * dptr, char * agent,char * tls )
{
	int	status;
	struct	xml_atribut * aptr;
	if (!( aptr = document_atribut( dptr, _CORDS_ID ) ))
		return(cords_append_error(dptr,701,"unresolved element"));
	else	return( 0 );
}

/*	---------------------------------------------------	*/
/*	  c o r d s _ t e r m i n a t e _ i n s t a n c e	*/
/*	---------------------------------------------------	*/
private	int	cords_terminate_instance( struct xml_element * dptr, char * agent,char * tls )
{
	int	status;
	struct	xml_atribut * aptr;
	if ((status = cords_append_links(dptr,_CORDS_CONTRACT,agent,tls)) != 0)
		return(cords_append_error(dptr,status,"linkage failure"));
	else if (!( aptr = document_atribut( dptr, _CORDS_ID ) ))
		return(cords_append_error(dptr,701,"unresolved element"));
	else if (!( cords_update_category( dptr, aptr->value, agent,tls ) ))
		return(cords_append_error(dptr,704,"updating category"));
	else	return(0);
}

/*	---------------------------------------------------	*/
/*	  c o r d s _ t e r m i n a t e _ c o n t r a c t 	*/
/*	---------------------------------------------------	*/
private	int	cords_terminate_contract( struct xml_element * dptr, char * agent,char * tls )
{
	int	status;
	struct	xml_atribut * aptr;
	char 		    * nodetype="simple";

	/* ----------------------------------------------------------------------- */
	/* attempt to retrieve an eventual node type which may be more than simple */
	/* ----------------------------------------------------------------------- */
	if (( aptr = document_atribut( dptr, _CORDS_TYPE )) != (struct xml_atribut *) 0)
		if (!( nodetype = aptr->value ))
			nodetype = "simple";

	if (!( strcmp(nodetype,"simple") ))
	{
		if ((status = cords_instance_identifier( dptr, _CORDS_PROVIDER )) != 0)
			return(cords_append_error(dptr,status,_CORDS_PROVIDER ));
		else if ((status = cords_instance_identifier( dptr, _CORDS_PROFILE )) != 0)
			return(cords_append_error(dptr,status,_CORDS_PROFILE));
	}

	/* ------------------------------ */
	/* update TYPE atribut if missing */
	/* ------------------------------ */
	if (!( aptr ))
		if (!( aptr = document_add_atribut( dptr, _CORDS_TYPE, nodetype ) ))
			return(cords_append_error(dptr,701,"cannot append type"));


	if (!( aptr = document_atribut( dptr, _CORDS_ID ) ))
		return(cords_append_error(dptr,701,"unresolved element"));
	else if (!( cords_update_category( dptr, aptr->value, agent,tls ) ))
		return(cords_append_error(dptr,704,"updating category"));
	else	return(0);
}

/*	---------------------------------------------------	*/
/*	      c o r d s _ t e r m i n a t e _ p l a n 		*/
/*	---------------------------------------------------	*/
private	int	cords_terminate_plan( struct xml_element * dptr, char * agent,char * tls )
{
	int	status;
	struct	xml_atribut * aptr;
	if (!( aptr = document_atribut( dptr, _CORDS_ID ) ))
		return(cords_append_error(dptr,701,"unresolved element"));
	else	return( 0 );
}

/*	---------------------------------------------------	*/
/*	 c o r d s _ t e r m i n a t e _ e x t e n s i o n	*/
/*	---------------------------------------------------	*/
/*	this function will provide the default behaviour of	*/
/*	legal CORDS model extensions. 				*/
/*	This currently requires the verification of the ID	*/
/*	element which indicates that a service provider has	*/
/*	been resolved for the processing of the element.	*/
/*	---------------------------------------------------	*/
private	int	cords_terminate_extension( struct xml_element * dptr, char * agent,char * tls )
{
	int	status;
	struct	xml_atribut * aptr;
	if (!( aptr = document_atribut( dptr, _CORDS_ID ) ))
		return(cords_append_error(dptr,701,"unresolved element"));
	else	return( 0 );
}

/*	---------------------------------------------------	*/
/*	     c o r d s _ t e r m i n a t e _ e l e m e n t	*/
/*	---------------------------------------------------	*/
/*	this function filters out the simple elements of 	*/
/*	the cords model for no further processing and then	*/
/*	applies a default and general algorithm approach to 	*/
/*	the handling of eventual un foreseen extensions.	*/
/*	---------------------------------------------------	*/
public	int	cords_terminate_element( struct xml_element * dptr, char * agent,char * tls )
{

	/* ------------------------------ */
	/* manifest element leaf elements */
	/* ------------------------------ */
	if (!( strcmp( dptr->name, _CORDS_DESCRIPTION ) ))
		return( 0 );

	/* ------------------------------------ */
	/* infrastructure element leaf elements */
	/* ------------------------------------ */
	if (!( strcmp( dptr->name, _CORDS_COMPUTE ) ))
		return( 0 );
	else if (!( strcmp( dptr->name, _CORDS_NETWORK ) ))
		return( 0 );
	else if (!( strcmp( dptr->name, _CORDS_STORAGE ) ))
		return( 0 );

	/* --------------------------- */
	/* image element leaf elements */
	/* --------------------------- */
	else if (!( strcmp( dptr->name, _CORDS_SYSTEM ) ))
		return( 0 );
	else if (!( strcmp( dptr->name, _CORDS_PACKAGE ) ))
		return( 0 );

	/* ----------------------------------- */
	/* configuration element leaf elements */
	/* ----------------------------------- */
	else if (!( strcmp( dptr->name, _CORDS_ACTION ) ))
		return( 0 );

	/* ----------------------------- */
	/* account element leaf elements */
	/* ----------------------------- */
	else if (!( strcmp( dptr->name, _CORDS_USER ) ))
		return( 0 );

	/* ------------------------------------ */
	/* legal extension elements termination */
	/* ------------------------------------ */
	else 	return( cords_terminate_extension( dptr, agent, tls ) );
}

/*	---------------------------------------------------	*/
/*	     c o r d s _ t e r m i n a t e _ x m l 		*/
/*	---------------------------------------------------	*/
private	int	cords_terminate_xml( struct xml_element * dptr, char * agent,char * tls )
{
	if (!( strcmp( dptr->name, _CORDS_MANIFEST ) ))
		return( cords_terminate_request( dptr, agent,tls ) );
	else if (!( strcmp( dptr->name, _CORDS_SERVICE ) ))
		return( cords_terminate_instance( dptr, agent,tls ) );
	else if (!( strcmp( dptr->name, _CORDS_CONTRACT ) ))
		return( cords_terminate_contract( dptr, agent,tls ) );
	else if (!( strcmp( dptr->name, _CORDS_NODE ) ))
		return( cords_terminate_node( dptr, agent,tls ) );
	else if (!( strcmp( dptr->name, _CORDS_NETWORK ) ))
		return( cords_terminate_network( dptr, agent,tls ) );
	else if (!( strcmp( dptr->name, _CORDS_PACKAGE ) ))
		return( cords_terminate_package( dptr, agent,tls ) );
	else if (!( strcmp( dptr->name, _CORDS_INFRASTRUCTURE ) ))
		return( cords_terminate_infrastructure( dptr, agent,tls ) );
	else if (!( strcmp( dptr->name, _CORDS_IMAGE ) ))
		return( cords_terminate_image( dptr, agent,tls ) );
	else if (!( strcmp( dptr->name, _CORDS_CONFIGURATION ) ))
		return( cords_terminate_configuration( dptr, agent,tls ) );
	else if (!( strcmp( dptr->name, _CORDS_INTERFACE ) ))
		return( cords_terminate_interface( dptr, agent,tls ) );
	else if (!( strcmp( dptr->name, _CORDS_ACCOUNT ) ))
		return( cords_terminate_account( dptr, agent,tls ) );
	else if (!( strcmp( dptr->name, _CORDS_SECURITY ) ))
		return( cords_terminate_security( dptr, agent,tls ) );
	else	return( cords_terminate_element( dptr, agent, tls) );
}

/*	---------------------------------------------------	*/
/*	     c o r d s _ t e r m i n a t e _ l e v e l		*/
/*	---------------------------------------------------	*/
/*	completion of certain levels of a cords document is	*/
/*	in need of post processing to position atribut info	*/
/*	this algorithm has been extended to allow control 	*/
/*	to be handled by an XSD for attribute validation 	*/
/*	and link creation.					*/
/*	---------------------------------------------------	*/
public	int	cords_terminate_level( struct xml_element * dptr, char * agent,char * tls )
{
	if ( check_debug() )
	{
		if ( dptr )
			if ( dptr->name )
				printf("terminate level( %s )\n",dptr->name);
	}
	return( cords_terminate_xml( dptr, agent, tls ) );
}

/*	-----------------------------------------------------------------	*/
/*		c o r d s _ p a r s e r _ x s d _ d e f a u l t			*/
/*	-----------------------------------------------------------------	*/
private	int	cords_parser_xsd_default( 
	struct xml_element * wptr,
	struct xml_element * dptr )
{
	int	modifications=0;
	char *	vptr;
	char *	nptr;
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;
	struct	xml_atribut * xptr;
	for ( 	eptr=wptr->first;
		eptr != (struct xml_element *) 0;
		eptr = eptr->next )
	{
		/* ---------------------- */
		/* locate an xsd:attribue */
		/* ---------------------- */
		if (!( eptr->name ))
			continue;
		else if ( strcmp( eptr->name, _XSD_ATRIBUT ) )
			continue;

		/* ------------------------------------------- */
		/* locate a specific "default" attribute value */
		/* ------------------------------------------- */
		for (	nptr=(char *) 0,
			vptr=(char *) 0,
			aptr=eptr->firstatb;
			aptr !=(struct xml_atribut *) 0;
			aptr = aptr->next )
		{
			if (!( aptr->name ))
				continue;
			else if (!( strcmp( aptr->name, _XSD_DEFAULT ) ))
			{
				if (!( aptr->value ))
					continue;
				else if (!( vptr = occi_unquoted_value( aptr->value ) ))
					continue;
				else if (!( nptr ))
					continue;
			}
			else if (!( strcmp( aptr->name, _XSD_NAME ) ))
			{
				if (!( aptr->value ))
					continue;
				else if (!( nptr = occi_unquoted_value( aptr->value ) ))
					continue;
				else if (!( vptr ))
					continue;
			}
			else	continue;

			/* ------------------------------------- */
			/* detect the presence of this attribute */
			/* ------------------------------------- */
			for (	xptr=dptr->firstatb;
				xptr != (struct xml_atribut *) 0;
				xptr = xptr->next )
			{
				if (!( xptr->name ))
					continue;
				else if (!( strcmp( xptr->name, nptr ) ))
					break;
			}
			/* ---------------------------------------------- */
			/* add the attribute since it has a default value */
			/* and does not exist				  */
			/* ---------------------------------------------- */

			if (!( xptr ))
			{
				document_add_atribut(dptr, nptr, vptr );
				modifications++;
			}
		}
	}
	return( modifications );
}

/*	-------------------------------------------------	*/
/*	 c o r d s _ c a s c a d i n g _ a t t r i b u t	*/
/*	-------------------------------------------------	*/
/*	this function allows an attribute to be resolved 	*/
/*	back up the document tree in the parent attribute	*/
/*	lists of an xml document element.			*/
/*	This is useful for gaining access to an XML NS or	*/
/*	an element name for example				*/
/*	-------------------------------------------------	*/
private	struct	xml_atribut * cords_cascading_attribute( 
	struct xml_element * dptr,
	char * nptr )
{
	struct	xml_atribut * bptr;
	if (!( bptr = document_atribut( dptr, nptr ) ))
		return( cords_cascading_attribute( dptr->parent, nptr ) );
	else	return( bptr );
}

/*	-----------------------------------------------------------------	*/
/*		c o r d s _ i n v o c a t i o n _ p a r a m e t e r s		*/
/*	-----------------------------------------------------------------	*/
private	struct occi_element * cords_invocation_parameters(
	struct	xml_atribut * aptr,
	struct xml_element * dptr,
	struct	cordscript_action * fptr )
{
	char *	nptr;
	char *	vptr;
	struct	xml_atribut * bptr;
	struct	cordscript_element * eptr;
	struct	occi_element * root=(struct occi_element *) 0;
	struct	occi_element * last=(struct occi_element *) 0;
	struct	occi_element * hptr=(struct occi_element *) 0;
	char	buffer[1024];
	for (	eptr=fptr->rvalue;
		eptr !=(struct cordscript_element *) 0;
		eptr = eptr->next )
	{
		if (!( nptr = eptr->prefix ))
			continue;
		else if (!( strcmp( nptr, "id" ) ))
			vptr = dptr->name;
		else	vptr = nptr;

		sprintf(buffer,"occi.%s.%s",fptr->lvalue->prefix,vptr);

		if (!( bptr = cords_cascading_attribute( dptr, nptr ) ))
			continue;
		else if (!( bptr->value ))
			continue;
		else if (!( vptr = occi_unquoted_value( bptr->value )))
			continue;
		else if (!( hptr = occi_create_element( buffer, vptr ) ))
			continue;
		else if (!( hptr->previous = last ))
			root = hptr;
		else	last->next = hptr;
		if ( vptr ) vptr = liberate( vptr );
		last = hptr;
	}
	return( root );
}

/*	-----------------------------------------------------------------	*/
/*	c o r d s _ r e s o l v e _ c o r d s c r i p t _ i n s t a n c e	*/
/*	-----------------------------------------------------------------	*/
private	int	cords_resolve_cordscript_instance(
	struct	xml_atribut * aptr,
	struct xml_element * wptr,
	struct xml_element * dptr,
	struct	cordscript_action * fptr,
	char *	agent,
	char *	tls )
{
	struct	cordscript_element * eptr;
	struct	occi_response * yptr;
	struct	occi_response * uptr;
	char	buffer[1024];
	char *	mptr;
	char *	nptr;
	char *	vptr;
	struct	xml_atribut * bptr;
	struct	occi_element * hptr=(struct occi_element *) 0;

	/* ----------------------------------------- */
	/* recover the category.attribute expression */
	/* ----------------------------------------- */
	if (!( eptr=fptr->rvalue ))
		return(0);
	else if (!( mptr = eptr->prefix ))
		return(0);
	else	sprintf(buffer,"%s.%s.%s","occi",mptr, eptr->value );

	/* -------------------------- */
	/* recover the attribute name */
	/* -------------------------- */
	if (!( eptr = eptr->next ))
		return(0);
	else if (!( nptr = eptr->prefix ))
		return(0);

	/* ------------------------------------------ */
	/* then resolve the cascading attribute value*/
	/* ------------------------------------------ */
	else if (!( bptr = cords_cascading_attribute( dptr, nptr ) ))
		return(0);
	else if (!( bptr->value ))
		return(0);
	else if (!( vptr = occi_unquoted_value( bptr->value )))
		return(0);

	if (!( yptr = cords_retrieve_named_instance_list( mptr, buffer, vptr, agent,tls ) ))
		return( 0 );
	else if (!( uptr = cords_retrieve_named_instance( yptr, agent,tls )))
	{
		yptr = occi_remove_response( yptr );
		return( 0 );
	}
	else	
	{
		sprintf(buffer,"%s%s",uptr->host,uptr->name);
		yptr = occi_remove_response( yptr );
		uptr = occi_remove_response( uptr );
		if ( bptr->value ) bptr->value = liberate( bptr->value );
		bptr->value = allocate_string( buffer );
		return(0);
	}
}

/*	-----------------------------------------------------------------	*/
/*		c o r d s _ n e w _ c o r d s c r i p t _ i n s t a n c e	*/
/*	-----------------------------------------------------------------	*/
private	int	cords_new_cordscript_instance(
	struct	xml_atribut * aptr,
	struct xml_element * wptr,
	struct xml_element * dptr,
	struct	cordscript_action * fptr,
	char *	agent,
	char *	tls )
{
	struct	occi_response * zptr;
	struct	occi_element * hptr=(struct occi_element *) 0;

	char *	vptr;
	char	buffer[1024];
	struct	cordscript_element *eptr;

	hptr = cords_invocation_parameters( aptr, dptr, fptr );	

	if (!( zptr = cords_create_instance(
		fptr->lvalue->prefix, agent, hptr, tls ) ))
		return(0);
	else if (!( vptr = occi_extract_location( zptr ) ))
	{
		zptr = occi_remove_response( zptr );
		return(0);
	}
	else
	{
		sprintf(buffer,"%s://%s",rest_http_prefix(),vptr);
		if ( aptr->value ) aptr->value = liberate( aptr->value );
		aptr->value = allocate_string( buffer );
		zptr = occi_remove_response( zptr );
		if ( aptr->value )
			return(0);
		else	return(27);
	}
}

/*	-----------------------------------------------------------------	*/
/*	   c o r d s _ i n v o k e _ c o r d s c r i p t _ a c t i o n		*/
/*	-----------------------------------------------------------------	*/
private	int	cords_invoke_cordscript_action(
	struct	xml_atribut * aptr,
	struct xml_element * wptr,
	struct xml_element * dptr,
	struct	cordscript_action * fptr,
	char *	agent,
	char *	tls )
{
	struct	occi_response * zptr;
	struct	xml_atribut * bptr;
	if (!( bptr = document_atribut( dptr, fptr->lvalue->prefix )))
		return(0);
	else if (!( bptr->value ))
		return(0);
	else
	{
		if (!( zptr = cords_invoke_action( bptr->value, fptr->lvalue->value, agent, tls )))
			return(0);
		else
		{
			zptr = occi_remove_response( zptr );
			return( 0 );
		}
	}
}

/*	-----------------------------------------------------------------	*/
/*		c o r d s _ p a r s e r _ a t r i b u t _ a c t i o n 		*/
/*	-----------------------------------------------------------------	*/
private	int	cords_parser_atribut_action(
	struct	xml_atribut * aptr,
	struct xml_element * wptr,
	struct xml_element * dptr,
	char *	agent,
	char *	tls )
{
	struct	cordscript_action * eptr;
	struct	cordscript_action * fptr;
	struct	occi_response * zptr;
	char *	xptr;
	int	status=0;

	if (!( aptr->name ))
		return(0);
	else if (!(xptr = aptr->value))
		return(0);
	else if (!( strncmp( xptr, _CORDSCRIPT_PREFIX, strlen( _CORDSCRIPT_PREFIX ) ) ))
	{
		/* ----------------------------- */
		/* step over leading white space */
		/* ----------------------------- */
		xptr += strlen(_CORDSCRIPT_PREFIX);
		while ( ( *xptr == ' ' ) 
		|| 	( *xptr == '\t') 
		|| 	( *xptr == '\r')
		||	( *xptr == '\n') )
		{
			 xptr++;
		}
	}

	/* ---------------------------------------------------- */
	/* detect a "cordscript" named attribute to be consumed */
	/* ---------------------------------------------------- */
	else if (!( strcmp( aptr->name, "cordscript" ) ))
		status=1;
	else	return( 0 );

	/* ---------------------------- */
	/* invoke cordscript expression */
	/* ---------------------------- */
	if (( xptr ) && ( check_debug() ))
	{
		printf("invoke cordscript:(%s)\n",xptr);
	}
	if (!( eptr = cordscript_parse_statement( xptr ) ))
		return(0);
	else
	{
		if ( check_debug() )
		{
			cordscript_show( eptr );
		}
		for (	fptr = eptr;
			fptr != (struct cordscript_action *) 0;
			fptr = fptr->next )
		{
			switch ( fptr->type )
			{
			case	_CORDSCRIPT_NEW	:
				cords_new_cordscript_instance( aptr, wptr, dptr, fptr, agent, tls );
				continue;
			case	_CORDSCRIPT_RESOLVE	:
				cords_resolve_cordscript_instance( aptr, wptr, dptr, fptr, agent, tls );
				continue;
			case	_CORDSCRIPT_START	:
			case	_CORDSCRIPT_STOP	:
			case	_CORDSCRIPT_SAVE	:
			case	_CORDSCRIPT_SNAPSHOT	:
			case	_CORDSCRIPT_DELETE	:
			case	_CORDSCRIPT_BUILD	:
				cords_invoke_cordscript_action( aptr, wptr, dptr, fptr, agent, tls );
				continue;
			}
		}
		eptr =  liberate_cordscript_actions( eptr );
		return(status);
	}
}

/*	-----------------------------------------------------------------	*/
/*		c o r d s _ p a r s e r _ x s d _ a c t i o n s			*/
/*	-----------------------------------------------------------------	*/
private	int	cords_parser_xsd_actions( 
	struct xml_element * wptr,
	struct xml_element * dptr,
	char *	agent,
	char *	tls )
{
	int	modifications=0;
	int	status=0;
	struct	xml_atribut * aptr;
	struct	xml_atribut * bptr;
	/* ------------------------------------- */
	/* detect the presence of this attribute */
	/* ------------------------------------- */
	aptr=dptr->firstatb;
	while (aptr != (struct xml_atribut *) 0)
	{
		switch ((status = cords_parser_atribut_action( aptr, wptr, dptr, agent, tls )))
		{
		case	1	:
			/* ---------------------- */
			/* consume this attribute */
			/* ---------------------- */
			bptr = aptr->next;
			if (!( aptr->previous ))
				dptr->firstatb = aptr->next;
			else	aptr->previous->next = aptr->next;
			if (!( aptr->next ))
				dptr->lastatb = aptr->previous;
			else	aptr->next->previous = aptr->previous;
			aptr = liberate_atribut( aptr );
			aptr = bptr;
			modifications++;
			continue;
		case	0	:
			modifications++;
		default		:
			aptr = aptr->next;
		}
	}
	return(modifications);
}

/*	---------------------------------------------------	*/
/*		c o r d s _ t e r m i n a t e _ x s d		*/
/*	---------------------------------------------------	*/
/*	validation of the conformity of the element and its	*/
/*	attributes with respect to the indicated XSD.		*/
/*	TODO: This will then allow the standard terminate	*/
/*	level operation which is hard coded. It must be 	*/
/*	performed under control of the XSD to allow new XSD	*/
/*	types of documents to be correctly processed by the	*/
/*	standard ACCORDS parser engine.				*/
/*	---------------------------------------------------	*/
private	int	xsd_builder=1;
private	int	cords_terminate_xsd( 
	struct xml_element * wptr,
	struct xml_element * dptr, char * agent,char * tls )
{
	int	status;
	char *	vptr;
	char *	nptr;
	char	buffer[1024];
	struct	xml_element * bptr;
	struct	xml_element * eptr;
	struct	xml_element * fptr;
	struct	xml_atribut * aptr;
	int	modifications=0;
	int	linked=0;

	/* ---------------------------------- */
	/* for each attributes of the element */
	/* ---------------------------------- */
	for (	aptr=dptr->firstatb;
		aptr != (struct xml_atribut *) 0;
		aptr = aptr->next )
	{
		/* -------------------------------------------- */
		/* ensure the attribute is specified in the XSD */
		/* -------------------------------------------- */
		if (!( bptr = xsd_atribut( wptr, aptr->name ) ))
		{
			sprintf(buffer,"xsd:incorrect attribute:%s",aptr->name);
			return(cords_append_error(dptr,799,buffer));
		}
	}

	/* ---------------------------------------------------------- */
	/* if not local xsd controlled build then do hard wired build */
	/* ---------------------------------------------------------- */
	if (!( xsd_builder ))
		return( cords_terminate_level( dptr, agent, tls ) );		
	else
	{
		/* ------------------------------------------------- */
		/* link up the single elements to their attribute ID */
		/* ------------------------------------------------- */
		for (	eptr=dptr->first;
			eptr !=(struct xml_element *) 0;
			eptr = eptr->next )
		{

			/* ---------------------------------------------------- */
			/* locate the atribut of the same name as the attribute */
			/* ---------------------------------------------------- */
			if (!( fptr = xsd_atribut( wptr, eptr->name ) ))
				continue;

			/* ----------------------------------------------------- */
			/* locate id of sub element and set same named attribute */
			/* ----------------------------------------------------- */
			else if (!(status = cords_instance_identifier( dptr, eptr->name ) ))
			{
				modifications++;
				continue;
			}
			else	return( status );
		}

		/* ------------------------------------- */
		/* locate the first max occurs unbounded */
		/* ------------------------------------- */

		for ( 	eptr=first_xsd_element( wptr );
			eptr != (struct xml_element *) 0;
			eptr = eptr->next )
		{
			for (	aptr=eptr->firstatb;
				aptr !=(struct xml_atribut *) 0;
				aptr = aptr->next )
			{
				if (!( aptr->name ))
					continue;
				else if (!( strcmp( aptr->name, _XSD_NAME ) ))
				{
					nptr = aptr->value;
					continue;
				}
				else if ( strcmp( aptr->name, _XSD_MAXOCCURS ) )
					continue;
				else if (!( aptr->value ))
					continue;
				else if (!( vptr = occi_unquoted_value( aptr->value ) ))
					continue;
				else if (!( strcmp( vptr, _XSD_UNBOUNDED ) ))
				{
					liberate(vptr);
					if (!( nptr ))
						continue;
					else if (!( nptr = occi_unquoted_value( nptr ) ))
						continue;

					else if ((status = cords_add_links(dptr,nptr,agent,tls)) != 0)
					{
						liberate( nptr );
						sprintf(buffer,"linkage failure:%s",nptr);
						return(cords_append_error(dptr,status,buffer));
					}
					else
					{
						linked++;
						liberate( nptr );
						break;
					}
				}
				else	liberate(vptr);
			}
		}

		
		/* ----------------------------------------- */
		/* ensure existance of an ID attribute value */
		/* ----------------------------------------- */
		if (!( aptr = document_atribut( dptr, _CORDS_ID ) ))
			return(cords_append_error(dptr,701,"unresolved element"));
		else
		{
			/* -------------------------------- */
			/* recover default attribute values */
			/* -------------------------------- */
			modifications += cords_parser_xsd_default( wptr, dptr );

			if ( modifications )
			{
				if (!( cords_update_category( dptr, aptr->value, agent,tls ) ))
					return(cords_append_error(dptr,704,"updating category"));
				else	modifications=0;
			}

			/* ------------------------------------- */
			/* detect and perform cordscript actions */
			/* ------------------------------------- */
			modifications = cords_parser_xsd_actions( wptr, dptr, agent, tls );
			
			if (!( modifications ))
				return( 0 );
			else if (!( cords_update_category( dptr, aptr->value, agent,tls ) ))
				return(cords_append_error(dptr,704,"updating category"));
			else	return(0);
		}
	}
}

/*	---------------------------------	*/
/*	c o r d s _ d o m a i n _ n a m e	*/
/*	---------------------------------	*/
private	struct	xml_atribut *	cords_domain_name( struct xml_atribut * aptr, char * nptr )
{
	char *	wptr;
	if (!( aptr ))
		return( aptr );
	else if (!( nptr ))
		return( aptr );
	else if (!( wptr = aptr->value ))
		return( aptr );
	else if ( *wptr =='"' )
	{
		if (!( wptr = occi_unquoted_value( wptr ) ))
			return( aptr );
		else
		{
			aptr->value = liberate( aptr->value );
			aptr->value = wptr;
		}
	}
	if ( *wptr != ':' )
		return( aptr );
	else if (!( wptr = allocate( strlen( nptr ) + strlen( aptr->value ) + 3 ) ))
		return( aptr );
	else
	{
		sprintf( wptr, "%s%s",nptr, aptr->value );
		aptr->value = liberate( aptr->value );
		aptr->value = wptr;
		return( aptr );
	}
}

/*	---------------------------------------------------	*/
/*	    c o r d s _ i n t e g r a t e _ f i e l d s		*/
/*	---------------------------------------------------	*/
private	struct occi_response * cords_integrate_fields(
		struct xml_element * document,
		struct occi_response * zptr,
		char * domain, char * agent, char * tls )
{
	char	buffer[2048];
	struct	xml_atribut * aptr;
	struct	occi_element * eptr;
	
	char *	nptr;
	char *	vptr;
	if (!( aptr = document_atribut( document, _CORDS_ID )))
		return( zptr );
	sprintf(buffer,"%s.%s.",domain,document->name);
	for (	eptr = zptr->first;
		eptr != (struct occi_element *) 0;
		eptr = eptr->next )
	{
		/* ----------------------------- */
		/* detect an OCCI Attribute name */
		/* ----------------------------- */
		if (!( nptr = eptr->name ))
			continue;
		else if ( strncmp( eptr->name, buffer,strlen(buffer) ) )
			continue;
		else	nptr += strlen(buffer);

		/* -------------------------- */
		/* Avoid empty or null values */
		/* -------------------------- */
		if (!( eptr->value ))
			continue;
		else if (!( strcmp( eptr->value, _CORDS_NULL ) ))
			continue;
		else if (( aptr = document_atribut( document, nptr )) != (struct xml_atribut *) 0)
			continue;

		/* --------------------------------------- */
		/* add the attribut to the output document */
		/* --------------------------------------- */
		if ( check_debug() )
		{
			printf("integrate_field(%s,%s)\n",nptr,eptr->value);
		}
		if (!( vptr = occi_unquoted_value( eptr->value ) ))
			continue;
		else if (!( aptr = document_add_atribut( document, nptr, vptr ) ))
			continue;
		else	vptr = liberate( vptr );
	}
	return( zptr );
}

/*	---------------------------------------------------	*/
/*		c o r d s _ p a r s e _ e l e m e n t		*/
/*	---------------------------------------------------	*/
/*	this function performs the major decisional work of 	*/
/*	the cords parser. Each element presented here will 	*/
/*	be analysed with an aim to resolving identifier of 	*/
/*	an instance of the required category. The name of 	*/
/*	XML Element will be used as the name of the OCCI	*/
/*	Category. The Attribute list of the XML Element is	*/
/*	inspected to detect an existing instance identifier	*/
/*	in which case the corresponding instance record will 	*/
/*	be retrieved and its Occi Instance Attributes values 	*/
/*	will be used to correct tand extend the values of the	*/
/*	the atribut in the XML Document Attribute List of the	*/
/*	original request.					*/
/*	If an Instance Identifier is not present then an	*/
/*	existing name attribute will be used to locate a	*/
/*	suitable matching category instance. If both these 	*/
/*	fail then remaining attribute of the original XML	*/
/*	request will be tried in turn to try and locate any	*/
/*	suitable matching categories.				*/
/*	In the two latter cases the thus selected Instance	*/
/*	Identifier will be appended to the list of atributs	*/
/*	of the XML Document.					*/
/*	---------------------------------------------------	*/
private	int 	ll_cords_parse_element( 
	char *	domain,
	struct	xml_element * xst, 
	struct 	xml_element * document, char * agent, char * tls, int level )
{
	int	status;
	struct	rest_header * 	hptr;
	struct	xml_element *	eptr;
	struct	xml_atribut *	aptr;
	struct	xml_element *	iptr;
	struct	occi_response *	zptr;
	struct	occi_response *	yptr;
	char		*	vptr;

	/* --------------------- */
	/* ensure valid category */
	/* --------------------- */
	if (!( document ))
		return( 0 );
	else if ((!( strcmp( document->name, _CORDS_ERROR ) ))
	     ||  (!( strcmp( document->name, _CORDS_WARNING ) ))
	     ||  (!( strcmp( document->name, _CORDS_DESCRIPTION ) ))
	     ||  (!( strcmp( document->name, _CORDS_CHOICE  ) )))
		return( 0 );

	/* ------------------------------------- */
	/* check if instance identity is present */
	/* ------------------------------------- */
	if (( aptr = document_atribut( document, _CORDS_ID )) != (struct xml_atribut *) 0)
	{
		/* ------------------------------- */
		/* attempt to retrieve the element */
		/* ------------------------------- */
		if (!( zptr = cords_retrieve_category( document, aptr->value, agent,tls ) ))
			return(cords_append_error(document,705,"retrieving category"));
		/* ----------------------------------- */
		/* integrate retrieved instance fields */
		/* ----------------------------------- */
		else if (!( zptr = cords_integrate_fields( document, zptr, "occi", agent, tls ) ))
			return(cords_append_error(document,705,"integrating category"));
	}
	else
	{
		/* ------------------------------------------ */
		/* check for and use name attribut if present */
		/* ------------------------------------------ */
		if (( aptr = document_atribut( document, _CORDS_NAME )) != (struct xml_atribut *) 0)
		{
			/* ----------------------------- */
			/* set the domain name value now */
			/* ----------------------------- */
			if (!( domain ))
				domain = aptr->value;
			else if (!( aptr = cords_domain_name( aptr, domain ) ))
				return(cords_append_error(document,715,"prefixing domain"));
		
			/* ------------------------------- */
			/* attempt to retrieve the element */
			/* ------------------------------- */
			if (!( zptr = cords_resolve_category( document, aptr, agent,tls ) ))
				return(cords_append_error(document,705,"resolving category by name"));

	  	 	else if (!(status = cords_resolve_location( zptr, document )))
			{
				/* ------------------------------------------ */
				/* retrieve the instance and integrate fields */
				/* ------------------------------------------ */
				if (!( aptr = document_atribut( document, _CORDS_ID )))
					return(cords_append_error(document,705,"resolving category id"));
				else if (( yptr = cords_retrieve_category( document, aptr->value, agent,tls )) != (struct occi_response *) 0)
					if (!( yptr = cords_integrate_fields( document, yptr, "occi", agent, tls ) ))
						return(cords_append_error(document,705,"integrating category"));
			}
			else
			{
				/* ----------------------------- */
				/* attempt to create an instance */
				/* ----------------------------- */
				if (!( zptr = cords_create_category( document, agent,tls )))
					return(cords_append_error(document,706,"creating category"));
				else if ((status = cords_resolve_location( zptr, document )) != 0)
					return(cords_append_error(document,status,"resolving location"));
			}
		}
		else
		{
			/* ---------------------------------------------- */
			/* attempt to resolve for other atributs in order */
			/* ---------------------------------------------- */
			for (	zptr=(struct occi_response *) 0,
				aptr=document->firstatb;
				aptr != (struct xml_atribut *) 0;
				aptr = aptr->next )
			{
				if (!( aptr->name ))
					continue;
				else if (!( strcmp( aptr->name, _CORDS_NAME ) ))
					continue;
				/* ----------------------------------------------------------- */
				/* attempt to resolve the element for the particular attribute */
				/* ----------------------------------------------------------- */
				/* this may turn out to be a little heavy handed: we shall see */
				/* ----------------------------------------------------------- */
				else if (!( zptr = cords_resolve_category( document, aptr, agent,tls ) ))
					continue;
				else	break;
			}

			if ((!( zptr ))
		        ||  ((status = cords_resolve_location( zptr, document )) != 0))
			{
				/* ----------------------------- */
				/* attempt to create an instance */
				/* ----------------------------- */
				if (!( zptr = cords_create_category( document, agent,tls )))
					return(cords_append_error(document,706,"creating category"));
				else if ((status = cords_resolve_location( zptr, document )) != 0)
					return(cords_append_error(document,status,"resolving location"));
			}
		}
	}

	/* --------------------- */
	/* then process sub tree */
	/* --------------------- */
	for (	eptr=document->first;
		eptr != (struct xml_element *) 0;
		eptr = eptr->next )
		if ((status = cords_parse_element( domain, xst, eptr, agent,tls,(level+1) )) != 0)
			return( status );

	/* --------------------------------------------- */
	/* this level of processing can now be completed */
	/* --------------------------------------------- */
	if ( xst )
		return( cords_terminate_xsd( xst, document, agent, tls ) );
	else	return( cords_terminate_level( document, agent,tls ) );

}

/*	---------------------------------------------------	*/
/*	      c o r d s _ p a r s e _ e l e m e n t 		*/
/*	---------------------------------------------------	*/
/*	this is a wrapper for the above function offering 	*/
/*	trace entry and exit trace information in debug mode	*/
/*	---------------------------------------------------	*/
public	int 	cords_parse_element( 
		char *	domain,
		struct xml_element * xst, 
		struct xml_element * document, char * agent, char * tls, int level )
{
	int	status;
	if ( check_debug() )
		printf("\n#enter: cords_parse_element(%s,%u, %s, %s )\n",(domain ? domain : _CORDS_NULL),level,document->name,agent);

	if (!( xst ))
		status = ll_cords_parse_element( domain, xst, document, agent, tls, level );

	else if (!( xst = xsd_element( xst, document->name ) ))
		status = cords_append_error(document,798,"xsd:incorrect element");

	else	status = ll_cords_parse_element( domain, xst, document, agent, tls, level );

	if ( check_debug() )
		printf("#leave: cords_parse_element(%u, %s, %s )\n",level,document->name,agent);
	return( status );
}


/*	---------------------------------------------------	*/
/*	     c o r d s _ d o c u m e n t _ p a r s e r		*/
/*	---------------------------------------------------	*/
/*	this function performs CORDS document parsing from 	*/
/*	the provided filename. The url of the publication 	*/
/*	service to be used during the request, is provided 	*/
/*	by the first parameter.					*/
/*	The function will return the eventually modified 	*/
/*	version of the request with instance identifiers 	*/
/*	and exact details of attribute values for all of 	*/
/*	fields of the different instances comprising the 	*/
/*	provisioning request.					*/
/*	------------------------------------------------	*/
public	struct	xml_element * cords_document_parser( 
	char * host, char * filename, char * agent, char * tls, int usexsd )
{
	int	status=0;
	struct	xml_element *	document;
	struct	xml_element *	xsd;
	struct	xml_element *	xst=(struct xml_element *) 0;
	struct	xml_atribut *	aptr;
	char *			sptr;

	if ( check_verbose() )
		printf("   CORDS Request Parser Phase 1\n");

	if (!( document = document_parse_file( filename ) ))
		return(document);

	else if (!( xsd = document_load_xsd( document ) ))
		return(document);

	else
	{
		if ( check_verbose() )
			printf("   CORDS Request Parser Phase 2\n");

		if ( usexsd )
			cords_document_xsd((xst = xsd));
		else	cords_document_xsd((xst = (struct xml_element *) 0));

		initialise_occi_resolver( host, (char *) 0, (char *) 0, (char *) 0 );

		(void) cords_parse_element( (char *) 0,xst, document, agent, tls, 0 );

		xsd = cords_drop_document( xsd );
		cords_document_xsd( xsd );
		return( document );

	}
}

/*	---------------------------------------------------	*/
/*		c o r d s _ d r o p _ d o c u m e n t 		*/
/*	---------------------------------------------------	*/
/*	This function simply releases the complete document 	*/
/*	tree : all XML atribut lists, and nested elements.	*/
/*	---------------------------------------------------	*/
public	struct	xml_element * cords_drop_document( struct xml_element * document )
{
	return( document_drop( document ) );
}

/*	---------------------------------------------------	*/
/*	  c o r d s _ s e r i a l i s e _ d o c u m e n t	*/
/*	---------------------------------------------------	*/
/*	This function serialises the document provided by	*/
/*	the first parameter in the file identified by the	*/
/*	the second parameter and then release the document.	*/
/*	---------------------------------------------------	*/
public	struct	xml_element * cords_serialise_document( struct xml_element * document, char * result )
{
	if ( document )
	{
		if ( result )
			document_serialise_file( document, result );

		document = cords_drop_document( document );
	}
	return((struct xml_element *) 0);
}


	/* --------------- */
#endif	/* _cords_parser_c */
	/* --------------- */

