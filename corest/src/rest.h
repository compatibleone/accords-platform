/* ---------------------------------------------------------------------------- */
/* Advanced Capabilities for Compatible One Resources Delivery System - ACCORDS	*/
/* (C) 2011 by Iain James Marshall <ijm667@hotmail.com>				*/
/* ---------------------------------------------------------------------------- */
/*										*/
/* This is free software; you can redistribute it and/or modify it		*/
/* under the terms of the GNU Lesser General Public License as			*/
/* published by the Free Software Foundation; either version 2.1 of		*/
/* the License, or (at your option) any later version.				*/
/*										*/
/* This software is distributed in the hope that it will be useful,		*/
/* but WITHOUT ANY WARRANTY; without even the implied warranty of		*/
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU		*/
/* Lesser General Public License for more details.				*/
/*										*/
/* You should have received a copy of the GNU Lesser General Public		*/
/* License along with this software; if not, write to the Free			*/
/* Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA		*/
/* 02110-1301 USA, or see the FSF site: http://www.fsf.org.			*/
/*										*/
/* ---------------------------------------------------------------------------- */
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
#define	_HTTP_BASIC		"Basic realm=*"

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
	struct rest_extension * (*extension)(void * i,struct rest_server * sptr,struct rest_extension * xptr);
	int 			(*before)(void * i,struct rest_client * cptr, struct rest_request * rptr);
	int 			(*after)(void * i,struct rest_client * cptr, struct rest_request * rptr);
	int 			(*security)(void * i,struct rest_client * cptr, struct rest_request * rptr);
	struct rest_response * 	(*transaction)(void * i,struct rest_client * cptr, struct rest_request * rptr, struct rest_response * aptr);
	struct rest_response *  (*alert)(void * i, 
					struct rest_client * cptr, 
					struct rest_response * rptr, 
					int status, char * message, 
					char * nature, 
					char * agent, char * tls);

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






