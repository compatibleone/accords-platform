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
#ifndef	_dccontract_c
#define	_dccontract_c

/*	---------------------------------------------------	*/
/* 	c r e a t e _ d e l t a c l o u d _ c o n t r a c t	*/
/*	---------------------------------------------------	*/
public	int	create_deltacloud_contract(
		struct occi_category * optr,
		struct deltacloud * pptr,
		char * agent,
		char * tls )
{
	struct	dc_config * kptr;
	if (!( kptr = use_deltacloud_configuration( pptr->profile )))
		return( 777 );
	else	return(0);
}

/*	---------------------------------------------------	*/
/* 	d e l e t e _ d e l t a c l o u d _ c o n t r a c t	*/
/*	---------------------------------------------------	*/
public	int	delete_deltacloud_contract(
		struct occi_category * optr,
		struct deltacloud * pptr,
		char * agent,
		char * tls )
{
	struct	dc_config * kptr;
	if (!( kptr = use_deltacloud_configuration( pptr->profile )))
		return( 777 );
	else	return(0);
}

	/* ------------- */
#endif	/* _dccontract_c */
	/* ------------- */


