/* ------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                   */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>    */
/* --------------------------------------------------------------------*/
/*  This is free software; you can redistribute it and/or modify it    */
/*  under the terms of the GNU Lesser General Public License as        */
/*  published by the Free Software Foundation; either version 2.1 of   */
/*  the License, or (at your option) any later version.                */
/*                                                                     */
/*  This software is distributed in the hope that it will be useful,   */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of     */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   */
/*  Lesser General Public License for more details.                    */
/*                                                                     */
/*  You should have received a copy of the GNU Lesser General Public   */
/*  License along with this software; if not, write to the Free        */
/*  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA */
/*  02110-1301 USA, or see the FSF site: http://www.fsf.org.           */
/* --------------------------------------------------------------------*/
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

