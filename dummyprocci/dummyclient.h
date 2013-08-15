#ifndef	_dummyclient_h
#define	_dummyclient_h

#include "standard.h"
#include "rest.h"
#include "url.h"
#include "urlpublic.h"
#include "restclient.h"
#include "json.h"
#include "document.h"
#include "dummyconfig.h"

#define _TEXT_NONE      0
#define _TEXT_XML       1
#define _TEXT_JSON      2

struct  dummy_response
{
        int     nature;
        char *  content;
        struct  rest_response * response;
        struct  xml_element * xmlroot;
        struct  data_element * jsonroot;
};


public struct	dummy_response * dummy_liberate_response( struct dummy_response * rptr );

public struct	dummy_config * dummy_initialise_client( char * user, char * pass, char * host, char * agent, char * version, char * tls );

	/* ----------- */
#endif 	/* _dummyclient_h */
	/* ----------- */



