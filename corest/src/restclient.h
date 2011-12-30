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

