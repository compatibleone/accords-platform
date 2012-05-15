/* ------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                   */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>    */
/* --------------------------------------------------------------------*/
/*  This is free software; you can redistribute it and/or modify it    */
/*  under the terms of the GNU Lesser General Public License as        */
/*  published by the Free Software Foundation; either version 2.1 of   */
/*  the License, or (at your option) any later version.                */
/*                                                                     */
/*  This software is distributed in the hope that it will be useful,   */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of     */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   */
/*  Lesser General Public License for more details.                    */
/*                                                                     */
/*  You should have received a copy of the GNU Lesser General Public   */
/*  License along with this software; if not, write to the Free        */
/*  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA */
/*  02110-1301 USA, or see the FSF site: http://www.fsf.org.           */
/* --------------------------------------------------------------------*/
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


