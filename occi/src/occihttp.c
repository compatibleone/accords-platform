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
#ifndef	_occihttp_c
#define _occihttp_c

/*	---------------------------------------------------	*/
/*		o c c i _ h t t p _ c a p a c i t y 		*/
/*	---------------------------------------------------	*/
public	char *	occi_http_capacity( struct occi_category * optr )
{
	char *	result=(char *) 0;
	char *	wptr;
	char	buffer[8192];
	char  *	term;
	int	i;
	struct	occi_attribute * mptr;
	struct	occi_action    * fptr;

	sprintf(buffer,"%s; scheme=\"%s\"; class=%s; rel=\"%s\";",
		optr->id,optr->scheme,optr->class,
		( rest_valid_string( optr->rel ) ? optr->rel : "") );

	if (!( result = allocate_string( buffer ) ))
		return( result );

	if ( optr->first )
	{
		strcpy( buffer, " attributes");
		if (!( result = join_string( result, buffer )))
			return( result );
		term = "=\"";
		for (	mptr = optr->first;
			mptr != (struct occi_attribute *) 0;
			mptr = mptr->next )
		{
			strcpy( buffer, term );
			strcat( buffer, optr->domain );
			strcat( buffer, "." );
			strcat( buffer, optr->id   );
			strcat( buffer, "." );
			strcat( buffer, mptr->name );
			if ( mptr->mandatory )
				strcat( buffer,"{required}" );
			if ( mptr->immutable )
				strcat( buffer,"{immutable}" );
			if (!( result = join_string( result, buffer )))
				return( result );
			term=" ";
		}
		strcpy( buffer, "\";" );
		if (!( result = join_string( result, buffer )))
			return( result );
	}
	if ( optr->firstact )
	{
		strcpy( buffer, " actions");
		if (!( result = join_string( result, buffer )))
			return( result );
		term = "=\"";
		for (	fptr = optr->firstact;
			fptr != (struct occi_action *) 0;
			fptr = fptr->next )
		{
			strcpy( buffer, term );
			strcat( buffer, optr->scheme );
			strcat( buffer, optr->id );
			for ( 	i=0;
				buffer[i] != 0;
				i++ )
			{
				if ( buffer[i] == '#' )
					buffer[i] = '/';
			}
			strcat( buffer, "/action#" );
			strcat( buffer, fptr->name );
			if (!( result = join_string( result, buffer )))
				return( result );
			term=" ";
		}
		strcpy( buffer, "\";" );
		if (!( result = join_string( result, buffer )))
			return( result );
	}

	strcpy( buffer, " location=\"" );
	strcat( buffer, optr->location );
	strcat( buffer, "\";" );

	return( join_string( result, buffer ));

}

/*	---------------------------------------------------	*/
/*		o c c i _ h t t p _ c a t e g o r y  		*/
/*	---------------------------------------------------	*/
public	char *	occi_http_category( struct occi_category * optr )
{
	char	buffer[8192];
	char  *	term;
	sprintf(buffer,"%s; scheme=\"%s\"; class=%s; rel=\"%s\";",
		optr->id,optr->scheme,optr->class,
		( rest_valid_string( optr->rel ) ? optr->rel : "") );

	return( allocate_string( buffer ) );
}

/*	---------------------------------------------------	*/
/*		o c c i _ h t t p _ l i n k			*/
/*	---------------------------------------------------	*/
public	char *	occi_http_link( struct occi_category * optr, char * target, char * id )
{
	char	buffer[8192];
	char *	rptr;
	char *	mptr;
	char *	wptr;
	char *	sptr;
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

		/* generate the link content */
		/* ------------------------- */
		sprintf(buffer,"<%s>; rel=\"%s%s\"; self=\"%s\"; category=\"%s\";",
			target, "http://scheme.compatibleone.fr/occi/compatible#",sptr,
			id, optr->scheme );
		liberate( mptr );
		liberate( uptr );
		return( allocate_string( buffer ) );
	}
}

/*	---------------------------------------------------	*/
/*		o c c i _ h t t p _ a t t r i b u t e 		*/
/*	---------------------------------------------------	*/
public	char *	occi_http_attribute( struct occi_category * optr, struct occi_attribute * aptr )
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
	strcat( buffer, "=" );
	return( allocate_string( buffer ) );
}

/*	---------------------------------------------------	*/
/* 	 o c c i _ h t t p _ a t t r i b u t e _ v a l u e	*/
/*	---------------------------------------------------	*/
public	char *	occi_http_attribute_value( struct occi_category * optr, struct occi_attribute * aptr, char * vptr )
{
	char	buffer[8192];
	char	* rptr;
	if ( optr->domain )
	{
		strcpy( buffer, optr->domain );
		strcat( buffer, "." );
		strcat( buffer, optr->id   );
		strcat( buffer, "." );
		strcat( buffer, aptr->name );
	}
	else 	strcpy( buffer, aptr->name );
	if (!( rptr = allocate( strlen(buffer) + strlen( vptr ) + 4 ) ))
		return( rptr );
	else
	{
		sprintf(rptr,"%s=%c%s%c",buffer,0x0022,vptr,0x0022);
		return( rptr );
	}
}

#endif	/* _occihttp_c */
	/* ----------- */


