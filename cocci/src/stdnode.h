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
#ifndef	_stdnode_h
#define	_stdnode_h

struct	standard_message
{
	char *	id;
	struct	occi_response * message;
};

struct	standard_node
{
	struct	standard_message node;
	struct	standard_message infrastructure;
	struct	standard_message compute;
	struct	standard_message network;
	struct	standard_message storage;
	struct	standard_message image;
	struct	standard_message system;
	struct	standard_message package;
	struct	standard_message firewall;
	struct	standard_message port;
};

public	void			release_standard_message( struct standard_message * mptr );
public	int			get_standard_message( struct standard_message * mptr, char * id, char * agent, char * tls );
public	struct 	occi_element * 	first_standard_message_link( struct occi_response * zptr );
public	struct 	occi_element * 	next_standard_message_link( struct occi_element * eptr );
public	char *	standard_message_link_value( char * sptr );
public	struct	standard_node * liberate_standard_node( struct	standard_node *  nptr );
public	struct	standard_node * allocate_standard_node();
public	struct	standard_node *	get_standard_node( char * id, char * agent, char * tls, int * result );
public	struct	standard_node *	drop_standard_node( struct standard_node *  nptr );

#endif	/* _stdnode_h */
	/* ---------- */

