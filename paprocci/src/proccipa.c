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

#ifndef	_proccipa_c
#define	_proccipa_c

#include "paclient.h"
#include "cordslang.h"

#include "proactive.h"
#include "proactive.c"
#include "occiproactive.c"

/*! 
 * Return the occi_kind_node content such that its name matches the given string parameter. 
 * The content is always a pa_config*.  
 * \param sptr name string to match.
 * \return the matching content of the occi kind list. */
private	struct	pa_config * resolve_pa_configuration( char * sptr )
{
	struct	occi_kind_node * nptr;
	struct	pa_config * pptr=(struct pa_config *) 0;        // Resulting list. 
	struct	occi_kind_node  * occi_first_pa_config_node();  // This function is somewhere defined. 
	for (	nptr = occi_first_pa_config_node();             // Get the already existing list of occi kind nodes for this procci. 
		nptr != (struct occi_kind_node *) 0;                // Traverse them all...
		nptr = nptr->next )                                 // sptr string name, nptr occi kind node, pptr content of the node. 
	{
		if (!( pptr = nptr->contents ))                     // If the current one has no contents, and inside it has no name, skip it. 
			continue;
		else if (!( pptr->name ))                           
			continue;
		else if (!( strcmp( pptr->name, sptr ) ))           // If the name of the current contents of the occi kind node matches the given string... Return the content.
			break;
	}
	return( pptr );
}

/*! 
 * Loads ProActive account configuration, like user,
 * password, host, etc. */
private	int	use_proactive_configuration( char * sptr )
{
	struct	pa_config * pptr;

	if (!( pptr = resolve_pa_configuration( sptr ))) // Get from the procci occi kinds list, the node that matches the given string sptr.
	 	return( 404 );

	else 	return( pa_initialise_client(           // Initialize the current client with the information obtained. 
			pptr->user, pptr->password, 
			pptr->host, pptr->agent, pptr->version, pptr->tls,
			pptr->namespace, pptr->subscription ));
}

/*!
 * Reset the structure that represents the proactive server. */
private	int	reset_structure_proactive_server( struct proactive * pptr )
{
	if ( pptr )
	{
		if ( pptr->number ) pptr->number = liberate( pptr->number );
		if ( pptr->hostname ) pptr->hostname = liberate( pptr->hostname );
		if ( pptr->reference ) pptr->reference = liberate( pptr->reference );
		if ( pptr->publicaddr ) pptr->publicaddr = liberate( pptr->publicaddr );
		if ( pptr->privateaddr ) pptr->privateaddr = liberate( pptr->privateaddr );
		pptr->number = allocate_string("");
		pptr->hostname = allocate_string("");
		pptr->publicaddr = allocate_string("");
		pptr->nopanodes = allocate_string("");
		pptr->status = _OCCI_IDLE;
	}
	return(0);
}

/*! Check if the output of the pa_delete_server function was correct. */
private	int	parse_delete_server_response( struct pa_response * server_created)
{
	char *	vptr; // Auxiliar variable. 

    // Checking that the field error is not there. 
    if (vptr = json_atribut( server_created->jsonroot, "error")) // Obtaining the error of the operation (if present, there was an error). 
    {
        fprintf(stderr, "An error occured while contacting ProActive: '%s'...\n", vptr);
        return( 27 );
    }

    return (0); 

}

/*	--------------------------------------------------------	*/
/* 	     c o n n e c t _ p r o a c t i v e _ s e r v e r		*/
/*	--------------------------------------------------------	*/
/*! This function fills in the struct proactive* given by usign the information that lies into 
 * the pa_response structure + calling the server to get info about the ProActive node. 
 * It also frees some proactive* structure fields. */
private	int	parse_create_server_response( struct pa_response * server_created,struct proactive * server_data )
{
	struct	pa_response * zptr;
	struct	pa_response * yptr;
	char *	vptr; // Auxiliar variable. 
    // MMM check it.
	if (!( server_data ))
		return( 118 );
	else
	{
		/* ---------------------------------------------------- */
		/* we must now analyse the response from the open stack	*/
		/* and collect the valuable data : admin pass is only 	*/
		/* available at this point and cannot be retrieved by	*/
		/* any other means so it must not be lost.		*/ 
		/* ---------------------------------------------------- */

        // If the analysed pointers were pointing to something, free that something. 
		if ( server_data->number ) 
			server_data->number = liberate( server_data->number );

        // Checking that the field error is not there. 
		if (vptr = json_atribut( server_created->jsonroot, "error")) // Obtaining the error of the operation (if present, there was an error). 
		{
			fprintf(stderr, "An error occured while contacting ProActive: '%s'...\n", vptr);
			reset_structure_proactive_server( server_data );
			return( 27 );
		}

        // Filling in the server_data structure given. 
		if (!( vptr = json_atribut( server_created->jsonroot, "id") )) // Obtaining the ID of the operation. 
		{
			reset_structure_proactive_server( server_data );
			return( 27 );
		}
		else if (!( server_data->number = allocate_string(vptr))) // Putting the ID of the ProActive node in proactive*->number.
		{
			reset_structure_proactive_server( server_data );
			return( 27 );
		}

		autosave_proactive_nodes();
		/* ----------------------------------------------------- */
		/* we must now await ACTIVE status to be able to collect */
		/* the final identification information to complete the  */
		/* proactive provisioning request.			 */
		/* ----------------------------------------------------- */
		yptr = server_created;
		zptr = (struct pa_response *) 0;
		while (1)
		{
			if (!( vptr = json_atribut( yptr->jsonroot, "status" ))) // Get the status of the node. 
			{
				reset_structure_proactive_server( server_data );
				return( 27 );
			}
			else if (!( strcmp( vptr, "RUNNING" )))         // Check if the node is in Running (correct) status, if so, exit the loop. 
				break;
			else                  				// Else... 
			{
				reset_structure_proactive_server( server_data );
				return( 555 );
			}
		}
		if ( server_data->hostname ) server_data->hostname = liberate( server_data->hostname );
		if ( server_data->reference ) server_data->reference = liberate( server_data->reference );
		if ( server_data->publicaddr ) server_data->publicaddr = liberate( server_data->publicaddr );
		if ( server_data->privateaddr ) server_data->privateaddr = liberate( server_data->privateaddr );
		if (!( vptr = json_atribut( yptr->jsonroot, "hostname") ))    // Get the hostname. 
		{
			reset_structure_proactive_server( server_data );
			return( 27 );
		}
		else if (!( server_data->reference = allocate_string(vptr)))       // Put the hostId in the reference field. 
		{
			reset_structure_proactive_server( server_data );
			return( 27 );
		}
		if (( vptr = json_atribut( yptr->jsonroot, "privateip")) != (char *) 0) // Get the "private" json field. 
		{
			if (!( server_data->privateaddr  = allocate_string(vptr)))               // Put in privateaddr the private ip address. 
			{
				reset_structure_proactive_server( server_data );
				return( 27 );
			}
		}
		if (( vptr = json_atribut( yptr->jsonroot, "publicip")) != (char *) 0) // Get the "public" json field. 
		{
			if (!( server_data->publicaddr  = allocate_string(vptr)))               // Put in proactive*->publicaddr the public ip address of the ProActive node. 
			{
				reset_structure_proactive_server( server_data );
				return( 27 );
			}
		}
		if (( server_data->publicaddr ) && ( strlen( server_data->publicaddr ) != 0))     // Check if there is a way of accessing the node (either publicip, or hostname).
		{                                                                   // and put it in the hostname field. 
			if (!( server_data->hostname = allocate_string( server_data->publicaddr ) ))
			{
				reset_structure_proactive_server( server_data );
				return( 27 );
			}
		}
		else if (( server_data->privateaddr ) && ( strlen( server_data->privateaddr ) != 0)) // Same as before, but with the privateip. 
		{
			if (!( server_data->hostname = allocate_string( server_data->privateaddr ) ))
			{
				reset_structure_proactive_server( server_data );
				return( 27 );
			}
		}
		server_data->when = time((long *) 0);
		server_data->status = _OCCI_RUNNING;
		autosave_proactive_nodes();
		return(0);
	}
}

/*	----------------------------------------*/
/* 	     s t a r t _ p r o a c t i v e	    */
/*	----------------------------------------*/
/*! 
 * This function locks one ProActive node. */
private	struct	rest_response * start_proactive(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )       // It is void* , but is casted to a struct proactive* since the request comes
                                    // from the occiserver, which receives something generic (related maybe to OpenStack, or OpenNebula, or any other).
{
	struct	pa_response * paptr;
	struct	proactive * pptr;
	int	status;
	char	reference[512];

	// Create jobject complex type to pass it to Java. 
	//   rest_request
	//   rest_response
	// Fill it in. 
	
	//create_java_rest_request_object(rptr);

	printf("start_proactive\n");
	if (!( pptr = vptr ))							// Can't provide null.
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->status != _OCCI_IDLE )     {
		return( rest_html_response( aptr, 200, "OK" ) );
	}else if ((status = use_proactive_configuration( pptr->profile )) != 0){// Set up configuration according to profile (where the user and pass are).
		printf("ERROR Profile '%s' not found. \n", pptr->profile);
		return( rest_html_response( aptr, status, "Not Found" ) );
	}else if (!( paptr = pa_create_server(rptr, aptr, pptr))){		// Request of a node using constraints. 
	 	return( rest_html_response( aptr, 400, "Bad Request" ) );
	}else
	{
		/* ------------------------------- */
		/* parse data obtained from server */
		/* ------------------------------- */
		status = parse_create_server_response( paptr, pptr ); // Returns 0 if everything okay.
		paptr = liberate_pa_response( paptr );
		if (status) // If something went wrong (status != 0)
		{
			fprintf(stderr, "Something went wrong connecting to proactive %d...\n", status);
			return( rest_html_response( aptr, 4256, "Server Failure : Connect ProActive" ) );
		}
		sprintf(reference,"%s/%s/%s",WpaProcci.identity,_CORDS_PROACTIVE,pptr->id);
		/* ---------------------------- */
		/* launch the COSACS operations */
		/* ---------------------------- */
		cosacs_metadata_instructions( 
			pptr->hostname, _CORDS_CONFIGURATION,
			reference, WpaProcci.publisher );

		if (!( status ))  
		{
			return( rest_html_response( aptr, 200, "OK" ) );
		}
		else
		{    
			return( rest_html_response( aptr, 4256, "Server Failure : Connect ProActive" ) );
		}
    }


}

/*	-------------------------------------------	*/
/* 	      s a v e  _ p r o a c t i v e	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * save_proactive(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
    return( rest_html_response( aptr, 404, "Invalid Action" ) ); 
}

/*	--------------------------------------------------------	*/
/* 	 s t o p  _ p r o a c t i v e _ p r o v i s i o n i n g  	*/
/*	--------------------------------------------------------	*/
private	struct pa_response *	stop_proactive_provisioning2( struct proactive * pptr )
{
	int	status;
	struct	pa_response * osptr;

	if ((status = use_proactive_configuration( pptr->profile )) != 0)
		return((struct pa_response *) 0);
	else
	{
		/* ---------------------------- */
		/* launch the COSACS operations */
		/* ---------------------------- */
		cosacs_metadata_instructions( 
			pptr->hostname, _CORDS_RELEASE,
			"reference", WpaProcci.publisher );

		//remove_floating_address( pptr );
		return( osptr );
	}
}

/*	-------------------------------------------	*/
/* 	      s t o p  _ p r o a c t i v e	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * stop_proactive(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	pa_response * osptr;
	int		status;
	struct	proactive * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->status == _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if ((status = use_proactive_configuration( pptr->profile )) != 0)  // Set up configuration according to the profile (where user and pass are)
		return( rest_html_response( aptr, status, "Not Found" ) );
	else if (!( osptr = pa_delete_server(rptr, aptr, pptr)))
	 	return( rest_html_response( aptr, 400, "Bad Request" ) );
	else
	{
        if (parse_delete_server_response(osptr) != 0){ // If something went wrong...
            return( rest_html_response( aptr, 4256, "Server Failure : Connect ProActive" ) );
        }
		if ( pptr->status != _OCCI_IDLE )
		{
			reset_structure_proactive_server( pptr );
			pptr->when = time((long *) 0);
			osptr = liberate_pa_response( osptr );
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	      r e s t a r t  _ p r o a c t i v e	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * restart_proactive(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )                
{
	int	status;
	struct	proactive * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ((status = use_proactive_configuration( pptr->profile )) != 0)
		return( rest_html_response( aptr, status, "Not Found" ) );
	else
	{
		if ( pptr->status == _OCCI_SUSPENDED )
		{
			pptr->when = time((long *) 0);
			pptr->status = _OCCI_RUNNING;
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	     s u s p e n d  _ p r o a c t i v e	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * suspend_proactive(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	int	status;
	struct	proactive * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ((status = use_proactive_configuration( pptr->profile )) != 0)
		return( rest_html_response( aptr, status, "Not Found" ) );
	{
		if ( pptr->status == _OCCI_RUNNING )
		{
			pptr->when = time((long *) 0);
			pptr->status = _OCCI_SUSPENDED;
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	   s o f t b o o t _ p r o a c t i v e  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * softboot_proactive(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	proactive * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	   h a r d b o o t _ p r o a c t i v e  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * hardboot_proactive(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	proactive * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	     r e b u i l d_ p r o a c t i v e  		*/
/*	-------------------------------------------	*/
private	struct	rest_response * rebuild_proactive(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	proactive * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	      r e s i z e _ p r o a c t i v e  		*/
/*	-------------------------------------------	*/
private	struct	rest_response * resize_proactive(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	proactive * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	    c o n f i r m _ p r o a c t i v e  		*/
/*	-------------------------------------------	*/
private	struct	rest_response * confirm_proactive(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	proactive * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	      r e v e r t _ p r o a c t i v e  		*/
/*	-------------------------------------------	*/
private	struct	rest_response * revert_proactive(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	proactive * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

#include "pacontract.c"

/*	-------------------------------------------	*/
/* 	      c r e a t e _ p r o a c t i v e  		*/
/*	-------------------------------------------	*/
private	int	create_proactive(struct occi_category * optr, void * vptr)
{
	struct	proactive * pptr;
	pptr = vptr;
    if (!( pptr->node )) 
		return( 0 ); 
	else	return(create_proactive_contract(optr,pptr, _CORDS_CONTRACT_AGENT, WpaProcci.tls));
}

private	struct	occi_interface	proactive_interface = {
	create_proactive,
	NULL,
	NULL,
	NULL
	};

/*	-------------------------------------------	*/
/* 	       b u i l d _ p r o a c t i v e  		*/
/*	-------------------------------------------	*/
/*	this function is to be called to build the	*/
/*	complete proactive occi category to offer	*/
/*	an proactive compute provisioning service	*/
/*	through a compatible one published occi		*/
/*	service interface.				*/
/*	-------------------------------------------	*/
public	struct	occi_category * build_proactive( char * domain )
{
	struct	occi_category * optr;

	if (!( optr = occi_proactive_builder( domain,"proactive" ) ))
		return( optr );
	else
	{
		optr->callback  = &proactive_interface;
		optr->access |= _OCCI_PROVIDER;

		if (!( optr = occi_add_action( optr,_CORDS_START,"",start_proactive)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_SAVE,"",save_proactive)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_STOP,"",stop_proactive)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_SUSPEND,"",suspend_proactive)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_RESTART,"",restart_proactive)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"softboot","",softboot_proactive)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"hardboot","",hardboot_proactive)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"rebuild","",rebuild_proactive)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"resize","",resize_proactive)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"confirm","",confirm_proactive)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"revert","",revert_proactive)))
			return( optr );
		else	return( optr );
	}
}

/*	-------------------------------------------	*/
/*	 s e t _ d e f a u l t _ p r o a c t i v e	*/
/*	-------------------------------------------	*/
private	int	set_default_proactive(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	pa_config * pptr;
	struct	pa_config * wptr;
    if (!( nptr = vptr ))
        return(0);
    else if (!( pptr = nptr->contents ))
        return(0);
	else
	{
		while ( nptr->previous )
			nptr = nptr->previous;
		while ( nptr )
		{
			if ((wptr = nptr->contents) != (struct pa_config*) 0)
				wptr->current = 0,
			nptr = nptr->next;
		}
		pptr->current = 1;
		return(0);
	}
}

/*	---------------------------------------------------------	*/
/* 	b u i l d _ p r o a c t i v e _ c o n f i g u r a t i o n	*/
/*	---------------------------------------------------------	*/
/*	this category handles the configuration of the interface	*/
/*	to the proactive  server for the preceeding category of		*/
/*	provisioning instance requests.					*/
/*	---------------------------------------------------------	*/
public	struct	occi_category * build_proactive_configuration( char * domain )
{
	struct	occi_category * optr;
	if (!( optr = occi_pa_config_builder( domain, "proactive_configuration" ) ))
		return( optr );
	else if (!( optr = occi_add_action( optr,"current","",set_default_proactive)))
		return( optr );
	else	return( optr );
}

#endif	/* _proccipa_c */
	/* ----------- */



