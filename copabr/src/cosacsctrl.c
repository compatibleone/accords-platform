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
#include "restclient.h"
#include "restpublic.h"

/*	------------------------------------	*/
/*	the COSACS host is currently forced 	*/
/*	this will be relaxed when the module	*/
/*	is ready and correclty in place in 	*/
/*	the application image of the vm		*/
/*	------------------------------------	*/
#define	_COSACS_HOST "127.0.0.1"
#define	_COSACS_ITERATIONS 10

/*	-------------------------------------------------	*/
/*		c o s a c s _ s y n c h r o n i s e		*/
/*	-------------------------------------------------	*/
private	void	cosacs_synchronise()
{
	sleep(1);
	return;
}

/*	-------------------------------------------------	*/
/*		c o s a c s _ h t t p _ p r e f i x		*/
/*	-------------------------------------------------	*/
private	char * 	cosacs_http_prefix()
{
	return( "http" );
}

/*	-------------------------------------------------	*/
/*			c o s a c s _ t l s 			*/
/*	-------------------------------------------------	*/
private	char * 	cosacs_tls()
{
	return( (char *) 0 );
}


/*	-------------------------------------------------	*/
/*	c o s a c s _ o c c i _ c r e a t e _ c l i e n t	*/
/*	-------------------------------------------------	*/
/*	it may be necessary to repeat the attempt to make	*/
/*	the occi client connection for the cosacs module.	*/
/*	-------------------------------------------------	*/
private	struct	occi_client * cosacs_occi_create_client( char * url, char * agent, char * tls )
{
	struct	occi_client * cptr=(struct occi_client *) 0;
	int	period=1;
	int	iteration=_COSACS_ITERATIONS;


	if (!( iteration ))
		return( occi_create_client( url, agent, tls ) );
	else
	{
		while (( iteration ) && (!( cptr )))
		{
			if ( check_debug() )
				printf("create_cosacs_client(%s,%u;%u)\n",url,iteration,period);
			if (!(cptr = occi_create_client( url, agent, tls )))
			{
				sleep(period);
				period++;
				iteration--;
			}
		}
		cosacs_synchronise();
		return(cptr);
	}
}

/*	-----------------------------------------------------------	*/
/*		c o s a c s _ c r e a t e _ p r o b e 			*/
/*	-----------------------------------------------------------	*/
public	int	cosacs_create_probe( char * cosacs, char * prefix, char * symbol, char * stream, char * metric)
{
	char	buffer[1024];
	char 	work[1024];
	char *	agent=_CORDS_CONTRACT_AGENT;

	char *	host=_COSACS_HOST; 

	struct	occi_client * cptr;
	struct	occi_request * rptr;
	struct	occi_response * zptr;
	struct	occi_element * dptr;

	/* ----------------------- */
	/* prepare COSACS identity */
	/* ----------------------- */
	if (!( host = getenv( "COSACS" ) ))
		host = cosacs;

	sprintf(buffer,"%s://%s:%u/%s/",cosacs_http_prefix(),host,_COSACS_PORT,_CORDS_PROBE);

	if ( prefix )
		sprintf(work,"%s_%s",prefix,symbol);
	else	strcpy(work,symbol);

	if ( check_debug() )
		printf("create_cosacs_probe(%s,%s)\n",buffer,work);

	/* ----------------------------------- */
	/* create client and request then POST */
	/* ----------------------------------- */
	if (!( cptr = cosacs_occi_create_client( buffer, agent, cosacs_tls() ) ))
		return(1100);
	else if (!(rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
		return(1101);
	else if ((!(dptr=occi_request_element(rptr,"occi.probe.name"  , work   ) ))
	     ||  (!(dptr=occi_request_element(rptr,"occi.probe.metric", metric ) ))
	     ||  (!(dptr=occi_request_element(rptr,"occi.probe.stream", stream ) )))
	{
		rptr = occi_remove_request( rptr );
		return(1102);
	}
	else
	{
		zptr = occi_client_post( cptr, rptr );
		cosacs_synchronise();
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

	char *	host=_COSACS_HOST; 

	struct	occi_client * cptr;
	struct	occi_request * rptr;
	struct	occi_response * zptr;
	struct	occi_element * dptr;
	int	i;

	rest_log_debug("entry::cosacs_create_meta");

	/* ----------------------- */
	/* prepare COSACS identity */
	/* ----------------------- */
	if (!( host = getenv( "COSACS" ) ))
		host = cosacs;
	sprintf(buffer,"%s://%s:%u/%s/",cosacs_http_prefix(),host,_COSACS_PORT,_CORDS_METADATA);

	if ( prefix )
		sprintf(work,"%s_%s",prefix,symbol);
	else	strcpy(work,symbol);

	/* -------------------------------------------------------------------- */
	/* ensure symbol and prefix has no period, hyphen nor colon in the name */
	/* -------------------------------------------------------------------- */
	for (i=0; work[i] != 0; i++)
	{
		if (work[i] == '.')
			work[i]='_';
		else if (work[i] == ':')
			work[i]='_';
		else if (work[i] == '-')
			work[i]='_';
	}
	if ( check_debug() )
		printf("create_cosacs_metadata(%s,%s,%s)\n",buffer,work,value);

	/* ----------------------------------- */
	/* create client and request then POST */
	/* ----------------------------------- */
	if (!( cptr = cosacs_occi_create_client( buffer, agent, cosacs_tls() ) ))
	{
		rest_log_debug("failure::cosacs::occi_create_client");
		return(1100);
	}

	if (!(rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
	{
		rest_log_debug("failure::cosacs::occi_create_request");
		return(1101);
	}

	if (!(dptr=occi_request_element(rptr,"occi.metadata.name" , work  ) ))
	{
		rest_log_debug("failure::cosacs::occi_element(name)");
		rptr = occi_remove_request( rptr );
		return(1102);
	}

	if (!(dptr=occi_request_element(rptr,"occi.metadata.value", value ) ))
	{
		rest_log_debug("failure::cosacs::occi_element(value)");
		rptr = occi_remove_request( rptr );
		return(1103);
	}
	else
	{
		if (!( zptr = occi_client_post( cptr, rptr ) ))
			rest_log_debug("failure::cosacs::occi_client_post)");
		else	zptr = occi_remove_response( zptr );
		cosacs_synchronise();
		rptr = occi_remove_request( rptr );
		rest_log_debug("exit::cosacs_create_meta");
		return(0);
	}
}

/*	-----------------------------------------------------------	*/
/*		   c o s a c s _ c r e a t e _ s c r i p t 		*/
/*	-----------------------------------------------------------	*/
public	int	cosacs_create_script( char * cosacs, char * action, char * parameters, char * type )
{
	char	buffer[1024];
	char *	host=_COSACS_HOST; 
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
	sprintf(buffer,"%s://%s:%u/%s/",cosacs_http_prefix(),host,_COSACS_PORT,_CORDS_SCRIPT);

	/* ----------------------------------- */
	/* create client and request then POST */
	/* ----------------------------------- */
	if ( check_debug() )
		printf("create_cosacs_script(%s,%s,%s,%s)\n",buffer,action,parameters,type);

	if (!( cptr = cosacs_occi_create_client( buffer, agent, cosacs_tls() ) ))
		return(1100);
	else if (!(rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
		return(1101);
	else if ((!(dptr=occi_request_element(rptr,"occi.script.name" ,  action     ) ))
	     ||  (!(dptr=occi_request_element(rptr,"occi.script.syntax", parameters ) ))
	     ||  (!(dptr=occi_request_element(rptr,"occi.script.nature", type       ) )))
	{
		rptr = occi_remove_request( rptr );
		return(1102);
	}
	else
	{
		zptr = occi_client_post( cptr, rptr );
		cosacs_synchronise();
		zptr = occi_remove_response( zptr );
		rptr = occi_remove_request( rptr );
		return(0);
	}
}

/*	-----------------------------------------------------------	*/
/*		   c o s a c s _ t e s t _ in t e r f a c e		*/
/*	-----------------------------------------------------------	*/
public	int	cosacs_test_interface( char * cosacs, int timeout, int retry )
{
	char	buffer[1024];
	char *	host=_COSACS_HOST; 
	char *	agent=_CORDS_CONTRACT_AGENT;
	struct	rest_header * hptr=(struct rest_header *) 0;
	struct	rest_response * rptr;
	if (!( host = getenv( "COSACS" ) ))
		host = cosacs;
	sprintf(buffer,"%s://%s:%u/-/",cosacs_http_prefix(),host,_COSACS_PORT);

	if (!( rptr = rest_client_try_get_request( buffer, cosacs_tls(), agent, hptr, timeout, retry ) ))
		return(0);
	else	
	{
		rptr = liberate_rest_response( rptr );
		return(1);
	}
}

/*	-----------------------------------------------------------	*/
/*		   c o s a c s _ c r e a t e _ f i l e			*/
/*	-----------------------------------------------------------	*/
public	int	cosacs_create_file( char * cosacs, char * remotename, char * localname, char * type )
{
	char	buffer[1024];
	char *	host=_COSACS_HOST; 
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
	sprintf(buffer,"%s://%s:%u/%s/",cosacs_http_prefix(),host,_COSACS_PORT,_CORDS_FILE);

	/* ----------------------------------- */
	/* create client and request then POST */
	/* ----------------------------------- */
	if ( check_debug() )
		printf("create_cosacs_file(%s,%s,%s)\n",buffer,remotename,type);

	if (!( cptr = cosacs_occi_create_client( buffer, agent, cosacs_tls() ) ))
		return(1100);
	else if (!(rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
		return(1101);
	else if ((!(dptr=occi_request_element(rptr,"occi.file.name" , remotename ) ))
	     ||  (!(dptr=occi_request_element(rptr,"occi.file.type",  type       ) )))
	{
		rptr = occi_remove_request( rptr );
		return(1102);
	}
	else
	{
		zptr = occi_client_post( cptr, rptr );
		cosacs_synchronise();
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
	char	duffer[2048];
	int	length=0;

	if ( check_debug() )
	{
		sprintf(duffer,"start::cosacs::metadata(%s,%s)",cosacs,contract);
		rest_log_debug( duffer );
	}

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
	if ( check_debug() )
	{
		rest_log_debug("start::cosacs::metadata::static");
		sprintf(duffer,"cosacs::metadata(%s,%s)","publisher",publisher);
		rest_log_debug( duffer );
	}
	if (!( cosacs_create_metadata( cosacs, (char *) 0, "publisher", publisher ) ))
	{
		if ( check_debug() )
		{
			sprintf(duffer,"cosacs::metadata(%s,%s)","contract",contract);
			rest_log_debug( duffer );
		}
		if (!( cosacs_create_metadata( cosacs, (char *) 0, "contract",  contract  ) ))
		{
			if ( check_debug() )
			{
				sprintf(duffer,"cosacs::metadata(%s,%s)","cosacs",cosacs);
				rest_log_debug( duffer );
			}
			if (!( cosacs_create_metadata( cosacs, (char *) 0, "cosacs",    cosacs    ) ))
				rest_log_debug("end::cosacs::metadata::static");
			else	rest_log_debug("error::cosacs::metadata::cosacs");
		}
		else	rest_log_debug("error::cosacs::metadata::contract");
	}
	else	rest_log_debug("error::cosacs::metadata::publisher");

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

		if ( check_debug() )
		{
			sprintf(duffer,"cosacs::metadata::instruction(%s)",buffer);
			rest_log_debug( duffer );
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
					cosacs_create_script( cosacs,_COSACS_RUN, jptr->value, fptr->value );
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
					(void) cosacs_create_metadata( cosacs, fptr->value, gptr->value, jptr->value );

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

	if ( check_debug() )
	{
		sprintf(duffer,"cosacs::script::create(%s,%s)",contract,"process");
		rest_log_debug( duffer );
	}

	/* ---------------------------------------- */
	/* sync disks after script actions then run */
	/* ---------------------------------------- */
	cosacs_create_script( cosacs,_COSACS_RUN,   "sync",   "command" );
	cosacs_create_script( cosacs,_COSACS_START, contract, "process" );

	if ( check_debug() )
	{
		sprintf(duffer,"finish::cosacs::metadata(%s,%s)",cosacs,contract);
		rest_log_debug( duffer );
	}
	return(0);
}

	/* ------------- */
#endif	/* _cosacsctrl_c */
	/* ------------- */


