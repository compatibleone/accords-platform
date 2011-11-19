/* ------------------------------------------------------------------------------------	*/
/*				 CompatibleOne Cloudware				*/
/* ------------------------------------------------------------------------------------ */
/*											*/
/* Ce fichier fait partie de ce(tte) oeuvre de Iain James Marshall et est mise a 	*/
/* disposition selon les termes de la licence Creative Commons Paternit‚ : 		*/
/*											*/
/*			 	Pas d'Utilisation Commerciale 				*/
/*				Pas de Modification 					*/
/*				3.0 non transcrit.					*/
/*											*/
/* ------------------------------------------------------------------------------------ */
/* 			Copyright (c) 2011 Iain James Marshall for Prologue 		*/
/*				   All rights reserved					*/
/* ------------------------------------------------------------------------------------ */
#ifndef _os_client_c
#define _os_client_c

#include "osclient.h"

private	int	hack=0;

private	struct os_config Os = {
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

/*	------------------------------------------------------------	*/
/*		l i b e r a t e _ o s _ r e s p o n s e			*/
/*	------------------------------------------------------------	*/
public	struct os_response * liberate_os_response( struct os_response * rptr )	
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
	return((struct os_response *) 0);
}

/*	------------------------------------------------------------	*/
/*		 		o s _ c h e c k 			*/
/*	------------------------------------------------------------	*/
private	struct	os_response * os_check( struct rest_response * aptr )
{
	struct	os_response * rptr=(struct os_response *) 0;
	struct	rest_header * hptr=(struct rest_header *) 0;;
	if ( aptr )
	{
		if ( check_verbose() )
		{
			printf("   OS Client Response : %s %u %s\n",aptr->version,aptr->status,aptr->message);

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
	if (!( rptr = allocate( sizeof( struct os_response ) ) ))
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

/*	------------------------------------------------------------	*/
/*		 o s _ c l i e n t _ g e t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	os_response * 
	os_client_get_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	return( os_check( rest_client_get_request( target, tls, nptr, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*	      o s _ c l i e n t _ d e l e t e _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	os_response * 
	os_client_delete_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	return( os_check( rest_client_delete_request( target, tls, nptr, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*		 o s _ c l i e n t _ h e a d _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	os_response * 
	os_client_head_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	return( os_check( rest_client_head_request( target, tls, nptr, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*		 o s _ c l i e n t _ p o s t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	os_response * 
	os_client_post_request(
		char * target, char * tls, char * nptr, char * filename, struct rest_header * hptr )
{
	return( os_check( rest_client_post_request( target, tls, nptr, filename, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*		 o s _ c l i e n t _ p u t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	os_response * 
	os_client_put_request(
		char * target, char * tls, char * nptr, char * filename, struct rest_header * hptr )
{
	return( os_check( rest_client_put_request( target, tls, nptr, filename, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*			o s _ a u t h e n t i c a t e ()		*/
/*	------------------------------------------------------------	*/
public	struct	rest_header   *	os_authenticate	( )
{
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	struct	os_response	*	rptr;
	struct	url		*	uptr;
	char 			*	nptr;
	int				status;
	char	buffer[256];

	if (!( Os.user ))
		return( hptr );
	else if (!( Os.password ))
		return( hptr );
	else if (!( Os.version ))
		return( hptr );
	else if (!( Os.authenticate ))
	{

		sprintf(buffer,"/%s",Os.version);

		if (!( uptr = analyse_url( Os.host )))
			return( hptr );
		else if (!( uptr = validate_url( uptr ) ))
			return( hptr );
		else if (!( uptr->object = allocate_string( buffer ) ))
		{
			uptr = liberate_url( uptr );
			return( hptr );
		}
		else if (!( nptr = serialise_url( uptr,"" ) ))
		{
			uptr = liberate_url( uptr );
			return( hptr );
		}
		else	uptr = liberate_url( uptr );

		if (!( hptr = rest_create_header( "X-Auth-User", Os.user ) ))
		{
			liberate( nptr );
			return( hptr );
		}
		else if (!( hptr->next = rest_create_header( "X-Auth-Key", Os.password ) ))
		{
			liberate( nptr );
			return( liberate_rest_header( hptr ) );
		}
		else	hptr->next->previous = hptr;

		if (!( rptr = os_client_get_request( nptr, Os.tls, Os.agent, hptr ) ))
		{
			liberate( nptr );
			return( liberate_rest_header( hptr ) );
		}
		else if (!( hptr = rest_resolve_header(rptr->response->first,"X-Auth-Token") ))
		{
			if (!( Os.authenticate = allocate_string("abcde-1234-5678-fgh-ijklm") ))
			{
				liberate( nptr );
				return( (struct rest_header *) 0 );
			}
		}
		else if (!( Os.authenticate = allocate_string( hptr->value ) ))
		{
			liberate( nptr );
			return( (struct rest_header *) 0 );
		}
		else if (( hptr = rest_resolve_header(
				rptr->response->first,"X-Server-Management-Url")) 
				!= (struct rest_header *) 0)
		{
			if (!( Os.base = allocate_string( hptr->value ) ))
			{
				liberate( nptr );
				return( (struct rest_header *) 0 );
			}
		}
		else if (!( hptr = rest_resolve_header(rptr->response->first,"X-Identity") ))
		{
			if (!( Os.base = allocate( strlen( Os.host ) + strlen( Os.version ) + 16 ) ))
			{
				liberate( nptr );
				return( (struct rest_header *) 0 );
			}
			else	sprintf( Os.base, "%s/%s", Os.host, Os.version );
		}
		else if (!( Os.base = allocate_string( hptr->value ) ))
		{
			liberate( nptr );
			return( (struct rest_header *) 0 );
		}
	}
	if (!( Os.authenticate ))
		return((struct rest_header *) 0);
	else if (!( hptr = rest_create_header( "X-Auth-Token", Os.authenticate ) ))
		return( hptr );
	else if (!( hptr->next = rest_create_header( _HTTP_ACCEPT, "text/xml" ) ))
		return( liberate_rest_header( hptr ) );
	else	return((hptr->next->previous = hptr));
}

/*	------------------------------------------------------------	*/
/*			o s _ l i s t _ s e r v e r s			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_list_servers	( )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	sprintf(buffer,"/servers");

	if (!( hptr = os_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, Os.tls, Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			o s _ l i s t _ f l a v o u r s			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_list_flavors( )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	sprintf(buffer,"/flavors");

	if (!( hptr = os_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, Os.tls, Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			o s _ l i s t _ i m a g e s 			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_list_images( )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/images");

	if (!( hptr = os_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, Os.tls, Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}



/*	------------------------------------------------------------	*/
/*		    o s _ p a r s e _  m e t a d a t a 			*/
/*	------------------------------------------------------------	*/
private	int	os_parse_metadata( char ** rptr, char ** kptr, char ** dptr )
{
	char *	sptr;
	int	c;
	if (!( rptr ))
		return( 0 );
	else if (!( sptr = *rptr ))
		return( 0 );

	while ( ( *sptr == ' ' ) 
	|| 	( *sptr == '\r') 
	|| 	( *sptr == '\n' ) 
	|| 	( *sptr == '\t' ) )
		sptr++;

	if (!( *sptr ))		return(0);
	*kptr = sptr;
	while ( *sptr )
	{
		if ((( *sptr >= 'a' ) && ( *sptr <= 'z' ))
		||  (( *sptr >= 'A' ) && ( *sptr <= 'Z' ))
		||  (( *sptr >= '0' ) && ( *sptr <= '9' ))
		||  (( *sptr == '-' ) || ( *sptr == '_' )))
			sptr++;
	else	break;
	}
	c = *sptr;
	*(sptr++) = 0;
	if ( c != '=' )
	{
		while ( *sptr )
		{
			if ( *sptr != '=' )
				sptr++;
			else
			{
				*(sptr++) = 0;
				break;
			}
		}
		if (!( *sptr )) return( 0 );
	}
	*dptr = sptr;
	while ( ( *sptr == ' ' ) 
	|| 	( *sptr == '\r') 
	|| 	( *sptr == '\n' ) 
	|| 	( *sptr == '\t' ) )
		sptr++;
	if (!( *sptr ))	
	{
		*rptr = sptr;
		return(1);
	}
	while ( *sptr )
	{
		if ( *sptr == ',' )
			break;
		else if ( *sptr == ';' )
			break;
		else if ( *sptr == '\n' )
			break;
		else if ( *sptr == '\r' )
			break;
		else 	sptr++;
	}
	if ( *sptr ) *(sptr++) = 0;
	*rptr = sptr;
	return(1);	
}
			
/*	------------------------------------------------------------	*/
/*	   o s _ c r e a t e _  m e t a d a t a _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	char * 	os_create_metadata_request( char * personality )
{
	char *	filename;
	FILE *	h;
	int	bytes;
	char *	kptr;
	char *	dptr;
	char *	sptr;
	char *	tptr;
	struct	rest_header * hptr;
	if (!( hptr = os_authenticate() ))
		return((char *) 0);
	if (!( personality ))
		return( personality );
	else if (!( sptr = allocate_string( personality ) ))
		return( sptr );
	else	tptr = sptr;
	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<metadata xmlns=%c%s%c>\n",0x0022,Os.namespace,0x0022);
		while ( os_parse_metadata( &sptr, &kptr, &dptr ) != 0)
			fprintf(h,"<meta key='%s'>%s</meta>\n",kptr,dptr);
		fprintf(h,"</metadata>\n");
		fclose(h);
		liberate( tptr );
		return( filename );
	}


}

/*	------------------------------------------------------------	*/
/*	   	o s _ c r e a t e _  m e t a _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	char * os_create_meta_request( char * key, char * value )
{
	char *	filename;
	FILE *	h;
	int	bytes;

	struct	rest_header * hptr;
	if (!( hptr = os_authenticate() ))
		return((char *) 0);
	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<meta xmlns=%c%s%c\n",0x0022,Os.namespace,0x0022);
		fprintf(h," key=%c%s%c>%s</meta>\n",0x0022,key,0x0022,value);
		fclose(h);
		return( filename );
	}


}

/*	------------------------------------------------------------	*/
/*			o s _ g e t _ m e t a d a t a			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_get_metadata( char * id, char * name )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/servers/%s/metadata/%s",id,name);
	if (!( hptr = os_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, Os.tls, Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		   o s _ u p d a t e _ m e t a d a t a			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_update_metadata( char * id, char * name, char * value )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	char 			*	filename;
	sprintf(buffer,"/servers/%s/metadata/%s",id,name);
	if (!( filename = os_create_meta_request( name, value )))
		return( rptr );
	else if (!( hptr = os_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_put_request( nptr, Os.tls, Os.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		   o s _ d e l e t e _ m e t a d a t a			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_delete_metadata( char * id, char * name )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/servers/%s/metadata/%s",id,name);
	if (!( hptr = os_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_delete_request( nptr, Os.tls, Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

#include "eucahack.c"

/*	------------------------------------------------------------	*/
/*		o s _ c r e a te _  s e r v e r _ r e q u e s t		*/
/*	------------------------------------------------------------	*/

public	char * os_create_server_request(
		char * identity,	/* the identity of the server 	*/
		char * image,		/* the server image identifier  */
		char * flavor,		/* the server machine flavour	*/
		char * personality,	/* the source personality data	*/
		char * resource )	/* the target personality file  */
{
	char *	sptr=(char *) 0;
	char *	tptr=(char *) 0;
	char *	kptr=(char *) 0;
	char *	dptr=(char *) 0;

	char	encoded[8192];
	char *	eptr=encoded;
	char *	filename;
	FILE *	h;
	int	bytes;
	struct	rest_header * hptr;
	if ( hack )
		return( euca_data_hack( image, flavor, personality ) );
	if (!( hptr = os_authenticate() ))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		/* ---------------------------------------- */
		/* generate server creation request element */
		/* ---------------------------------------- */
		fprintf(h,"<server xmlns=%c%s%c\n",0x0022,Os.namespace,0x0022);
		if (!( strcmp( Os.version, "v1.0" ) ))
		{
			fprintf(h,"\timageId=%c%s%c\n",0x0022,image,0x0022);
			fprintf(h,"\tflavorId=%c%s%c\n",0x0022,flavor,0x0022);
		}
		else if (!( strcmp( Os.version, "v1.1" ) ))
		{
			fprintf(h,"\timageRef=%c%s/images/%s%c\n",0x0022,Os.base,image,0x0022);
			fprintf(h,"\tflavorRef=%c%s/flavors/%s%c\n",0x0022,Os.base,flavor,0x0022);
		}
		fprintf(h,"\tname=%c%s%c >\n",0x0022,identity,0x0022);

		/* ----------------------------- */
		/* generate meta data statements */
		/* ----------------------------- */
		fprintf(h,"\t<metadata>\n");
		fprintf(h,"\t\t<meta key='ServerName'>%s</meta>\n",identity);
		if ( personality )
		{
			if (( sptr = allocate_string( personality )) != (char *) 0)
			{
				tptr = sptr;
				while ( os_parse_metadata( &sptr, &kptr, &dptr ) != 0)
				{
					fprintf(h,"\t\t<meta key='%s'>%s</meta>\n",kptr,dptr);
				}
				liberate(tptr );
			}
		}
		fprintf(h,"\t</metadata>\n");

		/* ---------------------------- */
		/* generate personality section */
		/* ---------------------------- */
		if (( personality ) && ( resource ))
		{
			if (( strlen( personality ) )
			&&  ( EncodeBase64( eptr, personality, strlen(personality) ) > 0 ))
			{
				fprintf(h,"<personality>\n");
				fprintf(h,"<file path=%c%s%c>\n",0x0022,resource,0x0022);
				bytes = 0;
				while ( *eptr != 0 )
				{
					if (( bytes ) && (!( bytes % 48 )))
					{
						fprintf(h,"\n");
						bytes = 0;
					}
					fprintf(h,"%c",*(eptr++));
					bytes++;
				}
				if ( bytes ) fprintf(h,"\n");
				fprintf(h,"</file>\n");
				fprintf(h,"</personality>\n");
			}
		}
		fprintf(h,"</server>\n");
		fclose(h);
		return( filename );
	}
}

/*	------------------------------------------------------------	*/
/*		o s _ c r e a te _  i m a g e _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	char * os_create_image_request(char * identity, char * server )
{
	char *	filename;
	FILE *	h;

	struct	rest_header * hptr;
	if (!( hptr = os_authenticate() ))
		return((char *) 0);
	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		if (!( strcmp( Os.version, "v1.0" ) ))
		{
			fprintf(h,"<image xmlns=%c%s%c\n",0x0022,Os.namespace,0x0022);
			fprintf(h,"\tname=%c%s%c\n",0x0022,identity,0x0022);
			fprintf(h,"\tserverId=%c%s%c />\n",0x0022,server,0x0022);
		}
		else if (!( strcmp( Os.version, "v1.1" ) ))
		{
			fprintf(h,"<createImage xmlns=%c%s%c\n",0x0022,Os.namespace,0x0022);
			fprintf(h,"\tname=%c%s%c\n>",0x0022,identity,0x0022);
			fprintf(h,"\t<metadata>\n");
			fprintf(h,"\t\t<meta key=%cactionAgent%c>OS Client</meta>\n",0x0022,0x0022);
			fprintf(h,"\t\t<meta key=%ctimeStamp%c>%u</meta>\n",0x0022,0x0022,time((long *) 0));
			fprintf(h,"\t\t<meta key=%coriginServer%c>%s</meta>\n",0x0022,0x0022,server);
			fprintf(h,"\t</metadata>\n");
			fprintf(h,"</createImage>\n");
		}
		fclose(h);
		return( filename );
	}
}

/*	------------------------------------------------------------	*/
/*			o s _ c r e a t e _  i m a g e   		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_create_image( char * filename, char * id )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	if (!( strcmp( Os.version, "v1.0" ) ))
		sprintf(buffer,"/images");
	else if (!( strcmp( Os.version, "v1.1" ) ))
		sprintf(buffer,"/servers/%s/action",id);
	else	return( rptr );

	if (!( hptr = os_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_post_request( nptr, Os.tls, Os.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			o s _ c r e a t e _  s e r v e r 		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_create_server( char * filename )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	if ( hack )
		return( os_check( euca_command_hack( filename ) ) );
	sprintf(buffer,"/servers");
	if (!( hptr = os_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_post_request( nptr, Os.tls, Os.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			o s _ c r e a t e _  m e t a d a t a		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_create_metadata( char * id, char * filename )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/servers/%s/metadata",id);
	if (!( hptr = os_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_post_request( nptr, Os.tls, Os.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

	
/*	------------------------------------------------------------	*/
/*			o s _ l i s t  _ m e t a d a t a		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_list_metadata	(  char * id )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/servers/%s/metadata",id);
	if (!( hptr = os_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, Os.tls, Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			o s _ g e t _ s e r v e r 			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_get_server	(  char * id )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/servers/%s",id);
	if (!( hptr = os_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, Os.tls, Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			o s _ g e t _ f l av o u r			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_get_flavor(  char * id )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/flavors/%s",id);
	if (!( hptr = os_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, Os.tls, Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			o s _ g e t _ i m a g e 			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_get_image 	(  char * id )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/images/%s",id);
	if (!( hptr = os_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, Os.tls, Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			o s _ u p d a t e _ s e r v e r 		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_update_server(  char * id, char * filename )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/servers/%s",id);
	if (!( hptr = os_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_put_request( nptr, Os.tls, Os.agent,filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			o s _ d e l e t e _ s e r v e r 		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_delete_server(  char * id )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/servers/%s",id);
	if (!( hptr = os_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_delete_request( nptr, Os.tls, Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			o s _ d e l e t e _ i m a g e 			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_delete_image	(  char * id )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/images/%s",id);
	if (!( hptr = os_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, Os.tls, Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		o s _ i n i t i a l i s e _ c l i e n t 		*/
/*	------------------------------------------------------------	*/
public	int	os_initialise_client( 
		char * user, char * password, 
		char * host, char * agent, char * version, char * tls )
{
	char	*	eptr;
	if ((eptr = getenv("EUCAHACK")) != (char *) 0)
		hack = atoi(eptr);
	else	hack = 0;
	if (!( Os.user = allocate_string( user )))
		return( 27 );
	if (!( Os.password = allocate_string( password )))
		return( 27 );
	if (!( Os.host = allocate_string( host )))
		return( 27 );
	else if (!( Os.agent = allocate_string( agent )))
		return( 27 );
	else if (!( Os.version = allocate_string( version )))
		return( 27 );

	/* namespace selection */
	if (!( strcmp( Os.version, "v1.0" ) ))
	{
		if (!( Os.namespace = allocate_string(  _OS_NS_COMPUTE_V10 ) ))
			return( 27 );
	}
	else if (!( strcmp( Os.version, "v1.1" ) ))
	{
	     if (!( Os.namespace = allocate_string(  _OS_NS_COMPUTE_V11 ) ))
		return( 27 );
	}
	else	return( 55 );

	Os.authenticate= (char *) 0;
	if (!( tls ))
		Os.tls = (char *) 0;
	else if ((Os.tls = allocate_string(tls)) != (char *) 0)
		if ( (!( strlen( Os.tls ) )) || ( *(Os.tls) == '0' ) )
			Os.tls = liberate( Os.tls );

	return( 0 );
}



#endif	/* _os_client_c */
	/* ------------ */



