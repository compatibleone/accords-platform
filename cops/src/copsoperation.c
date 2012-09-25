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
	int	match;
	int	chosen;
};

/*	----------------------------------------------------------	*/
/*		l i b e r a t e _ c o p s _ s o l u t i o n		*/
/*	----------------------------------------------------------	*/
private struct	cops_solution * liberate_cops_solution(struct cops_solution * sptr)
{
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
private	int	preference=0; 	/* round robin preference */
private	struct	cops_solution *	select_cops_result( struct cops_solution * first )
{
	if ( preference )
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
	else	return( select_cops_result( sptr ) );
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
	else	return( select_cops_result( sptr ) );
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
	else	return( select_cops_result( sptr ) );
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
	else	return( select_cops_result( sptr ) );
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
	else	return( select_cops_result( sptr ) );
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
	else	return( select_cops_result( sptr ) );
}

/*	----------------------------------------------------------	*/
/*		r e s o l v e _ c o p s _ s o l u t i o n		*/
/*	----------------------------------------------------------	*/
private	struct	cops_solution *	select_cops_solution( struct cords_placement * pptr, struct cops_solution * first )
{
	char *	aptr;
	if (!( rest_valid_string(( aptr = pptr->algorithm )) ))
		aptr = "zone";

	if (!( strcasecmp( aptr, "zone" ) ))
		return( select_cops_zone( pptr, first ) );
	else if (!( strcasecmp( aptr,"security" ) ))
		return( select_cops_security( pptr, first ) );
	else if (!( strcasecmp( aptr, "opinion" ) ))
		return( select_cops_opinion( pptr, first ) );
	else if (!( strcasecmp( aptr, "energy" ) ))
		return( select_cops_energy( pptr, first ) );
	else if (!( strcasecmp( aptr, "price" ) ))
		return( select_cops_price( pptr, first ) );
	else 	return( select_cops_complex( pptr, first ) );
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
private	int	validate_cops_quota( struct occi_response * zptr, struct cords_placement * pptr )
{
	/* ---- */
	/* TODO */
	/* ---- */
	return( 1 );
}

/*	----------------------------------------------------------	*/
/*		r e s o l v e _ c o p s _ s o l u t i o n		*/
/*	----------------------------------------------------------	*/
private	char *	resolve_cops_solution( struct cords_placement * pptr )
{
	char	buffer[4096];
	char *	result=(char *) 0;
	struct	occi_element 	* eptr;
	struct	occi_response	* zptr;
	struct	occi_response	* yptr;
	struct	cops_solution 	* sptr;
	struct	cops_solution   * first=(struct cops_solution *) 0;
	struct	cops_solution   * last=(struct cops_solution *) 0;
	int			  solutions=0;

	/* ----------------------------------------------- */
	/* retrieve the OCCI resource instances that match */
	/* ----------------------------------------------- */
	if (!( pptr ))
		return((char *) 0);
	else if (!( rest_valid_string( pptr->provider ) ))
		return((char *) 0);
	else if (!( eptr = occi_create_element( "occi.provider.category", pptr->provider ) ))
		return((char *) 0);
	else if (!( zptr = occi_simple_list( buffer, eptr, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return((char *) 0);
	else 
	{
		for (	eptr=cords_first_link( zptr );
			eptr != (struct occi_element *) 0;
			eptr = cords_next_link( eptr ) )
		{
			if (!( eptr->value ))
				continue;
			else if (!( yptr = occi_simple_get( eptr->value, _CORDS_CONTRACT_AGENT, default_tls() ) ))
				continue;
			else if (!( validate_cops_quota( yptr, pptr ) ))
				continue;
			else if (!( sptr = allocate_cops_solution( eptr->value, yptr ) ))
			{
				yptr = occi_remove_response( yptr );
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
			/* ---------------------------------- */
			/* only one solution is the easy case */
			/* ---------------------------------- */
			if ( solutions == 1 )
			{
				if (( first ) && ( first->identity ))
				{
					result = allocate_string( first->identity );
					sptr = liberate_cops_solution( first );
					return( result );
				}
				else
				{
					sptr = liberate_cops_solution( first );
					return( (char *) 0);
				}
			}
			/* --------------------------------------- */
			/* multiple solutions need better handling */
			/* --------------------------------------- */
			else if (!( sptr = select_cops_solution( pptr, first ) ))
			{
				while ((sptr = first) != (struct cops_solution *) 0)
				{
					first = sptr->next;
					sptr = liberate_cops_solution( sptr );
				}
				return( (char *) 0 );
			}
			else
			{
				result = allocate_string( first->identity );
				while ((sptr = first) != (struct cops_solution *) 0)
				{
					first = sptr->next;
					sptr = liberate_cops_solution( sptr );
				}
				return( result );
			}
		}				
	}
}


	/* ---------------- */
#endif	/* _copsoperation_c */
	/* ---------------- */



