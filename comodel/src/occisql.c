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

#ifndef	_occi_sql_c
#define	_occi_sql_c

#include "occisql.h"

private	struct	occi_table * OcciTableFirst=(struct occi_table *) 0;
private	struct	occi_table * OcciTableLast=(struct occi_table *) 0;

/*	-------------------------------------------	*/
/*	   l i b e r a t e _ o c c i _ t a b l e	*/
/*	-------------------------------------------	*/
private	struct	occi_table *	liberate_occi_table( struct occi_table * tptr )
{
	if ( tptr )
	{
		if ( tptr->name )
			tptr->name = liberate(tptr->name );

		if ( tptr->handle )
		{
			mysql_close( tptr->handle );
			tptr->handle = (MYSQL *) 0;
		}
		tptr = liberate( tptr );
	}
	return((struct occi_table *) 0);
}

/*	-------------------------------------------	*/
/*	   a l l o c a t e _ o c c i _ t a b l e	*/
/*	-------------------------------------------	*/
private	struct	occi_table *	allocate_occi_table( char * catname )
{
	struct	occi_table * tptr;
	if (!( tptr = (struct occi_table *) allocate( sizeof( struct occi_table ) ) ))
		return( tptr );
	else
	{
		memset( tptr, 0, sizeof( struct occi_table ) );
		if (!( tptr->name = allocate_string( catname ) ))
			return( liberate_occi_table( tptr ) );
		else if (!( tptr->handle = mysql_init( (MYSQL *) 0 ) ))
			return( liberate_occi_table( tptr ) );
		else	return( tptr );
	}
}

/*	-----------------------------------------	*/
/*	l o c a t e _ o c c i _ s q l _ t a b l e  	*/
/*	-----------------------------------------	*/
private	struct occi_table * locate_occi_sql_table( char * tablename )
{
	struct	occi_table * tptr;
	for (	tptr=OcciTableFirst;
		tptr != (struct occi_table *) 0;
		tptr = tptr->next )
	{
		if (!( tptr->name ))
			continue;
		else if (!( strcmp( tptr->name, tablename ) ))
			break;
	}
	return( tptr );
}

/*	-----------------------------------------	*/
/*	    n e w _ o c c i _ s q l _ t a b l e  	*/
/*	-----------------------------------------	*/
private	struct occi_table * new_occi_sql_table( char * tablename, char * description )
{
	struct	occi_table * tptr;

	if (!( tptr = allocate_occi_table( tablename ) ))
		return( tptr );
	else if (!( tptr->description = allocate_string( description )))
		return( liberate_occi_table( tptr ) );
	{
		if (!( tptr->previous = OcciTableLast ))
			OcciTableFirst = tptr;
		else	tptr->previous->next = tptr;
		OcciTableLast = tptr;
		return( tptr );
	}
}

/*	-------------------------------------------	*/
/*	  i n i t i a l i s e _ o c c i _ s q l 	*/
/*	-------------------------------------------	*/
public	int	initialise_occi_sql( char * databasename, char * username, char * password )
{
	return( 0 );
}

/*	-------------------------------------------	*/
/*	    t e r m i n a t e _ o c c i _ s q l		*/
/*	-------------------------------------------	*/
public	void	terminate_occi_sql()
{
	return;
}

/*	-------------------------------------------------	*/
/*	    c r e a t e _ o c c i _ s q l _ t a b l e  		*/
/*	-------------------------------------------------	*/
private	int	create_occi_sql_table( struct occi_table * tptr )
{
	char *	xptr;
	if (!( xptr = allocate( 
			strlen( _CREATE_TABLE ) + 
			strlen( tptr->name ) + 
			strlen( tptr->description ) + 16 ) ))
		return( 27 );
	else 
	{
		sprintf(xptr,"%s %s %s", _CREATE_TABLE, tptr->name, tptr->description );
		tptr->status = mysql_query( tptr->handle, xptr );
		liberate( xptr );
		return( ( tptr->status ? 46 : 0 ) );
	}
}
/*	-------------------------------------------------	*/
/*	 c r e a t e _ o c c i _ s q l _ d a t a b a s e  	*/
/*	-------------------------------------------------	*/
private	int	create_occi_sql_database( struct occi_table * tptr )
{
	char *	xptr;
	if (!( xptr = allocate( 
			strlen( _CREATE_DATABASE  ) + 
			strlen( tptr->base ) + 16 ) ))
		return( 27 );
	else 
	{
		sprintf(xptr,"%s %s ", _CREATE_DATABASE, tptr->base );
		tptr->status = mysql_query( tptr->handle, xptr );
		liberate( xptr );
		return( ( tptr->status ? 46 : 0 ) );
	}
}

/*	-------------------------------------------------	*/
/*	    d e f a u l t _ o c c i _ s q l _ h o s t  		*/
/*	-------------------------------------------------	*/
private	int	default_occi_sql_host( struct occi_table * tptr )
{
	if (!( tptr->host ))
		if (!( tptr->host = allocate_string( _DEFAULT_HOST ) ))
			return(0);
	if (!( tptr->user ))
		if (!( tptr->user = allocate_string( _DEFAULT_USER ) ))
			return(0);
	if (!( tptr->pass ))
		if (!( tptr->pass = allocate_string( _DEFAULT_PASS ) ))
			return(0);
	if (!( tptr->base ))
		if (!( tptr->base = allocate_string( _DEFAULT_BASE ) ))
			return(0);
	return(1);
}

/*	-------------------------------------------------	*/
/*	i n i t i a l i s e _ o c c i _ s q l _ t a b l e  	*/
/*	-------------------------------------------------	*/
public	int	initialise_occi_sql_table( char * tablename, char * description )
{
	MYSQL		   * rptr;
	struct	occi_table * tptr;

	if (!( tptr = locate_occi_sql_table( tablename )))
		if (!( tptr = new_occi_sql_table( tablename, description ) ))
			return( 27 );

	if ((!( tptr->host ))
	||  (!( tptr->user ))
	||  (!( tptr->pass )))
		if (!( default_occi_sql_host( tptr ) ))
			return( 27 );

	if (!( rptr = mysql_real_connect( tptr->handle, tptr->host, tptr->user, tptr->pass, tptr->base, 0, 0, 0 ) ))
		return( 50 );
	else  if ( create_occi_sql_database( tptr ) != 0 )
		return( tptr->status );
	else if ( create_occi_sql_table( tptr ) != 0 )
		return( tptr->status );
	else	return( 0 );
}

/*	-------------------------------------------	*/
/*	   d r o p _ o c c i _ s q l _ t a b l e 	*/
/*	-------------------------------------------	*/
public	int	drop_occi_sql_table( char * tablename )
{
	struct	occi_table * tptr;
	if (!( tptr = locate_occi_sql_table( tablename ) ))
		return( 40 );
	else
	{
		if (!( tptr->previous ))
		{
			if (!(OcciTableFirst = tptr->next))
				OcciTableLast = (struct occi_table *) 0;
			else	OcciTableFirst->previous = (struct occi_table *) 0;
		}
		else
		{
			if (!( tptr->previous->next = tptr->next ))
				OcciTableLast = tptr->previous;
			else 	tptr->next->previous = tptr->previous;
		}
		tptr = liberate_occi_table( tptr );
	}
	return( 0 );
}

/*	-------------------------------------------	*/
/*	       o c c i _ s q l _ f i e l d  		*/
/*	-------------------------------------------	*/
/*	this function will transfer back the field	*/
/*	data to the caller for storage or processing	*/
/*	-------------------------------------------	*/
private	int	occi_sql_field( struct occi_expression *expression, char * nptr, char * vptr, int length )
{
	if (!( expression ))
		return(0);
	else if (!( expression->handler ))
		return(0);
	else	return( (*expression->handler)( nptr, vptr, length ) );
}

/*	-------------------------------------------	*/
/*	       o c c i _ s q l _ r e c o r d 		*/
/*	-------------------------------------------	*/
/*	parses a result set and delivers field data	*/
/*	-------------------------------------------	*/
private	int	occi_sql_record( struct	occi_table * tptr,  struct occi_expression *expression )
{
	MYSQL_RES *	rptr;
	MYSQL_ROW	row;
	MYSQL_FIELD *	fields;
	unsigned long *	lengths;
	unsigned int	fieldcount;
	unsigned int	i;

	if (!( rptr = mysql_use_result( tptr->handle )))
		return(118);

	else if (!( fieldcount = mysql_num_fields(rptr) ))
	{
		mysql_free_result( rptr );
		return(119);
	}
	else if (!( fields = mysql_fetch_fields(rptr) ))
	{
		mysql_free_result( rptr );
		return(119);
	}
	else
	{
		while ((row = mysql_fetch_row(rptr)))
		{
			if (!( lengths = mysql_fetch_lengths(rptr) ))
				continue;
			else
			{
				for(	i=0; 
					i < fieldcount; i++)
					occi_sql_field( expression, fields[i].name, row[i], lengths[i] );				
			}
		}
		mysql_free_result( rptr );
		return(0);
	}
}

/*	-------------------------------------------	*/
/*	 f i r s t _ o c c i _ s q l _ r e c o r d 	*/
/*	-------------------------------------------	*/
public	int	first_occi_sql_record( char * category,  struct occi_expression *expression )
{
	struct	occi_table * tptr;
	if (!( tptr = locate_occi_sql_table( category ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else if ( mysql_query( tptr->handle, expression->value ) != 0)
		return( 64 );
	else	return( occi_sql_record( tptr, expression ) );
}

/*	------------------------------------------------	*/
/*	p r e v i o u s _ o c c i _ s q l _ r e c o r d 	*/
/*	------------------------------------------------	*/
public	int	previous_occi_sql_record( char * category,  struct occi_expression *expression )
{
	struct	occi_table * tptr;
	if (!( tptr = locate_occi_sql_table( category ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else if (!( tptr->handle ))
		return( 50 );
	else if ( mysql_query( tptr->handle, expression->value ) != 0)
		return( 64 );
	else	return( occi_sql_record( tptr, expression ) );
}

/*	-------------------------------------------	*/
/*	i n s e r t _ o c c i _ s q l _ r e c o r d 	*/
/*	-------------------------------------------	*/
public	int	insert_occi_sql_record( char * category,  struct occi_expression *expression )
{
	struct	occi_table * tptr;
	if (!( tptr = locate_occi_sql_table( category ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else if ( mysql_query( tptr->handle, expression->value ) != 0)
		return( 81 );
	else	return( 0 );
}

/*	-------------------------------------------	*/
/*	s e a r c h _ o c c i _ s q l _ r e c o r d 	*/
/*	-------------------------------------------	*/
public	int	search_occi_sql_record( char * category,  struct occi_expression *expression )
{
	struct	occi_table * tptr;
	if (!( tptr = locate_occi_sql_table( category ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else if ( mysql_query( tptr->handle, expression->value ) != 0)
		return( 78 );
	else	return( occi_sql_record( tptr, expression ) );
}

/*	-------------------------------------------	*/
/*	u p d a t e _ o c c i _ s q l _ r e c o r d 	*/
/*	-------------------------------------------	*/
public	int	update_occi_sql_record( char * category,  struct occi_expression *expression )
{
	struct	occi_table * tptr;
	if (!( tptr = locate_occi_sql_table( category ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else if ( mysql_query( tptr->handle, expression->value ) != 0)
		return( 78 );
	else	return( 0 );
}

/*	-------------------------------------------	*/
/*	d e l e t e _ o c c i _ s q l _ r e c o r d 	*/
/*	-------------------------------------------	*/
public	int	delete_occi_sql_record( char * category,  struct occi_expression *expression )
{
	struct	occi_table * tptr;
	if (!( tptr = locate_occi_sql_table( category ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else if ( mysql_query( tptr->handle, expression->value ) != 0)
		return( 78 );
	else	return( 0 );
}

/*	-------------------------------------------	*/
/*	  n e x t _ o c c i _ s q l _ r e c o r d 	*/
/*	-------------------------------------------	*/
public	int	next_occi_sql_record( char * category,  struct occi_expression *expression )
{
	struct	occi_table * tptr;
	if (!( tptr = locate_occi_sql_table( category ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else if ( mysql_query( tptr->handle, expression->value ) != 0)
		return( 48 );
	else	return( occi_sql_record( tptr, expression ) );
}

/*	-------------------------------------------	*/
/*	  l a s t _ o c c i _ s q l _ r e c o r d 	*/
/*	-------------------------------------------	*/
public	int	last_occi_sql_record( char * category,  struct occi_expression *expression )
{
	struct	occi_table * tptr;
	if (!( tptr = locate_occi_sql_table( category ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else if ( mysql_query( tptr->handle, expression->value ) != 0)
		return( 48 );
	else	return( occi_sql_record( tptr, expression ) );
}


	/* ----------- */
#endif	/* _occi_sql_c */
	/* ----------- */



