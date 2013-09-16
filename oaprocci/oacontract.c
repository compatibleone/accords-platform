/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2013  Iain JAmes Marshall (Prologue) <ijm667@hotmail.com>	*/
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License");      */
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at                              */
/*                                                                      */
/*  http://www.apache.org/licenses/LICENSE-2.0                          */
/*                                                                      */
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or      */
/* implied.                                                             */
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License.                                       */
/* -------------------------------------------------------------------- */
#ifndef	_oacontract_c	
#define	_oacontract_c

#include <assert.h>

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occiresolver.h"
#include "oaconfig.h"
#include "onapp_extras.h"
#include "oaclient.h"
#include "cb.h"
#include "oaconfig_occi_filter.h"
#include "onapp_helpers.h"
#include "oaconfig_backend_interface.h"
#include "oaconfig_occi_filter.h"

#define _USE_OCCI_ONAPPEXTRAS

struct	cords_vector
{
	char *	id;
	struct occi_response * message;
};

struct	cords_oa_contract
{
	const struct oa_config *config;
	struct	cords_vector	node;
	struct	cords_vector	infrastructure;
	struct	cords_vector	compute;
	struct	cords_vector	network;
	struct	cords_vector	firewall;
	struct	cords_vector	storage;
	struct	cords_vector	image;
	struct	cords_vector	system;
	struct	cords_vector	package;
	struct	oa_response *	flavors;
	struct	oa_response *	images;
};

struct onapp_action_context
{
	OnAppBool onapp_dirty;
};

static struct onapp_action_context const onapp_action_context_default = { OABOOL_FALSE // onapp_dirty
};

private	int	terminate_onapp_contract( int status, struct cords_oa_contract * cptr );
private	struct oa_response * stop_onapp_provisioning( struct onapp * pptr, struct onapp_action_context *context );
private	struct oa_response * start_onapp_provisioning( struct onapp * pptr, struct onapp_action_context *context );
private	struct oa_response * shutdown_onapp_provisioning( struct onapp * pptr, struct onapp_action_context *context );
private	struct oa_response * restart_onapp_provisioning( struct onapp * pptr, struct onapp_action_context *context );
private	struct oa_response * destroy_onapp_provisioning( struct onapp * pptr, struct onapp_action_context *context );

/*  --------------------------------------------------- */
/*  OCCI attribute handling                             */
/*  --------------------------------------------------- */
int set_string_from_occi_response_attribute_default(char * * const target, struct occi_response * const pocci_response
    , char * const domain, char * const category, char * const name
    , char * const default_value)
{
  int found_string = 0;

  char * const pattribute_iter = occi_extract_atribut(pocci_response, domain, category, name);
  if (pattribute_iter == NULL)
  {
    *target = allocate_string(default_value);
  }
  else
  {
    *target = allocate_string(pattribute_iter);
    found_string = 1;
  }

  return found_string;
}

int set_string_from_occi_response_attribute(char * * const target, struct occi_response * const pocci_response
    , char * const domain, char * const category, char * const name)
{
  return set_string_from_occi_response_attribute_default(target, pocci_response, domain, category, name, NULL);
}

int set_int_from_occi_response_attribute_default(int * const target, struct occi_response * const pocci_response
    , char * const domain, char * const category, char * const name
    , int default_value)
{
  int found_int = 0;

  char * const pattribute_iter = occi_extract_atribut(pocci_response, domain, category, name);
  if (pattribute_iter == NULL)
  {
    *target = default_value;
  }
  else
  {
    *target = atoi(pattribute_iter);
    found_int = 1;
  }

  return found_int;
}

int set_int_from_occi_response_attribute(int * const target, struct occi_response * const pocci_response
    , char * const domain, char * const category, char * const name)
{
  return set_int_from_occi_response_attribute_default(target, pocci_response, domain, category, name, 0);
}


/*  --------------------------------------------------- */
/*  Alert handling                                      */
/*  --------------------------------------------------- */

struct rest_response *  oaprocci_alert_relay_name(
		void * i,
		struct rest_client * prest_client,
		struct rest_response * prest_response,
		char * name,
		int status, char * message,
		char * nature,
		char * agent, char * tls)
{
	/* detect and convert REST Alerts */
	/* ------------------------------ */
	char * new_agent_is_really_source = agent;
	char * new_message_is_really_subject = message;
	char subject[4096];
	char source[4096];
	int body_size = 0;

	if ( ( agent != NULL && ( strncmp( agent, "REST", 4 ) == 0 ) && agent[4] == '\0' )
	 && prest_response != NULL && prest_response->request != NULL
	)
	{
		// Note - if need to include action it's captured in the prest_response->request->parameters.
		// No obvious field to put this in (source ?).
		body_size = read_body_bytes(subject, 4096, prest_response->body, prest_response->type);

		new_message_is_really_subject = subject;

		sprintf(source,"%s %s %s",prest_response->request->method, prest_response->request->object, message);
		new_agent_is_really_source = source;
	}

	return( occi_alert( i, prest_client, prest_response, name, status, new_message_is_really_subject, nature, new_agent_is_really_source, tls ) );
}

/*	------------------------------------------------------------------	*/
/*	    o a p r o c c i _ a l e r t _ r e l a y                         */
/*	------------------------------------------------------------------	*/
struct rest_response *  oaprocci_alert_relay(
		void * i,
		struct rest_client * prest_client,
		struct rest_response * prest_response,
		int status, char * message,
		char * nature,
		char * agent, char * tls)
{
	char *name = NULL;

	if ( ( agent != NULL && ( strncmp( agent, "REST", 4 ) == 0 ) && agent[4] == '\0' )
	 && prest_response != NULL && prest_response->request != NULL
	)
	{
		if (prest_response->request->object != NULL && prest_response->request->object[0] != '\0')
		{
			name = prest_response->request->object;
		}

	}

	return oaprocci_alert_relay_name(i, prest_client, prest_response, name, status, message, nature, agent, tls);
}

/*	--------------------------------------------------------	*/
/*   OnApp action boilerplate handling                          */
/*	--------------------------------------------------------	*/

struct error_data
{
	int status;
	char * error_message;
	int do_liberate;
};

static struct error_data const error_ok = { 200, "OK", 0 };

struct action_errors_data
{
	struct error_data invalid_action;
	struct error_data null_response;
	struct error_data null_nested_response;
	struct error_data null_nested_response_build_failure;
	struct error_data unhappy_response_build_failure;
	struct error_data happy_response;
};

struct onapp_action_input_data
{
	struct occi_category * pocci_category;
	struct rest_client * prest_client;
	struct rest_request * prest_request;
	struct rest_response * prest_response;
	struct onapp *ponapp;
};

typedef struct oa_response * (*action_fn)( struct onapp *, struct onapp_action_context * );

struct onapp_action_data
{
	struct onapp_action_input_data input;
	struct rest_response *prest_response_result;
	action_fn action;
	struct action_errors_data action_errors;
	struct onapp_action_context context;
};

void allocate_action_error_data_message(struct error_data *perror_data, char const action[], char const *format)
{
	char buffer[512];
	buffer[0] = '\0';
	snprintf(buffer, 512, format, action);
	buffer[511] = '\0';
	perror_data->error_message = allocate_string(buffer);
	perror_data->do_liberate = 1;
}

struct action_errors_data create_action_errors_data_default(char const action[])
{
	struct action_errors_data result = { 0 };
	result.invalid_action.status = 500;
	result.invalid_action.error_message = "Invalid Action";

	result.null_response.status = 4010;
	allocate_action_error_data_message(&result.null_response, action, "Unexpected error during OnApp VM action '%s'");

	result.null_nested_response_build_failure.status = 911;
	allocate_action_error_data_message(&result.null_nested_response_build_failure, action, "OnApp Failure : No Response during action '%s'");

	result.null_nested_response.status = 4010;
	allocate_action_error_data_message(&result.null_nested_response, action, "Bad Request : No response during to action '%s'");

	result.unhappy_response_build_failure.status = 911;
	allocate_action_error_data_message(&result.unhappy_response_build_failure, action, "OnApp Failure : Bad Request ('%s' action HTTP status >= 400)");

	result.happy_response = error_ok;

	return result;
}
void liberate_error_data_message(struct error_data *perror_data)
{
	if (perror_data != NULL)
	{
		if (perror_data->do_liberate != 0)
		{
			perror_data->do_liberate = 0;
			perror_data->error_message = liberate(perror_data->error_message);
		}
	}
}

void liberate_onapp_action_error_data(struct action_errors_data *paction_errors)
{
	liberate_error_data_message(&paction_errors->invalid_action);
	liberate_error_data_message(&paction_errors->null_response);
	liberate_error_data_message(&paction_errors->null_nested_response);
	liberate_error_data_message(&paction_errors->null_nested_response_build_failure);
	liberate_error_data_message(&paction_errors->unhappy_response_build_failure);
	liberate_error_data_message(&paction_errors->happy_response);
}

int	onapp_build_failure( struct onapp * pptr, int status, char * message, struct onapp_action_context *context )
{
	// TODO: Clear up ongoing OnApp activity.
	rest_log_message( message );
	pptr->build_state = allocate_string("failed");

	return( status );
}

struct onapp_action_input_data const initialise_onapp_action_input_data (
		struct occi_category * pocci_category,
		struct rest_client * prest_client,
		struct rest_request * prest_request,
		struct rest_response * prest_response,
		void * vptr
)
{
	struct onapp_action_input_data const input = { pocci_category, prest_client, prest_request, prest_response, vptr };
	return input;
}

struct onapp_action_data create_onapp_action_data(char const action[], struct onapp_action_input_data onapp_action_input
		, action_fn paction
		, struct action_errors_data * paction_errors)
{
	struct action_errors_data action_errors;

	if (paction_errors == NULL)
	{
		action_errors = create_action_errors_data_default(action);
		paction_errors = &action_errors;
	}
	struct onapp_action_data const onapp_action = { onapp_action_input, NULL, paction, *paction_errors };

	return onapp_action;
}

struct onapp_action_data create_onapp_action_data_default_errors(char const action[], struct onapp_action_input_data onapp_action_input
		, action_fn paction)
{
	return create_onapp_action_data(action, onapp_action_input, paction, NULL);
}

void handle_onapp_action_context(struct onapp *ponapp, struct onapp_action_context *context)
{
	struct onapp *ponapp_update = ponapp;

	if (context != NULL && ponapp != NULL && ponapp->id != NULL && ponapp->id[0] != '\0')
	{
		if (context->onapp_dirty != OABOOL_FALSE) // If onapp has changed
		{
			// Update clones onapp * parameter.
			onapp_backend->update(ponapp_update->id, ponapp_update);

		} // Ends if onapp has changed
	}
}

typedef enum GenerateAlert_
{
	  GA_NONE = 0
	, GA_OWN = 1
	, GA_RELAY = 2
} GenerateAlert;

struct rest_response * handle_alert(int generate_alert,
	struct occi_category *pocci_category,
	struct rest_client * prest_client,
	struct oa_response * poa_response,
	struct rest_request *prest_request,
	struct rest_response *prest_response_result,
	struct onapp *ponapp)
{
	struct rest_request *rest_request_local;
	struct rest_response *rest_response_local;
	struct rest_header *rest_header;
	char *name = NULL;
	char buffer[1024];

	if ((generate_alert & GA_OWN) == GA_OWN)
	{
		// Skip the alert_relay, we'll do it ourselves.
		rest_request_local = poa_response->response->request;
		poa_response->response->request = prest_request;

		if (prest_client != NULL && prest_client->server->method.alert != NULL)
		{
			(void) (*prest_client->server->method.alert )
				( prest_client->server->method.instance, prest_client, poa_response->response,
				name,
				poa_response->response->status, poa_response->response->message, "ALERT","REST", default_tls() );
		}
		else
		{
			snprintf(buffer, 1024, "%s%s", pocci_category->location, ponapp->id);
			// Skip the alert_relay, we'll do it ourselves, and rely on the fact that the rest_client isn't used if it's NULL.
			oaprocci_alert_relay_name(poa_response, prest_client, poa_response->response, buffer, poa_response->response->status, poa_response->response->message,
				"ALERT", "REST", default_tls());
		}
		poa_response->response->request = rest_request_local;
		rest_request_local = NULL;
	}

	if ((generate_alert & GA_RELAY) == GA_RELAY)
	{
		// ALTERNATIVELY pass the response back up the call stack, but then there's no guarantee the data won't have been altered
		// before it reaches the alert_relay.
		// This response pointer will ultimately end up being used to populate an alert, as well as being returned as the response.
		prest_response_result = poa_response->response;

		// Create a response based on the oa_response, duplicating only the useful fields.
		if (prest_client != NULL)
		{
			rest_response_local = rest_allocate_response(prest_client);
		}
		else
		{
			rest_response_local = allocate_rest_response();
		}

		if (rest_response_local != NULL)
		{
			// Pass over body ownership.
			rest_response_body(rest_response_local, poa_response->response->body, poa_response->response->type);
			rest_response_body(poa_response->response, NULL, poa_response->response->type);

			// If set status
			if ((rest_response_local = rest_response_status(rest_response_local, poa_response->response->status, poa_response->response->message)))
			{
				// Set Content-Type
				if ((rest_header = rest_resolve_header(poa_response->response->first, _HTTP_CONTENT_TYPE)))
				{
					rest_header = rest_response_header(rest_response_local, rest_header->name, rest_header->value);

				}

				// Set Content-Length
				if (get_http_body_length_string(buffer, 1024, rest_response_local->type, rest_response_local->body))
				{
					rest_response_header(rest_response_local, _HTTP_CONTENT_LENGTH, buffer);

				}

				prest_response_result = rest_response_local;

			} // Ends if set status
			else // Else failed to set status
			{
				liberate_rest_response(rest_response_local);

			} // Ends else failed to set status

			rest_response_local = NULL;
		}
	}

	return prest_response_result;
}

public struct rest_response * do_onapp_action_alert(
	struct onapp_action_data * const ponapp_action,
	int forbidden_occi_state,
	char const * const price_action,
	int generate_alert
	)
{
	char price_action_buffer[256];
	char reference[512];
	struct action_errors_data const * const action_errors = &ponapp_action->action_errors;
	struct oa_response * poa_response;
	struct rest_response *prest_response_result = NULL;
	struct rest_request *old_rest_request;
	int status;

	if (1)
	{
		dump_rest_request(stdout, ponapp_action->input.prest_request);
	}

	if (ponapp_action->input.ponapp == NULL)
	{
		prest_response_result = rest_html_response( ponapp_action->input.prest_response,
			action_errors->invalid_action.status, action_errors->invalid_action.error_message );
	}
	else if (ponapp_action->prest_response_result == NULL)
	{
		if (1)
		{
			dump_onapp_data(stdout, ponapp_action->input.ponapp);
		}

		if ( ponapp_action->input.ponapp->state != forbidden_occi_state )
		{
			if ((poa_response = ponapp_action->action( ponapp_action->input.ponapp, &ponapp_action->context )) == (struct oa_response *) 0)
			{
				prest_response_result = rest_html_response (ponapp_action->input.prest_response,
					action_errors->null_response.status, action_errors->null_response.error_message);
			}
			else
			{
				if (!( poa_response->response ))
				{
					onapp_build_failure( ponapp_action->input.ponapp,
							action_errors->null_nested_response_build_failure.status, action_errors->null_nested_response_build_failure.error_message,
							&ponapp_action->context);
					prest_response_result = rest_html_response( ponapp_action->input.prest_response,
						action_errors->null_nested_response.status, action_errors->null_nested_response.error_message );
				}
				else if ( poa_response->response->status >= 400 )
				{
					prest_response_result = handle_alert(generate_alert, ponapp_action->input.pocci_category, ponapp_action->input.prest_client, poa_response, ponapp_action->input.prest_request, prest_response_result, ponapp_action->input.ponapp);

					onapp_build_failure( ponapp_action->input.ponapp,
						action_errors->unhappy_response_build_failure.status, action_errors->unhappy_response_build_failure.error_message,
						&ponapp_action->context);
				}
				else
				{
					if (price_action != NULL)
					{
						// CompatibleOne price handling.
						snprintf(reference, 512, "%s/%s/%s",OnAppConfiguration.identity,_CORDS_ONAPP,ponapp_action->input.ponapp->id);
						if ( rest_valid_string( ponapp_action->input.ponapp->price ) )
						{
							snprintf(price_action_buffer, 256, "action=%s", price_action);
							occi_send_transaction( _CORDS_ONAPP, ponapp_action->input.ponapp->price, price_action_buffer, ponapp_action->input.ponapp->id, reference );
						}
					}
				}

				poa_response = liberate_oa_response( poa_response );

			}
		}
	}

	if (prest_response_result == NULL)
	{
		prest_response_result = rest_html_response( ponapp_action->input.prest_response, action_errors->happy_response.status, action_errors->happy_response.error_message );
	}

	ponapp_action->prest_response_result = prest_response_result;

	handle_onapp_action_context(ponapp_action->input.ponapp, &ponapp_action->context);

	return prest_response_result;
}

/*	-------------------------------------------	*/
/* 	   	d o _ o n a p p _ a c t i o n           */
/*	-------------------------------------------	*/
public	struct	rest_response * do_onapp_action(
		struct onapp_action_data * const ponapp_action,
		int forbidden_occi_state,
		char const * const price_action
	)
{
	return do_onapp_action_alert(ponapp_action, forbidden_occi_state, price_action, GA_OWN | GA_RELAY);
}

/*	--------------------------------------------------------	*/

/*	--------------------------------------------------------	*/
/*	     r e t r i e v e _ o n a p p _ d a t a                  */
/*	--------------------------------------------------------	*/
int	retrieve_onapp_data(const struct oa_config * pptr, struct cords_oa_contract * cptr
		, struct onapp *ponapp)
{
	char *			vptr;

	if ( ponapp == NULL )
		return 1;

	// TODO: Populate these fields.
	ponapp->location = 0; // Not relevant (you could populate some combo of the onapp_configuration locations)
	ponapp->provider = 0; // Should point somehow to the onapp_configuration instance
	ponapp->description = 0; // Manifest has this I think
	ponapp->ipaddress = 0; // VM IP address
	ponapp->region = 0; // From onapp_configuration (not important)
	ponapp->local_id = 0; // VM id from CP once created
	ponapp->status = 0; // Can't recall right now
	ponapp->original = 0; // Can't recall right now

	/* -------------------------------------------------------------- */
	/* retrieve appropriate parameters from infrastructure components */
	/* -------------------------------------------------------------- */
	if (!( vptr = occi_extract_atribut( cptr->compute.message, "occi",
		_CORDS_COMPUTE, _CORDS_MEMORY ) ))
		ponapp->memory = 0;
	else	onapp_normalise_value_liberate( vptr, 'M', &ponapp->memory );

	if (!( vptr = occi_extract_atribut( cptr->compute.message, "occi",
		_CORDS_COMPUTE, _CORDS_CORES ) ))
		ponapp->cpus= 0;
	else	ponapp->cpus = onapp_atoi( vptr );

	if (!( vptr = occi_extract_atribut( cptr->storage.message, "occi",
		_CORDS_STORAGE, _CORDS_SIZE ) ))
		ponapp->disk_size = 0;
	else	onapp_normalise_value_liberate(vptr,'G', &ponapp->disk_size );

	// FIREWALL from FIREWALL Node Name ?
	if (!( vptr = occi_extract_atribut( cptr->firewall.message, "occi",
		_CORDS_FIREWALL, _CORDS_NAME) ))
		ponapp->firewall = 0;
	else	ponapp->firewall = vptr;

	if (!( vptr = occi_extract_atribut( cptr->image.message, "occi",
		_CORDS_IMAGE, _CORDS_NAME ) ))
		ponapp->image = 0;
	else	ponapp->image = vptr;

	// OnApp specific
//	ponapp->disk_type_of_format = 0; // OnApp specific
//	ponapp->swap_size = allocate_string("1"); // OnApp specific
//	ponapp->required_ip_address_assignment = 1; // OnApp specific
//	ponapp->required_virtual_machine_build = 1; // OnApp specific
//	ponapp->required_automatic_backup = 0; // OnApp specific (mE)
	ponapp->zone = 0; // A CO thing - probably not relevant in short term

	static char default_hostname[] = "testhost";
	if (!( vptr = occi_extract_atribut( cptr->compute.message, "occi",
		_CORDS_COMPUTE, _CORDS_HOSTNAME ) )) {
		ponapp->hostname = allocate_string( default_hostname );
		ponapp->host_name = allocate_string( default_hostname );
	}
	else {
		ponapp->hostname = vptr;
		ponapp->host_name = allocate_string( vptr );
	}

	ponapp->label = allocate_string(pptr->label);
	ponapp->access = 0; // TODO
	ponapp->state = 0; // TODO VM State

	ponapp->username = allocate_string(pptr->user); // CP username (I think it will be e-mail address)
	ponapp->password = allocate_string(pptr->password); // CP password (actually key)

	static char default_description[] = "Created by oaprocci";
	vptr = default_description;

	if (pptr->description != NULL && *pptr->description != '\0')
	{
		vptr = pptr->description;
	}

	ponapp->description = allocate_string(vptr); // CP description

	// Represent the default build state. In the future this should be building, and the market should poll
	// the VM status and move to active once it is built. When 'destroyed' it should move to 'deleted' but
	// never delete the actual vm / onapp record.
	static char default_build_state[] = "active";
	ponapp->build_state = allocate_string(default_build_state);

#ifdef XXX
	// TODO: Populate ALL the other fields.
	if (!( vptr = occi_extract_atribut( cptr->storage.message, "occi",
		_CORDS_YYY, _CORDS_XXX ) ))
		ponapp->xxx = 0;
	else	ponapp->xxx = vptr;
#endif // XXX

	return 0;
}

int retrieve_onapp_extras_data(struct cords_onapp_extras const * const onapp_extras,
		struct onapp* ponapp)
{
	ponapp->initial_root_password = 0; // OnApp specific
	ponapp->initial_root_password_encryption_key = 0; // OnApp specific
	ponapp->required_ip_address_assignment = 0; // OnApp specific
	ponapp->required_automatic_backup = 0; // OnApp specific
	ponapp->required_virtual_machine_build = 1;
	ponapp->required_virtual_machine_startup = 0; // OnApp specific

	if (onapp_extras == NULL)
	{
		ponapp->disk_type_of_format = NULL; // ?
		//ponapp->label = allocate_string("");
		ponapp->swap_size = allocate_string("1");
		ponapp->cpu_shares = allocate_string("1");
		return 0;
	}

	//onapp_extras->id; // Irrelevant.
	//onapp_extras->name; // Irrelevant.
	ponapp->initial_root_password = allocate_string(onapp_extras->initial_root_password);
	ponapp->initial_root_password_encryption_key = allocate_string(onapp_extras->initial_root_password_encryption_key);
	ponapp->required_ip_address_assignment = onapp_extras->required_ip_address_assignment;
	ponapp->disk_type_of_format = allocate_string(onapp_extras->disk_type_of_format);
	ponapp->label = (char *)liberate(ponapp->label);
	ponapp->label = allocate_string(onapp_extras->label);
	ponapp->required_automatic_backup = onapp_extras->required_automatic_backup;
	onapp_normalise_value(onapp_extras->swap_size, 'G', &ponapp->swap_size );
	ponapp->required_virtual_machine_build = onapp_extras->required_virtual_machine_build;
	ponapp->required_virtual_machine_startup = onapp_extras->required_virtual_machine_startup;
	ponapp->cpu_shares = allocate_string(onapp_extras->cpu_shares);

	return 0;
}


/*	-------------------------------------------	*/
/*	    c r e a t e _ o n a p p _ v m          	*/
/*	-------------------------------------------	*/
public	struct	rest_response * create_onapp_vm(
	struct occi_category * optr,
	struct rest_client * cptr,
	struct rest_request * rptr,
	struct rest_response * aptr,
	void * vptr,
	struct onapp_action_context *context,
	int generate_alert)
{
	struct	onapp * pptr;
	const struct  oa_config * config;
	struct  oa_response * oaptr;
	struct rest_request *old_rest_request;
	struct	data_element * json_vm;
	int status;
	char *  sptr;
	char * filename;
	char reference[1024];

	if (1)
	{
		dump_rest_request(stdout, rptr);
		//test_dump_occi_category(optr, stdout);
	}

	status = 404;
	strcpy(reference,"reference");

	/* --------------------------------- */
	/* retrieve the instance information */
	/* --------------------------------- */
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );

	/* ------------------------------------- */
	/* retrieve the subscription information */
	/* ------------------------------------- */
	else if ( pptr->profile == NULL )
		return ( rest_html_response ( aptr, 500, "No Onapp Profile supplied" ) );
	else if (!( config = use_onapp_configuration_crud( pptr->profile ) ))
		return( rest_html_response( aptr, status, "Configuration Not Found" ) );
		
	if ( rest_valid_string( pptr->price ) )
		occi_send_transaction( _CORDS_ONAPP, pptr->price, "action=start", pptr->account, reference );

	if (!( filename = oa_create_server_request(config, pptr) )) {
		onapp_build_failure (pptr, 911, "Failure Creating Server Message", context );
	 	return( rest_html_response( aptr, 4004, "Server Failure : Create Server Message" ) );
	}
	else if (!( oaptr = oa_create_server( config, filename )))
	{
		onapp_build_failure( pptr, 911, "Failure Creating Server", context );
	 	return( rest_html_response( aptr, 4008, "Server Failure : Create Server Request" ) );
	}
	else if (!( oaptr->response ))
	{
		oaptr = liberate_oa_response( oaptr );
		onapp_build_failure( pptr, 911, "OA Failure : No Response", context );
	 	return( rest_html_response( aptr, 4010, "Bad Request : Create Server No Response" ) );
	}
	else if ( oaptr->response->status >= 400 )
	{
		aptr = handle_alert(generate_alert, optr, cptr, oaptr, rptr, aptr, pptr);

		oaptr = liberate_oa_response( oaptr );
		onapp_build_failure( pptr, 911, "OA Failure : Bad Request", context );
		// This response pointer will ultimately end up being used to populate an alert, as well as being returned as the response.
		return( aptr );
	}

	/* --------------------------------- */
	/* retrieve crucial data from server */
	/* --------------------------------- */

	if (oaptr->jsonroot != NULL) {
		if ( (json_vm = json_element(oaptr->jsonroot, "virtual_machine")) ) {
			pptr->local_id = allocate_string(json_atribut(json_vm, "identifier"));
		}
	}

	oaptr = liberate_oa_response( oaptr );

	if (pptr->required_virtual_machine_startup != 0)
	{
		pptr->state = _OCCI_RUNNING;
	}
	else
	{
		pptr->state = _OCCI_IDLE;
	}

	return( rest_html_response( aptr, 200, "OK" ) );	
}

/*	-------------------------------------------	*/
/* 	   	s a v e _ o n a p p         */
/*	-------------------------------------------	*/
public	struct	rest_response * save_onapp(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_contract * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		/* ----------------------------------- */
		/* add code here to allocate resources */	
		/* ----------------------------------- */
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	   	s t o p _ o n a p p         */
/*	-------------------------------------------	*/
public	struct	rest_response * stop_onapp(
		struct occi_category * pocci_category,
		struct rest_client * prest_client,
		struct rest_request * prest_request,
		struct rest_response * prest_response,
		void * vptr )
{
	char reference[512];
	struct oa_config * config=(struct oa_config *) 0;
	struct oa_response * poa_response;
	struct rest_response *prest_response_result = NULL;
	int status;
	struct onapp * ponapp;
	struct onapp_action_context context = onapp_action_context_default;
	if (!( ponapp = vptr ))
	{
		prest_response_result = rest_html_response( prest_response, 404, "Invalid Action" );
	}
  else if ((poa_response = stop_onapp_provisioning( ponapp, &context )) == (struct oa_response *) 0)
  {
    prest_response_result = rest_html_response (prest_response, 500, "Unexpected error stopping OnApp VM");
  }
  else
  {
    if (!( poa_response->response ))
    {
      onapp_build_failure( ponapp, 911, "OA Failure : No Response", &context  );
      prest_response_result = rest_html_response( prest_response, 4010, "Bad Request : Stop Server No Response" );
    }
    else if ( poa_response->response->status >= 400 )
    {
      // This response pointer will ultimately end up being used to populate an alert, as well as being returned as the response.
      prest_response_result = poa_response->response;
      poa_response->response = NULL;

      onapp_build_failure( ponapp, 911, "OA Failure : Bad Request (stop VM HTTP status >= 400)", &context  );
    }
    else
    {
      // CompatibleOne price handling.
      sprintf(reference,"%s/%s/%s",OnAppConfiguration.identity,_CORDS_ONAPP,ponapp->id);
      if ( rest_valid_string( ponapp->price ) )
      {
        occi_send_transaction( _CORDS_ONAPP, ponapp->price, "action=stop", ponapp->id, reference );
      }
    }

    poa_response = liberate_oa_response( poa_response );

  }
	if (prest_response_result == NULL)
	{
		prest_response_result = rest_html_response( prest_response, 200, "OK" );
	}

	handle_onapp_action_context(ponapp, &context);

	return prest_response_result;
}

public struct rest_response * start_onapp(struct occi_category * pocci_category,
		struct rest_client * prest_client,
		struct rest_request * prest_request,
		struct rest_response * prest_response,
		void * vptr
)
{
	struct onapp_action_input_data input = initialise_onapp_action_input_data(pocci_category, prest_client, prest_request, prest_response, vptr);
	struct onapp_action_data onapp_action = create_onapp_action_data_default_errors("start", input, &start_onapp_provisioning);
	do_onapp_action(&onapp_action, _OCCI_INVALID, NULL);
	liberate_onapp_action_error_data(&onapp_action.action_errors);
	return onapp_action.prest_response_result;
}

public struct rest_response * shutdown_onapp(struct occi_category * pocci_category,
		struct rest_client * prest_client,
		struct rest_request * prest_request,
		struct rest_response * prest_response,
		void * vptr
)
{
	struct onapp_action_input_data input = initialise_onapp_action_input_data(pocci_category, prest_client, prest_request, prest_response, vptr);
	struct onapp_action_data onapp_action = create_onapp_action_data_default_errors("shutdown", input, &shutdown_onapp_provisioning);
	do_onapp_action(&onapp_action, _OCCI_INVALID, "stop");
	liberate_onapp_action_error_data(&onapp_action.action_errors);
	return onapp_action.prest_response_result;
}

public struct rest_response * restart_onapp(struct occi_category * pocci_category,
		struct rest_client * prest_client,
		struct rest_request * prest_request,
		struct rest_response * prest_response,
		void * vptr
)
{
	struct onapp_action_input_data input = initialise_onapp_action_input_data(pocci_category, prest_client, prest_request, prest_response, vptr);
	struct onapp_action_data onapp_action = create_onapp_action_data_default_errors("restart", input, &restart_onapp_provisioning);
	do_onapp_action(&onapp_action, _OCCI_INVALID, NULL);
	liberate_onapp_action_error_data(&onapp_action.action_errors);
	return onapp_action.prest_response_result;
}

public struct rest_response * destroy_onapp(struct occi_category * pocci_category,
		struct rest_client * prest_client,
		struct rest_request * prest_request,
		struct rest_response * prest_response,
		void * vptr
)
{
	struct onapp_action_input_data input = initialise_onapp_action_input_data(pocci_category, prest_client, prest_request, prest_response, vptr);
	struct onapp_action_data onapp_action = create_onapp_action_data_default_errors("destroy", input, &destroy_onapp_provisioning);
	do_onapp_action(&onapp_action, _OCCI_INVALID, NULL);
	liberate_onapp_action_error_data(&onapp_action.action_errors);
	return onapp_action.prest_response_result;
}

/*  -----------------------------------------------------------------  */
/*      c r e a t e _ o n a p p _ c o n t r a c t          */
/*  -----------------------------------------------------------------  */
public	int	create_onapp_contract(
		struct occi_category * optr,
		struct onapp * pptr,
		struct rest_request  * rptr,
		char * agent,
		char * tls )
{
	
	const struct	oa_config *                config = (struct oa_config *) 0;
	//struct	cords_onapp_extras const * onapp_extras = (struct cords_onapp_extras const *)0;
	struct cords_onapp_extras_handle   onapp_extras_handle = { 0 };
	struct	cords_oa_contract          contract;
	struct	oa_response *              instancetypes = (struct oa_response *) 0;
	struct	oa_response *              images = (struct oa_response *) 0;
	int	status;
	int result = 0; // Stores result for a function call.
	char * vptr;
	char buffer[256];

	status = 404;

	if (!(config = use_onapp_configuration_crud( pptr->profile )))
	{
		rest_log_message("Could Not Find Configuration");
		return( status );
	}
	else
	{
		memset( &contract, 0, sizeof( struct cords_oa_contract ));
		contract.config = config;
	}

	// Find the onapp extras for this nodes.
	if (pptr->node != NULL) // If got a node
	{
		vptr = strrchr( pptr->node, '/');

		if (vptr == NULL)
		{
			rest_log_message("onapp node id not found");
		}
		else
		{
			onapp_extras_handle = use_cords_onapp_extras_handle( vptr+1, agent, tls );
			if (onapp_extras_handle.ponapp_extras == NULL)
			{
				rest_log_message("Could not find onapp_extras");
			}
			else
			{
				if (1)
				{
					fputs("===onapp_extras===\n", stdout);
					dump_onapp_extras(stdout, onapp_extras_handle.ponapp_extras);
					fputc('\n', stdout);
				}
			}
		}
	}
	else
	{
		rest_log_message("No node found in onapp");
	}

	/* ---------------------------- */
	/* recover the node description */
	/* ---------------------------- */
	if (!( contract.node.id = pptr->node ))
		return( 0 );
	else if (!( contract.node.message = occi_simple_get( contract.node.id, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( terminate_onapp_contract( 1170, &contract ) );
	else
	{
		/* ---------------------------------------------------- */
		/* recover and store the public/private access property */
		/* ---------------------------------------------------- */
		if (!( vptr = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_ACCESS ) ))
			vptr = _CORDS_PUBLIC;
		if ( pptr->access ) pptr->access = liberate( pptr->access );
		if (!( pptr->access = allocate_string( vptr ) ))
			return( terminate_onapp_contract( 1127, &contract ) );
	}

	/* -------------------------------- */
	/* recover the firewall description */
	/* -------------------------------- */
	if (( contract.firewall.id = occi_extract_atribut( contract.node.message, "occi",
		_CORDS_NODE, _CORDS_FIREWALL )) != (char * ) 0)
	{
		if (!( contract.firewall.message = occi_simple_get( contract.firewall.id, agent, tls ) ))
			return( terminate_onapp_contract( 1171, &contract ) );
		else if (!( pptr->firewall = allocate_string( contract.firewall.id ) ))
			return( terminate_onapp_contract( 1172, &contract ) );
	}

	/* -------------------------------------- */
	/* recover the infrastructure description */
	/* -------------------------------------- */
	if (!( contract.infrastructure.id = occi_extract_atribut( contract.node.message, "occi",
		_CORDS_NODE, _CORDS_INFRASTRUCTURE ) ))
		return( terminate_onapp_contract( 1171, &contract ) );
	else if (!( contract.infrastructure.message = occi_simple_get( contract.infrastructure.id, agent, tls ) ))
		return( terminate_onapp_contract( 1172, &contract ) );

	else if (!( contract.compute.id = occi_extract_atribut( contract.infrastructure.message, "occi",
		_CORDS_INFRASTRUCTURE, _CORDS_COMPUTE ) ))
		return( terminate_onapp_contract( 1173, &contract ) );
	else if (!( contract.compute.message = occi_simple_get( contract.compute.id, agent, tls ) ))
		return( terminate_onapp_contract( 1174, &contract ) );

	else if (!( contract.network.id = occi_extract_atribut( contract.infrastructure.message, "occi",
		_CORDS_INFRASTRUCTURE, _CORDS_NETWORK ) ))
		return( terminate_onapp_contract( 1175, &contract ) );
	else if (!( contract.network.message = occi_simple_get( contract.network.id, agent, tls ) ))
		return( terminate_onapp_contract( 1177, &contract ) );
//	else if (!( pptr->network = allocate_string( contract.network.id ) ))
//		return( terminate_onapp_contract( 1176, &contract ) );

	else if (!( contract.storage.id = occi_extract_atribut( contract.infrastructure.message, "occi",
		_CORDS_INFRASTRUCTURE, _CORDS_STORAGE ) ))
		return( terminate_onapp_contract( 1178, &contract ) );
	else if (!( contract.storage.message = occi_simple_get( contract.storage.id, agent, tls ) ))
		return( terminate_onapp_contract( 1179, &contract ) );

	/* ---------------------------------- */
	/* can now recover all the necessary  */
	/* information from the manifest occi */
	/* data.                              */
	/* ---------------------------------- */

	/* ---------------------------------- */
	/* recover the node image description */
	/* ---------------------------------- */
	if (!( contract.image.id = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_IMAGE ) ))
		return( terminate_onapp_contract( 1182, &contract ) );
	else if (!( contract.image.message = occi_simple_get( contract.image.id, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( terminate_onapp_contract( 1183, &contract ) );

	/* ------------------------ */
	/* retrieve the cosacs flag */
	/* ------------------------ */
	else if ((!( pptr->agent = occi_extract_atribut( contract.image.message, "occi", 
		_CORDS_IMAGE, "agent" ) ))
			//&&  (!( pptr->agent = allocate_string("cosacs") )))
			||  (!( pptr->agent = allocate_string(_CORDS_OA_AGENT) )))
		return( terminate_onapp_contract( 1283, &contract ) );

	else if (!( contract.system.id = occi_extract_atribut( contract.image.message, "occi", 
		_CORDS_IMAGE, _CORDS_SYSTEM ) ))
		return( terminate_onapp_contract( 1184, &contract ) );
	else if (!( contract.system.message = occi_simple_get( contract.system.id, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( terminate_onapp_contract( 1185, &contract ) );

	/* ----------------------------------------------- */
	/* resolve any price information for this category */
	/* ----------------------------------------------- */
	pptr->price = occi_resolve_category_price( _CORDS_ONAPP, default_operator(), _CORDS_CONTRACT_AGENT, default_tls() );

	/* ---------------------------------- */
	/* recover the onapp_extras data.     */
	/* ---------------------------------- */

	if (result != 0)
	{
		return( terminate_onapp_contract( 1186, &contract ) );
	}

	if ( retrieve_onapp_data( config, &contract, pptr) != 0 )
	{
		return( terminate_onapp_contract( 1187, &contract ) );
	}

	result = retrieve_onapp_extras_data(onapp_extras_handle.ponapp_extras, pptr);
	close_cords_onapp_extras_handle(&onapp_extras_handle);

	dump_oa_config_data_stdout(config);
	dump_onapp_data_stdout(pptr);

	return( terminate_onapp_contract( 0, &contract ) );
}

/*  -----------------------------------------------------------------  */
/*      d e l e t e _ o n a p p _ c o n t r a c t          */
/*  -----------------------------------------------------------------  */
public	int	delete_onapp_contract(
		struct occi_category * optr,
		struct onapp * pptr,
		struct rest_request  * rptr)
{
	struct	oa_response * oaptr;
	int result = 0;
	struct onapp_action_context context = onapp_action_context_default;

	if (1)
	{
		dump_rest_request(stdout, rptr);
		dump_onapp_data(stdout, pptr);
	}

	oaptr = stop_onapp_provisioning( pptr, &context );

	if (oaptr != NULL && oaptr->response != NULL && oaptr->response->status < 400)
	{
		oaptr = liberate_oa_response( oaptr );
		result = 1;
	}

	pptr->state = _OCCI_IDLE;
	pptr->build_state = allocate_string("deleted");
	handle_onapp_action_context(pptr, &context);

	return result;
}


/*	-----------------------------------------------------------------  */
/* 	    u s e _ o n a p p _ c o n f i g u r a t i o n      */
/*	-----------------------------------------------------------------  */
//private	struct oa_config * use_onapp_configuration( char * nptr )
//{
//	struct	oa_config * sptr;
//	if (( sptr = resolve_oa_configuration( nptr )) != (struct oa_config *) 0)
//		return( sptr );
//	else if (!( nptr = get_operator_profile() ))
//		return( sptr );
//	else 	return( resolve_oa_configuration( nptr ) );
//}

const struct oa_config * use_onapp_configuration( char * nptr, int is_active, int check_is_active )
{
	const struct	oa_config * sptr;
	struct onapp * ponapp;
	struct oa_contract *poacontract;
	char * operatorProfileName = NULL;
	if (( sptr = resolve_oa_configuration( nptr, is_active, check_is_active )) != (struct oa_config *) 0)
		return( sptr );
	else if (!( operatorProfileName = get_operator_profile() ))
		return( sptr );
	else if ( nptr == NULL )
		return( sptr );
	else if ( strcmp( nptr, operatorProfileName ) == 0 )
		return( sptr );
	else 	return( resolve_oa_configuration( operatorProfileName, is_active, check_is_active ) );
}

const struct oa_config * use_onapp_configuration_action( char * nptr )
{
	return use_onapp_configuration(nptr, 0, 0);
}

const struct oa_config * use_onapp_configuration_crud( char * nptr )
{
	return use_onapp_configuration(nptr, 1, 1);
}

/*  -----------------------------------------------------------------  */
/* 	    r e s o l v e _ o a _ c o n f i g u r a t i o n               */
/*  -----------------------------------------------------------------  */
const struct oa_config * resolve_oa_configuration( char * sptr, int is_active, int check_is_active )
{
    assert(oa_config_backend);
    assert(oa_config_backend->retrieve_from_filter);
    
	// Some proccis have data over and above that contained in their config
    // typically stored in a struct which contains a config pointer (e.g. xxx_subscription).
    // OnApp have everything they need in the loaded config already,
    // so rather than creating a new instance of some config containing struct,
    // we'll just return the config directly.
	
	struct oa_config_occi_filter filter;
	memset(&filter, 0, sizeof(struct oa_config_occi_filter));
	filter.attributes = allocate_oa_config();
	filter.attributes->name = allocate_string(sptr);
	filter.name = 1;
	filter.attributes->is_active = check_is_active;
	filter.is_active = is_active;
	filter.attributes->deleted = 0;
	filter.deleted = 1;
	
	oa_config_id_list matches = oa_config_backend->list(&filter); 
	
	liberate_oa_config(filter.attributes);
	const struct oa_config *retVal = NULL;
	if (matches.count > 0) {	    
	    retVal = oa_config_backend->retrieve_from_id(matches.ids[0]);
	}
	oa_config_free_id_list(&matches);	
	return (retVal);
}


/*	-----------------------------------------------------------------  */
/* 	    u s e _ o n a p p _ e x t r a s                               */
/*	-----------------------------------------------------------------  */
struct cords_onapp_extras_handle use_cords_onapp_extras_handle( char * sptr, char *agent, char *tls )
{
	struct	cords_onapp_extras_handle handle = resolve_cords_onapp_extras_handle( sptr, agent, tls );
	return handle;
}

void close_cords_onapp_extras_handle(struct cords_onapp_extras_handle *handle)
{
  struct cords_onapp_extras *ponapp_extras;
  if (handle->owner)
  {
    ponapp_extras = handle->ponapp_extras;
    handle->ponapp_extras = NULL;
    handle->owner = 0;
    liberate_cords_onapp_extras(ponapp_extras);
  }
  else
  {
    handle->ponapp_extras = NULL;

  }
}

void extract_onapp_extras_from_occi_response(struct cords_onapp_extras *ponapp_extras, struct occi_response *pocci_response)
{
  static char occi[] = "occi";
  static char onapp_extras[] = "onapp_extras";
  set_string_from_occi_response_attribute(&ponapp_extras->id, pocci_response, occi, onapp_extras, "id");
  set_string_from_occi_response_attribute(&ponapp_extras->initial_root_password, pocci_response, occi, onapp_extras, "initial_root_password");
  set_string_from_occi_response_attribute(&ponapp_extras->name, pocci_response, occi, onapp_extras, "name");
  set_string_from_occi_response_attribute(&ponapp_extras->initial_root_password_encryption_key, pocci_response, occi, onapp_extras, "initial_root_password_encryption_key");
  set_int_from_occi_response_attribute(&ponapp_extras->required_ip_address_assignment, pocci_response, occi, onapp_extras, "required_ip_address_assignment");
  set_string_from_occi_response_attribute(&ponapp_extras->disk_type_of_format, pocci_response, occi, onapp_extras, "disk_type_of_format");
  set_string_from_occi_response_attribute(&ponapp_extras->label, pocci_response, occi, onapp_extras, "label");
  set_int_from_occi_response_attribute(&ponapp_extras->required_automatic_backup, pocci_response, occi, onapp_extras, "required_automatic_backup");
  set_string_from_occi_response_attribute(&ponapp_extras->swap_size, pocci_response, occi, onapp_extras, "swap_size");
  set_int_from_occi_response_attribute(&ponapp_extras->required_virtual_machine_build, pocci_response, occi, onapp_extras, "required_virtual_machine_build");
  set_int_from_occi_response_attribute(&ponapp_extras->required_virtual_machine_startup, pocci_response, occi, onapp_extras, "required_virtual_machine_startup");
  set_string_from_occi_response_attribute(&ponapp_extras->cpu_shares, pocci_response, occi, onapp_extras, "cpu_shares");
}

/*  -----------------------------------------------------------------  */
/* 	    r e s o l v e _ o a _ c o n f i g u r a t i o n               */
/*  -----------------------------------------------------------------  */
const struct cords_onapp_extras_handle resolve_cords_onapp_extras_handle( char * sptr, char *agent, char *tls )
{
  struct cords_onapp_extras_handle handle = { 0 };
#ifdef _USE_OCCI_ONAPPEXTRAS
  struct cords_onapp_extras *ponapp_extras = NULL;
  struct occi_response *pocci_response_list = NULL;
  struct occi_response *pocci_response_get_onapp_extras = NULL;

  pocci_response_list = (struct occi_response *)cords_retrieve_named_instance_list(
      "onapp_extras" // category
    , "occi.onapp_extras.name" // namename
    , sptr // namevalue
    , agent // agent
    , tls // tls
  );

  if (pocci_response_list == NULL)
  {
    // TODO Error couldn't find onapp_extras
  }
  else
  {
    pocci_response_get_onapp_extras = occi_simple_get(pocci_response_list->first->value, agent, tls);

    if (pocci_response_get_onapp_extras == NULL)
    {
      // TODO Error Couldn't get onapp_extras instance data.
    }
    else
    {
      ponapp_extras = allocate_cords_onapp_extras();
      if (ponapp_extras == NULL)
      {
        // TODO Error ran out of memory allocating ponapp_extras
      }
      else
      {
        extract_onapp_extras_from_occi_response(ponapp_extras, pocci_response_get_onapp_extras);

        // Got it !
        handle.owner = 1; // This handle owns its pointer.
        handle.ponapp_extras = ponapp_extras;
      }

      pocci_response_get_onapp_extras = occi_remove_response(pocci_response_get_onapp_extras);
    }

    pocci_response_list = occi_remove_response(pocci_response_list);
  }

  return handle;
#else // !_USE_OCCI_ONAPPEXTRAS
  Fail to build
  // This functionality was out of date.  If required, update to match resolve_oa_configuration for backend access
#endif // _USE_OCCI_ONAPPEXTRAS
}

/*  -----------------------------------------------------------------  */
/*      	s t o p  _ o n a p p _ p r o v i s i o n i n g         */
/*  -----------------------------------------------------------------  */
private	struct oa_response * stop_onapp_provisioning( struct onapp * pptr, struct onapp_action_context *context )
{
	int	   status;
	struct oa_response * oaptr;
	const struct oa_config * config=(struct oa_config *) 0;
	struct	data_element * fptr;
	char   reference[512];
	char *	vptr;

	if (!( config = use_onapp_configuration_action( pptr->profile )))
		return((struct oa_response *) 0);
	else
	{
		/* ------------------------------------------- */
		/* perform pre-release actions for destruction */
		/* ------------------------------------------- */
		sprintf(reference,"%s/%s/%s",OnAppConfiguration.identity,_CORDS_ONAPP,pptr->id);

		if ( use_cosacs_agent( pptr->agent ) )
		{
			if ( cosacs_test_interface( pptr->hostname, _COSACS_STOP_TIMEOUT, _COSACS_STOP_RETRY ) )
			{
				cosacs_metadata_instructions( 
					pptr->hostname, _CORDS_RELEASE,
					reference, OnAppConfiguration.publisher, pptr->account );
			}
		}

		oaptr = oa_stop_server(config, pptr);

		if (oaptr != NULL && oaptr->response != NULL && oaptr->response->status < 400)
		{
			if (pptr->state != _OCCI_IDLE)
			{
				pptr->state = _OCCI_IDLE;
			}
		}
//		if ( pptr->hostname )
//			pptr->hostname = liberate(pptr->hostname);
//		if ( pptr->ipaddress )
//			pptr->ipaddress = liberate(pptr->ipaddress);
		return( oaptr );
	}
}

/*  -----------------------------------------------------------------  */
/*      s t a r t _ o n a p p _ p r o v i s i o n i n g                */
/*  -----------------------------------------------------------------  */
private	struct oa_response * start_onapp_provisioning( struct onapp * pptr, struct onapp_action_context *context )
{
	struct oa_response * oaptr;
	const struct oa_config * config=(struct oa_config *) 0;

	if (!( config = use_onapp_configuration_action( pptr->profile )))
	{
		oaptr = NULL;
	}
	else
	{
		oaptr = oa_start_server(config, pptr);

		if (oaptr != NULL && oaptr->response != NULL && oaptr->response->status < 400)
		{
			if (pptr->state != _OCCI_ACTIVE)
			{
				pptr->state = _OCCI_ACTIVE;
			}
		}
	}

	return( oaptr );
}

/*  -----------------------------------------------------------------  */
/*      s h u t d o w n _ o n a p p _ p r o v i s i o n i n g          */
/*  -----------------------------------------------------------------  */
private	struct oa_response * shutdown_onapp_provisioning( struct onapp * pptr, struct onapp_action_context *context )
{
	int	   status;
	struct oa_response * oaptr;
	const struct oa_config * config=(struct oa_config *) 0;
	struct	data_element * fptr;
	char   reference[512];
	char *	vptr;

	if (!( config = use_onapp_configuration_action( pptr->profile )))
		return((struct oa_response *) 0);
	else
	{
		/* ------------------------------------------- */
		/* perform pre-release actions for destruction */
		/* ------------------------------------------- */
		sprintf(reference,"%s/%s/%s",OnAppConfiguration.identity,_CORDS_ONAPP,pptr->id);

		if ( use_cosacs_agent( pptr->agent ) )
		{
			if ( cosacs_test_interface( pptr->hostname, _COSACS_STOP_TIMEOUT, _COSACS_STOP_RETRY ) )
			{
				cosacs_metadata_instructions(
					pptr->hostname, _CORDS_RELEASE,
					reference, OnAppConfiguration.publisher, pptr->account );
			}
		}

		oaptr = oa_shutdown_server(config, pptr);

		if (pptr->state != _OCCI_IDLE)
		{
			pptr->state = _OCCI_IDLE;
		}

		return( oaptr );
	}
}

/*  -----------------------------------------------------------------  */
/*      r e s t a r t _ o n a p p _ p r o v i s i o n i n g            */
/*  -----------------------------------------------------------------  */
private	struct oa_response * restart_onapp_provisioning( struct onapp * pptr, struct onapp_action_context *context )
{
	struct oa_response * oaptr;
	const struct oa_config * config=(struct oa_config *) 0;

	if (!( config = use_onapp_configuration_action( pptr->profile )))
	{
		oaptr = NULL;
	}
	else
	{
		oaptr = oa_restart_server(config, pptr);

		if (pptr->state != _OCCI_ACTIVE)
		{
			pptr->state = _OCCI_ACTIVE;
		}
	}

	return( oaptr );
}

/*  -----------------------------------------------------------------  */
/*      d e s t r o y _ o n a p p _ p r o v i s i o n i n g            */
/*  -----------------------------------------------------------------  */
private	struct oa_response * destroy_onapp_provisioning( struct onapp * pptr, struct onapp_action_context *context )
{
	struct oa_response * oaptr;
	const struct oa_config * config=(struct oa_config *) 0;

	if (!( config = use_onapp_configuration_action( pptr->profile )))
	{
		oaptr = NULL;
	}
	else
	{
		oaptr = oa_destroy_server(config, pptr);

		if (oaptr != NULL && oaptr->response != NULL && oaptr->response->status < 400)
		{
			pptr->state = _OCCI_IDLE;
			pptr->build_state = allocate_string("deleted");
		}
	}

	return( oaptr );
}

/*	-----------------------------------------------------------------  */
/*	    	t e r m i n a t e _ o n a p p _ c o n t r a c t    	   */
/*	-----------------------------------------------------------------  */
private	int	terminate_onapp_contract( int status, struct cords_oa_contract * cptr )
{
	
	return( 0 );
}

	/* ------------- */
#endif	/* _oacontract_c */
	/* ------------- */


