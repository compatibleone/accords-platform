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
#ifndef	_rest_thread_h
#define	_rest_thread_h

struct	rest_thread
{
	struct	rest_thread * 	previous;
	struct	rest_thread * 	next;
	struct 	rest_client *	client;
	struct 	rest_request *	request;
	int			status;
	int			started;
	int			item;
	pthread_mutex_t 	controlOne;
	pthread_mutex_t 	controlZero;
	pthread_mutex_t 	lock;
	pthread_t		id;
	pthread_attr_t		attributes;
};

#endif	/* _rest_thread_h */
	/* -------------- */




