#ifndef	_hdfs_client_h
#define	_hdfs_client_h

#ifndef	public
#define	public
#endif

#ifndef	private
#define	private static
#endif

#include "standard.h"
#include "rest.h"
#include "url.h"
#include "urlpublic.h"
#include "restclient.h"
#include "co_json.h"
#include "document.h"

#define	_TEXT_NONE 	0
#define	_TEXT_XML	1
#define	_TEXT_JSON	2

struct	hdfs_subscription
{
	char *	user;
	char *	password;
	char *	authenticate;
	char *	host;
	char *	base;
	char *	version;
	char *	agent;
	char *	tls;
	int	port;
	int	buffersize;
	int	blocksize;
	int	replication;
};

struct	hdfs_response
{
	int	nature;
	char *	content;
	struct	rest_response * response;	
	struct	xml_element * xmlroot;
	struct	data_element * jsonroot;
};

public	struct 	hdfs_response * hdfs_remove_response( struct hdfs_response * response );
public	struct 	hdfs_response * hdfs_result( struct rest_response *  response );
public	struct	hdfs_response *	hdfs_create_file( struct hdfs_subscription * sptr, char * path, int options );
public	struct	hdfs_response *	hdfs_write_file( struct hdfs_subscription * sptr, char * location, char * filename );
public	struct	hdfs_response *	hdfs_append_file( struct hdfs_subscription * sptr, char * path,char * buffer, int length );
public	struct	hdfs_response *	hdfs_open_file( struct hdfs_subscription * sptr, char * path, int seek, int length );
public	struct	hdfs_response *	hdfs_read_file( struct hdfs_subscription * sptr, char * location );
public	struct	hdfs_response *	hdfs_rename_file( struct hdfs_subscription * sptr, char * path, char * newpath );
public	struct	hdfs_response *	hdfs_delete_file( struct hdfs_subscription * sptr, char * path, int recursive );
public	struct	hdfs_response *	hdfs_mkdir( struct hdfs_subscription * sptr, char * path, int options);
public	struct	hdfs_response *	hdfs_stat( struct hdfs_subscription * sptr, char * path );
public	struct	hdfs_response *	hdfs_lsdir( struct hdfs_subscription * sptr, char * path );
public	struct	hdfs_subscription * hdfs_initialise_client( char * user, char * password, char * host, char * agent, char * version, char * tls, int port );
public	int			hdfs_terminate_client();

	/* -------------- */
#endif	/* _hdfs_client_h */
	/* -------------- */

