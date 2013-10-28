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
#ifndef	_rest_h
#define	_rest_h

#include "standard.h"
#include "allocate.h"
#include "connections.h"
#include "base64.h"
#include "restheader.h"
#include "restrequest.h"
#include "restresponse.h"
#include "tlsload.h"

#define	rest_debug_message( a, b )

#define	_rest_server
#define	_rest_client
#define	_rest_extension

#define	_FILE_BODY	0
#define	_TEXT_BODY	1

#define	_MAX_RESTBUFFER 16000
#define	_REST_THREAD_WORKER	1

#define _HTTP_CORS_DFT_AGE      "86400"

#define	_CORDS_REST_PATH	"rest/"
#define	_CORDS_REST_STUB	"resttemp"
#define	_CORDS_LAUNCH_CFG	"/etc/compatibleOneLaunch.cfg"
#define	_HTTP_USER_AGENT	"User-Agent"
#define	_HTTP_ACCEPT		"Accept"
#define	_HTTP_SERVER		"Server"
#define	_HTTP_HOST		"Host"
#define	_HTTP_CONTENT_TYPE	"Content-Type"
#define	_HTTP_CONTENT_LENGTH	"Content-Length"
#define	_HTTP_LOCATION		"Location"
#define	_HTTP_AUTHORIZATION	"Authorization"
#define	_HTTP_WWW_AUTHENTICATE	"WWW-Authenticate"
#define	_HTTP_PROXY_AUTHENTICATE "PROXY-Authenticate"
#define	_HTTP_TRANSFER_ENCODING	"Transfer-Encoding"
#define	_HTTP_CHUNKED		"Chunked"
#define	_HTTP_BASIC		"Basic realm=*"
#define _HTTP_ORIGIN            "Origin"
#define _HTTP_ALLOW             "Allow"
#define _HTTP_CORS_REQ_HEADERS  "Access-Control-Request-Headers"
#define _HTTP_CORS_REQ_METHOD   "Access-Control-Request-Method"
#define _HTTP_CORS_ORIGIN       "Access-Control-Allow-Origin"
#define _HTTP_CORS_CREDENTIALS  "Access-Control-Allow-Credentials"
#define _HTTP_CORS_EXP_HEADERS  "Access-Control-Expose-Headers"
#define _HTTP_CORS_MAX_AGE      "Access-Control-Max-Age"
#define _HTTP_CORS_METHODS      "Access-Control-Allow-Methods"
#define _HTTP_CORS_HEADERS      "Access-Control-Allow-Headers"

#define	_HTTP_OK  		200
#define	_HTTP_CREATED		201
#define	_HTTP_ACCEPTED		202
#define	_HTTP_NO_CONTENT	204
#define	_HTTP_RESET_CONTENT	204

#define	_HTTP_MOVED		301
#define	_HTTP_TEMPORARY		302
#define	_HTTP_TEMPORARY_GET	303
#define	_HTTP_TEMPORARY_NEW	307

#define	_HTTP_BAD_REQUEST	400
#define	_HTTP_UNAUTHORISED	401
#define	_HTTP_PAYMENT_REQUIRED	402
#define	_HTTP_FORBIDDEN		403
#define	_HTTP_NOT_FOUND		404
#define	_HTTP_NOT_ALLOWED	405
#define	_HTTP_NOT_ACCEPTABLE	406
#define	_HTTP_PROXY_AUTHENTICATION	407

struct	rest_server;
struct	rest_client;
struct	rest_extension;

struct	rest_interface
{
	void		      * instance;
	struct rest_server    * (*initialise)(void * i,struct rest_server * sptr );
	int	 	        (*authorise)(void * i,struct rest_client * cptr, char * username, char * password);
	struct rest_response  * (*get)(void * i,struct rest_client * cptr, struct rest_request * rptr);
	struct rest_response  * (*post)(void * i,struct rest_client * cptr, struct rest_request * rptr);
	struct rest_response  * (*put)(void * i,struct rest_client * cptr, struct rest_request * rptr);
	struct rest_response  * (*delete)(void * i,struct rest_client * cptr, struct rest_request * rptr);
	struct rest_response  * (*head)(void * i,struct rest_client * cptr, struct rest_request * rptr);
	struct rest_response  * (*options)(void * i,struct rest_client * cptr, struct rest_request * rptr);
	struct rest_extension * (*extension)(void * i,struct rest_server * sptr,struct rest_extension * xptr);
	int 			(*before)(void * i,struct rest_client * cptr, struct rest_request * rptr);
	int 			(*after)(void * i,struct rest_client * cptr, struct rest_request * rptr);
	int 			(*security)(void * i,struct rest_client * cptr, struct rest_request * rptr);
	struct rest_response * 	(*transaction)(void * i,struct rest_client * cptr, struct rest_request * rptr, struct rest_response * aptr);
	struct rest_response *  (*alert)(void * i, 
					struct rest_client * cptr, 
					struct rest_response * rptr, 
					char * name,
					int status, char * message, 
					char * nature, 
					char * agent, char * tls);
	struct rest_header *    (*allowed_headers)(void * i, 
						   struct rest_client * cptr, 
						   struct rest_request * rptr, 
						   struct rest_response * aptr);
};

struct	rest_client
{
	struct	rest_client 	* previous;
	struct	rest_client 	* next;
	struct	rest_server 	* parent;	/* used for list management */
	char			* agent;	
	char			* buffer;
	int			buffersize;
	int			bytes;
	int			consumed;
	char			* user;
	char			* pass;
	struct	rest_header 	* headers;
	struct 	connection 	net;
	struct	tls_configuration * tlsconf;
	struct 	rest_server	* server;	/* used for method access */
};

public void dump_rest_client(FILE *h, struct rest_client *cptr);

struct	rest_extension
{
	struct	rest_extension	* previous;
	struct	rest_extension	* next;
	void			* payload;
	int			type;
	struct	connection 	* net;
};

struct	rest_server
{
	int	port;
	char *	name;

	char *	user;
	char *	password;

	int	seconds;
	int	wakeup;
	int	fraction;
	int	workers;
	int	threads;

	struct	rest_header * 		headers;

	struct	rest_client * 		first;
	struct	rest_client * 		last;
	struct	rest_client * 		active;

	struct	rest_extension * 	firstx;
	struct	rest_extension * 	lastx;
	struct	rest_extension * 	activex;

	struct	rest_worker * 		firstw;
	struct	rest_worker * 		lastw;

	struct 	connection 		net;
	struct 	rest_interface 		method;
	struct	tls_configuration * 	tlsconf;

	pthread_mutex_t 		lock;

};


#include "restpublic.h"


#endif	/* _rest_h */
	/* ------- */






