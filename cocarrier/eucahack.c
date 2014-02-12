#include "rest.h"

char * euca_data_hack( char * image, char * flavor, char * data )
{
	char	work[1024];
	char	buffer[4096];
	char *	filename;
	FILE *	h;
	if (!( filename = rest_temporary_filename("tmp")))
		return((char *) 0);
	else if (!( h = fopen( filename,"w") ))
		return((char *) 0);
	else
	{
		fprintf(h,"%s\n",data);
		fclose(h);
	}
	sprintf(work,"ami-%08.8x",atoi( image ));
	image = work;
	switch ( atoi( flavor ) )
	{
	case	1	:	flavor = "m1.tiny"; break;
	case	2	:	flavor = "m1.small"; break;
	case	3	:	flavor = "m1.medium"; break;
	case	4	:	flavor = "m1.large"; break;
	case	5	:	flavor = "m1.xlarge"; break;
	default		:	flavor = "m1.tiny" ; break;
	}
	sprintf(buffer,"./co-euca %s %s %s",image,flavor,filename);
	if (!( h = fopen( "./euca-hack", "w" ) ))
		return((char *) 0);
	else 
	{
		fprintf(h,"%s\n",buffer);
		fclose(h);
		return( allocate_string( "./euca-hack" ) );
	}
}

struct	rest_response * euca_command_hack( char * execname )
{
	struct rest_response * rptr;
	char	buffer[4096];
	char *	filename;
	char * 	jsonname;
	int	contentlength;
	struct	rest_header * hptr;
	FILE *	h;
	int	iid;

	if (!( filename = rest_temporary_filename("tmp")))
	{
		printf("cannot allocate tempname for open pseudo stdout\n");
		return((struct rest_response *) 0);
	}

	
	sprintf(buffer,"chmod +x %s; %s > %s",execname,execname,filename);

	printf("**** HACK ( %s ) **** \n",buffer);

	int dummy = system( buffer );

	if (!( h = fopen( filename,"r") ))
	{
		printf("cannot open pseudo stdout : %s \n",filename);
		return((struct rest_response *) 0);
	}
	else if (!( fgets( buffer, 2000, h ) ))
	{
		printf("cannot read pseudo stdout : %s \n",filename);
		fclose(h);
		return((struct rest_response *) 0);
	}
	fclose(h);

	if (!( rptr = allocate_rest_response()))
		return( rptr );
	else
	{
		rest_response_status( rptr, 200, "OK" );
		rptr->version = allocate_string("HTTP/1.1");

		iid = atoi( buffer );

		sprintf(buffer,"{\n%cstatus%c : %cBUILD%c,\n%cadminPass%c : %cbidon%c,\n%cid%c : %c%u%c\n}\n",
				0x0022,0x0022,0x0022,0x0022,
				0x0022,0x0022,0x0022,0x0022,
				0x0022,0x0022,0x0022,iid,0x0022);

		if (!( hptr = rest_response_header( rptr,_HTTP_CONTENT_TYPE,"application/json" )))
		{
			printf("cannot create content type header\n");
			return( liberate_rest_response( rptr ) );
		}
		else if (!( jsonname = rest_temporary_filename("json")))
		{
			printf("cannot create json filename\n");
			return( liberate_rest_response( rptr ) );
		}
		else if (!( h = fopen( jsonname,"w" ) ))
		{
			printf("cannot create json file\n");
			return( liberate_rest_response( rptr ) );
		}
		else
		{
			fprintf(h,"%s",buffer);
			fclose( h );
		}

		rest_response_body( rptr, jsonname, _FILE_BODY );

		contentlength = strlen( buffer );
		sprintf(buffer,"%u",contentlength);

		if (!( hptr = rest_response_header( rptr,_HTTP_CONTENT_LENGTH,buffer )))
		{
			printf("cannot create content length header\n");
			return( liberate_rest_response( rptr ) );
		}
		else	return( rptr );

	}
}








