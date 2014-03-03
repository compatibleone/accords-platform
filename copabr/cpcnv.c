/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>     */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License"); 	*/
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at 				*/
/*  									*/
/*  http://www.apache.org/licenses/LICENSE-2.0 				*/
/*  									*/
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 	*/
/* implied. 								*/
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License. 					*/
/* -------------------------------------------------------------------- */
#ifndef	_cpcnv_c
#define	_cpcnv_c

public  struct xml_element * cords_document_convertor( char * filename, int mode );
private struct xml_element * cords_manifest_convertor( char * filename );
private struct xml_element * cords_agreement_convertor( char * filename );

/*	-----------------------------------------------------	*/
/* 	  	c p c n v _  n e w _ a g r e e m e n t		*/
/*	-----------------------------------------------------	*/
private	struct xml_element * cpcnv_new_agreement( char * name, char * id, struct xml_element * context )
{
	struct	xml_element * eptr;
	struct	xml_element * xptr;
	struct	xml_atribut * aptr;
	char 	buffer[2048];
	sprintf(buffer,"ws-agreement-conversion:%s",(id ? id : "unknown" ));
	if (!( eptr = allocate_element() ))
		return( eptr );
	else if (!( eptr->name = allocate_string( "agreement" ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "name", name ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "xmlns", "http://www.compatibleone.fr/schemes/slam.xsd" ) ))
		return( liberate_element( eptr ) );
	else if (!( xptr = document_element( context, "ws:AgreementInitiator" ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "initiator", xptr->value ) ))
		return( liberate_element( eptr ) );
	else if (!( xptr = document_element( context, "ws:AgreementResponder" ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "responder", xptr->value ) ))
		return( liberate_element( eptr ) );
	else if (!( xptr = document_element( context, "ws:ServiceProvider" ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "serviceprovider", xptr->value ) ))
		return( liberate_element( eptr ) );
	else if (!( xptr = document_element( context, "ws:TemplateId" ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "templateid", xptr->value ) ))
		return( liberate_element( eptr ) );
	else if (!( xptr = document_element( context, "ws:TemplateName" ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "templatename", xptr->value ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "description", buffer ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "initiation", "now" ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "expiration", "never" ) ))
		return( liberate_element( eptr ) );
	else	return( eptr );
}

/*	-----------------------------------------------------	*/
/* 	  	c p c n v _  n e w _ m a n i f e s t		*/
/*	-----------------------------------------------------	*/
private	struct xml_element * cpcnv_new_manifest( char * newname )
{
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;
	if (!( eptr = allocate_element() ))
		return( eptr );
	else if (!( eptr->name = allocate_string( "manifest" ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "name", newname ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "xmlns", "http://www.compatibleone.fr/schemes/manifest.xsd" ) ))
		return( liberate_element( eptr ) );
	else	return( eptr );
}

/*	-----------------------------------------------------	*/
/* 	  	c p c n v _  a d d _ e l e m e n t  		*/
/*	-----------------------------------------------------	*/
private	struct xml_element * cpcnv_add_element( struct xml_element * manifest, char * name )
{
	struct	xml_element * eptr;
	if (!( eptr = allocate_element() ))
		return( eptr );
	else if (!( eptr->name = allocate_string( name ) ))
		return( liberate_element( eptr ) );
	else	
	{
		if (!( eptr->previous = manifest->last ))
			manifest->first = eptr;
		else	eptr->previous->next = eptr;
		manifest->last = eptr;
		return( eptr );
	}
}

/*	-----------------------------------------------------	*/
/*			c p c n v _ a d d _ i m a g e 		*/
/*	-----------------------------------------------------	*/
private	struct	xml_element * cpcnv_add_image( struct xml_element * image, char * name, struct xml_element * resource )
{
	struct	xml_atribut * aptr;
	struct	xml_element * sptr;
	struct	xml_element * pptr;
	char *	vptr;
	int	packages=0;
	int	systems=0;
	char buffer[256];
	if (!( aptr = document_add_atribut( image, "name", name ) ))
		return( image );

	for (	;
		resource != (struct xml_element *) 0;
		resource = resource->next )
	{
		if (!( resource->name ))
			continue;
		else if ( strcmp( resource->name, "platformResources" ) != 0 )
			continue;
		else if (!( aptr = document_atribut(resource, "xsi:type" ) ))
			continue;
		else if (!( vptr = allocate_string( aptr->value ) ))
			continue;
		else if (!( vptr = occi_unquoted_value( vptr ) ))
			continue;
		if (!( strcmp( vptr, "ns4:OSResource" ) ))
		{
			vptr = liberate( vptr );
			if (!( aptr = document_atribut( resource, "distribution" ) ))
				continue;
			else if (!( vptr = allocate_string( aptr->value ) ))
				continue;
			else if (!( vptr = occi_unquoted_value( vptr ) ))
				continue;
			if ( systems++ )
			{
				vptr = liberate( vptr );
				break;
			}
			else if (!( sptr = cpcnv_add_element( image, "system" ) ))
			{
				vptr = liberate( vptr );
				break;
			}
			else if (!( aptr = document_add_atribut( sptr, "name", vptr) ))
			{
				vptr = liberate( vptr );
				break;
			}
			vptr = liberate( vptr );
		}
		else if (!( strcmp( vptr, "ns4:Package" ) ))
		{
			packages++;
			vptr = liberate( vptr );
			if (!( aptr = document_atribut( resource, "resourceID" ) ))
				continue;
			else if (!( vptr = allocate_string( aptr->value ) ))
				continue;
			else if (!( vptr = occi_unquoted_value( vptr ) ))
				continue;
			if (!( pptr = cpcnv_add_element( image, "package" ) ))
				break;
			else if (!( aptr = document_add_atribut( pptr, "name", vptr ) ))
				break;
			vptr = liberate( vptr );
		}
		else
		{
			vptr = liberate( vptr );
		}
	}
	return( image );
}

/*	-----------------------------------------------------	*/
/*		    c p c n v _ a d d _ c o m p u t e 		*/
/*	-----------------------------------------------------	*/
private	struct	xml_element * cpcnv_add_compute( struct xml_element * compute, char * name, struct xml_element * resource )
{
	struct	xml_atribut * aptr;
	char *	vptr;
	char buffer[256];
	if (!( aptr = document_add_atribut( compute, "name", name ) ))
		return( compute );
	if ((aptr = document_atribut( resource, "cores" )) != (struct xml_atribut *) 0)
	{
		if ((( vptr = allocate_string(aptr->value )) != (char *) 0)
		&&  (( vptr = occi_unquoted_value(vptr)) != (char *) 0))
		{
			aptr = document_add_atribut( compute, "cores", vptr);
			vptr = liberate( vptr );
		}
	}
	if ((aptr = document_atribut( resource, "architecture" )) != (struct xml_atribut *) 0)
	{
		if ((( vptr = allocate_string(aptr->value )) != (char *) 0)
		&&  (( vptr = occi_unquoted_value(vptr)) != (char *) 0))
		{
			aptr = document_add_atribut( compute, "architecture", vptr);
			vptr = liberate( vptr );
		}
	}
	if ((aptr = document_atribut( resource, "speed" )) != (struct xml_atribut *) 0)
	{
		if ((( vptr = allocate_string(aptr->value )) != (char *) 0)
		&&  (( vptr = occi_unquoted_value(vptr)) != (char *) 0))
		{
			sprintf(buffer,"%uGHz",atoi(vptr) / 1000);
			aptr = document_add_atribut( compute, "speed", buffer);
			vptr = liberate( vptr );
		}
	}
	if ((aptr = document_atribut( resource, "memory" )) != (struct xml_atribut *) 0)
	{
		if ((( vptr = allocate_string(aptr->value )) != (char *) 0)
		&&  (( vptr = occi_unquoted_value(vptr)) != (char *) 0))
		{
			sprintf(buffer,"%uG",atoi(vptr) / 1000);
			aptr = document_add_atribut( compute, "memory", buffer);
			vptr = liberate( vptr );
		}
	}

	return( compute );
}

/*	-----------------------------------------------------	*/
/*			c p c n v _ a d d _ n o d e 		*/
/*	-----------------------------------------------------	*/
private	struct xml_element * cpcnv_add_node( struct xml_element * manifest, struct xml_element * services, struct xml_element * part )
{
	struct	xml_element * eptr;
	struct	xml_element * iptr;
	struct	xml_element * xptr;
	struct	xml_element * pptr;
	struct	xml_element * qptr;
	struct	xml_atribut * aptr;
	char *	nodename;
	char *	wptr;
	if (!( aptr = document_atribut( part, "composable" ) ))
		return((struct xml_element *) 0);
	else if (!( nodename = allocate_string( aptr->value ) ))
		return((struct xml_element *) 0);
	else if (!( nodename = occi_unquoted_value( nodename ) ))
		return((struct xml_element *) 0);
	else 
	{
		for ( ;
			services != (struct xml_element *) 0;
			services = services->next )
		{
			if (!( aptr = document_atribut( services, "resourceID" ) ))
				continue;
			else if (!( wptr = allocate_string( aptr->value ) ))
				continue;
			else if (!( wptr = occi_unquoted_value( wptr ) ))
				continue;
			else if (!( strcmp( wptr, nodename ) ))
			{
				wptr = liberate( wptr );
				break;
			}
			else
			{
				wptr = liberate( wptr );
				continue;	
			}
		}
		if (!( services ))
			return((struct xml_element *) 0);
	}
	if (!( eptr = allocate_element() ))
		return( eptr );
	else if (!( eptr->name = allocate_string( "node" ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "name", nodename ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "access", "public" ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "scope", "normal" ) ))
		return( liberate_element( eptr ) );
	else if (!( aptr = document_add_atribut( eptr, "type", "simple" ) ))
		return( liberate_element( eptr ) );
	else
	{
		if (!( eptr->previous = manifest->last ))
			manifest->first = eptr;
		else	eptr->previous->next = eptr;
		manifest->last = eptr;
		if (!( iptr = cpcnv_add_element( eptr, "infrastructure" ) ))
			return( eptr );
		else if (!( xptr = cpcnv_add_element( iptr, "compute" ) ))
			return( eptr );
		else
		{
			if (((pptr = document_element(services,"resources" ) ) != (struct xml_element *) 0)
			&&  ((qptr = document_element(pptr,"infrastructureResources" ) ) != (struct xml_element *) 0))
				cpcnv_add_compute(xptr, nodename, qptr );
		}
		if (!( xptr = cpcnv_add_element( iptr, "storage" ) ))
			return( eptr );
		else if (!( xptr = cpcnv_add_element( iptr, "network" ) ))
			return( eptr );
		else if (!( pptr = cpcnv_add_element( xptr, "port" ) ))
			return( eptr );
		else if (!( pptr = cpcnv_add_element( xptr, "port" ) ))
			return( eptr );
		if (!( iptr = cpcnv_add_element( eptr, "image" ) ))
			return( eptr );
		else
		{
			if (((pptr = document_element(services,"resources" ) ) != (struct xml_element *) 0)
			&&  ((qptr = document_element(pptr,"platformResources" ) ) != (struct xml_element *) 0))
				cpcnv_add_image(iptr, nodename, qptr );
			else if (!( xptr = cpcnv_add_element( iptr, "system" ) ))
				return( eptr );
		}
		return( eptr );
	}
}

/*	-----------------------------------------------------	*/
/*	    c o r d s _ d o c u m e n t _ c o n v e r t o r 	*/
/*	-----------------------------------------------------	*/
public 	struct xml_element * cords_document_convertor( char * filename , int mode )
{
	switch ( mode )
	{
	case	1	:
		return( cords_manifest_convertor( filename ) );
	case	2	:
		return( cords_agreement_convertor( filename ) );
	default		:
		return((struct xml_element *) 0);
	}
}

/*	-----------------------------------------------------	*/
/*	  c o r d s _ a g r e e m e n t _ c o n v e r t o r 	*/
/*	-----------------------------------------------------	*/
private	struct xml_element * cords_agreement_convertor( char * filename )
{
	char *	newname=(char *) 0;
	struct	xml_element * document;
	struct	xml_element * eptr;
	struct	xml_element * sptr;
	struct	xml_element * kptr;
	struct	xml_element * cptr;
	struct	xml_element * gptr;
	struct	xml_element * mptr;
	struct	xml_element * nptr;
	struct	xml_element * dptr;
	struct	xml_element * node=(struct xml_element *) 0;
	struct	xml_element * pptr;
	struct	xml_atribut * aptr;
	struct	xml_element * agreement=(struct xml_element *) 0;
	struct	xml_element * usdl;

	if (!( document = document_parse_file( filename ) ))
		return( (struct xml_element *) 0);
	else if (!( eptr = document_element( document, "ws:AgreementProperties" ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	else if (!( nptr = document_element( eptr, "ws:Name" ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	else if (!( dptr = document_element( eptr, "ws:AgreementId" ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	else if (!( kptr = document_element( eptr, "ws:Context" ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	else if (!( eptr = document_element( eptr, "ws:Terms" ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	else if (!( eptr = document_element( eptr, "ws:All" ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	else if (!( eptr = document_element( eptr, "ws:ServiceDescriptionTerm" ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	/* ----------------------------- */
	/* locate start of USDL document */
	/* ----------------------------- */
	else if (!( usdl = document_element( eptr, "ns2:USDL3Document" ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	/* ---------------------------------- */
	/* locate service description section */
	/* ---------------------------------- */
	else if (!( sptr = document_element( usdl, "services" ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	/* ---------------------------------- */
	/* locate service composition section */
	/* ---------------------------------- */
	else if (!( mptr = document_element( usdl, "compositeServices" ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	else if (!( aptr = document_atribut( mptr, "resourceID" ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	else if (!( newname = allocate_string( aptr->value ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	else if (!( newname = occi_unquoted_value( aptr->value ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	else if (!( agreement = cpcnv_new_agreement(nptr->value, dptr->value, kptr) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	else
	{
		if ((!( sptr = cpcnv_add_element( agreement, "terms" )))
		|| (!( aptr = document_add_atribut( sptr, "name", ":services" ) ))
		|| (!( sptr = cpcnv_add_element( sptr, "term" )))
		|| (!( mptr = cpcnv_add_element( sptr, "manifest" )))
		|| (!( aptr = document_add_atribut( mptr, "name", newname ) ))
		|| (!( aptr = document_add_atribut( sptr, "name", ":services" ) ))
		|| (!( cptr = cpcnv_add_element( agreement, "terms" )))
		|| (!( aptr = document_add_atribut( cptr, "name", ":conditions" ) ))
		|| (!( cptr = cpcnv_add_element( cptr, "term" )))
		|| (!( aptr = document_add_atribut( cptr, "name", ":conditions" ) ))
		|| (!( gptr = cpcnv_add_element( agreement, "terms" )))
		|| (!( aptr = document_add_atribut( gptr, "name", ":guarantees" ) ))
		|| (!( gptr = cpcnv_add_element( gptr, "term" )))
		|| (!( aptr = document_add_atribut( gptr, "name", ":guarantees" ) )))
		{
			document = document_drop( document );
			return((struct xml_element *) 0);
		}
		document = document_drop( document );
		return( agreement );
	}
}

/*	-----------------------------------------------------	*/
/*	    c o r d s _ m a n i f e s t _ c o n v e r t o r 	*/
/*	-----------------------------------------------------	*/
private struct xml_element * cords_manifest_convertor( char * filename )
{
	char *	newname=(char *) 0;
	struct	xml_element * document;
	struct	xml_element * eptr;
	struct	xml_element * sptr;
	struct	xml_element * mptr;
	struct	xml_element * node=(struct xml_element *) 0;
	struct	xml_element * pptr;
	struct	xml_atribut * aptr;
	struct	xml_element * manifest=(struct xml_element *) 0;
	struct	xml_element * usdl;

	if (!( document = document_parse_file( filename ) ))
		return( (struct xml_element *) 0);
	else if (!( eptr = document_element( document, "ws:AgreementProperties" ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	else if (!( eptr = document_element( eptr, "ws:Terms" ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	else if (!( eptr = document_element( eptr, "ws:All" ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	else if (!( eptr = document_element( eptr, "ws:ServiceDescriptionTerm" ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	/* ----------------------------- */
	/* locate start of USDL document */
	/* ----------------------------- */
	else if (!( usdl = document_element( eptr, "ns2:USDL3Document" ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	/* ---------------------------------- */
	/* locate service description section */
	/* ---------------------------------- */
	else if (!( sptr = document_element( usdl, "services" ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	/* ---------------------------------- */
	/* locate service composition section */
	/* ---------------------------------- */
	else if (!( mptr = document_element( usdl, "compositeServices" ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	else if (!( aptr = document_atribut( mptr, "resourceID" ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	else if (!( newname = allocate_string( aptr->value ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	else if (!( newname = occi_unquoted_value( aptr->value ) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 

	else if (!( manifest = cpcnv_new_manifest(newname) ))
	{
		document = document_drop( document );
		return( (struct xml_element *) 0);
	} 
	else
	{
		/* --------------------------------- */
		/* process service composition parts */
		/* --------------------------------- */
		for (	pptr = document_element( mptr, "parts" );
			pptr != (struct xml_element *) 0;
			pptr = pptr->next )
		{
			if (!( node = cpcnv_add_node( manifest, sptr, pptr ) ))
				break;
		}
		if ((!( eptr = cpcnv_add_element( manifest, "configuration" )))
		||  (!( eptr = cpcnv_add_element( manifest, "release"       )))
		||  (!( eptr = cpcnv_add_element( manifest, "security"      )))
		||  (!( eptr = cpcnv_add_element( manifest, "account"       ))))
		{
			document = document_drop( document );
			manifest = document_drop( manifest );
			return( (struct xml_element *) 0);
		} 
		else if (!( aptr = document_add_atribut( eptr, "name", "accords" ) ))
		{
			document = document_drop( document );
			manifest = document_drop( manifest );
			return( (struct xml_element *) 0);
		} 
		document = document_drop( document );
		return( manifest );
	}
}

	/* ------- */
#endif 	/* cpcnv_c */
	/* ------- */

