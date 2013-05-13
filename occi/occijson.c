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
#ifndef	_occijson_c
#define _occijson_c

#include "global.h"

/*	----------------------------------------------------------	*/
/*		o c c i _ j s o n _ q u o t e d
/*	----------------------------------------------------------	*/
/*	Returns a quoted and allocated string version of the name.	*/
/*	----------------------------------------------------------	*/
private	char *	occi_json_quoted( char * nptr )
{
	char *	rptr;
	if (!( nptr ))
		return( nptr );
	else if (!( rptr = allocate( strlen( nptr ) + 3 ) ))
		return( rptr );
	else
	{
		sprintf(rptr,"%c%s%c",'"',nptr,'"');
		return( rptr );
	}
}

/*	----------------------------------------------------------	*/
/*		o c c i _ j s o n _ u r i				*/	
/*	----------------------------------------------------------	*/
/*	Concatenates the #name string to the url and quotes it all	*/
/*	----------------------------------------------------------	*/
private	char *	occi_json_uri( char * url, char * nptr )
{
	char *	rptr;
	if (!( nptr ))
		return( url );
	else if (!( url ))
		return( nptr );
	else if (!( rptr = allocate( strlen( nptr ) + strlen( url ) + 4 ) ))
		return( rptr );
	else
	{
		sprintf(rptr,"%c%s%s%c",'"',url,nptr,'"');
		return( rptr );
	}
}

/*	----------------------------------------------------------	*/
/*		o c c i _ j s o n _ d o m a i n				*/
/*	----------------------------------------------------------	*/
/*	This function builds a domain name from the provided parts	*/
/*	-----------------------------------------------------------	*/
private	char * occi_json_domain( char * dptr, char * cptr, char * nptr )
{
	char	*	rptr;
	if ((!( nptr )) || (!( cptr )) || (!( dptr )))
		return( (char *) 0);
	else if (!( rptr = allocate( strlen( nptr ) + strlen( cptr ) + strlen( dptr ) + 3 )))
		return( rptr );
	else	
	{
		sprintf(rptr, "%s.%s.%s",dptr, cptr, nptr );
		return( rptr );
	}
}

/*	----------------------------------------------------------	*/
/*		o c c i _ j s o n _ n a m e v a l u e			*/
/*	----------------------------------------------------------	*/
/*	This function builds a simple named value pair of the form	*/
/*									*/
/*		"name": "value"						*/
/*									*/
/*	NB: Both the name and the value of the pair will be quoted	*/
/*	-----------------------------------------------------------	*/
private	char * occi_json_namevalue( char * nptr, char * vptr )
{
	char *	rptr;
	if (!( nptr ))
		return( nptr );
	else if (!( vptr ))
		return( vptr );
	else if (!( rptr = occi_json_quoted( nptr )))
		return( rptr );
	else if (!( vptr = occi_json_quoted( vptr )))
		return( rptr );
	else if (!( rptr = join_string( rptr, ": " ) ))
		return( rptr );
	return( join_strings( rptr, vptr ) );
}

/*	-----------------------------------------------------------	*/
/*		o c c i _ j s o n _ n a m e c o m p le x		*
/*	-----------------------------------------------------------	*/
/*	This function builds a named complex item. The value string	*/
/*	will indicate the nature as either "{" or "[" and will not 	*/
/*	be quoted on output.						*/
/*	-----------------------------------------------------------	*/
private	char * occi_json_namecomplex( char * nptr, char * vptr )
{
	char *	rptr;
	if (!( nptr ))
		return( nptr );
	else if (!( vptr ))
		return( vptr );
	else if (!( rptr = occi_json_quoted( nptr )))
		return( rptr );
	else if (!( rptr = join_string( rptr, ": " ) ))
		return( rptr );
	else 	return( join_string( rptr, vptr ) );
}

/*	-----------------------------------------------------------	*/
/*		o c c i _ j s o n _ d o m a i n c o m p le x		*
/*	-----------------------------------------------------------	*/
/*	This function builds a domain named complex item. 		*/
/*	The value string will be either "{" or "[" and will not 	*/
/*	be quoted on output.						*/
/*	-----------------------------------------------------------	*/
private	char * occi_json_domaincomplex( struct occi_category * optr, char * nptr, char * vptr )
{
	char *	rptr;
	if (!( optr ))
		return( occi_json_namecomplex( nptr , vptr ) );
	else if (!( nptr ))
		return( nptr );
	else if (!( vptr ))
		return( vptr );
	else if (!( nptr = occi_json_domain( optr->domain, optr->id, nptr ) ))
		return( nptr );
	else if (!( rptr = occi_json_quoted( nptr )))
		return( rptr );
	else if (!( rptr = join_string( rptr, ": " ) ))
		return( rptr );
	else
	{
		liberate( nptr );
	 	return( join_string( rptr, vptr ) );
	}
}

/*	----------------------------------------------------------	*/
/*		o c c i _ j s o n _ a t t r i b u t e s 		*/
/*	----------------------------------------------------------	*/
private	char * occi_json_attributes( struct occi_category * optr )
{
	struct	occi_attribute * aptr;
	int	items=0;
	char *	rptr=(char *) 0;
	char *	wptr;

	if (!( rptr = occi_json_namecomplex( "attributes", "[" ) ))
		return( rptr );

	for (	aptr=optr->first;
		aptr != (struct occi_attribute *) 0;
		aptr = aptr->next )
	{
		if ( items++ )
			if (!( rptr = join_string( rptr, ",\r\n" ) ))
				return( rptr );		
		
		if (!( rptr = join_string( rptr, "{" ) ))
			return( rptr );

		else if (!( wptr = occi_json_namevalue( "name", aptr->name )))
			return( liberate( rptr ) );

		else if (!( rptr = join_strings( rptr , wptr ) ))
			return( rptr );

		else if (!( wptr = occi_json_namevalue( 
				"immutable", (aptr->immutable?"true":"false") ) ))
			return( liberate( rptr ) );

		else if (!( rptr = join_string( rptr, "," ) ))
			return( rptr );

		else if (!( rptr = join_strings( rptr , wptr ) ))
			return( rptr );

		else if (!( wptr = occi_json_namevalue( 
				"required", (aptr->mandatory?"true":"false") ) ))
			return( liberate( rptr ) );

		else if (!( rptr = join_string( rptr, "," ) ))
			return( rptr );

		else if (!( rptr = join_strings( rptr , wptr ) ))
			return( rptr );

		else if (!( wptr = occi_json_namevalue( 
				"type", "string" ) ))
			return( liberate( rptr ) );

		else if (!( rptr = join_string( rptr, "," ) ))
			return( rptr );

		else if (!( rptr = join_strings( rptr , wptr ) ))
			return( rptr );

		else if (!( rptr = join_string( rptr, "}" ) ))
			return( rptr );
	}
	return( join_string( rptr, "\r\n]" ) );
}

/*	----------------------------------------------------------	*/
/*		o c c i _ j s o n _ a c t i o n s			*/
/*	----------------------------------------------------------	*/
private	char * occi_json_actions( struct occi_category * optr )
{
	struct	occi_action * aptr;
	int	items=0;
	char *	rptr=(char *) 0;
	char *	tptr;
	
	if (!( rptr = occi_json_namecomplex( "actions", "[" ) ))
		return( rptr );

	for (	aptr=optr->firstact;
		aptr != (struct occi_action *) 0;
		aptr = aptr->next )
	{
		if ( items++ )
			if (!( rptr = join_string( rptr, ",\r\n" ) ))
				return( rptr );		

		if (!( tptr = occi_json_uri( optr->scheme, aptr->name ) ))
			return( liberate( rptr ) );

		else if (!( rptr = join_strings( rptr, tptr ) ))
			return( rptr );
	}
	return( join_string( rptr, "\r\n]" ) );
}

/*	---------------------------------------------------	*/
/*		o c c i _ j s o n _ c a t e g o r y 		*/
/*	---------------------------------------------------	*/
public	char *	occi_json_category( struct occi_category * optr )
{
	char *	rptr =(char *) 0;
	char *	tptr =(char *) 0;
	if (!( rptr = occi_json_namevalue( "term", optr->id ) ))
		return( rptr );
	else if (!( tptr = occi_json_namevalue( "scheme", optr->scheme ) ))
		return( liberate( rptr ) );
	else if (!( rptr = join_string( rptr, "," ) ))
		return( rptr );
	else if (!( rptr = join_strings( rptr, tptr ) ))
		return( rptr );
	else if (!( tptr = occi_json_namevalue( "class", optr->class ) ))
		return( liberate( rptr ) );
	else if (!( rptr = join_string( rptr, "," ) ))
		return( rptr );
	else if (!( rptr = join_strings( rptr, tptr ) ))
		return( rptr );
	else if (!( tptr = occi_json_namevalue( "title", optr->title ) ))
		return( liberate( rptr ) );
	else if (!( rptr = join_string( rptr, "," ) ))
		return( rptr );
	else if (!( rptr = join_strings( rptr, tptr ) ))
		return( rptr );
	else if (!( rptr = join_string( rptr, "," ) ))
		return( rptr );
	else if (!( tptr = occi_json_namevalue( "rel", optr->rel ) ))
		return( liberate( rptr ) );
	else	return( join_strings( rptr, tptr ) );
}

/*	---------------------------------------------------	*/
/*		o c c i _ j s o n _ c a p a c i t y 		*/
/*	---------------------------------------------------	*/
public	char *	occi_json_capacity( struct occi_category * optr )
{
	char *	rptr =(char *) 0;
	char *	tptr =(char *) 0;

	if (!( rptr = allocate_string("{\r\n") ))
		return( rptr );
	else if (!( tptr = occi_json_category( optr ) ))
		return( liberate( rptr ) );
	else if (!( rptr = join_strings( rptr, tptr ) ))
		return( rptr );
	else if (!( tptr = occi_json_attributes( optr ) ))
		return( liberate( rptr ) );
	else if (!( rptr = join_string( rptr, "," ) ))
		return( rptr );
	else if (!( rptr = join_strings( rptr, tptr ) ))
		return( rptr );
	else if (!( tptr = occi_json_actions( optr ) ))
		return( liberate( rptr ) );
	else if (!( rptr = join_string( rptr, "," ) ))
		return( rptr );
	else if (!( rptr = join_strings( rptr, tptr ) ))
		return( rptr );
	else if (!( tptr = occi_json_namevalue( "location", optr->location ) ))
		return( liberate( rptr ) );
	else if (!( rptr = join_string( rptr, "," ) ))
		return( rptr );
	else if (!( rptr = join_strings( rptr, tptr ) ))
		return( rptr );
	else	return( join_string( rptr, "\r\n}" ) );
}

/*	---------------------------------------------------	*/
/*		o c c i _ j s o n _ c a p a c i t i e s		*/
/*	---------------------------------------------------	*/
public	char *	occi_json_capacities( struct occi_category * optr )
{
	char *	rptr =(char *) 0;
	char *	tptr =(char *) 0;
	int	items=0;
	if (!( rptr = allocate_string("{\r\n") ))
		return( rptr );
	else if (!( tptr = occi_json_namecomplex( "categories", "[" ) ))
		return( liberate( rptr ) );
	else if (!( rptr = join_strings( rptr, tptr ) ))
		return( rptr );
	while ( optr )
	{
		if ( items++ )
			if (!( rptr = join_string( rptr, ",\r\n" ) ))
				return( rptr );		

		if (!( tptr = occi_json_capacity( optr ) ))
			return( liberate( rptr ) );
		else if (!( rptr = join_strings( rptr, tptr ) ))
			return( rptr );
		else	optr = optr->next;
	}
	return( join_string( rptr, "\r\n] }" ) );
}

/*	---------------------------------------------------	*/
/*		o c c i _ j s o n _ l i n k			*/
/*	---------------------------------------------------	*/
public	char *	occi_json_link( struct occi_category * optr, char * target, char * id )
{
	char	buffer[8192];
	char *	rptr;
	char *	mptr;
	char *	wptr;
	char *	sptr;
	char *	tptr;
	struct	url * uptr;

	if ((!( optr )) || (!( target )) || (!( id )))
		return((char *) 0);
	else if (!( uptr = analyse_url( target )))
		return((char *) 0);
	else if (!( uptr = validate_url( uptr )))
		return((char *) 0);
	else if (!( mptr = uptr->object))
	{
		liberate( uptr );
		return((char *) 0);
	}
	else if (!( mptr = allocate_string( mptr ) ))
	{
		liberate( uptr );
		return((char *) 0);
	}
	else		
	{
		/* cut out the target category name */
		/* -------------------------------- */
		wptr = mptr;
		if ( *wptr == '/' ) wptr++;
		rptr = sptr = wptr;
		while ( *wptr )
		{
			if ( *wptr == '/' )
				rptr = wptr;
			wptr++;
		}
		if ( *rptr == '/' )
			*(rptr++) = 0;

		if (!( rptr = allocate_string("{\r\n") ))
			return( rptr );
		else if (!( tptr = occi_json_namevalue( "target_uri", target ) ))
			return( liberate( rptr ) );
		else if (!( rptr = join_strings( rptr, tptr ) ))
			return( rptr );
		else if (!( wptr = occi_json_uri( optr->scheme, sptr ) ))
			return( liberate( rptr ) );
		else if (!( tptr = occi_json_namecomplex( "target_type", wptr ) ))
		{
			liberate( wptr );
			return( liberate( rptr ) );
		}
		else if (!( rptr = join_strings( rptr, tptr ) ))
			return( rptr );
		else	liberate( wptr );

		if (!( tptr = occi_json_namevalue( "link_uri", id ) ))
			return( liberate( rptr ) );
		else if (!( rptr = join_strings( rptr, tptr ) ))
			return( rptr );
		else if (!( tptr = occi_json_namevalue( 
				"link_type", 
				"http://schemas.ogf.org/occi/core#link"  ) ))
			return( liberate( rptr ) );
		else if (!( rptr = join_strings( rptr, tptr ) ))
			return( rptr );
		else
		{		
			liberate( mptr );
			liberate( uptr );
			return( rptr );
		}
	}
}

/*	---------------------------------------------------	*/
/*		o c c i _ j s o n _ a t t r i b u t e 		*/
/*	---------------------------------------------------	*/
public	char *	occi_json_attribute( struct occi_category * optr, struct occi_attribute * aptr )
{
	char	buffer[8192];
	char *	rptr;
	if ( optr->domain )
	{
		strcpy( buffer, optr->domain );
		strcat( buffer, "." );
		strcat( buffer, optr->id   );
		strcat( buffer, "." );
		strcat( buffer, aptr->name );
	}
	else 	strcpy( buffer, aptr->name );

	if (!( rptr = occi_json_quoted( buffer )))
		return( rptr );
	else 	return( join_string( rptr, ": ") );
}

/*	---------------------------------------------------	*/
/* 	 o c c i _ j s o n _ a t t r i b u t e _ v a l u e	*/
/*	---------------------------------------------------	*/
public	char *	occi_json_attribute_value( struct occi_category * optr, struct occi_attribute * aptr, char * vptr )
{
	char	buffer[8192];
	if ( optr->domain )
	{
		strcpy( buffer, optr->domain );
		strcat( buffer, "." );
		strcat( buffer, optr->id   );
		strcat( buffer, "." );
		strcat( buffer, aptr->name );
	}
	else 	strcpy( buffer, aptr->name );
	return( occi_json_namevalue( buffer, vptr ) ); 
}

#endif	/* _occijson_c */
	/* ----------- */


