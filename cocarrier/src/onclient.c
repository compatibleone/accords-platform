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
#ifndef _on_client_c
#define _on_client_c

#include "onclient.h"
#include "restpublic.h"

#define	_CORDS_NONE "none"

private	struct	on_config On = {
	(char *) 0,
	(char *) 0,

	(char *) 0,
	(char *) 0,

	(char *) 0,
	(char *) 0,

	(char *) 0,
	(char *) 0,

	(char *) 0,
	(char *) 0,

	0 

	};

public	struct	rest_header   *	on_authenticate();

/*	------------------------------------------------------------	*/
/*		l i b e r a t e _ o n _ r e s p o n s e			*/
/*	------------------------------------------------------------	*/
public	struct on_response * liberate_on_response( struct on_response * rptr )	
{
	if ( rptr )
	{
		if ( rptr->response )
			rptr->response = liberate_rest_response( rptr->response );
		if ( rptr->jsonroot )
			rptr->jsonroot = drop_data_element( rptr->jsonroot );
		if ( rptr->xmlroot )
			rptr->xmlroot = document_drop( rptr->xmlroot );
		if ( rptr->content )
			liberate( rptr->content );
		liberate( rptr );
	}
	return((struct on_response *) 0);
}

/*	----------------------------------------------------------	*/
/*		a l l o c a t e _ o n _ r e s p o n s e			*/
/*	----------------------------------------------------------	*/
private	struct	on_response * allocate_on_response()
{
	struct	on_response * rptr=(struct on_response *) 0;
	if (!( rptr = allocate( sizeof( struct on_response ) ) ))
		return( rptr );
	else
	{
		rptr->nature = _TEXT_NONE;
		rptr->content= (char *) 0;
		rptr->xmlroot = (struct xml_element *) 0;
		rptr->jsonroot = (struct data_element *) 0;
		rptr->response = (struct rest_response *) 0;
		return( rptr );
	}
}

/*	------------------------------------------------------------	*/
/*		 		o n _ c h e c k 			*/
/*	------------------------------------------------------------	*/
private	struct	on_response * on_check( struct rest_response * aptr )
{
	struct	on_response * rptr=(struct on_response *) 0;
	struct	rest_header * hptr=(struct rest_header *) 0;;
	if ( aptr )
	{
		if ( check_verbose() )
		{
			printf("   ON Client Response : %s %u %s\n",aptr->version,aptr->status,aptr->message);

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
	if (!( rptr = allocate_on_response() ))
		return( rptr );
	else 	rptr->response = aptr;
	
	if (!( aptr->body ))
		return(rptr);
	else if (!( hptr = rest_resolve_header( aptr->first, "Content-Type" ) ))
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
	else
	{
		rptr->nature = _TEXT_XML;
		rptr->xmlroot = document_parse_file( aptr->body );
		return( rptr );
	}
}

/*	----------------------------------------------------------------	*/
/*	 	o n _ c r e a t e _ n e t w o r k _ r e q u e s t		*/
/*	----------------------------------------------------------------	*/
public	char * on_create_network_request(
		char * name,
		char * address,
		char * size
		)
{
	char *	filename;
	FILE *	h;
	int	bytes;
	struct	rest_header * hptr;

	if (!( hptr = on_authenticate() ))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		/* ----------------------------------------- */
		/* generate network creation request element */
		/* ----------------------------------------- */
		fprintf(h,"<NETWORK>\n");
		fprintf(h,"<NAME>%s</NAME>\n",name);
		if (!( rest_valid_string( address ) ))
			fprintf(h,"<ADDRESS/>\n");
		else 	fprintf(h,"<ADDRESS>%s</ADDRESS>\n",address);
		if (!( rest_valid_string( size ) ))
		{
			fprintf(h,"<SIZE>256</SIZE>\n");
			fprintf(h,"<PUBLIC>NO</PUBLIC>\n");
		}
		else if (!( strcasecmp( size, "B" )))
		{
			fprintf(h,"<SIZE>%u</SIZE>\n",65635);
			fprintf(h,"<PUBLIC>NO</PUBLIC>\n");
		}

		else if (!( strcasecmp( size, "C" )))
		{
			fprintf(h,"<SIZE>%u</SIZE>\n",256);
			fprintf(h,"<PUBLIC>NO</PUBLIC>\n");
		}

		else if (!( strcasecmp( size, "PB" )))
		{
			fprintf(h,"<SIZE>%u</SIZE>\n",65635);
			fprintf(h,"<PUBLIC>YES</PUBLIC>\n");
		}
		else if (!( strcasecmp( size, "PC" )))
		{
			fprintf(h,"<SIZE>%u</SIZE>\n",256);
			fprintf(h,"<PUBLIC>YES</PUBLIC>\n");
		}
		else
		{
			fprintf(h,"<SIZE>256</SIZE>\n");
			fprintf(h,"<PUBLIC>NO</PUBLIC>\n");
		}
		fprintf(h,"</NETWORK>\n");
		fclose(h);
		return( filename );
	}
}

/*	----------------------------------------------------------------	*/
/*	 	o n _ c r e a t e _ s t o r a g e _ r e q u e s t		*/
/*	----------------------------------------------------------------	*/
public	char * on_create_storage_request(
		char * name,
		char * description,
		char * type,
		char * size
		)
{
	char *	filename;
	FILE *	h;
	int	bytes;
	struct	rest_header * hptr;

	if (!( hptr = on_authenticate() ))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		/* ----------------------------------------- */
		/* generate network creation request element */
		/* ----------------------------------------- */
		fprintf(h,"<STORAGE>\n");
		fprintf(h,"<NAME>%s</NAME>\n",name);
		fprintf(h,"<DESCRIPTION>%s</DESCRIPTION>\n",description);
		fprintf(h,"<TYPE>%s</TYPE>\n",type);
		fprintf(h,"<SIZE>%s</SIZE>\n",size);
		fprintf(h,"</STORAGE>\n");
		fclose(h);
		return( filename );
	}
}

/*	----------------------------------------------------------------	*/
/*	 	o n _ c r e a t e _ c o m p u t e _ r e q u e s t		*/
/*	----------------------------------------------------------------	*/
/*	this function creates an xml file containing the compute create		*/
/*	request providing the flavor, image and network description.		*/
/*	----------------------------------------------------------------	*/
public	char * on_create_compute_request(
		char * identity, 
		char * flavour, 
		char * image,
		char * network, 
		char * local,
		char * architecture,
		char * driver ) 
{
	char *	filename;
	FILE *	h;
	int	bytes;
	struct	rest_header * hptr;

	if (!( hptr = on_authenticate() ))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"w" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		/* ---------------------------------------- */
		/* generate server creation request element */
		/* ---------------------------------------- */
		fprintf(h,"<COMPUTE>\n");
		if ( rest_valid_string( identity ) )
		{
			fprintf(h,"<NAME>%s</NAME>\n",identity);
		}
		if ( rest_valid_string( flavour ) )
		{
			fprintf(h,"<INSTANCE_TYPE>%s</INSTANCE_TYPE>\n",flavour);
		}

		fprintf(h,"<STATE>running</STATE>\n");

		/* -------------------------------------------- */
		/* specify the base operating system disk image */
		/* -------------------------------------------- */
		fprintf(h,"<DISK>\n");
		if ( rest_valid_string( image ) )
		{
			fprintf(h,"<STORAGE href='%s/storage/%s'/>\n",On.base,image);
		}
		fprintf(h,"<TYPE>OS</TYPE>\n");
		if ( rest_valid_string( driver ) )
		{
			fprintf(h,"<DRIVER>%s</DRIVER>\n",driver);
		}
		fprintf(h,"</DISK>\n");

		/* ------------------------------ */
		/* 64 bit architecture is crucial */
		/* ------------------------------ */
		if ( rest_valid_string( architecture ) )
		{
			fprintf(h,"<OS><ARCH>%s</ARCH></OS>\n",architecture);
		}

		/* -------------------------------------- */
		/* a second public address may be present */
		/* -------------------------------------- */
		if ( rest_valid_string( network ) )
		{
			fprintf(h,"<NIC>\n");
			fprintf(h,"<NETWORK href='%s'/>\n",network);
			fprintf(h,"</NIC>\n");
		}

		/* -------------------------------------- */
		/* a local address must always be present */
		/* -------------------------------------- */
		if (!( rest_valid_string( local ) ))
		{
			/* ------------- */
			/* default local */
			/* ------------- */
			fprintf(h,"<NIC>\n");
			fprintf(h,"<NETWORK href='%s'/>\n","1");
			fprintf(h,"</NIC>\n");
		}
		else
		{
			/* --------------- */
			/* explicite local */
			/* --------------- */
			fprintf(h,"<NIC>\n");
			fprintf(h,"<NETWORK href='%s'/>\n",local);
			fprintf(h,"</NIC>\n");
		}

		/* ----------------------------------------------- */
		/* a stanhdard graphical interface would be useful */
		/* ----------------------------------------------- */
		fprintf(h,"<GRAPHICS>\n");
		fprintf(h,"<TYPE>vnc</TYPE>\n");
		fprintf(h,"<PORT>-1</PORT>\n");
		fprintf(h,"<LISTEN>0.0.0.0</LISTEN>\n");
		fprintf(h,"</GRAPHICS>\n");

		fprintf(h,"</COMPUTE>\n");
		fclose(h);
		return( filename );
	}
}

/*	----------------------------------------------------------------	*/
/*	 	o n _ s t o p _ c o m p u t e _ r e q u e s t		*/
/*	----------------------------------------------------------------	*/
public	char * on_stop_compute_request( char * identity )
{
	char *	filename;
	FILE *	h;
	int	bytes;
	struct	rest_header * hptr;

	if (!( hptr = on_authenticate() ))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"w" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		/* ---------------------------------------- */
		/* generate server creation request element */
		/* ---------------------------------------- */
		fprintf(h,"<COMPUTE>\n");
		fprintf(h,"<ID>%s</ID>\n",identity);
		fprintf(h,"<STATE>stopped</STATE>\n");
		fprintf(h,"</COMPUTE>\n");
		fclose(h);
		return( filename );
	}
}

/*	----------------------------------------------------------------	*/
/*	 	o n _ s h u t d o w n _ c o m p u t e _ r e q u e s t		*/
/*	----------------------------------------------------------------	*/
public	char * on_shutdown_compute_request( char * identity )
{
	char *	filename;
	FILE *	h;
	int	bytes;
	struct	rest_header * hptr;

	if (!( hptr = on_authenticate() ))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"w" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		/* ---------------------------------------- */
		/* generate server creation request element */
		/* ---------------------------------------- */
		fprintf(h,"<COMPUTE>\n");
		fprintf(h,"<ID>%s</ID>\n",identity);
		fprintf(h,"<STATE>shutdown</STATE>\n");
		fprintf(h,"</COMPUTE>\n");
		fclose(h);
		return( filename );
	}
}

/*	----------------------------------------------------------------	*/
/*	 	o n _ s t a r t _ c o m p u t e _ r e q u e s t			*/
/*	----------------------------------------------------------------	*/
public	char * on_start_compute_request( char * identity )
{
	char *	filename;
	FILE *	h;
	int	bytes;
	struct	rest_header * hptr;

	if (!( hptr = on_authenticate() ))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"w" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		/* ---------------------------------------- */
		/* generate server creation request element */
		/* ---------------------------------------- */
		fprintf(h,"<COMPUTE>\n");
		fprintf(h,"<ID>%s</ID>\n",identity);
		fprintf(h,"<STATE>running</STATE>\n");
		fprintf(h,"</COMPUTE>\n");
		fclose(h);
		return( filename );
	}
}

/*	----------------------------------------------------------------	*/
/*	 	o n _ c r e a t e _ i m a g e _ r e q u e s t			*/
/*	----------------------------------------------------------------	*/
public	char * on_create_image_request(
		char * number,
		char * oldnumber,
		char * newname,
		char * driver
		)
{
	char *	filename;
	FILE *	h;
	int	bytes;
	struct	rest_header * hptr;

	if (!( hptr = on_authenticate() ))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		/* ----------------------------------------- */
		/* generate image creation request element */
		/* ----------------------------------------- */
		fprintf(h,"<COMPUTE href='%s/compute/%s'>\n",On.base,number);
		fprintf(h,"<ID>%s</ID>\n",number);
		fprintf(h,"<DISK id='0'>");
		fprintf(h,"<STORAGE href='%s/storage/%s'/>\n",On.base,oldnumber);
		fprintf(h,"<SAVE_AS name='%s'/>\n",newname);
		if ( rest_valid_string( driver ) )
		{
			fprintf(h,"<DRIVER>%s</DRIVER>\n",driver);
		}
		fprintf(h,"</DISK>\n");
		fprintf(h,"</COMPUTE>\n");
		fclose(h);
		return( filename );
	}
}

/*	----------------------------------------------------------------	*/
/*	 	o n _ p u b l i c _ i m a g e _ r e q u e s t			*/
/*	----------------------------------------------------------------	*/
public	char * on_public_image_request( char * image )
{
	char *	filename;
	FILE *	h;
	int	bytes;
	struct	rest_header * hptr;

	if (!( hptr = on_authenticate() ))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<STORAGE><ID>%s</ID><PUBLIC>YES</PUBLIC></STORAGE>\n",image);
		fclose(h);
		return( filename );
	}
}
/*	----------------------------------------------------------------	*/
/*	 	o n _ p r i v a t e _ i m a g e _ r e q u e s t			*/
/*	----------------------------------------------------------------	*/
public	char * on_private_image_request( char * image )
{
	char *	filename;
	FILE *	h;
	int	bytes;
	struct	rest_header * hptr;

	if (!( hptr = on_authenticate() ))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<STORAGE><ID>%s</ID><PUBLIC>NO</PUBLIC></STORAGE>\n",image);
		fclose(h);
		return( filename );
	}
}

/*	----------------------------------------------------------------	*/
/*	    	o n _ r e n a m e _ i m a g e _ r e q u e s t			*/
/*	----------------------------------------------------------------	*/
public	char * on_rename_image_request( char * image, char * newname )
{
	char *	filename;
	FILE *	h;
	int	bytes;
	struct	rest_header * hptr;

	if (!( hptr = on_authenticate() ))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<STORAGE><ID>%s</ID><NAME>%s</NAME></STORAGE>\n",image,newname);
		fclose(h);
		return( filename );
	}
}

/*	----------------------------------------------------------------	*/
/*	    o n _ p e r s i s t e n t _ i m a g e _ r e q u e s t		*/
/*	----------------------------------------------------------------	*/
public	char * on_persistent_image_request( char * image )
{
	char *	filename;
	FILE *	h;
	int	bytes;
	struct	rest_header * hptr;

	if (!( hptr = on_authenticate() ))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<STORAGE><ID>%s</ID><PERSISTENT>YES</PERSISTENT></STORAGE>\n",image);
		fclose(h);
		return( filename );
	}
}

/*	----------------------------------------------------------------	*/
/*	    	o n _ v o l a t i l e _ i m a g e _ r e q u e s t		*/
/*	----------------------------------------------------------------	*/
public	char * on_volatile_image_request( char * image )
{
	char *	filename;
	FILE *	h;
	int	bytes;
	struct	rest_header * hptr;

	if (!( hptr = on_authenticate() ))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<STORAGE><ID>%s</ID><PERSISTENT>NO</PERSISTENT></STORAGE>\n",image);
		fclose(h);
		return( filename );
	}
}

/*	----------------------------------------------------------------	*/
/*			o n _ c r e a t e _ i m a g e 				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_create_image( char * id, char * filename ) 
{ 
	char 	buffer[2048];
	sprintf(buffer,"/compute/%s",id);
	return( on_put_request( buffer, filename ) );
}


/*	----------------------------------------------------------------	*/
/*	 	o n _ c r e a t e _ s e r v e r _ r e q u e s t			*/
/*	----------------------------------------------------------------	*/
public	char * on_create_server_request(
		char * identity, 
		char * flavour, 
		char * image,
		char * network, 
		char * local,
		char * personality,
		char * filename ) 
{
	return( on_create_compute_request( identity, flavour, image, network,local, personality, filename ) );
}

/*	----------------------------------------------------------------	*/
/*		o n _ c l i e n t _ g e t _ r e q u e s t			*/
/*	----------------------------------------------------------------	*/
public	struct	on_response * 
	on_client_get_request(
		char * target, char * tls, char * agent, struct rest_header * hptr ) 
{ 
	return( on_check( rest_client_get_request( target, tls, agent, hptr ) ) );
}

/*	----------------------------------------------------------------	*/
/*		o n _ c l i e n t _ d e l e t e _ r e q u e s t			*/
/*	----------------------------------------------------------------	*/
public	struct	on_response * 
	on_client_delete_request(
		char * target, char * tls, char * agent, struct rest_header * hptr ) 
{
	return( on_check( rest_client_delete_request( target, tls, agent, hptr ) ) );
}

/*	----------------------------------------------------------------	*/
/*		o n _ c l i e n t _ h e a d _ r e q u e s t			*/
/*	----------------------------------------------------------------	*/
public	struct	on_response * 
	on_client_head_request(
		char * target, char * tls, char * agent, struct rest_header * hptr ) 
{
	return( on_check( rest_client_head_request( target, tls, agent, hptr ) ) );
}

/*	----------------------------------------------------------------	*/
/*		o n _ c l i e n t _ p o s t _ r e q u e s t			*/
/*	----------------------------------------------------------------	*/
public	struct	on_response * 
	on_client_post_request(
		char * target, char * tls, char * agent, char * filename, struct rest_header * hptr ) 
{ 
	return( on_check( rest_client_post_request( target, tls, agent, filename, hptr ) ) );
}

/*	----------------------------------------------------------------	*/
/*		o n _ c l i e n t _ p u t _ r e q u e s t			*/
/*	----------------------------------------------------------------	*/
public	struct	on_response * 
	on_client_put_request(
		char * target, char * tls, char * agent, char * filename, struct rest_header * hptr ) 
{
	return( on_check( rest_client_put_request( target, tls, agent, filename, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*			o n _ a u t h e n t i c a t e ()		*/
/*	------------------------------------------------------------	*/
public	struct	rest_header   *	on_authenticate	( )
{
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	struct	on_response	*	rptr;
	struct	url		*	uptr;
	char 			*	nptr;
	int				status;
	char	buffer[256];
	char	uwork[256];
	char	pwork[256];
	if (!( On.user ))
		return((struct rest_header *) 0);
	else if (!( On.password ))
		return((struct rest_header *) 0);
	else if (!( On.version ))
		return((struct rest_header *) 0);
	else if (!( On.authenticate ))
		if (!( On.authenticate = rest_encode_credentials( On.user, On.password ) ))
			return((struct rest_header *) 0);
	if (!( On.authenticate ))
		return((struct rest_header *) 0);
	else if (!( hptr = rest_create_header( "Authorization", On.authenticate ) ))
		return( hptr );
	else if (!( hptr->next = rest_create_header( _HTTP_ACCEPT, "text/xml" ) ))
		return( liberate_rest_header( hptr ) );
	else	return((hptr->next->previous = hptr));
}

/*	----------------------------------------------------------------	*/
/*			o n _ l i s t _ r e q u e s t 				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_list_request( char * buffer ) 
{ 
	struct	on_response	*	rptr=(struct on_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	if (!( hptr = on_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( On.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = on_client_get_request( nptr, On.tls, On.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
	return(0); 
}

/*	----------------------------------------------------------------	*/
/*			o n _ g e t _ r e q u e s t 				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_get_request( char * buffer ) 
{ 
	struct	on_response	*	rptr=(struct on_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	if (!( hptr = on_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( On.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = on_client_get_request( nptr, On.tls, On.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
	return(0); 
}

/*	----------------------------------------------------------------	*/
/*			o n _ d e l e t e _ r e q u e s t 				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_delete_request( char * buffer ) 
{ 
	struct	on_response	*	rptr=(struct on_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	if (!( hptr = on_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( On.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = on_client_delete_request( nptr, On.tls, On.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
	return(0); 
}

/*	----------------------------------------------------------------	*/
/*			o n _ p u t _ r e q u e s t 				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_put_request( char * buffer, char * filename ) 
{ 
	struct	on_response	*	rptr=(struct on_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	if (!( hptr = on_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( On.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = on_client_put_request( nptr, On.tls, On.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
	return(0); 
}

/*	----------------------------------------------------------------	*/
/*			o n _ p o s t _ r e q u e s t 				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_post_request( char * buffer, char * filename ) 
{ 
	struct	on_response	*	rptr=(struct on_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	if (!( hptr = on_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( On.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = on_client_post_request( nptr, On.tls, On.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
	return(0); 
}

/*	----------------------------------------------------------------	*/
/*		   o n _ l i s t _ c o m p u t e _ p o o l			*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_list_compute_pool( ) 
{ 
	return( on_list_request( "/compute" ) );
}

/*	----------------------------------------------------------------	*/
/*		   o n _ l i s t _ n e t w o r k _ p o o l			*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_list_network_pool( ) 
{ 
	return( on_list_request( "/network" ) );
}

/*	----------------------------------------------------------------	*/
/*		   o n _ l i s t _ s t o r a g e _ p o o l			*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_list_storage_pool( ) 
{ 
	return( on_list_request( "/storage" ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ l i s t _ s e r v e r s				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_list_servers	( )
{ 
	return( on_list_compute_pool() );
}

/*	----------------------------------------------------------------	*/
/*			o n _ l i s t _ f l a v o r s				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_list_flavors ( )
{ 
	return( allocate_on_response() );
}

/*	----------------------------------------------------------------	*/
/*			o n _ l i s t _ i m a g e S				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_list_images  ( ) 
{
	return(on_list_storage_pool()); 
}

/*	----------------------------------------------------------------	*/
/*			o n _ c r e a t e _ c o m p u t e			*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_create_compute( char * filename ) 
{ 
	char 	buffer[2048];
	sprintf(buffer,"/compute");
	return( on_post_request( buffer, filename ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ s t o p _ c o m p u t e				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_stop_compute( char * id, char * filename ) 
{ 
	char 	buffer[2048];
	sprintf(buffer,"/compute/%s",id);
	return( on_put_request( buffer, filename ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ s h u t d o w n _ c o m p u t e			*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_shutdown_compute( char * id, char * filename ) 
{ 
	char 	buffer[2048];
	sprintf(buffer,"/compute/%s",id);
	return( on_put_request( buffer, filename ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ s t a r t _ c o m p u t e				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_start_compute( char * id, char * filename ) 
{ 
	char 	buffer[2048];
	sprintf(buffer,"/compute/%s",id);
	return( on_put_request( buffer, filename ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ c r e a t e _ s t o r a g e			*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_create_storage( char * filename ) 
{ 
	char 	buffer[2048];
	sprintf(buffer,"/storage");
	return( on_post_request( buffer, filename ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ c r e a t e _ n e t w o r k 			*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_create_network( char * filename ) 
{ 
	char 	buffer[2048];
	sprintf(buffer,"/network");
	return( on_post_request( buffer, filename ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ c r e a t e _ s e r v e r				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_create_server( char * filename ) 
{ 
	return( on_create_compute( filename ) ); 
}

/*	----------------------------------------------------------------	*/
/*			o n _ g e t _ c o m p u t e 				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_get_compute( char * id ) 
{ 
	char 	buffer[2048];
	sprintf(buffer,"/compute/%s",id);
	return( on_get_request( buffer ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ g e t _ s t o r a g e 				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_get_storage	( char * id ) 
{ 
	char 	buffer[2048];
	sprintf(buffer,"/storage/%s",id);
	return( on_get_request( buffer ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ g e t _ n e t w o r k 				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_get_network	( char * id ) 
{ 
	char 	buffer[2048];
	sprintf(buffer,"/network/%s",id);
	return( on_get_request( buffer ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ g e t _ s e r v e r				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_get_server	( char * id ) 
{ 
	return( on_get_compute( id ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ g e t _ f l a v o u r				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_get_flavour  ( char * id ) 
{
	return( allocate_on_response() ); 
}

/*	----------------------------------------------------------------	*/
/*			o n _ g e t _ i m a g e 				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_get_image    ( char * id ) 
{
	return( on_get_storage( id ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ u p d a t e _ c o m p u t e			*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_update_compute( char * id, char * filename ) 
{ 
	char 	buffer[2048];
	sprintf(buffer,"/compute/%s",id);
	return( on_put_request( buffer, filename ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ u p d a t e _ s t o r a g e			*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_update_storage( char * id, char * filename ) 
{ 
	char 	buffer[2048];
	sprintf(buffer,"/storage/%s",id);
	return( on_put_request( buffer, filename ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ u p d a t e _ n e t w o r k			*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_update_network( char * id, char * filename ) 
{ 
	char 	buffer[2048];
	sprintf(buffer,"/network/%s",id);
	return( on_put_request( buffer, filename ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ u p d a t e _ s e r v e r				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_update_server( char * id, char * filename ) 
{ 
	return( on_update_compute( id, filename ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ d e l e t e _ c o m p u t e			*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_delete_compute( char * id ) 
{
	char 	buffer[2048];
	sprintf(buffer,"/compute/%s",id);
	return( on_delete_request( buffer ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ d e l e t e _ s t o r a g e			*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_delete_storage( char * id ) 
{
	char 	buffer[2048];
	sprintf(buffer,"/storage/%s",id);
	return( on_delete_request( buffer ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ d e l e t e _ n e t w o r k			*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_delete_network( char * id ) 
{
	char 	buffer[2048];
	sprintf(buffer,"/network/%s",id);
	return( on_delete_request( buffer ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ d e l e t e _ s e r v e r				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_delete_server( char * id ) 
{
	return( on_delete_compute( id ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ d e l e t e _ i m a g e				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_delete_image ( char * id ) 
{
	return( on_delete_storage( id ));
}

/*	----------------------------------------------------------------	*/
/*			o n _ p u b l i c _ i m a g e				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_public_image ( char * id, char * filename ) 
{
	char 	buffer[2048];
	sprintf(buffer,"/storage/%s",id);
	return( on_put_request( buffer, filename ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ p r i v a t e _ i m a g e				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_private_image ( char * id, char * filename ) 
{
	char 	buffer[2048];
	sprintf(buffer,"/storage/%s",id);
	return( on_put_request( buffer, filename ) );
}

/*	----------------------------------------------------------------	*/
/*			o n _ r e n a m e _ i m a g e				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_rename_image ( char * id, char * filename ) 
{
	char 	buffer[2048];
	sprintf(buffer,"/storage/%s",id);
	return( on_put_request( buffer, filename ) );
}

/*	----------------------------------------------------------------	*/
/*		o n _ p e r s i s t e n t _ i m a g e				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_persistent_image ( char * id, char * filename ) 
{
	char 	buffer[2048];
	sprintf(buffer,"/storage/%s",id);
	return( on_put_request( buffer, filename ) );
}

/*	----------------------------------------------------------------	*/
/*		o n _ v o l a t i l e _ i m a g e				*/
/*	----------------------------------------------------------------	*/
public	struct	on_response *	on_volatile_image ( char * id, char * filename ) 
{
	char 	buffer[2048];
	sprintf(buffer,"/storage/%s",id);
	return( on_put_request( buffer, filename ) );
}

/*	------------------------------------------------------------	*/
/*		o n _ i n i t i a l i s e _ c l i e n t 		*/
/*	------------------------------------------------------------	*/
public	int	on_initialise_client( 
		char * user, char * password, 
		char * host, char * agent, char * version, char * tls )
{
	if (!( On.user = allocate_string( user )))
		return( 27 );
	if (!( On.password = allocate_string( password )))
		return( 27 );
	if (!( On.host = allocate_string( host )))
		return( 27 );
	if (!( On.base = allocate_string( host )))
		return( 27 );
	else if (!( On.agent = allocate_string( agent )))
		return( 27 );
	else if (!( On.version = allocate_string( version )))
		return( 27 );
	else if (!( On.namespace = allocate( strlen( _ON_NS_COMPUTE ) + strlen( On.version ) + 16 ) ))
		return( 27 );
	else	
	{
		sprintf(On.namespace,"%s%s",_ON_NS_COMPUTE,On.version);
		On.authenticate= (char *) 0;
		if (!( tls ))
			On.tls = (char *) 0;
		else if ((On.tls = allocate_string(tls)) != (char *) 0)
			if ( (!( strlen( On.tls ) )) || ( *(On.tls) == '0' ) )
				On.tls = liberate( On.tls );
		return( 0 );
	}
}

	/* ----------- */
#endif	/* _onclient_c */
	/* ----------- */

