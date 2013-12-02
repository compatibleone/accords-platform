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

#ifndef	_occi_sql_h
#define	_occi_sql_h

#include "standard.h"

#define	_OCCI_MYSQL		1
// #define	_OCCI_POSTGRE 	2

#ifndef	public
#define	public
#endif

#ifndef	private
#define	private	static
#endif

#ifdef	_OCCI_MYSQL
#include <mysql/mysql.h>
#endif

#ifdef	_OCCI_POSTGRE
#include <libpq-fe.h>
#endif

#define	_CREATE_DATABASE	"CREATE DATABASE IF NOT EXISTS "
#define	_CREATE_TABLE		"CREATE TABLE IF NOT EXISTS "
#define	_INSERT_INTO		"INSERT INTO"
#define	_UPDATE_WHERE		"UPDATE"
#define	_SELECT_ALL_FROM	"SELECT * FROM"
#define	_SELECT_ID_FROM		"SELECT id FROM"
#define	_ORDER_BY_ORDERID	"ORDER BY orderid ASC "
#define	_ORDER_BY_FIRST		"ORDER BY orderid ASC LIMIT 1"
#define	_ORDER_BY_LAST		"ORDER BY orderid DESC LIMIT 1"
#define	_DELETE_FROM		"DELETE FROM"
#define	_SINGLE_ROW		"LIMIT 1"
#define	_START_TRANSACTION	"START TRANSACTION"
#define	_COMMIT_TRANSACTION	"COMMIT"
#define	_ROLLBACK_TRANSACTION	"ROLLBACK"

struct	occi_database_thread
{
	struct	occi_database_thread *	next;
	pthread_t			tid;
};

struct	occi_table
{
	struct	occi_table * previous;
	struct	occi_table * next;
	char * 	name;
	char *	description;
	char *	base;
	char *	host;
	int	status;
	int	port;
	char *	user;
	char * 	pass;
	void *	handle;
	void *	result;
};

struct	occi_database 
{
	int	status;
	char * 	nature;
	void *	handle;
	char *	hostname;
	char *	basename;
	char *	username;
	char *	password;
	struct	occi_table * FirstTable;
	struct	occi_table * LastTable;
};

struct	occi_expression
{
	char *	value;
	int	(*handler)(void *, char *, char *, int);
	void *	context;
	char *	orderby;
};

struct	response_context
{
	struct	rest_response * response;
	char *	host;
	int	port;
	char *	location;
};

public	void *	allocate(int);
public	void *	liberate(void *);
public	char *	allocate_string(char *);


public	int	initialise_occi_sql( char * hostname, char * databasename, char * username, char * password );
public	void	terminate_occi_sql();

public	struct	occi_table * initialise_occi_sql_table( char * tablename, char * description );
public	int	drop_occi_sql_table( char * tablename );

public	int	first_occi_sql_record( char * category, struct occi_expression *expression );
public	int	previous_occi_sql_record( char * category, struct occi_expression *expression );
public	int	insert_occi_sql_record( char * category, struct occi_expression *expression );
public	int	search_occi_sql_record( char * category, struct occi_expression *expression );
public	int	update_occi_sql_record( char * category, struct occi_expression *expression );
public	int	delete_occi_sql_record( char * category, struct occi_expression *expression );
public	int	next_occi_sql_record( char * category, struct occi_expression *expression );
public	int	last_occi_sql_record( char * category, struct occi_expression *expression );

public	char *	build_sql_filter(char * nptr, char * vptr, int type );
public	char *	sql_escaped_value( char * sptr );
public	char *	sql_unescaped_value( char * sptr );
public	char *	sql_escaped_filter( char * sptr );

public	void	order_sql_timestamp( char * buffer );

public	int	start_sql_transaction(char * tablename);
public	int	commit_sql_transaction(char * tablename);
public	int	rollback_sql_transaction(char * tablename);

	/* ----------- */
#endif	/* _occi_sql_h */
	/* ----------- */


