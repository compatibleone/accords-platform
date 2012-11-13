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
#ifndef	_accords_h
#define	_accords_h

struct	accords_configuration
{
	int	monitor;		/* monitoring information options	*/
	int	autopub;		/* auto publication mechanisms		*/
	int	verbose;		/* activate verbose messages		*/
	int	debug;			/* activate debug messages		*/
	int	threads;		/* allow use of threads			*/
	int	ipv6;			/* require use of IP V6			*/

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
	char *	zone;			/* location of host			*/
	char *	storage;		/* the data storage table or file name 	*/

	struct	occi_category *	firstcat;
	struct	occi_category *	lastcat;

};

public	void	load_accords_configuration( struct accords_configuration * configuration, char * section );
public	void	accords_configuration_options();
public	int	accords_configuration_option( char * aptr, int argi, char * argv[] );

#endif	/* _accords_h */
	/* ---------- */


