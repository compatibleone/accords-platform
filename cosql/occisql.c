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
	"UNKNOWN",
	(void *) 0,
	0,
	(char *) 0,
	(char *) 0, 
	(char *) 0, 
	(char *) 0,
	(struct occi_table *) 0,
	(struct occi_table *) 0
	};

private pthread_mutex_t database_lock=PTHREAD_MUTEX_INITIALIZER;
private	struct	occi_database_thread * threadheap=(struct occi_database_thread*) 0;

/*	-------------------------------------------	*/
/*		o c c i _ s q l _ l o c k		*/
/*	-------------------------------------------	*/
private	void	occi_sql_lock()
{
	rest_log_message("OCCI-SQL LOCK DATABASE");
	pthread_mutex_lock( &database_lock );
	return;
}

/*	-------------------------------------------	*/
/*		o c c i _ s q l _ l o c k		*/
/*	-------------------------------------------	*/
private	void	occi_sql_unlock()
{
	rest_log_message("OCCI-SQL UNLOCK DATABASE");
	pthread_mutex_unlock( &database_lock );
	return;
}

/*	---------------------------		*/
/*	o c c i _ s q l _ e r r o r 	*/
/*	---------------------------		*/
private	char * occi_sql_error( void * h )
{
#ifdef	_OCCI_POSTGRE
	PGconn * pqptr;
#endif
	switch ( Database.status )
	{

#ifdef	_OCCI_MYSQL
	case	_OCCI_MYSQL		:
		return( (char *) mysql_error( (MYSQL *) h ) );
#endif

#ifdef	_OCCI_POSTGRE
	case	_OCCI_POSTGRE	:
			if (!(pqptr = (PGconn *) h))
				return( "incorrect handle" );
			else	return( PQerrorMessage( pqptr ) );
#endif

	default					:
		return( "unknown database type" );
	}
}

/*	-------------------------------------------	*/
/*	o c c i _ d a t a b a s e _ f a i l u r e 	*/
/*	-------------------------------------------	*/
private	int	occi_database_failure(char * msg)
{
	char 	buffer[4096];
	sprintf(buffer,"%s: DATABASE FAILURE(%s): ERROR: %s ***\n",
		Database.nature,msg, occi_sql_error(Database.handle));
	rest_log_message( buffer );
	return(0);
}

/*	-------------------------------------------	*/
/*	o c c i _ d a t a b a s e _ l o g w r i t E	*/
/*	-------------------------------------------	*/
private	int	occi_database_logwrite(char * msg)
{
	char 	buffer[4096];
	sprintf(buffer,"%s-QUERY: %s",Database.nature, msg);
	rest_log_message( buffer );
	return(0);
}

/*	-------------------------------------------	*/
/*	o c c i _ s q l _ f a i l u r e 		*/
/*	-------------------------------------------	*/
private	int	occi_sql_failure( struct occi_table * tptr, int result, char * msg,int id )
{
	char 	buffer[4096];
	sprintf(buffer,"%s(%u,%s): TABLE(%s) FAILURE: %s \n",
	       	Database.nature,id,msg,tptr->name,occi_sql_error(tptr->handle));
	rest_log_message( buffer );
	return( result );
}

/*	-----------------------------		*/
/*	d e b u g _ s q l _ q u e r y 		*/
/*	-----------------------------		*/
private	void	debug_sql_query(struct occi_table * tptr, char * xptr )
{
	if ( check_verbose() )
	{
		printf("Occi %s Query: %s: %s\n",Database.nature,tptr->name, xptr);
	}
	return;
}

/*	-------------------------------------------------	*/
/*	o c c i _ d a t a b a s e _ t h r e a d _ i n i t	*/
/*	-------------------------------------------------	*/
/*	this is pure MYSQL stuff.							*/
/*	-------------------------------------------------	*/
private	void	occi_database_thread_init()
{
	char	buffer[1024];
	struct	occi_database_thread * tptr;
	pthread_t	tid;
	tid = pthread_self();
	for ( 	tptr=threadheap;
		tptr != (struct occi_database_thread *) 0;
		tptr = tptr->next )
		if ( tptr->tid == tid )
			return;
	sprintf(buffer,"%s THREAD INIT: %u\n",Database.nature,tid);	
	rest_log_message( buffer );
	if (!( tptr = allocate( sizeof( struct occi_database_thread ) ) ))
		return;
	else
	{
		tptr->tid = tid;
		mysql_thread_init();
		tptr->next = threadheap;
		threadheap = tptr;
		return;
	}
}

/*	-------------------------------		*/
/*	o c c i _ m y s q l _ q u e r y 	*/
/*	-------------------------------		*/
private	int occi_sql_query(int id, struct occi_table * tptr, char * xptr )
{
	int	status;

	debug_sql_query( tptr, xptr );
	occi_database_logwrite( xptr );

	switch ( Database.status )
	{

#ifdef	_OCCI_MYSQL
	/* ----------- */
	/* MYSQL Query */
	/* ----------- */
	case	_OCCI_MYSQL		:
		occi_database_thread_init();
		if ((status = mysql_ping(tptr->handle)) != 0)
			return( occi_sql_failure( tptr, status, "mysql_ping", id ) );
		if ((status = mysql_query(tptr->handle, xptr)) != 0)
			return( occi_sql_failure( tptr, status, "mysql_query", id ) );
		else	return( status );
#endif

#ifdef	_OCCI_POSTGRE
	/* ----------------- */
	/* POSTGRE SQL Query */
	/* ----------------- */
	case	_OCCI_POSTGRE	:
		tptr->result = PQexec( tptr->handle, xptr );
		if ( PQresultStatus( tptr->result ) == PGRES_FATAL_ERROR )
			return( occi_sql_failure( tptr, status, "PQexec", id ) );
		else	return( 0 );
#endif	
	default						:
		return( occi_sql_failure( tptr, 40, "undefined database type", id ) );
	}
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
			switch ( Database.status )
			{

#ifdef	_OCCI_MYSQL
			case	_OCCI_MYSQL		:	
				mysql_close( tptr->handle );
				tptr->handle = (void *) 0;
				break;
#endif

#ifdef	_OCCI_POSTGRE
			case	_OCCI_POSTGRE		:
				tptr->handle = (void *) 0;
				break;
#endif	
			}

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
	char * nature;
	if (!( Database.status ))
	{
		if (!( nature = getenv("OCCISQLTYPE" ) ))
			nature = "MYSQL";

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
		else if (!( Database.nature = allocate_string( nature ) ))
			return( 27 );
		else if (!( Database.username = allocate_string( username ) ))
			return( 27 );
		else if (!( Database.password = allocate_string( password ) ))
			return( 27 );

		/* ------------------------------------------- */
		/* determine the database type behind the host */
		/* ------------------------------------------- */
#ifdef	_OCCI_MYSQL
		if (!( strcasecmp( Database.nature, "MYSQL" ) ))
			Database.status = _OCCI_MYSQL;
#endif
#ifdef	_OCCI_POSTGRE
		if (!( strcasecmp( Database.nature, "POSTGRESQL" ) ))
			Database.status = _OCCI_POSTGRE;
#endif

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
	char * 	command=_CREATE_TABLE_INE;
	int	expect_error=0;

	/* ------------------------------------------------- */
	/* some versions do not allow the conditional clause */
	/* ------------------------------------------------- */
	switch( Database.status )
	{
#ifdef	_OCCI_POSTGRE
	case	_OCCI_POSTGRE	:
		if ( Database.version < 90100 )
		{
			expect_error = 1;
			command = _CREATE_TABLE;
		}
		break;
#endif
	default			:
		expect_error = 0;
		command = _CREATE_TABLE_INE;
	}

	/* ----------------------- */
	/* allocate the expression */
	/* ----------------------- */
	if (!( xptr = allocate( 
			strlen( command ) + 
			strlen( tptr->name ) + 
			strlen( tptr->description ) + 16 ) ))
		return( 27 );
	else 
	{
		sprintf(xptr,"%s %s %s", command, tptr->name, tptr->description );
		tptr->status = occi_sql_query( 1,tptr, xptr );
		liberate( xptr );
		return( ( tptr->status ? ( expect_error ? 0 :46 ) : 0 ) );
	}
}
/*	-------------------------------------------------	*/
/*	 c r e a t e _ o c c i _ s q l _ d a t a b a s e  	*/
/*	-------------------------------------------------	*/
private	int	create_occi_sql_database( struct occi_table * tptr )
{
	char *	xptr;
	if (!( xptr = allocate( 
			strlen( _CREATE_DATABASE_INE  ) + 
			strlen( tptr->base ) + 16 ) ))
		return( 27 );
	else 
	{
		sprintf(xptr,"%s %s ", _CREATE_DATABASE_INE, tptr->base );
		if ((tptr->status = occi_sql_query( 2,tptr, xptr )) != 0)
		{
			liberate( xptr );
			return(46);
		}
		switch ( Database.status )
		{
#ifdef	_OCCI_MYSQL
		case	_OCCI_MYSQL		:
			if ((tptr->status = mysql_select_db( tptr->handle, tptr->base )) != 0)
			{
				liberate( xptr );
				return(40);
			}
			break;
#endif
		}
		liberate( xptr );
		return(0);
	}
}

/*	-------------------------------------------------	*/
/*	    d e f a u l t _ o c c i _ s q l _ h o s t  		*/
/*	-------------------------------------------------	*/
private	int	default_occi_sql_host( struct occi_table * tptr )
{
	if (!( Database.status ))
		initialise_occi_sql( "storage", "accords_database", "root", "root" );

	if ( tptr )
	{

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

	}

	return(1);
}

/*	-------------------------------------------------	*/
/*	i n i t i a l i s e _ o c c i _ s q l _ t a b l e  	*/
/*	-------------------------------------------------	*/
private	int	sql_initialised=0;

public	struct	occi_table * initialise_occi_sql_table( char * tablename, char * description )
{
#ifdef	_OCCI_MYSQL
	MYSQL		   * rptr;
#endif
	char 	buffer[2048];

	struct	occi_table * tptr;
	int		option=1;

	/* ------------------------------- */
	/* check and perform intialisation */
	/* ------------------------------- */
	if (!( sql_initialised ))
	{
		default_occi_sql_host((struct occi_table *) 0);
		switch ( Database.status )
		{
#ifdef	_OCCI_MYSQL	
		case	_OCCI_MYSQL	:
			my_init();
			sql_initialised=1;
			break;
#endif
#ifdef	_OCCI_POSTGRE	
		case	_OCCI_POSTGRE	:
			sql_initialised=1;
			break;
#endif
		}
	}

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
		switch ( Database.status )
		{
#ifdef	_OCCI_MYSQL
		case	_OCCI_MYSQL		:
			if (!( Database.handle = mysql_init((MYSQL *) 0) ))
			{
				occi_database_failure("initialisation");
				return( tptr );
			}
			else	occi_database_thread_init();
			if ( mysql_options( Database.handle, MYSQL_OPT_RECONNECT, (const void *)&option ) != 0)
			{
				occi_database_failure("options");
				return( tptr );
			}
			else if (!( tptr->handle = mysql_real_connect( Database.handle, tptr->host, tptr->user, tptr->pass, (char *) 0, 0, 0, CLIENT_REMEMBER_OPTIONS) ))
			{
				occi_database_failure("connect");
				return( tptr );
			}
			else 	if ( create_occi_sql_database( tptr ) != 0 )
			{
				occi_database_failure("connect");
				return( tptr );
			}
			else	break;
#endif
#ifdef	_OCCI_POSTGRE
		case	_OCCI_POSTGRE	:
		{
			char 	buffer[2048];
			int	retries=0;
			while (!( Database.handle ))
			{
				/* --------------------------------------- */
				/* attempt connection to required database */
				/* --------------------------------------- */
				sprintf(buffer,"host = %s dbname = %s user = %s password = %s",
					Database.hostname,
					Database.basename,
					Database.username,
					Database.password);
				if (!( Database.handle = PQconnectdb(buffer) ))
				{
					occi_database_failure("connect");
					return( tptr );
				}
				/* ----------------------- */
				/* check connection status */
				/* ----------------------- */
				else if ( PQstatus( Database.handle ) != CONNECTION_OK)
				{
					if ( retries++ > 0 )
					{
						occi_database_failure("connect");
						PQfinish( Database.handle );
						tptr->handle = (void *) 0;
						Database.handle = (void *) 0;
						return( tptr );
					}
					else
					{
						PQfinish( Database.handle );
						tptr->handle = (void *) 0;
						Database.handle = (void *) 0;
					}
					/* ------------------------------------ */
					/* attempt connection to temporary base */
					/* ------------------------------------ */
					sprintf(buffer,"host = %s dbname = %s user = %s password = %s",
						Database.hostname,
						Database.basename,
						Database.username,
						Database.password);
					if (!( Database.handle = PQconnectdb(buffer) ))
					{
						occi_database_failure("connect");
						return( tptr );
					}
					else if ( PQstatus( Database.handle ) != CONNECTION_OK)
					{
						occi_database_failure("connect");
						PQfinish( Database.handle );
						tptr->handle = (void *) 0;
						Database.handle = (void *) 0;
						return( tptr );
					}
					else	tptr->handle = Database.handle;

					/* ------------------------- */
					/* attempt database creation */
					/* ------------------------- */
					if ( create_occi_sql_database( tptr ) != 0 )
					{
						occi_database_failure("create");
						PQfinish( Database.handle );
						tptr->handle = (void *) 0;
						Database.handle = (void *) 0;
						return( tptr );
					}
					else
					{
						PQfinish( Database.handle );
						tptr->handle = (void *) 0;
						Database.handle = (void *) 0;
					}
				}
				else
				{
					Database.version = PQserverVersion( Database.handle );
					break;
				}
			}
			break;
		}
#endif
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

#ifdef	_OCCI_MYSQL
/*	-------------------------------------------	*/
/*	     o c c i _ m y s q l _ r e c o r d 		*/
/*	-------------------------------------------	*/
private	int	occi_mysql_record( struct	occi_table * tptr,  struct occi_expression *expression )
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
/*	    o c c i _ m y s q l _ r e c o r d s 	*/
/*	-------------------------------------------	*/
/*	parses a result set and delivers record set	*/
/*	-------------------------------------------	*/
private	int	occi_mysql_records( struct occi_table * tptr,  struct occi_expression *expression )
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
#endif	/* MYSQL */

/*	-------------------------------------------	*/
/*	 p o s t g r e s _ r t r i m _ s t r i n g	*/
/*	-------------------------------------------	*/
private	int	postgres_rtrim_string( char * sptr, int slen )
{
	int	rlen=0;
	int	x;
	for (	x=0;
		x < slen;
		x++ )
		if ( *(sptr+x) != ' ' )
			rlen = (x+1);
	return( rlen );
}

#ifdef	_OCCI_POSTGRE
/*	-------------------------------------------	*/
/*	  o c c i _ p o s t g r e  _ r e c o r d 	*/
/*	-------------------------------------------	*/
private	int	occi_postgre_record( struct	occi_table * tptr,  struct occi_expression *expression )
{
	unsigned int	nf=0;
	unsigned int 	nr=0;
	unsigned int	r;
	unsigned int 	f;
	unsigned int	fs=0;
	char *			fn=(char*) 0;
	char *			vptr=(char *) 0;
	int				vlen=0;

	if (!( tptr->result ))
		return(118);

	else if (!( nf = PQnfields(tptr->result) ))
	{
		PQclear( tptr->result );
		return(119);
	}
	else
	{
		/* ------------- */
		/* for row count */
		/* ------------- */
		for (	r=0, 
				nr = PQntuples( tptr->result );
				r < nr;
				r++ )
		{
				/* ---------------- */
				/* for column count */
				/* ---------------- */
				for (	f=0;
						f < nf;
						f++ )
				{
					/* -------------------------- */
					/* retrieve field information */
					/* -------------------------- */
					if (!( fn = PQfname( tptr->result, f ) ))
						continue;
					else if (!( fs = PQfsize( tptr->result, f ) ))
						continue;
					else if (!( vlen = PQgetlength( tptr->result, r, f ) ))
						continue;
					else if (!( vptr = PQgetvalue( tptr->result, r, f ) ))
						continue;
					else if (!( vlen = postgres_rtrim_string( vptr, vlen ) ))
						continue;
					/* ----------------------------------- */
					/* use field as required by expression */
					/* ----------------------------------- */
					else occi_sql_field( expression, fn, vptr, vlen );
				}
		}
		PQclear( tptr->result );
		return(0);
	}
}

/*	-------------------------------------------	*/
/*	  o c c i _ p o s t g r e _ r e c o r d s 	*/
/*	-------------------------------------------	*/
/*	parses a result set and delivers record set	*/
/*	-------------------------------------------	*/
private	int	occi_postgre_records( struct occi_table * tptr,  struct occi_expression *expression )
{
	unsigned int	nf=0;
	unsigned int 	nr=0;
	unsigned int	r;
	unsigned int 	f;
	unsigned int	fs=0;
	char *			fn=(char*) 0;
	char *			vptr=(char *) 0;
	int				vlen=0;

	if (!( tptr->result ))
		return(118);

	else if (!( nf = PQnfields(tptr->result) ))
	{
		PQclear( tptr->result );
		return(119);
	}
	else
	{
		/* ------------- */
		/* for row count */
		/* ------------- */
		for (	r=0, 
				nr = PQntuples( tptr->result );
				r < nr;
				r++ )
		{
				/* ---------------- */
				/* for column count */
				/* ---------------- */
				for (	f=0;
						f < nf;
						f++ )
				{
					/* -------------------------- */
					/* retrieve field information */
					/* -------------------------- */
					if (!( fn = PQfname( tptr->result, f ) ))
						continue;
					else if (!( fs = PQfsize( tptr->result, f ) ))
						continue;
					else if (!( vlen = PQgetlength( tptr->result, r, f ) ))
						continue;
					else if (!( vptr = PQgetvalue( tptr->result, r, f ) ))
						continue;
					else if (!( vlen = postgres_rtrim_string( vptr, vlen ) ))
						continue;
					/* ----------------------------------- */
					/* use field as required by expression */
					/* ----------------------------------- */
					else occi_sql_list_item( expression, fn, vptr, vlen );
				}
		}
		PQclear( tptr->result );
		return(0);
	}
}

#endif	/* POSTGRE */

/*	-------------------------------------------	*/
/*	       o c c i _ s q l _ r e c o r d 		*/
/*	-------------------------------------------	*/
/*	parses a result set and delivers field data	*/
/*	-------------------------------------------	*/
private	int	occi_sql_record( struct	occi_table * tptr,  struct occi_expression *expression )
{
	switch ( Database.status )
	{
#ifdef	_OCCI_MYSQL
	case	_OCCI_MYSQL		:
		return( occi_mysql_record(tptr, expression) );
#endif
#ifdef	_OCCI_POSTGRE
	case	_OCCI_POSTGRE	:
		return( occi_postgre_record( tptr, expression ) );
#endif
	default					:
		return( 55 );
	}
}

/*	-------------------------------------------	*/
/*	       o c c i _ s q l _ r e c o r d 		*/
/*	-------------------------------------------	*/
/*	parses a result set and delivers field data	*/
/*	-------------------------------------------	*/
private	int	occi_sql_records( struct	occi_table * tptr,  struct occi_expression *expression )
{
	switch ( Database.status )
	{
#ifdef	_OCCI_MYSQL
	case	_OCCI_MYSQL		:
		return( occi_mysql_records(tptr, expression) );
#endif
#ifdef	_OCCI_POSTGRE
	case	_OCCI_POSTGRE	:
		return( occi_postgre_records( tptr, expression ) );
#endif
	default					:
		return( 55 );
	}
}

/*	-------------------------------------------	*/
/*	 f i r s t _ o c c i _ s q l _ r e c o r d 	*/
/*	-------------------------------------------	*/
public	int	first_occi_sql_record( char * category,  struct occi_expression *expression )
{
	char *	xptr;
	struct	occi_table * tptr;
	int	status;
	char	buffer[2048];
	if ( expression->value )
		sprintf(buffer,"WHERE %s",expression->value);
	else	buffer[0] = 0;
	if (!( tptr = locate_occi_sql_table( category ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else if (!( xptr = allocate( 
			strlen( _SELECT_ALL_FROM ) + 
			strlen( tptr->name ) + 
			strlen( buffer ) + 
			strlen( _ORDER_BY_FIRST ) + 16 ) ))
		return( 27 );
	else	sprintf(xptr,"%s %s %s %s",_SELECT_ALL_FROM,tptr->name,buffer,_ORDER_BY_FIRST);

	occi_sql_lock();
	status = occi_sql_query( 3,tptr, xptr );
	liberate( xptr );
	if ( status != 0)
	{
		occi_sql_unlock();
		return( 64 );
	}
	else
	{
		status = occi_sql_record( tptr, expression );
		occi_sql_unlock();
		return( status );
	}
}

/*	------------------------------------------------	*/
/*	p r e v i o u s _ o c c i _ s q l _ r e c o r d 	*/
/*	------------------------------------------------	*/
public	int	previous_occi_sql_record( char * category,  struct occi_expression *expression )
{
	char *	xptr;
	struct	occi_table * tptr;
	int	status;
	if (!( tptr = locate_occi_sql_table( category ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else if (!( tptr->handle ))
		return( 50 );
	else if (!( xptr = allocate( 
				strlen( ( expression->value ? expression->value : "" ) ) + 
				strlen( tptr->name ) + 
				strlen( _SELECT_ALL_FROM ) + 
				strlen( _ORDER_BY_LAST ) + 16 ) ))
		return( 27 );
	else	sprintf( xptr,"%s %s %s %s",_SELECT_ALL_FROM,tptr->name,( expression->value ? expression->value : "" ),_ORDER_BY_LAST);

	occi_sql_lock();
	status = occi_sql_query( 4,tptr, xptr );
	liberate( xptr );
	if ( status != 0)
	{
		occi_sql_unlock();
		return( 64 );
	}
	else
	{
		status = occi_sql_record( tptr, expression );
		occi_sql_unlock();
		return( status );
	}
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
	occi_sql_lock();
	status = occi_sql_query( 5,tptr, xptr );
	liberate( xptr );
	occi_sql_unlock();
	if ( status )
		return( 81 );
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
	occi_sql_lock();
	status = occi_sql_query( 6,tptr, xptr );
	liberate( xptr );
	if ( status != 0 )
	{
		occi_sql_unlock();
		return( 78 );
	}
	else
	{
		status = occi_sql_record( tptr, expression );
		occi_sql_unlock();
		return( status );
	}
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
	else if (!( xptr = allocate( strlen( ( expression->value ? expression->value : "" ) ) + strlen( tptr->name ) + strlen( _SELECT_ALL_FROM ) + strlen( _ORDER_BY_ORDERID ) + 16 ) ))
		return( 27 );
	else	sprintf( xptr,"%s %s %s %s",
			_SELECT_ID_FROM,tptr->name,
			( expression->value ? expression->value : "" ),
			_ORDER_BY_ORDERID);
	occi_sql_lock();
	status = occi_sql_query( 7,tptr, xptr );
	liberate( xptr );
	if ( status )
	{
		occi_sql_unlock();
		return( 78 );
	}
	else
	{
		status = occi_sql_records( tptr, expression );
		occi_sql_unlock();
		return( status );
	}
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
	occi_sql_lock();
	status = occi_sql_query( 8,tptr, xptr );
	liberate( xptr );
	occi_sql_unlock();
	if ( status )
		return( 78 );
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
	occi_sql_lock();
	status = occi_sql_query( 9,tptr, xptr );
	liberate( xptr );
	occi_sql_unlock();
	if ( status )
		return( 78 );
	else	return( 0 );
}

/*	-------------------------------------------	*/
/*	  n e x t _ o c c i _ s q l _ r e c o r d 	*/
/*	-------------------------------------------	*/
public	int	next_occi_sql_record( char * category,  struct occi_expression *expression )
{
	int	status;
	char *	xptr;
	struct	occi_table * tptr;
	if (!( tptr = locate_occi_sql_table( category ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else if (!( xptr = allocate( 
				strlen( ( expression->value ? expression->value : "" ) ) + 
				strlen( tptr->name ) + 
				strlen( _SELECT_ALL_FROM ) + 
				strlen( _ORDER_BY_FIRST ) + 16 ) ))
		return( 27 );
	else	sprintf( xptr,"%s %s %s %s",_SELECT_ALL_FROM,tptr->name,( expression->value ? expression->value : "" ),_ORDER_BY_FIRST);

	occi_sql_lock();
	status = occi_sql_query( 10,tptr, xptr );
	liberate( xptr );
	if ( status != 0)
	{
		occi_sql_unlock();
		return( 48 );
	}
	else
	{
		status = occi_sql_record( tptr, expression );
		occi_sql_unlock();
		return( status );
	}
}

/*	-------------------------------------------	*/
/*	  l a s t _ o c c i _ s q l _ r e c o r d 	*/
/*	-------------------------------------------	*/
public	int	last_occi_sql_record( char * category,  struct occi_expression *expression )
{
	int	status;
	char *	xptr;
	struct	occi_table * tptr;
	char	buffer[2048];
	if ( expression->value )
		sprintf(buffer,"WHERE %s",expression->value);
	else	buffer[0] = 0;
	if (!( tptr = locate_occi_sql_table( category ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else if (!( xptr = allocate( 
			strlen( _SELECT_ALL_FROM ) + 
			strlen( tptr->name ) +  
			strlen( buffer ) +			
			strlen( _ORDER_BY_LAST ) + 16 ) ))
		return( 27 );
	else	sprintf( xptr,"%s %s %s %s",_SELECT_ALL_FROM,tptr->name,buffer,_ORDER_BY_LAST);

	occi_sql_lock();
	status = occi_sql_query( 11,tptr, xptr );
	liberate( xptr );
	if ( status != 0)
	{
		occi_sql_unlock();
		return( 48 );
	}
	else
	{
		status = occi_sql_record( tptr, expression );
		occi_sql_unlock();
		return( status );
	}
}

/*	-------------------------------------------	*/
/*	 s t a r t _ s q l _ t r a n s a c t i o n	*/
/*	-------------------------------------------	*/
public	int	start_sql_transaction(char * tablename)
{
	int	status;
	struct	occi_table * tptr;
	if (!( tptr = locate_occi_sql_table( tablename ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else
	{
		occi_sql_lock();
		status = occi_sql_query( 12,tptr, _START_TRANSACTION );
		occi_sql_unlock();
		if (!(status))
			return( status );
		else	return( status );
	}
}

/*	-------------------------------------------	*/
/*	c o m m i t _ s q l _ t r a n s a c t i o n	*/
/*	-------------------------------------------	*/
public	int	commit_sql_transaction(char * tablename)
{
	int	status;
	struct	occi_table * tptr;
	if (!( tptr = locate_occi_sql_table( tablename ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else
	{
		occi_sql_lock();
		status = occi_sql_query( 13,tptr, _COMMIT_TRANSACTION );
		occi_sql_unlock();
		if (!(status))
			return( status );
		else	return( status );
	}
}

/*	-------------------------------------------------	*/
/*	 r o l l b a c k _ s q l _ t r a n s a c t i o n	*/
/*	-------------------------------------------------	*/
public	int	rollback_sql_transaction(char * tablename)
{
	int	status;
	struct	occi_table * tptr;
	if (!( tptr = locate_occi_sql_table( tablename ) ))
		return( 40 );
	else if (!( tptr->handle ))
		return( 50 );
	else
	{
		occi_sql_lock();
		status = occi_sql_query( 14,tptr, _ROLLBACK_TRANSACTION );
		occi_sql_unlock();
		if (!( status ))
			return( status );
		else	return( status );
	}
}

/*	-------------------------------------------	*/
/*	     s q l _ e s c a p e d _ v a l u e 		*/
/*	-------------------------------------------	*/
private	char	sql_escape_buffer[8192];
public	char *	sql_escaped_value( char * sptr )
{
	char *	rptr;
	char *	wptr;
	int	c;
	int	extra=0;
	int	total=0;
	for (	wptr=sptr;
		*wptr != 0;
		wptr++ )
	{
		switch ( *wptr )
		{
		case	'\t'	:
		case	'\r'	:
		case	'\n'	:
		case	'"'	:
		case	0x0027:
		case	0x005C	:
			extra++;
		}
		total++;
	}
	if (!( extra ))
		return( sptr );
	else if ((extra + total) > 8192 )
		return( sptr );
	else if (!( rptr = sql_escape_buffer ))
		return( sptr );
	switch ( Database.status )
	{
#ifdef	_OCCI_MYSQL
	case	_OCCI_MYSQL	:
		wptr = rptr;
		while ((c = *(sptr++)) != 0)
		{
			switch ( c )
			{
			case	'\t'	:
				*(wptr++) = 0x005C;
				*(wptr++) = 't';
				break;
			case	'\r'	:
				*(wptr++) = 0x005C;
				*(wptr++) = 'n';
				break;
			case	'\n'	:
				*(wptr++) = 0x005C;
				*(wptr++) = 'n';
				break;
			case	'"'	:
			case	0x0027	:	/* ' */
			case	0x005C	:	/* \ */
				*(wptr++) = 0x005C;
			default		:
				*(wptr++) = c;
			}
		}
		*(wptr++) = 0;
		return( rptr );
#endif
#ifdef	_OCCI_POSTGRE
	case	_OCCI_POSTGRE	:
		if (((total*2)+1) > 8192 )
			return( sptr );
		else
		{
			PQescapeStringConn(Database.handle,rptr, sptr,total, (int *) 0);
			return( rptr );
		}
#endif
	default			:
		return( sptr );
	}

}

/*	-------------------------------------------	*/
/*	   s q l _ u n e s c a p e d _ v a l u e 	*/
/*	-------------------------------------------	*/
public	char *	sql_unescaped_value( char * sptr )
{
	int	c;
	char * rptr;
	char * wptr;
	if (!( sptr ))
		return( sptr );
#ifdef	_OCCI_POSTGRE
	else if ( Database.status  == _OCCI_POSTGRE )
		return( allocate_string( sptr ) );
#endif
	else if (!( rptr = allocate_string( sptr ) ))
		return( rptr );
	else
	{
		wptr = rptr;
		while ((c = *(sptr++)) != 0)
		{
			if ( c == 0x005C )
			{
				switch ((c = *(sptr++)))
				{
				case	't'	:
					c = '\t';
					break;
				case	'n'	:
					c = '\n';
					break;
				case	'r'	:
					c = '\r';
					break;
				}
			}
			*(wptr++) = c;
		}
		return( rptr );
	}
}

/*	-------------------------------------------	*/
/*	     s q l _ e s c a p e d _ f i l t e r	*/
/*	-------------------------------------------	*/
public	char *	sql_escaped_filter( char * sptr )
{
	char *	rptr;
	char *	wptr;
	int	c;
	int	extra=0;
	int	total=0;
#ifdef	_OCCI_POSTGRE
	if ( Database.status  == _OCCI_POSTGRE )
		return( sql_escaped_value( sptr ) );
#endif
	for (	wptr=sptr;
		*wptr != 0;
		wptr++ )
	{
		switch ( *wptr )
		{
		case	'\t'	:
		case	'\r'	:
		case	'\n'	:
		case	'"'	:
		case	0x0025	:
		case	0x0027	:
		case	0x005C	:
			extra++;
		}
		total++;
	}
	if (!( extra ))
		return( sptr );
	else if ((extra + total) > 4096 )
		return( sptr );
	else if (!( rptr = sql_escape_buffer ))
		return( sptr );
	else
	{
		wptr = rptr;
		while ((c = *(sptr++)) != 0)
		{
			switch ( c )
			{
			case	'\t'	:
				*(wptr++) = 0x005C;
				*(wptr++) = 't';
				break;
			case	'\r'	:
				*(wptr++) = 0x005C;
				*(wptr++) = 'n';
				break;
			case	'\n'	:
				*(wptr++) = 0x005C;
				*(wptr++) = 'n';
				break;
			case	'"'	:
			case	0x0027	:	/* ' */
			case	0x0025	:	/* % */
			case	0x005C	:	/* \ */
				*(wptr++) = 0x005C;
			default		:
				*(wptr++) = c;
			
			}
		}
		*(wptr++) = 0;
		return( rptr );
	}
}

/*	-------------------------------------	*/
/*	o r d e r _ s q l _ t i m e s t a m p	*/
/*	------------------------------------- 	*/
public	void	order_sql_timestamp( char * buffer )
{
	struct timeval  TV = { 0, 0 };
	struct timezone TZ = { 0, 0 };
	gettimeofday(&TV,&TZ);
	sprintf(buffer,"%08.8X%08.8X",TV.tv_sec,TV.tv_usec);
	return;
}

/*	-------------------------------		*/
/*	b u i l d _ s q l _ f i l t e r		*/
/*	-------------------------------		*/
public	char *	build_sql_filter(char * nptr, char * vptr, int type )
{
	char	buffer[2048];
	char *	rptr;
	if ((!( nptr )) || (!( vptr )))
		return( (char *) 0 );
	else	
	{
		switch ( type )
		{
		case	0 :
			sprintf(buffer,"_%s = '%s'",nptr,sql_escaped_filter(vptr));
			break;
		case	1 :
			sprintf(buffer,"_%s LIKE '%s%c'",nptr,sql_escaped_filter(vptr),0x0025);
			break;
		case	2 :
			sprintf(buffer,"_%s LIKE '%c%s'",nptr,0x0025,sql_escaped_filter(vptr));
			break;
		case	3 :
			sprintf(buffer,"_%s LIKE '%c%s%c'",nptr,0x0025,sql_escaped_filter(vptr),0x0025);
			break;
		case	4 :
			sprintf(buffer,"_%s > '%s'",nptr,sql_escaped_filter(vptr));
			break;
		case	5 :
			sprintf(buffer,"_%s >= '%s'",nptr,sql_escaped_filter(vptr));
			break;
		case	6 :
			sprintf(buffer,"_%s < '%s'",nptr,sql_escaped_filter(vptr));
			break;
		case	7 :
			sprintf(buffer,"_%s <= '%s'",nptr,sql_escaped_filter(vptr));
			break;
		default	:
			return((char *) 0);
		}
		return( allocate_string( buffer ) );
	}
}

	/* ----------- */
#endif	/* _occi_sql_c */
	/* ----------- */



