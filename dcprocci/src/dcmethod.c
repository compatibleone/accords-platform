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
#ifndef	_dcmethod_c
#define	_dcmethod_c

/*	-------------------------------------------	*/
/* 	      c r e a t e _ d e l t a c l o u d  	*/
/*	-------------------------------------------	*/
private	int	create_deltacloud(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	deltacloud * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( pptr->node ))
		return( 0 ); 
	else	return(create_deltacloud_contract(optr,pptr, _CORDS_CONTRACT_AGENT, DcProcci.tls));
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ d e l t a c l o u d  	*/
/*	-------------------------------------------	*/
private	int	retrieve_deltacloud(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	deltacloud * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ d e l t a c l o u d  	*/
/*	-------------------------------------------	*/
private	int	update_deltacloud(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	deltacloud * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ d e l t a c l o u d  	*/
/*	-------------------------------------------	*/
private	int	delete_deltacloud(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	deltacloud * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(delete_deltacloud_contract(optr, pptr, _CORDS_CONTRACT_AGENT, DcProcci.tls));
}

private	struct	occi_interface	deltacloud_interface = {
	create_deltacloud,
	retrieve_deltacloud,
	update_deltacloud,
	delete_deltacloud
	};


	/* ----------- */
#endif	/* _dcmethod_c */
	/* ----------- */

