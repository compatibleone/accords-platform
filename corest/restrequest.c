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
#ifndef _restrequest_c_
#define _restrequest_c_

#include "restrequest.h"

/*	------------------------------------------	*/
/*	l i b e r a t e _ r e s t _ r e q u e s t 	*/
/*	------------------------------------------	*/
public struct rest_request * liberate_rest_request(struct rest_request * sptr)
{
	struct rest_header * mptr;
	if ( sptr )
	{
		while ((mptr = sptr->first) != (struct rest_header *) 0)
		{
			sptr->first = mptr->next;
			mptr = liberate_rest_header( mptr );
		}
		if ( sptr->method )
			 sptr->method = liberate(sptr->method);
		if ( sptr->object )
			 sptr->object = liberate(sptr->object);
		if ( sptr->parameters )
			 sptr->parameters = liberate(sptr->parameters);
		if ( sptr->version )
			 sptr->version = liberate(sptr->version);
		if ( sptr->body )
			 sptr->body = liberate(sptr->body);
		if ( sptr->host )
			 sptr->host = liberate(sptr->host);
		sptr = liberate( sptr );
	}
	return((struct rest_request *) 0);

}

/*	------------------------------------	*/
/*	r e s e t _ r e s t _ r e q u e s t 	*/
/*	------------------------------------	*/
public struct rest_request * reset_rest_request(struct rest_request * sptr)
{
	if ( sptr )
	{
		sptr->first = (struct rest_header*) 0;
		sptr->last = (struct rest_header*) 0;
		sptr->method = (char*) 0;
		sptr->object = (char*) 0;
		sptr->parameters = (char*) 0;
		sptr->version = (char*) 0;
		sptr->type =  0;
		sptr->port =  0;
		sptr->body = (char*) 0;
		sptr->host = (char*) 0;
	}
	return(sptr);

}

/*	------------------------------------------	*/
/*	a l l o c a t e _ r e s t _ r e q u e s t 	*/
/*	------------------------------------------	*/
public struct rest_request * allocate_rest_request()
{
	struct rest_request * sptr;
	if (!( sptr = allocate( sizeof( struct rest_request ) ) ))
		return( sptr );
	else	return( reset_rest_request(sptr) );
}

void dump_rest_request(FILE *h, struct rest_request *rptr)
{
	/* rest_request *rptr */
	fprintf(h, "\n=== REST Request Port %d, %s, '%s', '%s'===\n", rptr->port, rptr->method, rptr->object, rptr->parameters);
	struct rest_header * header = rptr->first;
	if (header != NULL) while (header->previous != NULL) header = header->previous;
	while (header != NULL && header != rptr->last)
	{
	  fprintf(h, " %s : %s\n", header->name, header->value);
	  header = header->next;
	}

}
#endif	/* _restrequest_c_ */
