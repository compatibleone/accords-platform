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

/* -------------------------- */
/* user management interfaces */
/* -------------------------- */


/*	-------------------------------		*/
/*	o r g a _ c r e a t e _ u s e r		*/
/*	-------------------------------		*/
public	int orga_create_user  ( struct orga_subscription * sptr, struct orga_user * uptr )
{
	return( 30 );
}

/*	-------------------------------		*/
/*	o r g a _ u p d a t e _ u s e r		*/
/*	-------------------------------		*/
public	int orga_update_user  ( struct orga_subscription * sptr, struct orga_user * uptr )
{
	return( 30 );
}

/*	-----------------------------------	*/
/*	o r g a _ r e t r i e v e _ u s e r	*/
/*	-----------------------------------	*/
public	int orga_retrieve_user( struct orga_subscription * sptr, struct orga_user * uptr )
{
	return( 30 );
}

/*	-------------------------------		*/
/*	o r g a _ d e l e t e _ u s e r		*/
/*	-------------------------------		*/
public	int orga_delete_user  ( struct orga_subscription * sptr, struct orga_user * uptr )
{
	return( 30 );
}


/* ----------------------------- */
/* account management interfaces */
/* ----------------------------- */


/*	-------------------------------------		*/
/*	o r g a _ c r e a t e _ a c c o u n t	 	*/
/*	-------------------------------------		*/
public	int orga_create_account  ( struct orga_subscription * sptr, struct orga_account * aptr )
{
	return( 30 );
}

/*	-------------------------------------		*/
/*	o r g a _ u p d a t e _ a c c o u n t	 	*/
/*	-------------------------------------		*/
public	int orga_update_account  ( struct orga_subscription * sptr, struct orga_account * aptr )
{
	return( 30 );
}

/*	-----------------------------------------	*/
/*	o r g a _ r e t r i e v e _ a c c o u n t	*/
/*	-----------------------------------------	*/
public	int orga_retrieve_account( struct orga_subscription * sptr, struct orga_account * aptr )
{
	return( 30 );
}

/*	-------------------------------------		*/
/*	o r g a _ d e l e t e _ a c c o u n t	 	*/
/*	-------------------------------------		*/
public	int orga_delete_account  ( struct orga_subscription * sptr, struct orga_account * aptr )
{
	return( 30 );
}


/* --------------------------------- */
/* transaction management interfaces */
/* --------------------------------- */


/*	---------------------------------------------		*/
/*	o r g a _ c r e a t e _ t r a n s a c t i o n 		*/
/*	---------------------------------------------		*/
public	int orga_create_transaction  ( struct orga_subscription * sptr, struct orga_transaction * tptr )
{
	return( 30 );
}

/*	---------------------------------------------		*/
/*	o r g a _ u p d a t e _ t r a n s a c t i o n 		*/
/*	---------------------------------------------		*/
public	int orga_update_transaction  ( struct orga_subscription * sptr, struct orga_transaction * tptr )
{
	return( 30 );
}

/*	-------------------------------------------------	*/
/*	o r g a _ r e t r i e v e _ t r a n s a c t i o n 	*/
/*	-------------------------------------------------	*/
public	int orga_retrieve_transaction( struct orga_subscription * sptr, struct orga_transaction * tptr )
{
	return( 30 );
}

/*	---------------------------------------------		*/
/*	o r g a _ d e l e t e _ t r a n s a c t i o n 		*/
/*	---------------------------------------------		*/
public	int orga_delete_transaction  ( struct orga_subscription * sptr, struct orga_transaction * tptr )
{
	return( 30 );
}

	/* ------------- */
#endif 	/* _orgaclient_c */
	/* ------------- */
