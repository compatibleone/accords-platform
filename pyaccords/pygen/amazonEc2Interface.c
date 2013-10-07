/*-------------------------------------------------------------------------------*/
/* ACCORDS Platform                                                              */
/* copyright 2012 ,Hamid MEDJAHE & Elyes ZEKRI  (hmedjahed@prologue.fr) Prologue */
/*-------------------------------------------------------------------------------*/
/* Licensed under the Apache License, Version 2.0 (the "License");               */
/* you may not use this file except in compliance with the License.              */
/* You may obtain a copy of the License at                                       */
/*                                                                               */
/*       http://www.apache.org/licenses/LICENSE-2.0                              */
/*                                                                               */
/* Unless required by applicable law or agreed to in writing, software           */
/* distributed under the License is distributed on an "AS IS" BASIS,             */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.      */
/* See the License for the specific language governing permissions and           */
/* limitations under the License.                                                */
/*-------------------------------------------------------------------------------*/
#include "occi.h"
#include "../pysrc/ctools.h"
#include "../pysrc/pytools.h"
#include "../pysrc/listcateg.h"
#include "amazonEc2.h"
#include "../pysrc/ec2contract.h"
#include <Python.h>

private int amazonEc2_create(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct occi_kind_node * nptr;
	struct amazonEc2 * pptr;

	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( pptr->node ))
		return( 0 ); 
	else return (create_ec2_contract(optr,pptr, _CORDS_CONTRACT_AGENT, default_tls()));
}


private int amazonEc2_retrieve(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct occi_kind_node * nptr;
	struct amazonEc2 * pptr;

	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return (0);
}


private int amazonEc2_update(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct occi_kind_node * nptr;
	struct amazonEc2 * pptr;

	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
        else 	return(0);
}

private int amazonEc2_delete(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	amazonEc2 * pptr;

	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else return (delete_ec2_contract(optr, pptr, _CORDS_CONTRACT_AGENT, default_tls()));
}


