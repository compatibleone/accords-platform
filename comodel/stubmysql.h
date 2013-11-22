#ifndef	_mysql_h
#define	_mysql_h

/*	------------------------	*/
/*	dummy my sql header file	*/
/*	------------------------	*/
struct	mysql_field
{
	int	number;
	char *	name;
	char *	type;
	char *	value;
	struct	mysql_field * next;
	struct	mysql_field * previous;
};

typedef	struct mysql_field MYSQL_FIELD;

typedef	char ** MYSQL_ROW;

struct	mysql_result
{
	char *	request;
	int	fields;
	MYSQL_ROW	rows;
};

typedef	struct mysql_result MYSQL_RES;

struct	mysql_handle
{
	FILE * h;
	int	status;
	char *	hostname;
	char *	database;
	char *	username;
	char *	password;
	struct	mysql_field * first;
	struct	mysql_field * last;
	struct	mysql_result * result;
};


typedef struct mysql_handle MYSQL;

int			mysql_query( MYSQL * handle, char * query );
MYSQL *		mysql_init( MYSQL * handle );
MYSQL *		mysql_real_connect( MYSQL * handle, char * host, char * user, char * pass, char * base, int a, int b, int c );
MYSQL_RES *	mysql_use_result( MYSQL * handle );
int			mysql_num_fields( MYSQL_RES * result );
MYSQL_FIELD *	mysql_fetch_field( MYSQL_RES * result );
MYSQL_ROW 	mysql_fetch_row( MYSQL_RES * result );
unsigned long * mysql_fetch_lengths( MYSQL_RES * result );
void		mysql_field_seek( MYSQL_RES * result, int offset );
void		mysql_close( MYSQL * handle );
void 		mysql_free_result(MYSQL_RES *result);
char *		mysql_error(MYSQL * handle);
int		mysql_select_db( MYSQL * handle, char * database );

	/* -------- */
#endif 	/* _mysql_h */
	/* -------- */



