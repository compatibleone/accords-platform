/*
 * testocci.c
 *
 *  Created on: 9 Jul 2013
 *      Author: me
 */
#include <stdio.h>
#include "standard.h"
//#include "broker.h"
#include "cp.h" // cords_retrieve_named_instance_list()
#include "restresponse.h"
#include "rest.h"
#include "occiclient.h"
#include "occi.h"
#include "accords.h"
//#include "document.h"
//#include "cordspublic.h"
//#include "occipublisher.h"
//#include "occiresolver.h"

//#include "oaconfig.h"
//#include "oaclient.h"
//#include "cb.h"

// Cut'n'Paste from auto-generated onapp_extras.h
struct  cords_onapp_extras
{
  char *  id;
  char *  initial_root_password;
  char *  name;
  char *  initial_root_password_encryption_key;
  int required_ip_address_assignment;
  char *  disk_type_of_format;
  char *  label;
  int required_automatic_backup;
  char *  swap_size;
  int required_virtual_machine_build;
  int required_virtual_machine_startup;
  char *  cpu_shares;
};

// Required by CompatibleOne
struct  accords_configuration TestOcciConfiguration = {
  .monitor = 0, .autopub=0,
  .verbose=0, .debug=0, .threads=0, .ipv6=0,
  .tls = (char *) 0,
  .identity = (char *) 0,
  .publisher = _CORDS_DEFAULT_PUBLISHER,
  .operator = _CORDS_DEFAULT_OPERATOR,
  .user = _CORDS_DEFAULT_USER,
  .password = _CORDS_DEFAULT_PASSWORD,
  .resthost = "http",  .restport = 80,
  .chathost = "xmpp",  .chatport = 8000,
  .domain = "domain",
  .config = "accords.xml",
  .zone = "europe",
  .storage = "storage",
  .component = "testocci",
  .firstcat = (struct occi_category *) 0,
  .lastcat = (struct occi_category *) 0
};

public  int check_debug()   { return(TestOcciConfiguration.debug);   }
public  int check_verbose()   { return(TestOcciConfiguration.verbose);   }
public  char *  default_publisher() { return(TestOcciConfiguration.publisher); }
public  char *  default_operator()  { return(TestOcciConfiguration.operator);    }
public  char *  default_tls()   { return(TestOcciConfiguration.tls);   }
public  char *  default_zone()    { return(TestOcciConfiguration.zone);    }
public  int failure( int e, char * m1, char * m2 )
{
  if ( e )
  {
    printf("\n*** failure %u",e);
    if ( m1 )
      printf(" : %s",m1);
    if ( m2 )
      printf(" : %s",m2);
    printf(" **`\n");
  }
  return( e );
}

private void  testocci_configuration()
{
  load_accords_configuration( &TestOcciConfiguration, "testocci" );
  return;
}

// Ends required by CompatibleOne

// Helper functions
size_t write_filecontent_to_filehandle(char const * const filename, FILE* h, char const * const prefix, char const * const suffix)
{
  FILE* hbody;
  const size_t buffersize = 4095;
  char buffer[buffersize + 1];
  buffer[0] = '\0';
  buffer[buffersize] = '\0';
  size_t bytes = 0;
  size_t readbytes = 0;
  size_t totalreadbytes = 0;
  size_t writebytes = 0;

  if (prefix != NULL) {
    fwrite(prefix, 1, strlen(prefix), h);
  }

  if (filename != NULL)
  {
    if ((hbody = fopen(filename, "rb"))) { // If opened input file OK
      while (1) {
        bytes =  fread(buffer + readbytes, 1, (buffersize - readbytes), hbody);
        if (bytes == 0) { // If end of read
          fwrite(buffer, 1, readbytes, h);
          break;

        } // Ends if end of read
        else { // Else read bytes
          readbytes += bytes;
          totalreadbytes += bytes;
          if (readbytes >= buffersize) { // If filled the buffer
            fwrite(buffer, 1, readbytes, h);
            // Reset the buffer.
            buffer[0] = '\0';
            buffer[buffersize] = '\0';
            readbytes = 0;

          } // Ends if filled the buffer
        } // Ends else read bytes
      } // Ends while reading bytes
      buffer[readbytes] = '\0';
      fclose(hbody);

    } // Ends if opened input file OK

  }

  if (suffix != NULL) {
    fwrite(suffix, 1, strlen(suffix), h);
  }

  fflush(h);

  return totalreadbytes;
}

void dump_url(struct url *purl, FILE *hOutput)
{
  fwrite("Url: '", 1, 5, hOutput);
  fwrite(purl->host, 1, strlen(purl->host), hOutput);

  if (purl->port != 0)
  {
    fprintf(hOutput, ":%d", purl->port);
  }

  if (purl->object != NULL)
  {
    fwrite(purl->object, 1, strlen(purl->object), hOutput);
  }

  fwrite("'", 1, 1, hOutput);

  if (purl->service != NULL)
  {
    fwrite(". Service: '", 1, 12, hOutput);
    fwrite(purl->service, 1, strlen(purl->service), hOutput);
    fwrite("'.", 1, 2, hOutput);
  }
}

/*
struct  occi_attribute
{
  struct  occi_attribute  * previous;
  struct  occi_attribute  * next;
  struct  occi_category   * parent;
  char          * name;
  char          * type;
  char          * units;
  char          * validation;
  char          * defaultvalue;
  int       mandatory;
  int       indexed;
  int       immutable;
};
*/
void dump_occi_attribute(struct occi_attribute *pocci_attribute, FILE *hOutput)
{
  if (pocci_attribute == NULL)
  {
    fwrite("occi_attribute (null).", 1, 22, hOutput);
  }
  else
  {
    fprintf(hOutput,
        "occi_attribute name='%s' "
        "type='%s' "
        "units='%s' "
        "validation='%s' "
        "defaultvalue='%s' "
        "mandatory='%d' "
        "indexed='%d' "
        "immutable='%d'."
      , pocci_attribute->name
      , pocci_attribute->type
      , pocci_attribute->units
      , pocci_attribute->validation
      , pocci_attribute->defaultvalue
      , pocci_attribute->mandatory
      , pocci_attribute->indexed
      , pocci_attribute->immutable
    );
  }
}
/*
struct  occi_category
{
  struct  occi_category   * previous;
  struct  occi_category   * next;
  struct  occi_attribute  * first;
  struct  occi_attribute  * last;
  struct  occi_action * firstact;
  struct  occi_action * lastact;
  char      * domain;
  char          * id;
  char      * scheme;
  char      * class;
  char      * title;
  char      * rel;
  char      * version;
  char      * location;
  char      * price;
  char      * rating;
  int       access;
  void      * interface;
  void      * callback;
  void      * payload;
  char *  (*html_rendering)(struct occi_category * cptr,struct rest_header  * hptr );

};
 */
void test_dump_occi_category(struct occi_category *pocci_category, FILE *hOutput)
{
  struct occi_attribute *pocci_attribute_iter = NULL;

  if (pocci_category == NULL)
  {
    fwrite("occi_category (null).", 1, 21, hOutput);
  }
  else
  {
    fprintf(hOutput,
        "occi_category domain='%s' "
        "id='%s' "
        "scheme='%s' "
        "class='%s' "
        "title='%s' "
        "rel='%s' "
        "version='%s' "
        "location='%s' "
        "price='%s' "
        "rating='%s' "
        "access='%d' "
        "interface='0x%p' "
        "callback='0x%p' "
        "payload='0x%p'."
      , pocci_category->domain
      , pocci_category->id
      , pocci_category->scheme
      , pocci_category->class
      , pocci_category->title
      , pocci_category->rel
      , pocci_category->version
      , pocci_category->location
      , pocci_category->price
      , pocci_category->rating
      , pocci_category->access
      , pocci_category->interface
      , pocci_category->callback
      , pocci_category->payload
    );
  }
  if (pocci_category->first == NULL)
  {

  }
  else
  {
    fputc('\n', hOutput);

    for (pocci_attribute_iter = pocci_category->first; pocci_attribute_iter != NULL; pocci_attribute_iter=pocci_attribute_iter->next)
    {
      dump_occi_attribute(pocci_attribute_iter, hOutput);
      fputc('\n', hOutput);
    }
  }
}
/*
struct  rest_response
{
  struct  rest_header * first;
  struct  rest_header * last;
  int     status;
  char      * version;
  char      * message;
  int     type;
  char      * body;
};
*/
void dump_rest_response(struct rest_response *prest_response, FILE *hOutput)
{
  struct rest_header *prest_header;
  fprintf(hOutput, "rest_response status=%d version='%s' message='%s' type=%d body='%s'\n"
    , prest_response->status
    , prest_response->version
    , prest_response->message
    , prest_response->type
    , prest_response->body
  );
  for (prest_header = prest_response->first; prest_header != NULL; prest_header = prest_header->next)
  {
    fprintf(hOutput, " %s:%s\n", prest_header->name, prest_header->value);
  }

  write_filecontent_to_filehandle(prest_response->body, hOutput, "Response Body:\n", "\n");
}
/*
struct  occi_response
{
  struct  occi_element * first;
  struct  occi_element * last;
  int type;
  char *  name;
  char *  host;
  struct  occi_category * category;
  struct  rest_response * response;
};
 */
void dump_occi_response(struct occi_response* pocci_response, FILE* hOutput)
{
  struct occi_element* pocci_element_iter = (struct occi_element *) NULL;
  char* psz = NULL;
  unsigned int counter = 0;

  fprintf(hOutput, "occi_response type='%d' name='%s' host='%s'\n", pocci_response->type, pocci_response->name, pocci_response->host);
  for (pocci_element_iter = pocci_response->first;
      pocci_element_iter != (struct occi_element*) 0;
      pocci_element_iter = pocci_element_iter->next)
  {
    if (pocci_element_iter->name == NULL )
    {
      fprintf(hOutput, "%u] occi_response element missing name\n", counter);
      continue;
    }
    else if (pocci_element_iter->value == NULL )
    {
      fprintf(hOutput, "%u] occi_response element missing value\n", counter);
      continue;
    }
    else
    {
      psz = occi_category_id(pocci_element_iter->value);
      fprintf(hOutput,
        "%u] occi_response element name='%s', value='%s'. category id='%s'\n",
        counter, pocci_element_iter->name, pocci_element_iter->value, psz
      );
    }
    //else  sprintf(url,"%s%s",buffer,vptr);
  }

  test_dump_occi_category(pocci_response->category, hOutput);
  fputc('\n', hOutput);
  dump_rest_response(pocci_response->response, hOutput);
  fputc('\n', hOutput);
}

struct occi_response * dump_occi_response_liberate(struct occi_response * pocci_response, FILE* hOutput)
{
  if (pocci_response == NULL)
  {
    fputs("No response\n", hOutput);
  }
  else
  {
    dump_occi_response(pocci_response, hOutput);
  }

  return occi_remove_response(pocci_response);
}

void dump_occi_client(struct occi_client *pocci_client, FILE *hOutput)
{
  fwrite("===Occi Client===\n", 1, 18, hOutput);
  fwrite("Target ", 1, 7, hOutput);
  dump_url(pocci_client->target, hOutput);
  fputc('\n', hOutput);

  fprintf(hOutput, "Host: '%s'\nAgent: '%s'\nMimetype: '%s'\nCategories: %d\nStarted: %d\nRequests: %d\nFailures: %d\n"
     , pocci_client->host
     , pocci_client->agent
     , pocci_client->mimetype
     , pocci_client->categories
     , pocci_client->started
     , pocci_client->requests
     , pocci_client->failures
  );
  fputc('\n', hOutput);
}




static int const interractive = 0;
static char test_agent[] = "Bob's Hardware and Shed Maintenance 5.3";

struct printf_field
{
  char * const name;
  char const * const field;
};

void dump_onapp_extras_occi(struct occi_response *pocci_response, FILE *hOutput)
{
  static struct printf_field const onapp_printf_fields[] =
  {
      { "required_automatic_backup" }
    , { "initial_root_password" }
    , { "name" }
    , { "initial_root_password_encryption_key" }
    , { "required_ip_address_assignment" }
    , { "disk_type_of_format" }
    , { "label" }
    , { "required_automatic_backup" }
    , { "swap_size" }
    , { "required_virtual_machine_build" }
    , { "required_virtual_machine_startup" }
    , { "cpu_shares" }
  };
  static size_t const onapp_printf_fields_length = sizeof(onapp_printf_fields) / sizeof(*onapp_printf_fields);

  //struct occi_attribute *pocci_attribute_iter;
  struct printf_field const *pfield_iter;
  char *pattribute_iter;

  fputs("===Get Onapp occi_response===\n", hOutput);
  dump_occi_response(pocci_response, hOutput);
  fputc('\n', hOutput);
  //else if (!( contract.node.message = occi_simple_get( contract.node.id, _CORDS_CONTRACT_AGENT, default_tls() ) ))

  fputs("===Parsing out onapp_extras===\n", hOutput);
  for (pfield_iter = onapp_printf_fields; pfield_iter < onapp_printf_fields + onapp_printf_fields_length; ++pfield_iter)
  {
    pattribute_iter = occi_extract_atribut(pocci_response, "occi", "onapp_extras", pfield_iter->name);
//    if (pattribute_iter == NULL)
//    {
//      pocci_attribute_iter = occi_resolve_attribute(pocci_response->category, pfield_iter->name);
//    }
    fprintf(hOutput, " %s=", pfield_iter->name);
    fprintf(hOutput, pfield_iter->field != NULL ? pfield_iter->field : "%s", pattribute_iter);
    fputc('\n', hOutput);
  }
  fputc('\n', hOutput);
}

void dump_onapp_extras(struct cords_onapp_extras *ponapp_extras, FILE *hOutput)
{
  fprintf(hOutput, "  id = %s\n"
    "  initial_root_password = %s\n"
    "  name = %s\n"
    "  initial_root_password_encryption_key = %s\n"
    "  required_ip_address_assignment = %d\n"
    "  disk_type_of_format = %s\n"
    "  label = %s\n"
    "  required_automatic_backup = %d\n"
    "  swap_size = %s\n"
    "  required_virtual_machine_build = %d\n"
    "  required_virtual_machine_startup = %d\n"
    "  cpu_shares = %s\n"
    , ponapp_extras->id
    , ponapp_extras->initial_root_password
    , ponapp_extras->name
    , ponapp_extras->initial_root_password_encryption_key
    , ponapp_extras->required_ip_address_assignment
    , ponapp_extras->disk_type_of_format
    , ponapp_extras->label
    , ponapp_extras->required_automatic_backup
    , ponapp_extras->swap_size
    , ponapp_extras->required_virtual_machine_build
    , ponapp_extras->required_virtual_machine_startup
    , ponapp_extras->cpu_shares
  );
}


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

// OCCI code
int handle_request(char *purl, char *pcategory, char *pname, char *pvalue, FILE *hOutput)
{
  static size_t const url_buffer_size = 1024;
  char url_buffer[url_buffer_size];

  int result = 0;
  struct occi_client *pocci_client = (struct occi_client *) 0;
  struct occi_request *pocci_request = (struct occi_request *) 0;
  struct occi_response *pocci_response_list = (struct occi_response *) 0;
  struct occi_response *pocci_response_instance = (struct occi_response *)0;
  struct occi_response *pocci_response_get_onapp;
  struct occi_element *pocci_element = (struct occi_element *)0;
  struct occi_element *pocci_element_iter = (struct occi_element *)0;
  char *publication = NULL;
  char *psz = NULL;
  unsigned int counter = 0;
  struct cords_onapp_extras onapp_extras = { 0 };

  if (isdigit(*purl))
  {
    // CompatibleOne requires HTTP prefix.
    snprintf(url_buffer, url_buffer_size, "http://%s", purl);
    purl = url_buffer;
  }

  if (interractive)
  {
    puts("> Press a key to continue: ");
    getchar();
    putchar('\n');
  }

  pocci_response_list = (struct occi_response *)cords_retrieve_named_instance_list(
      pcategory
    , pname
    , pvalue
//      "onapp_extras" // category
//    , "name" // namename
//    , "4042748f-1913-4456-8780-af3579af59aa" // namevalue
    , test_agent // agent
    , NULL // tls
  );
  fprintf(hOutput, "===Retrieve named instance list category='%s' name='%s' value='%s'===\n", pcategory, pname, pvalue);
  dump_occi_response(pocci_response_list, hOutput);
  fputc('\n', hOutput);

  //pocci_client = occi_create_client( purl, test_agent, 0 );

  pocci_response_get_onapp = occi_simple_get(pocci_response_list->first->value, test_agent, 0);

  fputs("occi onapp_extras data\n", hOutput);
  dump_onapp_extras_occi(pocci_response_get_onapp, hOutput);
  fputc('\n', hOutput);
  extract_onapp_extras_from_occi_response(&onapp_extras, pocci_response_get_onapp);
  fputs("===onapp_extras===\n", hOutput);
  dump_onapp_extras(&onapp_extras, hOutput);
  fputc('\n', hOutput);

  if (0)
  {
  pocci_response_instance = (struct occi_response *)cords_retrieve_named_instance(pocci_response_list, test_agent, 0);
  fprintf(hOutput, "===Retrieve named instance category='%s' name='%s' value='%s'===\n", pcategory, pname, pvalue);
  dump_occi_response(pocci_response_list, hOutput);
  fputc('\n', hOutput);
  }

  if (pocci_client != NULL)
  {
    dump_occi_client(pocci_client, hOutput);

    if (0)
    {
    //pocci_element = occi_create_element( "occi.vm.image" , iid );
      occi_remove_response(pocci_response_list);
      pocci_response_list = occi_simple_list( purl, pocci_element, test_agent, default_tls() );
      pocci_response_list = dump_occi_response_liberate(pocci_response_list, hOutput);
    }
//    if (!( yptr = occi_simple_list( buffer, dptr, _CORDS_CONTRACT_AGENT, default_tls() ) ))

    pocci_request = occi_create_request( pocci_client, "/-/", _OCCI_NORMAL );

    if (pocci_request != NULL)
    {


      pocci_request = occi_remove_request(pocci_request);
    }

    pocci_client = (struct occi_client *)liberate_occi_client(pocci_client);
  }

  if (pocci_element != NULL)
  {
    occi_remove_element(pocci_element);
  }

  occi_remove_response(pocci_response_get_onapp);
  occi_remove_response(pocci_response_instance);
  occi_remove_response(pocci_response_list);
//  prest_response = (struct rest_response *)rest_client_post_request(
//      purl
//      , (char *)NULL // tls
//      , "testocci" // agent
//      , (char *)NULL // filename
//      , (struct rest_header *)NULL // rest_header
//  );
//
//  if (prest_response != NULL)
//  {
//    for (prest_header = prest_response->first; prest_header != NULL; prest_header = prest_header->next)
//    {
//      fprintf(hOutput, " %s : %s\n", prest_header->name, prest_header->value);
//    }
//
//    write_filecontent_to_filehandle(prest_response->body, hOutput, "Response Body:\n", "\n");
//  }


  return result;
}

enum ArgType { AT_INT, AT_STRING, AT_BOOLEAN };
enum ArgBoolean { AB_FALSE, AB_TRUE };

struct arg_holder
{
  char const *short_option;
  char const *long_option;
  size_t long_option_length;
  enum ArgBoolean has_arg;
  enum ArgType type;
  union ArgVal
  {
    char *string_val;
    int int_val;
    int bool_val;
  } value, default_value;
  enum ArgBoolean set;
};

int _set_arg(struct arg_holder *option_holder, char const * option, size_t option_length, char *argv[], int argc, int arg_index, int arg_offset)
{
  static size_t const long_buffer_length = 32;
  char long_buffer[long_buffer_length];
  char *arg_value;
  int result = 0;

  if (option != NULL)
  {
    if (strncmp(option, (argv[arg_index])+arg_offset, option_length) == 0)
    {
      if (option_holder->has_arg != AB_FALSE && arg_index + 1 < argc)
      {
        ++arg_index;
        arg_value = argv[arg_index];

        switch(option_holder->type)
        {
          case AT_STRING:
          default:
            option_holder->value.string_val = arg_value;
            break;
          case AT_INT:
            option_holder->value.int_val = atoi(arg_value);
            break;
          case AT_BOOLEAN:
            if (isdigit(arg_value))
            {
              option_holder->value.bool_val = (atoi(arg_value) == 0 ? 0 : 1);
            }
            else if (strncasecmp("false", arg_value, 5) == 0)
            {
              option_holder->value.bool_val = AB_FALSE;
            }
            else
            {
              option_holder->value.bool_val = AB_TRUE;
            }
            break;
        }
      }
      else
      {
        switch(option_holder->type)
        {
          case AT_BOOLEAN:
            option_holder->value.bool_val = AB_TRUE;
            break;
        }
      }
      option_holder->set = 1;
      result = arg_index;
    }
  }

  return result;
}

int test_occi_command (int argc, char * argv[])
{
  struct arg_holder args[] =
  {
      { .short_option = "u",  .long_option = "url",       .long_option_length = 3, .has_arg = AB_TRUE,  .type = AT_STRING,  .value = { 0 }, .default_value = { "http://co.market.onapp.com::8080" } }
     ,{ .short_option = "p",  .long_option = "publisher", .long_option_length = 9, .has_arg = AB_TRUE,  .type = AT_STRING,  .value = { 0 }, .default_value = { "http://co.market.onapp.com:8086" } }
     ,{ .short_option = "c",  .long_option = "category" , .long_option_length = 8, .has_arg = AB_TRUE,  .type = AT_STRING,  .value = { 0 }, .default_value = { "onapp_extras" } }
     ,{ .short_option = "n",  .long_option = "name"     , .long_option_length = 4, .has_arg = AB_TRUE,  .type = AT_STRING,  .value = { 0 }, .default_value = { "name" } }
     ,{ .short_option = "v",  .long_option = "value"    , .long_option_length = 5, .has_arg = AB_TRUE,  .type = AT_STRING,  .value = { 0 }, .default_value = { 0 } }
     ,{ .short_option = NULL, .long_option = "debug",     .long_option_length = 5, .has_arg = AB_FALSE, .type = AT_BOOLEAN, .value = { 0 }, .default_value = { 0 } }
     ,{ .short_option = NULL, .long_option = "verbose",   .long_option_length = 7, .has_arg = AB_FALSE, .type = AT_BOOLEAN, .value = { 0 }, .default_value = { 0 } }
  };
  enum ArgIndices { AI_URL, AI_PUBLISHER, AI_CATEGORY, AI_NAME, AI_VALUE, AI_DEBUG, AI_VERBOSE };
  size_t count_args = sizeof(args) / sizeof (*args);
  struct arg_holder *option_iter;
  char const *purl = NULL;
  char * * argiter;
  int set_arg_result;

  // publisher should be http://co.market.onapp.com:8086
  testocci_configuration();

  // Handle CompatibleOne args.
  for (argiter = argv+1; argiter < argv + argc; ++argiter)
  {
    if (*argiter != NULL && **argiter != '\0') // If arg not NULL
    {
      if ((*argiter)[0] == '-') // If option string
      {
        if ((*argiter)[1] == '-') // If long option string
        {
          if ((*argiter)[2] == '\0') // If no long option name
          {

          } // Ends if no long option name
          else if ((*argiter)[2] == ' ') // If long option has space
          {

          } // Ends if long option has space
          else // Else long option
          {
            set_arg_result = accords_configuration_option( &(*argiter)[2], argiter-argv, argv );
            if (set_arg_result != 0)
            {
              argiter = &argv[set_arg_result];
              continue; // This was an accords configuration parameter.
            }
          } // Ends else long option
        } // Ends if long option string
        else // Else not long option string
        {
          if ((*argiter)[1] == '\0') // If no short option name
          {

          } // Ends if no short option name
          else if ((*argiter)[1] == ' ') // If short option has space
          {

          } // Ends if short option has space
          else // Else short option
          {
            set_arg_result = accords_configuration_option( &(*argiter)[1], argiter-argv, argv );
            if (set_arg_result != 0)
            {
              argiter = &argv[set_arg_result];
              continue; // This was an accords configuration parameter.
            }
          } // Ends else short option
        } // Ends else not short option string
      } // Ends if option string
    } // Ends if arg not NULL
  } // Ends loop over args

  // Handle other command line arguments.
  for (argiter = argv+1; argiter < argv + argc; ++argiter)
  {
    if (*argiter != NULL && **argiter != '\0') // If arg not NULL
    {
      if ((*argiter)[0] == '-') // If option string
      {
        if ((*argiter)[1] == '-') // If long option string
        {
          if ((*argiter)[2] == '\0') // If no long option name
          {

          } // Ends if no long option name
          else if ((*argiter)[2] == ' ') // If long option has space
          {

          } // Ends if long option has space
          else // Else long option
          {
            for (option_iter = args; option_iter < args + count_args; ++option_iter)
            {
              set_arg_result = _set_arg(option_iter, option_iter->long_option, option_iter->long_option_length, argv, argc, argiter-argv, 2);
              if (set_arg_result != 0)
              {
                argiter = &argv[set_arg_result];
                break;
              }
            } // Ends loop over options
          } // Ends else long option
        } // Ends if long option string
        else // Else not long option string
        {
          if ((*argiter)[1] == '\0') // If no short option name
          {

          } // Ends if no short option name
          else if ((*argiter)[1] == ' ') // If short option has space
          {

          } // Ends if short option has space
          else // Else short option
          {
            for (option_iter = args; option_iter < args + count_args; ++option_iter)
            {
              set_arg_result = _set_arg(option_iter, option_iter->short_option, 1, argv, argc, argiter-argv, 1);
              if (set_arg_result != 0)
              {
                argiter = &argv[set_arg_result];
                break;
              }
            } // Ends loop over options
          } // Ends else short option
        } // Ends else not short option string
      } // Ends if option string
    } // Ends if arg not NULL
  } // Ends loop over args

  for (option_iter = args; option_iter < args + count_args; ++option_iter)
  {
    if (option_iter->set == 0)
    {
      option_iter->value = option_iter->default_value;
    }
  } // Ends loop over options

  if (args[AI_PUBLISHER].value.string_val != NULL) // If publisher set
  {
    TestOcciConfiguration.publisher = args[AI_PUBLISHER].value.string_val;

  } // Ends if publisher set

  TestOcciConfiguration.debug = args[AI_DEBUG].value.bool_val;
  TestOcciConfiguration.verbose = args[AI_VERBOSE].value.bool_val;

  initialise_occi_resolver( TestOcciConfiguration.publisher, (char *) 0, (char *) 0, (char *) 0 );
  handle_request(args[AI_URL].value.string_val, args[AI_CATEGORY].value.string_val, args[AI_NAME].value.string_val, args[AI_VALUE].value.string_val, stdout);

  return 0;
}

int test_rest_banner(int argc, char * argv[])
{
  printf("\n%s: When poking procci's just isn't enough.", argv[0]);
  printf("\n Parameters:");
  printf("\n  -u|--url [URL]    URL to send request to");
  printf("\n  -i|--input [INPUTFILE]  File to read input from.");

  printf("\n");
  return 0;
}

int  main( int argc, char * argv[] )
{
  int result = 0;
  char cwd[1024];
  getcwd(cwd, 1024);
  fprintf(stdout, "Current Working Directory: '%s'\n", cwd);

  if ( argc == 1 )
  {
    result = test_rest_banner(argc, argv);
  }
  else
  {
    result = test_occi_command(argc, argv);
  }
  return result;
}
