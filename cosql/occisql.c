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

private	struct	occi_database Database = 
	{
	0,
	(MYSQL*) 0,
	(char *) 0,
	(char *) 0, 
	(char *) 0, 
	(char *) 0,
	(struct occi_table *) 0,
	(struct occi_table *) 0
	};

/*	-------------------------------------------	*/
/*	o c c i _ d a t a b a s e _ f a i l u r e 	*/
/*	-------------------------------------------	*/
private	int	occi_database_failure()
{
	printf("*** mysql database failure: error: %s ***\n",
	       	mysql_error(Database.handle));
	return(0);
}

/*	-------------------------------------------	*/
/*	o c c i _ s q l _ f a i l u r e 		*/
/*	-------------------------------------------	*/
private	int	occi_sql_failure( struct occi_table * tptr, int result )
{
	printf("*** mysql table failure: error: %s ***\n",
	       	mysql_error(tptr->handle));
	return( result );
}

/*	-----------------------------		*/
/*	d e b u g _ s q l _ q u e r y 		*/
/*	-----------------------------		*/
private	void	debug_sql_query(struct occi_table * tptr, char * xptr )
{
	if ( check_verbose() )
	{
		printf("Occi MySQL Query: %s: %s\n",tptr->name, xptr);
	}
	return;
}
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
		else	return( tptr );
	}
}

/*	-----------------------------------------	*/
/*	l o c a t e _ o c c i _ s q l _ t a b l e  	*/
/*	-----------------------------------------	*/
private	struct occi_table * locate_occi_sql_table( char * tablename )
{
	struct	occi_table * tptr;
	for (	tptr=Database.FirstTable;
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
		if (!( tptr->previous = Database.LastTable ))
			Database.FirstTable = tptr;
		else	tptr->previous->next = tptr;
		Database.LastTable = tptr;
		return( tptr );
	}
}

/*	-------------------------------------------	*/
/*	  i n i t i a l i s e _ o c c i _ s q l 	*/
/*	-------------------------------------------	*/
public	int	initialise_occi_sql( char * hostname, char * basename, char * username, char * password )
{
	char * eptr;
	if (!( Database.status ))
	{
		if (( eptr = getenv("OCCISQLHOST")) != (char *) 0)
			hostname = eptr;

		if (( eptr = getenv("OCCISQLBASE")) != (char *) 0)
			basename = eptr;

		if (( eptr = getenv("OCCISQLUSER")) != (char *) 0)
			username = eptr;

		if (( eptr = getenv("OCCISQLPASSWD")) != (char *) 0)
			password = eptr;

		if ( hostname )
			if (!( strcmp( hostname , "storage" ) ))
				hostname =  "localhost";

		if (!( Database.hostname = allocate_string( hostname ) ))
			return( 27 );
		else if (!( Database.basename = allocate_string( basename ) ))
			return( 27 );
		else if (!( Database.username = allocate_string( username ) ))
			return( 27 );
		else if (!( Database.password = allocate_string( password ) ))
			return( 27 );
		Database.status = 1;
	}
	return( 0 );
}

/*	-------------------------------------------	*/
/*	    t e r m i n a t e _ o c c i _ s q l		*/
/*	-------------------------------------------	*/
public	void	terminate_occi_sql()
{
	if ( Database.status )
	{
		Database.status = 0;
		if ( Database.hostname )	
			Database.hostname = liberate( Database.hostname );
		if ( Database.basename )	
			Database.basename = liberate( Database.basename );
		if ( Database.username )
			Database.username = liberate( Database.username );
		if ( Database.password )
			Database.password = liberate( Database.password );
	}
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
		debug_sql_query( tptr, xptr );
		if ((tptr->status = mysql_query( tptr->handle, xptr )) != 0)
			occi_sql_failure( tptr, 46 );

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
		debug_sql_query( tptr, xptr );
		if ((tptr->status = mysql_query( tptr->handle, xptr )) != 0)
			occi_sql_failure( tptr, 46 );
		else if ((tptr->status = mysql_select_db( tptr->handle, tptr->base )) != 0)
			occi_sql_failure( tptr, 40 );
		liberate( xptr );
		return( ( tptr->status ? 46 : 0 ) );
	}
}

/*	-------------------------------------------------	*/
/*	    d e f a u l t _ o c c i _ s q l _ h o s t  		*/
/*	-------------------------------------------------	*/
private	int	default_occi_sql_host( struct occi_table * tptr )
{
	if (!( Database.status ))
		initialise_occi_sql( "storage", "accords_database", "root", "root" );

	if (!( tptr->host ))
		if (!( tptr->host = allocate_string( Database.hostname ) ))
			return(0);
	if (!( tptr->user ))
		if (!( tptr->user = allocate_string( Database.username ) ))
			return(0);
	if (!( tptr->pass ))
		if (!( tptr->pass = allocate_string( Database.password ) ))
			return(0);
	if (!( tptr->base ))
		if (!( tptr->base = allocate_string( Database.basename ) ))
			return(0);
	return(1);
}

/*	-------------------------------------------------	*/
/*	i n i t i a l i s e _ o c c i _ s q l _ t a b l e  	*/
/*	-------------------------------------------------	*/
public	struct	occi_table * initialise_occi_sql_table( char * tablename, char * description )
{
	MYSQL		   * rptr;
	struct	occi_table * tptr;

	/* --------------------------- */
	/* retrieve the table if known */
	/* --------------------------- */
	if (!( tptr = locate_occi_sql_table( tablename )))
		if (!( tptr = new_occi_sql_table( tablename, description ) ))
			return( tptr );

	/* --------------------------- */
	/* set the database table info */
	/* --------------------------- */
	if ((!( tptr->host ))
	||  (!( tptr->base ))
	||  (!( tptr->user ))
	||  (!( tptr->pass )))
		if (!( default_occi_sql_host( tptr ) ))
			return( tptr );

	/* -------------------------------- */
	/* validate the database connection */
	/* -------------------------------- */
	while (!( tptr->handle = Database.handle ))
	{
		if (!( Database.handle = mysql_init((MYSQL *) 0) ))
			return( tptr );
		else if (!( tptr->handle = mysql_real_connect( Database.handle, tptr->host, tptr->user, tptr->pass, (char *) 0, 0, 0, 0 ) ))
		{
			occi_database_failure();
			return( tptr );
		}
		else if ( create_occi_sql_database( tptr ) != 0 )
		{
			occi_sql_failure( tptr, 55 );
			return( tptr );
		}
	}

	/* -------------------------------- */
	/* validate the datatable existance */
	/* -------------------------------- */
	if ( create_occi_sql_table( tptr ) != 0 )
		return( tptr );
	else	return( tptr );
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
			if (!(Database.FirstTable = tptr->next))
				Database.LastTable = (struct occi_table *) 0;
			else	Database.FirstTable->previous = (struct occi_table *) 0;
		}
		else
		{
			if (!( tptr->previous->next = tptr->next ))
				Database.LastTable = tptr->previous;
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
	else	return( (*expression->handler)( expression->context, nptr, vptr, length ) );
}

/*	-------------------------------------------	*/
/*	       o c c i _ s q l _ f i e l d  		*/
/*	-------------------------------------------	*/
/*	this function will transfer back the field	*/
/*	data to the caller for storage or processing	*/
/*	-------------------------------------------	*/
private	int	occi_sql_list_item( struct occi_expression *expression, char * nptr, char * vptr, int length )
{
	if (!( expression ))
		return(0);
	else if (!( expression->handler ))
		return(0);
	else	return( (*expression->handler)(expression->context, nptr, vptr, length ) );
}

/*	-------------------------------------------	*/
/*	       o c c i _ s q l _ r e c o r d 		*/
/*	-------------------------------------------	*/
/*	parses a result set and delivers field data	*/
/*	-------------------------------------------	*/
private	int	occi_sql_record( struct	occi_table * tptr,  struct occi_expression *expression )
{
	MYSQL_RES *	result;
	MYSQL_ROW 	row;
	MYSQL_FIELD *	field;
	unsigned long *	lengths;
	unsigned int	fieldcount;
	unsigned int	i;

	if (!( result = mysql_use_result( tptr->handle )))
		return(118);

	else if (!( fieldcount = mysql_num_fields(result) ))
	{
		mysql_free_result( result );
		return(119);
	}
	else
	{
		while ((row = mysql_fetch_row(result)))
		{
			if (!( lengths = mysql_fetch_lengths(result) ))
				continue;
			else
			{
				mysql_field_seek( result, 0 );
				for(	i=0; 
					i < fieldcount; i++)
				{
					if (!( field = mysql_fetch_field( result )))
						break;
					else	occi_sql_field( expression, field->name, row[i], lengths[i] );				
				}
			}
		}
		mysql_free_result( result );
		return(0);
	}
}

/*	-------------------------------------------	*/
/*	       o c c i _ s q l _ r e c o r d s 		*/
/*	-------------------------------------------	*/
/*	parses a result set and delivers record set	*/
/*	-------------------------------------------	*/
private	int	occi_sql_records( struct occi_table * tptr,  struct occi_expression *expression )
{
	MYSQL_RES *	result;
	MYSQL_ROW 	row;
	MYSQL_FIELD *	field;
	unsigned long *	lengths;
	unsigned int	fieldcount;
	unsigned int	i;

	if (!( result = mysql_use_result( tptr->handle )))
		return(118);

	else if (!( fieldcount = mysql_num_fields(result) ))
	{
		mysql_free_result( result );
		return(119);
	}
	else
	{
		while ((row = mysql_fetch_row(result)))
		{
			if (!( lengths = mysql_fetch_lengths(result) ))
				continue;
			else
			{
				mysql_field_seek( result, 0 );
				for(	i=0; 
					i < fieldcount; i++)
				{
					if (!( field = mysql_fetch_field( result )))
						break;
					else	occi_sql_list_item( expression, field->name, row[i], lengths[i] );				
				}
			}
		}
		mysql_free_result( result );
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
	else if (!( expression->value = allocate( strlen( tptr->name ) + strlen( _SELECT_MIN_ID_FROM ) + 16 ) ))
		return( 27 );
	else	sprintf(expression->value,"%s %s",_SELECT_MIN_ID_FROM,tptr->name);

	debug_sql_query( tptr, expression->value );

	if ( mysql_query( tptr->handle, expression->value ) != 0)
		return( occi_sql_failure( tptr, 64 ) );
	else	return( occi_sql_record( tptr, expression ) );
}

/*	------------------------------------------------	*/
/*	p r e v i o u s _ o c c i _ s q l _ r e c o r d 	*/
/*	------------------------------------------------	*/
public	int	previous_occi_sql_record( char * category,  struct occi_expression *expression )
{
	char *	xptr;
	struct	occi_table * tptr;
	if (!( tptr = locate_occi_sql_table( category ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else if (!( tptr->handle ))
		return( 50 );
	else if (!( xptr = allocate( 
				strlen( ( expression->value ? expression->value : "" ) ) + 
				strlen( tptr->name ) + 
				strlen( _SELECT_ID_FROM ) + 
				strlen( _SINGLE_ROW ) + 16 ) ))
		return( 27 );
	else	sprintf( xptr,"%s %s %s %s",_SELECT_ID_FROM,tptr->name,( expression->value ? expression->value : "" ),_SINGLE_ROW);
	debug_sql_query( tptr, xptr );
	if ( mysql_query( tptr->handle, xptr ) != 0)
		return( occi_sql_failure( tptr, 64 ) );
	else	return( occi_sql_record( tptr, expression ) );
}

/*	-------------------------------------------	*/
/*	i n s e r t _ o c c i _ s q l _ r e c o r d 	*/
/*	-------------------------------------------	*/
public	int	insert_occi_sql_record( char * category,  struct occi_expression *expression )
{
	int	status;
	char *	xptr=(char *) 0;
	struct	occi_table * tptr;
	if (!( expression ))
		return( 118 );
	else if (!( expression->value ))
		return( 118 );
	else if (!( tptr = locate_occi_sql_table( category ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else if (!( xptr = allocate( strlen( expression->value ) + strlen( tptr->name ) + strlen( _INSERT_INTO ) + 16 ) ))
		return( 27 );
	else	sprintf( xptr,"%s %s %s",_INSERT_INTO,tptr->name,expression->value);
	debug_sql_query( tptr, xptr );
	status = mysql_query( tptr->handle, xptr );
	liberate( xptr );
	if ( status )
		return( occi_sql_failure( tptr, 81 ) );
	else	return( 0 );
}

/*	-------------------------------------------	*/
/*	s e a r c h _ o c c i _ s q l _ r e c o r d 	*/
/*	-------------------------------------------	*/
public	int	search_occi_sql_record( char * category,  struct occi_expression *expression )
{
	int	status;
	char *	xptr=(char *) 0;
	struct	occi_table * tptr;
	if (!( expression ))
		return( 118 );
	else if (!( expression->value ))
		return( 118 );
	else if (!( tptr = locate_occi_sql_table( category ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else if (!( xptr = allocate( strlen( expression->value ) + strlen( tptr->name ) + strlen( _SELECT_ALL_FROM ) + 16 ) ))
		return( 27 );
	else	sprintf( xptr,"%s %s %s",_SELECT_ALL_FROM,tptr->name,expression->value);
	debug_sql_query( tptr, xptr );
	status = mysql_query( tptr->handle, xptr );
	liberate( xptr );
	if ( status )
		return( occi_sql_failure( tptr, 78 ) );
	else	return( occi_sql_record( tptr, expression ) );
}

/*	-------------------------------------------	*/
/*	s e a r c h _ o c c i _ s q l _ r e c o r d 	*/
/*	-------------------------------------------	*/
public	int	collect_occi_sql_records( char * category,  struct occi_expression *expression )
{
	int	status;
	char *	xptr=(char *) 0;
	struct	occi_table * tptr;
	if (!( expression ))
		return( 118 );
	else if (!( tptr = locate_occi_sql_table( category ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else if (!( xptr = allocate( strlen( ( expression->value ? expression->value : "" ) ) + strlen( tptr->name ) + strlen( _SELECT_ALL_FROM ) + 16 ) ))
		return( 27 );
	else	sprintf( xptr,"%s %s %s",_SELECT_ID_FROM,tptr->name,( expression->value ? expression->value : "" ));
	debug_sql_query( tptr, xptr );
	status = mysql_query( tptr->handle, xptr );
	liberate( xptr );
	if ( status )
		return( occi_sql_failure( tptr, 78 ) );
	else	return( occi_sql_records( tptr, expression ) );
}

/*	-------------------------------------------	*/
/*	u p d a t e _ o c c i _ s q l _ r e c o r d 	*/
/*	-------------------------------------------	*/
public	int	update_occi_sql_record( char * category,  struct occi_expression *expression )
{
	int	status;
	char *	xptr=(char *) 0;
	struct	occi_table * tptr;
	if (!( expression ))
		return( 118 );
	else if (!( expression->value ))
		return( 118 );
	else if (!( tptr = locate_occi_sql_table( category ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else if (!( xptr = allocate( strlen( expression->value ) + strlen( tptr->name ) + strlen( _UPDATE_WHERE ) + 16 ) ))
		return( 27 );
	else	sprintf( xptr,"%s %s %s",_UPDATE_WHERE,tptr->name,expression->value);
	debug_sql_query( tptr, xptr );
	status = mysql_query( tptr->handle, xptr );
	liberate( xptr );
	if ( status )
		return( occi_sql_failure( tptr, 78 ) );
	else	return( 0 );
}

/*	-------------------------------------------	*/
/*	d e l e t e _ o c c i _ s q l _ r e c o r d 	*/
/*	-------------------------------------------	*/
public	int	delete_occi_sql_record( char * category,  struct occi_expression *expression )
{
	int	status;
	char *	xptr=(char *) 0;
	struct	occi_table * tptr;
	if (!( expression ))
		return( 118 );
	else if (!( expression->value ))
		return( 118 );
	else if (!( tptr = locate_occi_sql_table( category ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else if (!( xptr = allocate( strlen( expression->value ) + strlen( tptr->name ) + strlen( _DELETE_FROM ) + 16 ) ))
		return( 27 );
	else	sprintf( xptr,"%s %s %s",_DELETE_FROM,tptr->name,expression->value);
	debug_sql_query( tptr, xptr );
	status = mysql_query( tptr->handle, xptr );
	liberate( xptr );
	if ( status )
		return( occi_sql_failure( tptr, 78 ) );
	else	return( 0 );
}

/*	-------------------------------------------	*/
/*	  n e x t _ o c c i _ s q l _ r e c o r d 	*/
/*	-------------------------------------------	*/
public	int	next_occi_sql_record( char * category,  struct occi_expression *expression )
{
	char *	xptr;
	struct	occi_table * tptr;
	if (!( tptr = locate_occi_sql_table( category ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else if (!( xptr = allocate( 
				strlen( ( expression->value ? expression->value : "" ) ) + 
				strlen( tptr->name ) + 
				strlen( _SELECT_ID_FROM ) + 
				strlen( _SINGLE_ROW ) + 16 ) ))
		return( 27 );
	else	sprintf( xptr,"%s %s %s %s",_SELECT_ID_FROM,tptr->name,( expression->value ? expression->value : "" ),_SINGLE_ROW);
	debug_sql_query( tptr, xptr );
	if ( mysql_query( tptr->handle, xptr ) != 0)
		return( occi_sql_failure( tptr, 48 ) );
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
	else if (!( expression->value = allocate( strlen( tptr->name ) + strlen( _SELECT_MIN_ID_FROM ) + 16 ) ))
		return( 27 );
	else	sprintf(expression->value,"%s %s",_SELECT_MAX_ID_FROM,tptr->name);
	debug_sql_query( tptr, expression->value );
	if ( mysql_query( tptr->handle, expression->value ) != 0)
		return( occi_sql_failure( tptr, 48 ) );
	else	return( occi_sql_record( tptr, expression ) );
}


	/* ----------- */
#endif	/* _occi_sql_c */
	/* ----------- */



