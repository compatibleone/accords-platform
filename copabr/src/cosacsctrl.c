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
#ifndef	_cosacsctrl_c
#define	_cosacsctrl_c
#include "standard.h"
#include "rest.h"
#include "occi.h"
#include "cosacsctrl.h"
#include "cordslang.h"

/*	------------------------------------	*/
/*	the COSACS host is currently forced 	*/
/*	this will be relaxed when the module	*/
/*	is ready and correclty in place in 	*/
/*	the application image of the vm		*/
/*	------------------------------------	*/
#define	_COSACS_HOST "http://127.0.0.1"
#define	_COSACS_PORT 8286
#define	_COSACS_START "cosacs:start"

/*	-----------------------------------------------------------	*/
/*		c o s a c s _ c r e a t e _ p r o b e 			*/
/*	-----------------------------------------------------------	*/
public	int	cosacs_create_probe( char * cosacs, char * prefix, char * symbol, char * stream, char * metric)
{
	char	buffer[1024];
	char 	work[1024];
	char *	agent=_CORDS_CONTRACT_AGENT;

	char *	host=_COSACS_HOST; /* TODO will be cosacs parameter when ready */

	struct	occi_client * cptr;
	struct	occi_request * rptr;
	struct	occi_response * zptr;
	struct	occi_element * dptr;

	/* ----------------------- */
	/* prepare COSACS identity */
	/* ----------------------- */
	if (!( host = getenv( "COSACS" ) ))
		host = cosacs;
	sprintf(buffer,"%s:%u/%s/",host,_COSACS_PORT,_CORDS_PROBE);

	if ( prefix )
		sprintf(work,"%s_%s",prefix,symbol);
	else	strcpy(work,symbol);

	/* ----------------------------------- */
	/* create client and request then POST */
	/* ----------------------------------- */
	if (!( cptr = occi_create_client( buffer, agent, default_tls() ) ))
		return(0);
	else if (!(rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
		return(0);
	else if ((!(dptr=occi_request_element(rptr,"occi.probe.name"  , work   ) ))
	     ||  (!(dptr=occi_request_element(rptr,"occi.probe.metric", metric ) ))
	     ||  (!(dptr=occi_request_element(rptr,"occi.probe.stream", stream ) )))
	{
		rptr = occi_remove_request( rptr );
		return(0);
	}
	else
	{
		zptr = occi_client_post( cptr, rptr );
		zptr = occi_remove_response( zptr );
		rptr = occi_remove_request( rptr );
		return(0);
	}
}

/*	-----------------------------------------------------------	*/
/*		c o s a c s _ c r e a t e _ m e t a d a t a		*/
/*	-----------------------------------------------------------	*/
public	int	cosacs_create_metadata( char * cosacs, char * prefix, char * symbol, char * value )
{
	char	buffer[1024];
	char 	work[1024];
	char *	agent=_CORDS_CONTRACT_AGENT;

	char *	host=_COSACS_HOST; /* TODO will be cosacs parameter when ready */

	struct	occi_client * cptr;
	struct	occi_request * rptr;
	struct	occi_response * zptr;
	struct	occi_element * dptr;

	/* ----------------------- */
	/* prepare COSACS identity */
	/* ----------------------- */
	if (!( host = getenv( "COSACS" ) ))
		host = cosacs;
	sprintf(buffer,"%s:%u/%s/",host,_COSACS_PORT,_CORDS_METADATA);
	if ( prefix )
		sprintf(work,"%s_%s",prefix,symbol);
	else	strcpy(work,symbol);

	/* ----------------------------------- */
	/* create client and request then POST */
	/* ----------------------------------- */
	if (!( cptr = occi_create_client( buffer, agent, default_tls() ) ))
		return(0);
	else if (!(rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
		return(0);
	else if ((!(dptr=occi_request_element(rptr,"occi.metadata.name" , work  ) ))
	     ||  (!(dptr=occi_request_element(rptr,"occi.metadata.value", value ) )))
	{
		rptr = occi_remove_request( rptr );
		return(0);
	}
	else
	{
		zptr = occi_client_post( cptr, rptr );
		zptr = occi_remove_response( zptr );
		rptr = occi_remove_request( rptr );
		return(0);
	}
}

/*	-----------------------------------------------------------	*/
/*		   c o s a c s _ c r e a t e _ s c r i p t 		*/
/*	-----------------------------------------------------------	*/
public	int	cosacs_create_script( char * cosacs, char * action, char * parameters, char * type )
{
	char	buffer[1024];
	char *	host=_COSACS_HOST; /* TODO will be cosacs parameter when ready */
	char *	agent=_CORDS_CONTRACT_AGENT;

	struct	occi_client * cptr;
	struct	occi_request * rptr;
	struct	occi_response * zptr;
	struct	occi_element * dptr;

	/* ----------------------- */
	/* prepare COSACS identity */
	/* ----------------------- */
	if (!( host = getenv( "COSACS" ) ))
		host = cosacs;
	sprintf(buffer,"%s:%u/%s/",host,_COSACS_PORT,_CORDS_SCRIPT);

	/* ----------------------------------- */
	/* create client and request then POST */
	/* ----------------------------------- */
	if (!( cptr = occi_create_client( buffer, agent, default_tls() ) ))
		return(0);
	else if (!(rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
		return(0);
	else if ((!(dptr=occi_request_element(rptr,"occi.script.name" ,  action     ) ))
	     ||  (!(dptr=occi_request_element(rptr,"occi.script.syntax", parameters ) ))
	     ||  (!(dptr=occi_request_element(rptr,"occi.script.nature", type       ) )))
	{
		rptr = occi_remove_request( rptr );
		return(0);
	}
	else
	{
		zptr = occi_client_post( cptr, rptr );
		zptr = occi_remove_response( zptr );
		rptr = occi_remove_request( rptr );
		return(0);
	}
}

/*	-----------------------------------------------------------	*/
/*		   c o s a c s _ c r e a t e _ f i l e			*/
/*	-----------------------------------------------------------	*/
public	int	cosacs_create_file( char * cosacs, char * remotename, char * localname, char * type )
{
	char	buffer[1024];
	char *	host=_COSACS_HOST; /* TODO will be cosacs parameter when ready */
	char *	agent=_CORDS_CONTRACT_AGENT;

	struct	occi_client * cptr;
	struct	occi_request * rptr;
	struct	occi_response * zptr;
	struct	occi_element * dptr;

	/* ----------------------- */
	/* prepare COSACS identity */
	/* ----------------------- */
	if (!( host = getenv( "COSACS" ) ))
		host = cosacs;
	sprintf(buffer,"%s:%u/%s/",host,_COSACS_PORT,_CORDS_FILE);

	/* ----------------------------------- */
	/* create client and request then POST */
	/* ----------------------------------- */
	if (!( cptr = occi_create_client( buffer, agent, default_tls() ) ))
		return(0);
	else if (!(rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
		return(0);
	else if ((!(dptr=occi_request_element(rptr,"occi.file.name" , remotename ) ))
	     ||  (!(dptr=occi_request_element(rptr,"occi.file.type",  type       ) )))
	{
		rptr = occi_remove_request( rptr );
		return(0);
	}
	else
	{
		zptr = occi_client_post( cptr, rptr );
		zptr = occi_remove_response( zptr );
		rptr = occi_remove_request( rptr );
		return(0);
	}
}

/*	-----------------------------------------------------------	*/
/* 	  c o s a c s _ m e t a d a t a _ i n s t r u c t i o n s 	*/
/*	-----------------------------------------------------------	*/
/*	This function provides instruction processing on a provider	*/
/*	specific contract basis. The meta data ensuing will be sent	*/
/*	to the provisioned instance through the COSACS interface.	*/
/*	-----------------------------------------------------------	*/
public	int	cosacs_metadata_instructions( 
		char * cosacs, 
		char * nature,
		char * contract, 
		char * publisher )
{

	char	*	ihost;
	char 	*	vptr;

	char 	*	stream="streamid";
	char 	*	metric="samples";

	struct	occi_response * zptr=(struct occi_response *) 0;
	struct	occi_response * zzptr=(struct occi_response *) 0;
	struct	occi_response * yptr=(struct occi_response *) 0;
	struct	occi_element  * fptr=(struct occi_element  *) 0;
	struct	occi_element  * gptr=(struct occi_element  *) 0;
	struct	occi_element  * jptr=(struct occi_element  *) 0;
	struct	occi_element  * eptr=(struct occi_element  *) 0;
	struct	occi_client   * kptr=(struct occi_client   *) 0;
	struct	occi_request  * qptr=(struct occi_request  *) 0;
	char	buffer[4096];
	char	tempname[4096];
	int	length=0;

	/* ------------------------------------------------------------------------- */
	/* select / resolve the instruction category service provider identification */
	/* ------------------------------------------------------------------------- */
	if (!( ihost = occi_resolve_category_provider( _CORDS_INSTRUCTION, _CORDS_CONTRACT_AGENT, default_tls() ) ))
	 	return( 0 );

	sprintf(buffer,"%s/%s/",ihost,_CORDS_INSTRUCTION);
	liberate( ihost );
	length = strlen(buffer);

	/* --------------------------------------------------------------------------- */
	/* retrieve the collection of instruction category instances for this contract */
	/* --------------------------------------------------------------------------- */
	if (!( kptr = occi_create_client( buffer, _CORDS_CONTRACT_AGENT, default_tls() ) ))
	 	return( 0 );

	else if (!(qptr = occi_create_request( 
			kptr, 
			kptr->target->object, 
			_OCCI_NORMAL )))
	 	return( 0 );
	else if (!( fptr = occi_request_element( 
			qptr,
			"occi.instruction.provision", contract ) ))
	{
		qptr = occi_remove_request( qptr );
	 	return( 0 );
	}
	else if (!( yptr = occi_client_get( kptr, qptr ) ))
	{
		qptr = occi_remove_request( qptr );
	 	return( 0 );
	}

	qptr = occi_remove_request ( qptr );

	/* -------------------------------------------------- */
	/* for each instruction category instance in the list */
	/* -------------------------------------------------- */
	cosacs_create_metadata( cosacs, (char *) 0, "publisher", publisher );
	cosacs_create_metadata( cosacs, (char *) 0, "contract",  contract  );
	cosacs_create_metadata( cosacs, (char *) 0, "cosacs",    cosacs    );
	for (	eptr = yptr->first;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;
		else if (!( vptr = allocate_string( eptr->value )))
			continue;
		else if (!( vptr = occi_category_id( vptr ) ))
			continue;
		else
		{
			strcat( buffer, vptr );
			liberate( vptr );
		}

		/* -------------------------------------------------- */
		/* retrieve the current instruction category instance */
		/* -------------------------------------------------- */
		if (( zptr = occi_simple_get( buffer, _CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
		{
			/* ensure its a configuration method */
			/* --------------------------------- */
			if (!(fptr = occi_locate_element( zptr->first, "occi.instruction.nature" )))
				zptr = occi_remove_response ( zptr );
			else if ( strcasecmp( fptr->value, nature ) )
				zptr = occi_remove_response ( zptr );
			else if (!(fptr = occi_locate_element( zptr->first, "occi.instruction.type" )))
				zptr = occi_remove_response ( zptr );
			else if ( strcasecmp( fptr->value, "method" ) )
				zptr = occi_remove_response ( zptr );
			else if (!(fptr = occi_locate_element( zptr->first, "occi.instruction.method" )))
				zptr = occi_remove_response ( zptr );
			else if ((!( strcasecmp( fptr->value, "system" ) ))
			     ||  (!( strcasecmp( fptr->value, "fork"   ) ))
			     ||  (!( strcasecmp( fptr->value, "process") ))
			     ||  (!( strcasecmp( fptr->value, "command") )))
			{
				if (!(jptr = occi_locate_element( zptr->first, "occi.instruction.value" )))
					zptr = occi_remove_response ( zptr );

				else
				{
					/* ------------------------------------------------------------ */
					/* Create a COSACS Meta Data instance using the name value pair */
					/* ------------------------------------------------------------ */
					cosacs_create_script( cosacs, "cosacs:run", jptr->value, fptr->value );
					zzptr = occi_remove_response ( zzptr );
					zptr = occi_remove_response ( zptr );
				}
			}
			else if (!(  strcasecmp( fptr->value, "configure" ) ))
			{
				/* collect the configuration details */
				/* --------------------------------- */
				if (!(fptr = occi_locate_element( zptr->first, "occi.instruction.symbol" )))
					zptr = occi_remove_response ( zptr );
				else if (!(gptr = occi_locate_element( zptr->first, "occi.instruction.property" )))
					zptr = occi_remove_response ( zptr );
				else if (!(jptr = occi_locate_element( zptr->first, "occi.instruction.value" )))
					zptr = occi_remove_response ( zptr );
				else
				{
					/* ------------------------------------------------------------ */
					/* Create a COSACS Meta Data instance using the name value pair */
					/* ------------------------------------------------------------ */
					cosacs_create_metadata( cosacs, fptr->value, gptr->value, jptr->value );
					zzptr = occi_remove_response ( zzptr );
					zptr = occi_remove_response ( zptr );
				}
			}

			else if (!(  strcasecmp( fptr->value, "monitor" ) ))
			{
				/* collect the configuration details */
				/* --------------------------------- */
				if (!(fptr = occi_locate_element( zptr->first, "occi.instruction.symbol" )))
					zptr = occi_remove_response ( zptr );
				else if (!( stream = occi_resolve_consumer( fptr->value, _CORDS_CONTRACT_AGENT, default_tls() )))
					zptr = occi_remove_response ( zptr );
				else if (!(gptr = occi_locate_element( zptr->first, "occi.instruction.property" )))
					zptr = occi_remove_response ( zptr );
				else if (!(jptr = occi_locate_element( zptr->first, "occi.instruction.value" )))
					zptr = occi_remove_response ( zptr );
				else
				{
					/* ------------------------------------------------------------ */
					/* Create a COSACS Probe instance for the stream and metric ids */
					/* ------------------------------------------------------------ */
					cosacs_create_probe( cosacs, fptr->value, gptr->value, stream, metric );
					zzptr = occi_remove_response ( zzptr );
					zptr = occi_remove_response ( zptr );
					stream = liberate( stream );
				}
			}

			else	zptr = occi_remove_response ( zptr );
		}
		/* ----------------------- */
		/* quick reset of base url */
		/* ----------------------- */
		buffer[length] = 0;
	}

	yptr = occi_remove_response ( yptr );
	cosacs_create_script( cosacs,_COSACS_START, contract, "process" );

	return(0);
}

	/* ------------- */
#endif	/* _cosacsctrl_c */
	/* ------------- */


