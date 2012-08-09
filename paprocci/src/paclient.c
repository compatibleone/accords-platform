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


/*! Function that calls the Java procci layer. */
public void pa_jvm_create_node()
{ 

	char* result = NULL;                   // output of the execution of the command 

	fprintf(stderr, "JVM Creating node...");
	if (jvmp==NULL){
		// start the JVM
		jvmp = startjvm();
		connect_to_provider(jvmp);
	}
	start_server(jvmp);

}

/*! Function that calls the Java procci layer. */
public char * pa_java_procci_call(char * specific_parameters)
{ 
    #define RESULT_SIZE 1024*8

    char* javaprocci = NULL;               // example: /home/mjost/Projects/gitorious/proactive/procci/co-proactive-procci/procci "
    char* general_parameters = NULL;       // example: --host xx -- user xx --pass xx 
    char* all_command = NULL;              // all the command together, including the specific_parameters
    char* result = NULL;                   // output of the execution of the command 

    fprintf(stderr, "Calling the Java ProActive Procci...");
    if      (!(javaprocci = getenv("PA_PROCCI_PATH"))){
        fprintf(stderr, "Environment variable PA_PROCCI_PATH not defined...");
        return NULL;
    }else if(!(general_parameters = allocate(100 + strlen(Wpa.user) + strlen(Wpa.password)))){
        return NULL;
    }else if(sprintf(general_parameters, " --user %s --pass %s ", Wpa.user, Wpa.password)<0){
        return NULL;
    }else if((all_command = allocate(strlen(javaprocci) + strlen(general_parameters) + strlen(specific_parameters) + 16))==0){
        return NULL;
    }else if(sprintf(all_command, "%s %s %s", javaprocci, general_parameters, specific_parameters)<0){
        return NULL;
    }else{
        FILE* pf = NULL;
        size_t size = 0;

        fprintf(stderr, "ProActive Java call: '%s'\n", all_command);

        pf = popen(all_command, "r");
        if (!pf){
            fprintf(stderr, "Could not open pipe for output.\n");
            return NULL;
        }

        ssize_t bytes_read = getdelim(&result, &size, '\0',  pf);
        if(bytes_read != -1){
            // ok
            fprintf(stderr, "Java-Procci stdout: %s", result);
        }else{
            result = NULL;
            fprintf(stderr, "Java-Procci stdout: <empty>");
        }

        if (pclose(pf) !=0){
            fprintf(stderr, "Error: Failed to close command stream.\n");
        }
        return result;
    }

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


/*	------------------------------------------------------------	*/
/*			p a _ l i s t _ s e r v e r s			*/
/*	------------------------------------------------------------	*/
/*! List nodes available. */
public	struct	pa_response *	pa_list_servers	( )
{
    //printf("ProActive Listing servers (ProActive nodes)... Using host %s  user %s  pass %s  version %s\n", Wpa.host, Wpa.user, Wpa.password, Wpa.version);
    char * filename = NULL;
    char * raw_list = NULL;

    struct pa_response* result = (struct pa_response*) NULL;
    if (!(result = (struct pa_response*) malloc(sizeof(struct pa_response)))){
        return NULL;
    }else if (!(raw_list = pa_java_procci_call("--list-nodes"))){ // Parameters for java layer. 
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
	//return( proactive_list_operation( "/services/hostedservices" ) );
}


/*	------------------------------------------------------------	*/
/*			p a _ c r e a t e _ s e r v e r                         */
/*	------------------------------------------------------------	*/
/*! 
 * Lock a ProActive node using as parameters the constraints given.  */
public	struct	pa_response * pa_create_server(struct proactive * constr)
{
    char * filename = NULL;
    char * raw_list = NULL;
    char command[1024];

    printf("Simulating creation of a node...\n");
    pa_jvm_create_node();
    printf("Returning a NULL node...\n");

    return NULL;

    struct pa_response* result = (struct pa_response*) NULL;
    sprintf(command,"--get-cosacs --select-by-os %s", constr->image); // Parameters for java layer. 

    if (!(result = (struct pa_response*) malloc(sizeof(struct pa_response)))){
        return NULL;
    }else if (!(raw_list = pa_java_procci_call(command))){
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
/*			p a _ g e t _ s u b s c r i p t i o n		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_get_subscription()
{

	return( proactive_list_operation( "" ));
}

/*	------------------------------------------------------------	*/
/*			p a _ l i s t _ d e p l o y m e n t s			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_list_deployments(char * server)
{
	char	buffer[1024];
	sprintf(buffer,"/services/hostedservices/%s/deploymentslots" , server ); 
	return( proactive_list_operation( buffer ) );
}


/*	------------------------------------------------------------	*/
/*				p a _ g e t _ d e p l o y m e n t 			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_get_deployment( char * server, char * slot )
{
	char	buffer[1024];
	sprintf(buffer,"/services/hostedservices/%s/deploymentslots/%s" , server, slot ); 
	return( proactive_retrieve_operation( buffer ) );
}

/*	------------------------------------------------------------	*/
/*			p a _ d e l e t e _ d e p l o y m e n t 			*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_delete_deployment( char * server, char * slot )
{
	char	buffer[1024];
	sprintf(buffer,"/services/hostedservices/%s/deploymentslots/%s" , server, slot ); 
	return( proactive_delete_operation( buffer ) );
}

/*! 
 * Get information about one node already locked
 * and returns this information. */
public	struct	pa_response *	pa_get_server	(  char * id )
{

	// information used before:  nptr, Wpa.tls, Wpa.agent, hptr 

    char * filename = NULL;
    char * raw_list = NULL;
    char command[1024];

    struct pa_response* result = (struct pa_response*) NULL;
    if (id != NULL){
        sprintf(command,"-y list_information_about_this_node %s" , id ); 
    }else{
        fprintf(stderr, "Invalid id for the server...\n");
        return NULL;
    }

    if (!(result = (struct pa_response*) malloc(sizeof(struct pa_response)))){
        return NULL;
    }else if (!(raw_list = pa_java_procci_call(command))){
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
            fprintf(stderr, "Filename: %s\n", filename);
            result->jsonroot = json_parse_file(filename);
            result->response = NULL;
        }
    }
	return(result);
}


/*	------------------------------------------------------------	*/
/*			p a _ u p d a t e _ s e r v e r 		*/
/*	------------------------------------------------------------	*/
public	struct	pa_response *	pa_update_server(  char * id, char * filename )
{
	struct	pa_response	*	rptr=(struct pa_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/services/hostedservices/%s",id);
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
	else if (!( rptr = pa_client_put_request( nptr, Wpa.tls, Wpa.agent,filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			p a _ d e l e t e _ s e r v e r                         */
/*	------------------------------------------------------------	*/
/*! Unlock one ProActive node from the Scheduler/RM. */
public	struct	pa_response *	pa_delete_server(  char * id )
{
    char * filename = NULL;
    char * raw_list = NULL;
    char command[1024];

    struct pa_response* result = (struct pa_response*) NULL;
    if (id != NULL){    // Check valid id. 
        sprintf(command,"--release-node %s" , id );  // Build command to call. 
    }else{
        fprintf(stderr, "Invalid name for the server...\n");
        return NULL;
    }

    if (!(result = (struct pa_response*) malloc(sizeof(struct pa_response)))){
        return NULL;
    }else if (!(raw_list = pa_java_procci_call(command))){ // Call the java layer with the "delete" command. 
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


