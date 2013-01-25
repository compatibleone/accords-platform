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
#ifndef	_copsoperation_c
#define	_copsoperation_c

#include "cb.h"
#include "stdnode.h"

struct	cops_quota
{
	struct	cops_quota * previous;
	struct	cops_quota * next;
	char *	uuid;
	char *	reference;
	char *	property;
	int	quantity;
	int	state;
};

struct	cops_solution
{
	struct	cops_solution	* previous;
	struct	cops_solution	* next;
	char *	uuid;
	struct	occi_response 	* message;
	char *	technology;
	char *	identity;
	char *	zone;
	char *	security;
	char *	opinion;
	char *	energy;
	char *	price;
	struct	cops_quota 	* first;
	struct	cops_quota 	* last;
	int	match;
	int	chosen;
};

struct	cops_options
{
	char *	algorithm;	/* default algorithm		*/
	int	preference;	/* 0: round robin, 1: oldest	*/
	int	overbooking;	/* overbooking allowed 		*/
}	Coptions = 
{
	"quota:zone",
	0,
	0
};

/*	----------------------------------------------------------	*/
/*		l i b e r a t e _ c o p s _ q u o t a 			*/
/*	----------------------------------------------------------	*/
private	struct	cops_quota * liberate_cops_quota(struct	cops_quota * qptr)
{
	if ( qptr )
	{
		if ( qptr->uuid )
			qptr->uuid = liberate( qptr->uuid );
		if ( qptr->property )
			qptr->property = liberate( qptr->property );
		if ( qptr->reference )
			qptr->reference = liberate( qptr->reference );
		qptr = liberate( qptr );
	}
	return((struct cops_quota *) 0);
}

/*	----------------------------------------------------------	*/
/*		a l l o c a t e _ c o p s _ q u o t a 			*/
/*	----------------------------------------------------------	*/
private	struct	cops_quota * allocate_cops_quota()
{
	struct	cops_quota * qptr;
	if (!( qptr = allocate( sizeof( struct cops_quota ) ) ))
		return( qptr );
	else 
	{
		qptr->previous 	= 
		qptr->next 	= (struct cops_quota *) 0;
		qptr->reference =
		qptr->uuid 	= 
		qptr->property	= (char *) 0;
		qptr->quantity  = 0;
		return( qptr );
	}
}
	
/*	----------------------------------------------------------	*/
/*		l i b e r a t e _ c o p s _ s o l u t i o n		*/
/*	----------------------------------------------------------	*/
private struct	cops_solution * liberate_cops_solution(struct cops_solution * sptr)
{
	struct	cops_quota * qptr;
	if ( sptr )
	{
		if ( sptr->uuid )
			sptr->uuid = liberate( sptr->uuid );
		if ( sptr->identity )
			sptr->identity = liberate( sptr->identity );
		if ( sptr->security )
			sptr->security = liberate( sptr->security );
		if ( sptr->zone )
			sptr->zone = liberate( sptr->zone );
		if ( sptr->opinion )
			sptr->opinion = liberate( sptr->opinion );
		if ( sptr->price )
			sptr->price = liberate( sptr->price );
		if ( sptr->energy )
			sptr->energy = liberate( sptr->energy );
		if ( sptr->message )
			sptr->message = occi_remove_response( sptr->message );
		while ((qptr = sptr->first) != (struct cops_quota *) 0)
		{
			sptr->first = qptr->next;
			qptr = liberate_cops_quota( qptr );
		}
		sptr->first = sptr->last = (struct cops_quota *) 0;
		sptr = liberate( sptr );
	}
	return((struct cops_solution *) 0);
}

/*	----------------------------------------------------------	*/
/*		a l l o c a t e _ c o p s _ s o l u t i o n		*/
/*	----------------------------------------------------------	*/
private struct	cops_solution * allocate_cops_solution(char * uuid, struct occi_response * zptr)
{
	struct	occi_element  * eptr;
	struct	cops_solution * sptr;
	if (!( zptr ))
		return((struct cops_solution *) 0);
	else if (!( sptr = allocate( sizeof( struct cops_solution ) ) ))
		return( sptr );
	else 	memset( sptr, 0, sizeof( struct cops_solution ));

	sptr->first = sptr->last = (struct cops_quota *) 0;
	
	/* ----------------------------------------- */
	/* store the unique and universal identifier */
	/* ----------------------------------------- */
	if (!( sptr->uuid = allocate_string( uuid ) ))
		return( liberate_cops_solution( sptr ) );
	else	sptr->message = zptr;

	/* --------------------------------------- */
	/* retrieve and store identity information */
	/* --------------------------------------- */
	if ((( eptr = occi_locate_element( zptr->first, "occi.provider.identity" )) != (struct occi_element *) 0)
	&&   ( rest_valid_string( eptr->value ) )
	&&   (!( sptr->identity = allocate_string( eptr->value ) )))
		return( liberate_cops_solution( sptr ) );

	/* ----------------------------------- */
	/* retrieve and store zone information */
	/* ----------------------------------- */
	if ((( eptr = occi_locate_element( zptr->first, "occi.provider.zone" )) != (struct occi_element *) 0)
	&&   ( rest_valid_string( eptr->value ) )
	&&   (!( sptr->zone = allocate_string( eptr->value ) )))
		return( liberate_cops_solution( sptr ) );

	/* --------------------------------------- */
	/* retrieve and store security information */
	/* --------------------------------------- */
	if ((( eptr = occi_locate_element( zptr->first, "occi.provider.security" )) != (struct occi_element *) 0)
	&&   ( rest_valid_string( eptr->value ) )
	&&   (!( sptr->security = allocate_string( eptr->value ) )))
		return( liberate_cops_solution( sptr ) );

	/* ----------------------------------------- */
	/* retrieve and store opinion information */
	/* ----------------------------------------- */
	if ((( eptr = occi_locate_element( zptr->first, "occi.provider.opinion" )) != (struct occi_element *) 0)
	&&   ( rest_valid_string( eptr->value ) )
	&&   (!( sptr->opinion = allocate_string( eptr->value ) )))
		return( liberate_cops_solution( sptr ) );

	/* --------------------------------------- */
	/* retrieve and store category information */
	/* --------------------------------------- */
	if ((( eptr = occi_locate_element( zptr->first, "occi.provider.category" )) != (struct occi_element *) 0)
	&&   ( rest_valid_string( eptr->value ) )
	&&   (!( sptr->technology = allocate_string( eptr->value ) )))
		return( liberate_cops_solution( sptr ) );

	/* ------------------------------------ */
	/* retrieve and store price information */
	/* ------------------------------------ */
	if ((( eptr = occi_locate_element( zptr->first, "occi.provider.price" )) != (struct occi_element *) 0)
	&&   ( rest_valid_string( eptr->value ) )
	&&   (!( sptr->price = allocate_string( eptr->value ) )))
		return( liberate_cops_solution( sptr ) );

	/* ----------------------------------- */
	/* retrieve and store energy information */
	/* ----------------------------------- */
	if ((( eptr = occi_locate_element( zptr->first, "occi.provider.energy" )) != (struct occi_element *) 0)
	&&   ( rest_valid_string( eptr->value ) )
	&&   (!( sptr->energy = allocate_string( eptr->value ) )))
		return( liberate_cops_solution( sptr ) );

	/* ------------------------ */
	/* retrieve last usage date */
	/* ------------------------ */
	if ((( eptr = occi_locate_element( zptr->first, "occi.provider.energy" )) != (struct occi_element *) 0)
	&&   ( rest_valid_string( eptr->value ) ))
		sptr->chosen = atoi( eptr->value );

	return( sptr );
}

/*	----------------------------------------------------------	*/
/*		r e l e a s e _ c o p s _ s o l u t i o n		*/
/*	----------------------------------------------------------	*/
private	struct	cops_solution *	release_cops_solutions( struct cops_solution * first )
{
	struct	cops_solution * sptr;
	while ((sptr = first) != (struct cops_solution *) 0)
	{
		first = sptr->next;
		sptr = liberate_cops_solution( sptr );
	}
	return((struct cops_solution *) 0);
}

/*	----------------------------------------------------------	*/
/*		s e l e c t _ c o p s _ o l d e s t			*/
/*	----------------------------------------------------------	*/
private	struct	cops_solution *	select_cops_oldest( struct cops_solution * first )
{
	struct	cops_solution * rptr=(struct cops_solution *) 0;
	struct	cops_solution * sptr;
	int	oldest;

	oldest = time((long *) 0);

	for (	sptr=first;
		sptr != (struct cops_solution *) 0;
		sptr = sptr->next )
	{
		if (!( sptr->match ))
			continue;	
		else if ( sptr->chosen >= oldest )
			continue;
		else
		{
			oldest = sptr->chosen;
			rptr   = sptr;
			continue;
		}
	}
	return( rptr );
}

/*	----------------------------------------------------------	*/
/*		s e l e c t _ c o p s _ r e p u t a t i o n		*/
/*	----------------------------------------------------------	*/
private	struct	cops_solution *	select_cops_reputation( struct cops_solution * first )
{
	struct	cops_solution * rptr=(struct cops_solution *) 0;
	struct	cops_solution * sptr;
	int	best=0;

	for (	sptr=first;
		sptr != (struct cops_solution *) 0;
		sptr = sptr->next )
	{
		if (!( sptr->match ))
			continue;
		if (!( rest_valid_string( sptr->opinion ) ))
			continue;
		else if ( atoi(sptr->opinion) <= best )
			continue;
		else
		{
			best = atoi(sptr->opinion);
			rptr   = sptr;
			continue;
		}
	}
	return( rptr );
}

/*	----------------------------------------------------------	*/
/*		s e l e c t _ c o p s _ r e s u l t 			*/
/*	----------------------------------------------------------	*/
private	struct	cops_solution *	select_cops_result( struct cops_solution * first )
{
	if ( Coptions.preference & 1 )
		return( select_cops_reputation( first ) );
	else 	return( select_cops_oldest( first ) );
}

/*	----------------------------------------------------------	*/
/*		s e l e c t _ c o p s _ z o n e 			*/
/*	----------------------------------------------------------	*/
private	struct	cops_solution *	select_cops_zone( struct cords_placement * pptr, struct cops_solution * first )
{
	struct	cops_solution * sptr;
	int	items=0;
	if (!( pptr->zone ))
		return((struct cops_solution *) 0);

	for (	sptr=first;
		sptr != (struct cops_solution *) 0;
		sptr = sptr->next )
	{
		if (!( rest_valid_string( sptr->zone ) ))
			continue;
		else if (!( strcasecmp( sptr->zone, pptr->zone ) ))
			sptr->match=++items;
	}

	if (!( items ))
		return((struct cops_solution *) 0);
	else	return( select_cops_result( first ) );
}

/*	----------------------------------------------------------	*/
/*		s e l e c t _ c o p s _ s e c u r i t y 		*/
/*	----------------------------------------------------------	*/
private	struct	cops_solution *	select_cops_security( struct cords_placement * pptr, struct cops_solution * first )
{
	struct	cops_solution * sptr;
	int	items=0;

	if (!( pptr->security ))
		return((struct cops_solution *) 0);

	for (	sptr=first;
		sptr != (struct cops_solution *) 0;
		sptr = sptr->next )
	{
		if (!( rest_valid_string( sptr->security ) ))
			continue;
		else if (!( strcasecmp( sptr->security, pptr->security ) ))
			sptr->match=++items;
	}
	if (!( items ))
		return((struct cops_solution *) 0);
	else	return( select_cops_result( first ) );
}

/*	----------------------------------------------------------	*/
/*		s e l e c t _ c o p s _ e n e r g y 			*/
/*	----------------------------------------------------------	*/
private	struct	cops_solution *	select_cops_energy( struct cords_placement * pptr, struct cops_solution * first )
{
	struct	cops_solution * sptr;
	int	items=0;

	if (!( pptr->energy ))
		return((struct cops_solution *) 0);

	for (	sptr=first;
		sptr != (struct cops_solution *) 0;
		sptr = sptr->next )
	{
		if (!( rest_valid_string( sptr->energy ) ))
			continue;
		else if (!( strcasecmp( sptr->energy, pptr->energy ) ))
			sptr->match=++items;
	}
	if (!( items ))
		return((struct cops_solution *) 0);
	else	return( select_cops_result( first ) );
}

/*	----------------------------------------------------------	*/
/*		s e l e c t _ c o p s _ o p i n i o n			*/
/*	----------------------------------------------------------	*/
private	struct	cops_solution *	select_cops_opinion( struct cords_placement * pptr, struct cops_solution * first )
{
	struct	cops_solution * sptr;
	int	items=0;

	if (!( pptr->opinion ))
		return((struct cops_solution *) 0);

	for (	sptr=first;
		sptr != (struct cops_solution *) 0;
		sptr = sptr->next )
	{
		if (!( rest_valid_string( sptr->opinion ) ))
			continue;
		else if (!( strcasecmp( sptr->opinion, pptr->opinion ) ))
			sptr->match=++items;
	}
	if (!( items ))
		return((struct cops_solution *) 0);
	else	return( select_cops_result( first ) );
}

/*	----------------------------------------------------------	*/
/*		s e l e c t _ c o p s _ p r i c e			*/
/*	----------------------------------------------------------	*/
private	struct	cops_solution *	select_cops_price( struct cords_placement * pptr, struct cops_solution * first )
{
	struct	cops_solution * sptr;
	int	items=0;

	if (!( pptr->price ))
		return((struct cops_solution *) 0);

	for (	sptr=first;
		sptr != (struct cops_solution *) 0;
		sptr = sptr->next )
	{
		if (!( rest_valid_string( sptr->price ) ))
			continue;
		else if (!( strcasecmp( sptr->price, pptr->price ) ))
			sptr->match=++items;
	}
	if (!( items ))
		return((struct cops_solution *) 0);
	else	return( select_cops_result( first ) );
}

/*	----------------------------------------------------------	*/
/*		s e l e c t _ c o p s _ c o m p l e x 			*/
/*	----------------------------------------------------------	*/
private	struct	cops_solution *	select_cops_complex( struct cords_placement * pptr, struct cops_solution * first )
{
	struct	cops_solution * sptr;
	int	items=0;

	for (	sptr=first;
		sptr != (struct cops_solution *) 0;
		sptr = sptr->next )
	{
		if (( rest_valid_string( sptr->zone      ) ) 
		&&  ( rest_valid_string( pptr->zone      ) ) 
		&&  ( strcasecmp( sptr->zone, pptr->zone ) ))
			continue;
		if (( rest_valid_string( sptr->security      ) ) 
		&&  ( rest_valid_string( pptr->security      ) ) 
		&&  ( strcasecmp( sptr->security, pptr->security ) ))
			continue;
		if (( rest_valid_string( sptr->energy      ) ) 
		&&  ( rest_valid_string( pptr->energy      ) ) 
		&&  ( strcasecmp( sptr->energy, pptr->energy ) ))
			continue;
		if (( rest_valid_string( sptr->opinion      ) ) 
		&&  ( rest_valid_string( pptr->opinion      ) ) 
		&&  ( strcasecmp( sptr->opinion, pptr->opinion ) ))
			continue;
		if (( rest_valid_string( sptr->price      ) ) 
		&&  ( rest_valid_string( pptr->price      ) ) 
		&&  ( strcasecmp( sptr->price, pptr->price ) ))
			continue;
		sptr->match = ++items;
	}

	if (!( items ))
		return((struct cops_solution *) 0);
	else	return( select_cops_result( first ) );
}

/*	----------------------------------------------------------	*/
/*		 s e l e c t _ c o p s _ s o l u t i o n		*/
/*	----------------------------------------------------------	*/
private	struct	cops_solution *	select_cops_solution( struct cords_placement * pptr, struct cops_solution * first )
{
	char *	aptr;
	if (!( rest_valid_string(( aptr = pptr->algorithm )) ))
		aptr = Coptions.algorithm;

	if (!( strcasecmp( aptr, "quota:zone" ) ))
		return( select_cops_zone( pptr, first ) );
	else if (!( strcasecmp( aptr,"quota:security" ) ))
		return( select_cops_security( pptr, first ) );
	else if (!( strcasecmp( aptr, "quota:opinion" ) ))
		return( select_cops_opinion( pptr, first ) );
	else if (!( strcasecmp( aptr, "quota:energy" ) ))
		return( select_cops_energy( pptr, first ) );
	else if (!( strcasecmp( aptr, "quota:price" ) ))
		return( select_cops_price( pptr, first ) );
	else 	return( select_cops_complex( pptr, first ) );
}

/*	----------------------------------------------------------	*/
/*		 r e s t o r e _ c o p s _ q u a n t i t i e s 		*/
/*	----------------------------------------------------------	*/
private	struct	cops_solution *	restore_cops_quantities( struct cords_placement * pptr, struct cops_solution * sptr )
{
	return( sptr );
}

/*	----------------------------------------------------------	*/
/*		 r e l e a s e _ c o p s _ q u a n t i t i e s 		*/
/*	----------------------------------------------------------	*/
private	struct	cops_solution *	release_cops_quantities( struct cords_placement * pptr, struct cops_solution * sptr )
{
	return( sptr );
}

/*	----------------------------------------------------------	*/
/*		 c o n s u m e _ c o p s _ q u a n t i t i e s 		*/
/*	----------------------------------------------------------	*/
private	struct	cops_solution *	consume_cops_quantities( struct cords_placement * pptr, struct cops_solution * sptr )
{
	return( sptr );
}

/*	----------------------------------------------------------	*/
/*		 r e s e r v e _ c o p s _ q u a n t i t i e s 		*/
/*	----------------------------------------------------------	*/
private	struct	cops_solution *	reserve_cops_quantities( struct cords_placement * pptr, struct cops_solution * sptr )
{
	return( sptr );
}

/*	----------------------------------------------------------	*/
/*		 b u i l d _ c o p s _ f a i l u r e			*/
/*	----------------------------------------------------------	*/
private	struct	cops_solution *	build_cops_failure( struct cords_placement * pptr, struct cops_solution * sptr )
{
	return( sptr );
}

/*	----------------------------------------------------------	*/
/*		 b u i l d _ c o p s _ q u a n t i t i e s 		*/
/*	----------------------------------------------------------	*/
private	struct	cops_solution *	build_cops_quantities( struct cords_placement * pptr, struct cops_solution * sptr )
{
	struct	cops_quota    * qptr;
	struct	occi_response * zptr;
	struct	occi_element  * first = (struct occi_element *) 0;
	struct	occi_element  * last = (struct occi_element *) 0;	
	struct	occi_element  * eptr = (struct occi_element *) 0;	
	char 	identity[2048];
	char 	buffer[2048];
	char 	value[256];
	char 	self[2048];
	char 	qname[64];
	int	items=0;
	char *	ihost;

	/* ---------------------------------------------------- */
	/* -resolve the quantity category manager		*/
	/* ---------------------------------------------------- */
	if (!( ihost = occi_resolve_category_provider( _CORDS_QUANTITY, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( build_cops_failure( pptr,sptr ) );
	else
	{
		sprintf(identity,"%s/%s/",ihost,_CORDS_QUANTITY);
		liberate( ihost );
	}

	/* ---------------------------------------------------- */
	/* -prepare the "placement" instance self identifier 	*/
	/* ---------------------------------------------------- */
	if (!( rest_valid_string( Cops.identity ) ))
		return( build_cops_failure( pptr,sptr ) );
	else	sprintf(self,"%s/%s/%s",Cops.identity,_CORDS_PLACEMENT,pptr->id);

	/* ---------------------------------------------------- */
	/* -delete quantity links from this placement		*/
	/* ---------------------------------------------------- */
	if (!( zptr = occi_delete_links( self, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( build_cops_failure( pptr,sptr ) );
	else	zptr = occi_remove_response( zptr );

	/* ---------------------------------------------------- */
	/* -build a "quantity" instance for each quota record 	*/
	/* ---------------------------------------------------- */
	/* -link each quantity instance to the placement record	*/
	/* ---------------------------------------------------- */

	for ( 	qptr=sptr->first;
		qptr != (struct cops_quota *) 0;
		qptr = qptr->next )
	{
		sprintf(qname,"q%u",++items);

		/* ----------------------------- */
		/* the provider quota identifier */
		/* ----------------------------- */
		if (!( eptr = occi_create_element( "occi.quantity.quota", qptr->uuid ) ))
			return( build_cops_failure( pptr,sptr ) );
		else 	first = last = eptr;

		/* ----------------------------- */
		/* the quantity value concerned  */
		/* ----------------------------- */
		sprintf(value,"%u", qptr->quantity);
		if (!( eptr = occi_create_element( "occi.quantity.value", value ) ))
			return( build_cops_failure( pptr,sptr ) );
		else
		{
			eptr->previous = last;
			last->next = eptr;
			last = eptr;
		}

		/* ----------------------------- */
		/* the timestamp when occured    */
		/* ----------------------------- */
		sprintf(value,"%u",time((long *) 0));
		if (!( eptr = occi_create_element( "occi.quantity.when", value ) ))
			return( build_cops_failure( pptr,sptr ) );
		else
		{
			eptr->previous = last;
			last->next = eptr;
			last = eptr;
		}

		/* ----------------------------- */
		/* the quantity name token       */
		/* ----------------------------- */
		sprintf(value,"%u",time((long *) 0));
		if (!( eptr = occi_create_element( "occi.quantity.name", qname ) ))
			return( build_cops_failure( pptr,sptr ) );
		else
		{
			eptr->previous = last;
			last->next = eptr;
			last = eptr;
		}

		/* ----------------------------- */
		/* add the placement parent uuid */
		/* ----------------------------- */
		if (!( eptr = occi_create_element( "occi.quantity.placement", self ) ))
			return( build_cops_failure( pptr,sptr ) );
		else
		{
			eptr->previous = last;
			last->next = eptr;
			last = eptr;
		}

		/* ------------------------------ */
		/* add the placement account uuid */
		/* ------------------------------ */
		if (!( eptr = occi_create_element( "occi.quantity.account", pptr->account ) ))
			return( build_cops_failure( pptr,sptr ) );
		else
		{
			eptr->previous = last;
			last->next = eptr;
			last = eptr;
		}

		/* ----------------------------- */
		/* the quota property concerned  */
		/* ----------------------------- */
		if (!( eptr = occi_create_element( "occi.quantity.property", qptr->property ) ))
			return( build_cops_failure( pptr,sptr ) );
		else
		{
			eptr->previous = last;
			last->next = eptr;
			last = eptr;
		}

		/* ----------------------------- */
		/* the quantity state concerned  */
		/* ----------------------------- */
		if (!( eptr = occi_create_element( "occi.quantity.state", "0" ) ))
			return( build_cops_failure( pptr,sptr ) );
		else
		{
			eptr->previous = last;
			last->next = eptr;
			last = eptr;
		}

		/* --------------------------------------------- */
		/* create the quota "quantity" category instance */
		/* --------------------------------------------- */
		if (!( zptr = occi_simple_post( identity, first, _CORDS_CONTRACT_AGENT, default_tls() )))
			return( build_cops_failure( pptr,sptr ) );

		else if (!( ihost = occi_extract_location( zptr ) ))
		{
			zptr = occi_remove_response( zptr );
			return( build_cops_failure( pptr,sptr ) );
		}
		else
		{
			/* --------------------------------------- */
			/* build and store the quantity identifier */
			/* --------------------------------------- */
			rest_add_http_prefix(buffer,2048,ihost);
			if (!( qptr->reference = allocate_string( buffer ) ))
			{
				zptr = occi_remove_response( zptr );
				return( build_cops_failure( pptr,sptr ) );
			}
			else
			{
				/* -------------------------------------------------------------- */
				/* link the quota "quantity" instance to the "placement" instance */
				/* -------------------------------------------------------------- */
				zptr = occi_remove_response( zptr );
				if (!( zptr = occi_create_link( self, qptr->reference, _CORDS_CONTRACT_AGENT, default_tls() )))
					return( build_cops_failure( pptr,sptr ) );
				else	zptr = occi_remove_response( zptr );
			}
		}
	}
	return( sptr );
}

/*	----------------------------------------------------------	*/
/*		v a l i d a t e _ c o p s _ q u o t a			*/
/*	----------------------------------------------------------	*/
/* 	 need to read the node information and the provider quota 	*/
/*	 and figure out if there is enough available of each type 	*/
/* 	 and perform the corresponding reservation recomendations 	*/
/*	----------------------------------------------------------	*/
/*	  NB: the zptr memory must be released when not accepted	*/
/*	----------------------------------------------------------	*/
private	struct cops_solution * validate_cops_quota( 
				struct cops_solution * sptr,
				struct occi_response * zptr, 
				struct cords_placement * pptr, 
				struct rest_header * hptr )
{
	struct	rest_header  * iptr;
	struct	occi_element * eptr;
	struct	occi_element * dptr;
	struct	occi_response * yptr;
	struct	cops_quota * qptr;
	int	offered=0;
	int	reserved=0;
	int	consumed=0;
	int	granularity=0;
	int	required=0;
	char 	units[32];
	char *	vptr;
	for (	eptr=cords_first_link( zptr );
		eptr != (struct occi_element *) 0;
		eptr = cords_next_link( eptr ) )
	{
		required=granularity=offered=reserved=consumed=0;
		strcpy(units,"U");
		if (!( eptr->value ))
			continue;
		else if (!( vptr =  occi_unquoted_link( eptr->value ) ))
			continue;
		else if (!( yptr = occi_simple_get( vptr, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		{
			liberate( vptr );
			continue;
		}
		else	liberate( vptr );
		if (!( dptr = occi_locate_element( yptr->first, "occi.quota.property" ) ))
		{
			yptr = occi_remove_response( yptr );
			continue;
		}
		else if (!( qptr = allocate_cops_quota()))
		{
			yptr = occi_remove_response( yptr );
			continue;
		}
		else if (!( qptr->previous = sptr->last ))
			sptr->first = qptr;
		else	sptr->last->next = qptr;
		sptr->last = qptr;
		if (!( qptr->property = allocate_string( dptr->value )))
		{
			yptr = occi_remove_response( yptr );
			continue;
		}
		else if (!( qptr->uuid =  occi_unquoted_link( eptr->value ) ))
		{
			yptr = occi_remove_response( yptr );
			continue;
		}
		else if (!( dptr = occi_locate_element( yptr->first, "occi.quota.offered" ) ))
		{
			yptr = occi_remove_response( yptr );
			continue;
		}
		else	offered = atoi( dptr->value );
		if (!( dptr = occi_locate_element( yptr->first, "occi.quota.reserved" ) ))
		{
			yptr = occi_remove_response( yptr );
			continue;
		}
		else	reserved = atoi( dptr->value );
		if (!( dptr = occi_locate_element( yptr->first, "occi.quota.consumed" ) ))
		{
			yptr = occi_remove_response( yptr );
			continue;
		}
		else	consumed = atoi( dptr->value );

		if (!( dptr = occi_locate_element( yptr->first, "occi.quota.granularity" ) ))
		{
			yptr = occi_remove_response( yptr );
			continue;
		}
		else if (!( granularity = atoi( dptr->value ) ))
			granularity = 1;

		if (!( dptr = occi_locate_element( yptr->first, "occi.quota.units" ) ))
			strcpy(units,"U");
		else if (!( rest_valid_string( dptr->value ) ))
			strcpy(units,"U");
		else	strcpy(units,dptr->value);

		yptr = occi_remove_response( yptr );

		/* ------------------------------------- */
		/* locate consumption of this quota item */
		/* ------------------------------------- */
		for (	iptr=hptr;
			iptr != (struct rest_header *) 0;
			iptr = iptr->next )
		{
			if (!( iptr->name ))
				continue;
			else if (!( iptr->value ))
				continue;
			else if (!( strcmp( iptr->name, qptr->property ) ))
			{
				if ((required = rest_normalise_value( iptr->value, units[0] )) < granularity)
					required = granularity;
				if ((offered - reserved) >= required )
					qptr->quantity = required;
				else if (!( Coptions.overbooking ))
					return( liberate_cops_solution( sptr ) );
				else	qptr->quantity = required;
				break;
			}
		}
	}
	return( sptr );
}

/*	----------------------------------------------------------	*/
/*		e v a l u a t e _ c o p s _ n e e d s			*/
/*	----------------------------------------------------------	*/
/*	calculation of placement quota needs in terms of		*/
/*		accords.vm.count : 1					*/
/*		accords.address.count : 1				*/
/*		occi.compute.cores					*/
/*		occi.compute.memory					*/
/*		occi.storage.size 					*/
/*	----------------------------------------------------------	*/
private	struct	rest_header *	evaluate_cops_needs( struct standard_node * nptr )
{
	struct	rest_header * first=(struct rest_header *) 0;
	struct	rest_header * last=(struct rest_header *) 0;
	struct	rest_header * hptr=(struct rest_header *) 0;
	struct	occi_element* eptr;

	if (!( nptr ))
		return( first );

	if (!( first = rest_create_header( "accords.vm.count", "1" )))
		return( first );
	else 	last = first;

	if (!( nptr->compute.message ))
		return( first );

	if (( eptr = occi_locate_element( nptr->compute.message->first, "occi.compute.cores" )) != (struct occi_element *) 0)
		if (!( last = rest_postfix_header( last, "occi.compute.cores", eptr->value )))
			return( first );

	if (( eptr = occi_locate_element( nptr->compute.message->first, "occi.compute.memory" )) != (struct occi_element *) 0)
		if (!( last = rest_postfix_header( last, "occi.compute.memory", eptr->value )))
			return( first );

	if (!( nptr->storage.message ))
		return( first );

	if (( eptr = occi_locate_element( nptr->storage.message->first, "occi.storage.size" )) != (struct occi_element *) 0)
		if (!( last = rest_postfix_header( last, "occi.storage.size", eptr->value )))
			return( first );

	if (!( nptr->network.message ))
		return( first );

	if (!( last = rest_postfix_header( last, "accords.address.count", "1" )))
		return( first );
	else	return( first );

}

/*	----------------------------------------------------------	*/
/*		r e s o l v e _ c o p s _ s o l u t i o n		*/
/*	----------------------------------------------------------	*/
private	char *	resolve_cops_solution( struct cords_placement * pptr )
{
	char	buffer[4096];
	char	prefix[1024];
	char *	ihost;
	char *	result=(char *) 0;
	struct	occi_element 	* eptr;
	struct	occi_response	* zptr;
	struct	occi_response	* yptr;
	struct	cops_solution 	* sptr;
	struct	standard_node 	* nptr;
	struct	rest_header 	* qptr;
	struct	cops_solution   * first=(struct cops_solution *) 0;
	struct	cops_solution   * last=(struct cops_solution *) 0;
	int			  solutions=0;
	int			  status=0;

	/* ---------------------------------------------------- */
	/* -resolve the provider category manager		*/
	/* ---------------------------------------------------- */
	if (!( ihost = occi_resolve_category_provider( _CORDS_PROVIDER, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( ihost );
	else
	{
		sprintf(prefix,"/%s/",_CORDS_PROVIDER);
		sprintf(buffer,"%s%s",ihost,prefix);
		liberate( ihost );
	}

	/* ----------------------------------------------- */
	/* retrieve the OCCI resource instances that match */
	/* ----------------------------------------------- */
	if (!( pptr ))
		return((char *) 0);
	else if (!( rest_valid_string( pptr->provider ) ))
		return((char *) 0);
	else if (!( rest_valid_string( pptr->node ) ))
		return((char *) 0);
	else if (!( nptr = get_standard_node( pptr->node, _CORDS_CONTRACT_AGENT, default_tls(), &status ) ))
		return((char *) 0);
	else if (!( qptr = evaluate_cops_needs( nptr ) ))
		return((char *) 0);
	else if (!( eptr = occi_create_element( "occi.provider.category", pptr->provider ) ))
		return((char *) 0);
	else if (!( zptr = occi_simple_list( buffer, eptr, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return((char *) 0);
	else 
	{
		for (	eptr = zptr->first;
			eptr != (struct occi_element *) 0;
			eptr = eptr->next )
		{
			if (!( rest_valid_string( eptr->name ) ))
				continue;
			else if ( strcmp( eptr->name, prefix ) != 0 )
				continue;
			else if (!( rest_valid_string( eptr->value ) ))
				continue;
			else if (!( yptr = occi_simple_get( eptr->value, _CORDS_CONTRACT_AGENT, default_tls() ) ))
				continue;
			else if (!( sptr = allocate_cops_solution( eptr->value, yptr ) ))
			{
				yptr = occi_remove_response( yptr );
				continue;
			}
			else if (!( sptr = validate_cops_quota( sptr, yptr, pptr, qptr ) ))
			{
				continue;
			}
			else if (!( sptr->previous = last ))
				first = sptr;
			else	last->next = sptr;
			last = sptr;
			solutions++;
		}
		zptr = occi_remove_response( zptr );
		if (!( solutions ))
			return( (char *) 0 );
		else 
		{
			/* ----------------------------- */
			/* select the resulting solution */
			/* ----------------------------- */
			if (!( sptr = select_cops_solution( pptr, first ) ))
			{
				release_cops_solutions( first );
				return( (char *) 0 );
			}
			/* -------------------------- */
			/* build the quota quantities */
			/* -------------------------- */
			else if (!( sptr = build_cops_quantities( pptr, sptr ) ))
			{
				release_cops_solutions( first );
				return( (char *) 0 );
			}
			/* ---------------------------- */
			/* reserve the quota quantities */
			/* ---------------------------- */
			else if (!( sptr = reserve_cops_quantities( pptr, sptr ) ))
			{
				release_cops_solutions( first );
				return( (char *) 0 );
			}
			else
			{
				result = allocate_string( sptr->identity );
				release_cops_solutions( first );
				return( result );
			}
		}				
	}
}


	/* ---------------- */
#endif	/* _copsoperation_c */
	/* ---------------- */

