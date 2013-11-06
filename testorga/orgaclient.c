#ifndef	_orgaclient_c
#define _orgaclient_c

#include "orgaclient.h"
#include "orgaconfig.c"

/* ----------------------- */
/* subscription interfaces */
/* ----------------------- */
static	int	orgaReferenceCode=0;

static	int	orga_reference_code()
{
	return( ++orgaReferenceCode );
}	


/*	-------------------------------------------------------		*/
/*	  o r g a _ l i b e r a t e _ s u b s c r i p t i o n		*/
/*	-------------------------------------------------------		*/
public	struct orga_subscription * orga_liberate_subscription( struct orga_subscription * sptr )
{
	if ( sptr )
	{
		if ( sptr->endUserId )
			sptr->endUserId = liberate( sptr->endUserId );
		if ( sptr->tls )
			sptr->tls = liberate( sptr->tls );
		if ( sptr->password )
			sptr->password = liberate( sptr->password );
		if ( sptr->host )
			sptr->host = liberate( sptr->host );
		if ( sptr->provider )
			sptr->provider = liberate( sptr->provider );
		if ( sptr->operator )
			sptr->operator = liberate( sptr->operator );
		if ( sptr->base )
			sptr->base = liberate( sptr->base );
		if ( sptr->version )
			sptr->version = liberate( sptr->version );
		liberate( sptr );
	}
	return((struct orga_subscription *) 0);
}

/*	-------------------------------------------------------		*/
/*	o r g a _ i n i t i a l i s e _ s u b s c r i p t i o n		*/
/*	-------------------------------------------------------		*/
public	struct orga_subscription * orga_initialise_subscription( char * host, char * account, char * password, char * operator, char * provider, char * version, char * tls )
{
	char	buffer[2048];
	struct	orga_subscription * sptr;
	if (!( sptr = allocate( sizeof (struct orga_subscription) ) ))
		return( sptr );
	else
	{
		memset( sptr, 0, sizeof (struct orga_subscription));
		if (!( rest_valid_string( tls ) ))
			sprintf(buffer,"http://%s",(host ? host : "127.0.0.1:8080"));
		else	sprintf(buffer,"https://%s",(host ? host : "127.0.0.1:8080"));
		if (!( sptr->host = allocate_string( host ) ))
			return( orga_liberate_subscription( sptr ) );
		if (!( sptr->base = allocate_string( buffer ) ))
			return( orga_liberate_subscription( sptr ) );
		if (!( sptr->endUserId = allocate_string( account ) ))
			return( orga_liberate_subscription( sptr ) );
		if (!( sptr->password = allocate_string( password ) ))
			return( orga_liberate_subscription( sptr ) );
		if (!( sptr->provider = allocate_string( provider ) ))
			return( orga_liberate_subscription( sptr ) );
		if (!( sptr->operator = allocate_string( operator ) ))
			return( orga_liberate_subscription( sptr ) );
		if (!( sptr->version = allocate_string( version ) ))
			return( orga_liberate_subscription( sptr ) );
		else if (!( rest_valid_string( tls ) ))
			return( sptr );
		else if (!( sptr->tls = allocate_string( tls ) ))
			return( orga_liberate_subscription( sptr ) );
		else	return( sptr );
	}
}

/*	-----------------------------------------	*/
/*	 o r g a _ d e f a u l t _ h e a d e r s	*/
/*	-----------------------------------------	*/
private	struct	rest_header * orga_default_headers( struct orga_subscription * sptr )
{
	return( rest_create_header( _HTTP_ACCEPT, "application/json" ) );
}

/*	-----------------------------------------	*/
/*	 o r g a _ d e f a u l t _ h e a d e r s	*/
/*	-----------------------------------------	*/
private	struct	rest_header * orga_json_headers( struct orga_subscription * sptr )
{
	return( orga_default_headers( sptr ) );
}

/*	---------------------------------	*/
/*	o r g a _ p o s t _ r e q u e s t	*/
/*	---------------------------------	*/
private	struct rest_response *  orga_post_request(struct orga_subscription * sptr,  char * url, char * filename )
{
	struct	rest_header * hptr;
	if (!( hptr = orga_json_headers( sptr ) ))
		return( (struct rest_response *) 0 );
	else	return( rest_client_post_request( url, sptr->tls, get_default_agent(), filename, hptr ));
}

/*	---------------------------------	*/
/*	 o r g a _ g e t _ r e q u e s t	*/
/*	---------------------------------	*/
private	struct rest_response *  orga_get_request(struct orga_subscription * sptr,  char * url, char * filename )
{
	struct	rest_header * hptr;
	if (!( hptr = orga_json_headers( sptr ) ))
		return( (struct rest_response *) 0 );
	if (!( rest_valid_string( filename ) ))
		return( rest_client_get_request( url, sptr->tls, get_default_agent(), hptr ));
	else	return( rest_client_get_request_body( url, sptr->tls, get_default_agent(), hptr, filename ));
}

/*	---------------------------------	*/
/*	 o r g a _ p u t _ r e q u e s t	*/
/*	---------------------------------	*/
private	struct rest_response *  orga_put_request(struct orga_subscription * sptr, char * url, char * filename )
{
	struct	rest_header * hptr;
	if (!( hptr = orga_json_headers( sptr ) ))
		return( (struct rest_response *) 0 );
	else	return( rest_client_put_request( url, sptr->tls, get_default_agent(), filename, hptr ));
}

/*	----------------------------------------	*/
/*	  o r g a _ d e l e t e _ r e q u e s t 	*/
/*	----------------------------------------	*/
private	struct rest_response *  orga_delete_request(struct orga_subscription * sptr, char * url, char * filename )
{
	struct	rest_header * hptr;
	if (!( hptr = orga_json_headers( sptr ) ))
		return( (struct rest_response *) 0 );
	else if (!( rest_valid_string( filename ) ))
		return( rest_client_delete_request( url, sptr->tls, get_default_agent(), hptr ));
	else	return( rest_client_delete_request_body( url, sptr->tls, get_default_agent(), hptr, filename ));
}

/* -------------------------- */
/* user management interfaces */
/* -------------------------- */

/*	----------------------------------------------------	*/
/*	these interfaces allow end user customer accounts to	*/
/*	be created, updated, retrieved, listed and destroyed	*/
/*	----------------------------------------------------	*/

/*	-------------------------------		*/
/*	o r g a _ c r e a t e _ u s e r		*/
/*	-------------------------------		*/
public struct rest_response *  orga_create_user  ( struct orga_subscription * sptr, struct cords_account * uptr, char * nature )
{
	char	buffer[2048];
	char *	filename=(char *) 0;
	FILE *	h;
	char *	euid=(char *) 0;
	if (!( filename = rest_temporary_filename( "json" ) ))
		return((struct rest_response *) 0);
	else if (!( h = fopen(filename,"w") ))
		return((struct rest_response *) 0);
	else
	{
		if (!( strcmp( nature, "Customer" ) ))
			euid = sptr->provider;
		else 	euid = sptr->operator;
		fprintf(h,"{\n");
		fprintf(h,"\"customerNumber\":\"%s\"",uptr->id);
		fprintf(h,",\"referenceCode\":\"ORGABAS-REF-%u\"\n",orga_reference_code()); 
		fprintf(h,",\"transactionOperationStatus\":\"%s\"\n","Registered"); 
		fprintf(h,",\"customerProfile\":{\n"); 
		fprintf(h,",\"customerName\":\"%s\"\n",uptr->name); 
		fprintf(h,",\"baasRoleType\":\"%s\"\n",nature); 
		fprintf(h,",\"isActive\":\"true\"\n");
		fprintf(h,"}\n}\n");
		fclose(h);
	}
	sprintf( buffer,"%s/customerProfile/%s",sptr->base,euid);
	if ( check_verbose() ) { printf("ORGA CLIENT: CREATE USER:\n\turl=%s\n\tfile=%s\n",buffer,filename); }
	return( orga_post_request(sptr, buffer, filename  ) );
}

/*	-------------------------------		*/
/*	o r g a _ u p d a t e _ u s e r		*/
/*	-------------------------------		*/
public struct rest_response *  orga_update_user  ( struct orga_subscription * sptr, struct cords_account * uptr, char * nature )
{
	char	buffer[2048];
	char *	filename=(char *) 0;
	FILE *	h;
	char *	euid;
	if (!( filename = rest_temporary_filename( "json" ) ))
		return((struct rest_response *) 0);
	else if (!( h = fopen(filename,"w") ))
		return((struct rest_response *) 0);
	else
	{
		if (!( strcmp( nature, "Customer" ) ))
			euid = sptr->provider;
		else 	euid = sptr->operator;
		fprintf(h,"{\n");
		fprintf(h,"\"customerNumber\":\"%s\"",uptr->id);
		fprintf(h,",\"referenceCode\":\"ORGABAS-REF-%u\"\n",orga_reference_code()); 
		fprintf(h,",\"transactionOperationStatus\":\"%s\"\n","Updated"); 
		fprintf(h,",\"customerProfile\":{\n"); 
		fprintf(h,",\"customerName\":\"%s\"\n",uptr->name); 
		fprintf(h,",\"baasRoleType\":\"%s\"\n",nature); 
		fprintf(h,",\"isActive\":\"true\"\n");
		fprintf(h,"}\n}\n");
		fclose(h);
	}
	sprintf( buffer,"%s/customerProfile/%s",sptr->base,euid);
	if ( check_verbose() ) { printf("ORGA CLIENT: UPDATE USER:\n\turl=%s\n\tfile=%s\n",buffer,filename); }
	return( orga_put_request( sptr,  buffer, filename ) );
}

/*	-----------------------------------	*/
/*	o r g a _ r e t r i e v e _ u s e r	*/
/*	-----------------------------------	*/
public struct rest_response *  orga_retrieve_user( struct orga_subscription * sptr, struct cords_account * uptr, char * nature )
{
	char	buffer[2048];
	char *	euid;
	if (!( strcmp( nature, "Customer" ) ))
		euid = sptr->provider;
	else 	euid = sptr->operator;
	sprintf( buffer,"%s/customerProfile/%s/%s",sptr->base,euid,uptr->id);
	if ( check_verbose() ) { printf("ORGA CLIENT: RETRIEVE USER:\n\turl=%s\n",buffer); }
	return( orga_get_request( sptr,  buffer, (char *) 0 ) );
}

/*	---------------------------	*/
/*	o r g a _ l i s t _ u s e r	*/
/*	---------------------------	*/
public struct rest_response *  orga_list_user( struct orga_subscription * sptr, struct cords_account * uptr )
{
	char	buffer[2048];
	sprintf( buffer,"%s/customerProfile",sptr->base);
	if ( check_verbose() ) { printf("ORGA CLIENT: LIST USER:\n\turl=%s\n",buffer); }
	return( orga_get_request( sptr,  buffer, (char *) 0 ) );
}

/*	-------------------------------		*/
/*	o r g a _ d e l e t e _ u s e r		*/
/*	-------------------------------		*/
public struct rest_response *  orga_delete_user  ( struct orga_subscription * sptr, struct cords_account * uptr , char * nature)
{
	char	buffer[2048];
	char *	filename=(char *) 0;
	char *	euid=(char *) 0;
	FILE *	h;
	if (!( filename = rest_temporary_filename( "json" ) ))
		return((struct rest_response *) 0);
	else if (!( h = fopen(filename,"w") ))
		return((struct rest_response *) 0);
	else
	{
		if (!( strcmp( nature, "Customer" ) ))
			euid = sptr->provider;
		else 	euid = sptr->operator;
		fprintf(h,"{\n");
		fprintf(h,"\"customerNumber\":\"%s\"",uptr->id);
		fprintf(h,",\"referenceCode\":\"ORGABAS-REF-%u\"\n",orga_reference_code()); 
		fprintf(h,",\"transactionOperationStatus\":\"%s\"\n","Deregistered"); 
		fprintf(h,"}\n");
		fclose(h);
	}
	sprintf( buffer,"%s/customerProfile/%s",sptr->base,euid);
	if ( check_verbose() ) { printf("ORGA CLIENT: DELETE USER:\n\turl=%s\n\tfile=%s\n",buffer,filename); }
	return( orga_post_request( sptr,  buffer, filename ) );
}


/* ----------------------------- */
/* account management interfaces */
/* ----------------------------- */


/*	-------------------------------------		*/
/*	o r g a _ c r e a t e _ a c c o u n t	 	*/
/*	-------------------------------------		*/
public struct rest_response *  orga_create_account  ( struct orga_subscription * sptr, struct orga_account * aptr )
{
	char	buffer[2048];
	char *	filename=(char *) 0;
	FILE *	h;
	if (!( filename = rest_temporary_filename( "json" ) ))
		return((struct rest_response *) 0);
	else if (!( h = fopen(filename,"w") ))
		return((struct rest_response *) 0);
	else
	{
		fprintf(h,"{\n");
		fprintf(h,"\"customerAccount\":{\n"); 

		fprintf(h,"}\n}\n");
		fclose(h);
	}
	sprintf( buffer,"%s/account",sptr->base);
	return( orga_post_request( sptr,  buffer,filename ) );
}

/*	-------------------------------------		*/
/*	o r g a _ u p d a t e _ a c c o u n t	 	*/
/*	-------------------------------------		*/
public struct rest_response *  orga_update_account  ( struct orga_subscription * sptr, struct orga_account * aptr )
{
	char	buffer[2048];
	char *	filename=(char *) 0;
	FILE *	h;
	if (!( filename = rest_temporary_filename( "json" ) ))
		return((struct rest_response *) 0);
	else if (!( h = fopen(filename,"w") ))
		return((struct rest_response *) 0);
	else
	{
		fprintf(h,"{\n");
		fprintf(h,"\"customerAccount\":{\n"); 

		fprintf(h,"}\n}\n");
		fclose(h);
	}
	sprintf( buffer,"%s/account/%s",sptr->base,aptr->id);
	return( orga_put_request( sptr,  buffer,filename ) );
}

/*	-----------------------------------------	*/
/*	o r g a _ r e t r i e v e _ a c c o u n t	*/
/*	-----------------------------------------	*/
public struct rest_response *  orga_retrieve_account( struct orga_subscription * sptr, struct orga_account * aptr )
{
	char	buffer[2048];
	sprintf( buffer,"%s/account/%s",sptr->base,aptr->id);
	return( orga_get_request( sptr,buffer, (char *) 0 ) );
}

/*	---------------------------------	*/
/*	o r g a _ l i s t _ a c c o u n t	*/
/*	---------------------------------	*/
public struct rest_response *  orga_list_account( struct orga_subscription * sptr, struct orga_account * aptr )
{
	char	buffer[2048];
	sprintf( buffer,"%s/account",sptr->base);
	return( orga_get_request( sptr,buffer, (char *) 0 ) );
}

/*	-------------------------------------		*/
/*	o r g a _ d e l e t e _ a c c o u n t	 	*/
/*	-------------------------------------		*/
public struct rest_response *  orga_delete_account  ( struct orga_subscription * sptr, struct orga_account * aptr )
{
	char	buffer[2048];
	sprintf( buffer,"%s/account/%s",sptr->base,aptr->id);
	return( orga_delete_request( sptr,buffer, (char *) 0 ) );
}


/* --------------------------------- */
/* transaction management interfaces */
/* --------------------------------- */


/*	---------------------------------------------		*/
/*	o r g a _ c r e a t e _ t r a n s a c t i o n 		*/
/*	---------------------------------------------		*/
public struct rest_response *  orga_create_transaction  ( struct orga_subscription * sptr, struct orga_transaction * tptr )
{
	char	buffer[2048];
	char *	filename=(char *) 0;
	FILE *	h;
	if (!( filename = rest_temporary_filename( "json" ) ))
		return((struct rest_response *) 0);
	else if (!( h = fopen(filename,"w") ))
		return((struct rest_response *) 0);
	else
	{
		fprintf(h,"{\n");
		fprintf(h,"\"endUserId\":\"%s\"\n",sptr->provider); 
		fprintf(h,"\"onBehalfOf\":\"%s|%s\"\n",sptr->provider,tptr->account); 
		fprintf(h,",\"transactionOperationStatus\":\"%s\"\n","Charged"); 
		fprintf(h,",\"referenceCode\":\"ORGABAS-REF-%u\"\n",orga_reference_code()); 
		fprintf(h,",\"serviceId\":\"%s\"\n",tptr->label);
		fprintf(h,",\"amount\":\"%s\"\n",tptr->value);
		fprintf(h,",\"currency\":\"%s\"\n",tptr->currency);
		fprintf(h,",\"measures\":[\"%s\"]\n",tptr->measures);
		fprintf(h,",\"description\":\"charged %s\"\n",tptr->label);
		fprintf(h,"}\n");
		fclose(h);
	}
	sprintf( buffer,"%s/payment/%s/transactions/amount",sptr->base,sptr->provider);
	if ( check_verbose() ) { printf("ORGA CLIENT: CREATE TRANSACTION:\n\turl=%s\n\tfile=%s\n",buffer,filename); }
	return( orga_post_request( sptr,buffer,filename ) );
}

/*	---------------------------------------------		*/
/*	o r g a _ u p d a t e _ t r a n s a c t i o n 		*/
/*	---------------------------------------------		*/
public struct rest_response *  orga_update_transaction  ( struct orga_subscription * sptr, struct orga_transaction * tptr )
{
	char	buffer[2048];
	char *	filename=(char *) 0;
	FILE *	h;
	if (!( filename = rest_temporary_filename( "json" ) ))
		return((struct rest_response *) 0);
	else if (!( h = fopen(filename,"w") ))
		return((struct rest_response *) 0);
	else
	{
		fprintf(h,"{\n");
		fprintf(h,"\"endUserId\":\"%s\"\n",sptr->provider);
		fprintf(h,",\"onBehalfOf\":\"%s|%s\"\n",sptr->provider,tptr->account); 
		fprintf(h,",\"transactionOperationStatus\":\"%s\"\n","Adjusted"); 
		fprintf(h,",\"referenceCode\":\"ORGABAS-REF-%u\"\n",orga_reference_code()); 
		fprintf(h,",\"serviceId\":\"%s\"\n",tptr->label);
		fprintf(h,",\"ammount\":\"%s\"\n",tptr->value);
		fprintf(h,",\"currency\":\"%s\"\n",tptr->currency);
		fprintf(h,",\"measures\":[\"%s\"]\n",tptr->measures);
		fprintf(h,",\"description\":\"adjustment %s\"\n",tptr->label);
		fprintf(h,"}\n");
		fclose(h);
	}
	sprintf( buffer,"%s/payment/%s/transactions/amount",sptr->base,sptr->provider);
	if ( check_verbose() ) { printf("ORGA CLIENT: UPDATE TRANSACTION:\n\turl=%s\n\tfile=%s\n",buffer,filename); }
	return( orga_put_request( sptr,buffer,filename ) );
}

/*	-------------------------------------------------	*/
/*	o r g a _ r e t r i e v e _ t r a n s a c t i o n 	*/
/*	-------------------------------------------------	*/
public struct rest_response *  orga_retrieve_transaction( struct orga_subscription * sptr, struct orga_transaction * tptr )
{
	char	buffer[2048];
	sprintf( buffer,"%s/payment/%s/transactions/amount",sptr->base,sptr->provider);
	if ( check_verbose() ) { printf("ORGA CLIENT: RETRIEVE TRANSACTION:\n\turl=%s\n",buffer); }
	return( orga_get_request( sptr,buffer, (char *) 0 ) );
}

/*	-----------------------------------------	*/
/*	o r g a _ l i s t _ t r a n s a c t i o n 	*/
/*	-----------------------------------------	*/
public struct rest_response *  orga_list_transaction( struct orga_subscription * sptr, struct orga_transaction * tptr )
{
	char	buffer[2048];
	sprintf( buffer,"%s/payment/%s/transactions/amount",sptr->base,sptr->provider);
	if ( check_verbose() ) { printf("ORGA CLIENT: LIST TRANSACTION:\n\turl=%s\n",buffer); }
	return( orga_get_request( sptr,buffer, (char *) 0 ) );
}

/*	---------------------------------------------		*/
/*	o r g a _ d e l e t e _ t r a n s a c t i o n 		*/
/*	---------------------------------------------		*/
public struct rest_response *  orga_delete_transaction  ( struct orga_subscription * sptr, struct orga_transaction * tptr )
{
	char	buffer[2048];
	char *	filename=(char *) 0;
	FILE *	h;
	if (!( filename = rest_temporary_filename( "json" ) ))
		return((struct rest_response *) 0);
	else if (!( h = fopen(filename,"w") ))
		return((struct rest_response *) 0);
	else
	{
		fprintf(h,"{\n");
		fprintf(h,"\"endUserId\":\"%s\"\n",sptr->provider); 
		fprintf(h,"\"onBehalfOf\":\"%s|%s\"\n",sptr->provider,tptr->account); 
		fprintf(h,",\"transactionOperationStatus\":\"%s\"\n","Refunded"); 
		fprintf(h,",\"referenceCode\":\"ORGABAS-REF-%u\"\n",orga_reference_code()); 
		fprintf(h,",\"serviceId\":\"%s\"\n",tptr->label);
		fprintf(h,",\"amount\":\"%s\"\n",tptr->value);
		fprintf(h,",\"currency\":\"%s\"\n",tptr->currency);
		fprintf(h,",\"measures\":[\"%s\"]\n",tptr->measures);
		fprintf(h,",\"description\":\"deleted or refunded %s\"\n",tptr->label);
		fprintf(h,"\n");
		fclose(h);
	}
	sprintf( buffer,"%s/payment/%s/transactions/amount",sptr->base,sptr->provider);
	if ( check_verbose() ) { printf("ORGA CLIENT: DELETE TRANSACTION:\n\turl=%s\n\tfile=%s\n",buffer,filename); }
	return( orga_delete_request( sptr,buffer, filename ) );
}

	/* ------------- */
#endif 	/* _orgaclient_c */
	/* ------------- */

