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
#ifndef	_conetsengine_c
#define	_conetsengine_c


#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"
#include "occiresolver.h"
#include "conetsengine.h"

/*	-------------------------------------------	*/
/* 	      i s _ i p _ a v a i l a b l e  		*/
/*	-------------------------------------------	*/


public int is_ip_available(char *ip, char * agent, char * tls, char * version)
{

      	struct	occi_element 	*	eptr=(struct occi_element*) 0;
	struct	occi_element 	*	eptr2=(struct occi_element*) 0;
	struct	occi_element 	*	eptr3=(struct occi_element*) 0;
	struct	occi_element 	*	eptr4=(struct occi_element*) 0;
	struct	occi_client  	*	cptr=(struct occi_client*) 0;
	struct	occi_response	*	aptr=(struct occi_response*) 0;
	struct	occi_response	*	aaptr=(struct occi_response*) 0;
	struct	occi_response	*	aaptr2=(struct occi_response*) 0;
	char	buffer[4096];
        char	buffer2[4096];


	if ( check_verbose() )
		printf("   OCCI Conets ( %s, %s )\n","ipaddress",agent);

	initialise_occi_resolver( _DEFAULT_PUBLISHER , (char *) 0, (char *) 0, (char *) 0 );


	if (!( aptr = occi_resolver("ipaddress" , agent ) ))
	{ 
                 aptr = occi_remove_response( aptr );
 		 return(0);
        }
	
	for (	eptr =aptr->first;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;

		sprintf(buffer,"%s/%s/",eptr->value,"ipaddress");
		if (!( aaptr = occi_simple_get( buffer, (char *)agent, (char *)tls ) ) )
		{ 
                 aaptr = occi_remove_response( aaptr );
 		 continue;	
	        }
                for (	eptr2 =aaptr->first;
		eptr2 != (struct occi_element*) 0;
		eptr2 = eptr2->next )
		{
		 rest_add_http_prefix(buffer2,4096,eptr2->value);
                 if (!( aaptr2 = occi_simple_get( buffer2, (char *)agent, (char *)tls ) ) )
		 { 
                  aaptr2 = occi_remove_response( aaptr2 );
 		  continue;	
	         }

		 if (    (   (eptr3 = occi_locate_element(aaptr2->first, "occi.ipaddress.value")))  
                      && (   (eptr4 = occi_locate_element(aaptr2->first, "occi.ipaddress.version" ))) 
	              && (  !(strcmp( eptr3->value, ip))) 
		      && (  !(strcmp(eptr4->value, version ))) )	 
                  return(0);
                 else continue;          
                 
                }
         }               
	aaptr2 = occi_remove_response( aaptr2 );
        aaptr = occi_remove_response( aaptr );
	aptr = occi_remove_response( aaptr );
	cptr = occi_remove_client( cptr );
	return(1) ;	
  
}

/*	-------------------------------------------	*/
/* 	      m y  _ a t o i _ h e x a  		*/
/*	-------------------------------------------	*/

unsigned int my_atoi_hexa(char **nbr)
{
  unsigned int	ret = 0;

  printf("my_atoi_hexa B '%s'\n", *nbr);

  while (**nbr != '\0')
  {
    switch (**nbr)
    {
      case '0' :
      case '1' :
      case '2' :
      case '3' :
      case '4' :
      case '5' :
      case '6' :
      case '7' :
      case '8' :
      case '9' :
        ret = ret * 16 + (**nbr - '0' + 0);
        break;
      case 'a' :
      case 'b' :
      case 'c' :
      case 'd' :
      case 'e' :
      case 'f' :
        ret = ret * 16 + (**nbr - 'a' + 10);
        break;
      case 'A' :
      case 'B' :
      case 'C' :
      case 'D' :
      case 'E' :
      case 'F' :
        ret = ret * 16 + (**nbr - 'A' + 10);
        break;
      default:
        printf("my_atoi_hexa RET '%x'\n", ret);
        printf("my_atoi_hexa A '%s'\n", *nbr);
        return (ret);
    }
    ++(*nbr);
  }

  printf("my_atoi_hexa RET '%d'\n", ret);
  printf("my_atoi_hexa A '%s'\n", *nbr);

  return (ret);
}



/*	-------------------------------------------	*/
/* 	      g e t _ i p v 6 _ f r o m _ r a n g e     */
/*	-------------------------------------------	*/


public char * get_ipv6_from_range(char *range, char *agent, char *tls)
{

 

  char *tmp = (char *)range, *ret = NULL;
  unsigned int ipv6[4] = {0}, cidr = 0;
  int i, j, colon_nbr = 0, consec_colon = 0;


  /*
   * count ':' and '::'
   */
  for (i = 0 ; tmp[i] != '\0' ; ++i)
    if (tmp[i] == ':')
    {
      if (tmp[i + 1] == ':')
        ++consec_colon;
      else
        ++colon_nbr;
    }

  if (!(colon_nbr == MAX_COLON_NBR_V6
        || (colon_nbr <= MAX_COLON_NBR_V6 && consec_colon == 1)))
    return (NULL);

  /*
   * parse
   */

  for (j = 0 ; j <= 8 && *tmp != '\0' ; ++j)
  {

    if (*tmp == '/')
    {
      cidr = atoi(++tmp);
      break;
    }

    ipv6[j / 2] |= my_atoi_hexa(&tmp) & MAX_IP_ELEMENT_VALUE_V6;

    if (j % 2 == 0)
      ipv6[j / 2] <<= 16;

    if (*tmp == ':')
    {
      ++tmp;
      if (*tmp == ':')
      {
        ++tmp;
        j += MAX_COLON_NBR_V6 - colon_nbr;
      }
    }

  }

  /***********************************/
  /* try to return a valid ip adress */
  /***********************************/

  unsigned int ref[4] = {0}, test[4];


  i = 0;
  while (i < (int)cidr)
  {
    ref[i / 32] |= ipv6[i / 32] & (1 << (32 - (i % 32)));
    ++i;
  }


  test[0] = ref[0];
  test[1] = ref[1];
  test[2] = ref[2];
  test[3] = ref[3];
  while ((ref[0] & test[0]) == ref[0] &&
      (ref[1] & test[1]) == ref[1] &&
      (ref[2] & test[2]) == ref[2] &&
      (ref[3] & test[3]) == ref[3])
  {
    if (asprintf(&ret, "%x:%x:%x:%x:%x:%x:%x:%x",
          (test[0] & (MAX_IP_ELEMENT_VALUE_V6 << 16)) >> 16,
          test[0] & (MAX_IP_ELEMENT_VALUE_V6),
          (test[1] & (MAX_IP_ELEMENT_VALUE_V6 << 16)) >> 16,
          test[1] & (MAX_IP_ELEMENT_VALUE_V6),
          (test[2] & (MAX_IP_ELEMENT_VALUE_V6 << 16)) >> 16,
          test[2] & (MAX_IP_ELEMENT_VALUE_V6),
          (test[3] & (MAX_IP_ELEMENT_VALUE_V6 << 16)) >> 16,
          test[3] & (MAX_IP_ELEMENT_VALUE_V6)) == -1)
      return (NULL);

    if (is_ip_available(ret, agent, tls, "v6"))
      return (ret);
    free(ret);

    ++(test[3]);
    if (test[3] == 0)
    {
      ++(test[2]);
      if (test[2] == 0)
      {
        ++(test[1]);
        if (test[1] == 0)
          ++(test[0]);
      }
    }
  }

  /* no ip adress found, return NULL */
  return (NULL);
}



/*	-------------------------------------------	*/
/* 	     g e t _ i p v 4 _ f r o m _ r a n g e	*/
/*	-------------------------------------------	*/

public char * get_ipv4_from_range(char * range, char * agent, char * tls  )
{

 
  char *tmp = NULL, *ret = NULL;
  unsigned int ip = 0, cidr = 0;

  // ip 

  if ((tmp = (char *)range) == NULL)
    return (NULL);

  ip = atoi(tmp);
  if (ip == 0 || ip > MAX_IP_ELEMENT_VALUE_V4)
    return (NULL);
  ip <<= 8;

  if ((tmp = index(tmp, (int)'.')) == NULL)
    return (NULL);
  if (((ip |= atoi(++tmp)) & 255) > MAX_IP_ELEMENT_VALUE_V4)
    return (NULL);
  ip <<= 8;

  if ((tmp = index(tmp, (int)'.')) == NULL)
    return (NULL);
  if (((ip |= atoi(++tmp)) & 255) > MAX_IP_ELEMENT_VALUE_V4)
    return (NULL);
  ip <<= 8;

  if ((tmp = index(tmp, (int)'.')) == NULL)
    return (NULL);
  if (((ip |= atoi(++tmp)) & 255) > MAX_IP_ELEMENT_VALUE_V4)
    return (NULL);

  if ((tmp = index(tmp, (int)'/')) == NULL)
    return (NULL);

  // cidr 

  ++tmp;
  if (!strisdigit(tmp))
    return (NULL);
  cidr = atoi(tmp);
  if (cidr < MIN_CIDR_VALUE_V4 || cidr > MAX_CIDR_VALUE_V4)
    return (NULL);

 // try to return a valid IP address 

  unsigned int ref = 0, i = 0, j;

  while (i++ < cidr)
    ref |= ip & (1 << (MAX_CIDR_VALUE_V4 - i ));

  j = ref;
  while ((ref & j) == ref)
  {
    if (asprintf(&ret, "%d.%d.%d.%d",
        (j & (255 << 24)) >> 24,
        (j & (255 << 16)) >> 16,
        (j & (255 <<  8)) >>  8,
        (j & (255 <<  0)) >>  0) == -1)
      return (NULL);

    if (is_ip_available(ret, agent, tls, "v4"))
      return (ret);
    free(ret);

    ++j;
    if ((j & (255 <<  0)) == (255 <<  0))
      j += 1 <<  0;
    if ((j & (255 <<  8)) == (255 <<  8))
      j += 1 <<  8;
    if ((j & (255 << 16)) == (255 << 16))
      j += 1 << 16;
    if ((j & (255 << 24)) == (255 << 24))
      j += 1 << 24;
  }

  // no IP adress found, return NULL 

  return (NULL); 
 
}


/*	-------------------------------------------	*/
/* 	  r e t r i e v e _ d o m a i n 	 	*/
/*	-------------------------------------------	*/


public char * retrieve_domain(char * attribval, char * agent, char * tls)
{
      
   	struct	occi_element 	*	eptr=(struct occi_element*) 0;
	struct	occi_element 	*	eptr2=(struct occi_element*) 0;
	struct	occi_element 	*	eptr3=(struct occi_element*) 0;
	struct	occi_response	*	aptr=(struct occi_response*) 0;
	struct	occi_response	*	aaptr=(struct occi_response*) 0;
	struct	occi_response	*	aaptr2=(struct occi_response*) 0;
	char	buffer[4096];
        char	buffer2[4096];
        char    attribuff[1024];   
   
	if ( check_verbose() )
		printf("   OCCI Conets ( %s, %s )\n","domain",agent);

	initialise_occi_resolver( _DEFAULT_PUBLISHER , (char *) 0, (char *) 0, (char *) 0 );


	if (!( aptr = occi_resolver("domain" , agent ) ))
	{ 
                 aptr = occi_remove_response( aptr );
 		 return((char *)0);
        }
	
	for (	eptr =aptr->first;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;

		sprintf(buffer,"%s/%s/",eptr->value,"domain");
		if (!( aaptr = occi_simple_get( buffer, agent, tls ) ) )
		{ 
                 aaptr = occi_remove_response( aaptr );
 		 continue;	
	        }
                for (	eptr2 =aaptr->first;
		eptr2 != (struct occi_element*) 0;
		eptr2 = eptr2->next )
		{
			rest_add_http_prefix(buffer2,4096,eptr2->value);
                	if (!( aaptr2 = occi_simple_get( buffer2, agent, tls ) ) )
			{ 
                		aaptr2 = occi_remove_response( aaptr2 );
 				continue;	
	        	}

			sprintf(attribuff,"%s%s", "occi.","domain.name");

 
			if (    (eptr3 = occi_locate_element(aaptr2->first, attribuff) ) 
        	            &&  ( ! (strcmp(eptr3->value, attribval) ) ) ) 
                	  return(eptr3->value);
                	 else continue;          
                 
                }
         }               
	aaptr2 = occi_remove_response( aaptr2 );
        aaptr = occi_remove_response( aaptr );
	aptr = occi_remove_response( aaptr );
        return ( (char*) 0);
}

/*	-------------------------------------------	*/
/* 	  r e t r i e v e _ i p r a n g e 	 	*/
/*	-------------------------------------------	*/


public char * retrieve_iprange(char * version, char * type, char * agent, char * tls)
{
      
   	struct	occi_element 	*	eptr=(struct occi_element*) 0;
	struct	occi_element 	*	eptr2=(struct occi_element*) 0;
	struct	occi_element 	*	eptr3=(struct occi_element*) 0;
	struct	occi_element 	*	eptr4=(struct occi_element*) 0;
	struct	occi_element 	*	eptr5=(struct occi_element*) 0;
	struct	occi_response	*	aptr=(struct occi_response*) 0;
	struct	occi_response	*	aaptr=(struct occi_response*) 0;
	struct	occi_response	*	aaptr2=(struct occi_response*) 0;
	char	buffer[4096];
        char	buffer2[4096];
        char    versionbuff[1024];   
        char    typebuff[1024];   
        char    rangebuff[1024];      

	if ( check_verbose() )
		printf("   OCCI Conets ( %s, %s )\n","iprange",agent);

	initialise_occi_resolver( _DEFAULT_PUBLISHER, (char *) 0, (char *) 0, (char *) 0 );


	if (!( aptr = occi_resolver("iprange" , agent ) ))
	{ 
                 aptr = occi_remove_response( aptr );
 		 return((char *)0);
        }
	
	for (	eptr =aptr->first;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;

		sprintf(buffer,"%s/%s/",eptr->value,"iprange");
		if (!( aaptr = occi_simple_get( buffer, agent, tls ) ) )
		{ 
                 aaptr = occi_remove_response( aaptr );
 		 continue;	
	        }
                for (	eptr2 =aaptr->first;
		eptr2 != (struct occi_element*) 0;
		eptr2 = eptr2->next )
		{
		rest_add_http_prefix(buffer2,4096,eptr2->value);
                if (!( aaptr2 = occi_simple_get( buffer2, agent, tls ) ) )
		{ 
                 aaptr2 = occi_remove_response( aaptr2 );
 		 continue;	
	        }

		sprintf(versionbuff,"%s%s", "occi.","iprange.version");
		sprintf(typebuff,"%s%s", "occi.","iprange.type");
		sprintf(rangebuff,"%s%s", "occi.","iprange.value");
 

		if (    (eptr3 = occi_locate_element(aaptr2->first, versionbuff) )
		    &&  (eptr4 = occi_locate_element(aaptr2->first, typebuff) ) 
                    &&  (eptr5 = occi_locate_element(aaptr2->first, rangebuff) ) 
		    &&  ( ! (strcmp(eptr3->value, version) ) ) 
                    &&  ( ! (strcmp(eptr4->value, type   ) ) ) 
	            &&  ( eptr5->value)    )
		 return(eptr5->value);
                 else continue;          
                 
                }
         }               
	aaptr2 = occi_remove_response( aaptr2 );
        aaptr = occi_remove_response( aaptr );
	aptr = occi_remove_response( aaptr );
        return ( (char*) 0);
}



#endif /*_conetsengine_c*/

