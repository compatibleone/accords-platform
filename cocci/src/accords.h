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
#ifndef	_accords_h
#define	_accords_h

struct	accords_configuration
{
	int	monitor;		/* monitoring information options	*/
	int	autopub;		/* auto publication mechanisms		*/
	int	verbose;		/* activate verbose messages		*/
	int	debug;			/* activate debug messages		*/
	int	threads;		/* allow use of threads			*/

	char *  tls;			/* security configuration filename 	*/
	char *	identity;		/* identity for publication of service	*/
	char *	publisher;		/* identity of publication service	*/
	char *	operator;		/* service provider operator identity	*/
	char *	user;			/* service user name 			*/
	char *	password;		/* authentication code			*/
	char *	resthost;		/* root name of host			*/
	int	restport;		/* server tcp port			*/
	char *	chathost;		/* xmpp chat host url			*/
	int	chatport;		/* xmpp chat host prot			*/
	char *	domain;			/* category domain			*/
	char *	config;			/* configuration file			*/

	struct	occi_category *	firstcat;
	struct	occi_category *	lastcat;

};

public	void	load_accords_configuration( struct accords_configuration * configuration, char * section );
public	void	accords_configuration_options();
public	int	accords_configuration_option( char * aptr, int argi, char * argv[] );

#endif	/* _accords_h */
	/* ---------- */


