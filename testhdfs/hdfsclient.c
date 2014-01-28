#ifndef	_hdfs_client_c
#define	_hdfs_client_c

#include "hdfsclient.h"
#include "restclient.h"
#include "restpublic.h"

/*	--------------------------	*/
/*	Hdfs Client Configuration	*/
/*	--------------------------	*/
private	struct	hdfs_client
{
	char *	agent;
	char *	tls;
	char *	host;
	int	port;
} Hdfs = 
{
	(char *) 0,	/* agent 	*/
	(char *) 0,	/* tls		*/
	(char *) 0,	/* host		*/
		 0	/* port		*/
};

/*	-----------------------------------------	*/
/*	   h d f s _ a u t h e n t i c a t e		*/
/*	-----------------------------------------	*/
private	struct	rest_header * hdfs_authenticate()
{
	struct	rest_header * hptr;
	return( rest_create_header( "X-HDFS-AUTHORIZE", "None" ) );
}

/*	-----------------------------------------	*/
/*	    h d f s _ g e t _ r e q u e s t		*/
/*	-----------------------------------------	*/
private	struct	rest_response * hdfs_get_request( char * url )
{
	struct	rest_header * hptr;
	if (!( hptr = hdfs_authenticate()))
		return((struct rest_response *) 0);
	else 	return( rest_client_get_request( url, Hdfs.tls, Hdfs.agent, hptr ) );
}

/*	-----------------------------------------	*/
/*	  h d f s _ d e l e t e _ r e q u e s t		*/
/*	-----------------------------------------	*/
private	struct	rest_response * hdfs_delete_request( char * url )
{
	struct	rest_header * hptr;
	if (!( hptr = hdfs_authenticate()))
		return((struct rest_response *) 0);
	else	return( rest_client_get_request( url, Hdfs.tls, Hdfs.agent, hptr ) );
}

/*	-----------------------------------------	*/
/*	    h d f s _ p o s t _ r e q u e s t		*/
/*	-----------------------------------------	*/
private	struct	rest_response * hdfs_post_request( char * url, char * filename )
{
	struct	rest_header * hptr;
	if (!( hptr = hdfs_authenticate()))
		return((struct rest_response *) 0);
	else	return( rest_client_post_request( url, Hdfs.tls, Hdfs.agent, filename, hptr ) );
}

/*	-----------------------------------------	*/
/*	    h d f s _ p u t _ r e q u e s t		*/
/*	-----------------------------------------	*/
private	struct	rest_response * hdfs_put_request( char * url, char * filename )
{
	struct	rest_header * hptr;
	if (!( hptr = hdfs_authenticate()))
		return((struct rest_response *) 0);
	else	return( rest_client_put_request( url, Hdfs.tls, Hdfs.agent, filename, hptr ) );
}

/*	-----------------------------------------	*/
/*	 h d f s  _ r e m o v e _ r e s p o n s e 	*/
/*	-----------------------------------------	*/
public	struct	hdfs_response * hdfs_remove_response( struct hdfs_response *  rptr )
{
	if ( rptr )
	{
		if ( rptr->jsonroot )
			rptr->jsonroot = drop_data_element( rptr->jsonroot );
		if ( rptr->xmlroot )
			rptr->xmlroot = document_drop( rptr->xmlroot );
		if ( rptr->content )
			liberate( rptr->content );
		if ( rptr->response )
			rptr->response = liberate_rest_response( rptr->response );
		rptr = liberate( rptr );
	}
	return((struct hdfs_response *) 0);
}

/*	-----------------------------------------	*/
/*		h d f s  _ r e s u l t 			*/
/*	-----------------------------------------	*/
public	struct	hdfs_response * hdfs_result( struct rest_response *  aptr )
{
	struct	hdfs_response * rptr;
	struct	rest_header * hptr=(struct rest_header *) 0;;
	if ( aptr )
	{
		if ( check_verbose() )
		{
			printf("   HDFS Client Response : %s %u %s\n",aptr->version,aptr->status,aptr->message);

			if ( check_debug() )
			{
				for (	hptr=aptr->first;
					hptr != (struct rest_header *) 0;
					hptr = hptr->next )
				{
					if (!( hptr->name )) 
						continue;
					else	printf("   %s: %s\n",hptr->name,hptr->value);
				}
				if ( aptr->body )
				{
					printf("   [ %s ] \n",aptr->body );
				}
			}
		}
	}
	if (!( rptr = allocate( sizeof( struct hdfs_response ) ) ))
		return( rptr );
	else
	{
		rptr->nature = _TEXT_NONE;
		rptr->content= (char *) 0;
		rptr->xmlroot = (struct xml_element *) 0;
		rptr->jsonroot = (struct data_element *) 0;
		rptr->response = aptr;
		if (!( aptr->body ))
			return(rptr);
		if (!( hptr = rest_resolve_header( aptr->first, "Content-Type" ) ))
			return(rptr);
		else if (!( rptr->content = allocate_string( hptr->value ) ))
			return( rptr );
		else if ((!( strcmp( rptr->content, "text/json" 	)))
		     ||  (!( strcmp( rptr->content, "application/json" 	)))
		     ||  (!( strcmp( rptr->content, "x-application/json"))))
		{
			rptr->nature = _TEXT_JSON;
			rptr->jsonroot = json_parse_file( aptr->body );
			return( rptr );
		}
		else if ((!( strcmp( rptr->content, "text/xml" 		)))
		     ||  (!( strcmp( rptr->content, "application/xml" 	)))
		     ||  (!( strcmp( rptr->content, "x-application/xml"))))
		{
			rptr->nature = _TEXT_XML;
			rptr->xmlroot = document_parse_file( aptr->body );
			return( rptr );
		}
		else	return( rptr );
		
	}
}	

/*	--------------------------------------------------------------------------------	*/
/*				CREATE AND WRITE AND APPEND					*/
/*	--------------------------------------------------------------------------------	*/
/*				H D F S   C R E A T E   F I L E					*/
/*	--------------------------------------------------------------------------------	*/
/*	curl -i -X PUT "http://<HOST>:<PORT>/webhdfs/v1/<PATH>?op=CREATE			*/
/*                    [&overwrite=<true|false>][&blocksize=<LONG>][&replication=<SHORT>]	*/
/*                    [&permission=<OCTAL>][&buffersize=<INT>]"					*/
/*	--------------------------------------------------------------------------------	*/
/*	HTTP/1.1 307 TEMPORARY_REDIRECT								*/
/*	Location: http://<DATANODE>:<PORT>/webhdfs/v1/<PATH>?op=CREATE...			*/
/*	Content-Length: 0									*/
/*	--------------------------------------------------------------------------------	*/
public	struct	hdfs_response *	hdfs_create_file( struct hdfs_subscription * sptr, char * path, int options )
{
	char	buffer[4096];
	sprintf(buffer,"%s%s?%sop=CREATE&overwrite=%s&blocksize=%u&replication=%u&permission=%o&buffersize=%u",
			sptr->base,path,( sptr->authenticate ? sptr->authenticate : "" ),
			(options & 0x1000 ? "true" : "false"),
			sptr->blocksize,
			sptr->replication,
			options & 0777,
			sptr->buffersize);
	return( hdfs_result( hdfs_put_request( buffer, (char *) 0 ) ));
}

/*	--------------------------------------------------------------------------------	*/
/*				H D F S   W R I T E   F I L E					*/
/*	--------------------------------------------------------------------------------	*/
/*	curl -i -X PUT -T <LOCAL_FILE> "http://<DATANODE>:<PORT>/webhdfs/v1/<PATH>?op=CREATE..."*/
/*	--------------------------------------------------------------------------------	*/
/*	HTTP/1.1 201 Created									*/
/*	Location: webhdfs://<HOST>:<PORT>/<PATH>						*/
/*	Content-Length: 0									*/
/*	--------------------------------------------------------------------------------	*/
public	struct	hdfs_response *	hdfs_write_file( struct hdfs_subscription * sptr, char * location, char * filename )
{
	return( hdfs_result( hdfs_put_request( location, filename ) ));
}

/*	--------------------------------------------------------------------------------	*/
/*				H D F S   A P P E N D   F I L E					*/
/*	--------------------------------------------------------------------------------	*/
/*	curl -i -X POST "http://<HOST>:<PORT>/webhdfs/v1/<PATH>?op=APPEND[&buffersize=<INT>]"	*/
/*	--------------------------------------------------------------------------------	*/
/*	HTTP/1.1 307 TEMPORARY_REDIRECT								*/
/*	Location: http://<DATANODE>:<PORT>/webhdfs/v1/<PATH>?op=APPEND...			*/
/*	Content-Length: 0									*/
/*	--------------------------------------------------------------------------------	*/
/*	curl -i -X POST -T <LOCAL_FILE> "http://<DATANODE>:<PORT>/webhdfs/v1/<PATH>?op=APPEND."	*/
/*	--------------------------------------------------------------------------------	*/
/*	HTTP/1.1 200 OK										*/
/*	Content-Length: 0									*/
/*	--------------------------------------------------------------------------------	*/
public	struct	hdfs_response *	hdfs_append_file( struct hdfs_subscription * sptr, char * path, char * buffer, int length )
{
	struct	hdfs_response * rptr=(struct hdfs_response *) 0;
	return( rptr );
}

/*	--------------------------------------------------------------------------------	*/
/*					OPEN AND READ						*/
/*	--------------------------------------------------------------------------------	*/
/*				H D F S   O P E N   F I L E					*/
/*	--------------------------------------------------------------------------------	*/
/*	curl -i -L "http://<HOST>:<PORT>/webhdfs/v1/<PATH>?op=OPEN				*/
/*                    [&offset=<LONG>][&length=<LONG>][&buffersize=<INT>]"			*/
/*	--------------------------------------------------------------------------------	*/
/*	HTTP/1.1 307 TEMPORARY_REDIRECT								*/
/*	Location: http://<DATANODE>:<PORT>/webhdfs/v1/<PATH>?op=OPEN...				*/
/*	Content-Length: 0									*/
/*	--------------------------------------------------------------------------------	*/
public	struct	hdfs_response *	hdfs_open_file( struct hdfs_subscription * sptr, char * path, int seek, int length )
{
	char	buffer[4096];
	sprintf(buffer,"%s%s?%sop=OPEN&offset=%u&length=%u&buffersize=%u",
			sptr->base,path,( sptr->authenticate ? sptr->authenticate : "" ),
			seek,length,
			sptr->buffersize);
	return( hdfs_result( hdfs_get_request( buffer ) ));
}

/*	--------------------------------------------------------------------------------	*/
/*				H D F S   R E A D   F I L E					*/
/*	--------------------------------------------------------------------------------	*/
/*	Follow redirect from OPEN								*/
/*	--------------------------------------------------------------------------------	*/
/*	HTTP/1.1 200 OK										*/
/*	Content-Type: application/octet-stream							*/
/*	Content-Length: 22									*/
/*												*/
/*	Hello, webhdfs user!									*/
/*	--------------------------------------------------------------------------------	*/
public	struct	hdfs_response *	hdfs_read_file( struct hdfs_subscription * sptr, char * location )
{
	return( hdfs_result( hdfs_get_request( location ) ));
}

/*	--------------------------------------------------------------------------------	*/
/*				H D F S   R E N A M E   F I L E					*/
/*	--------------------------------------------------------------------------------	*/
/*	curl -i -X PUT "<HOST>:<PORT>/webhdfs/v1/<PATH>?op=RENAME&destination=<PATH>"		*/
/*	HTTP/1.1 200 OK										*/
/*	Content-Type: application/json								*/
/*	Transfer-Encoding: chunked								*/
/*												*/
/*	{"boolean": true}									*/
/*	--------------------------------------------------------------------------------	*/
public	struct	hdfs_response *	hdfs_rename_file( struct hdfs_subscription * sptr, char * path, char * newpath )
{
	char	buffer[4096];
	sprintf(buffer,"%s%s?%sop=RENAME&destination=%s",
			sptr->base,path,( sptr->authenticate ? sptr->authenticate : "" ),newpath);
	return( hdfs_result( hdfs_put_request( buffer, (char *) 0 ) ));
}

/*	--------------------------------------------------------------------------------	*/
/*				H D F S   D E L E T E   F I L E					*/
/*	--------------------------------------------------------------------------------	*/
/*	curl -i -X DELETE "http://<host>:<port>/webhdfs/v1/<path>?op=DELETE			*/
/*                              [&recursive=<true|false>]"					*/
/*	--------------------------------------------------------------------------------	*/
/*	HTTP/1.1 200 OK										*/
/*	Content-Type: application/json								*/
/*	Transfer-Encoding: chunked								*/
/*												*/
/*	{"boolean": true}									*/
/*	--------------------------------------------------------------------------------	*/
public	struct	hdfs_response *	hdfs_delete_file( struct hdfs_subscription * sptr, char * path, int recursive )
{
	char	buffer[4096];
	sprintf(buffer,"%s%s?%sop=DELETE&recursive=%s",
			sptr->base,path,( sptr->authenticate ? sptr->authenticate : "" ),(recursive ? "true" : "false"));
	return( hdfs_result( hdfs_delete_request( buffer ) ));
}

/*	--------------------------------------------------------------------------------	*/
/*					H D F S   M K D I R 					*/
/*	--------------------------------------------------------------------------------	*/
/*	curl -i -X PUT "http://<HOST>:<PORT>/<PATH>?op=MKDIRS[&permission=<OCTAL>]"		*/
/*	--------------------------------------------------------------------------------	*/
/*	HTTP/1.1 200 OK										*/
/*	Content-Type: application/json								*/
/*	Transfer-Encoding: chunked								*/
/*												*/
/*	{"boolean": true}									*/
/*	--------------------------------------------------------------------------------	*/
public	struct	hdfs_response *	hdfs_mkdir( struct hdfs_subscription * sptr, char * path, int options)
{
	char	buffer[4096];
	sprintf(buffer,"%s%s?%sop=MKDIRS&permission=%o",
			sptr->base,path,( sptr->authenticate ? sptr->authenticate : "" ),options);
	return( hdfs_result( hdfs_put_request( buffer, (char *) 0 ) ));
}

/*	--------------------------------------------------------------------------------	*/
/*					H D F S   S T A T 					*/
/*	--------------------------------------------------------------------------------	*/
/*	curl -i  "http://<HOST>:<PORT>/webhdfs/v1/<PATH>?op=GETFILESTATUS"			*/
/*	--------------------------------------------------------------------------------	*/
/*	HTTP/1.1 200 OK										*/
/*	Content-Type: application/json								*/
/*	Transfer-Encoding: chunked								*/
/*												*/
/*	{											*/
/*	  "FileStatus":										*/
/*	  {											*/
/*	    "accessTime"      : 0,								*/
/*	    "blockSize"       : 0,								*/
/*	    "group"           : "supergroup",							*/
/*	    "length"          : 0,             //in bytes, zero for directories			*/
/*	    "modificationTime": 1320173277227,							*/
/*	    "owner"           : "webuser",							*/
/*	    "pathSuffix"      : "",								*/
/*	    "permission"      : "777",								*/
/*	    "replication"     : 0,								*/
/*	    "type"            : "DIRECTORY"    //enum {FILE, DIRECTORY}				*/
/*	  }											*/
/*	}											*/
/*	--------------------------------------------------------------------------------	*/
public	struct	hdfs_response *	hdfs_stat( struct hdfs_subscription * sptr, char * path )
{
	char	buffer[4096];
	sprintf(buffer,"%s%s?%sop=GETFILESTATUS",
			sptr->base,path,( sptr->authenticate ? sptr->authenticate : "" ));
	return( hdfs_result( hdfs_get_request( buffer ) ));
}

/*	--------------------------------------------------------------------------------	*/
/*					H D F S   L S D I R 					*/
/*	--------------------------------------------------------------------------------	*/
/*	curl -i  "http://<HOST>:<PORT>/webhdfs/v1/<PATH>?op=LISTSTATUS"				*/
/*	--------------------------------------------------------------------------------	*/
/*	HTTP/1.1 200 OK										*/
/*	Content-Type: application/json								*/
/*	Content-Length: 427									*/
/*												*/
/*	{											*/
/*	  "FileStatuses":									*/
/*	  {											*/
/*	    "FileStatus":									*/
/*	    [											*/
/*	      {											*/
/*	        "accessTime"      : 1320171722771,						*/
/*	        "blockSize"       : 33554432,							*/
/*	        "group"           : "supergroup",						*/
/*	        "length"          : 24930,							*/
/*	        "modificationTime": 1320171722771,						*/
/*	        "owner"           : "webuser",							*/
/*	        "pathSuffix"      : "a.patch",							*/
/*	        "permission"      : "644",							*/
/*	        "replication"     : 1,								*/
/*	        "type"            : "FILE"							*/
/*	      },										*/
/*	      {											*/
/*	        "accessTime"      : 0,								*/
/*	        "blockSize"       : 0,								*/
/*	        "group"           : "supergroup",						*/
/*	        "length"          : 0,								*/
/*	        "modificationTime": 1320895981256,						*/
/*	        "owner"           : "szetszwo",							*/
/*	        "pathSuffix"      : "bar",							*/
/*	        "permission"      : "711",							*/
/*	        "replication"     : 0,								*/
/*	        "type"            : "DIRECTORY"							*/
/*	      },										*/
/*	      ...										*/
/*	    ]											*/
/*	  }											*/
/*	}											*/
/*	--------------------------------------------------------------------------------	*/
public	struct	hdfs_response *	hdfs_lsdir( struct hdfs_subscription * sptr, char * path )
{
	char	buffer[4096];
	sprintf(buffer,"%s%s?%sop=LISTSTATUS",
			sptr->base,path,( sptr->authenticate ? sptr->authenticate : "" ));
	return( hdfs_result( hdfs_get_request( buffer ) ));
}

/*	------------------------------------------------	*/
/*	  t e r m i n a t e _ h d f s _ c l i e n t		*/
/*	------------------------------------------------	*/
public	int	hdfs_terminate_client()
{
	if ( Hdfs.agent )
		Hdfs.agent = liberate( Hdfs.agent );
	if ( Hdfs.tls )
		Hdfs.tls = liberate( Hdfs.tls );
	if ( Hdfs.host )
		Hdfs.host = liberate( Hdfs.host );
	return( 0 );
}

/*	------------------------------------------------	*/
/*	  i n i t i a l i s e _ h d f s _ c l i e n t		*/
/*	------------------------------------------------	*/
public	struct	hdfs_subscription * hdfs_initialise_client( char * user, char * password, char * host, char * agent, char * version, char * tls, int port )
{
	struct	hdfs_subscription * sptr;

	if (!( sptr = allocate( sizeof( struct hdfs_subscription ) )))
		return( sptr );
	else	memset( sptr, 0, sizeof( struct hdfs_subscription ) );

	if (!( sptr->user = allocate_string( user )))
		return( liberate( sptr ) );
	if (!( sptr->password = allocate_string( password )))
		return( liberate( sptr ) );
	if (!( sptr->host = allocate_string( host )))
		return( liberate( sptr ) );
	else if (!( sptr->agent = allocate_string( agent )))
		return( liberate( sptr ) );
	else if (!( sptr->version = allocate_string( version )))
		return( liberate( sptr ) );
	else if (!( sptr->base = allocate( strlen( sptr->host ) + strlen( sptr->version ) + strlen( "/webhdfs" + 8 ) )))
		return( liberate( sptr ) );
	else	sprintf(sptr->base,"%s%s%s",sptr->host,"/webhdfs/",sptr->version);

	if (!( sptr->authenticate = allocate( strlen( sptr->user ) + strlen( "user.name" ) + 8 ) ))
		return( liberate( sptr ) );
	else	sprintf( sptr->authenticate, "user.name=%s&", sptr->user );

	sptr->tls = tls;

	return( sptr );
}

	/* -------------- */
#endif	/* _hdfs_client_c */
	/* -------------- */

