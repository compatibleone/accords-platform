#ifndef	_mysql_c
#define	_mysql_c

#include <stdio.h>
#include <mysql/mysql.h>

void * allocate(int);
void * liberate(void *);
char * allocate_string(char *);

char *	mysql_error( MYSQL * handle )
{
		return("stub mysql standard error");
}

void	mysql_close( MYSQL * handle )
{
	printf("mysql_close(%s)\n",
		(handle ? "MYSQL" : "NULL" ));
	if ( handle )
	{

		liberate( handle );
	}
	return;
}


int 	mysql_query(MYSQL * handle, char *query)
{
	printf("mysql_query(%s,%s)\n",
		(handle ? "MYSQL" : "NULL" ),query);
	return(0);
}

int 	mysql_select_db(MYSQL * handle, char *database)
{
	printf("mysql_select_db(%s,%s)\n",
		(handle ? "MYSQL" : "NULL" ),database);
	return(0);
}

MYSQL *		mysql_init( MYSQL * handle )
{
	printf("mysql_init(%s)\n",
		(handle ? "MYSQL" : "NULL" ));
	return((MYSQL *) 0);
}

MYSQL *		mysql_real_connect( MYSQL * handle, char * host, char * user, char * pass, char * base, int a, int b, int c )
{
	MYSQL * h;
	printf("mysql_real_connect(%s,%s,%s,%s,%s,%u,%u,%u)\n",
		(handle ? "MYSQL" : "NULL" ),
		host, user, pass, base, a, b, c );
	if (!( h = allocate( sizeof( MYSQL ) ) ))
		return( h );
	else 
	{
		memset( h, 0, sizeof( MYSQL ) );
		if (!( h->hostname = allocate_string( host ) ))
		{
			mysql_close( h );
			return((MYSQL *) 0);
		}
		if (!( h->database = allocate_string( base ) ))
		{
			mysql_close( h );
			return((MYSQL *) 0);
		}
		if (!( h->username = allocate_string( user ) ))
		{
			mysql_close( h );
			return((MYSQL *) 0);
		}
		if (!( h->password = allocate_string( pass ) ))
		{
			mysql_close( h );
			return((MYSQL *) 0);
		}
		return( h );
	}
}

MYSQL_RES *	mysql_use_result( MYSQL * handle )
{
	printf("mysql_use_result(%s)\n",
		(handle ? "MYSQL" : "NULL" ));
	return(0);
}

int		mysql_num_fields( MYSQL_RES * result )
{
	printf("mysql_num_fields(%s)\n",
		(result ? "MYSQL_RES" : "NULL" ));
	return(0);
}

MYSQL_FIELD *	mysql_fetch_field( MYSQL_RES * result )
{
	printf("mysql_fetch_field(%s)\n",
		(result ? "MYSQL_RES" : "NULL" ));
	return((MYSQL_FIELD *)0);
}

void	mysql_field_seek( MYSQL_RES * result, int offset )
{
	printf("mysql_field_seek(%s,%u)\n",
		(result ? "MYSQL_RES" : "NULL" ),offset);
	return;
}

MYSQL_ROW 	mysql_fetch_row( MYSQL_RES * result )
{
	printf("mysql_fetch_row(%s)\n",
		(result ? "MYSQL_RES" : "NULL" ));
	return(0);
}

unsigned long * mysql_fetch_lengths( MYSQL_RES * result )
{
	printf("mysql_fetch_lengths(%s)\n",
		(result ? "MYSQL_RES" : "NULL" ));
	return(0);
}

void 	mysql_free_result(MYSQL_RES *result)
{
	printf("mysql_free_result(%s)\n",
		(result ? "MYSQL_RES" : "NULL" ));
	return;
}

	/* -------- */
#endif	/* _mysql_c */
	/* -------- */

