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
#ifndef	_occi_c
#define _occi_c

#include "occi.h"
#include "url.h"
#include "occipar.c"
#include "occiact.c"
#include "occiatb.c"
#include "occicat.c"
#include "occilink.c"
#include "occimixin.c"
#include "linknode.c"
#include "kindnode.c"

/*	---------------------------------------------------	*/
/*		o c c i _ r e m o v e _ c a t e g o r y		*/
/*	---------------------------------------------------	*/
public	struct	occi_category * occi_remove_category( struct occi_category * cptr )
{
	if ( cptr->interface )
		cptr->interface = liberate( cptr->interface );
	return( liberate_occi_category( cptr ) );
}

/*	---------------------------------------------------	*/
/*	   a l l o c a t e _ o c c i _ i n t e r f a c e	*/
/*	---------------------------------------------------	*/
private	struct	rest_interface * allocate_occi_interface()
{
	struct	rest_interface * iptr;
	if (!( iptr = allocate( sizeof( struct rest_interface ) ) ))
		return( iptr );
	else
	{
		memset( iptr, 0, sizeof( struct rest_interface ));
		return( iptr );
	}
}

/*	---------------------------------------------------	*/
/*		o c c i _ c r e a t e _ c a t e g o r y		*/
/*	---------------------------------------------------	*/
public	struct	occi_category * occi_create_category( char * domain, char * name, char * scheme, char * klass, char * rel, char * title )
{
	struct	occi_category * cptr;
	if (!( cptr = allocate_occi_category() ))
		return( cptr );
	else if (!( cptr->domain = allocate_string( domain ) ))
		return( occi_remove_category( cptr ) );
	else if (!( cptr->id = allocate_string( name ) ))
		return( occi_remove_category( cptr ) );
	else if (!( cptr->scheme = allocate_string( scheme ) ))
		return( occi_remove_category( cptr ) );
	else if (!( cptr->class = allocate_string( klass ) ))
		return( occi_remove_category( cptr ) );
	else if (!( cptr->rel = allocate_string( rel ) ))
		return( occi_remove_category( cptr ) );
	else if (!( cptr->title = allocate_string( title ) ))
		return( occi_remove_category( cptr ) );
	else if (!( cptr->location = allocate( strlen( name ) + 8 ) ))
		return( occi_remove_category( cptr ) );
	else if (!( cptr->interface = allocate_occi_interface() ))
		return( occi_remove_category( cptr ) );
	{
		sprintf(cptr->location,"/%s/",cptr->id);
		return( cptr );
	}
}

public	struct	occi_category * occi_create_os_template( char * domain, char * name, char * schema, char * title )
{
	return( occi_create_category( domain, name, schema, "mixin", _OCCI_OS_TEMPLATE , title ) );
}

 
public	struct	occi_category * occi_create_resource_template( char * domain, char * name, char * schema, char * title )
{
	return( occi_create_category( domain, name, schema, "mixin", _OCCI_RESOURCE_TEMPLATE, title ) );
}

 
/*	---------------------------------------------------	*/
/*		o c c i _ a d d _ a c t i o n           	*/
/*	---------------------------------------------------	*/
public	struct	occi_category * occi_add_action(struct occi_category * cptr,char * name,char * parameters, void * target )
{
	struct	occi_action * aptr;
	if (!( aptr = add_occi_action( cptr ) ))
		return( occi_remove_category( cptr ) );
	else if (!( aptr->name = allocate_string( name ) ))
		return( occi_remove_category( cptr ) );
	else
	{
		aptr->action = target;
		return( cptr );
	}
}

/*	---------------------------------------------------	*/
/*		o c c i _ r e s o l v e _ a c t i o n		*/
/*	---------------------------------------------------	*/
public	struct	occi_action *	occi_resolve_action( struct occi_category * optr, char * name )
{
	struct	occi_action * aptr;
	for ( aptr=optr->firstact;
		aptr != (struct occi_action *) 0;
		aptr = aptr->next )
		if (!( strcmp( aptr->name, name ) ))
			break;
	return( aptr );
}

/*	---------------------------------------------------	*/
/*		o c c i _ a d d _ a t t r i b u t e     	*/
/*	---------------------------------------------------	*/
public	struct	occi_category * occi_add_attribute(struct occi_category * cptr,char * name,int m,int i )
{
	struct	occi_attribute * aptr;
	if (!( aptr = add_occi_attribute( cptr ) ))
		return( occi_remove_category( cptr ) );
	else if (!( aptr->name = allocate_string( name ) ))
		return( occi_remove_category( cptr ) );
	else
	{
		aptr->mandatory = m;
		aptr->immutable = i;
		return( cptr );
	}
}

/*	---------------------------------------------------	*/
/*	     o c c i _ p a r s e _ a t t r i b u t e s		*/
/*	---------------------------------------------------	*/
public	struct	occi_category *	occi_parse_attributes( struct occi_category * optr, char * sptr )
{
	char *	nptr;
	int	m;
	int	i;
	int	c;
	char *	vptr;
	while ( *sptr )
	{
		while (( *sptr == ' ' ) || ( *sptr == '"')) sptr++;
		nptr = sptr;
		while (( *sptr ) && ( *sptr != ' ') && ( *sptr != ',' ) && ( *sptr != '{')) sptr++;
		if ((c = *sptr) != 0) *(sptr++) = 0;
		if ( c == '{' )
		{
			vptr = sptr;
			while (( *sptr ) && ( *sptr != '}')) sptr++;
			if ( *sptr ) *(sptr++) = 0;
			if (!( strncmp(vptr,"mandatory",strlen("mandatory")) ))
				m = 1;
			else if (!( strncmp(vptr,"immutable",strlen("immutable")) ))
				i = 1;
		}
		else	m=i=0;
		if (!( optr = occi_add_attribute( optr, nptr, m, i ) ))
			break;
	}
	return( optr );
}

/*	---------------------------------------------------	*/
/*		o c c i _ p a r s e _ a c t i o n s 		*/
/*	---------------------------------------------------	*/
public	struct	occi_category *	occi_parse_actions( struct occi_category * optr, char * sptr )
{
	return( optr );
}


/*	---------------------------------------------------	*/
/*	     o c c i _ u n q u o t e d _ v a l u e  		*/
/*	---------------------------------------------------	*/
public 	char *	occi_unquoted_value( char * sptr )
{
	int	c;
	int	l;
	if (!( sptr ))
		return( sptr );
	else if (( *sptr != '"' ) && ( *sptr != 0x0027 ))
		return( allocate_string( sptr ) );
	else
	{
		c = *(sptr++);
		if ((l = strlen(sptr)) > 0)
			if ( *(sptr+(l-1)) == c )
				*(sptr+(l-1)) = 0;
		return( allocate_string( sptr ) );
	}
}

/*	---------------------------------------------------	*/
/*	     o c c i _ u n q u o t e d _ v a l u e  		*/
/*	---------------------------------------------------	*/
public 	char *	occi_unquoted_link( char * sptr )
{
	char *	rptr;
	if (!( sptr ))
		return( sptr );
	else if (!( sptr = occi_unquoted_value( sptr )))
		return( sptr );
	else if ( *sptr != '<' )
		return( allocate_string( sptr ) );
	else
	{
		sptr++;
		for ( rptr=sptr; *sptr != 0; sptr++ )
			if ( *sptr == '>' ) *sptr = 0;
		return( allocate_string( rptr ) );
	}
}

/*	---------------------------------------------------	*/
/*		o c c i _ p a r s e _ c a p a c i t y 		*/
/*	---------------------------------------------------	*/
public	struct	occi_category *	occi_parse_category( char * sptr )
{
	struct	occi_category *	optr;
	struct	occi_category *	xptr;
	char	*	wptr;
	char	*	rptr;
	char	*	vptr;
	char 		terminator;
	if (!( optr = allocate_occi_category() ))
		return( optr );
	else if (!( wptr = allocate_string( sptr )))
		return( occi_remove_category( optr ) );
	else	
	{
		sptr = wptr;
	
		/* category name */
		/* ------------- */
		while ( *sptr == ' ' ) sptr++;
		rptr = sptr;
		while (( *sptr ) && ( *sptr != ';' )) sptr++;
		if ( *sptr == ';' ) *(sptr++) = 0;
		if (!( optr->id = allocate_string( rptr )))
		{
			liberate( wptr );
			return( occi_remove_category( optr ) );
		}
		else	rptr = sptr;

		while ( *rptr != 0 )
		{
			while ( *rptr == ' ' ) rptr++;
			if (!( *rptr )) 
				break;
			else	sptr = rptr;
			while (( *sptr ) && ( *sptr != '=' ) && ( *sptr != ';')) sptr++;
			if ( *sptr == '=' )
			{
				*(sptr++) = 0;
				vptr = sptr;
				if ( *sptr == 0x0022 )
				{
					sptr++;
					while (( *sptr ) && ( *sptr != 0x0022)) sptr++;
					while (( *sptr ) && ( *sptr != ';') && ( *sptr != ',')) sptr++;
				}
				else
				{
					while (( *sptr ) && ( *sptr != ';')) sptr++;
				}
				switch (( terminator = *sptr ))
				{
				case	';'	:
				case	','	:
					*(sptr++) = 0;
					break;
				}
				if (!( strcmp( rptr, "scheme" )))
					optr->scheme=occi_unquoted_value( vptr );
				else if (!( strcmp( rptr, "class" )))
					optr->class=occi_unquoted_value( vptr );
				else if (!( strcmp( rptr, "rel" )))
					optr->rel=occi_unquoted_value( vptr );
				else if (!( strcmp( rptr, "title" )))
					optr->title=occi_unquoted_value( vptr );
				else if (!( strcmp( rptr, "location" )))
					optr->location=occi_unquoted_value( vptr );
				else if (!( strcmp( rptr, "attributes" )))
				{
					if (!( optr = occi_parse_attributes( optr, vptr )))
					{
						liberate( wptr );
						return( optr );
					}
				}
				else if (!( strcmp( rptr, "actions" )))
				{
					if (!( optr = occi_parse_actions( optr, vptr )))
					{
						liberate( wptr );
						return( optr );
					}
				}
				if ( terminator == ',' )
				{
					if ((xptr = occi_parse_category( sptr )) != (struct occi_category *) 0)
					{
						xptr->previous = optr;
						optr->next = xptr;
					}
					break;
				}
			}
			else if ( *sptr == ';' )
				*(sptr++) = 0;
			rptr = sptr;
		}
		liberate( wptr );
		return( optr );
	}
}

#include "occihttp.c"
#include "occijson.c"
#include "occixml.c"

/*	---------------------------------------------------	*/
/*	    o c c i _ r e s o l v e _ a t t r i b u t e		*/
/*	---------------------------------------------------	*/
public	struct	occi_attribute * occi_resolve_attribute( struct occi_category * optr, char * nptr )
{
	struct	occi_attribute * aptr;
	
	for (	aptr = optr->first;
		aptr != (struct occi_attribute *) 0;
		aptr = aptr->next )
	{
		if (!( aptr->name ))
			continue;
		else if (!( strcmp( aptr->name, nptr ) ))
			break;
	}
	return( aptr );

}

/*	---------------------------------------------------	*/
/*		  o c c i _ c a t e g o r y _ i d		*/
/*	---------------------------------------------------	*/
public	char *	occi_category_id( char * sptr )
{
	char	*	wptr;
	char	*	last;
	
	if (!( sptr ))
		return( sptr );
	else 
	{
		for ( 	last=wptr=sptr;
			*wptr != 0;
			wptr ++ )
		{
			if ( *wptr == '/' )
				last = wptr;
		}
		if ( *last == '/' ) last++;
		return( allocate_string( last ) );
	}
}
		
#endif	/* _occi_c */
	/* ------- */


