#ifndef	_oaclient_h
#define	_oaclient_h

#include "standard.h"
#include "rest.h"
#include "url.h"
#include "urlpublic.h"
#include "restclient.h"
#include "json.h"
#include "document.h"
#include "oaconfig.h"

#define _TEXT_NONE      0
#define _TEXT_XML       1
#define _TEXT_JSON      2

struct  oa_response
{
        int     nature;
        char *  content;
        struct  rest_response * response;
        struct  xml_element * xmlroot;
        struct  data_element * jsonroot;
};


public struct	oa_response * oa_liberate_response( struct oa_response * rptr );

public struct	oa_config * oa_initialise_client( char * user, char * pass, char * host, char * agent, char * version, char * tls );

	/* ----------- */
#endif 	/* _oaclient_h */
	/* ----------- */



