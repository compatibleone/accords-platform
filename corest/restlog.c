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
#ifndef	_rest_log_c
#define	_rest_log_c

#include "rest.h"
#include "url.h"

#define	_DEFAULT_LOGFILE	"./co-log"

#define	_OPTIMISE_LOG		4
#define	_LOG_TO_FILE		1
#define	_LOG_TO_COMONS		2

private	char *	restlogagent=(char *) 0;
private	char *	restlogname=(char *) 0;
private	char *	restcomons=(char *) 0;
private	int	restlogmons=0;
private	int	restlognest=0;

public	char *	occi_resolve_category_provider( char * c, char * a, char * t );

/*	---------------------------------------------------------	*/
/*	    r e s t _ l o g _ c o m o n s _ i d e n t i t y		*/
/*	---------------------------------------------------------	*/
/*	this function allows optimisation of the COMONS reference	*/
/*	resolution when required by the configuration of the log	*/
/*	subsystem via the "monitor" value of the accords config.	*/
/*	---------------------------------------------------------	*/
public	char *	rest_log_comons_identity(char * category, char * agent, char * tls)
{
	char *	result;
	if ( restcomons )
		return( allocate_string( restcomons ) );
	else if (!( result = occi_resolve_category_provider( category, agent, tls ) ))
		return( result );
	else if (!( restlogmons & _OPTIMISE_LOG ))
		return( result );
	else	return( allocate_string( (restcomons=result) ) );
}

/*	---------------------------------------------------------	*/
/*			r e s t _ l o g _ a g e n t 			*/
/*	---------------------------------------------------------	*/
public	void	rest_log_agent( char * aname )
{
	restlogagent = aname;
	return;
}

/*	---------------------------------------------------------	*/
/*		r e s t _ i n i t i a l i s e _ l o g 			*/
/*	---------------------------------------------------------	*/
public	void	rest_initialise_log( int state )
{
	restlogmons = state;
	return;
}

/*	---------------------------------------------------------	*/
/*		r e s t _ s e t _ l o g f i l e _ n a m  e		*/
/*	---------------------------------------------------------	*/
public	int	rest_set_logfile_name( char * nptr )
{
	if ( restlogname )
		restlogname = liberate( restlogname );
	if (!( nptr ))
		return(0);
	else if (!( restlogname = allocate_string( nptr )))
		return( 27 );
	else	return( 0 );
}

/*	---------------------------------------------------------	*/
/*		r e s t _ g e t _ l o g f i l e _ n a m  e		*/
/*	---------------------------------------------------------	*/
private	char *	rest_get_logfile_name()
{
	if (!( restlogname ))
		if ( rest_set_logfile_name( _DEFAULT_LOGFILE ) != 0)
			return( (char *) 0 );
	return( restlogname );
}

/*	---------------------------------------------------------	*/
/*		    r e s t _ l o g _ c o m o n s  			*/
/*	---------------------------------------------------------	*/
public 	int	rest_log_comons( char * buffer, char * nature )
{
	restlognest++;
	occi_post_event( buffer, nature, ( restlogagent ? restlogagent : "unknown") );
	restlognest--;
	return(0);
}

/*	---------------------------------------------------------	*/
/*			r e s t _ t h r e a d _ i d 			*/
/*	---------------------------------------------------------	*/
private	int	rest_thread_id()
{
	pthread_t	t;
	t = pthread_self();
	return((int) t);
}

/*	---------------------------------------------------------	*/
/*		    r e s t _ l o g _ f i l e 				*/
/*	---------------------------------------------------------	*/
public 	int	rest_log_file( char * buffer )
{
	char *	filename;
	FILE *	h;
	if (!( filename = rest_get_logfile_name() ))
		return(30);
	else if (!( h = fopen( filename,"a") ))
		return(46);
	else
	{
		fprintf(h,"%u:%u:%u: %s\n",(unsigned) time((long *) 0),getpid(),rest_thread_id(),buffer);
		fclose(h);
		return(0);
	}
}

/*	---------------------------------------------------------	*/
/*		    r e s t _ l o g _ f i l e 				*/
/*	---------------------------------------------------------	*/
public 	int	rest_debug_log_file( char * buffer )
{
	if (!( check_debug() ))
		return( 0 );
	else	return( rest_log_file( buffer ) );
}

/*	---------------------------------------------------------	*/
/*		    r e s t _ l o g _ m e s s a g e			*/
/*	---------------------------------------------------------	*/
public 	int	rest_log_message( char * buffer )
{

	if ( restlogmons & _LOG_TO_FILE )
	{
		rest_log_file( buffer );
	}

	if ( restlogmons & _LOG_TO_COMONS )
	{
		if (!( restlognest ))
			rest_log_comons( buffer, "message" );
	}

	return(0);	
}

/*	---------------------------------------------------------	*/
/*		    r e s t _ l o g _ f a i l u r e			*/
/*	---------------------------------------------------------	*/
public 	int	rest_log_failure( char * buffer )
{

	if ( restlogmons & _LOG_TO_FILE )
	{
		rest_log_file( buffer );
	}

	if ( restlogmons & _LOG_TO_COMONS )
	{
		if (!( restlognest ))
			rest_log_comons( buffer, "failure" );
	}

	return(0);	
}

/*	---------------------------------------------------------	*/
/*		    r e s t _ l o g _ w a r n i n g 			*/
/*	---------------------------------------------------------	*/
public 	int	rest_log_warning( char * buffer )
{

	if ( restlogmons & _LOG_TO_FILE )
	{
		rest_log_file( buffer );
	}

	if ( restlogmons & _LOG_TO_COMONS )
	{
		if (!( restlognest ))
			rest_log_comons( buffer, "warning" );
	}

	return(0);	
}

/*	---------------------------------------------------------	*/
/*		    r e s t _ l o g _ t r a n s a c t i o n		*/
/*	---------------------------------------------------------	*/
public 	int	rest_log_transaction( char * buffer )
{

	if ( restlogmons & _LOG_TO_FILE )
	{
		rest_log_file( buffer );
	}

	if ( restlogmons & _LOG_TO_COMONS )
	{
		if (!( restlognest ))
			rest_log_comons( buffer, "transaction" );
	}

	return(0);	
}

/*	---------------------------------------------------------	*/
/*		    r e s t _ c h e c k _ d e b u g 			*/
/*	---------------------------------------------------------	*/
private	int	rest_debug=-1;
public	int	rest_check_debug()
{
	char *	eptr;
	if ( check_debug() )
		return( 1 );
	else if ( rest_debug != -1 )
		return( rest_debug );
	else if (!( eptr = getenv("RESTDEBUG") ))
		return( 0 );
	else	return((rest_debug = atoi(eptr)));
}

/*	---------------------------------------------------------	*/
/*		    r e s t _ l o g _ d e b u g 			*/
/*	---------------------------------------------------------	*/
public 	void	rest_log_debug( char * message )
{
	if ( rest_check_debug() )
		rest_log_message( message );
	return;
}

/*	---------------------------------------------------------	*/
/*		r e s t _ l o g _ h e a d e r 				*/
/*	---------------------------------------------------------	*/
private	int	rest_log_detail=0;

public 	int	rest_log_header( char * agent, struct rest_header * hptr, char * symbol )
{
	char	buffer[4096];
	int	status;
	if (!( rest_log_detail ))
		return( 0 );
	rest_log_agent( agent );
	for (	;
		hptr != (struct rest_header *) 0;
		hptr = hptr->next )
	{
		sprintf(buffer,"%s %s %s: %s ",agent, symbol, hptr->name, hptr->value);
		if ((status = rest_log_message( buffer )) != 0)
			return( status );
	}
	return( 0 );

}

/*	---------------------------------------------------------	*/
/*		r e s t _ l o g _ s e n d _ r e q u e s t		*/
/*	---------------------------------------------------------	*/
public	int	rest_log_send_request ( char * agent, char * method, struct url * uptr )
{
	char	buffer[4096];
	char *	symbol="-->";
	rest_log_agent( agent );
	sprintf(buffer,"%s %s %s %s://%s:%u%s ",agent, symbol, method, 
			( uptr->service ? uptr->service : "http"),
			uptr->host,uptr->port, uptr->object);
	return( rest_log_message( buffer ) );
}

/*	---------------------------------------------------------	*/
/*		r e s t _ l o g _ r e c v _ r e s p o n s e		*/
/*	---------------------------------------------------------	*/
public	int	rest_log_recv_response( char * agent, struct rest_response * aptr )
{
	char	buffer[4096];
	char *	symbol="<--";
	rest_log_agent( agent );
	sprintf(buffer,"%s %s %s %u %s ",agent,symbol, 
		(aptr ? aptr->version : "???" ),
		(aptr ? aptr->status  : -1 ),
		(aptr ? aptr->message : "no response"));
	return( rest_log_message( buffer ) );
}

/*	---------------------------------------------------------	*/
/*		r e s t _ l o g _ s e n d _ r e s p o n s e 		*/
/*	---------------------------------------------------------	*/
public	int	rest_log_send_response( char * agent, struct rest_response * aptr )
{
	char	buffer[4096];
	int	status;
	char *	symbol="-->";
	rest_log_agent( agent );
	sprintf(buffer,"%s %s %s %u %s ",agent,symbol,
		(aptr ? aptr->version : "???" ),
		(aptr ? aptr->status  : -1 ),
		(aptr ? aptr->message : "no response"));
	if ((status = rest_log_message( buffer )) != 0)
		return( status );
	else if (!( rest_log_detail ))
		return( 0 );
	else	return( rest_log_header( agent, aptr->first, symbol ) );
}

/*	---------------------------------------------------------	*/
/*		r e s t _ l o g _ r e c v _ r e q u e s t		*/
/*	---------------------------------------------------------	*/
public	int	rest_log_recv_request ( char * agent, struct rest_request * rptr )
{
	char	buffer[4096];
	int	status;
	char *	symbol="<--";
	rest_log_agent( agent );
	sprintf(buffer,"%s %s %s %s ",agent,symbol, rptr->method, rptr->object );
	if ((status = rest_log_message( buffer )) != 0)
		return( status );
	else if (!( rest_log_detail ))
		return( 0 );
	else 	return( rest_log_header( agent, rptr->first, symbol ) );
}


	/* ---------- */
#endif	/* _restlog_h */
	/* ---------- */


