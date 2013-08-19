/*
 * testrest.c
 *
 *  Created on: 9 Jul 2013
 *      Author: me
 */
#include <stdio.h>
#include "standard.h"
//#include "broker.h"
#include "restresponse.h"
#include "rest.h"
//#include "occi.h"
//#include "document.h"
//#include "cordspublic.h"
//#include "occipublisher.h"
//#include "occiresolver.h"

//#include "oaconfig.h"
//#include "oaclient.h"
//#include "cb.h"

// Required by CompatibleOne
public  int check_debug()   { return(0);   }
public  int check_verbose()   { return(0);   }
public  char *  default_publisher() { return(0); }
public  char *  default_operator()  { return(0);    }
public  char *  default_tls()   { return(0);   }
public  char *  default_zone()    { return(0);    }
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

// Rest code
int handle_request(char *purl, FILE *hOutput)
{
  int result = 0;
  struct rest_response *prest_response = NULL;
  struct rest_header *prest_header = NULL;

  static size_t const url_buffer_size = 1024;
  char url_buffer[url_buffer_size];

  if (isdigit(*purl))
  {
    // CompatibleOne requires HTTP prefix.
    snprintf(url_buffer, url_buffer_size, "http://%s", purl);
    purl = url_buffer;
  }

  prest_response = (struct rest_response *)rest_client_post_request(
      purl
      , (char *)NULL // tls
      , "testrest" // agent
      , (char *)NULL // filename
      , (struct rest_header *)NULL // rest_header
  );

  if (prest_response != NULL)
  {
    for (prest_header = prest_response->first; prest_header != NULL; prest_header = prest_header->next)
    {
      fprintf(hOutput, " %s : %s\n", prest_header->name, prest_header->value);
    }

    write_filecontent_to_filehandle(prest_response->body, hOutput, "Response Body:\n", "\n");
  }

  liberate_rest_response(prest_response);
//  struct rest_client * prest_client = rest_allocate_client();
//
//  if (prest_client != NULL)
//  {
//    prest_client->
//
//    liberate_rest_client(prest_client);
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

  if (strncmp(option_holder->long_option, (argv[arg_index])+arg_offset, option_length) == 0)
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
  }

  return arg_index;
}

int test_rest_command (int argc, char * argv[])
{
  struct arg_holder args[] =
  {
      { "u", "url", 3, AB_TRUE, AT_STRING, { 0 }, { "127.0.0.1:8080" } }
  };
  enum ArgIndices { AI_URL };
  size_t count_args = sizeof(args) / sizeof (*args);
  struct arg_holder *option_iter;
  char const *purl = NULL;
  char * * argiter;
  int set_arg_result;

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

          } // Ends if no long option name
          else if ((*argiter)[1] == ' ') // If short option has space
          {

          } // Ends if long option has space
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
        } // Ends else not long option string
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

  handle_request(args[AI_URL].value.string_val, stdout);
  //rest_allocate_client

  return 0;
}

int test_rest_banner(int argc, char * argv[])
{
  printf("\n%s: When poking procci's just isn't enough.", argv[0]);
  printf("\n Parameters:");
  //printf("\n  -h|--host [HOST]  Host to send a request to.");
  printf("\n  -u|--url [URL]    URL to send request to");
  printf("\n  -i|--input [INPUTFILE]  File to read input from.");

//  printf("\n   --verbose                         activate status  messages       ");
//  printf("\n   --debug                           activate debug   messages       ");
//  printf("\n   --php                             format output for php           ");
//  printf("\n   --tls       <filename>            specify TLS configuration       ");
//  printf("\n   --accept    <filename>            specify ACCEPT MIME type        ");
//  printf("\n   --publisher <hostname>            set publisher URL               ");
//  printf("\n   --agent     <agentname>           set client agent identification \n\n");
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
    result = test_rest_command(argc, argv);
  }
  return result;
}
