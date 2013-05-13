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
#ifndef	_dcmethod_c
#define	_dcmethod_c

/*	-------------------------------------------	*/
/* 	      c r e a t e _ d e l t a c l o u d  	*/
/*	-------------------------------------------	*/
private	int	create_deltacloud(struct occi_category * optr, void * vptr,struct rest_request *rptr)
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
private	int	retrieve_deltacloud(struct occi_category * optr, void * vptr,struct rest_request *rptr)
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
private	int	update_deltacloud(struct occi_category * optr, void * vptr,struct rest_request *rptr)
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
private	int	delete_deltacloud(struct occi_category * optr, void * vptr,struct rest_request *rptr)
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

