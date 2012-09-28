/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2012 by Oasis (INRIA Sophia Antipolis) and ActiveEon teams.     */
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

#ifndef _pa_client_c
#define _pa_client_c

#include "paclient.h"
#include "tlsconfig.h"
#include "tlsload.h"

private	struct pa_config Wpa = {
	(char *) 0,
	(char *) 0,

	(char *) 0,
	(char *) 0,

	(char *) 0,
	(char *) 0,

	(char *) 0,
	(char *) 0,

	(char *) 0,
	(char *) 0,

	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,

	0

	};

private struct jvm_struct * jvmp = NULL;

/*	------------------------------------------------------------	*/
/*		l i b e r a t e _ p a _ r e s p o n s e			*/
/*	------------------------------------------------------------	*/
/*! Free the pa_response structure. */
public	struct pa_response * liberate_pa_response( struct pa_response * rptr )	
{
	if ( rptr )
	{
		if ( rptr->response )
			rptr->response = liberate_rest_response( rptr->response );
		if ( rptr->jsonroot )
			rptr->jsonroot = drop_data_element( rptr->jsonroot );
		if ( rptr->xmlroot )
			rptr->xmlroot = document_drop( rptr->xmlroot );
		if ( rptr->content )
			liberate( rptr->content );
		liberate( rptr );
	}
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*		 		p a _ c h e c k 			*/
/*	------------------------------------------------------------	*/
/*! Translate (if possible) a rest_response structure into a pa_response one. */
private	struct	pa_response * pa_check( struct rest_response * aptr )
{
	struct	pa_response * rptr=(struct pa_response *) 0;
	struct	rest_header * hptr=(struct rest_header *) 0;;
	if ( aptr )
	{
		if ( check_verbose() )
		{
		printf("   PA Client Response : %s %u %s\n",aptr->version,aptr->status,aptr->message);

			if ( check_debug() )
			{
				for (	hptr=aptr->first;
					hptr != (struct rest_header *) 0;
					hptr = hptr->next )
				{
					if (!( hptr->name )) 
						continue;
					else	printf("   %s: %s\n",hptr->name,hptr->value);
				}
				if ( aptr->body )
				{
					printf("   [ %s ] \n",aptr->body );
				}
			}
		}
	}
	if (!( rptr = allocate( sizeof( struct pa_response ) ) ))
		return( rptr );
	else
	{
		rptr->nature = _TEXT_NONE;
		rptr->content= (char *) 0;
		rptr->xmlroot = (struct xml_element *) 0;
		rptr->jsonroot = (struct data_element *) 0;
		rptr->response = aptr;
		if (!( aptr->body ))
			return(rptr);
		if (!( hptr = rest_resolve_header( aptr->first, "Content-Type" ) ))
			return(rptr);
		else if (!( rptr->content = allocate_string( hptr->value ) ))
			return( rptr );
		else if ((!( strcmp( rptr->content, "text/json" 	)))
		     ||  (!( strcmp( rptr->content, "application/json" 	)))
		     ||  (!( strcmp( rptr->content, "x-application/json"))))
		{
			rptr->nature = _TEXT_JSON;
			rptr->jsonroot = json_parse_file( aptr->body );
			return( rptr );
		}
		else if ((!( strcmp( rptr->content, "text/xml" 		)))
		     ||  (!( strcmp( rptr->content, "application/xml" 	)))
		     ||  (!( strcmp( rptr->content, "x-application/xml"))))
		{
			rptr->nature = _TEXT_XML;
			rptr->xmlroot = document_parse_file( aptr->body );
			return( rptr );
		}
		else	return( rptr );
		
	}
}

/*	------------------------------------------------------------	*/
/*		 p a _ c l i e n t _ g e t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response * 
	pa_client_get_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	return( pa_check( rest_client_get_request( target, tls, nptr, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*	      p a _ c l i e n t _ d e l e t e _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response * 
	pa_client_delete_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	return( pa_check( rest_client_delete_request( target, tls, nptr, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*		 p a _ c l i e n t _ h e a d _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response * 
	pa_client_head_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	return( pa_check( rest_client_head_request( target, tls, nptr, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*		 p a _ c l i e n t _ p o s t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response * 
	pa_client_post_request(
		char * target, char * tls, char * nptr, char * filename, struct rest_header * hptr )
{
	return( pa_check( rest_client_post_request( target, tls, nptr, filename, hptr ) ) );
}

/*	------------------------------------------------------------	*/
/*		 p a _ c l i e n t _ p u t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response * 
	pa_client_put_request(
		char * target, char * tls, char * nptr, char * filename, struct rest_header * hptr )
{
	return( pa_check( rest_client_put_request( target, tls, nptr, filename, hptr ) ) );
}

/*! 
 * Prepare rest header for authentication. */
public	struct	rest_header   *	pa_authenticate	( )
{
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	if (!( Wpa.user ))
		return( hptr );
	else if (!( Wpa.password ))
		return( hptr );
	else if (!( Wpa.version ))
		return( hptr );
	else if (!( hptr = rest_create_header( "x-ms-version", Wpa.version ) ))
		return( hptr );
	else if (!( hptr->next = rest_create_header( _HTTP_CONTENT_TYPE, "text/xml" ) ))
		return( hptr );
	else	return((hptr->next->previous = hptr));
}

/*	------------------------------------------------------------	*/
/*		p r o a c t i v e  _ c r e a t e _ o p e r a t i o n		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	proactive_create_operation( char * action, char * filename )
{
	struct	pa_response	*	rptr=(struct pa_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	if (!( hptr = pa_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Wpa.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, action ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = pa_client_post_request( nptr, Wpa.tls, Wpa.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		p r o a c t i v e  _ l i s t _ o p e r a t i o n		*/
/*	------------------------------------------------------------	*/
/*! Put a raw string list into a pa_response structure. */
private	struct	pa_response * proactive_list_operation( char * buffer )
{
	struct	pa_response	*	rptr=(struct pa_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	if (!( hptr = pa_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Wpa.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = pa_client_get_request( nptr, Wpa.tls, Wpa.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		p r o a c t i v e  _ re t r i e v e _ o p e r a t i o n		*/
/*	------------------------------------------------------------	*/
private	struct	pa_response * proactive_retrieve_operation( char * buffer )
{
	struct	pa_response	*	rptr=(struct pa_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	if (!( hptr = pa_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Wpa.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = pa_client_get_request( nptr, Wpa.tls, Wpa.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		p r o a c t i v e  _ d e l e t e _ o p e r a t i o n		*/
/*	------------------------------------------------------------	*/
private	struct	pa_response * proactive_delete_operation( char * buffer )
{
	struct	pa_response	*	rptr=(struct pa_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	if (!( hptr = pa_authenticate() ))
		return( rptr );
	else if (!( uptr = analyse_url( Wpa.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = pa_client_delete_request( nptr, Wpa.tls, Wpa.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		p a _ l i s t _ o p e r a t i o n s			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_list_operations(char * start,char * end)
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*		p a _ l i s t _ W A T M _ p r o f i l e s		*/
/*	------------------------------------------------------------	*/
//public	struct	pa_response *	pa_list_WATM_profiles()
//{
//	return((struct pa_response *) 0);
//}

/*	------------------------------------------------------------	*/
/*		p a _ l i s t _ W A T M _ definitions			*/
/*	------------------------------------------------------------	*/
//public	struct	pa_response *	pa_list_WATM_definitions(char * profile )
//{
//	return((struct pa_response *) 0);
//}

/*	------------------------------------------------------------	*/
/*	     p a _ c r e a t e _ h o s t e d _ s e r v i c e 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_create_hosted_service( char * filename )
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*	   p a _ r e t r i e v e _ h o s t e d _ s e r v i c e 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_retrieve_hosted_service( char * filename )
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*	     p a _ u p d a t e _ h o s t e d _ s e r v i c e 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_update_hosted_service( char * filename )
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*	     p a _ d e l e t e _ h o s t e d _ s e r v i c e 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_delete_hosted_service( char * filename )
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*		p a _ l i s t _ h o s t e d _ s e r v i c e s		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_list_hosted_services()
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*	     p a _ c r e a t e _ c e r t i f i c a t e 			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_create_certificate( char * filename )
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*	   p a _ r e t r i e v e _ c e r t i f i c a t e 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_retrieve_certificate( char * filename )
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*	     p a _ d e l e t e _ c e r t i f i c a t e 			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_delete_certificate( char * filename )
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*		p a _ l i s t _ c e r t i f i c a t e s			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_list_certificates(char * server)
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*			p a _ l i s t _ l o c a t i o n s 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_list_locations()
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*			p a _ l i s t _ s e r v e r s			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_list_servers	( )
{
	return((struct pa_response *) 0);
}

private char * put_jsonstring_in_file(char * jsontext){
    char * tempfile = rest_temporary_filename("txt");

    FILE* file = (FILE*) NULL;
    if (!( file = fopen(tempfile,"wb" ))) {
        fprintf(stderr, "Cannot open file %s...\n", tempfile);
        free(tempfile);
        return NULL;
    } else if ( fwrite(jsontext,strlen(jsontext),1,file) <= 0 ) {
        fprintf(stderr, "Cannot write file %s...\n", tempfile);
        free(tempfile);
        fclose(file);
        return NULL;
    }else{
        fclose(file);
        return( tempfile);
    }
}


/*
void start_provider_if_needed(){
	if (jvmp==NULL){
		char ** args = (char**)malloc(2 * sizeof(char*));
		args[0] = Wpa.user;
		args[1] = Wpa.password;
		fprintf(stderr, "Using user='%s', demo='%s'...\n", args[0], args[1]);
		jvmp = start_jvm();
		connect_to_provider(jvmp, args);
	}else{
	}
}
*/


/*	------------------------------------------------------------	*/
/*			p a _ c r e a t e _ s e r v e r                         */
/*	------------------------------------------------------------	*/
/** 
 * Lock a ProActive node using as parameters the constraints given.  
 * @param constr constraints for the node to provide.
 * @return the parsed information about the reservation process.
 */
public	struct	pa_response * pa_create_server(struct proactive * constr)
{
	char * filename = NULL;
	char * raw_list = NULL;

	struct pa_response* result = (struct pa_response*) NULL;

	if (constr == NULL){    // Check valid category instance. 
		fprintf(stderr, "Invalid category instance...\n");
		return NULL;
	}

	if (!(result = (struct pa_response*) malloc(sizeof(struct pa_response)))){
		return NULL;
	}else if (!(raw_list = start_server(&Wpa, &jvmp, constr))){
		fprintf(stderr, "Problem making call to the java layer...\n");
		free(result);
		return NULL;
	}else{
		result->nature = _TEXT_JSON;
		result->content = raw_list;
		result->xmlroot = NULL;
		if (!(filename = put_jsonstring_in_file(raw_list))){
			fprintf(stderr, "Problem putting the json in a file...\n");
		}else{
			result->jsonroot = json_parse_file(filename);
			result->response = NULL;
		}
	}
	return(result);
}
	
/*	------------------------------------------------------------	*/
/*			p a _ d e l e t e _ s e r v e r                         */
/*	------------------------------------------------------------	*/
/*! Unlock one ProActive node from the Scheduler/RM. */
public	struct	pa_response *	pa_delete_server(struct proactive * constr)
{
	char * filename = NULL;
	char * raw_list = NULL;
	
	//start_provider_if_needed();

	struct pa_response* result = (struct pa_response*) NULL;

	if (constr == NULL){    // Check valid category instance. 
		fprintf(stderr, "Invalid category instance...\n");
		return NULL;
	}

	if (!(result = (struct pa_response*) malloc(sizeof(struct pa_response)))){
		return NULL;
	}else if (!(raw_list = stop_server(&Wpa, &jvmp, constr))){
		fprintf(stderr, "Problem making call to the java layer...\n");
		free(result);
		return NULL;
	}else{      // Prepare the result to return. 
		result->nature = _TEXT_JSON;
		result->content = raw_list;
		result->xmlroot = NULL;
		if (!(filename = put_jsonstring_in_file(raw_list))){
			fprintf(stderr, "Problem putting the json in a file...\n");
		}else{
			result->jsonroot = json_parse_file(filename);
			result->response = NULL;
		}
	}
	return(result);
}

/*	------------------------------------------------------------	*/
/*			p a _ g e t _ s u b s c r i p t i o n		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_get_subscription()
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*			p a _ l i s t _ d e p l o y m e n t s			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_list_deployments(char * server)
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*				p a _ g e t _ d e p l o y m e n t 			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_get_deployment( char * server, char * slot )
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*			p a _ d e l e t e _ d e p l o y m e n t 			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_delete_deployment( char * server, char * slot )
{
	return((struct pa_response *) 0);
}

/*	------------------------------------------------------------	*/
/*			p a _ u p d a t e _ s e r v e r 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_update_server(  char * id, char * filename )
{
	return((struct pa_response *) 0);
}


/*	------------------------------------------------------------	*/
/*		p a _ i n i t i a l i s e _ c l i e n t 		*/
/*	------------------------------------------------------------	*/
public	int	pa_initialise_client( 
		char * user, char * password, 
		char * host, char * agent, 
		char * version, char * tls,
		char * ns, char * subscription )
{
	if (!( Wpa.user = allocate_string( user )))
		return( 27 );
	if (!( Wpa.password = allocate_string( password )))
		return( 27 );
	if (!( Wpa.host = allocate_string( host )))
		return( 27 );
	else if (!( Wpa.agent = allocate_string( agent )))
		return( 27 );
	else if (!( Wpa.version = allocate_string( version )))
		return( 27 );
	else if (!( Wpa.namespace = allocate_string( ns ) ))
		return( 27 );
	else if (!( Wpa.subscription = allocate_string( subscription ) ))
		return( 27 );

	else if (!( Wpa.base = allocate( strlen( Wpa.host ) + strlen( Wpa.subscription ) + 16 ) ))
		return( 27 );
	else	sprintf(Wpa.base,"%s%s",Wpa.host,Wpa.subscription);

	Wpa.authenticate= (char *) 0;
	Wpa.tls = tls;
	return( 0 );
}


	/* ------------ */
#endif	/* _pa_client_c */
	/* ------------ */


