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
#ifndef	_document_c
#define	_document_c

#include "document.h"

private	struct xml_element * parseresult=(struct xml_element *) 0;

/*	---------------------------------------------------	*/
/*		d o c u m e n t _ o p e n _ t a g 		*/
/*	---------------------------------------------------	*/
private	int	document_open_tag( struct xml_application * xptr, char * token )
{
	struct	xml_element * eptr;
	if (!( eptr = allocate_element() ))
		return(27);
	else if (!( eptr->name = allocate_string( token ) ))
		return( 27 );
	else
	{
		add_element( xptr->payload[0], eptr );
		xptr->payload[0] = eptr;
		return(0);
	}
}

/*	---------------------------------------------------	*/
/*		d o c u m e n t _ a t b _ n a m e 		*/
/*	---------------------------------------------------	*/
private	int	document_atb_name( struct xml_application * xptr, char * token )
{
	struct	xml_atribut * aptr;
	if (!( aptr = allocate_atribut() ))
		return(27);
	else if (!( aptr->name = allocate_string( token ) ))
		return( 27 );
	else
	{
		add_atribut( xptr->payload[0], aptr );
		return(0);
	}
}

/*	---------------------------------------------------	*/
/*		d o c u m e n t _ a t b _ v a l u e		*/
/*	---------------------------------------------------	*/
private	int	document_atb_value( struct xml_application * xptr, char * token )
{
	struct	xml_atribut * aptr;
	struct	xml_element * eptr;
	if (!( eptr = xptr->payload[0] ))
		return(118 );
	else if (!( aptr = eptr->lastatb ))
		return( 48 );
	else if (!( aptr->value = allocate_string( token ) ))
		return( 27 );
	else	return(0);
}

/*	---------------------------------------------------	*/
/*	      d o c u m e n t _ a d d _ e l e m e n t		*/
/*	---------------------------------------------------	*/
public	struct	xml_element * 	document_add_element( struct xml_element * dptr, char * nptr )
{
	struct	xml_element * eptr;
	if (!( eptr = allocate_element() ))
		return(eptr);
	else if (!( eptr->name = allocate_string( nptr ) ))
		return(liberate_element(eptr));
	else 
	{
		add_element( dptr, eptr );
		return( eptr );
	}
}

/*	---------------------------------------------------	*/
/*	   d o c u m e n t _ p r e f ix _ e l e m e n t		*/
/*	---------------------------------------------------	*/
public	struct	xml_element * 	document_prefix_element( struct xml_element * dptr, char * nptr )
{
	struct	xml_element * eptr;
	if (!( eptr = allocate_element() ))
		return(eptr);
	else if (!( eptr->name = allocate_string( nptr ) ))
		return(liberate_element(eptr));
	else if (!( eptr->next = dptr->first ))
		dptr->last = eptr;
	else	eptr->next->previous = eptr;
	dptr->first = eptr;
	eptr->parent = dptr;
	return( eptr );
}

/*	---------------------------------------------------	*/
/*	      d o c u m e n t _ a d d _ a t r i b u t		*/
/*	---------------------------------------------------	*/
public	struct	xml_atribut * 	document_add_atribut( struct xml_element * eptr, char * nptr, char * vptr )
{
	struct	xml_atribut * aptr;
	if (!( aptr = allocate_atribut() ))
		return(aptr);
	else if (!( aptr->name = allocate_string( nptr ) ))
		return(liberate_atribut(aptr));
	else if (!( aptr->value = allocate_string( vptr ) ))
		return(liberate_atribut(aptr));
	else
	{
		add_atribut( eptr, aptr );
		return(aptr);
	}
}

/*	---------------------------------------------------	*/
/*		d o c u m e n t _ w h i t e _ s p a c e		*/
/*	---------------------------------------------------	*/
private	int	document_white_space( struct xml_application * xptr, int c )
{
	struct	xml_element * eptr;
	char	token[2];
	token[0] = c;
	token[1] = 0;
	if (!( eptr = xptr->payload[0] ))
		return(48);
	else if (!( eptr->value ))
		return(0);
	else 	return( extend_value( &eptr->value , token, strlen( token ) ) );
}

/*	---------------------------------------------------	*/
/*		d o c u m e n t _ p un c t u a t i o n		*/
/*	---------------------------------------------------	*/
private	int	document_punctuation( struct xml_application * xptr, int c )
{
	struct	xml_element * eptr;
	char	token[2];
	token[0] = c;
	token[1] = 0;
	if (!( eptr = xptr->payload[0] ))
		return(48);
	else 	return( extend_value( &eptr->value , token, strlen( token ) ) );
}

/*	---------------------------------------------------	*/
/*		d o c u m e n t _ t o k e n			*/
/*	---------------------------------------------------	*/
private	int	document_token( struct xml_application * xptr, char * token )
{
	struct	xml_element * eptr;
	if (!( eptr = xptr->payload[0] ))
		return(48);
	else 	return( extend_value( &eptr->value , token, strlen( token ) ) );
}

/*	---------------------------------------------------	*/
/*		d o c u m e n t _ c l o s e _ t a g		*/
/*	---------------------------------------------------	*/
private	int	document_close_tag( struct xml_application * xptr )
{
	struct	xml_element * eptr;
	if (!( eptr = xptr->payload[0] ))
		return(48);
	else
	{
		if (!( eptr->parent ))
			parseresult = eptr;
	 	xptr->payload[0] = eptr->parent;
		return(0);
	}
}

/*	---------------------------------------------------	*/
/*		d o c u m e n t _ e l e m e n t 		*/
/*	---------------------------------------------------	*/
public	struct	xml_element * document_element( struct xml_element * document, char * token )
{
	struct	xml_element * eptr;
	if (!( document ))
		return( document );
	else if (( document->name )
	     &&  (!( strcmp( document->name, token ) )))
		return( document );
	for (	eptr = document->first;
		eptr != (struct xml_element *) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( strcmp( eptr->name, token ) ))
			break;
	}
	return( eptr );
}

/*	---------------------------------------------------	*/
/*		d o c u m e n t _ a t r i b u t 		*/
/*	---------------------------------------------------	*/
public	struct	xml_atribut * document_atribut( struct xml_element * element, char * token )
{
	struct	xml_atribut * aptr;
	if (!( element ))
		return((struct xml_atribut *) 0);
	for (	aptr = element->firstatb;
		aptr != (struct xml_atribut *) 0;
		aptr = aptr->next )
	{
		if (!( aptr->name ))
			continue;
		else if (!( strcmp( aptr->name, token ) ))
			break;
	}
	return( aptr );
}

/*	---------------------------------------------------	*/
/*	    d o c u m e n t _ a t r i b u t _ v a l u e 	*/
/*	---------------------------------------------------	*/
public	int	document_atribut_value( struct xml_atribut * aptr )
{
	char *	s;
	if (!( s = aptr->value ))
		return( 0 );
	else if (( *s != 0x0022 ) && ( *s != 0x0027 ))
		return( atoi( s ) );
	else	return( atoi( (s+1) ) );
}

/*	---------------------------------------------------	*/
/*	   d o c u m e n t _ a t r i b u t _ s t r i n g	*/
/*	---------------------------------------------------	*/
public	char * 	document_atribut_string( struct xml_atribut * aptr )
{
	char *	r;
	char *	s;
	int	l;
	if (!( s = aptr->value ))
		return( s );
	else if (( *s != 0x0022 ) && ( *s != 0x0027 ))
		return( allocate_string(s) );
	else if (!( r = allocate((l = strlen(++s))) ))
		return( r );
	else 	{
		l--;
		memcpy(r, s , l );
		*(r+l) = 0;
		return( r );
		}
}

/*	---------------------------------------------------	*/
/*	    		d o c u m e n t _ d r o p 		*/
/*	---------------------------------------------------	*/
public	struct	xml_element * document_drop( struct xml_element * document )
{
	liberate_element( document );
	return((struct xml_element *) 0);
}

/*	---------------------------------------------------	*/
/*		d o c u m e n t _ p a r s e _ f i l e		*/
/*	---------------------------------------------------	*/
public	struct	xml_element * document_parse_file( char * sptr )
{
	struct	xml_relay relays = 
	{
	document_open_tag,	
	document_atb_name,	
	document_atb_value,	
	document_white_space,	
	document_token,		
	document_punctuation,	
	document_close_tag,
	(void *) 0
	};
	parseresult = (struct xml_element *) 0;
	xml_parse( sptr, &relays, (void *) 0);
	return( parseresult );
	return(0);
}

/*	---------------------------------------------------	*/
/*	d o c u m e n t _ s e r i a l i s e _ e l e m e n t	*/
/*	---------------------------------------------------	*/
private	void	document_serialise_element( FILE * h, struct xml_element * eptr, int level )
{
	struct	xml_atribut * aptr;
	struct	xml_element * cptr;
	char	*	sptr;
	int	i;

	for (;  eptr != (struct xml_element *) 0; eptr = eptr->next )
	{
		for (i=0; i < level; i++) fprintf(h,"\t");
		fprintf(h,"<");
		fprintf(h,"%s",eptr->name);
		for (	aptr=eptr->firstatb; 
			aptr != (struct xml_atribut *) 0; 
			aptr = aptr->next )
		{
			fprintf(h,"\n");
			for (i=0; i < (level+1); i++) fprintf(h,"\t");
			fprintf(h,"%s=%c%",aptr->name,0x0022);
			if ( aptr->value )
			{
				for ( 	sptr=aptr->value;
					*sptr != 0;
					sptr++)
					if ( *sptr != '"' )
						fprintf(h,"%c",*sptr);
			
			}
			fprintf(h,"%c",0x0022);
		}
		if ((!( eptr->first )) && (!( eptr->value )))
		{	fprintf(h,"/>\n");	}
		else	
		{
			fprintf(h,">\n");
			document_serialise_element( h, eptr->first,(level+1) );
			if ( eptr->value )
			{
				for (i=0; i < level; i++) fprintf(h,"\t");
				fprintf(h,"%s\n",eptr->value);
			}
			for (i=0; i < level; i++) fprintf(h,"\t");
			fprintf(h,"</%s>\n",eptr->name);
		}
	}
}

/*	---------------------------------------------------	*/
/*	   d o c u m e n t _ s e r i a l i s e _ f i l e 	*/
/*	---------------------------------------------------	*/
public	int	document_serialise_file( struct xml_element * eptr, char * filename )
{
	FILE * h;
	if (!( h = fopen( filename, "w" ) ))
		return(46);
	else
	{
		document_serialise_element( h, eptr, 0 );
		fclose(h);
		return(0);
	}
}


#endif	/* _document_c */
	/* ----------- */

