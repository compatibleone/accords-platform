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
#ifndef	_corcs_thread_h
#define	_corcs_thread_h

struct	corcs_thread
{
	struct	corcs_thread 		* 	previous;
	struct	corcs_thread 		* 	next;
	struct 	corcs_asynch_context 	*	request;
	int			number;
	pid_t			tid;
	int			status;
	int			started;
	int			item;
	pthread_mutex_t 	controlOne;
	pthread_mutex_t 	controlZero;
	pthread_mutex_t 	lock;
	pthread_t		id;
	pthread_attr_t		attributes;
};

public	struct	corcs_thread * allocate_corcs_thread();
public	struct	corcs_thread * liberate_corcs_thread(struct corcs_thread * tptr);


#endif	/* _corcs_thread_h */
	/* -------------- */




