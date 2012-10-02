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
#ifndef	_copsquantity_c
#define	_copsquantity_c

#include "quantity.h"
#include "quantity.c"
#include "occiquantity.c"

/*	---------------------------------------------------------	*/
/*		i n v o k e _ q u a n t i t y _ a c t i o n		*/
/*	---------------------------------------------------------	*/
private	int	invoke_quantity_action( struct cords_quantity * pptr, int action )
{
	struct	occi_response * zptr;
	struct	occi_response * zzptr;
	struct	occi_element  * eptr;
	int			result;
	int			value;
	char			buffer[128];

	if (!( pptr ))
		return(0);
	else if (!( rest_valid_string( pptr->quota )))
		return(0);
	else if (!( rest_valid_string( pptr->value )))
		return(0);
	else if (!( zptr = occi_simple_get( pptr->quota, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return(0);
	else
	{
		/* ----------------------------- */
		/* collect the appropriate value */
		/* ----------------------------- */
		if (!( action & 2 ))
		{
			if (!( eptr = occi_locate_element( zptr->first,"occi.quota.reserved") ))
			{
				zptr = occi_remove_response( zptr );
				return(0);
			}
		}
		else
		{
			if (!( eptr = occi_locate_element( zptr->first,"occi.quota.consumed") ))
			{
				zptr = occi_remove_response( zptr );
				return(0);
			}
		}

		/* ----------------------- */
		/* retrieve the two values */
		/* ----------------------- */
		value  = atoi( pptr->value );

		if ( rest_valid_string( eptr->value ) )
			result = atoi( eptr->value );
		else	result = 0;

		/* ----------------------- */
		/* calculate the new value */
		/* ----------------------- */
		if ( action & 1 )
			result += value;
		else if ( result >= value )
			result -= value;
		else	result  = 0;
		
		/* --------------------- */
		/* allocate occi element */
		/* --------------------- */
		sprintf( buffer,"%u",result);

		if ( eptr->value ) 
			eptr->value = liberate( eptr->value );		

		if (!( eptr->value = allocate_string( buffer ) ))
		{
			zptr = occi_remove_response( zptr );
			return(0);
		}
		else
		{
			/* --------------------- */
			/* update the instance	 */
			/* --------------------- */
			if ((zzptr = occi_simple_put( pptr->quota, zptr->first, _CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
				zzptr = occi_remove_response ( zzptr );
		}							

		/* ------------------------------ */
		/* handle an eventual transaction */
		/* ------------------------------ */
		if (!( action & 2 ))
		{
			zptr = occi_remove_response( zptr );
			return( 0 );
		}
		else if (!( eptr = occi_locate_element( zptr->first, "occi.quota.price" ) ))
		{
			zptr = occi_remove_response( zptr );
			return(0);
		}
		else if (!( rest_valid_string( eptr->value ) ))
		{
			zptr = occi_remove_response( zptr );
			return(0);
		}
		else
		{
			occi_send_transaction( 
				_CORDS_QUANTITY, eptr->value, 
				( action & 1 ? "action=start" : "action=stop" ),
				pptr->account, pptr->id );
			zptr = occi_remove_response( zptr );
			return( 0 );
		}
	}
}

/*	-------------------------------------------	*/
/* 	    c o n s u m e  _ q u a n t i t y 		*/
/*	-------------------------------------------	*/
/*	the quantity has been provisioned and is	*/
/*	now active and deployed.			*/
/*	-------------------------------------------	*/
private	struct rest_response * consume_quantity(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_quantity * pptr;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Incorrect Message Category" ) );
	else if ( pptr->state != 1 )
		return( rest_html_response( aptr, 200, "OK" ) );
	else 
	{
		pptr->state=2;
		autosave_cords_quantity_nodes();
		invoke_quantity_action(pptr,3);
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}
	
/*	-------------------------------------------	*/
/* 	    r e s t o r e  _ q u a n t i t y 		*/
/*	-------------------------------------------	*/
/*	the quantity is reserved but no longer is	*/
/*	provisioned and deployed.			*/
/*	-------------------------------------------	*/
private	struct rest_response * restore_quantity(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_quantity * pptr;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Incorrect Message Category" ) );
	else if ( pptr->state != 2 )
		return( rest_html_response( aptr, 200, "OK" ) );
	else 
	{
		pptr->state=1;
		autosave_cords_quantity_nodes();
		invoke_quantity_action(pptr,2);
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}
	
/*	-------------------------------------------	*/
/* 	    r e l e a s e  _ q u a n t i t y 		*/
/*	-------------------------------------------	*/
/*	the quantity is no longer reserved		*/
/*	-------------------------------------------	*/
private	struct rest_response * release_quantity(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_quantity * pptr;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Incorrect Message Category" ) );
	else if (!( pptr->state ))
		return( rest_html_response( aptr, 200, "OK" ) );
	else 
	{
		pptr->state=0;
		autosave_cords_quantity_nodes();
		invoke_quantity_action(pptr,0);
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}
	
/*	-------------------------------------------	*/
/* 	      r e s e r v e _ q u a n t i t y 		*/
/*	-------------------------------------------	*/
private	struct rest_response * reserve_quantity(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_quantity * pptr;
	int	status;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Incorrect Message Category" ) );
	else if ( pptr->state > 0 )
		return( rest_html_response( aptr, 200, "OK" ) );
	else 
	{
		pptr->state = 1;
		autosave_cords_quantity_nodes();
		invoke_quantity_action(pptr,1);
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

	/* --------------- */
#endif	/* _copsquantity_c */
	/* --------------- */


