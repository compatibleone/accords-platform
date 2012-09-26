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




