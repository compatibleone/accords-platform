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
#ifndef	_conetsengine_h
#define	_conetsengine_h


#define	_DEFAULT_VERSION "v4"
#define	_DEFAULT_TYPE "private"
#define _DEFAULT_DOMAIN "default_domain"


#define MIN_CIDR_VALUE_V4 (0)
#define MAX_CIDR_VALUE_V4 (32)
#define MAX_IP_ELEMENT_VALUE_V4 (254)

#define MIN_CIDR_VALUE_V6 (0)
#define MAX_CIDR_VALUE_V6 (128)
#define MAX_IP_ELEMENT_VALUE_V6 (0xffff)
#define MAX_COLON_NBR_V6 (7)

#define strisdigit(v) (strspn(v, "0123456789") == strlen(v))

public int is_ip_available(char *ip, char * agent, char * tls, char * version);
public char * retrieve_domain(char * attribval, char * agent, char * tls);
private unsigned int my_atoi_hexa(char **nbr);
public char * get_ipv6_from_range(char *range, char *agent, char *tls);
public char * get_ipv4_from_range(char * range, char * agent, char * tls );
public char * retrieve_iprange(char * version, char * type, char * agent, char * tls);
public struct occi_response * create_new_domain(char * domain, char * agent, char * tls);


/* ----------------------------------------------------------------------------------------------*/


	/* --------------- */
#endif	/* _conetsengine_h */
	/* --------------- */




