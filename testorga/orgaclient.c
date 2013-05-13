#ifndef	_orgaclient_c
#define _orgaclient_c


#include "orgaclient.h"


/* ----------------------- */
/* subscription interfaces */
/* ----------------------- */

public	struct orga_subscription * orga_initialise_subscription( char * account )
{
	return((struct orga_subscription *) 0);
}

public	struct orga_subscription * orga_liberate_subscription( struct orga_subscription * sptr )
{
	return((struct orga_subscription *) 0);
}


private	struct rest_response *  orga_post_request( char * url )
{
	return( (struct rest_response *) 0 );
}

private	struct rest_response *  orga_get_request( char * url )
{
	return( (struct rest_response *) 0 );
}

private	struct rest_response *  orga_put_request( char * url )
{
	return( (struct rest_response *) 0 );
}

private	struct rest_response *  orga_delete_request( char * url )
{
	return( (struct rest_response *) 0 );
}

/* -------------------------- */
/* user management interfaces */
/* -------------------------- */


/*	-------------------------------		*/
/*	o r g a _ c r e a t e _ u s e r		*/
/*	-------------------------------		*/
public struct rest_response *  orga_create_user  ( struct orga_subscription * sptr, struct orga_user * uptr )
{
	char	buffer[2048];
	sprintf( buffer,"%s/%s/customerProfile",sptr->base,sptr->version);
	return( orga_post_request( buffer ) );
}

/*	-------------------------------		*/
/*	o r g a _ u p d a t e _ u s e r		*/
/*	-------------------------------		*/
public struct rest_response *  orga_update_user  ( struct orga_subscription * sptr, struct orga_user * uptr )
{
	char	buffer[2048];
	sprintf( buffer,"%s/%s/customerProfile/%s",sptr->base,sptr->version,uptr->id);
	return( orga_put_request( buffer ) );
}

/*	-----------------------------------	*/
/*	o r g a _ r e t r i e v e _ u s e r	*/
/*	-----------------------------------	*/
public struct rest_response *  orga_retrieve_user( struct orga_subscription * sptr, struct orga_user * uptr )
{
	char	buffer[2048];
	sprintf( buffer,"%s/%s/customerProfile/%s",sptr->base,sptr->version,uptr->id);
	return( orga_get_request( buffer ) );
}

/*	---------------------------	*/
/*	o r g a _ l i s t _ u s e r	*/
/*	---------------------------	*/
public struct rest_response *  orga_list_user( struct orga_subscription * sptr, struct orga_user * uptr )
{
	char	buffer[2048];
	sprintf( buffer,"%s/%s/customerProfile",sptr->base,sptr->version);
	return( orga_get_request( buffer ) );
}

/*	-------------------------------		*/
/*	o r g a _ d e l e t e _ u s e r		*/
/*	-------------------------------		*/
public struct rest_response *  orga_delete_user  ( struct orga_subscription * sptr, struct orga_user * uptr )
{
	char	buffer[2048];
	sprintf( buffer,"%s/%s/customerProfile/%s",sptr->base,sptr->version,uptr->id);
	return( orga_delete_request( buffer ) );
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
	sprintf( buffer,"%s/%s/account",sptr->base,sptr->version);
	return( orga_post_request( buffer ) );
}

/*	-------------------------------------		*/
/*	o r g a _ u p d a t e _ a c c o u n t	 	*/
/*	-------------------------------------		*/
public struct rest_response *  orga_update_account  ( struct orga_subscription * sptr, struct orga_account * aptr )
{
	char	buffer[2048];
	sprintf( buffer,"%s/%s/account/%s",sptr->base,sptr->version,aptr->id);
	return( orga_put_request( buffer ) );
}

/*	-----------------------------------------	*/
/*	o r g a _ r e t r i e v e _ a c c o u n t	*/
/*	-----------------------------------------	*/
public struct rest_response *  orga_retrieve_account( struct orga_subscription * sptr, struct orga_account * aptr )
{
	char	buffer[2048];
	sprintf( buffer,"%s/%s/account/%s",sptr->base,sptr->version,aptr->id);
	return( orga_get_request( buffer ) );
}

/*	---------------------------------	*/
/*	o r g a _ l i s t _ a c c o u n t	*/
/*	---------------------------------	*/
public struct rest_response *  orga_list_account( struct orga_subscription * sptr, struct orga_account * aptr )
{
	char	buffer[2048];
	sprintf( buffer,"%s/%s/account",sptr->base,sptr->version);
	return( orga_get_request( buffer ) );
}

/*	-------------------------------------		*/
/*	o r g a _ d e l e t e _ a c c o u n t	 	*/
/*	-------------------------------------		*/
public struct rest_response *  orga_delete_account  ( struct orga_subscription * sptr, struct orga_account * aptr )
{
	char	buffer[2048];
	sprintf( buffer,"%s/%s/account/%s",sptr->base,sptr->version,aptr->id);
	return( orga_delete_request( buffer ) );
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
	sprintf( buffer,"%s/%s/payment",sptr->base,sptr->version);
	return( orga_delete_request( buffer ) );
}

/*	---------------------------------------------		*/
/*	o r g a _ u p d a t e _ t r a n s a c t i o n 		*/
/*	---------------------------------------------		*/
public struct rest_response *  orga_update_transaction  ( struct orga_subscription * sptr, struct orga_transaction * tptr )
{
	char	buffer[2048];
	sprintf( buffer,"%s/%s/payment/%s",sptr->base,sptr->version,tptr->id);
	return( orga_put_request( buffer ) );
}

/*	-------------------------------------------------	*/
/*	o r g a _ r e t r i e v e _ t r a n s a c t i o n 	*/
/*	-------------------------------------------------	*/
public struct rest_response *  orga_retrieve_transaction( struct orga_subscription * sptr, struct orga_transaction * tptr )
{
	char	buffer[2048];
	sprintf( buffer,"%s/%s/payment/%s",sptr->base,sptr->version,tptr->id);
	return( orga_delete_request( buffer ) );
}

/*	-----------------------------------------	*/
/*	o r g a _ l i s t _ t r a n s a c t i o n 	*/
/*	-----------------------------------------	*/
public struct rest_response *  orga_list_transaction( struct orga_subscription * sptr, struct orga_transaction * tptr )
{
	char	buffer[2048];
	sprintf( buffer,"%s/%s/payment",sptr->base,sptr->version);
	return( orga_delete_request( buffer ) );
}

/*	---------------------------------------------		*/
/*	o r g a _ d e l e t e _ t r a n s a c t i o n 		*/
/*	---------------------------------------------		*/
public struct rest_response *  orga_delete_transaction  ( struct orga_subscription * sptr, struct orga_transaction * tptr )
{
	char	buffer[2048];
	sprintf( buffer,"%s/%s/payment/%s",sptr->base,sptr->version,tptr->id);
	return( orga_delete_request( buffer ) );
}

	/* ------------- */
#endif 	/* _orgaclient_c */
	/* ------------- */
