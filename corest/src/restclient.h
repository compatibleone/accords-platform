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
#ifndef	_restclient_h
#define	_restclient_h

/*	------	*/
/*	HEADER 	*/
/*	------	*/
public struct rest_header * 
	rest_client_header( 
		struct rest_header * hptr, char * n, char * v );

/*	---	*/
/*	GET	*/
/*	---	*/
public	struct	rest_response * 
	rest_client_get_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr );

/*	---	*/
/*	TRY	*/
/*	---	*/
public	struct	rest_response * 
	rest_client_try_get_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr, int timeout, int retry );

/*	------	*/
/*	DELETE	*/
/*	------	*/
public	struct	rest_response * 
	rest_client_delete_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr );

/*	----	*/
/*	HEAD	*/
/*	----	*/
public	struct	rest_response * 
	rest_client_head_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr );

/*	----	*/
/*	POST	*/
/*	----	*/
public	struct	rest_response * 
	rest_client_post_request(
		char * target, char * tls, char * nptr, char * filename, struct rest_header * hptr );

/*	---	*/
/*	PUT	*/
/*	---	*/
public	struct	rest_response * 
	rest_client_put_request(
		char * target, char * tls, char * nptr, char * filename, struct rest_header * hptr );

#endif	/* _restclient_h */
	/* ------------- */

